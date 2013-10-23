#include "sbs2sourcereconstruction.h"


Sbs2SourceReconstruction::Sbs2SourceReconstruction(QObject *parent) :
    QObject(parent)
{
    sbs2SourceReconstructionLoreta = 0;
    sourceReconstructionMethod = "";

}

void Sbs2SourceReconstruction::turnOnLoreta(int sourceReconstructionSamples, int sourceReconstructionDelta, int sourceReconstructionModelUpdateLength, int sourceReconstructionModelUpdateDelta, QString hardware, QString sourceReconstructionMethod_)
{

    sbs2SourceReconstructionLoreta = new Sbs2SourceReconstrucionLoreta(Sbs2Common::channelsNo(), sourceReconstructionSamples, sourceReconstructionDelta, Sbs2Common::verticesNo(), hardware, this, sourceReconstructionModelUpdateLength, sourceReconstructionModelUpdateDelta);
    sourceReconstructionMethod = sourceReconstructionMethod_;
}

void Sbs2SourceReconstruction::turnOnSparse(int sourceReconstructionSamples, QString hardware, QVector<double> lambdas, QString sourceReconstructionMethod_)
{
    qDebug() << Q_FUNC_INFO << sourceReconstructionMethod_;
    sbs2SourceReconstructionSparse = new Sbs2SourceReconstructionSparse(Sbs2Common::channelsNo(), Sbs2Common::verticesNo(), sourceReconstructionSamples, lambdas, hardware, this);
    sourceReconstructionMethod = sourceReconstructionMethod_;
}

void Sbs2SourceReconstruction::doReconstruction(DTU::DtuArray2D<double> *input_, DTU::DtuArray2D<double> *output_, int *sourceReconstructionReady)
{
    if (sourceReconstructionMethod == "")
	    return;
    if (sourceReconstructionMethod == "loreta")
	sbs2SourceReconstructionLoreta->doRec(input_, output_, sourceReconstructionReady);
    if (sourceReconstructionMethod == "sparse")
	sbs2SourceReconstructionSparse->doRec(input_, output_, sourceReconstructionReady);
}

void Sbs2SourceReconstruction::doReconstructionSpectrogram(DTU::DtuArray2D<double> *input_, DTU::DtuArray2D<double> *output_, int *sourceReconstructionReady)
{
    if (sourceReconstructionMethod == "")
	    return;
    if (sourceReconstructionMethod == "loreta")
	sbs2SourceReconstructionLoreta->doRecPow(input_, output_, sourceReconstructionReady);
    if (sourceReconstructionMethod == "sparse")
	sbs2SourceReconstructionSparse->doRecPow(input_, output_, sourceReconstructionReady);

}

void Sbs2SourceReconstruction::stopReconstruction()
{

}

void Sbs2SourceReconstruction::turnOff()
{

}
