#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include <QObject>
#include <QString>

class DataHandler : public QObject {
    Q_OBJECT
public:
    explicit DataHandler(QObject *parent = nullptr);

    Q_INVOKABLE void receiveParameters(const QString &filePath, int canny1, int canny2,
                                       double contourEpsilon, QString workspaceWidth,
                                       QString workspaceHeight, int spindleSpeed, int feedRate,
                                        double cutDepth,QString serialport,QString filetype);

signals:
    void parametersReceived();
    void sendData(const QString &filePath, int canny1, int canny2,double contourEpsilon,QString workspaceWidth,
                  QString workspaceHeight);
    void sendCNC_Data( QString workspaceWidth,
                      QString workspaceHeight, int spindleSpeed, int feedRate,
                       double cutDepth,QString filetype);
    void sendSerialData(QString serialportid);

};

#endif
