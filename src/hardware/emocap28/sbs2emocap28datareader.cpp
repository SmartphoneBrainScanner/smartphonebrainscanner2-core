#include <hardware/emocap28/sbs2emocap28datareader.h>

int mod(int x, int m)
{
    int r = x%m;
    return r<0 ? r+m : r;
}

void Sbs2Emocap28DataContainer::update(char *data_, int counter_)
{
    counter = counter_;
    memcpy(data,data_,32);
}

Sbs2Emocap28DataReader* Sbs2Emocap28DataReader::m_pInstance = 0;

Sbs2Emocap28DataReader* Sbs2Emocap28DataReader::New(Sbs2Callback *sbs2Callback_, int readOnlyFromNetwork_, QObject *parent)
{
    if (!m_pInstance)
	m_pInstance = new Sbs2Emocap28DataReader(sbs2Callback_, readOnlyFromNetwork_, parent);

    return m_pInstance;
}

Sbs2Emocap28DataReader::Sbs2Emocap28DataReader(Sbs2Callback *sbs2Callback_, int readOnlyFromNetwork_, QObject *parent):Sbs2DataReader(sbs2Callback_,readOnlyFromNetwork_,parent)
{


    packetsRead1 = packetsRead2 = 0;
    amp1Found = amp2Found =  0;
    amp1Number = amp2Number = 0;

    inMapping = 0;
    mapped = 0;
    aligning = 0;

    //todo
    testDummyRead = 0;

    path1 = "";
    path2 = "";
    serialNumber1 = "";
    serialNumber2 = "";

    amp1PacketLoss = amp2PacketLoss = 0;
    amp1LastPacket = amp2LastPacket = -1;

    samplesInAligning = 0;

    mapping = -999;
    mappingAlignment = mappingCorr = 0;
    collecting = 0;

    amp1MappingPacketsCollected = amp2MappingPacketsCollected = 0;

    ampBufferSize = 8;
    amp1BufferPosition = amp2BufferPosition = 0;

    for (int i=0; i < ampBufferSize; ++i)
    {
	amp1Buffer.append(new Sbs2Emocap28DataContainer());
	amp2Buffer.append(new Sbs2Emocap28DataContainer());
    }

    for (int i=0; i< bufferSize; ++i)
    {
	sbs2Packets.append(new Sbs2Emocap28Packet(this));
    }

    for (int i=0; i< 256; ++i)
    {
	a1v.append(-1);
	a2v.append(-1);
	a1c.append(-1);
	a2c.append(-1);
    }

    a1value = a2value = -1;
    submittedAmp1Value = -1;

    amp1SinglePacket = new Sbs2Emocap28Packet(this);
    amp2SinglePacket = new Sbs2Emocap28Packet(this);

    if (!readOnlyFromNetwork) //read from the local device
    {
	sbs2EmotivDecryptor1 = new Sbs2EmotivDecryptor(this);
	sbs2EmotivDecryptor2 = new Sbs2EmotivDecryptor(this);
	sbs2Emocap28Mounter = Sbs2Emocap28Mounter::New(this);
	QObject::connect(sbs2Emocap28Mounter,SIGNAL(deviceFound(QMap<QString,QVariant>)),this,SLOT(deviceFound(QMap<QString,QVariant>)));
	QObject::connect(sbs2Emocap28Mounter,SIGNAL(deviceLost()),this,SLOT(deviceLost()));
	sbs2Emocap28Mounter->start();
    }
    else //receive packets from remote host, we do nothing, waiting for turnOn
    {
	sbs2Emocap28Mounter = 0;
    }
}

Sbs2Emocap28DataReader::~Sbs2Emocap28DataReader()
{
    m_pInstance = 0;
}

void Sbs2Emocap28DataReader::amp1(int number)
{
    if (!amp1Found)
    {
	amp1Found = 1;
	amp1Number = number;

	if (amp1Number == 1)
	{
	    qDebug() << "amp1 1" << path1;
	    emit amp1FoundSignal(1, path1, serialNumber1);
	}
	else if (amp1Number == 2)
	{
	    qDebug() << "amp1 2" << path2;
	    emit amp1FoundSignal(2, path2, serialNumber2);
	}

    }
}

