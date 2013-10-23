#ifndef SBS2FAKEDATAREADER_H
#define SBS2FAKEDATAREADER_H

#include <hardware/sbs2datareader.h>
#include <hardware/fake/sbs2fakepacket.h>

class Sbs2FakeDataReader : public Sbs2DataReader
{
    Q_OBJECT
public:
    static Sbs2FakeDataReader* New(Sbs2Callback* sbs2Callback_, int readOnlyFromNetwork_ = 0, QObject *parent = 0);
    ~Sbs2FakeDataReader();
    
signals:
    
public slots:
    void start();
    void stop() { running = 0; }
    void setFilename(QString filename_) { filename = filename_; }

private:
    Sbs2FakeDataReader(Sbs2Callback* sbs2Callback_, int readOnlyFromNetwork_ = 0, QObject *parent = 0);
    void execute();

    static Sbs2FakeDataReader* m_pInstance;
    QVector<Sbs2FakePacket*> sbs2Packets;
    QString filename;
    QFile* file;
    double* buffer;
};

#endif // SBS2FAKEDATAREADER_H
