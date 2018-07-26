#include <hardware/emotiv/sbs2emotivmounter.h>
#include "QFile"

Sbs2EmotivMounter* Sbs2EmotivMounter::m_pInstance = 0;

/* Handle to hid_device, used in OSX for querying serial number and reading the data. */
#if defined(Q_OS_MAC) || defined(Q_OS_WIN)
hid_device* Sbs2EmotivMounter::handle = 0;
#endif

Sbs2EmotivMounter* Sbs2EmotivMounter::New(QObject *parent)
{
    if (!m_pInstance)
	m_pInstance = new Sbs2EmotivMounter(parent);
    return m_pInstance;
}

Sbs2EmotivMounter::Sbs2EmotivMounter(QObject *parent)
{
    init();
}

Sbs2EmotivMounter::~Sbs2EmotivMounter()
{
    umount();
}

void Sbs2EmotivMounter::start()
{
    mount();
    readHardwareParameters();
}

void Sbs2EmotivMounter::stop()
{
    umount();
}

void Sbs2EmotivMounter::invalidate()
{
    /* Simply set first character to X which is not sane. */
    serialNumber[0] = 'X';
}

void Sbs2EmotivMounter::init()
{
    serialNumber = "";
    path = "";
}

void Sbs2EmotivMounter::mount()
{
    mountedHardware = "emotiv";

#if defined(Q_OS_ANDROID)
    /* On Android we set the permissions on the device to 777 so our application doesn't have to be run as root. This is not the niceset way, but for now there doesn't seem to be any better one. */
    system("su -c 'chmod 777 /dev/hidraw*'");
#endif
    /* For OSX use hid_open to access the device. */
#if defined(Q_OS_MAC) || defined(Q_OS_WIN)
    struct hid_device_info *devs, *cur_dev;
    int found = 0;
    wchar_t* _serial_number = 0;
    found = 0;
    devs = hid_enumerate(0x1234, 0xed02);
    cur_dev = devs;
    while (cur_dev) {
        if (found == 0) {
            _serial_number = (wchar_t*)malloc(60*sizeof(wchar_t));
            wcscpy(_serial_number,cur_dev->serial_number);
            found++;
        } else {
            if (wcscmp(cur_dev->serial_number,_serial_number) == 0) {
                if (cur_dev->interface_number == 1) {
                    handle = hid_open_path(cur_dev->path);
                    //handle = hid_open(0x1234,0xed02,cur_dev->serial_number);
                    found++;
                    break;
                }
            }
        }
        cur_dev = cur_dev->next;
    }
    if (!handle) {
        qDebug() << "unable to open device";
    }
#endif
}

void Sbs2EmotivMounter::umount()
{
    //nothing to do
}

QString Sbs2EmotivMounter::readSerialNumber()
{
    QString tempSerialNumber = QString();

#if defined(Q_OS_ANDROID)
    QFileInfoList list = QDir("/sys/class/hidraw/").entryInfoList();
    QRegExp typeReg(".{4}:.{4}:.{4}\\..{4}");
    //QRegExp interfaceReg("\\d-\\d\\.\\d:\\d\\.\\d");
    QRegExp interfaceReg("\\d-\\d:\\d\\.\\d");


    foreach (QFileInfo file, list)
    {
	QString device = file.baseName();
	QString symlink = file.symLinkTarget();
	if (device != "")
	{
	    foreach (QString v, symlink.split("/"))
	    {
		if (typeReg.exactMatch(v))
		{
		    if (v.split(":").at(1) == "1234" && v.split(":").at(2).left(4) == "ED02")
		    {
			{
			    foreach (QString v2, symlink.split("/"))
			    {

				if (interfaceReg.exactMatch(v2))
				{

				    if (v2.right(1) == "1")
				    {

					QFile file(symlink+"/device/uevent");

					if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
					    continue;
					QTextStream in(&file);
					int i = 0;
					while(i < 100)
					{
					    QString line = in.readLine();
					    if (line.split("=").at(0) == "HID_UNIQ")
					    {
						tempSerialNumber = line.split("=").at(1).left(16);
						return tempSerialNumber;
					    }
					    ++i;
					}
				    }
				}
			    }
			}
		    }
		}
	    }
	}
    }
#elif defined (Q_OS_LINUX)
    struct hid_device_info *devs, *cur_dev;

    devs = hid_enumerate(0x0, 0x0);
    cur_dev = devs;
    while (cur_dev)
    {

	if (cur_dev->vendor_id == 0x1234 && cur_dev->product_id == 0xed02) //1234 ed02
	{
	    if (cur_dev->interface_number == 1)
	    {
		if (path == "")
		{
		    char* s1 = new char[16];
		    wcstombs(s1,cur_dev->serial_number,16);
		    tempSerialNumber = QString(s1).left(16);
		    delete s1;
		    return tempSerialNumber;
		}
	    }
	}

	cur_dev = cur_dev->next;
    }
    hid_free_enumeration(devs);

#elif defined(Q_OS_MAC) || defined(Q_OS_WIN)
    wchar_t wstr[17];
    char tmpSerial[17];
    tmpSerial[16]=0;

    if (!handle)
    {
	qDebug("unable to open device\n");
	return tempSerialNumber;
    }
    wstr[0] = 0x0000;
    /* Get serial number. */
    hid_get_serial_number_string(handle, wstr, sizeof(wstr));
    /* Copy serial number to our temporary container. */
    for (int i=0; i<16; ++i)
    {
	tmpSerial[i] = (char)wstr[i];
    }
    tempSerialNumber = QString(tmpSerial);
    return tempSerialNumber;
#endif
}

void Sbs2EmotivMounter::readHardwareParameters()
{
    serialNumber = readSerialNumber();
#if defined(Q_OS_ANDROID)
    path = QString("/dev/hidraw1");
    QFile file(path);
    if(file.exists()){
        qDebug() << "[ANDROID] hidraw device found :  " << path << " with serial number : " << serialNumber;
    }else{
        qDebug() << "[ANDROID] hidraw device not found at expected place : " << path;
    }
#elif defined (Q_OS_LINUX)
    struct hid_device_info *devs, *cur_dev;

    devs = hid_enumerate(0x0, 0x0);
    cur_dev = devs;
    while (cur_dev)
    {

	if (cur_dev->vendor_id == 0x1234 && cur_dev->product_id == 0xed02) //1234 ed02
	{
	    if (cur_dev->interface_number == 1)
	    {
		if (path == "")
		{
		    path = cur_dev->path;
		}
	    }
	}

	cur_dev = cur_dev->next;
    }
    hid_free_enumeration(devs);

    qDebug() << "[LINUX] hidraw device found:  " << serialNumber << path;
#elif defined (Q_OS_WIN)
    path = QString("/dev/null");
    qDebug() << "[WINDOWS] hidraw device found:  " << serialNumber;
#elif defined(Q_OS_MAC)
    path = QString("/dev/null");
    qDebug() << "[OSX] hidraw device found:  " << serialNumber;
#endif
    if (serialNumber.isEmpty() || !serialNumber.startsWith("SN"))
    {
		invalidate();
		emit deviceLost();
		return;
    }
    identifier = serialNumber;
    QMap<QString, QVariant> params = QMap<QString, QVariant>();
    params["serialNumber"] = serialNumber;
    params["path"] = path;

    emit deviceFound(params);
}
