#include "sbs2sourcereconstruction_sparse.h"

#include<cassert>
#include<iostream>

#include<sbs2common.h>

Sbs2SourceReconstructionSparse::Sbs2SourceReconstructionSparse(int channels_input, int sources_input, int samples_, QVector<double> lambdas_, QString hardware_, QObject *parent, int numDatosTrain_input, int numDatosTest_input, double error_tol_):
    QObject(parent),channels(channels_input),
    sources(sources_input), samples(samples_),
    numDatosTrain(numDatosTrain_input), numDatosTest(numDatosTest_input), hardware(hardware_), error_tol(error_tol_), lambdas(lambdas_)
{

    Y = new DTU::DtuArray2D<double>(channels, samples);//channel matrix
    S = new DTU::DtuArray2D<double>(sources, samples);//sources matrix

    Y_meanMatrix = new DTU::DtuArray2D<double>(channels, samples);//channel matrix


    /*Variables used in the function f_objective_general_group_lasso*/

    A_S = new DTU::DtuArray2D<double>(channels, samples);
    A_S_minus_Y = new DTU::DtuArray2D<double>(channels, samples);

    /*Variables used in the function derivative_square_loss_frobenius*/

    A_transpose = new DTU::DtuArray2D<double>(sources, channels);

    /*Variables used in the function proximal_operator_standard_group_lasso*/

    norm_2_each_row.resize(sources);//contains the L21 of each row
    ones_vector.resize(samples,1);
    thresholdingMatrix = new DTU::DtuArray2D<double>(sources, samples);

    /**Variables used in the function fista_method_group_lasso_v2**/

    S_ant = new DTU::DtuArray2D<double>(sources, samples);
    V = new DTU::DtuArray2D<double>(sources, samples);
    grad_f_y = new DTU::DtuArray2D<double>(sources, samples);
    S_act = new DTU::DtuArray2D<double>(sources, samples);
    one_over_L_grad_f_y = new DTU::DtuArray2D<double>(sources, samples);
    V_minus_one_over_L_grad_f_y = new DTU::DtuArray2D<double>(sources, samples);
    S_act_minus_S_ant = new DTU::DtuArray2D<double>(sources, samples);
    scaled_S_act_minus_S_ant = new DTU::DtuArray2D<double>(sources, samples);
    S_temp = new DTU::DtuArray2D<double>(sources, samples);

    /*** Load all the forward models for training and testing ***/

    A_normalized = new DTU::DtuArray2D<double>(channels,sources);
    /*
    A_normalized_train_1 = new DTU::DtuArray2D<double>(numDatosTrain, sources);
    A_normalized_train_2 = new DTU::DtuArray2D<double>(numDatosTrain, sources);
    A_normalized_train_3 = new DTU::DtuArray2D<double>(numDatosTrain, sources);

    A_normalized_test_1 = new DTU::DtuArray2D<double>(numDatosTest, sources);
    A_normalized_test_2 = new DTU::DtuArray2D<double>(numDatosTest, sources);
    A_normalized_test_3 = new DTU::DtuArray2D<double>(numDatosTest, sources);
*/
    QString pathTrainTestData("/Users/arks/Desktop/SparseReconstruction_SmartPhoneBrainScannerFiles/EmotivIntegration/"); //TODO: fixme
    loadData(pathTrainTestData+"newforwardmodel_spheres_reduced_1028.txt",A_normalized);
    loadData(pathTrainTestData+"L_forwardmodel_sphere_reduced_1028.txt",&L);

    sbs2Spectrogram = new Sbs2Spectrogram(samples,this);
}

void Sbs2SourceReconstructionSparse::f_objective_general_group_lasso(DTU::DtuArray2D<double> *A_normalized, DTU::DtuArray2D<double> *S, DTU::DtuArray2D<double> *Y, double lambda, double *out)
{

    //group_lasso_S = || ||_{21}
    double group_lasso_S;

    matrixL21Norm(S,&group_lasso_S);

    A_normalized->multiply(S,A_S);
    A_S->subtract(Y,A_S_minus_Y);

    double frobenius;
    matrixFrobNorm(A_S_minus_Y,&frobenius);

    (*out) = 0.5*pow(frobenius,2) + lambda*group_lasso_S;
}



