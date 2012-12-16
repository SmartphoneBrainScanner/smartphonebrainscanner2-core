#ifndef SBS2SOURCERECONSTRUCTION_H
#define SBS2SOURCERECONSTRUCTION_H

#include <QObject>
#include <source_reconstruction/sparse/sbs2sourcereconstruction_sparse.h>
#include <source_reconstruction/loreta/sbs2sourcereconstruction_loreta.h>
#include <sbs2common.h>

class Sbs2SourceReconstruction : public QObject
{
    Q_OBJECT
public:
    explicit Sbs2SourceReconstruction(QObject *parent = 0);
    
private:
    Sbs2SourceReconstrucionLoreta* sbs2SourceReconstructionLoreta;
    Sbs2SourceReconstructionSparse* sbs2SourceReconstructionSparse;

    QString sourceReconstructionMethod;

signals:
    
public slots:
    void turnOnLoreta(int sourceReconstructionSamples, int sourceReconstructionDelta, int sourceReconstructionModelUpdateLength, int sourceReconstructionModelUpdateDelta, QString hardware,  QString sourceReconstructionMethod_);
    void turnOnSparse(int sourceReconstructionSamples, QString hardware, QVector<double> lambdas, QString sourceReconstructionMethod_);
    void doReconstruction(DTU::DtuArray2D<double>* input_, DTU::DtuArray2D<double>* output_, int* sourceReconstructionReady);
    void doReconstructionSpectrogram(DTU::DtuArray2D<double>* input_, DTU::DtuArray2D<double>* output_, int* sourceReconstructionReady);
    void stopReconstruction();
    void turnOff();
    
};

#endif // SBS2SOURCERECONSTRUCTION_H
