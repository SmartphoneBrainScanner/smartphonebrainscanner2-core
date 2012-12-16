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
#include <sbs2sourcereconstruction.h>
#include <QtCore>


class Sbs2NetworkHandler;




class Sbs2DataHandler : public QObject
{
    Q_OBJECT
public:
    explicit Sbs2DataHandler(QObject *parent = 0);
    ~Sbs2DataHandler();
    
    //filtering
    virtual void filter();

    //recording
    virtual void record();
    virtual QString getRawFilename();

    //spectrogram
    virtual void spectrogramChannel();

    //source reconstruction
    virtual void sourceReconstruction();
    virtual void sourceReconstructionPower();

    //network
    virtual void sendRawData();

    //access
    DTU::DtuArray2D<double>* getPowerValues();
    DTU::DtuArray2D<double>* getSourceReconstructionPowerValues();

    int getPacketZero();

protected:
    int samplesCollected;
    Sbs2Packet* thisPacket;


    //filtering
    int filterOn;
    int filterOrder;
    int fbandLow;
    int fbandHigh;
	//objects
    Sbs2Filter* sbs2Filter;
	//data matrices
    DTU::DtuArray2D<double>* toFilterValues;
    DTU::DtuArray2D<double>* filterResultValues;

    //recording
    int recording;
	//objects
    Sbs2FileHandler* sbs2FileHandler;


    //spectral analysis
    int spectrogramChannelOn;
    int spectrogramChannelSamples; //on how many samples perform spectrogram
    int spectrogramChannelLength; //freq, 128 for emotiv
    int spectrogramChannelDelta;    //distance between spectrograms
    int spectrogramChannelDeltaCollected; //how many samples already collected
	//objects
    Sbs2Spectrogram* sbs2Spectrogram;
	//data matrices
    DTU::DtuArray2D<double>* toSpectrogramValues;
    DTU::DtuArray2D<double>* spectrogramValues;
    DTU::DtuArray2D<double>* powerValues;


    //source reconstruction
    int sourceReconstructionOn;
    int sourceReconstructionPowerOn;
    int isSourceReconstructionReady;
    int sourceReconstructionSamples;
    int sourceReconstructionDelta;
    int sourceReconstructionDeltaCollected;
    int sourceReconstructionModelUpdateLength;
    int sourceReconstructionModelUpdateDelta;

    QString hardware;

	//objects
    Sbs2SourceReconstrucion* sbs2SourceReconstruction;
	//data matrices
    DTU::DtuArray2D<double>* toSourceReconstructionValues;
    DTU::DtuArray2D<double>* sourceReconstructionValues;
    DTU::DtuArray2D<double>* sourceReconstructionPowerValues;



    //network
    int networkSendRawDataOn;
	//objects
    Sbs2NetworkHandler* sbs2NetworkHandler;

protected:
    virtual void reset();

signals:
    void spectrogramUpdated();
    void sourceReconstructionReady();
    void sourceReconstructionPowerReady();
    void setWindowTypeSignal(Sbs2Spectrogram::WindowType windowType);
    void udpMessageReceived(QString data, QString sender, int port);

public slots:
    void setThisPacket(Sbs2Packet* thisPacket_);

    //filtering
    void turnFilterOn(int fbandLow_, int fbandHigh_, int filterOrder_);
    void turnFilterOff();

    //recording
    void startRecording(QString user, QString description);
    void stopRecording();
    void insertIntoMetaFile(QString event);

    //spectral analysis
    void turnChannelSpectrogramOn(int spectrogramChannelSamples_ = 128, int spectrogramChannelLength_ = 128, int spectrogramChannelDelta_ = 0);
    void turnChannelSpectrogramOff();
    void setWindowType(Sbs2Spectrogram::WindowType windowType);

    //source reconstruction
    void turnSourceReconstructionOn(int sourceReconstructionSamples_, int sourceReconstructionDelta_, int sourceReconstructionModelUpdateLength_, int sourceReconstructionModelUpdateDelta_, QString hardware_);
    void turnSourceReconstructionOff();

    void turnSourceReconstructionPowerOn(int sourceReconstructionSamples_, int sourceReconstructionDelta_, int sourceReconstructionModelUpdateLength_, int sourceReconstructionModelUpdateDelta_, QString hardware_);
    void turnSourceReconstructionPowerOff();
    void setVerticesToExtract(QVector<int>* verticesToExtract);

    //network
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
