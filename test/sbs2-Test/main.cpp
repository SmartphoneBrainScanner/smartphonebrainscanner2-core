#include <QApplication>
#include "qmlapplicationviewer.h"

#include <mycallback.h>
#include <hardware/emotiv/sbs2emotivdatareader.h>

#include <tests.h>

#define CORE_VERSION 0.1

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));

    qDebug() << "root path: " << Sbs2Common::setDefaultRootAppPath();
    qDebug() << "catalog path: " << Sbs2Common::setDefaultCatalogPath();

    MyCallback* myCallback = new MyCallback();
    Sbs2EmotivDataReader* sbs2DataReader = Sbs2EmotivDataReader::New(myCallback);

    Tests* tests = new Tests();
    tests->run();

    QmlApplicationViewer viewer;
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/sbs2-Test/main.qml"));
    viewer.showExpanded();

    return app->exec();
}