void Sbs2Emocap28DataReader::amp2(int number)
{
    if (amp1Found && !amp2Found && number != amp1Number && amp1Number != 0)
    {
	amp2Found = 1;
	amp2Number = number;

	if (amp2Number == 1)
	{
	    qDebug() << "amp2 1" << path1;
	    emit amp2FoundSignal(1, path1, serialNumber1);
	}
	else if (amp2Number == 2)
	{
	    qDebug() << "amp2 2" << path2;
	    emit amp2FoundSignal(2, path2, serialNumber2);
	}



    }
}



void Sbs2Emocap28DataReader::execute1()
{
    int counter = 0;
    while(1)
    {
	if(!running)
	    return;


	rawFile1.read(buffer_main1, 32);

	++packetsRead1;

	if (packetsRead1 == 128)
	{
	    amp1(1);
	    amp2(1);
	}

	sbs2EmotivDecryptor1->decrypt(buffer_main1,buffer_final1);
	collect(buffer_final1, 1);

	if (inMapping && amp1LastPacket == -1)
	    amp1LastPacket = amp1SinglePacket->getCounter(buffer_final1) - 1;
	if (inMapping)
	{

	    amp1CurrentPacketIndex = amp1SinglePacket->getCounter(buffer_final1);
	    amp1PacketLoss += mod((amp1CurrentPacketIndex - amp1LastPacket),129) - 1;
	    amp1LastPacket = amp1CurrentPacketIndex;
	    mappingHist[(mod(amp1CurrentPacketIndex - amp2CurrentPacketIndex,129))] += 1;
	    ++amp1MappingPacketsCollected;


	}
	if (aligning)
	{
	    amp1CurrentPacketIndex = amp1SinglePacket->getCounter(buffer_final1);
	    a1value = amp1SinglePacket->getValue(buffer_final1);
	}
    }
}

bool lessThan(const QVector<double>& s1, const QVector<double>& s2)
{
    return s1.at(0) > s2.at(0);
}

