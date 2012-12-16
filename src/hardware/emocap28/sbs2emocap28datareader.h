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

#ifndef SBS2EMOCAP28DATAREADER_H
#define SBS2EMOCAP28DATAREADER_H

#include <hardware/sbs2datareader.h>
#include <hardware/emocap28/sbs2emocap28mounter.h>
#include <hardware/emotiv/sbs2emotivdecryptor.h>
#include <hardware/emocap28/sbs2emocap28packet.h>

#ifdef Q_OS_MAC
#include <platform/osx/hidapi.h>
#endif

class Sbs2Emocap28DataContainer
{
public:
    Sbs2Emocap28DataContainer() {data = new char[32]; counter = -1;}
    void update(char* data_, int counter_);
public:
    char* data;
    int counter;

};

class Sbs2Emocap28DataReader: public Sbs2DataReader
{
    Q_OBJECT

public:
    static Sbs2Emocap28DataReader* New(Sbs2Callback* sbs2Callback_, int readOnlyFromNetwork_ = 0, QObject *parent = 0);
    ~Sbs2Emocap28DataReader();

private:
    Sbs2Emocap28DataReader(Sbs2Callback* sbs2Callback_, int readOnlyFromNetwork_ = 0, QObject *parent = 0);
    void execute1();
    void execute2();

    void amp1(int number);
    void amp2(int number);

    double correlation(QVector<int>* v1, QVector<int>* v2);
    void collect(char* data, int amp);

private:
    std::ifstream rawFile1;
    std::ifstream rawFile2;
    char buffer_main1[32];
    char buffer_final1[32];
    char buffer_main2[32];
    char buffer_final2[32];
    Sbs2EmotivDecryptor* sbs2EmotivDecryptor1;
    Sbs2EmotivDecryptor* sbs2EmotivDecryptor2;
    Sbs2Emocap28Mounter* sbs2Emocap28Mounter;
    static Sbs2Emocap28DataReader* m_pInstance;
    QVector<Sbs2Emocap28Packet*> sbs2Packets;

    QString path1;
    QString path2;

    QString serialNumber1;
    QString serialNumber2;

    int packetsRead1;
    int packetsRead2;

    int amp1Found;
    int amp1Number;
    int amp2Found;
    int amp2Number;

    int inMapping;

    int amp1PacketLoss;
    int amp2PacketLoss;
    int amp1LastPacket;
    int amp2LastPacket;

    int amp1MappingPacketsCollected;
    int amp2MappingPacketsCollected;

    int mapped;
    int mapping;
    QMap<int, int> mappingHist;

    Sbs2Emocap28Packet* amp1SinglePacket;
    Sbs2Emocap28Packet* amp2SinglePacket;

    int amp1CurrentPacketIndex;
    int amp2CurrentPacketIndex;

    int aligning;
    int samplesInAligning;
    int mappingAlignment;
    double mappingCorr;

    QVector<int> a1v;
    QVector<int> a2v;
    QVector<int> a1c;
    QVector<int> a2c;

    int a1value;
    int a2value;

    int collecting;

    int submittedAmp1Value;
    int crossedZero;


    QVector<Sbs2Emocap28DataContainer*> amp1Buffer;
    QVector<Sbs2Emocap28DataContainer*> amp2Buffer;
    int amp1BufferPosition;
    int amp2BufferPosition;
    int ampBufferSize;

#ifdef Q_OS_MAC
    hid_device *handle;
#endif

signals:
    void amp1FoundSignal(QVariant number, QVariant path, QVariant serialNumber);
    void amp2FoundSignal(QVariant number, QVariant path, QVariant serialNumber);
    void readyForData();
    void inMappingSignal();
    void mappingSuccessful(int mapping);
    void mappingFailed();
    void alignedSignal(int mapping, int mappingAlignment, int mappingCorr);

public slots:
    void deviceFound(QMap<QString, QVariant> params);
    void deviceLost();
    void aboutToQuit();
    void udpDataReceived(QVector<char*>* data, int counter);
    void udpDataReceived(QUdpSocket* rawDataUdpInputSocket);
    void turnReceiveUdpDataOn(QString address,int port);
    void turnReceiveUdpDataOff();

};

#endif // SBS2EMOCAP28DATAREADER_H
