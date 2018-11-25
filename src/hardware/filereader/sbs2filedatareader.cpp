#include "sbs2filedatareader.h"
#include <QtConcurrent/QtConcurrent>

Sbs2FileDataReader::Sbs2FileDataReader(Sbs2Callback *sbs2Callback_, const QString& filename, QObject *parent)
    : Sbs2DataReader(sbs2Callback_, 0,parent)
{
    rawFile.setFileName(filename);

    for (int i=0; i< bufferSize; ++i)
    {
        sbs2Packets.append(new Sbs2FilePacket(this));
    }
    deviceFound({});
}

Sbs2FileDataReader::~Sbs2FileDataReader()
{
}

void Sbs2FileDataReader::execute()
{
    while(1) {
        if (!running) {
            return;
        }
        if (rawFile.atEnd())
        {
            qDebug() << "restarting file";
            rawFile.seek(0);
        }

        QByteArray buffer = rawFile.read(32);

        ++framesRead;
        sbs2Packets[currentIndex] -> update(buffer);
        sbs2Callback->getData(sbs2Packets[currentIndex]);
        currentIndex = (currentIndex+1) % bufferSize;
        QThread::msleep(6); // Found by trial and error. Probably the amount of time spent in real hardware or in decryptor
    }
}

void Sbs2FileDataReader::deviceFound(QMap<QString, QVariant> params)
{
    Sbs2DataReader::deviceFound(params);

    running = 1;
    rawFile.open(QIODevice::ReadOnly);
    QtConcurrent::run(this,&Sbs2FileDataReader::execute);
}

void Sbs2FileDataReader::deviceLost()
{
    running = 0;
    rawFile.close();
    qDebug() << "device lost";
}

void Sbs2FileDataReader::aboutToQuit()
{
    running = 0;
}
