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

#ifndef MATH_UTILITIES_H
#define MATH_UTILITIES_H

#include<dtu_array_2d.h>
#include<vector>
#include<QString>


void getMean(DTU::DtuArray2D<double> *matrix, DTU::DtuArray2D<double> *matrixMean);

void loadData(QString pathFile, DTU::DtuArray2D<double> *matrix);

void loadData(QString pathFile, double *scalar);

void matrixL21Norm(DTU::DtuArray2D<double> *matrix, double *out);

void matrixFrobNorm(DTU::DtuArray2D<double> *matrix, double *out);

void matrixL21NormEachRow(DTU::DtuArray2D<double> *matrix, vector<double> &out_vector);

void printVector(vector <double> &out_vector);

void printMatrix(DTU::DtuArray2D<double> *matrix);

void scalarDividedbyVectorComponentWise_insitu(double *scalar, vector<double> &x_vector);

void scalarMinusVector_insitu(double *scalar,vector<double> &x_vector);

void thresholding_insitu(vector<double> &x_vector);

void vectorOuterProduct(vector<double> &vector1 , vector<double> &vector2, DTU::DtuArray2D<double> *out);

void matrixMultiplicationComponentWise(DTU::DtuArray2D<double> *matrix_A, DTU::DtuArray2D<double> *matrix_B, DTU::DtuArray2D<double> *out);

void  copyMatrix(DTU::DtuArray2D<double> *matrix_source , DTU::DtuArray2D<double> *matrix_destiny);

#endif // MATH_UTILITIES_H
