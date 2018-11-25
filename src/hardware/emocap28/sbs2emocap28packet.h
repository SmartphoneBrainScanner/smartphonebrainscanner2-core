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

#ifndef SBS2EMOCAP28PACKET_H
#define SBS2EMOCAP28PACKET_H

#include <hardware/sbs2packet.h>

class Sbs2Emocap28Packet: public Sbs2Packet
{
    Q_OBJECT

public:
    Sbs2Emocap28Packet(QObject *parent);
    void update(const char *data) override;
    void update(char *data1, char* data2);
    int getCounter(char* data);
    int getValue(char* data);
private:
    int counter1;
    int counter2;
    int gyroX1;
    int gyroX2;
    int gyroY1;
    int gyroY2;

    int battery1;
    int battery2;

};

#endif // SBS2EMOCAP28PACKET_H
