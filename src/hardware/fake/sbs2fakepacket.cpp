#include "sbs2fakepacket.h"


Sbs2FakePacket::Sbs2FakePacket(QObject *parent):Sbs2Packet(parent)
{
}

void Sbs2FakePacket::update(double *data)
{
  for(int ch = 0; ch < Sbs2Common::channelsNo(); ch++)
    {
      values[Sbs2Common::getChannelNames()->at(ch)] = data[ch];
      filteredValues[Sbs2Common::getChannelNames()->at(ch)] = data[ch];
    }
}

