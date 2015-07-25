#include "sbs2pca.h"

Sbs2Pca* Sbs2Pca::m_pInstance = 0;

// Private constructor
Sbs2Pca::Sbs2Pca(int channels_, int blockSize_, int blockSkip_, int threshold_, QObject *parent):
    QObject(parent), channels(channels_), blockSize(blockSize_), blockSkip(blockSkip_)
{
    initialized = 0;


    threshold = threshold_*blockSize;

    mean = new double[channels];
    for(int c = 0; c < channels; c++)
        mean[c] = 0;

    numOverlap = blockSize/blockSkip;

    // make sure blockSize/blockSkip is an integer
    if(blockSkip*numOverlap != blockSize)
    {
        qDebug() << "Sbs2Pca Error: Ratio blockSize/blockSkip must be integer!";
        return;
    }


    signalIndex = 0;

    cov = new DTU::DtuArray2D<double>(channels, channels);
    (*cov) = 0;

    eigen_val = new DTU::DtuArray2D<double>(channels, channels);
    (*eigen_val) = 0;

    eigen_vec = new DTU::DtuArray2D<double>(channels, channels);
    (*eigen_vec) = 0;

    inputData = new DTU::DtuArray2D<double>(blockSize, channels);
    (*inputData) = 0;

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

    initialized = 1;

    //qDebug() << "PCA Channels = " << channels << ", blocksize = " << blockSize << ", blockskip = " << blockSkip << " and threshold = " << threshold;
}


void Sbs2Pca::newData(DTU::DtuArray2D<double>* data)
{
    if(!initialized)
    {
        qDebug() << "PCA: Something is wrong in initialization";
        return;
    }

    for(int c = 0; c < channels; c++)
    {
        // shift old data back
        for(int r = blockSize-1; r >= blockSkip; r--)
        {
            (*inputData)[r][c] = (*inputData)[r-blockSkip][c];
        }

        // store new data
        for(int r = 0; r < blockSkip; r++)
        {
            (*inputData)[r][c] = (*data)[r][c];
        }
    }

}

// do actual pca stuff
void Sbs2Pca::doPca(DTU::DtuArray2D<double>* returnValues)
{
    if(!initialized)
    {
        qDebug() << "PCA: Something is wrong in initialization";
        return;
    }

    for(int c = 0; c < channels; c++)
    {
        // compute mean
        mean[c] = 0;
        for(int r = 0; r < blockSize;r++)
        {
            mean[c] += (*inputData)[r][c];
        }
        mean[c] /= blockSize;

        // subtract mean
        for(int r = 0; r < blockSize; r++)
        {
            (*inputDataZeroMean)[r][c] = (*inputData)[r][c] - mean[c];
        }
    }

    // compute transpose
    inputDataZeroMean->transpose(inputDataZeroMeanT);

    // compute covariance matrix
    inputDataZeroMeanT->multiply(inputDataZeroMean, cov);

    // Eigenvectors decomposition
    cov->getEig(eigen_vec, eigen_val);

    // Transform to PC-space
    inputDataZeroMean->multiply(eigen_vec, transformedData);

    for(int ch = 0; ch < channels; ch++)
    {
        // check comp. of eigenvalues
        if((*eigen_val)[ch][ch] > threshold)
        {
            //qDebug() << "Channel" << ch << "with variance" << (*eigen_val)[ch][ch]/blockSize << "is removed";
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
    for(int c = 0; c < channels; c++)
    {
        for(int r = 0; r < blockSize; r++)
        {
            (*reconstructedData)[r][c] = (*reconstructedData)[r][c] + mean[c];
        }
    }

    // store
    int rowOffset = signalIndex*blockSize;
    for(int r = 0; r < blockSize; r++)
    {
        for(int c = 0; c < channels; c++)
        {
            (*averageData)[r+rowOffset][c] = (*reconstructedData)[r][c];
        }
    }

    // update average offsets
    for(int i = 0; i < numOverlap; i++)
    {
        int index = (signalIndex-i <0 ? numOverlap+signalIndex-i : signalIndex-i);
        averageOffsets[i] = index*blockSize + (numOverlap-i-1)*blockSkip;
    }

    // average over blocks
    (*returnValues) = 0;
    for(int r = 0; r < blockSkip; r++)
    {
        for(int c = 0; c < channels; c++)
        {
            for(int i = 0; i < numOverlap; i++)
            {
                (*returnValues)[r][c] += (*averageData)[r+averageOffsets[i]][c]/numOverlap;
            }
        }
    }

    // pca done - ready for next block
    signalIndex = (signalIndex+1)%numOverlap;
}

// create or return instance
Sbs2Pca* Sbs2Pca::New(int channels_, int blockSize_, int blockSkip_, int threshold_, QObject *parent)
{
    if (!m_pInstance)
        m_pInstance = new Sbs2Pca(channels_, blockSize_, blockSkip_, threshold_ ,parent);

    return m_pInstance;
}

// Destructor
Sbs2Pca::~Sbs2Pca()
{
    //qDebug() << "PCA destructor()";

    m_pInstance = 0;

    if(mean != 0)
    {
        delete mean;    mean = 0;
    }

    if(cov != 0)
    {
        delete cov;     cov = 0;
    }

    if(eigen_val != 0)
    {
        delete eigen_val;   eigen_val = 0;
    }

    if(eigen_vec != 0)
    {
        delete eigen_val;   eigen_val = 0;
    }

    if(inputData != 0)
    {
        delete inputData;   inputData = 0;
    }

    if(inputDataZeroMean != 0)
    {
        delete inputDataZeroMean;   inputDataZeroMean = 0;
    }

    if(inputDataZeroMeanT != 0)
    {
        delete inputDataZeroMeanT;   inputDataZeroMeanT = 0;
    }

    if(transformedData != 0)
    {
        delete transformedData;   transformedData = 0;
    }

    if(averageData != 0)
    {
        delete averageData;   averageData = 0;
    }

    if(averageOffsets != 0)
    {
        delete averageOffsets;   averageOffsets = 0;
    }

    if(reconstructedData != 0)
    {
        delete reconstructedData;   reconstructedData = 0;
    }
}



