#include <sbs2callback.h>
#include <sbs2datahandler.h>

int Sbs2Callback::currentPacketCounter = 0;
int Sbs2Callback::currentPacket = 0;

Sbs2Callback::Sbs2Callback(QObject *parent):QObject(parent)
{
    sbs2DataHandler = new Sbs2DataHandler();
    samplesCollected = 0;
    currentPacket = 0;
    sbs2Region = 0;
    isRecording = 0;
    devicePresent = 0;
}

void Sbs2Callback::startRecording(QString user, QString description)
{
    isRecording = 1;
    sbs2DataHandler->startRecording(user,description);
}

void Sbs2Callback::stopRecording()
{
    isRecording = 0;
    sbs2DataHandler->stopRecording();
}

void Sbs2Callback::insertIntoMetaFile(QString event)
{
    sbs2DataHandler->insertIntoMetaFile(event);
}

void Sbs2Callback::turnFilterOff()
{
    sbs2DataHandler->turnFilterOff();
}

void Sbs2Callback::turnFilterOn(int fbandLow_, int fbandHigh_, int filterOrder_)
{
    sbs2DataHandler->turnFilterOn(fbandLow_,fbandHigh_,filterOrder_);
}

QString Sbs2Callback::getRawFilename()
{
    return sbs2DataHandler->getRawFilename();
}

void Sbs2Callback::turnChannelSpectrogramOff()
{
    sbs2DataHandler->turnChannelSpectrogramOff();
}

void Sbs2Callback::turnChannelSpectrogramOn(int spectrogramChannelSamples_, int spectrogramChannelLength_, int spectrogramChannelDelta_)
{
    sbs2DataHandler->turnChannelSpectrogramOn(spectrogramChannelSamples_,spectrogramChannelLength_, spectrogramChannelDelta_);
}

void Sbs2Callback::setWindowType(Sbs2Spectrogram::WindowType windowType)
{
    sbs2DataHandler->setWindowType(windowType);
}
void Sbs2Callback::setWindowType(int windowType)
{
    sbs2DataHandler->setWindowType((Sbs2Spectrogram::WindowType)windowType);
}

void Sbs2Callback::turnSourceReconstructionOff()
{
    sbs2DataHandler->turnSourceReconstructionOff();
}

void Sbs2Callback::turnSourceReconstructionPowerOff()
{
    sbs2DataHandler->turnSourceReconstructionPowerOff();
}

void Sbs2Callback::turnSourceReconstructionOn(int sourceReconstructionSamples_, int sourceReconstructionDelta_, int sourceReconstructionModelUpdateLength_, int sourceReconstructionModelUpdateDelta_, QString hardware)
{

    sbs2DataHandler->turnSourceReconstructionOn(sourceReconstructionSamples_,sourceReconstructionDelta_,sourceReconstructionModelUpdateLength_,sourceReconstructionModelUpdateDelta_, hardware);
    if (!(sbs2Region == 0))
	sbs2DataHandler->setVerticesToExtract(sbs2Region->getVerticesToExtract());
}

void Sbs2Callback::turnSourceReconstructionPowerOn(int sourceReconstructionSamples_, int sourceReconstructionDelta_, int sourceReconstructionModelUpdateLength_, int sourceReconstructionModelUpdateDelta_, QString hardware)
{
    sbs2DataHandler->turnSourceReconstructionPowerOn(sourceReconstructionSamples_,sourceReconstructionDelta_,sourceReconstructionModelUpdateLength_,sourceReconstructionModelUpdateDelta_, hardware);
    if (!(sbs2Region == 0))
	sbs2DataHandler->setVerticesToExtract(sbs2Region->getVerticesToExtract());
}

void Sbs2Callback::turnSendRawDataOn(QString rawDataServerAddress_, int rawDataPort_, int rawDataSize_, int rawDataQueueLength_)
{
    sbs2DataHandler->turnSendRawDataOn(rawDataServerAddress_,rawDataPort_,rawDataSize_,rawDataQueueLength_);
}

void Sbs2Callback::turnSendRawDataOff()
{
    sbs2DataHandler->turnSendRawDataOff();
}

void Sbs2Callback::addRawDataHost(QString address, int port)
{
    sbs2DataHandler->addRawDataHost(address,port);
}

void Sbs2Callback::removeRawDataHost(QString address, int port)
{
    sbs2DataHandler->removeRawDataHost(address,port);
}


void Sbs2Callback::sendMessage(QString message)
{
    sbs2DataHandler->sendMessage(message);
}

void Sbs2Callback::sendMessage(QString message, QString address, int port)
{
    sbs2DataHandler->sendMessage(message,address,port);
}

void Sbs2Callback::addMessageUdpOutputHost(QString address, int port)
{
    sbs2DataHandler->addMessageUdpOutputHost(address,port);
}

void Sbs2Callback::removeMessageUdpOutputHost(QString address)
{
    sbs2DataHandler->removeMessageUdpOutputHost(address);
}

void Sbs2Callback::clearMessageUdpOutputHosts()
{
    sbs2DataHandler->clearMessageUdpOutputHosts();
}

void Sbs2Callback::turnReceiveMessageOn(QString address, int port)
{
    sbs2DataHandler->turnReceiveMessageOn(address,port);
    QObject::connect(sbs2DataHandler,SIGNAL(udpMessageReceived(QString,QString,int)),this,SLOT(readMessage(QString,QString,int)));
}

void Sbs2Callback::turnReceiveMessageOff()
{
    sbs2DataHandler->turnReceiveMessageOff();
      QObject::disconnect(sbs2DataHandler,SIGNAL(udpMessageReceived(QString,QString,int)),this,SLOT(readMessage(QString,QString,int)));
}

void Sbs2Callback::readMessage(QString data, QString sender, int senderPort)
{

    emit udpMessageReceived(data,sender,senderPort);
}

/**
    used for setting custom data handlers
*/
void Sbs2Callback::setSbs2DataHandler(Sbs2DataHandler *sbs2DataHandler_)
{
    if (!(sbs2DataHandler == 0))
	delete sbs2DataHandler;

    sbs2DataHandler = sbs2DataHandler_;
}

void Sbs2Callback::setHardware(QString hardware)
{
    Sbs2Common::setHardware(hardware);
    emit hardwareChanged(hardware);
}


int Sbs2Callback::getCurrentPacket()
{
    return currentPacket;
}

int Sbs2Callback::getCurrentPacketCounter()
{
    return currentPacketCounter;
}


void Sbs2Callback::spectrogramUpdatedSlot()
{
    emit spectrogramUpdated();
}

void Sbs2Callback::getNetworkAddresses()
{
    QList<QVariant> interfaces;

    foreach(QNetworkInterface v, QNetworkInterface::allInterfaces())
    {
	if (v.flags().testFlag(QNetworkInterface::IsRunning))
	{

	    foreach(QNetworkAddressEntry v2, v.addressEntries())
	    {
		if (v2.ip().protocol() == 0)
		    interfaces.append(QString(v.name()).append("&").append(v2.ip().toString()));
	    }

	}

    }

    emit networkAddresses(interfaces);

}

void Sbs2Callback::deviceFound(QMap<QString, QVariant> params_)
{
    params = params_;
    devicePresent = 1;
    emit deviceFoundSignal(params);
}
