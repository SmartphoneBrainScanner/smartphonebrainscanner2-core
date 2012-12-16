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

#ifndef SBS2SOURCERECONSTRUCTIONLORETA_H
#define SBS2SOURCERECONSTRUCTIONLORETA_H

#include <QObject>
#include <dtu_array_2d.h>
#include <sbs2common.h>
#include <QFile>
#include <QStringList>
#include <QDateTime>
#include <QtCore>
#include <utils/sbs2timer.h>
#include <complex>
#include <sbs2spectrogram.h>

#define PI 3.14159265

class Sbs2SourceReconstrucionLoreta : public QObject
{
    Q_OBJECT
public:
    explicit Sbs2SourceReconstrucionLoreta(int channels_, int samples_, int samplesDelta_, int vertices_, QString hardware_, QObject* parent_, int modelUpdateLength_ = 8, int modelUpdateDelta_ = 24);

public:
    enum SumType{MEAN, POWER};

    //DELETME
    int tempModelUpdatedReady;
private:
    void preprocessData();
    void readMNEFixedWeights();
    void weight();
    void reconstruct();
    void calculateMean();
    void calculatePower();
    void collectDataForModelUpdate();
    void doModelUpdate();

    void setupFixedModel();

    void setupModel();
    void updateModel();
    void updateAlpha();
    void updateBeta();
    void updateW();
    void readForwardModel(); //a
    void readPriorSpatialCoherence(); //k
    void readPriorSpatialCoherenceInverse(); //kInv
    void calculateSigma();
    void calculateInputMatrix();

    double window(int n);
    void sourceSpectrogram();

private:

    QString hardware;

    DTU::DtuArray2D<double>* input;
    DTU::DtuArray2D<double>* output;
    int channels; //rows in input
    int samples; //columns in input
    int samplesDelta; //new information in input
    int vertices; //vertices in output
    DTU::DtuArray2D<double>* w; //weights
    DTU::DtuArray2D<double>* weightedInput; // w * input (sss)
    SumType sumType;

    int toModelUpdateValuesIndex;


    DTU::DtuArray2D<double>* toModelUpdateValues;
    DTU::DtuArray2D<double>* currentModelUpdateValues;
    DTU::DtuArray2D<double>* toModelUpdateValuesVertices;
    DTU::DtuArray2D<double>* currentModelUpdateValuesVertices;
    DTU::DtuArray2D<double>* tempModelUpdateValuesVertices; //dummy holder
    DTU::DtuArray2D<double>* tempModelUpdateValues; //dummy holder
    int modelUpdateLength;
    int modelUpdateDelta;
    int modelUpdateDeltaCollected;
    int modelUpdateReady;
    int modelUpdateSamplesSeen;
    DTU::DtuArray2D<double>* a;
    DTU::DtuArray2D<double>* k;
    DTU::DtuArray2D<double>* kInv;
    double invAlpha;
    double invBeta;

    DTU::DtuArray2D<double>* at;
    DTU::DtuArray2D<double>* akat;
    DTU::DtuArray2D<double>* kat;
    DTU::DtuArray2D<double>* ak;
    DTU::DtuArray2D<double>* akInv;
    DTU::DtuArray2D<double>* akatInvAlpha;
    DTU::DtuArray2D<double>* sigmaS;
    DTU::DtuArray2D<double>* inputMatrix; //channels:channel
    DTU::DtuArray2D<double>* identity;
    DTU::DtuArray2D<double>* identityInvBeta;
    DTU::DtuArray2D<double>* tempW;
    DTU::DtuArray2D<double>* midW; //dummy holder, vertices:channels
    DTU::DtuArray2D<double>* invSigmaE; //channels:channels
    DTU::DtuArray2D<double>* invSigmaEA; //channels:vertices
    DTU::DtuArray2D<double>* atInvSigmaEA; //vertices:vertices
    DTU::DtuArray2D<double>* invSigmaEAS; //channels:samples
    DTU::DtuArray2D<double>* katInput; //vertices:channels

    int paramMeanExtractionOn;
    int paramAScaling;

    Sbs2Spectrogram* sbs2Spectrogram;
    DTU::DtuArray2D<double>* tempInput; //1:weightedInput->dim2() used for spectro
    DTU::DtuArray2D<double>* tempOutput; //1:weightedInput->dim2() used for spectro

    QVector<int>* verticesToExtract;

    int modelUpdateSamplesLength;

signals:
    
public slots:
    void setSumType(SumType sumType_);
    void setMeanExtraction(int enabled);
    void setAScaling(int scaling);
    void setVerticesToExtract(QVector<int>* verticesToExtract_);
    void doRec(DTU::DtuArray2D<double>* input_, DTU::DtuArray2D<double>* output_, int* sourceReconstructionReady);
    void doRecPow(DTU::DtuArray2D<double>* input_, DTU::DtuArray2D<double>* output_, int* sourceReconstrutionReady);
    
};

#endif // SBS2SOURCERECONSTRUCTIONLORETA_H
