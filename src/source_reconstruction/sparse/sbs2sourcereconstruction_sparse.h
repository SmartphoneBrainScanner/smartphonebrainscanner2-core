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

#ifndef SPARSERECONSTRUCTION_H
#define SPARSERECONSTRUCTION_H

#include <QObject>
#include<dtu_array_2d.h>
#include<vector>
#include<QString>
#include<source_reconstruction/sparse/math_utilities.h>
#include <QVector>
#include <sbs2spectrogram.h>


class Sbs2SourceReconstructionSparse : public QObject
{
    Q_OBJECT
public:

    explicit Sbs2SourceReconstructionSparse(int channels_input, int sources_input, int samples_, QVector<double> lambdas_, QString hardware_, QObject *parent = 0, int numDatosTrain_input = 8, int numDatosTest_input = 6, double error_tol_ = 0.0001);

    void f_objective_general_group_lasso(DTU::DtuArray2D<double> *A_normalized, DTU::DtuArray2D<double> *S, DTU::DtuArray2D<double> *Y, double lambda, double *out);

    void derivative_square_loss_frobenius(DTU::DtuArray2D<double> *A, DTU::DtuArray2D<double> *Y, DTU::DtuArray2D<double> *S, DTU::DtuArray2D<double> *out);

    void proximal_operator_standard_group_lasso(DTU::DtuArray2D<double> *X, double *regularizer_factor, DTU::DtuArray2D<double> *out);

    void fista_method_group_lasso_v2(DTU::DtuArray2D<double> *A_normalized, DTU::DtuArray2D<double> *Y, double lambda, double L, DTU::DtuArray2D<double> *estimated_S);

    void cross_validation_k_channel(DTU::DtuArray2D<double> *Y_mean_0,DTU::DtuArray2D<double> *estimated_S);

    void rootMeanSquareError(DTU::DtuArray2D<double> *Y_mean_0_test, DTU::DtuArray2D<double> *A_normalized_test, DTU::DtuArray2D<double> *estimated_S, double *rmse);

    void doRec(DTU::DtuArray2D<double> *Y_input, DTU::DtuArray2D<double> *S_output, int *isSourceReconstructionReady);

    void doRecPow(DTU::DtuArray2D<double> *Y_input, DTU::DtuArray2D<double> *S_output, int *isSourceReconstructionReady);

    void preprocessData();
    void calculateMean(DTU::DtuArray2D<double> *input, DTU::DtuArray2D<double> *output);
    void calculatePower(DTU::DtuArray2D<double> *input, DTU::DtuArray2D<double> *output);


private:

    DTU::DtuArray2D<double> *A_normalized;
    DTU::DtuArray2D<double> *S_temp;
    double L;

    DTU::DtuArray2D<double> *Y;

    DTU::DtuArray2D<double> *Y_meanMatrix;
    DTU::DtuArray2D<double> *S;

    int channels;
    int sources;
    int samples;

    int numDatosTrain;
    int numDatosTest;

    QString hardware;
    QVector<double> lambdas;
    double error_tol;

    Sbs2Spectrogram* sbs2Spectrogram;

    /*Variables used in the function f_objective_general_group_lasso*/
    DTU::DtuArray2D<double> *A_S;//A_S = A*S
    DTU::DtuArray2D<double> *A_S_minus_Y ;//A_S_minus_Y = A*S-Y

    /*Variables used in the function derivative_square_loss_frobenius*/
    DTU::DtuArray2D<double> *A_transpose;//A_transpose = A'

    /*Variables used in the function proximal_operator_standard_group_lasso*/
    vector<double> norm_2_each_row;
    vector<double> ones_vector;
    DTU::DtuArray2D<double> * thresholdingMatrix;

    /**Variables used in the function fista_method_group_lasso_v2**/
    DTU::DtuArray2D<double> *S_ant;
    DTU::DtuArray2D<double> *V;
    DTU::DtuArray2D<double> *grad_f_y;
    DTU::DtuArray2D<double> *S_act;
    DTU::DtuArray2D<double> *one_over_L_grad_f_y;
    DTU::DtuArray2D<double> *V_minus_one_over_L_grad_f_y;
    DTU::DtuArray2D<double> *S_act_minus_S_ant;
    DTU::DtuArray2D<double> *scaled_S_act_minus_S_ant;

signals:

public slots:

};

#endif // SPARSERECONSTRUCTION_H
