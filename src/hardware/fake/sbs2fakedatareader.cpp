#include "sbs2fakedatareader.h"
#include <QtCore>

Sbs2FakeDataReader* Sbs2FakeDataReader::m_pInstance = 0;

Sbs2FakeDataReader* Sbs2FakeDataReader::New(Sbs2Callback *sbs2Callback_, int readOnlyFromNetwork_, QObject *parent)
{
    if (!m_pInstance)
        m_pInstance = new Sbs2FakeDataReader(sbs2Callback_, readOnlyFromNetwork_, parent);

    return m_pInstance;
}

Sbs2FakeDataReader::Sbs2FakeDataReader(Sbs2Callback *sbs2Callback_, int readOnlyFromNetwork_, QObject *parent):Sbs2DataReader(sbs2Callback_,readOnlyFromNetwork_,parent)
{
    filename = "";
    file = 0;
    buffer = 0;

    for (int i=0; i< bufferSize; ++i)
    {
        sbs2Packets.append(new Sbs2FakePacket(this));
    }
}

void Sbs2FakeDataReader::execute()
{
    int ms = 1000.0/Sbs2Common::samplingRate();
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    QString line ("");
    QStringList values;

    int loop = 0;

    while (running)
    {
        ++framesRead;

        // read line from file
        line = file->readLine();
        values = line.split(",");

        for(int ch = 0; ch < Sbs2Common::channelsNo(); ch++)
        {
            buffer[ch] = values.at(ch).toDouble();
        }

        sbs2Packets[currentIndex]->update(buffer);
        sbs2Callback->getData(sbs2Packets[currentIndex]);
        currentIndex = (currentIndex+1) % 256;

        nanosleep(&ts, NULL);

        if (loop)
        {
            // reset file after 256
            if (framesRead % 256 == 0)
            {
                file->seek(0);
                // discard first line
                file->readLine();
            }
        }

        // loop file
        if (file->atEnd())
        {
            file->seek(0);
            // discard first line
            file->readLine();
        }
    }

    qDebug() << "Sbs2FakeDataReader. Out of loop - stopped...";
}

void Sbs2FakeDataReader::start()
{
    if (filename.length() == 0)
    {
        qDebug() << "No data file!";
        return;
    }

    if (file == 0)
        file = new QFile(Sbs2Common::getRootAppPath() + filename);

    if (!file->open(QIODevice::ReadOnly))
    {
        qDebug() << "Error opening data file:" << Sbs2Common::getRootAppPath() + filename;
        return;
    }

    qDebug() << "File:" << Sbs2Common::getRootAppPath() + filename << "opened!";

    QString line = file->readLine('\n');

    // MRA TODO: Implement check for no. of channels
    if (buffer == 0)
        buffer = new double[Sbs2Common::channelsNo()];

    running = 1;
    QtConcurrent::run(this,&Sbs2FakeDataReader::execute);
}

Sbs2FakeDataReader::~Sbs2FakeDataReader()
{
    m_pInstance = 0;

    // MRA TODO: clean up dynamic allocated memory

    if (file != 0)
    {
        if (file->isOpen())
            file->close();

        delete file;
        file = 0;
    }
}
