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

#ifndef SBS2FILEHANDLER_H
#define SBS2FILEHANDLER_H

#include <QObject>
#include <sbs2common.h>
#include <fstream>
#include <QFile>
#include <stdlib.h>
#include <QDateTime>
#include <hardware/sbs2hardwaremounter.h>
#include <sbs2callback.h>


class Sbs2FileHandler : public QObject
{
    Q_OBJECT
public:
    static Sbs2FileHandler* New(QObject* parent = 0);
    void dumpRawData(char* rawData);
    ~Sbs2FileHandler();
    QString getRawFilename();
    int getPacketZero();

private:
    QString user;
    QString description;
    QString rawFilename;
    QString metaFilename;
    std::ofstream rawFile;
    QDateTime* currentTime;
    QFile* metaFile;
    int packetZero;

private:
    QString normalizeInt(int data);
    static Sbs2FileHandler* m_pInstance;
    Sbs2FileHandler(QObject *parent = 0);

signals:
    
public slots:
    void insertIntoMetaFile(QString event);
    void close();
    void createMetaFile(QString user_, QString description_);
    
};

#endif // SBS2FILEHANDLER_H
