#include "sbs2sourcereconstruction_loreta.h"

//TODO: change readPriorSpatialCoherence(); and inverse to binary files


Sbs2SourceReconstrucionLoreta::Sbs2SourceReconstrucionLoreta(int channels_, int samples_, int samplesDelta_, int vertices_, QString hardware_, QObject *parent, int modelUpdateLength_, int modelUpdateDelta_):
    QObject(parent), channels(channels_), samples(samples_), samplesDelta(samplesDelta_), vertices(vertices_), modelUpdateLength(modelUpdateLength_), modelUpdateDelta(modelUpdateDelta_), hardware(hardware_)
{

    modelUpdateSamplesLength = modelUpdateLength*samplesDelta;

    sumType = MEAN;
    input = new DTU::DtuArray2D<double>(channels,samples);
    (*input) = 0;
    w = new DTU::DtuArray2D<double>(vertices,channels);
    (*w) = 0;
    weightedInput = new DTU::DtuArray2D<double>(vertices,samples);
    (*weightedInput) = 0;

    verticesToExtract = 0;

    paramMeanExtractionOn = 1;

    if (modelUpdateDelta < modelUpdateLength) modelUpdateDelta = modelUpdateLength;
    setupModel();

    sbs2Spectrogram = new Sbs2Spectrogram(samples,this);

}

void Sbs2SourceReconstrucionLoreta::setupFixedModel()
{
    readMNEFixedWeights();
}

void Sbs2SourceReconstrucionLoreta::setupModel()
{
    toModelUpdateValuesIndex = 0;
    modelUpdateSamplesSeen = 0;

    paramAScaling = 1000000;

    k = new DTU::DtuArray2D<double>(vertices,vertices);
    kInv = new DTU::DtuArray2D<double>(vertices,vertices);
    a = new DTU::DtuArray2D<double>(channels,vertices);
    //toModelUpdateValues = new DTU::DtuArray2D<double>(channels,modelUpdateLength*samples);
    //currentModelUpdateValues = new DTU::DtuArray2D<double>(channels,modelUpdateLength*samples);
    //currentModelUpdateValuesVertices = new DTU::DtuArray2D<double>(vertices,modelUpdateLength*samples);
    toModelUpdateValues = new DTU::DtuArray2D<double>(channels,modelUpdateSamplesLength);
    currentModelUpdateValues = new DTU::DtuArray2D<double>(channels,modelUpdateSamplesLength);
    currentModelUpdateValuesVertices = new DTU::DtuArray2D<double>(vertices,modelUpdateSamplesLength);

    (*k) = 0;
    k->toIdentityMatrix();
    kInv->toIdentityMatrix();
    (*kInv) = 0;
    (*a) = 0;
    (*toModelUpdateValues) = 0;
    (*currentModelUpdateValues) = 0;
    (*currentModelUpdateValuesVertices) = 0;

    invAlpha = 0.0100;
    invBeta = 0.3781;

    at = new DTU::DtuArray2D<double>(vertices,channels);
    akat = new DTU::DtuArray2D<double>(channels,channels);
    kat = new DTU::DtuArray2D<double>(vertices,channels);
    ak = new DTU::DtuArray2D<double>(channels,vertices);
    akInv = new DTU::DtuArray2D<double>(channels,vertices);
    akatInvAlpha = new DTU::DtuArray2D<double>(channels,channels);
    sigmaS = new DTU::DtuArray2D<double>(vertices,vertices);
    inputMatrix = new DTU::DtuArray2D<double>(channels,channels);
    identity = new DTU::DtuArray2D<double>(channels,channels);
    identityInvBeta = new DTU::DtuArray2D<double>(channels,channels);
    tempW = new DTU::DtuArray2D<double>(vertices,channels);
    invSigmaE = new DTU::DtuArray2D<double>(channels,channels);
    invSigmaEA = new DTU::DtuArray2D<double>(channels,vertices);
    atInvSigmaEA = new DTU::DtuArray2D<double>(vertices,vertices);
    //invSigmaEAS = new DTU::DtuArray2D<double>(channels,modelUpdateLength*samples);
    invSigmaEAS = new DTU::DtuArray2D<double>(channels,modelUpdateSamplesLength);

    katInput = new DTU::DtuArray2D<double>(vertices,channels);

    (*kat) = 0;
    (*katInput) = 0;

    readForwardModel();
    readPriorSpatialCoherence();
    readPriorSpatialCoherenceInverse();

    a->multiply(k,ak);
    invSigmaE->toIdentityMatrix();
    identity->toIdentityMatrix();


    a->transpose(at);
    k->multiply(at,kat);
    a->multiply(kat,akat);
    a->multiply(kInv,akInv);

    akat->multiply(invAlpha,akatInvAlpha);
    identity->multiply(invBeta,identityInvBeta);
    akatInvAlpha->add(identityInvBeta,inputMatrix);

    inputMatrix->pinv(inputMatrix);

    calculateSigma();
    invSigmaE->multiply(a,invSigmaEA);
    at->multiply(invSigmaEA,atInvSigmaEA);


    kat->multiply(inputMatrix,katInput);

    modelUpdateReady = 1;
    modelUpdateDeltaCollected = 0;
    katInput->multiply(invAlpha,w);

    tempModelUpdatedReady = 1;


    tempInput = new DTU::DtuArray2D<double>(1,samples);
    tempOutput = new DTU::DtuArray2D<double>(1,samples);

}

