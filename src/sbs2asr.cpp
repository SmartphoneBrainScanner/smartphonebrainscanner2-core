#include "sbs2asr.h"
#include <math.h>


Sbs2Asr::Sbs2Asr(int channels_, int blockSize_, int blockSkip_,
             float threshold_, int calibrationTime_, QObject *parent):
    QObject(parent), channels(channels_), blockSize(blockSize_),
    blockSkip(blockSkip_), calibrationTime(calibrationTime_)
{
    threshold = threshold_ * blockSize;
    numOverlap = blockSize / blockSkip;

    // make sure blockSize / blockSkip is an integer
    if (blockSkip * numOverlap != blockSize)
    {
        qDebug() << "Sbs2Asr Error: Ratio blockSize/blockSkip must be integer!";
        return;
    }

    signalIndex = 0;
    thresholdMultiplier = 5;
    lookAhead = 10;
    eigComputeFreq = blockSize/4;
    currentSamp = blockSize-1-lookAhead; // Sample being cleaned (constant distance from head)

    // Initialize ring buffer for input data
    dataDeque = new DTU::DtuArray2D<double>(blockSize+1, channels);
    (*dataDeque) = 0;
    dataDequeRows = dataDeque->dim1();

    calibrationNumEl = calibrationTime*Sbs2Common::samplingRate();
    calibrationData = new DTU::DtuArray2D<double>(calibrationNumEl, channels);
    (*calibrationData) = 0;

    calibrationDataZeroMean = new DTU::DtuArray2D<double>(calibrationNumEl, channels);
    (*calibrationDataZeroMean) = 0;

    calibrationDataZeroMeanT = new DTU::DtuArray2D<double>(channels, calibrationNumEl);
    (*calibrationDataZeroMeanT) = 0;

    calibrationDataHead = 0;
    dataDequeHead = 0;
    dataDequeCurrent = 0;

    calibrationMean = new DTU::DtuArray2D<double>(1, channels);
    (*calibrationMean) = 0;

    calibrationCov = new DTU::DtuArray2D<double>(channels, channels);
    (*calibrationCov) = 0;

    calibrationCovTransformed = new DTU::DtuArray2D<double>(channels, channels);
    (*calibrationCovTransformed) = 0;

    calibrationCovFinal = new DTU::DtuArray2D<double>(channels, channels);
    (*calibrationCovFinal) = 0;

    calibrationEigenVal = new DTU::DtuArray2D<double>(channels, channels);
    (*calibrationEigenVal) = 0;

    calibrationEigenVec = new DTU::DtuArray2D<double>(channels, channels);
    (*calibrationEigenVec) = 0;
    
    calibrationTransformedData = new DTU::DtuArray2D<double>(channels, channels);
    (*calibrationTransformedData) = 0;

    calibrationTransformedDataDiag = new DTU::DtuArray2D<double>(channels, channels);
    (*calibrationTransformedDataDiag) = 0;

    thresholdFinal = new DTU::DtuArray2D<double>(1, channels);
    (*thresholdFinal) = threshold;

    mean = new DTU::DtuArray2D<double>(1, channels);
    (*mean) = 0;

    cov = new DTU::DtuArray2D<double>(channels, channels);
    (*cov) = 0;

    covFinal = new DTU::DtuArray2D<double>(channels, channels);
    (*covFinal) = 0;

    eigen_val = new DTU::DtuArray2D<double>(channels, channels);
    (*eigen_val) = 0;

    eigen_vec = new DTU::DtuArray2D<double>(channels, channels);
    (*eigen_vec) = 0;

    eigen_vecT = new DTU::DtuArray2D<double>(channels, channels);
    (*eigen_vecT) = 0;

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

    //printValues = 0;

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
        if ((calibrationDataHead % Sbs2Common::samplingRate()) == 0)
            qDebug() << "Performing calibration.. total time:" << calibrationTime << "seconds. time left: " << calibrationTime-calibrationDataHead/Sbs2Common::samplingRate();

        this->doCalibration(values);
        // Bypass processing: Just copy input values to output
        for (int k=0; k<channels; k++)
            (*returnValues)[0][k] = (*values)[0][k];
    }
    else if(action==2)
    {
        // do PCA with calibrated threshold
        //qDebug() << "Filter started.";
        this->doPca(values, returnValues, thresholdFinal);
    }
    else if(action==3)
    {
        // do PCA with fixed threshold
        this->doPca(values, returnValues, thresholdFinal);
    }
    else
    {
        // Bypass processing: Just copy input values to output
        for (int k=0; k<channels; k++)
            (*returnValues)[0][k] = (*values)[0][k];

        // Reset calibration
        if (calibrationDataHead != 0)
            calibrationDataHead = 0;
    }

//    if ((printValues+64)%128==0) {
//        for (int i = 0; i < channels; i++)
//            qDebug() << "return values: " << (*returnValues)[0][i] << "input values: " << (*values)[0][i];
//    }
//    printValues++;
}

