#include "sbs2filter.h"


Sbs2Filter::Sbs2Filter(int fbandLow_, int fbandHigh_, int order_, QObject *parent):
    QObject(parent),fbandLow(fbandLow_), fbandHigh(fbandHigh_), order(order_)
{
    on = false;
    hcoef2 = new DTU::DtuArray2D<double>(1, order+1);
    loadFilter();
}


void Sbs2Filter::loadFilter()
{
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
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "ERROR: file problem for " << filename;
        return;
    }

    int i=0;
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        QString str = line.data();
        QStringList list1 = str.split("\t");
        for (int j = 0; j < list1.size(); j++) {
            (*hcoef2)[0][j] = list1.at(j).toDouble();
        }
        i++;
    }
}


void Sbs2Filter::updateFilter(int fbandLow_, int fbandHigh_)
{
    assert(fbandLow_ < fbandHigh_);

    fbandHigh = fbandHigh_;
    fbandLow = fbandLow_;
    loadFilter();
}


void Sbs2Filter::doFilter(DTU::DtuArray2D<double>* values, DTU::DtuArray2D<double>* returnValues)
{
    assert(values->dim1() == returnValues->dim1());
    assert(values->dim2() == (order + 1));  // Columns of the values

    if (on) {
        // FIR filter convolution
        double yn;

        for (int j=0; j<values->dim1(); j++) {
            yn = 0.0;
            for (int k=0; k<values->dim2(); k++)
                yn += (*hcoef2)[0][k] * ((*values)[j][k]);

            (*returnValues)[j][0] = yn;
        }
    } else {
        // Bypass the filter
        for (int j=0; j<values->dim1(); j++)
            (*returnValues)[j][0] = (*values)[j][0];
    }
}


Sbs2Filter::~Sbs2Filter()
{
    delete hcoef2;
}
