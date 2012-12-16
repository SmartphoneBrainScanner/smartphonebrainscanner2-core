#include <hardware/emocap28/sbs2emocap28packet.h>

Sbs2Emocap28Packet::Sbs2Emocap28Packet(QObject *parent):Sbs2Packet(parent)
{

    counter1 = 0;
    counter2 = 0;
}

void Sbs2Emocap28Packet::update(char *data)
{
    for (int i=0; i<Sbs2Common::rawDataSize(); ++i)
    {
        rawData[i] = data[i];
    }
    counter = Sbs2Common::normalize((int)data[0]);
    gyroX = ((int)data[29]<<4 & 0xfff0) + (data[31]>>4 & 0x0f);
    gyroY = ((int)data[30]<<4 & 0xfff0) + (data[31] & 0x0f);

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

void Sbs2Emocap28Packet::update(char *data1, char *data2)
{

    for (int i=0; i<Sbs2Common::rawDataSize()/2; ++i)
    {
        rawData[i] = data1[i];
        rawData[i+Sbs2Common::rawDataSize()/2] = data2[i];

    }
    counter1 = Sbs2Common::normalize((int)data1[0]);
    counter2 = Sbs2Common::normalize((int)data2[0]);

    if(counter1>127)
    {
        battery1 = counter1 & 127;
        counter1 = 128;
    }
    if(counter2>127)
    {
        battery2 = counter2 & 127;
        counter2 = 128;
    }

    gyroX1 = Sbs2Common::normalize((int)data1[29]) <<4 & 0xfff0 + (data1[31]>>4 & 0x0f);
    gyroY1 = Sbs2Common::normalize((int)data1[30]) << 4 & 0xfff0 + (data1[31] & 0x0f);
    gyroX2 = Sbs2Common::normalize((int)data2[29]) <<4 & 0xfff0 + (data2[31]>>4 & 0x0f);
    gyroY2 = Sbs2Common::normalize((int)data2[30]) << 4 & 0xfff0 + (data2[31] & 0x0f);

    for (int m=0; m<14; ++m)
    {

        int level = 0;
        int b=0;
        int o=0;
        for (int i = 13; i>-1; --i)
        {
            level = level << 1;
            b = (*Sbs2Common::getChannels())[(*Sbs2Common::getChannelNames())[m]][i] / 8 + 1;
            o = (*Sbs2Common::getChannels())[(*Sbs2Common::getChannelNames())[m]][i] % 8;
            level = level | (Sbs2Common::normalize((int)data1[b]) >> o  & 1 );

        }

        values[Sbs2Common::getChannelNames()->at(m)] = level;
        filteredValues[Sbs2Common::getChannelNames()->at(m)] = level;
    }

    for (int m=14; m<28; ++m)
    {

        int level = 0;
        int b=0;
        int o=0;
        for (int i = 13; i>-1; --i)
        {
            level = level << 1;
            b = (*Sbs2Common::getChannels())[(*Sbs2Common::getChannelNames())[m]][i] / 8 + 1;
            o = (*Sbs2Common::getChannels())[(*Sbs2Common::getChannelNames())[m]][i] % 8;
            level = level | (Sbs2Common::normalize((int)data2[b]) >> o  & 1 );

        }
        values[Sbs2Common::getChannelNames()->at(m)] = level;
        filteredValues[Sbs2Common::getChannelNames()->at(m)] = level;
    }

    //qDebug() << counter1 << counter2 << values["O2"] << values["O2_2"];

}

int Sbs2Emocap28Packet::getCounter(char *data)
{
    update(data);
    return counter;
}

int Sbs2Emocap28Packet::getValue(char *data)
{
    update(data);
    return values["O2"];
}