void Sbs2Emocap28DataReader::execute2()
{
    int counter = 0;
    while(1)
    {

	if(!running)
	    return;

	rawFile2.read(buffer_main2, 32);

	++packetsRead2;

	if (packetsRead2 == 128)
	{
	    amp1(2);
	    amp2(2);
	}

	sbs2EmotivDecryptor2->decrypt(buffer_main2,buffer_final2);
	collect(buffer_final2, 2);


	if (amp1Found && amp2Found && packetsRead2 > 160 && !inMapping && !mapped)
	{
	    inMapping = 1;
	}


	if (inMapping && amp2LastPacket == -1)
	    amp2LastPacket = amp2SinglePacket->getCounter(buffer_final2) - 1;

	if (inMapping)
	{
	    amp2CurrentPacketIndex = amp2SinglePacket->getCounter(buffer_final2);
	    amp2PacketLoss += mod((amp2CurrentPacketIndex - amp2LastPacket),129) - 1;
	    amp2LastPacket = amp2CurrentPacketIndex;
	    ++amp2MappingPacketsCollected;
	}

	if (inMapping && amp2MappingPacketsCollected > 850)
	{


	    qDebug() << amp1PacketLoss << amp2PacketLoss << amp1MappingPacketsCollected << amp2MappingPacketsCollected;
	    qDebug() << mappingHist;


	    //TODO: greafecully handle case when we have two competing packet distances
	    //top m1 & m2 sum and m2 > m1*1.2
	    QMapIterator<int, int> i(mappingHist);
	    while (i.hasNext())
	    {
		i.next();
		if (i.value() >= 0.7 * amp1MappingPacketsCollected)
		    mapping = i.key() - 1; //HERE

	    }


	    if (mapping != -999)
	    {
		mapped = 1;
		inMapping = 0;
		aligning = 1;
		emit mappingSuccessful(mapping);
	    }
	    else
	    {
		//emit mappingFailed();
	    }
	}

	if (aligning)
	{
	    amp2CurrentPacketIndex = amp2SinglePacket->getCounter(buffer_final2);
	    a2value = amp2SinglePacket->getValue(buffer_final2);

	    if (a1value == -1 || a2value == -1)
		continue;

	    a1v[samplesInAligning] = a1value;
	    a2v[samplesInAligning] = a2value;
	    a1c[samplesInAligning] = amp1CurrentPacketIndex;
	    a2c[samplesInAligning] = amp2CurrentPacketIndex;

	    ++samplesInAligning;

	    if (samplesInAligning >= 6)
	    {

		QVector<int> a10;
		QVector<int> a11;
		QVector<int> a12;
		QVector<int> a20;
		QVector<int> a21;
		QVector<int> a22;


		for (int i = 0; i < samplesInAligning; ++i)
		{
		    if (i < samplesInAligning - 2)
		    {
			a10.append(a1v[i]);
			a20.append(a2v[i]);
		    }
		    if (i > 0 && i < (samplesInAligning - 1))
		    {
			a11.append(a1v[i]);
			a21.append(a2v[i]);
		    }
		    if (i > 1)
		    {
			a12.append(a1v[i]);
			a22.append(a2v[i]);
		    }
		}



		double r_0 = correlation(&a10,&a20);
		double r_1 = correlation(&a11,&a20);
		double r_2 = correlation(&a12,&a20);
		double r_m1 = correlation(&a10,&a21);
		double r_m2 = correlation(&a10,&a22);







		if (samplesInAligning == 200)
		{
		    aligning = 0;

		    QVector<QVector<double> > corrs;
		    corrs.append(QVector<double>());
		    corrs.last().append(r_0);
		    corrs.last().append(0);
		    corrs.append(QVector<double>());
		    corrs.last().append(r_1);
		    corrs.last().append(1);
		    corrs.append(QVector<double>());
		    corrs.last().append(r_2);
		    corrs.last().append(2);
		    corrs.append(QVector<double>());
		    corrs.last().append(r_m1);
		    corrs.last().append(-1);
		    corrs.append(QVector<double>());
		    corrs.last().append(r_m2);
		    corrs.last().append(-2);

		    qDebug() << corrs;

		    //in descending order due to lessThan implemetation
		    qSort(corrs.begin(), corrs.end(), lessThan);

		    qDebug() <<  corrs;
		    qDebug() << mapping;


		    for (int k = 0; k < corrs.size(); ++k)
		    {
			double r = corrs.at(k).at(0);
			double v = corrs.at(k).at(1);

			mappingAlignment = v;
			mappingCorr = r;


			if (v == 0)
			    break;
			if (v == 1 || v == -1 && r > r_0)
			{
			    mapping += v;
			    break;
			}
			if (v == 2 || v == -2 && r*0.9 > r_0)
			{
			    mapping += v;
			    break;
			}

		    }

		//    qDebug() << a1v << a2v << a1c << a2c;
		    qDebug() <<  mapping << mappingAlignment << mappingCorr;

		    collecting = 1;
		    aligning = 0;
		    emit alignedSignal(mapping, mappingAlignment, mappingCorr);



		}

	    }

	}

    }
}


void Sbs2Emocap28DataReader::collect(char *data, int amp)
{
    if (!collecting)
	return;

    //push packet


    int counter = -1;

    if (amp == 1)
    {
	counter = amp1SinglePacket->getCounter(data);
	amp1Buffer[amp1BufferPosition]->update(data,counter);
	amp1BufferPosition = (amp1BufferPosition+1)%ampBufferSize;

    }
    if (amp == 2)
    {

	counter = amp2SinglePacket->getCounter(data);
	amp2Buffer[amp2BufferPosition]->update(data,counter);
	amp2BufferPosition = (amp2BufferPosition+1)%ampBufferSize;
    }

    for (int i = 0; i<ampBufferSize; ++i)
    {
	if (amp1Buffer[i]->counter != -1)
	{

	    for (int k = 0; k < ampBufferSize; ++k)
	    {
		//qDebug() << "here" << amp2Buffer[k]->counter;
		if (amp2Buffer[k]->counter != -1)
		{



		    if ((amp1Buffer[i]->counter != -1) && (amp2Buffer[k]->counter != -1) && mod((amp1Buffer[i]->counter - amp2Buffer[k]->counter),129) == mapping && amp1Buffer[i]->counter != submittedAmp1Value)
		    {
			//qDebug() << amp1Buffer[i]->counter << amp2Buffer[k]->counter << mapping;

			submittedAmp1Value = amp1Buffer[i]->counter;
			++framesRead;
			sbs2Packets[currentIndex] -> update(amp1Buffer[i]->data, amp2Buffer[k]->data);
			sbs2Callback->getData(sbs2Packets[currentIndex]);
			currentIndex = (currentIndex+1)%256;

			amp1Buffer[i]->counter = -1;
			amp2Buffer[k]->counter = -1;

			break;
		    }
		}
	    }
	}
    }



}

