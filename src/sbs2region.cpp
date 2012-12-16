#include "sbs2region.h"

Sbs2Region::Sbs2Region(QObject *parent) :
    QObject(parent)
{
    loadRegionsNames();
    loadRegionsList();
    loadVerticesList();
}

void Sbs2Region::loadRegionsNames()
{
    QString filename;
    filename.append(Sbs2Common::getRootAppPath());
    filename.append("List_NamesOfRegions.txt");


    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
	qDebug() << "ERROR: file problem" << filename;
	return;
    }

    int i=1;
    while (!file.atEnd())
    {
	QByteArray line = file.readLine();
	QString str = line.data();
	str.chop(1); //get rid of the trailing \n
	regionsNames.insert(i,str);
	regionsIndices.insert(str,i);
	++i;
    }
}

void Sbs2Region::loadRegionsList()
{
    QString filename;
    filename.append(Sbs2Common::getRootAppPath());
    filename.append("List_regions.txt");


    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
	qDebug() << "ERROR: file problem" << filename;
	return;
    }

    int i=1;
    while (!file.atEnd())
    {
	QByteArray line = file.readLine();
	QString str = line.data();
	QStringList list1 = str.split(",");
	QVector<int> vertices;


	for (int j = 1; j < list1.size(); j++)
	{
	    vertices.append(list1.at(j).toInt());
	}

	regionsList.insert(list1.at(0).toInt(),vertices);
	++i;
    }
}

void Sbs2Region::loadVerticesList()
{
    QString filename;
    filename.append(Sbs2Common::getRootAppPath());
    filename.append("List_vertices.txt");


    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
	qDebug() << "ERROR: file problem" << filename;
	return;
    }

    int i=1;
    while (!file.atEnd())
    {
	QByteArray line = file.readLine();
	QString str = line.data();
	QStringList list1 = str.split(",");
	QVector<int> regions;


	for (int j = 1; j < list1.size(); j++)
	{
	    regions.append(list1.at(j).toInt());
	}

	verticesList.insert(list1.at(0).toInt(),regions);
	++i;
    }

}


void Sbs2Region::clearVerticesToExtract()
{
    verticesToExtract.clear();
}



void Sbs2Region::addRegion(QString region)
{

    if (!(regionsIndices.contains(region)))
	return;


    foreach (int vertex,regionsList[regionsIndices[region]])
	if (!(verticesToExtract.contains(vertex-1)))
	    verticesToExtract.append(vertex-1);

    qSort(verticesToExtract);

    QVector<int> vertices;
    foreach (int vertex,regionsList[regionsIndices[region]])
	if (!vertices.contains(vertex-1))
	    vertices.append(vertex-1);

    regionsToExtract.append(vertices);

}

void Sbs2Region::addRegionsIntersection(QString region1, QString region2)
{
    if (!(regionsIndices.contains(region1)))
	return;
    if (!(regionsIndices.contains(region2)))
	return;



    foreach (int vertex,regionsList[regionsIndices[region1]])
	if (!(verticesToExtract.contains(vertex-1)))
	    if (regionsList[regionsIndices[region2]].contains(vertex-1))
		verticesToExtract.append(vertex-1);

    foreach (int vertex,regionsList[regionsIndices[region2]])
	if (!(verticesToExtract.contains(vertex-1)))
	    if (regionsList[regionsIndices[region1]].contains(vertex-1))
		verticesToExtract.append(vertex-1);

    qSort(verticesToExtract);

    QVector<int> vertices;


    foreach (int vertex,regionsList[regionsIndices[region1]])
	if (!(vertices.contains(vertex-1)))
	    if (regionsList[regionsIndices[region2]].contains(vertex-1))
		vertices.append(vertex-1);

    foreach (int vertex,regionsList[regionsIndices[region2]])
	if (!(vertices.contains(vertex-1)))
	    if (regionsList[regionsIndices[region1]].contains(vertex-1))
		vertices.append(vertex-1);

    regionsToExtract.append(vertices);

}

QVector<int>* Sbs2Region::getVerticesToExtract()
{
    return (&verticesToExtract);
}

QVector<QVector<int> >* Sbs2Region::getRegionsToExtract()
{
    return (&regionsToExtract);
}
