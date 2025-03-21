#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

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

class ImageProcessing : public QObject
{
    Q_OBJECT
public:
    explicit ImageProcessing(QObject *parent = nullptr);

public slots:
    void getData(const QString &filePath, int canny1, int canny2,double contourEpsilon,QString workspaceWidth,QString workspaceHeight);
    Q_INVOKABLE void processImage();

private:
    QString filePath;
    int canny1;
    int canny2;
    double contourEpsilon;
    QString workspaceWidth;
    QString workspaceHeight;

    cv::Mat sourceImage;
    std::vector<std::vector<cv::Point>> contours;

    void loadImage();

    void saveContoursToSvg(const std::vector<std::vector<cv::Point>> &contours,
                           const QString &svgFilePath,
                           int imageWidth,
                           int imageHeight);
};

#endif
