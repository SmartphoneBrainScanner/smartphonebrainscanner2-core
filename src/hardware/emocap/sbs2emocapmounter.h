/**
* Smartphone Brain Scanner 2 License Agreement (MIT License)
*
* Copyright (c) 2012 Arkadiusz Stopczynski, Jakob Eg Larsen, Carsten Stahlhut, Michael Kai Petersen, Lars Kai Hansen.
* Technical University of Denmark, DTU Informatics, Cognitive Systems Section. http://code.google.com/p/smartphonebrainscanner2
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
* modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom
* the Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
* LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef SBS2EMOCAPMOUNTER_H
#define SBS2EMOCAPMOUNTER_H

#include <hardware/sbs2hardwaremounter.h>

/** On OSX we use hidapi to access raw data.*/
#ifdef Q_OS_MAC
#include <platform/osx/hidapi.h>
#endif

#ifdef Q_OS_LINUX
#include <platform/linux/hidapi.h>
#endif

class Sbs2EmocapMounter: public Sbs2HardwareMounter
{
    Q_OBJECT

public:
    static Sbs2EmocapMounter* New(QObject* parent = 0);
    ~Sbs2EmocapMounter();
#ifdef Q_OS_MAC
    static hid_device* getHandle() {return handle;};
#endif

private:
    Sbs2EmocapMounter(QObject* parent = 0);
    void init();
    void mount();
    void umount();
    void readHardwareParameters();
    QString readSerialNumber();

private:
    static Sbs2EmocapMounter* m_pInstance;
#ifdef Q_OS_MAC
    static hid_device *handle;
#endif
#ifdef Q_OS_LINUX
    static hid_device *handle;
#endif
    QString serialNumber;
    QString path;

public slots:
    void start();
    void stop();
    void invalidate();
};

#endif // SBS2EMOCAPMOUNTER_H
