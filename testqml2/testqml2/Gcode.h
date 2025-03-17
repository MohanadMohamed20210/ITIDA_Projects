#ifndef GCODE_H
#define GCODE_H

#include <QObject>
#include <DataHandler.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <QPointF>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QProcess>
#include <QDebug>
#include <limits>
#include <algorithm>
#include <QString>
#include <QRegularExpression>

class Gcode : public QObject
{
    Q_OBJECT
public:
    explicit Gcode(QObject *parent = nullptr);
    Q_INVOKABLE void invokeGeneration();

public slots:
    void getCNC_Data(QString workspaceWidth,QString workspaceHeight,int spindleSpeed,
                 int feedRate,double cutDepth,QString filetype);

private:
    QString workspaceWidth;
    QString workspaceHeight;
    int spindleSpeed;
    int feedRate;
    double cutDepth;
    QString filetype;

    void cleanGCodeFileGerber(const QString &gcodePath);
    void generateGCodeSVG(const QString &svgPath, const QString &gcodePath);
    void generateGCodeWithGerber(const QString &gcodePath);

};

#endif // GCODE_H