void Sbs2SourceReconstrucionLoreta::doRec(DTU::DtuArray2D<double> *input_, DTU::DtuArray2D<double> *output_, int* sourceReconstrutionReady)
{

    qDebug() << Q_FUNC_INFO;

    (*sourceReconstrutionReady) = 0;

    output = output_;
    if (!(input_->dim1() == channels))
	return;
    if (!(input->dim2() == samples))
	return;
    if (!(output->dim1()) == 1)
	return;
    if (!(output->dim2()) == vertices)
	return;

    for (int row=0; row<input_->dim1(); ++row)
    {
	for (int column=0; column<input_->dim2(); ++column)
	{
	    (*input)[row][column] = (*input_)[row][column];
	}
    }

    (*output) = 0;




    preprocessData();

    weight();

    collectDataForModelUpdate();

    doModelUpdate();

    //Sbs2Timer::tic("reconstruct()");
    reconstruct();
    //Sbs2Timer::toc();

    output->print();

    (*sourceReconstrutionReady) = 1;


}



void Sbs2SourceReconstrucionLoreta::doRecPow(DTU::DtuArray2D<double> *input_, DTU::DtuArray2D<double> *output_, int* sourceReconstrutionReady)
{
    (*sourceReconstrutionReady) = 0;

    output = output_;
    if (!(input_->dim1() == channels))
	return;
    if (!(input->dim2() == samples))
	return;
    if (!(output->dim1() == Sbs2Common::samplingRate()/2)) //freq bins
	return;
    if (!(output->dim2() == vertices))
	return;

    for (int row=0; row<input_->dim1(); ++row)
    {
	for (int column=0; column<input_->dim2(); ++column)
	{
	    (*input)[row][column] = (*input_)[row][column];
	}
    }

    (*output) = 0;

    preprocessData();
    weight();

    sourceSpectrogram();
    collectDataForModelUpdate();
    doModelUpdate();
    (*sourceReconstrutionReady) = 1;
}


void Sbs2SourceReconstrucionLoreta::sourceSpectrogram()
{
    for (int vertex = 0; vertex < weightedInput->dim1(); ++vertex)
    {
	if (verticesToExtract != 0 && !verticesToExtract->contains(vertex))
	    continue;

	for (int sample = 0; sample < weightedInput->dim2(); ++sample)
	{
	    (*tempInput)[0][sample] = (*weightedInput)[vertex][sample];
	}
	sbs2Spectrogram->doSpectrogram(tempInput,tempOutput);
	for (int v = 0; v < tempOutput->dim2()/2; ++v) //re & img
	{
	    if (v == 0)
		(*output)[v][vertex] = std::pow((*tempOutput)[0][v],2.0);
	    else
		(*output)[v][vertex] = std::pow((*tempOutput)[0][v],2.0) + std::pow((*tempOutput)[0][v+tempOutput->dim2()/2],2.0);
	}
    }
}



