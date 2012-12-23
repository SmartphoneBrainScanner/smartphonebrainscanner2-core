#include "mycallback.h"

MyCallback::MyCallback(QObject *parent) :
    Sbs2Callback(parent)
{
}

void MyCallback::getData(Sbs2Packet *packet)
{
    setPacket(packet);
}
