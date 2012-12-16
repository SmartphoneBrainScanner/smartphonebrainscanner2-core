#include "sbs2packet.h"

Sbs2Packet::Sbs2Packet(QObject *parent) :
    QObject(parent)
{
    counter = gyroX = gyroY = 0;
    cq = cqIndex = 0;
    battery = 0;
    rawData = new char[Sbs2Common::rawDataSize()];

    for (int m=0; m<Sbs2Common::getChannelNames()->size(); ++m)
    {
        values[Sbs2Common::getChannelNames()->at(m)] = 0;
        filteredValues[Sbs2Common::getChannelNames()->at(m)] = 0;
    }
}

void Sbs2Packet::update(char* data) {}
