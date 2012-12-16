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

#ifndef SBS2DATAREADER_H
#define SBS2DATAREADER_H

#include <QObject>
#include <fstream>
#include <sbs2callback.h>
#include <sbs2networkhandler.h>


class Sbs2Common;
class Sbs2Callback;
class Sbs2NetworkHandler;


/**
  @class Class responsible for reading the raw data from the device and delivering single well formed packet.
  */


class Sbs2DataReader : public QObject
{
    Q_OBJECT

public:
    ~Sbs2DataReader();

protected:
    Sbs2DataReader(Sbs2Callback* sbs2Callback_, int readOnlyFromNetwork_ = 0, QObject *parent = 0);
    virtual void execute();

protected:
    int framesRead;
    int currentIndex;
    int bufferIndex;
    int bufferSize;
    int running;
    Sbs2Callback* sbs2Callback;
    int testDummyRead; //push empty packets, used only for testing
    Sbs2NetworkHandler* sbs2NetworkHandler;
    int readOnlyFromNetwork;
    int lastReceiveRawDataCounter;

signals:
    void deviceFoundSignal(QMap<QString, QVariant> params);
    
public slots:
    virtual void deviceFound(QMap<QString, QVariant> params);
    virtual void deviceLost();
    virtual void aboutToQuit();
    virtual void udpDataReceived(QVector<char*>* data, int counter);
    virtual void udpDataReceived(QUdpSocket* rawDataUdpInputSocket);
    virtual void turnReceiveUdpDataOn(QString address,int port);
    virtual void turnReceiveUdpDataOff();

};

#endif // SBS2DATAREADER_H
