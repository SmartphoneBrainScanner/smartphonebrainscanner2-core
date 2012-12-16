#include <hardware/sbs2hardwaremounter.h>



Sbs2HardwareMounter::Sbs2HardwareMounter(QObject *parent): QObject(parent)
{
    mountedHardware = "";
    init();
}

Sbs2HardwareMounter::~Sbs2HardwareMounter()
{
    umount();
}

void Sbs2HardwareMounter::start()
{
    mount();
    readHardwareParameters();
}

void Sbs2HardwareMounter::mySleep()
{
#ifdef Q_WS_MAEMO_5
    usleep(500000);
#else
    Waiter w(5000);
    w.run();
#endif
}

void Sbs2HardwareMounter::init() {}
void Sbs2HardwareMounter::mount() {}
void Sbs2HardwareMounter::umount() {}
void Sbs2HardwareMounter::readHardwareParameters() {}

QString Sbs2HardwareMounter::getIdentifier()
{
    return mountedHardware+"_"+identifier;
}

QString Sbs2HardwareMounter::mountedHardware = "";
QString Sbs2HardwareMounter::identifier = "";
