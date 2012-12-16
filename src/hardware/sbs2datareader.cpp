#include "sbs2datareader.h"


//Sbs2DataReader* Sbs2DataReader::m_pInstance = 0;

//Sbs2DataReader* Sbs2DataReader::New(Sbs2Callback *sbs2Callback_, int overclockLevel, int readOnlyFromNetwork_, QObject *parent)
//{
//    if (!m_pInstance)
//	m_pInstance = new Sbs2DataReader(sbs2Callback_, overclockLevel, readOnlyFromNetwork_, parent);

//    return m_pInstance;
//}



Sbs2DataReader::Sbs2DataReader(Sbs2Callback *sbs2Callback_, int readOnlyFromNetwork_,  QObject *parent):QObject(parent), sbs2Callback(sbs2Callback_)
{
    framesRead = 0;
    currentIndex = 0;
    bufferIndex = 0;
    bufferSize = 256;
    running = 0;

    testDummyRead = 0;

    readOnlyFromNetwork = readOnlyFromNetwork_;
    lastReceiveRawDataCounter = 0;

    QObject::connect(this,SIGNAL(deviceFoundSignal(QMap<QString,QVariant>)),sbs2Callback, SLOT(deviceFound(QMap<QString,QVariant>)));
}

void Sbs2DataReader::turnReceiveUdpDataOn(QString address, int port) {}

void Sbs2DataReader::turnReceiveUdpDataOff() {}

void Sbs2DataReader::udpDataReceived(QVector<char *> *data, int counter)
{
    lastReceiveRawDataCounter = 0;
}

void Sbs2DataReader::udpDataReceived(QUdpSocket *rawDataUdpInputSocket) {}

void Sbs2DataReader::execute() {}

void Sbs2DataReader::deviceFound(QMap<QString, QVariant> params) {emit deviceFoundSignal(params);}

void Sbs2DataReader::deviceLost() {}

Sbs2DataReader::~Sbs2DataReader() {}

void Sbs2DataReader::aboutToQuit() {}
