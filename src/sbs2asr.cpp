#include "sbs2asr.h"
#include <math.h>


Sbs2Asr::Sbs2Asr(int channels_, int blockSize_, int blockSkip_,
             float threshold_, QObject *parent):
    QObject(parent), channels(channels_), blockSize(blockSize_),
    blockSkip(blockSkip_)
{
    action = 4;

    threshold = threshold_ * blockSize;
    numOverlap = blockSize / blockSkip;

    // make sure blockSize / blockSkip is an integer
    if (blockSkip * numOverlap != blockSize)
    {
        qDebug() << "Sbs2Asr Error: Ratio blockSize/blockSkip must be integer!";
        return;
    }

    signalIndex = 0;

    // Initialize ring buffer for input data
    dataDeque = new DTU::DtuArray2D<double>(blockSize+1, channels);
    (*dataDeque) = 0;

    numElCalibration = 60*Sbs2Common::samplingRate();
    dataCalibration = new DTU::DtuArray2D<double>(numElCalibration, channels);
    (*dataCalibration) = 0;

    dataCalibrationHead = 0;
    dataDequeHead = 0;

    meanCalibrated = new DTU::DtuArray2D<double>(1, channels);
    (*meanCalibrated) = 0;

    thresholdFinal = new DTU::DtuArray2D<double>(1, channels);
    (*thresholdFinal) = threshold;


    mean = new DTU::DtuArray2D<double>(1, channels);
    (*mean) = 0;

    cov = new DTU::DtuArray2D<double>(channels, channels);
    (*cov) = 0;

    eigen_val = new DTU::DtuArray2D<double>(channels, channels);
    (*eigen_val) = 0;

    eigen_vec = new DTU::DtuArray2D<double>(channels, channels);
    (*eigen_vec) = 0;

    inputDataZeroMean = new DTU::DtuArray2D<double>(blockSize, channels);
    (*inputDataZeroMean) = 0;

    inputDataZeroMeanT = new DTU::DtuArray2D<double>(channels, blockSize);
    (*inputDataZeroMeanT) = 0;

    transformedData = new DTU::DtuArray2D<double>(blockSize, channels);
    (*transformedData) = 0;

    averageData = new DTU::DtuArray2D<double>(numOverlap*blockSize, channels);
    (*averageData) = 0;

    averageOffsets = new double[numOverlap];

    reconstructedData = new DTU::DtuArray2D<double>(blockSize, channels);
    (*reconstructedData) = 0;

    qDebug() << "sbs2asr.cpp: Sbs2Asr::Sbs2Asr: on =" << action << ", channels =" << channels;
}


void Sbs2Asr::doAsr(DTU::DtuArray2D<double>* values, DTU::DtuArray2D<double>* returnValues)
{
    assert(values->dim1() == 1);
    assert(returnValues->dim1() == 1);
    assert(values->dim2() == returnValues->dim2());

    if (action==1)
    {
        // collect data and find threshold
        qDebug() << "Calibration Started.";
        this->doCalibration(values);
    }
    else if(action==2)
    {
        // do PCA with calibrated threshold
        qDebug() << "Filter started.";
        this->doPca(values, returnValues, thresholdFinal, meanCalibrated);
    }
    else if(action==3)
    {
        // do PCA with fixed threshold
        this->doPca(values, returnValues, thresholdFinal, mean);
    }
    else
    {
        // Bypass processing: Just copy input values to output
        for (int k=0; k<channels; k++)
            (*returnValues)[0][k] = (*values)[0][k];
    }

}

