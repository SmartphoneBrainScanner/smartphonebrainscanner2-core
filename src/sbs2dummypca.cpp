#include "sbs2dummypca.h"


Sbs2DummyPca* Sbs2DummyPca::m_pInstance = 0;


Sbs2DummyPca* Sbs2DummyPca::New(int channels_, QObject *parent)
{
    if (!m_pInstance)
        m_pInstance = new Sbs2DummyPca(channels_, parent);

    return m_pInstance;
}


Sbs2DummyPca::Sbs2DummyPca(int channels_, QObject *parent): QObject(parent), channels(channels_)
{
}


void Sbs2DummyPca::doPca(DTU::DtuArray2D<double>* values, DTU::DtuArray2D<double>* returnValues)
{

    if (values->dim1() != 1)
        return;

    if (values->dim2() != returnValues->dim2()) // columns of the values
        return;

    for (int j=0; j<values->dim1(); j++)
    {
        for (int k=0; k<values->dim2(); k++)
        {
            (*returnValues)[j][k] = 10 * (*values)[j][k];
        }
    }
}


Sbs2DummyPca::~Sbs2DummyPca()
{
    m_pInstance = 0;
}
