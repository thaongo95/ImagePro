#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <opencv2/opencv.hpp>
#include <QImage>
#include <QQmlContext>
#include "imageprovider.h"
#include "imagecontroller.h"
#include "style.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    auto provider = new CvImageProvider;
    engine.addImageProvider("cv", provider);

    ImageController controller(provider);
    engine.rootContext()->setContextProperty("cvController", &controller);

    Style style;
    engine.rootContext()->setContextProperty("systemStyle", &style);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("ImagePro", "Main");

    return app.exec();
}