void Sbs2Asr::doCalibration(DTU::DtuArray2D<double>* values) {

    // Collect calibration data
    for (int k=0; k < channels; k++) {
        (*dataCalibration)[dataCalibrationHead][k] = (*values)[0][k];
    }
    dataCalibrationHead++;

    // compute mean and threshold (multiplum of STD)
    if (dataCalibrationHead==numElCalibration) {

        // mean
        for (int k=0; k < channels; k++) {
            double sumTemp = 0;
            for (int i=0; i<numElCalibration; i++) {
                sumTemp += (*dataCalibration)[i][k];
            }
            (*meanCalibrated)[0][k] = sumTemp/numElCalibration;
        }

        // threshold pr. channel
        for (int k=0; k < channels; k++) {
            double sumTemp = 0;
            for (int i=0; i<numElCalibration; i++) {
                sumTemp += ((*dataCalibration)[i][k]-(*meanCalibrated)[0][k])*((*dataCalibration)[i][k]-(*meanCalibrated)[0][k]);
            }
            (*thresholdFinal)[0][k] = 3*sqrt(sumTemp/numElCalibration);
        }
        qDebug() << "Calibration completed. Starting filter.";
        action = 2;
    }
}


void Sbs2Asr::doPca(DTU::DtuArray2D<double>* values, DTU::DtuArray2D<double>* returnValues, DTU::DtuArray2D<double>* pcaThreshold, DTU::DtuArray2D<double>* pcaMean) {
    // Insert new data point in ring buffer
    for (int k=0; k < channels; k++) {
        (*dataDeque)[dataDequeHead][k] = (*values)[0][k];
    }

    //qDebug() << "sbs2asr.cpp: Sbs2Asr::doAsr: on = " <<(*dataDeque)[69][0];

    int dataDequeTail = (dataDequeHead+1)%(blockSize+1);

    if ((*pcaThreshold)[0][0]==threshold) { // only update if fixed threshold are used. "fast workaround"
    // Update mean
    for (int k=0; k < channels; k++)
        (*pcaMean)[0][k] = (*pcaMean)[0][k]
            - (*dataDeque)[dataDequeTail][k] / blockSize
            + (*dataDeque)[dataDequeHead][k] / blockSize;
    }

    // Remove mean from data
    for (int j=0; j < blockSize; j++)
    {
        int i = (dataDequeTail + j + 1) % (blockSize + 1);
        for (int k=0; k < channels; k++)
        {
            (*inputDataZeroMean)[j][k] = (*dataDeque)[i][k] - (*pcaMean)[0][k];
        }
    }
    //qDebug() << "sbs2asr.cpp: Sbs2Asr::doAsr: on, mean = " << (*mean)[0][0];

    inputDataZeroMean->transpose(inputDataZeroMeanT);
    inputDataZeroMeanT->multiply(inputDataZeroMean, cov);

    // Eigenvectors decomposition
    cov->getEig(eigen_vec, eigen_val);  // Timing: 200 microsec

    // Transform to PC-space
    inputDataZeroMean->multiply(eigen_vec, transformedData);  // Timing: 100 microsec

    for (int ch = 0; ch < channels; ch++)
    {
        // check comp. of eigenvalues
        if ((*eigen_val)[ch][ch] > (*pcaThreshold)[0][ch])
        {
            // qDebug() << "Channel" << ch << "with variance" << (*eigen_val)[ch][ch]/blockSize << "is removed";
            for(int s = 0; s < blockSize; s++)
            {
                (*transformedData)[s][ch] = 0;
            }
        }
    }

    // transpose yields inverse matrix
    eigen_vec->transpose_insitu();

    // reconstruct
    transformedData->multiply(eigen_vec, reconstructedData);

    // add mean back
    for (int c = 0; c < channels; c++)
    {
        for(int r = 0; r < blockSize; r++)
        {
            (*reconstructedData)[r][c] = (*reconstructedData)[r][c] + (*pcaMean)[0][c];
        }
    }

    for (int k=0; k< channels; k++)
        (*returnValues)[0][k] = (*reconstructedData)[0][k];

    // Update ringbuffer indices
    dataDequeHead = (dataDequeHead+1) % (blockSize+1);
}



Sbs2Asr::~Sbs2Asr()
{
    delete dataDeque;
    delete mean;
    delete cov;
    delete eigen_val;
    delete eigen_vec;
    delete inputDataZeroMean;
    delete inputDataZeroMeanT;
    delete transformedData;
    delete averageData;
    delete averageOffsets;
    delete reconstructedData;
}
