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

#ifndef SBS2PACKET_H
#define SBS2PACKET_H

#include <QObject>
#include <QMap>
#include <sbs2common.h>


/**
  @todo Meta data as QMap.
  */

/**
  @class Class encapsulating EEG and metadata (such as counters, signal quality, gyro). Certain number of empty packets is constructed in the buffer and then continuosly updated.
  */


class Sbs2Packet : public QObject
{
    Q_OBJECT

public:

    /** @brief Constructor
	    Constructs empty packet.

	    @param parent Pointer to teh parent QObject object.
	    */

    explicit Sbs2Packet(QObject *parent = 0);

    /** @brief Method for updating data in the packet.
	    To avoid continuous creation and destruction of objects, certain number of empty packets is constructed in initialization and then updated with wrap-around. Packets should see the raw data delieverd by Sbs2DataReader and form themselves.

	    @param data Pointer to raw data.
	    */

    void virtual update(char* data);

private:


public:
    int counter, gyroX, gyroY;
    int cq;
    int cqIndex;
    QString cqName;
    QMap<QString, double> values;
    QMap<QString, double> filteredValues;
    int battery;
    char* rawData;


signals:
    
public slots:
    
};

#endif // SBS2PACKET_H
