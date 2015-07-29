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

#ifndef SBS2DUMMYPCA_H
#define SBS2DUMMYPCA_H


#include <QObject>
#include <QFile>
#include <QStringList>

#include "sbs2common.h"
#include "dtu_array_2d.h"


/**
    @class DummyPca
    \brief Principal component temporal filter.
 */

class Sbs2DummyPca : public QObject
{
    Q_OBJECT

public:
    Sbs2DummyPca(int channels_, int blockSize_ = 64, QObject *parent = 0);
    ~Sbs2DummyPca();

    void doPca(DTU::DtuArray2D<double>* values, DTU::DtuArray2D<double>* returnValues);
    void turnOn() { on = true; }
    void turnOff() { on = false; }
    bool isOn() { return on; }

private:
     int channels; // number of channels
     int blockSize;
     bool on;

     DTU::DtuArray2D<double>* dataDeque;
     int dataDequeStart;
     int dataDequeEnd;

signals:
    
public slots:
};

#endif // SBS2DUMMYPCA_H