void Sbs2SourceReconstructionSparse::derivative_square_loss_frobenius(DTU::DtuArray2D<double> *A,
								      DTU::DtuArray2D<double> *Y,
								      DTU::DtuArray2D<double> *S,
								      DTU::DtuArray2D<double> *out)
{
    //out = (A')*( A*S - Y );

    //A_transpose = A'
    A->transpose(A_transpose);

    //A_S = A*S
    A->multiply(S,A_S);

    //A_S_minus_Y = A*S-Y
    A_S->subtract(Y,A_S_minus_Y);

    //out = (A')*( A*S - Y )
    A_transpose->multiply(A_S_minus_Y,out);


}




void Sbs2SourceReconstructionSparse::proximal_operator_standard_group_lasso(DTU::DtuArray2D<double> *X,
									    double *regularizer_factor,
									    DTU::DtuArray2D<double> *out)
{


    //norm_2_each_row  = || ||_{21}
    matrixL21NormEachRow(X , norm_2_each_row );

    //regularizer_factor./ norm_2_each_row
    scalarDividedbyVectorComponentWise_insitu(regularizer_factor , norm_2_each_row);

    // temp = 1 - (regularizer_factor./ norm_2_each_row);
    double temp = 1;
    scalarMinusVector_insitu(&temp , norm_2_each_row);

    //temp(temp<0) = 0;
    thresholding_insitu(norm_2_each_row);

    //vectorOuterProduct -> thresholdingMatrix

    //thresholdingMatrix = temp*ones(1,size(x,2))
    vectorOuterProduct(norm_2_each_row, ones_vector, thresholdingMatrix);

    //matrixMultiplicationComponentWise -> out = x.*thresholdingMatrix;
    matrixMultiplicationComponentWise(X,thresholdingMatrix,out);

}


void Sbs2SourceReconstructionSparse::fista_method_group_lasso_v2(DTU::DtuArray2D<double> *A_normalized, DTU::DtuArray2D<double> *Y, double lambda, double L, DTU::DtuArray2D<double> *estimated_S)
{
    (*S_ant)=0;
    double t_ant = 1;
    int flag = 1;
    double iteration_counter = 0;
    double f_obj_ant;
    f_objective_general_group_lasso(A_normalized, S_ant, Y, lambda, &f_obj_ant);
    (*V) = 0;
    double regularizer_factor = lambda / double(L);

    double t_act;
    double f_obj_act;
    double error_rel;

   // Y->print();

    while(flag==1)
    {

	derivative_square_loss_frobenius(A_normalized,Y,V,grad_f_y);
	grad_f_y->multiply( ( 1.0 )/(double)L , one_over_L_grad_f_y);
	V->subtract( one_over_L_grad_f_y , V_minus_one_over_L_grad_f_y);
	proximal_operator_standard_group_lasso(V_minus_one_over_L_grad_f_y , &regularizer_factor , S_act);
	t_act = ( 1.0 + sqrt( 1.0 + 4.0*pow(t_ant,2) ) ) / 2.0;
	S_act->subtract(S_ant,S_act_minus_S_ant);
	S_act_minus_S_ant->multiply( ( (t_ant-1.0)/t_act ) , scaled_S_act_minus_S_ant);
	S_act->add(scaled_S_act_minus_S_ant,V);
	f_objective_general_group_lasso(A_normalized,S_act,Y,lambda,&f_obj_act);

	error_rel = abs(f_obj_act - f_obj_ant);

	//qDebug() << error_rel << f_obj_act << f_obj_ant << L << lambda;
	if(error_rel <= error_tol)
	{
	    flag=0;
	}

	t_ant = t_act;
	copyMatrix(S_act,S_ant);
	f_obj_ant = f_obj_act;
	++iteration_counter;
    }

   copyMatrix(S_act,estimated_S);
}