void Sbs2SourceReconstrucionLoreta::collectDataForModelUpdate()
{

    //we only collect samples before we are going to do update
    modelUpdateSamplesSeen = (modelUpdateSamplesSeen +1)%modelUpdateDelta;
    if (modelUpdateSamplesSeen < (modelUpdateDelta-modelUpdateLength))
    {
	return;
    }

    //TODO: handle disjoint delta
    //collect new information from the input data, the new informatio has length samplesDelta
    for (int row = 0; row < channels; ++row)
    {
	for (int column = 0; column < samplesDelta; ++column)
	{
	    (*toModelUpdateValues)[row][(toModelUpdateValues->dim2() - toModelUpdateValuesIndex)-(samplesDelta-column)] = (*input)[row][column];
	}
    }

    toModelUpdateValuesIndex = (toModelUpdateValuesIndex + samplesDelta)%currentModelUpdateValues->dim2();


}



void Sbs2SourceReconstrucionLoreta::doModelUpdate()
{

    //FIXME
    if (modelUpdateReady) //DELETME
	++modelUpdateDeltaCollected;

    if (modelUpdateReady)
    {

	if (modelUpdateDeltaCollected < modelUpdateDelta)
	    return;

	tempModelUpdatedReady = 0;

	modelUpdateDeltaCollected = 0;

	tempModelUpdateValues = toModelUpdateValues;
	toModelUpdateValues = currentModelUpdateValues;
	currentModelUpdateValues = tempModelUpdateValues;

	QtConcurrent::run(this,&Sbs2SourceReconstrucionLoreta::updateModel);
    }
}

/**
  Extract mean on the per sample basis from all the channels
  */

void Sbs2SourceReconstrucionLoreta::preprocessData()
{
    if (!paramMeanExtractionOn)
	return;

    double ymean[samples];
    for (int column=0; column<samples; ++column)
    {
	ymean[column] = 0;
	for (int row=0; row<channels; ++row)
	{
	    ymean[column] += (*input)[row][column];
	}

	ymean[column] = ymean[column] / (double)channels;

	for (int row=0; row<channels; ++row)
	{
	    (*input)[row][column] = (*input)[row][column] - ymean[column];
	}
    }
}

void Sbs2SourceReconstrucionLoreta::readMNEFixedWeights()
{
    QFile file(QString(Sbs2Common::getRootAppPath())+QString("mobi_weights_spheres_reduced.txt"));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
	qDebug() << "readMNEFixedWeights() file problem";
	return;
    }

    int i=0;
    while(!file.atEnd())
    {
	QStringList list = QString(file.readLine().data()).split("\t");
	for (int j=0; j<list.size(); ++j)
	{
	    (*w)[i][j] = list.at(j).toDouble();

	}
	++i;
    }


}

void Sbs2SourceReconstrucionLoreta::weight()
{
    w->multiply(input,weightedInput);
}

void Sbs2SourceReconstrucionLoreta::reconstruct()
{

    if (sumType == MEAN)
	calculateMean();
    if (sumType == POWER)
	calculatePower();
}

void Sbs2SourceReconstrucionLoreta::calculateMean()
{
    for (int row = 0; row < weightedInput->dim1(); ++row)
    {
	if (verticesToExtract != 0 && !verticesToExtract->contains(row))
	    continue;

	double vertexMean = 0;
	double sum = 0;
	for (int column = 0; column < weightedInput->dim2(); ++column)
	{
	    sum += (*weightedInput)[row][column];
	}
	vertexMean = sum/(double)weightedInput->dim2();
	(*output)[0][row] = vertexMean;
    }
}

void Sbs2SourceReconstrucionLoreta::calculatePower()
{
    for (int row = 0; row < weightedInput->dim1(); ++row)
    {
	if (verticesToExtract != 0 && !verticesToExtract->contains(row))
	    continue;

	double vertexMean = 0;
	double sum = 0;
	for (int column = 0; column < weightedInput->dim2(); ++column)
	{
	    sum += std::pow((*weightedInput)[row][column],2);
	}
	vertexMean = sum/(double)weightedInput->dim2();
	(*output)[0][row] = vertexMean;
    }

}

