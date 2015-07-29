#include "sbs2dummypca.h"


Sbs2DummyPca::Sbs2DummyPca(int channels_, int blockSize_, QObject *parent): QObject(parent), channels(channels_)
{
    on = false;
    channels = channels_;
    blockSize = blockSize_;

    // Initialize ring buffer for input data
    dataDeque = new DTU::DtuArray2D<double>(2 * blockSize, channels);
    for (int j=0; j<dataDeque->dim1(); j++)
        for (int k=0; k<dataDeque->dim2(); k++)
            (*dataDeque)[j][k] = 0;
    dataDequeStart = 0;
    dataDequeEnd = blockSize;

    qDebug() << "sbs2dummypca.cpp: Sbs2DummyPca::Sbs2DummyPca: on =" << on << ", channels =" << channels;
}


void Sbs2DummyPca::doPca(DTU::DtuArray2D<double>* values, DTU::DtuArray2D<double>* returnValues)
{
    assert(values->dim1() == 1);
    assert(returnValues->dim1() == 1);
    assert(values->dim2() == returnValues->dim2());

    // Insert new data point in ring buffer
    int dataDequeNewEnd = (dataDequeEnd + 1) % blockSize;
    for (int k=0; k < dataDeque->dim2(); k++)
        (*dataDeque)[dataDequeNewEnd][k] = (*values)[0][k];

    // qDebug() << "sbs2dummypca.cpp: Sbs2DummyPca::doPca: on = " << (*dataDeque)[0][0];

    if (on)
    {
        // Do the actual processing
        for (int j=0; j<values->dim1(); j++)
        {
            for (int k=0; k<values->dim2(); k++)
            {
                // TODO: Dummy processing at the moment
                (*returnValues)[j][k] = 3 * (*values)[j][k];
            }
        }
    }
    else
    {
        // Bypass processing: Just copy input values to output
        for (int j=0; j<values->dim1(); j++)
            for (int k=0; k<values->dim2(); k++)
               (*returnValues)[j][k] = (*values)[j][k];
    }

    // Update ringbuffer indices
    dataDequeStart = (dataDequeStart + 1) % blockSize;
    dataDequeEnd = dataDequeNewEnd;
}


Sbs2DummyPca::~Sbs2DummyPca()
{
    delete dataDeque;
}
