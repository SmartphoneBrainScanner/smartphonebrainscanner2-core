#include <hardware/emocap/sbs2emocapdatareader.h>

Sbs2EmocapDataReader* Sbs2EmocapDataReader::m_pInstance = 0;

Sbs2EmocapDataReader* Sbs2EmocapDataReader::New(Sbs2Callback *sbs2Callback_, int readOnlyFromNetwork_, QObject *parent)
{
    if (!m_pInstance)
        m_pInstance = new Sbs2EmocapDataReader(sbs2Callback_, readOnlyFromNetwork_, parent);

    return m_pInstance;
}

Sbs2EmocapDataReader::Sbs2EmocapDataReader(Sbs2Callback *sbs2Callback_, int readOnlyFromNetwork_, QObject *parent):Sbs2DataReader(sbs2Callback_,readOnlyFromNetwork_,parent)
{

    for (int i=0; i< bufferSize; ++i)
    {
        sbs2Packets.append(new Sbs2EmocapPacket(this));
    }

    if (!readOnlyFromNetwork) //read from the local device
    {
        sbs2EmotivDecryptor = new Sbs2EmotivDecryptor(this);
        sbs2EmocapMounter = Sbs2EmocapMounter::New(this);
        QObject::connect(sbs2EmocapMounter,SIGNAL(deviceFound(QMap<QString,QVariant>)),this,SLOT(deviceFound(QMap<QString,QVariant>)));
        QObject::connect(sbs2EmocapMounter,SIGNAL(deviceLost()),this,SLOT(deviceLost()));
        sbs2EmocapMounter->start();
    }
    else //receive packets from remote host, we do nothing, waiting for turnOn
    {
        sbs2EmocapMounter = 0;
    }
}

Sbs2EmocapDataReader::~Sbs2EmocapDataReader()
{
    m_pInstance = 0;
}

void Sbs2EmocapDataReader::execute()
{
    int counter = 0;
    int cc = 0;
    int uu = 0;
    int res = 0;
    while(1)
    {
        if(!running)
            return;
#ifdef Q_OS_MAC
        if (testDummyRead)
        {
            counter = (counter +1)%1024000;
            if (counter > 0)
                continue;
        }
        else
        {
	    res = hid_read(handle, (unsigned char*)buffer_main, sizeof(buffer_main));
	    if (res > 0)
		cc = 1;
	    if (res == -1)
	    {
		uu += (1-cc);
		if (uu == 100000)
		{
		    resetHandle();
		    uu = 0;
		}
		continue;
	    }

        }
#else
        if (testDummyRead)
        {
            counter = (counter +1)%1024000;
            if (counter > 0)
                continue;
        }
        else
        {
            rawFile.read(buffer_main, 32);

        }
#endif

        ++framesRead;
        sbs2EmotivDecryptor->decrypt(buffer_main,buffer_final);
        sbs2Packets[currentIndex] -> update(buffer_final);
        sbs2Callback->getData(sbs2Packets[currentIndex]);
        currentIndex = (currentIndex+1)%256;
    }
}

void Sbs2EmocapDataReader::deviceFound(QMap<QString, QVariant> params)
{
    Sbs2DataReader::deviceFound(params);

#ifdef Q_OS_MAC
    handle = Sbs2EmocapMounter::getHandle();
#endif
    sbs2EmotivDecryptor->setSerialNumber(params["serialNumber"].toString());
    running = 1;
#ifdef Q_OS_MAC

#else
    rawFile.open(params["path"].toString().toStdString().c_str(), std::ios::in | std::ios::binary);
#endif
    QtConcurrent::run(this,&Sbs2EmocapDataReader::execute);


}

void Sbs2EmocapDataReader::deviceLost()
{
    running = 0;
    rawFile.close();
    qDebug() << "device lost";
}

void Sbs2EmocapDataReader::aboutToQuit()
{
    running = 0;
    sbs2EmocapMounter->stop();
}

