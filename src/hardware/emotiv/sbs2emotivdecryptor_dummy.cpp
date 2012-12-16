#include "sbs2emotivdecryptor.h"

Sbs2EmotivDecryptor::Sbs2EmotivDecryptor(QObject *parent) :
    QObject(parent)
{
    oRijndael = 0;
    serialNumber = new char[16];
    currentPacket = 0;

}

void Sbs2EmotivDecryptor::setSerialNumber(char *serialNumber_)
{
    for (int i=0; i<16; ++i)
    {
        serialNumber[i] = serialNumber_[i];
    }

    initialize();
}

void Sbs2EmotivDecryptor::setSerialNumber(QString serialNumber_)
{
    for (int i=0; i<16; ++i)
    {
        serialNumber[i] = serialNumber_.at(i).toAscii();
    }
    initialize();
}

void Sbs2EmotivDecryptor::initialize() {}

void Sbs2EmotivDecryptor::decrypt(char cipher[32], char plain[32])
{


    plain[0] = currentPacket;
    plain[1] = qrand()%RAND_MAX*10;
    plain[2] = qrand()%RAND_MAX*10;
    plain[3] = qrand()%RAND_MAX*10;
    plain[4] = qrand()%RAND_MAX*10;
    plain[5] = qrand()%RAND_MAX*10;
    plain[6] = qrand()%RAND_MAX*10;
    plain[7] = qrand()%RAND_MAX*10;
    plain[8] = qrand()%RAND_MAX*10;
    plain[9] = qrand()%RAND_MAX*10;
    plain[10] = qrand()%RAND_MAX*10;
    plain[11] = qrand()%RAND_MAX*10;
    plain[12] = qrand()%RAND_MAX*10;
    plain[13] = qrand()%RAND_MAX*10;
    plain[14] = qrand()%RAND_MAX*10;
    plain[15] = qrand()%RAND_MAX*10;
    plain[16] = qrand()%RAND_MAX*10;
    plain[17] = qrand()%RAND_MAX*10;
    plain[18] = qrand()%RAND_MAX*10;
    plain[19] = qrand()%RAND_MAX*10;
    plain[20] = qrand()%RAND_MAX*10;
    plain[21] = qrand()%RAND_MAX*10;
    plain[22] = qrand()%RAND_MAX*10;
    plain[23] = qrand()%RAND_MAX*10;
    plain[24] = qrand()%RAND_MAX*10;
    plain[25] = qrand()%RAND_MAX*10;
    plain[26] = qrand()%RAND_MAX*10;
    plain[27] = qrand()%RAND_MAX*10;
    plain[28] = qrand()%RAND_MAX*10;
    plain[29] = qrand()%RAND_MAX*10;
    plain[30] = qrand()%RAND_MAX*10;
    plain[31] = qrand()%RAND_MAX*10;


    currentPacket = (currentPacket+1)%129;


}
