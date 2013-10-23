#ifndef SBS2FAKEPACKET_H
#define SBS2FAKEPACKET_H

#include <hardware/sbs2packet.h>

class Sbs2FakePacket : public Sbs2Packet
{
    Q_OBJECT
public:
    Sbs2FakePacket(QObject* parent);
    void update(double *data);
};

#endif // SBS2FAKEPACKET_H
