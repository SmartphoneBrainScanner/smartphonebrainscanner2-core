/**
* Smartphone Brain Scanner 2 License Agreement (MIT License)
*
* Copyright (c) 2012 Arkadiusz Stopczynski, Jakob Eg Larsen, Carsten Stahlhut, Michael Kai Petersen, Lars Kai Hansen.
* Technical University of Denmark, DTU Informatics, Cognitive Systems Section. http://code.google.com/p/smartphonebrainscanner2
* 
* Copyright (c) 2018 Technical University of Denmark.
* Author: Sune Vuorela <sune@vuorela.dk>
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

#ifndef SBS2FILEDATAREADER_H
#define SBS2FILEDATAREADER_H

#include <hardware/sbs2datareader.h>
#include "sbs2filepacket.h"

class Sbs2FileDataReader: public Sbs2DataReader
{
    Q_OBJECT

public:
    Sbs2FileDataReader(Sbs2Callback* sbs2Callback_, const QString& filename, QObject *parent = nullptr);
    ~Sbs2FileDataReader();

private:
    void execute();

private:
    QFile rawFile;
    QVector<Sbs2FilePacket*> sbs2Packets;

public slots:
    void deviceFound(QMap<QString, QVariant> params);
    void deviceLost();
    void aboutToQuit();

};

#endif // SBS2EMOTIVDATAREADER_H
