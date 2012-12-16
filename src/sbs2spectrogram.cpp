#include "sbs2spectrogram.h"


Sbs2Spectrogram::Sbs2Spectrogram(int length_, QObject *parent) :
    QObject(parent), length(length_)
{
    setWindowType(RECT);
    fft = new FFTReal(length);
    x = new flt_t [length];
    f = new flt_t [length];
}

Sbs2Spectrogram::~Sbs2Spectrogram()
{
    delete fft;
    delete x;
    delete f;
}

void Sbs2Spectrogram::doSpectrogram(DTU::DtuArray2D<double> *input, DTU::DtuArray2D<double> *output)
{

    if (input->dim1() != input->dim1())
        return;
    if (output->dim2() != length)
        return;

    for (int row = 0; row < input->dim1(); ++row)
    {

        for (int column = 0; column < length; ++column)
        {
            if (column < input->dim2())
                x[column] = (*input)[row][column] * window(column);
            else
                x[column] = 0;
        }


        fft->do_fft(f,x);
        for (int column = 0; column < length; ++column)
            (*output)[row][column] = f[column];



    }

}


double Sbs2Spectrogram::window(int n)
{
    if (windowType == RECT)
    {
        return 1.0;
    }
    else if (windowType == HANN)//periodic, use length-1 for symmetric
    {
        return 0.5*(1-std::cos(2*PI*n/(length)));
    }
    else if (windowType == HAMMING)//periodic, use length-1 for symmetric
    {
        return 0.54 - 0.46*std::cos((2*PI*n)/(length));
    }
}

void Sbs2Spectrogram::setWindowType(Sbs2Spectrogram::WindowType windowType_)
{
    windowType = windowType_;
}

Sbs2Spectrogram::WindowType Sbs2Spectrogram::getWindowType()
{
    return windowType;
}
