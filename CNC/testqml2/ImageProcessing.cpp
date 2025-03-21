#include "ImageProcessing.h"

extern DataHandler dataHandler;

ImageProcessing::ImageProcessing(QObject *parent)
    : QObject{parent}
{
    connect(&dataHandler, &DataHandler::sendData, this, &ImageProcessing::getData);
}

void ImageProcessing::getData(const QString &filePath, int canny1, int canny2, double contourEpsilon, QString workspaceWidth, QString workspaceHeight)
{
    this->filePath = filePath;

    if (this->filePath.startsWith("file:///")) {
        this->filePath = this->filePath.mid(8);
    }
    this->filePath.prepend("/");
    this->canny1 = canny1;
    this->canny2 = canny2;
    this->contourEpsilon = contourEpsilon;
    this->workspaceWidth = workspaceWidth;
    this->workspaceHeight = workspaceHeight;

    qDebug() << "========= Received G-Code Parameters =========";
    qDebug() << "File Path: " << this->filePath;
    qDebug() << "Canny Threshold 1: " << canny1;
    qDebug() << "Canny Threshold 2: " << canny2;
    qDebug() << "Contour Approximation Epsilon: " << contourEpsilon;
    qDebug() << "========================================";

    loadImage();
}

void ImageProcessing::loadImage()
{
    sourceImage = cv::imread(filePath.toStdString(), cv::IMREAD_COLOR);
}

void ImageProcessing::processImage()
{
    if (sourceImage.empty()) {
        qDebug() << "Error: Image not loaded!";
        return;
    }

    // Convert to grayscale
    cv::Mat gray;
    cv::cvtColor(sourceImage, gray, cv::COLOR_BGR2GRAY);

    // Apply adaptive threshold for better text detection
    cv::Mat binarized;
    cv::adaptiveThreshold(gray, binarized, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 11, 2);

    // Find contours
    std::vector<std::vector<cv::Point>> tempContours;
    cv::findContours(binarized, tempContours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE); // RETR_TREE to get both shapes & text

    contours.clear();

    for (const auto &contour : tempContours) {
        if (cv::contourArea(contour) < 5) continue;  // Keep small text but remove noise

        std::vector<cv::Point> approx;
        double adaptiveEpsilon = 0.002 * cv::arcLength(contour, true);  // Slightly higher for smoother edges
        cv::approxPolyDP(contour, approx, adaptiveEpsilon, true);

        if (approx.size() >= 3) {  // Only keep valid closed shapes
            contours.push_back(approx);
        }
    }

    QString svgOutputPath = "file.svg";
    saveContoursToSvg(contours, svgOutputPath, sourceImage.cols, sourceImage.rows);
}

void ImageProcessing::saveContoursToSvg(const std::vector<std::vector<cv::Point>> &contours,
                                        const QString &svgFilePath, int imageWidth, int imageHeight)
{
    QFile file(svgFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qDebug() << "Can't open the file for writing!";
        return;
    }

    QTextStream out(&file);
    out << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" "
        << "width=\"" << workspaceWidth << "mm\" height=\"" << workspaceHeight << "mm\" "
        << "viewBox=\"0 0 " << workspaceWidth << " " << workspaceHeight << "\">\n";

    double scaleX = workspaceWidth.toDouble() / imageWidth;
    double scaleY = workspaceHeight.toDouble() / imageHeight;
    double scale = std::min(scaleX, scaleY); // Keep aspect ratio

    for (const auto &contour : contours) {
        if (contour.empty() || cv::arcLength(contour, true) < 5) continue;

        out << "  <path d=\"M "
            << contour[0].x * scale << " "
            << contour[0].y * scale << " ";

        for (int i = 1; i < contour.size(); ++i) {
            out << "L "
                << contour[i].x * scale << " "
                << contour[i].y * scale << " ";
        }
        out << "Z\" fill=\"none\" stroke=\"black\" stroke-width=\"0.5\"/>\n";
    }

    out << "</svg>\n";
    file.close();

    qDebug() << "SVG successfully saved to:" << svgFilePath;
}
