#include "DataHandler.h"
#include <QDebug>

DataHandler::DataHandler(QObject *parent) : QObject(parent) {}

void DataHandler::receiveParameters(const QString &filePath, int canny1, int canny2,
                                    double contourEpsilon, QString workspaceWidth,
                                    QString workspaceHeight, int spindleSpeed, int feedRate,
                                    int safeHeight, double cutDepth,QString serialport,QString filetype) {

    emit parametersReceived();
    if(filetype == "Image")
    emit sendData(filePath,canny1,canny2,contourEpsilon,workspaceWidth,workspaceHeight);
    emit sendCNC_Data(workspaceWidth,workspaceHeight,spindleSpeed,feedRate,safeHeight,cutDepth,filetype);
    emit sendSerialData(serialport);
}
