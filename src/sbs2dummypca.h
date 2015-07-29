/**
* Smartphone Brain Scanner 2 License Agreement (MIT License)
*
* Copyright (c) 2012 Arkadiusz Stopczynski, Jakob Eg Larsen, Carsten Stahlhut, Michael Kai Petersen, Lars Kai Hansen.
* Technical University of Denmark, DTU Informatics, Cognitive Systems Section. http://code.google.com/p/smartphonebrainscanner2
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
* modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom
* the Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
* LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef SBS2DUMMYPCA_H
#define SBS2DUMMYPCA_H


#include <QObject>
#include <QFile>
#include <QStringList>

#include "sbs2common.h"
#include "dtu_array_2d.h"


/**
    @class Sbs2DummyPca
    \brief Principal component temporal filter.

   An instance should be constructed in the start of the program and samples
   feed to the doPca method. The user of the class should allocate input and
   output arrays. The PCA processing can be turned on with turnOn. When turned
   on the input values are copied to the output with no further processing.

 */

class Sbs2DummyPca : public QObject
{
    Q_OBJECT

public:
    /** Contructor
     *
     * @param channels_ Number of channels, e.g., electrodes
     * @param blockSize_ The number of samples the PCA should work on at each step
     * @param blockskip_ The number of samples to skip when doing the PCA
     * @param threshold_ The threshold that cuts PCA components
     *
     * @return object
     */
    Sbs2DummyPca(int channels_, int blockSize_ = 64, int blockSkip_ = 1, float threshold_ = 12000, QObject *parent = 0);
    ~Sbs2DummyPca();

    /// Do the actual processing
    void doPca(DTU::DtuArray2D<double>* values, DTU::DtuArray2D<double>* returnValues);

    /// Turn processing in PCA filter on
    void turnOn() { on = true; }

    /// Bypass processing in PCA filter
    void turnOff() { on = false; }

    /// Test if filter is on
    bool isOn() { return on; }

private:
     int channels; // number of channels
     int blockSize;
     int blockSkip;
     float threshold;
     bool on;

     int numOverlap; // number of overlapping block, i.e. how many blocks contribute to reconstruction of a given signal segment
     int signalIndex; // index for signal circular buffer.

     DTU::DtuArray2D<double>* dataDeque;
     int dataDequeStart;
     int dataDequeEnd;

     DTU::DtuArray2D<double>* mean;
     DTU::DtuArray2D<double>* inputDataZeroMean;
     DTU::DtuArray2D<double>* inputDataZeroMeanT;

     DTU::DtuArray2D<double>* cov; // covariance matrix
     DTU::DtuArray2D<double>* eigen_val; // eigen values
     DTU::DtuArray2D<double>* eigen_vec; // eigen vectors

     DTU::DtuArray2D<double>* transformedData;

     DTU::DtuArray2D<double>* averageData; // circular buffer holding the last "numOverlap" blocks
     double* averageOffsets; // contains indices for reconstruction

     DTU::DtuArray2D<double>* reconstructedData; // reconstructed block

signals:
    
public slots:
};

#endif // SBS2DUMMYPCA_H


