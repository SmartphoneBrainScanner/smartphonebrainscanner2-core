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

#ifndef SBS2COMMON_H
#define SBS2COMMON_H

#include <QMap>
#include <QVector>
#include <QString>
#include <QDebug>
#include <QString>
#include <qplatformdefs.h>
#include <QDir>

/**
  @todo Loading hardware configuration from a file.
  */


#define DEPLOYMENT 0 //sets paths for deployment and packaging; remember to clean project

class Sbs2Common
{


public:
    static QMap<QString, QVector<int> > *getChannels();
    static QVector<QString>* getChannelNames();
    static QMap<QString,int>* getCqs();
    static QVector<QString>* getCqsMapping();
    static int normalize(int value);
    static QString setRootAppPath(QString rootAppPath_);
    static QString getRootAppPath();
    static QString setCatalogPath(QString catalogPath_);
    static QString getCatalogPath();
    static QString setDefaultRootAppPath();
    static QString setDefaultCatalogPath();
    static int channelsNo();
    static int samplingRate();
    static int verticesNo();
    static void setHardware(QString hardware_);
    static QString getCurrentHardware();
    static int rawDataSize();

private:
    Sbs2Common();
    static Sbs2Common* New();
    void generateHardware();
    static void setHardware(QString hardware_, Sbs2Common* me);
    void generateEmotiv();
    void generateEmocap();
    void generateEmocap28();


private:
    int channelsNumber;
    int samplingRateValue;
    int verticesNumber;
    static Sbs2Common* m_pInstance;
    QMap<QString,int> cqs;
    QVector<QString> cqsMapping;
    QMap<QString, QVector<int> > channels;
    QVector<QString> channelNames;
    QString rootAppPath;
    QString catalogPath;
    static QString hardware;
    static int hardwareSet;
    int rawDataSizeVar;

};



#endif // SBS2COMMON_H
