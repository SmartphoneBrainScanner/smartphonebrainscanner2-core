#include "sbs2filehandler.h"


Sbs2FileHandler* Sbs2FileHandler::m_pInstance = 0;

Sbs2FileHandler::Sbs2FileHandler(QObject *parent) :
    QObject(parent)
{

    if (!QFile::exists(Sbs2Common::getCatalogPath()))
	system(QString("mkdir ").append(Sbs2Common::getCatalogPath()).toStdString().c_str());

    packetZero = -1;
    metaFile = 0;

}

void Sbs2FileHandler::createMetaFile(QString user_, QString description_)
{
    user = user_;
    description = description_;

    currentTime = new QDateTime(QDateTime::currentDateTime ());
    qint64 timestamp = currentTime->toMSecsSinceEpoch(); //in msec and UTC (-2 hours)


    int year = currentTime->date().year();
    int month = currentTime->date().month();
    int day = currentTime->date().day();
    int hour = currentTime->time().hour();
    int minute = currentTime->time().minute();
    int sec = currentTime->time().second();

    rawFilename = QString(Sbs2Common::getCatalogPath());
    rawFilename.append("sbs2data_");
    rawFilename.append(QString::number(year));
    rawFilename.append("_");
    rawFilename.append(normalizeInt(month));
    rawFilename.append("_");
    rawFilename.append(normalizeInt(day));
    rawFilename.append("_");
    rawFilename.append(normalizeInt(hour));
    rawFilename.append("_");
    rawFilename.append(normalizeInt(minute));
    rawFilename.append("_");
    rawFilename.append(normalizeInt(sec));
    rawFilename.append("_");
    rawFilename.append(user);

    delete currentTime;

    qDebug() << rawFilename << " " <<timestamp;

    metaFilename = rawFilename + ".meta";

    metaFile = new QFile(metaFilename);

    if (!metaFile->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "problem creating meta file";
        return;
    }

    rawFile.open((rawFilename + ".raw").toStdString().c_str(), std::ios::out | std::ios::binary);
    int encrypted = 0;
    metaFile->write(QString(user+":"+description+":"+QString::number(timestamp)+":"+QString::number(encrypted)+":"+Sbs2HardwareMounter::getIdentifier()+"\n").toStdString().c_str());
    metaFile->flush();

}

void Sbs2FileHandler::insertIntoMetaFile(QString event)
{
    if (metaFile == 0)
	return;

    if (!metaFile->isOpen())
        return;

    //This prevents any data to be inserted into META file before the EEG recording starts
    //if (packetZero == -1)
      //  return;

    QString packetNumber = QString::number(Sbs2Callback::getCurrentPacket() - packetZero + 1);
    metaFile->write(QString(event+":"+QString::number(QDateTime::currentMSecsSinceEpoch())+":"+packetNumber+"\n").toStdString().c_str());
    metaFile->flush();
}

void Sbs2FileHandler::dumpRawData(char *rawData)
{
    if (!metaFile->isOpen())
        return;

    if (packetZero == -1)
    {
	packetZero = Sbs2Callback::getCurrentPacket();
        insertIntoMetaFile("START");
    }

    rawFile.write(rawData,Sbs2Common::rawDataSize());
    rawFile.flush();

}

QString Sbs2FileHandler::normalizeInt(int data)
{
    if (data>9)
        return QString::number(data);
    else
        return QString("0")+QString::number(data);
}

Sbs2FileHandler::~Sbs2FileHandler()
{
    close();
    m_pInstance = 0;
}

Sbs2FileHandler* Sbs2FileHandler::New(QObject *parent)
{
    if (!m_pInstance)
        m_pInstance = new Sbs2FileHandler(parent);

    return m_pInstance;
}

void Sbs2FileHandler::close()
{
    rawFile.close();
    metaFile->close();
    rawFilename.clear();
    metaFilename.clear();
    packetZero = -1;
    user.clear();
    description.clear();
}

QString Sbs2FileHandler::getRawFilename()
{
    return rawFilename;
}

int Sbs2FileHandler::getPacketZero()
{
    return packetZero;
}
