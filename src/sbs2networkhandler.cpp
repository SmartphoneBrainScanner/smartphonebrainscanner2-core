#include <sbs2networkhandler.h>

Sbs2NetworkHandler::Sbs2NetworkHandler(QObject *parent):QObject(parent)
{
    sendRawDataOn = 0;
    finalRawData = 0;
    rawDataCounter = 0;
    receiveRawDataOn = 0;
    rawDataUdpInputSocket = 0;
    rawDataUdpSocket = 0;
    messageUdpOutputSocket = 0;
    messageUdpOutputHosts.clear();
    messageUdpInputSocket = 0;


}

void Sbs2NetworkHandler::turnSendRawDataOn(QString rawDataServerAddress_, int rawDataPort_, int rawDataSize_, int rawDataQueueLength_)
{

    rawDataHosts.clear();
    finalRawData = 0;
    rawDataCounter = 0;

    rawDataSize = rawDataSize_;
    rawDataQueueLength = rawDataQueueLength_;
    finalRawDataSize = rawDataQueueLength*rawDataSize+sizeof(int)+1+1;

    while(finalRawDataSize > MAX_BUFFER_SIZE)
    {
        --rawDataQueueLength;
        finalRawDataSize = rawDataQueueLength*rawDataSize+sizeof(int)+1+1;
    }


    rawDataHosts.insert(rawDataServerAddress_,rawDataPort_);
    rawDataUdpSocket = new QUdpSocket(this);
    sendRawDataOn = 1;
}

void Sbs2NetworkHandler::turnSendRawDataOff()
{
    sendRawDataOn = 0;
    if (finalRawData != 0)
        delete finalRawData;
    finalRawData = 0;

    if (rawDataUdpSocket != 0)
        delete rawDataUdpSocket;
    rawDataUdpSocket = 0;

}

void Sbs2NetworkHandler::turnReceiveRawDataOn(QString rawDataUdpInputAddress_, int rawDataUdpInputPort_)
{
    qDebug() << "turnReceiveRawDataOn: "<<rawDataUdpInputAddress_<<rawDataUdpInputPort_;
    rawDataUdpInputAddress = rawDataUdpInputAddress_;
    rawDataUdpInputPort = rawDataUdpInputPort_;
    rawDataUdpInputSocket = new QUdpSocket(this);
    rawDataUdpInputSocket->bind(QHostAddress(rawDataUdpInputAddress),rawDataUdpInputPort);
    QObject::connect(rawDataUdpInputSocket,SIGNAL(readyRead()),this,SLOT(readRawData()));
    receiveRawDataOn = 1;
}

void Sbs2NetworkHandler::turnReceiveRawDataOff()
{
    receiveRawDataOn = 0;
    if (rawDataUdpInputSocket != 0)
        delete rawDataUdpInputSocket;
    rawDataUdpInputSocket = 0;
}


void Sbs2NetworkHandler::addRawDataHost(QString address, int port)
{
    rawDataHosts.insertMulti(address,port);
}

void Sbs2NetworkHandler::removeRawDataHost(QString address, int port)
{
    QMapIterator<QString, int> i(rawDataHosts);
    while (i.hasNext())
    {
        i.next();
        if (address.compare(i.key()) == 0 && port == i.value())
            rawDataHosts.remove(address);
    }


}

//todo: move the data forming to datareaders, so we can use one network handler for everythin

void Sbs2NetworkHandler::sendRawData(char *data)
{

    //0 :: last - sizeof(int): data from the oldest to the newest
    //last - sizeof(int) :: last - 1: rawDataCounter divided into 8 bit chunks
    //last - 1: sizeof(int) (size of int in this implementation)
    //last: rawDataSize (size of a single data chunk, usually 32 bytes)


    if (!sendRawDataOn)
        return;

    rawDataQueue.append(new char[rawDataSize]);
    char* newBuff = rawDataQueue.last();

    for (int i=0; i<rawDataSize; ++i)
    {
        newBuff[i] = data[i];
    }


    if (rawDataQueue.size() == rawDataQueueLength)
    {

        if (finalRawData != 0)
            delete finalRawData;

        finalRawData = new char[finalRawDataSize];

        for (int i=0; i<rawDataQueueLength; ++i)
        {
            for (int k=0; k<rawDataSize; ++k)
            {
                finalRawData[i*rawDataSize+k] = rawDataQueue.at(i)[k];
            }
        }

        finalRawData[finalRawDataSize-2] = char(sizeof(int));
        finalRawData[finalRawDataSize-1] = char(rawDataSize);


        for (int i=0; i<sizeof(int); ++i)
        {
            finalRawData[rawDataQueueLength*rawDataSize+i] = ((rawDataCounter >> i*8 ) & 0x0FF );
        }

        ++rawDataCounter;



        foreach(char* v, rawDataQueue)
        {
            delete[] v;
        }


        QMapIterator<QString, int> i(rawDataHosts);
        while (i.hasNext())
        {
            i.next();
            rawDataUdpSocket->writeDatagram(finalRawData,finalRawDataSize,QHostAddress(i.key()),i.value());
        }
        rawDataQueue.clear();
        emit rawDataSentSignal();
    }

}

void Sbs2NetworkHandler::readRawData()
{
    emit rawDataReceived(rawDataUdpInputSocket);
}

void Sbs2NetworkHandler::sendMessage(QString message, QString address, int port)
{

    if (messageUdpOutputSocket == 0)
        messageUdpOutputSocket = new QUdpSocket(this);


    QByteArray m;
    m.append(message);
    messageUdpOutputSocket->writeDatagram(m,QHostAddress(address),port);

}

void Sbs2NetworkHandler::sendMessage(QString message)
{

    if (messageUdpOutputSocket == 0)
        messageUdpOutputSocket = new QUdpSocket(this);

    QByteArray m;
    m.append(message);

    QMapIterator<QString, int> i(messageUdpOutputHosts);
    while (i.hasNext())
    {
        i.next();
        messageUdpOutputSocket->writeDatagram(m,QHostAddress(i.key()),i.value());
    }


}

void Sbs2NetworkHandler::addMessageUdpOutputHost(QString address, int port)
{
    messageUdpOutputHosts.insertMulti(address,port);
}

void Sbs2NetworkHandler::removeMessageUdpOutputHost(QString address)
{
    messageUdpOutputHosts.remove(address);
}

void Sbs2NetworkHandler::clearMessageUdpOutputHosts()
{
    messageUdpOutputHosts.clear();
}

void Sbs2NetworkHandler::turnReceiveMessageOn(QString address, int port)
{


    if (messageUdpInputSocket != 0)
        delete messageUdpInputSocket;

    messageUdpDataInputAddress = address;
    messageUdpDataInputPort = port;
    messageUdpInputSocket = new QUdpSocket(this);
    messageUdpInputSocket->bind(QHostAddress(messageUdpDataInputAddress),messageUdpDataInputPort);
    QObject::connect(messageUdpInputSocket,SIGNAL(readyRead()),this,SLOT(readMessage()));

}

void Sbs2NetworkHandler::turnReceiveMessageOff()
{
    QObject::disconnect(messageUdpInputSocket,SIGNAL(readyRead()),this,SLOT(readMessage()));
}

void Sbs2NetworkHandler::readMessage()
{


    while (messageUdpInputSocket->hasPendingDatagrams())
    {

        QByteArray datagram;
        datagram.resize(messageUdpInputSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        messageUdpInputSocket->readDatagram(datagram.data(), datagram.size(),&sender, &senderPort);

        emit messageReceived(QString(datagram.data()), sender.toString(), senderPort);
    }

}
