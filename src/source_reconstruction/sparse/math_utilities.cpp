#include<source_reconstruction/sparse/math_utilities.h>
#include<cmath>

#include<iostream>
#include<cstdio>


#include<QStringList>
#include<QFile>
#include<QTextStream>

#include<QDebug>


void getMean(DTU::DtuArray2D<double> *matrix, DTU::DtuArray2D<double> *matrixMean)
{

    int num_columns = matrix->dim2();
    int num_rows = matrix->dim1();

    vector<double> columnMean(num_columns);

    double column_mean;



    for(int column=0 ; column<matrix->dim2() ; column++)
    {
	column_mean = 0;

	for(int row=0 ; row<matrix->dim1() ; row++)
	{

	    column_mean += (*matrix)[row][column];

	}

	columnMean[column] = column_mean / num_rows;

    }

    vector<double> ones_vector(num_rows,1);

    vectorOuterProduct(ones_vector,columnMean,matrixMean);

}


void loadData(QString pathFile, DTU::DtuArray2D<double> *matrix)
{


    QFile fh(pathFile);

    if(!fh.open(QIODevice::ReadOnly|QIODevice::Text))
    {
	std::cout<<"Error while reading the file."<<std::endl;
	return;
    }

    QTextStream in(&fh);
    QString mydata=in.readAll();
    QStringList rows = mydata.split("\n");
    QStringList columns;

    int i=0;


    foreach(QString data, rows)
    {
	int j=0;
	columns = data.split("\t");

	foreach(QString col,columns)
	{
	   (*matrix)[i][j]=col.toDouble();

	    j++;
	}

	i++;

    }

    fh.close();

}


void loadData(QString pathFile, double *scalar)
{

    QFile fh(pathFile);

    if(!fh.open(QIODevice::ReadOnly|QIODevice::Text))
    {
	std::cout<<"Error while reading the file."<<std::endl;
	return;
    }

    QTextStream in(&fh);
    QString mydata=in.readAll();

    (*scalar) = mydata.toDouble();

    fh.close();

}





void printMatrix(DTU::DtuArray2D<double> *matrix)
{
    for(int i=0 ; i<matrix->dim1() ; i++)
    {
	for(int j=0 ; j<matrix->dim2() ; j++)
	{
	    printf("%.6f ",(*matrix)[i][j]);

	}
	std::cout<<"\n";
    }

}

void  copyMatrix(DTU::DtuArray2D<double> *matrix_source , DTU::DtuArray2D<double> *matrix_destiny)
{
    for(int i=0 ; i<matrix_source->dim1() ; i++)
    {
	for(int j=0 ; j<matrix_source->dim2() ; j++)
	{
	    (*matrix_destiny)[i][j] = (*matrix_source)[i][j];
	}

    }
}

void matrixMultiplicationComponentWise(DTU::DtuArray2D<double> *matrix_A, DTU::DtuArray2D<double> *matrix_B, DTU::DtuArray2D<double> *out)
{
    for(int i=0 ; i<matrix_A->dim1() ; i++)
    {
	for(int j=0 ; j<matrix_B->dim2() ; j++)
	{
	    (*out)[i][j] = ( (*matrix_A)[i][j] ) * ( (*matrix_B)[i][j] );
	}

    }

}

void vectorOuterProduct(vector<double> &vector1 , vector<double> &vector2, DTU::DtuArray2D<double> *out)
{
    for(int i=0 ; i<vector1.size() ; i++)
    {
	for(int j=0 ; j<vector2.size() ; j++)
	{
	    (*out)[i][j] = vector1[i]*vector2[j];
	}

    }

}

void thresholding_insitu(vector<double> & x_vector)
{
    for(int i=0 ; i<x_vector.size() ; i++)
    {
	if(x_vector[i]<0)
	{
	    x_vector[i]=0;
	}
    }

}

void scalarMinusVector_insitu(double *scalar,vector<double> &x_vector)
{
    for(int i=0 ; i<x_vector.size() ; i++)
    {
	x_vector[i] = (*scalar) - x_vector[i];
    }

}

void scalarDividedbyVectorComponentWise_insitu(double *scalar,vector<double> &x_vector)
{

    for(int i=0 ; i<x_vector.size() ; i++)
    {
	x_vector[i] = (*scalar)/x_vector[i];

    }

}

void printVector(vector <double> &out)
{

    for(int i=0 ; i<out.size() ; i++)
    {
	std::cout<<out[i]<<std::endl;

    }

    std::cout<<"\n\n";

}


void matrixL21NormEachRow(DTU::DtuArray2D<double> *matrix, vector<double> &out)
{
    double row_sum;
    for(int i=0 ; i < matrix->dim1() ; i++)
    {
	row_sum = 0;

	for(int j=0 ; j < matrix->dim2() ; j++)
	{
	    row_sum += pow((*matrix)[i][j], 2);

	}
	out[i] = sqrt(row_sum);
    }
}

void matrixL21Norm(DTU::DtuArray2D<double> *matrix, double *out)
{

    double row_sum;
    double column_sum = 0;

    double row_norm;

    for(int i=0 ; i < matrix->dim1() ; i++)
    {
	row_sum = 0;

	for(int j=0 ; j < matrix->dim2() ; j++)
	{
	    row_sum += pow((*matrix)[i][j], 2);

	}

	row_norm = sqrt(row_sum);

	column_sum += row_norm;


    }

    (*out) = column_sum;

}




void matrixFrobNorm(DTU::DtuArray2D<double> *matrix, double *out)
{
    double temp = 0;

    for(int i=0 ; i<matrix->dim1() ; i++)
    {
	for(int j=0 ; j<matrix->dim2() ; j++)
	{
	    temp += pow((*matrix)[i][j],2);
	}

    }

    (*out) = sqrt(temp);

}
