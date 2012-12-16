#include <hardware/emocap28/sbs2emocap28mounter.h>

Sbs2Emocap28Mounter* Sbs2Emocap28Mounter::m_pInstance = 0;

/* Handle to hid_device, used in OSX for querying serial number and reading the data. */
#ifdef Q_OS_MAC
hid_device* Sbs2Emocap28Mounter::handle = 0;
#endif

Sbs2Emocap28Mounter* Sbs2Emocap28Mounter::New(QObject *parent)
{
    if (!m_pInstance)
	m_pInstance = new Sbs2Emocap28Mounter(parent);
    return m_pInstance;
}

Sbs2Emocap28Mounter::Sbs2Emocap28Mounter(QObject *parent)
{
    init();
}

Sbs2Emocap28Mounter::~Sbs2Emocap28Mounter()
{
    umount();
}

void Sbs2Emocap28Mounter::start()
{
    mount();
    readHardwareParameters();
}

void Sbs2Emocap28Mounter::stop()
{
    umount();
}

void Sbs2Emocap28Mounter::invalidate()
{
    /* Simply set first character to X which is not sane. */
    serialNumber[0] = 'X';
    serialNumber1[0] = 'X';
    serialNumber2[0] = 'X';
}

void Sbs2Emocap28Mounter::init()
{
    serialNumber = new char[16];
    path1 = "";
    path2 = "";
    serialNumber1 = "";
    serialNumber2 = "";
}

void Sbs2Emocap28Mounter::mount()
{
    mountedHardware = "emocap28";

#if defined(Q_OS_ANDROID)
    /* On Android we set the permissions on the device to 777 so our application doesn't have to be run as root. This is not the niceset way, but for now there doesn't seem to be any better one. */
    system("su -c 'chmod 777 /dev/hidraw*'");
#endif
}

void Sbs2Emocap28Mounter::umount()
{
    //nothing to do
}

QString Sbs2Emocap28Mounter::readSerialNumber()
{
    QString baseDirString = "/sys/bus/usb/devices/";
    QDir baseDir = QDir(baseDirString);
    QStringList topList = baseDir.entryList();
    QRegExp usbReg("\\d\\-\\d");
    QRegExp serialNumberReg("SN\\d{14}");
    QString serialNumber = "";

    QStringList usbList;
    foreach (QString name, topList)
    {
	if(name.contains("usb"))
	{
	    usbList.append(name);
	}
    }

    foreach (QString name, usbList)
    {
	QString thisDirString = baseDirString+name;
	QDir thisDir = QDir(thisDirString);
	QStringList thisList = thisDir.entryList();

	foreach (QString name, thisList)
	{
	    if (usbReg.exactMatch(name))
	    {

		QString finalFileSerialString = thisDirString+QString("/")+name+QString("/serial");
		QFile file(finalFileSerialString);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		    continue;

		QTextStream in(&file);

		QString line = in.readLine();

		if (serialNumberReg.exactMatch(line))
		{
		    serialNumber = line;
		    return serialNumber;
		}
	    }
	}
    }
    return serialNumber;
}


void Sbs2Emocap28Mounter::readHardwareParameters()
{
    /* Temporary serial number. */
    char serialNumberNew[17];

#if defined(Q_OS_ANDROID)
    QFileInfoList list = QDir("/sys/class/hidraw/").entryInfoList();
    QRegExp typeReg(".{4}:.{4}:.{4}\\..{4}");
    QRegExp interfaceReg("\\d-\\d\\.\\d:\\d\\.\\d");

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
						if (path1 == "")
						{
						    path1 += "/dev/"+device;
						    serialNumber1 = line.split("=").at(1).left(16);
						    break;
						}
						else
						{
						    path2 += "/dev/"+device;
						    serialNumber2 = line.split("=").at(1).left(16);
						    break;
						}
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

    qDebug() << serialNumber1 << path1 << serialNumber2 << path2;

#elif defined (Q_OS_LINUX)

    struct hid_device_info *devs, *cur_dev;

    devs = hid_enumerate(0x0, 0x0);
    cur_dev = devs;
    while (cur_dev)
    {

	if (cur_dev->vendor_id == 4660 && cur_dev->product_id == 60674) //1234 ed02
	{
	    if (cur_dev->interface_number == 1)
	    {
		if (path1 == "")
		{
		    path1 = cur_dev->path;
		    char* s1 = new char[16];
		    wcstombs(s1,cur_dev->serial_number,16);
		    serialNumber1 = QString(s1).left(16);
		}
		else
		{
		    path2 = cur_dev->path;
		    char* s2 = new char[16];
		    wcstombs(s2,cur_dev->serial_number,16);
		    serialNumber2 = QString(s2).left(16);
		}
	    }
	}

	cur_dev = cur_dev->next;
    }
    hid_free_enumeration(devs);
    qDebug() << serialNumber1 << path1 << serialNumber2 << path2;
#endif



#ifdef Q_OS_MAC
#endif





    if (!(serialNumber1[0] == 'S' && serialNumber1[1] == 'N' && serialNumber2[0] == 'S' && serialNumber2[1] == 'N'))
    {
	invalidate();
	emit deviceLost();
	return;
    }

    identifier = serialNumber1+"_"+serialNumber2;
    QMap<QString, QVariant> params = QMap<QString, QVariant>();
    params["serialNumber1"] = serialNumber1;
    params["serialNumber2"] = serialNumber2;
    params["path1"] = path1;
    params["path2"] = path2;

    emit deviceFound(params);
}