void Sbs2SourceReconstructionSparse::cross_validation_k_channel(DTU::DtuArray2D<double> *Y_mean_0, DTU::DtuArray2D<double> *estimated_S)
{
    double lambda = 0.0;
    double rmse_temp = 0.0;
    vector<double> rmse(lambdas.size());
    int pos_best_lambda = 0;

    for(int i=0 ; i<lambdas.size() ; ++i)
    {
	lambda = lambdas.at(i);

	qDebug() << lambda << i;

	fista_method_group_lasso_v2(A_normalized, Y_mean_0, lambda, L, estimated_S);
	rootMeanSquareError(Y_mean_0, A_normalized, estimated_S, &rmse_temp);
	rmse[i] = rmse_temp;
	if (i!=0)
	{
	    if(rmse[i]<rmse[0])
	    {
		rmse[0]=rmse[i];
		pos_best_lambda=i;
	    }
	}
    }

    double best_lambda = lambdas[pos_best_lambda];
    fista_method_group_lasso_v2(A_normalized, Y_mean_0, best_lambda, L, S_temp);
}

void Sbs2SourceReconstructionSparse::rootMeanSquareError(DTU::DtuArray2D<double> *Y, DTU::DtuArray2D<double> *A, DTU::DtuArray2D<double> *estimated_S, double *rmse)
{
    double N = (Y->dim1())*(Y->dim2());
    A->multiply(estimated_S,A_S);
    A_S->subtract(Y,A_S_minus_Y);
    double frobenius;
    matrixFrobNorm(A_S_minus_Y,&frobenius);
    (*rmse) = ( ((double)1.0)/sqrt(N) )*frobenius;
}


void Sbs2SourceReconstructionSparse::doRec(DTU::DtuArray2D<double> *Y_input, DTU::DtuArray2D<double> *S_output, int *isSourceReconstructionReady)
{
    (*isSourceReconstructionReady) = 0;
    S = S_output;

    for (int row=0; row<Y_input->dim1(); ++row)
    {
	for (int column=0; column<Y_input->dim2(); ++column)
	{
	    (*Y)[row][column] = (*Y_input)[row][column];
	}
    }

    preprocessData();
    cross_validation_k_channel(Y,S_temp);
    calculateMean(S_temp, S_output);
    (*isSourceReconstructionReady) = 1;
}

void Sbs2SourceReconstructionSparse::doRecPow(DTU::DtuArray2D<double> *Y_input, DTU::DtuArray2D<double> *S_output, int *isSourceReconstructionReady)
{
    (*isSourceReconstructionReady) = 0;
    S = S_output;

    for (int row=0; row<Y_input->dim1(); ++row)
    {
	for (int column=0; column<Y_input->dim2(); ++column)
	{
	    (*Y)[row][column] = (*Y_input)[row][column];
	}
    }

    preprocessData();
    cross_validation_k_channel(Y,S_temp);
    calculatePower(S_temp, S_output);
    (*isSourceReconstructionReady) = 1;
}

void Sbs2SourceReconstructionSparse::preprocessData()
{
    getMean(Y,Y_meanMatrix);
    Y->subtract(Y_meanMatrix, Y);
}

void Sbs2SourceReconstructionSparse::calculateMean(DTU::DtuArray2D<double> *input, DTU::DtuArray2D<double> *output)
{
    for (int vertex = 0; vertex < input->dim1(); ++vertex)
    {
	double sum = 0;
	for (int sample = 0; sample < input->dim2(); ++sample)
	{
	    sum += (*input)[vertex][sample];
	}
	(*output)[0][vertex] = sum/double(input->dim2());
    }
}

void Sbs2SourceReconstructionSparse::calculatePower(DTU::DtuArray2D<double> *input, DTU::DtuArray2D<double> *output)
{
    DTU::DtuArray2D<double>* tempInput = new DTU::DtuArray2D<double>(1, input->dim2());
    DTU::DtuArray2D<double>* tempOutput = new DTU::DtuArray2D<double>(1, input->dim2());
    (*tempInput) = 0;
    (*tempOutput) = 0;

    for (int vertex = 0; vertex < input->dim1(); ++vertex)
    {
	for (int sample = 0; sample < input->dim2(); ++sample)
	{
	    (*tempInput)[0][sample] = (*input)[vertex][sample];
	}
	sbs2Spectrogram->doSpectrogram(tempInput,tempOutput);
	for (int v = 0; v < tempOutput->dim2()/2; ++v)
	{
	    if (v == 0)
		(*output)[v][vertex] = std::pow((*tempOutput)[0][v],2.0);
	    else
		(*output)[v][vertex] = std::pow((*tempOutput)[0][v],2.0) + std::pow((*tempOutput)[0][v+tempOutput->dim2()/2],2.0);
	}
    }
}