void Sbs2SourceReconstrucionLoreta::updateModel()
{
    if(!modelUpdateReady)
	return;
    modelUpdateReady = 0;


    //TODO: signal when new beta and alpha are ready

    //qDebug() << "****";

    //we collect only raw values and weight them right before updating the model
    w->multiply(currentModelUpdateValues,currentModelUpdateValuesVertices);


    //Sbs2Timer::tic("updateAlpha()");
    updateAlpha();
    //Sbs2Timer::toc();

    //Sbs2Timer::tic("updateBeta()");
    updateBeta();
    //Sbs2Timer::toc();

    //Sbs2Timer::tic("updateW()");
    updateW();
    //Sbs2Timer::toc();

    modelUpdateReady = 1;
    tempModelUpdatedReady = 1;

}

void Sbs2SourceReconstrucionLoreta::updateAlpha()
{

    double tempInvAlpha = 0.0;
    double tempEsMean = 0.0;

    calculateSigma();


    for (int j=0; j<vertices; ++j)
    {
	for (int i=0; i<vertices; ++i)
	{


	    tempInvAlpha += (*kInv)[i][j]*(*sigmaS)[j][i];
	    for (int t=0; t<modelUpdateSamplesLength; ++t)
	    {
		tempEsMean += (*currentModelUpdateValuesVertices)[i][t]*(*kInv)[i][j]*(*currentModelUpdateValuesVertices)[j][t];
	    }
	}
    }


    //    std::cout << "ALPHA:"<<std::endl;
    //    std::cout << "tempInvAlpha: "<<tempInvAlpha <<std::endl;
    tempInvAlpha = modelUpdateSamplesLength * tempInvAlpha + tempEsMean;
    invAlpha = tempInvAlpha / (double)(vertices*modelUpdateSamplesLength);
    //    std::cout << "tempInvAlpha: "<<tempInvAlpha <<std::endl;
    //    std::cout << "tempEsMean: "<<tempEsMean <<std::endl;
    //    std::cout << "invAlpha: "<<invAlpha <<std::endl;
    //    std::cout << "----" << std::endl;
    //    std::cout << invAlpha <<" ";
}

void Sbs2SourceReconstrucionLoreta::updateBeta()
{

    double tempInvBeta = 0.0;
    double tempEsMean = 0.0;
    double Ey = 0.0;

    invSigmaEA->multiply(currentModelUpdateValuesVertices,invSigmaEAS);

    for (int j=0; j<vertices; ++j)
    {
	for (int i=0; i<vertices; ++i)
	{

	    tempInvBeta += (*atInvSigmaEA)[i][j] * (*sigmaS)[j][i];

	    for (int t=0; t<modelUpdateSamplesLength; ++t)
	    {
		tempEsMean += (*currentModelUpdateValuesVertices)[i][t]*(*atInvSigmaEA)[i][j]*(*currentModelUpdateValuesVertices)[j][t];
	    }
	}
    }


    for (int t=0; t<modelUpdateSamplesLength; ++t)
    {
	for (int j=0; j<channels; ++j)
	{
	    for (int i=0; i<channels; ++i)
	    {
		Ey += (*currentModelUpdateValues)[i][t] * (*invSigmaE)[i][j] * (*currentModelUpdateValues)[j][t];
	    }
	    Ey -= 2*(*currentModelUpdateValues)[j][t]*(*invSigmaEAS)[j][t];
	}
    }


    //    std::cout << "BETA:"<<std::endl;
    //    std::cout << "tempInvBeta: "<<tempInvBeta<<std::endl;
    tempInvBeta = modelUpdateSamplesLength * tempInvBeta + tempEsMean + Ey;
    invBeta = tempInvBeta / (double)(channels*modelUpdateSamplesLength);
    //    std::cout << "tempInvBeta: "<<tempInvBeta<<std::endl;
    //    std::cout << "tempEsMean: "<<tempEsMean<<std::endl;
    //    std::cout << "Ey: "<<Ey<<std::endl;
    //    std::cout << "invBeta: "<<invBeta<<std::endl;
    //    std::cout << "----"<<std::endl;
    //     std::cout << invBeta << std::endl;

}

void Sbs2SourceReconstrucionLoreta::updateW()
{

    calculateInputMatrix();
    inputMatrix->pinv(inputMatrix);
    kat->multiply(inputMatrix,invAlpha,tempW);

    midW = w;
    w = tempW;
    tempW = midW;

}