void Sbs2EmocapDataReader::udpDataReceived(QVector<char *> *data, int counter)
{
    for (int i=0; i<data->size(); ++i)
    {
        ++framesRead;
        sbs2Packets[currentIndex] -> update(data->at(i));
        sbs2Callback->getData(sbs2Packets[currentIndex]);
        currentIndex = (currentIndex+1)%256;
    }

    for (int i=0; i<data->size(); ++i)
    {
        delete[] data->at(i);
    }
    data->clear();
    delete data;
}

void Sbs2EmocapDataReader::udpDataReceived(QUdpSocket *rawDataUdpInputSocket)
{

    //0 :: last - sizeof(int): data from the oldest to the newest
    //last - sizeof(int) :: last - 1: rawDataCounter divided into 8 bit chunks
    //last - 1: sizeof(int) (size of int in this implementation)
    //last: rawDataSize (size of a single data chunk, usually 32 bytes)

    while (rawDataUdpInputSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(rawDataUdpInputSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        rawDataUdpInputSocket->readDatagram(datagram.data(), datagram.size(),&sender, &senderPort);

        int rawDataSize = datagram.data()[datagram.size()-1];
        int intSize = datagram.data()[datagram.size()-2];
        int currentCounter = 1;
        int noPackets = (datagram.size() - 1 - intSize)/rawDataSize;

        for (int i=intSize-1; i>=0; --i)
        {
            int v = (int)datagram.data()[datagram.size()-3-i];
            if (v < 0) v+= 256;
	    currentCounter += v * pow(2,(double)8*(intSize-i-1));

        }


        QVector<char*>* data = new QVector<char*>;
        for (int i= 0; i<noPackets; ++i)
        {
            data->append(new char[rawDataSize]);
            char* buff = data->last();
            for (int k = 0; k<rawDataSize; ++k)
            {
                buff[k] = datagram.data()[i*rawDataSize+k];
            }
        }


        if (currentCounter > lastReceiveRawDataCounter)
        {
            lastReceiveRawDataCounter = currentCounter;
            udpDataReceived(data,currentCounter);
        }
    }


}

void Sbs2EmocapDataReader::turnReceiveUdpDataOff()
{
    QObject::disconnect(sbs2NetworkHandler,SIGNAL(rawDataReceived(QUdpSocket*)),this,SLOT(udpDataReceived(QUdpSocket*)));
    if (sbs2NetworkHandler != 0)
        delete sbs2NetworkHandler;
    sbs2NetworkHandler = 0;


    if (readOnlyFromNetwork)
    {

    }
    else
    {
        QObject::connect(sbs2EmocapMounter,SIGNAL(deviceFound(QMap<QString,QVariant>)),this,SLOT(deviceFound(QMap<QString,QVariant>)));
        QObject::connect(sbs2EmocapMounter,SIGNAL(deviceLost()),this,SLOT(deviceLost()));
        sbs2EmocapMounter->start();
    }
}

void Sbs2EmocapDataReader::turnReceiveUdpDataOn(QString address, int port)
{
    Sbs2DataReader::turnReceiveUdpDataOn(address, port);

    if (sbs2EmocapMounter != 0)
    {
        QObject::disconnect(sbs2EmocapMounter,SIGNAL(deviceFound(QMap<QString,QVariant>)),this,SLOT(deviceFound(QMap<QString,QVariant>)));
        QObject::disconnect(sbs2EmocapMounter,SIGNAL(deviceLost()),this,SLOT(deviceLost()));
        sbs2EmocapMounter->invalidate();

    }

    running = 0;
    if (sbs2NetworkHandler != 0)
        delete sbs2NetworkHandler;

    sbs2NetworkHandler = new Sbs2NetworkHandler(this);
    sbs2NetworkHandler->turnReceiveRawDataOn(address,port);
    QObject::connect(sbs2NetworkHandler,SIGNAL(rawDataReceived(QUdpSocket*)),this,SLOT(udpDataReceived(QUdpSocket*)));
}


void Sbs2EmocapDataReader::resetHandle()
{
#ifdef Q_OS_MAC
    hid_close(handle);
    hid_exit();
    handle = hid_open(0x1234, 0xed02, NULL);
#endif
}
