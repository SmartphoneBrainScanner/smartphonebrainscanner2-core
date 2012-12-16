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

#ifndef SBS2REGION_H
#define SBS2REGION_H


#include <QObject>
#include <sbs2common.h>
#include <QMap>
#include <QFile>
#include <QString>
#include <QStringList>
#include <dtu_array_2d.h>
#include <QtAlgorithms>

class Sbs2Region : public QObject
{
    Q_OBJECT
public:
    explicit Sbs2Region(QObject *parent = 0);
    QVector<int>* getVerticesToExtract();
    QVector<QVector<int> >* getRegionsToExtract();

private:
    QMap<int, QString> regionsNames;	//lookup of region name by its index
    QMap<QString, int> regionsIndices;	//lookup of region index by its name
    QMap <int, QVector <int> > regionsList; //region:vertices
    QMap <int, QVector <int> > verticesList;	//vertex:regions

    QVector<int> verticesToExtract;
    QVector<QVector<int> > regionsToExtract;

private:
    void loadRegionsNames(); //human readable names of the regions
    void loadRegionsList(); //region:vertices
    void loadVerticesList();	//vertex:regions

signals:


public slots:
    void clearVerticesToExtract();
    void addRegion(QString region);
    void addRegionsIntersection(QString region1, QString region2);

};

#endif // SBS2REGION_H