void Sbs2SourceReconstrucionLoreta::calculateInputMatrix()
{
    for (int row=0; row<channels; ++row)
    {
	for (int column=0; column<channels; ++column)
	{
	    if (row==column)
		(*inputMatrix)[row][column] = (*akat)[row][column] * invAlpha + invBeta;
	    else
		(*inputMatrix)[row][column] = (*akat)[row][column] * invAlpha;
	}
    }

}

void Sbs2SourceReconstrucionLoreta::calculateSigma()
{
    double* Bcolj = new double[channels];

    for (int j=0; j<vertices; ++j)
    {
	for (int k=0; k<channels; ++k)
	{
	    Bcolj[k] = (*ak)[k][j];
	}

	for (int i=0; i<vertices; ++i)
	{
	    double s = 0;
	    for (int k=0; k<channels; ++k)
	    {
		s += (*w)[i][k] * Bcolj[k];
	    }
	    (*sigmaS)[i][j] = ((*k)[i][j]-s)*invAlpha;
	}

    }
    delete[] Bcolj;
}


/**
* @brief Sbs2SourceReconstrucion::setSumType
* @param sumType_ should be either 'MEAN' or 'SUM'
*
* Set the 'sumType' variable
*/
void Sbs2SourceReconstrucionLoreta::setSumType(SumType sumType_)
{
    sumType = sumType_;
}

/**
  Reading a.
  */

/**
* @brief Sbs2SourceReconstrucion::readForwardModel
*
* Read the forward model from the "hardware/<hardware>/forwardmodel_spheres_reduced.txt" file.
* It sets up the (private) 'a' array
*/
void Sbs2SourceReconstrucionLoreta::readForwardModel()
{
    QFile file(QString(Sbs2Common::getRootAppPath()) + QString("hardware/") + hardware + QString("/") + QString("forwardmodel_spheres_reduced.txt"));

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
	qDebug() << "readForwardModel() file problem";
	return;
    }

    int i=0;
    while(!file.atEnd())
    {
	QStringList list = QString(file.readLine().data()).split("\t");
	for (int j=0; j<list.size(); ++j)
	{
	    (*a)[i][j] = list.at(j).toDouble() * paramAScaling;

	}
	++i;
    }

}

/**
  Reading k.
  */

void Sbs2SourceReconstrucionLoreta::readPriorSpatialCoherence()
{
    QFile file(QString(Sbs2Common::getRootAppPath())+ QString("hardware/")+hardware+QString("/")+QString("spatialCoherenceSmooth0-2_reduced.txt"));

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
	qDebug() << "readPriorSpatialCoherence() file problem";
	return;
    }

    int i=0;
    while(!file.atEnd())
    {
	QStringList list = QString(file.readLine().data()).split("\t");
	for (int j=0; j<list.size(); ++j)
	{
	    (*k)[i][j] = list.at(j).toDouble();

	}
	++i;
    }
}

/**
  Reading invK.
  */

void Sbs2SourceReconstrucionLoreta::readPriorSpatialCoherenceInverse()
{
    QFile file(QString(Sbs2Common::getRootAppPath())+QString("hardware/")+hardware+QString("/")+QString("spatialCoherenceSmooth0-2_reduced_inverse.txt"));

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
	qDebug() << "readPriorSpatialCoherenceInverse() file problem";
	return;
    }

    int i=0;
    while(!file.atEnd())
    {
	QStringList list = QString(file.readLine().data()).split("\t");
	for (int j=0; j<list.size(); ++j)
	{
	    (*kInv)[i][j] = list.at(j).toDouble();

	}
	++i;
    }
}

void Sbs2SourceReconstrucionLoreta::setMeanExtraction(int enabled)
{
    paramMeanExtractionOn = (enabled >= 1);
}

void Sbs2SourceReconstrucionLoreta::setAScaling(int scaling)
{
    paramAScaling = scaling;
}


/**
  Hann window.
  */
double Sbs2SourceReconstrucionLoreta::window(int n)
{
    int length = 128;
    return 0.5*(1-std::cos(2*PI*n/(length)));
}

void Sbs2SourceReconstrucionLoreta::setVerticesToExtract(QVector<int> *verticesToExtract_)
{
    verticesToExtract = verticesToExtract_;
}