void Sbs2Asr::doCalibration(DTU::DtuArray2D<double>* values) {

    // Collect calibration data
    for (int k=0; k < channels; k++) {
        (*calibrationData)[calibrationDataHead][k] = (*values)[0][k];
    }
    calibrationDataHead++;

    // compute calibration covariance matrix
    if (calibrationDataHead==calibrationNumEl) {

        // mean
        for (int k=0; k < channels; k++) {
            double sumTemp = 0;
            for (int i=0; i<calibrationNumEl; i++) {
                sumTemp += (*calibrationData)[i][k];
            }
            (*calibrationMean)[0][k] = sumTemp/calibrationNumEl;
        }

        // subtract mean
        for (int k=0; k < channels; k++) {
            for (int i=0; i<calibrationNumEl; i++) {
                (*calibrationDataZeroMean)[i][k] = (*calibrationData)[i][k]-(*calibrationMean)[0][k];
            }
        }

        // Compute covariance matrix
        calibrationDataZeroMean->transpose(calibrationDataZeroMeanT);
        calibrationDataZeroMeanT->multiply(calibrationDataZeroMean, calibrationCov);
        // normalize
        calibrationCov->multiply(1./calibrationNumEl, calibrationCovFinal);
        calibrationCov=calibrationCovFinal;

//        for (int i = 0; i < channels; i++)
//            qDebug() << "Covariance diag " << i << ": " << (*calibrationCov)[i][i];

        action = 2;
        calibrationDataHead = 0;
    }
}


void Sbs2Asr::doPca(DTU::DtuArray2D<double>* values, DTU::DtuArray2D<double>* returnValues, DTU::DtuArray2D<double>* pcaThreshold) {

    // Insert new data point in ring buffer
    for (int k=0; k < channels; k++) {
        (*dataDeque)[dataDequeHead][k] = (*values)[0][k];
    }

    //qDebug() << "sbs2asr.cpp: Sbs2Asr::doAsr: on = " <<(*dataDeque)[69][0];

    int dataDequeTail = (dataDequeHead+1)%(dataDequeRows);

    // Update mean
    for (int k=0; k < channels; k++)
        (*mean)[0][k] = (*mean)[0][k]
            - (*dataDeque)[dataDequeTail][k] / blockSize
            + (*dataDeque)[dataDequeHead][k] / blockSize;

    // Collect initial look ahead samples
    if ((*dataDeque)[lookAhead][0] == 0) {
        dataDequeHead = (dataDequeHead+1) % (dataDequeRows);
        return;
    }

    // Remove mean from data
    for (int j=0; j < blockSize; j++)
    {
        int i = (dataDequeTail + j + 1) % (blockSize + 1); // runs through elements of current block
        for (int k=0; k < channels; k++)
        {
            (*inputDataZeroMean)[j][k] = (*dataDeque)[i][k] - (*mean)[0][k];
        }
    }
    //qDebug() << "sbs2asr.cpp: Sbs2Asr::doAsr: on, mean = " << (*mean)[0][0];

    // Only recompute eigen decomposition every "eigComputeFreq" sample
    if ((((dataDequeHead)%blockSize)%eigComputeFreq) == 0) {
        //qDebug() << "Freq: " << eigComputeFreq << " current: " << (dataDequeHead%dataDequeRows)+1;
        inputDataZeroMean->transpose(inputDataZeroMeanT);
        inputDataZeroMeanT->multiply(inputDataZeroMean, cov);
        // normalize
        cov->multiply(1./blockSize,covFinal);

        // Eigenvectors decomposition
        covFinal->getEig(eigen_vec, eigen_val);  // Timing: 200 microsec

        eigen_vec->transpose(eigen_vecT);
        calibrationCov->multiply(eigen_vec,calibrationCovTransformed);
        eigen_vecT->multiply(calibrationCovTransformed,calibrationCovFinal);

        for (int i = 0; i<channels; i++) {
            (*pcaThreshold)[0][i] = thresholdMultiplier*sqrt((*calibrationCovFinal)[i][i]);
            //qDebug() << "Threshold " << i+1 << ": " << (*pcaThreshold)[0][i] << "with eigen-value " << sqrt((*eigen_val)[i][i]);
        }
    }

    // Transform to PC-space
    inputDataZeroMean->multiply(eigen_vec, transformedData);  // Timing: 100 microsec

    for (int ch = 0; ch < channels; ch++)
    {
        // check comp. of eigenvalues
        if (sqrt((*eigen_val)[ch][ch]) > (*pcaThreshold)[0][ch])
        {
            qDebug() << "Dimension" << ch << "with SD" << sqrt((*eigen_val)[ch][ch]) << "is removed";
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

    eigen_vec->transpose_insitu();

    // add mean back
    for (int c = 0; c < channels; c++)
    {
        //for(int r = 0; r < blockSize; r++)
        //{
            (*reconstructedData)[currentSamp][c] = (*reconstructedData)[currentSamp][c] + (*mean)[0][c];
        //}
    }

    for (int k=0; k< channels; k++)
        (*returnValues)[0][k] = (*reconstructedData)[currentSamp][k];

    // Update ringbuffer indices
    dataDequeCurrent = (dataDequeCurrent+1) % dataDequeRows;
    dataDequeHead = (dataDequeHead+1) % dataDequeRows;
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
