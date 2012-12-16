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

#ifndef SBS2NETWORKHANDLER_H
#define SBS2NETWORKHANDLER_H

#include <QString>
#include <QUdpSocket>
#include <vector>
#include <iostream>
#include <QTcpSocket>
#include <cmath>

//todo:
//move this to hardware
//abstract the sending somehow (use it in reader?)

//max buffer size in bytes, above that packets can be fragmented in network
#define MAX_BUFFER_SIZE 512

class Sbs2NetworkHandler: public QObject
{
    Q_OBJECT

public:
    Sbs2NetworkHandler(QObject* parent = 0);


private:
    int rawDataQueueLength;
    QVector<char*> rawDataQueue;
    int rawDataSize;
    char* finalRawData;
    int finalRawDataSize;

    QUdpSocket* rawDataUdpSocket;
    QMap<QString,int> rawDataHosts;

    int sendRawDataOn;
    unsigned int rawDataCounter;

    int receiveRawDataOn;
    QUdpSocket* rawDataUdpInputSocket;
    QString rawDataUdpInputAddress;
    int rawDataUdpInputPort;

    QUdpSocket* messageUdpOutputSocket;
    QMap<QString,int> messageUdpOutputHosts;

    QUdpSocket* messageUdpInputSocket;
    QString messageUdpDataInputAddress;
    int messageUdpDataInputPort;

signals:
    void rawDataSentSignal();
    void rawDataReceived(char* data, int size, QString address, int port);
    void rawDataReceived(QVector<char*>* data,int counter);
    void rawDataReceived(QUdpSocket* rawDataUdpInputSocket);
    void messageReceived(QString data, QString sender, int senderPort);

public slots:
    void turnSendRawDataOn(QString rawDataServerAddress_, int rawDataPort_, int rawDataSize_, int rawDataQueueLength_);
    void turnSendRawDataOff();
    void addRawDataHost(QString address, int port);
    void removeRawDataHost(QString address, int port);
    void sendRawData(char* data);

    void turnReceiveRawDataOn(QString rawDataUdpInputAddress_, int rawDataUdpInputPort_);
    void turnReceiveRawDataOff();
    void readRawData();

    void sendMessage(QString message, QString address, int port);
    void sendMessage(QString message);
    void addMessageUdpOutputHost(QString address, int port);
    void removeMessageUdpOutputHost(QString address);
    void clearMessageUdpOutputHosts();

    void turnReceiveMessageOn(QString address, int port);
    void turnReceiveMessageOff();
    void readMessage();

};




#endif //SBS2NETWORKHANDLER_H
