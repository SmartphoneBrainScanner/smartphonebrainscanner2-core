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

#ifndef SBS2SPECTROGRAM_H
#define SBS2SPECTROGRAM_H

#include <QObject>
#include <FFTReal.h>
#include <dtu_array_2d.h>


#define PI 3.14159265

class Sbs2Spectrogram : public QObject
{
    Q_OBJECT
    Q_ENUMS(WindowType)
public:
    enum WindowType {RECT, HANN, HAMMING};
    explicit Sbs2Spectrogram(int length_, QObject *parent = 0);
    void doSpectrogram(DTU::DtuArray2D<double>* input, DTU::DtuArray2D<double>* output);
    ~Sbs2Spectrogram();

    WindowType getWindowType();

private:
    int length;
    FFTReal* fft;
    flt_t* x;
    flt_t* f;
    WindowType windowType;



private:
    double window(int n);


signals:
    


public slots:
    void setWindowType(Sbs2Spectrogram::WindowType windowType_);
    
};

#endif // SBS2SPECTROGRAM_H
