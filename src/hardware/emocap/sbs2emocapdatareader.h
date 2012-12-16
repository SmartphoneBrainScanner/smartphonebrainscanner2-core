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

#ifndef SBS2EMOCAPDATAREADER_H
#define SBS2EMOCAPDATAREADER_H

#include <hardware/sbs2datareader.h>
#include <hardware/emocap/sbs2emocapmounter.h>
#include <hardware/emotiv/sbs2emotivdecryptor.h>
#include <hardware/emocap/sbs2emocappacket.h>

#ifdef Q_OS_MAC
#include <platform/osx/hidapi.h>
#endif

class Sbs2EmocapDataReader: public Sbs2DataReader
{
    Q_OBJECT

public:
    static Sbs2EmocapDataReader* New(Sbs2Callback* sbs2Callback_, int readOnlyFromNetwork_ = 0, QObject *parent = 0);
    ~Sbs2EmocapDataReader();

private:
    Sbs2EmocapDataReader(Sbs2Callback* sbs2Callback_, int readOnlyFromNetwork_ = 0, QObject *parent = 0);
    void execute();
    void resetHandle();

private:
    std::ifstream rawFile;
    char buffer_main[32];
    char buffer_final[32];
    Sbs2EmotivDecryptor* sbs2EmotivDecryptor;
    Sbs2EmocapMounter* sbs2EmocapMounter;
    static Sbs2EmocapDataReader* m_pInstance;
    QVector<Sbs2EmocapPacket*> sbs2Packets;

#ifdef Q_OS_MAC
    hid_device *handle;
#endif

public slots:
    void deviceFound(QMap<QString, QVariant> params);
    void deviceLost();
    void aboutToQuit();
    void udpDataReceived(QVector<char*>* data, int counter);
    void udpDataReceived(QUdpSocket* rawDataUdpInputSocket);
    void turnReceiveUdpDataOn(QString address,int port);
    void turnReceiveUdpDataOff();

};

#endif // SBS2EMOCAPDATAREADER_H
