#include "sbs2asr.h"


Sbs2Asr::Sbs2Asr(int channels_, int blockSize_, int blockSkip_,
             float threshold_, QObject *parent):
    QObject(parent), channels(channels_), blockSize(blockSize_),
    blockSkip(blockSkip_)
{
    on = false;

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
    dataDeque = new DTU::DtuArray2D<double>(2 * blockSize, channels);
    (*dataDeque) = 0;
    dataDequeStart = 0;
    dataDequeEnd = blockSize;

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

    qDebug() << "sbs2asr.cpp: Sbs2Asr::Sbs2Asr: on =" << on << ", channels =" << channels;
}


void Sbs2Asr::doAsr(DTU::DtuArray2D<double>* values, DTU::DtuArray2D<double>* returnValues)
{
    assert(values->dim1() == 1);
    assert(returnValues->dim1() == 1);
    assert(values->dim2() == returnValues->dim2());

    // Insert new data point in ring buffer
    int dataDequeNewEnd = (dataDequeEnd + 1) % blockSize;
    for (int k=0; k < dataDeque->dim2(); k++)
        (*dataDeque)[dataDequeNewEnd][k] = (*values)[0][k];

    // qDebug() << "sbs2asr.cpp: Sbs2Asr::doAsr: on = " << (*dataDeque)[0][0];

    if (on)
    {

        // Update mean
        for (int k=0; k < mean->dim2(); k++)
            (*mean)[0][k] = (*mean)[0][k]
                - (*dataDeque)[dataDequeStart][k] / blockSize
                + (*dataDeque)[dataDequeNewEnd][k] / blockSize;

        // Remove mean from data
        for (int j=0; j < inputDataZeroMean->dim1(); j++)
        {
            int i = (dataDequeStart + j + 1) % blockSize;
            for (int k=0; k < inputDataZeroMean->dim2(); k++)
            {
                (*inputDataZeroMean)[j][k] = (*dataDeque)[i][k] - (*mean)[0][k];
            }
        }
        inputDataZeroMean->transpose(inputDataZeroMeanT);
        inputDataZeroMeanT->multiply(inputDataZeroMean, cov);

        // Eigenvectors decomposition
        cov->getEig(eigen_vec, eigen_val);  // Timing: 200 microsec

        // Transform to PC-space
        inputDataZeroMean->multiply(eigen_vec, transformedData);  // Timing: 100 microsec

        for (int ch = 0; ch < channels; ch++)
        {
            // check comp. of eigenvalues
            if ((*eigen_val)[ch][ch] > threshold)
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
                (*reconstructedData)[r][c] = (*reconstructedData)[r][c] + (*mean)[0][c];
            }
        }

        // store
        int rowOffset = signalIndex * blockSize;
        for (int r = 0; r < blockSize; r++)
        {
            for(int c = 0; c < channels; c++)
            {
                (*averageData)[r+rowOffset][c] = (*reconstructedData)[r][c];
            }
        }

        // update average offsets
        for (int i = 0; i < numOverlap; i++)
        {
            int index = (signalIndex-i <0 ? numOverlap+signalIndex-i : signalIndex-i);
            averageOffsets[i] = index*blockSize + (numOverlap-i-1)*blockSkip;
        }

        // average over blocks
        (*returnValues) = 0;
        for (int r = 0; r < blockSkip; r++)
        {
            for(int c = 0; c < channels; c++)
            {
                for(int i = 0; i < numOverlap; i++)
                {
                    (*returnValues)[r][c] += (*averageData)[r+averageOffsets[i]][c]/numOverlap;
                }
            }
        }

        // ASR done - ready for next block
        signalIndex = (signalIndex+1) % numOverlap;
    }
    else
    {
        // Bypass processing: Just copy input values to output
        for (int j=0; j<values->dim1(); j++)
            for (int k=0; k<values->dim2(); k++)
               (*returnValues)[j][k] = (*values)[j][k];
    }

    // Update ringbuffer indices
    dataDequeStart = (dataDequeStart + 1) % blockSize;
    dataDequeEnd = dataDequeNewEnd;
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
