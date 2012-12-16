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


#ifndef SBS2HARDWAREMOUNTER_H
#define SBS2HARDWAREMOUNTER_H

#include <QObject>
#include <QTimer>
#include <QtCore>
#include <sbs2common.h>
#include <utils/waiter.h>
#include <QString>

class Sbs2HardwareMounter: public QObject
{
    Q_OBJECT

public:
    ~Sbs2HardwareMounter();
    static QString getIdentifier();

protected:
    Sbs2HardwareMounter(QObject* parent = 0);
    void mySleep();

    virtual void init();
    virtual void mount();
    virtual void umount();
    virtual void readHardwareParameters();

protected:
    static QString mountedHardware;
    static QString identifier;

signals:
    void deviceFound(QMap<QString, QVariant> params);
    void deviceLost();

public slots:
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void invalidate() = 0;

};

#endif // SBS2HARDWAREMOUNTER_H
