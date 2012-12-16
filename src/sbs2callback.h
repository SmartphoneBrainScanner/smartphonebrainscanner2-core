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

#ifndef SBS2CALLBACK_H
#define SBS2CALLBACK_H

#include <QObject>
#include <QNetworkInterface>
#include <QVariant>

#include <hardware/sbs2packet.h>
#include <sbs2spectrogram.h>
#include <sbs2region.h>


class Sbs2DataHandler;


class Sbs2Callback : public QObject
{
    Q_OBJECT
public:
    explicit Sbs2Callback(QObject *parent = 0);
    virtual void getData(Sbs2Packet* packet) {};
    static int getCurrentPacketCounter();
    static int getCurrentPacket();
    QString getRawFilename();


protected:
    Sbs2DataHandler* sbs2DataHandler;
    void setSbs2DataHandler(Sbs2DataHandler* sbs2DataHandler_);
    int samplesCollected;
    static int currentPacketCounter;
    static int currentPacket;
    Sbs2Packet* thisPacket;
    Sbs2Region* sbs2Region;
    int isRecording;
    QMap<QString, QVariant> params;
    int devicePresent;

signals:
    void timeTick10();
    void timeTick2();
    void timeTick4();
    void timeTick8();
    void timeTick0();
    void timeTick16();

    void setWindowTypeSignal(Sbs2Spectrogram::WindowType windowType);
    void batteryValue(QVariant value);
    void cqValues(QVariant channel, QVariant value);
    void cqValue(QString channel, double value);
    void spectrogramUpdated();

    void udpMessageReceived(QString data, QString sender, int port);
    void networkAddresses(QVariant data);
    void hardwareChanged(QString hardware);

    void deviceFoundSignal(QMap<QString, QVariant> params);

public slots:
    void startRecording(QString user, QString description);
    void stopRecording();
    void insertIntoMetaFile(QString event);


    void turnFilterOn(int fbandLow_, int fbandHigh_, int filterOrder_);
    void turnFilterOff();

    void turnChannelSpectrogramOn(int spectrogramChannelSamples_ = 128, int spectrogramChannelLength_ = 128, int spectrogramChannelDelta_ = 0);
    void turnChannelSpectrogramOff();

    void setWindowType(Sbs2Spectrogram::WindowType windowType);
    void setWindowType(int windowType);

    void turnSourceReconstructionOn(int sourceReconstructionSamples_, int sourceReconstructionDelta_, int sourceReconstructionModelUpdateLength_, int sourceReconstructionModelUpdateDelta_, QString hardware = "emotiv");
    void turnSourceReconstructionOff();

    void turnSourceReconstructionPowerOn(int sourceReconstructionSamples_, int sourceReconstructionDelta_, int sourceReconstructionModelUpdateLength_, int sourceReconstructionModelUpdateDelta_, QString hardware = "emotiv");
    void turnSourceReconstructionPowerOff();

    void spectrogramUpdatedSlot();

    void setHardware(QString hardware);

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
    void getNetworkAddresses();


    void deviceFound(QMap<QString, QVariant> params_);

};

#endif // SBS2CALLBACK_H
