#include "sbs2filter.h"

Sbs2Filter* Sbs2Filter::m_pInstance = 0;

Sbs2Filter* Sbs2Filter::New(int fbandLow_, int fbandHigh_, int order_, QObject *parent)
{
    if (!m_pInstance)
        m_pInstance = new Sbs2Filter(fbandLow_,fbandHigh_,order_,parent);

    return m_pInstance;
}


Sbs2Filter::Sbs2Filter(int fbandLow_, int fbandHigh_, int order_, QObject *parent): QObject(parent),fbandLow(fbandLow_), fbandHigh(fbandHigh_), order(order_)
{
    hcoef2 = 0;
    loadFilter();
}


void Sbs2Filter::loadFilter()
{

    if (!(hcoef2 == 0))
    {
        delete hcoef2;
        hcoef2 = 0;
    }

    hcoef2 = new DTU::DtuArray2D<double>(1,order+1);


    QString filename;
    filename.append(Sbs2Common::getRootAppPath());
    filename.append("filtercoef_fband");
    filename.append(QString::number(fbandLow));
    filename.append("-");
    filename.append(QString::number(fbandHigh));
    filename.append("_norder");
    filename.append(QString::number(order));
    filename.append(".txt");

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "ERROR: file problem for " << filename;
        return;
    }

    int i=0;
    while (!file.atEnd())
    {
        QByteArray line = file.readLine();
        QString str = line.data();
        QStringList list1 = str.split("\t");
        for (int j = 0; j < list1.size(); j++)
        {
            (*hcoef2)[0][j] = list1.at(j).toDouble();

        }
        i++;
    }

}

void Sbs2Filter::updateFilter(int order_, int fbandLow_, int fbandHigh_)
{
    order=order_;
    fbandHigh=fbandHigh_;
    fbandLow=fbandLow_;
    loadFilter();
}

void Sbs2Filter::doFilter(DTU::DtuArray2D<double>* values, DTU::DtuArray2D<double>* returnValues)
{

    if (!(values->dim1() == returnValues->dim1())) //14 for emotiv
        return;

    if (!(values->dim2() == (order +1))) //columns of the values
        return;

    double yn;

    for (int j=0; j<values->dim1(); j++)
    {
        yn = 0.0;
        for (int k=0; k<values->dim2(); k++)
        {
            yn += (*hcoef2)[0][k] * ((*values)[j][k]);
        }
        (*returnValues)[j][0] = yn;
    }
}


Sbs2Filter::~Sbs2Filter()
{
    if (!(hcoef2 == 0))
    {
        delete hcoef2;
        hcoef2 = 0;
    }
    m_pInstance = 0;

}
