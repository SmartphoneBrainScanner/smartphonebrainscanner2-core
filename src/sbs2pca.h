#ifndef SBS2PCA_H
#define SBS2PCA_H

#include <QObject>
#include <QDebug>

#include "dtu_array_2d.h"

class Sbs2Pca : public QObject
{
    Q_OBJECT
public:
    static Sbs2Pca* New(int channels_, int blockSize_, int blockSkip_, int threshold_, QObject *parent = 0);
    void doPca(DTU::DtuArray2D<double>* returnValues);
    ~Sbs2Pca();

    void newData(DTU::DtuArray2D<double>* data);

private:
    Sbs2Pca(int channels_, int blockSize_, int blockSkip_, int threshold_, QObject *parent = 0);

    // holds object instance
    static Sbs2Pca* m_pInstance;

    int channels; // number of channels
    int blockSize;
    int blockSkip;
    int threshold; // threshold for scaled eigenvalues

    bool initialized; // flag indicating whether initialization was successfull or not

    double* mean; // block mean

    int numOverlap; // number of overlapping block, i.e. how many blocks contribute to reconstruction of a given signal segment
    int signalIndex; // index for signal circular buffer.

    DTU::DtuArray2D<double>* inputData; // input data

    DTU::DtuArray2D<double>* inputDataZeroMean;
    DTU::DtuArray2D<double>* inputDataZeroMeanT; // transpose of inputDataZeroMean

    DTU::DtuArray2D<double>* cov; // covariance matrix
    DTU::DtuArray2D<double>* eigen_val; // eigen values
    DTU::DtuArray2D<double>* eigen_vec; // eigen vectors

    // Transformed data
    DTU::DtuArray2D<double>* transformedData; // data in PC space

    DTU::DtuArray2D<double>* averageData; // circular buffer holding the last "numOverlap" blocks
    double* averageOffsets; // contains indices for reconstruction

    DTU::DtuArray2D<double>* reconstructedData; // reconstructed block
    
signals:
    
public slots:
    
};

#endif // SBS2PCA_H