double Sbs2Emocap28DataReader::correlation(QVector<int> *v1, QVector<int> *v2)
{
    if (v1->size() != v2->size())
	return -1;

    double mean1 = 0;
    double mean2 = 0;
    double sum1 = 0;
    double sum2 = 0;

    for (int i = 0; i<v1->size(); ++i)
    {
	sum1 += v1->at(i);
	sum2 += v2->at(i);
    }

    mean1 = sum1/(double)v1->size();
    mean2 = sum2/(double)v2->size();

    double squaredSum1 = 0;
    double squaredSum2 = 0;
    double dSum = 0;

    for (int i = 0; i<v1->size(); ++i)
    {
	dSum += ((v1->at(i) - mean1)*(v2->at(i) - mean2));
	squaredSum1 += pow((v1->at(i)-mean1),2.0);
	squaredSum2 += pow((v2->at(i)-mean2),2.0);

    }
    double t = 1e-20;
    return (dSum/(double)(sqrt(squaredSum1*squaredSum2))+t);

}



void Sbs2Emocap28DataReader::deviceFound(QMap<QString, QVariant> params)
{

    Sbs2DataReader::deviceFound(params);

#ifdef Q_OS_MAC
    handle = Sbs2Emocap28Mounter::getHandle();
#endif
    sbs2EmotivDecryptor1->setSerialNumber(params["serialNumber1"].toString());
    sbs2EmotivDecryptor2->setSerialNumber(params["serialNumber2"].toString());
    path1 = params["path1"].toString();
    path2 = params["path2"].toString();
    serialNumber1 = params["serialNumber1"].toString();
    serialNumber2 = params["serialNumber2"].toString();

    running = 1;
#ifdef Q_OS_MAC

#else
    rawFile1.open(path1.toStdString().c_str(), std::ios::in | std::ios::binary);
    rawFile2.open(path2.toStdString().c_str(), std::ios::in | std::ios::binary);
#endif
    QtConcurrent::run(this,&Sbs2Emocap28DataReader::execute1);
    QtConcurrent::run(this,&Sbs2Emocap28DataReader::execute2);

    emit readyForData();

}

void Sbs2Emocap28DataReader::deviceLost()
{
    running = 0;
    rawFile1.close();
    rawFile2.close();
    qDebug() << "device lost";
}

void Sbs2Emocap28DataReader::aboutToQuit()
{
    running = 0;
    sbs2Emocap28Mounter->stop();
}

void Sbs2Emocap28DataReader::udpDataReceived(QVector<char *> *data, int counter)
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

void Sbs2Emocap28DataReader::udpDataReceived(QUdpSocket *rawDataUdpInputSocket)
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

void Sbs2Emocap28DataReader::turnReceiveUdpDataOff()
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
	QObject::connect(sbs2Emocap28Mounter,SIGNAL(deviceFound(QMap<QString,QVariant>)),this,SLOT(deviceFound(QMap<QString,QVariant>)));
	QObject::connect(sbs2Emocap28Mounter,SIGNAL(deviceLost()),this,SLOT(deviceLost()));
	sbs2Emocap28Mounter->start();
    }
}

void Sbs2Emocap28DataReader::turnReceiveUdpDataOn(QString address, int port)
{
    Sbs2DataReader::turnReceiveUdpDataOn(address, port);

    if (sbs2Emocap28Mounter != 0)
    {
	QObject::disconnect(sbs2Emocap28Mounter,SIGNAL(deviceFound(QMap<QString,QVariant>)),this,SLOT(deviceFound(QMap<QString,QVariant>)));
	QObject::disconnect(sbs2Emocap28Mounter,SIGNAL(deviceLost()),this,SLOT(deviceLost()));
	sbs2Emocap28Mounter->invalidate();

    }

    running = 0;
    if (sbs2NetworkHandler != 0)
	delete sbs2NetworkHandler;

    sbs2NetworkHandler = new Sbs2NetworkHandler(this);
    sbs2NetworkHandler->turnReceiveRawDataOn(address,port);
    QObject::connect(sbs2NetworkHandler,SIGNAL(rawDataReceived(QUdpSocket*)),this,SLOT(udpDataReceived(QUdpSocket*)));
}
