#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QQmlContext>
#include <DataHandler.h>
#include <QDebug>
#include <ImageProcessing.h>
#include <Gcode.h>
#include <Serialization.h>

DataHandler dataHandler;
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;
    engine.addImportPath("C:/Qt/6.8.0/mingw_64/qml");

    ImageProcessing image_processing;
    Gcode gcode;
    Serialization serial;

    engine.rootContext()->setContextProperty("dataHandler", &dataHandler);
    engine.rootContext()->setContextProperty("image_processing", &image_processing);
    engine.rootContext()->setContextProperty("gcode", &gcode);
    engine.rootContext()->setContextProperty("serial", &serial);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() {
            qCritical() << "Error: QML failed to load!";
            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    engine.loadFromModule("testqml2", "Main");

    if (engine.rootObjects().isEmpty()) {
        qCritical() << "Error: No QML objects loaded!";
        return -1;
    }

    return app.exec();
}
