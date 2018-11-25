#include "sbs2filehandler.h"


Sbs2FileHandler* Sbs2FileHandler::m_pInstance = 0;

Sbs2FileHandler::Sbs2FileHandler(QObject *parent) :
    QObject(parent)
{

    if (!QFile::exists(Sbs2Common::getCatalogPath()))
    {
        QDir d;
        d.mkpath(Sbs2Common::getCatalogPath());
    }

    packetZero = -1;
    metaFile = 0;

}

/**
 * @brief Create appropriately named .meta and .raw files and open them for
 * writing.
 *
 * The files will be located in the destination specified by
 * Sbs2Common::getCatalogPath() and will be named after the user string and the
 * current date and time.
 *
 * Additionally the following information will be written in the beginning of the meta
 * file seperated by colon(:) :
 *  - User string (from the argument)
 *  - Description (from the argument)
 *  - Number of milliseconds since unix epoch
 *  - Encryption status (binary: 0 or 1)
 *  - Hardware identifier (From Sbs2HardwareMounter::getIdentifier())
 *
 * @param user_ String to identify the user being recorded.
 * @param description_ Extra description for identifying the recording.
 */
void Sbs2FileHandler::createMetaFile(QString user_, QString description_)
{
    user = user_;
    description = description_;

    QDateTime currentTime = QDateTime::currentDateTime ();
    qint64 timestamp = currentTime.toMSecsSinceEpoch(); //in msec and UTC (-2 hours)


    int year = currentTime.date().year();
    int month = currentTime.date().month();
    int day = currentTime.date().day();
    int hour = currentTime.time().hour();
    int minute = currentTime.time().minute();
    int sec = currentTime.time().second();

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

/**
 * @brief Write event string to meta file.
 *
 * Write event string marked with milliseconds since unix epoch and current packet number to meta file.
 *
 * @param event String to write to the meta file.
 */
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

/**
 * @brief Write data to the raw data file.
 * @param rawData Data to be written.
 */
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

/**
 * @brief Convert integer to normalized string containing exactly 2 digits by
 * prefixing with a 0 if necessary.
 *
 * @param data The integer to be normalized.
 * @return Normalized string representation of data.
 */
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

/**
 * @brief Closes open data streams and resets the state of the Sbs2FileHandler
 * object.
 */
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

/**
 * @brief Get the path to the currently opened data files. (Without
 * .meta or .raw file extensions)
 *
 * Just append '.meta' or '.raw' to get the full path to that file. The
 * returned string will be empty if recording is currently stopped.
 *
 * @return Name of the raw data file.
 */
QString Sbs2FileHandler::getRawFilename()
{
    return rawFilename;
}

/**
 * @brief Get packetZero, which is the packet number of the first data packet
 * in the current recording.
 *
 * @return Packet number for the first packet in recording.
 */
int Sbs2FileHandler::getPacketZero()
{
    return packetZero;
}
