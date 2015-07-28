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

#ifndef SBS2DATAHANDLER_H
#define SBS2DATAHANDLER_H

#include <QObject>
#include <sbs2networkhandler.h>
#include <sbs2filter.h>
#include <hardware/sbs2packet.h>
#include <sbs2common.h>
#include <sbs2filehandler.h>
#include <sbs2spectrogram.h>
#include <source_reconstruction/sbs2sourcereconstruction.h>
#include <QtCore>

// MRA
#include <sbs2pca.h>
#include "sbs2dummypca.h"


class Sbs2NetworkHandler;

class Sbs2DataHandler : public QObject
{
    Q_OBJECT

public:
    explicit Sbs2DataHandler(QObject *parent = 0);
    ~Sbs2DataHandler();
    
    // Temporal filtering
    virtual void filter();

    // Recording
    virtual void record();
    virtual QString getRawFilename();

    // Spectrogram
    virtual void spectrogramChannel();

    // Network
    virtual void sendRawData();

    //access
    DTU::DtuArray2D<double>* getPowerValues();
    DTU::DtuArray2D<double>* getSourceReconstructionSpectrogramValues();
    DTU::DtuArray2D<double>* getSourceReconstructionMeanValues();

    int getPacketZero();

protected:
    int samplesCollected;
    Sbs2Packet* thisPacket;

    // MRA
    Sbs2DummyPca* sbs2Pca;
    int pcaSamplesSkipped;
    int pcaBlockSize;
    int pcaBlockSkip;
    int pcaThreshold;
    int pcaOn;
    DTU::DtuArray2D<double>* toPcaValues;
    DTU::DtuArray2D<double>* pcaReturnValues;

    //filtering
    int filterOn;
    int filterOrder;
    int fbandLow;
    int fbandHigh;
	//objects
    Sbs2Filter* sbs2Filter;
    int filterOrder;
    DTU::DtuArray2D<double>* toFilterValues;
    DTU::DtuArray2D<double>* filterResultValues;


    // Recording
    int recording;
    Sbs2FileHandler* sbs2FileHandler;


    // Spectral analysis
    int spectrogramChannelOn;
    int spectrogramChannelSamples; //on how many samples perform spectrogram
    int spectrogramChannelLength; //freq, 128 for emotiv
    int spectrogramChannelDelta;    //distance between spectrograms
    int spectrogramChannelDeltaCollected; //how many samples already collected
    // objects
    Sbs2Spectrogram* sbs2Spectrogram;
	//data matrices
    DTU::DtuArray2D<double>* toSpectrogramValues;
    DTU::DtuArray2D<double>* spectrogramValues;
    DTU::DtuArray2D<double>* powerValues;


    // Source reconstruction
    QString sourceReconstructionMethod;
    int sourceReconstructionOn;
    int isSourceReconstructionReady;
    int sourceReconstructionSamples;
    int sourceReconstructionDelta;
    int sourceReconstructionDeltaCollected;
    int sourceReconstructionModelUpdateLength;
    int sourceReconstructionModelUpdateDelta;
    int readyToReconstruct;

    QString hardware;

    // objects
    Sbs2SourceReconstruction* sbs2SourceReconstruction;
    // data matrices
    DTU::DtuArray2D<double>* toSourceReconstructionValues;
    DTU::DtuArray2D<double>* sourceReconstructionValues;
    DTU::DtuArray2D<double>* sourceReconstructionSpectrogramValues;


    // Network
    int networkSendRawDataOn;
	//objects
    Sbs2NetworkHandler* sbs2NetworkHandler;

    int packetsSeen;

protected:
    virtual void reset();

signals:
    void spectrogramUpdated();
    void sourceReconstructionReady();
    void sourceReconstructionSpectrogramReady();
    void setWindowTypeSignal(Sbs2Spectrogram::WindowType windowType);
    void udpMessageReceived(QString data, QString sender, int port);

    // MRA
    void pcaUpdated();

public slots:
    void setThisPacket(Sbs2Packet* thisPacket_);

<<<<<<< HEAD
    // Temporal filtering
    void turnFilterOn(int fbandLow_, int fbandHigh_);
=======
    // MRA PCA
    void turnPcaOn(int threshold_);
    void turnPcaOff();

    //filtering
    void turnFilterOn(int fbandLow_, int fbandHigh_, int filterOrder_);
>>>>>>> Include MRA's pca artifact filter
    void turnFilterOff();

    // Recording
    void startRecording(QString user, QString description);
    void stopRecording();
    void insertIntoMetaFile(QString event);

    // Spectral analysis
    void turnChannelSpectrogramOn(int spectrogramChannelSamples_ = 128, int spectrogramChannelLength_ = 128, int spectrogramChannelDelta_ = 0);
    void turnChannelSpectrogramOff();
    void setWindowType(Sbs2Spectrogram::WindowType windowType);

    // Source reconstruction
    void setSourceReconstructionVerticesToExtract(QVector<int>* verticesToExtract);

    void turnOnSourceReconstructionLoreta(int sourceReconstructionSamples_, int sourceReconstructionDelta_, int sourceReconstructionModelUpdateLength_, int sourceReconstructionModelUpdateDelta_, QString hardware_);
    void turnOnSourceReconstructionSparse(int sourceReconstructionSamples_, QVector<double> lambdas, QString hardware_);
    void doSourceReconstruction();
    void doSourceReconstructionSpectrogram();
    void turnOffSourceReconstruction();
    void setVerticesToExtract(QVector<int>* verticesToExtract);

    // Network
    void turnSendRawDataOn(QString rawDataServerAddress_, int rawDataPort_, int rawDataSize_ = 32, int rawDataQueueLength_ = 8);
    void turnSendRawDataOff();
    void addRawDataHost(QString address, int port);
    void removeRawDataHost(QString address, int port);

    void sendMessage(QString message, QString address, int port);
    void sendMessage(QString message);
    void addMessageUdpOutputHost(QString address, int port);
    void removeMessageUdpOutputHost(QString address);
    void clearMessageUdpOutputHosts();

    void turnReceiveMessageOn(QString address, int port);
    void turnReceiveMessageOff();
    void readMessage(QString data, QString sender, int senderPort);
};

#endif // SBS2DATAHANDLER_H
