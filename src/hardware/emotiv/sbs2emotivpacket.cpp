#include <hardware/emotiv/sbs2emotivpacket.h>

Sbs2EmotivPacket::Sbs2EmotivPacket(QObject *parent):Sbs2Packet(parent)
{

}

void Sbs2EmotivPacket::update(char *data)
{
    for (int i=0; i<Sbs2Common::rawDataSize(); ++i)
    {
	rawData[i] = data[i];
    }
    counter = Sbs2Common::normalize((int)data[0]);

    gyroX = Sbs2Common::normalize((int)data[29]) << 4 & 0xfff0 + (data[31]>>4 & 0x0f);
    gyroY = Sbs2Common::normalize((int)data[30]) << 4 & 0xfff0 + (data[31] & 0x0f);

    cq = -1;
    cqIndex = -1;
    cqName = "";

    battery = -1;
    if(counter>127)
    {
	battery = counter & 127;
	counter = 128;
    }
    else
    {
	int index = counter % 64;
	if(index < 14)
	{

	    int level = 0;
	    int b=0;
	    int o=0;
	    int m=14;
	    for (int i = 13; i>-1; --i)
	    {
		level = level << 1;
		b = (*Sbs2Common::getChannels())[(*Sbs2Common::getChannelNames())[m]][i] / 8 + 1;
		o = (*Sbs2Common::getChannels())[(*Sbs2Common::getChannelNames())[m]][i] % 8;
		level = level | (Sbs2Common::normalize((int)data[b]) >> o  & 1 );

	    }

	    cq = level;
	    cqIndex = index;
	    cqName = Sbs2Common::getCqsMapping()->at(index);

	    // blck < 81, Red < 221, Orange < 314, Yellow < 407, Green >= 407
	}
    }

    for (int m=0; m<Sbs2Common::getChannelNames()->size() - 1; ++m)
    {

	int level = 0;
	int b=0;
	int o=0;
	for (int i = 13; i>-1; --i)
	{
	    level = level << 1;
	    b = (*Sbs2Common::getChannels())[(*Sbs2Common::getChannelNames())[m]][i] / 8 + 1;
	    o = (*Sbs2Common::getChannels())[(*Sbs2Common::getChannelNames())[m]][i] % 8;
	    level = level | (Sbs2Common::normalize((int)data[b]) >> o  & 1 );

	}

	values[Sbs2Common::getChannelNames()->at(m)] = level;
	filteredValues[Sbs2Common::getChannelNames()->at(m)] = level;
    }

}

