#include "Gcode.h"

extern DataHandler dataHandler;

Gcode::Gcode(QObject *parent)
    : QObject{parent}
{
    connect(&dataHandler, &DataHandler::sendCNC_Data, this, &Gcode::getCNC_Data);
}

void Gcode::invokeGeneration()
{
    QString svgOutputPath = "file.svg";
    QString gcodeOutputPath = "file.gcode";
    if(filetype == "Image")
        generateGCodeSVG(svgOutputPath, gcodeOutputPath);
    else if(filetype == "Gerber")
        generateGCodeWithGerber(gcodeOutputPath);
}

void Gcode::getCNC_Data(QString workspaceWidth, QString workspaceHeight,
                        int spindleSpeed, int feedRate,
                        int safeHeight, double cutDepth,QString filetype)
{
    this->workspaceWidth = workspaceWidth;
    this->workspaceHeight = workspaceHeight;
    this->spindleSpeed = spindleSpeed;
    this->feedRate = feedRate;
    this->safeHeight = safeHeight;
    this->cutDepth = cutDepth;
    this->filetype = filetype;
}

void Gcode::generateGCodeWithGerber(const QString &gcodePath)
{
    qDebug() << "Generating G-code for milling only...";

    QProcess process;
    QString program = "pcb2gcode";
    QStringList arguments;

    QString frontGerber = "thermal.gbr";

    arguments << "--metric"
              << "--metricoutput"
              << "--zchange=" + QString::number(safeHeight)
              << "--front" << frontGerber
              << "--zsafe=" + QString::number(safeHeight)
              << "--zwork=" + QString::number(-cutDepth)
              << "--mill-feed=" + QString::number(feedRate)
              << "--mill-speed=" + QString::number(spindleSpeed)
              << "--mill-diameters=0.8";

    qDebug() << "Running command: " << program << arguments.join(" ");

    process.start(program, arguments);

    if (!process.waitForFinished(30000)) {
        qDebug() << "pcb2gcode process did not finish in time.";
        return;
    }

    QByteArray stdOutput = process.readAllStandardOutput();
    QByteArray stdError = process.readAllStandardError();

    qDebug() << "pcb2gcode standard output:" << stdOutput;
    qDebug() << "pcb2gcode standard error:" << stdError;

    if (process.exitStatus() != QProcess::NormalExit || process.exitCode() != 0) {
        qDebug() << "pcb2gcode encountered an error.";
        return;
    }

    qDebug() << "G-code for milling successfully generated....";

    QFile resultFile("front.ngc");
    if (resultFile.exists()) {
        if (QFile::remove(gcodePath))
        {
            qDebug() << "Removed existing file at: " << gcodePath;
        }
        if (resultFile.rename(gcodePath))
        {
            qDebug() << "G-code file successfully renamed to: " << gcodePath;
            cleanGCodeFileGerber("file.gcode");
        }
        else
        {
            qDebug() << "Failed to rename G-code file to: " << gcodePath;
        }
    }
    else
    {
        qDebug() << "Error: front.ngc not found!";
    }
}

void Gcode::cleanGCodeFileGerber(const QString &gcodePath)
{
    qDebug() << "Cleaning and validating G-code file: " << gcodePath;

    QFile inputFile(gcodePath);
    QFile outputFile(gcodePath + ".tmp");

    if (!inputFile.open(QIODevice::ReadOnly) || !outputFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        qDebug() << "Failed to open G-code files!";
        return;
    }

    QTextStream in(&inputFile);
    QTextStream out(&outputFile);

    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();

        int start = line.indexOf('(');
        int end = line.indexOf(')');
        if (start != -1 && end != -1 && end > start)
        {
            line = line.left(start).trimmed();
        }

        int semicolonIndex = line.indexOf(';');
        if (semicolonIndex != -1)
        {
            line = line.left(semicolonIndex).trimmed();
        }

        if (!line.isEmpty())
        {
            out << line << "\n";
        }
    }

    inputFile.close();
    outputFile.close();

    inputFile.remove();
    outputFile.rename(gcodePath);

    qDebug() << "G-code cleaned successfully!";
}


void Gcode::generateGCodeSVG(const QString &svgPath, const QString &gcodePath)
{
    qDebug() << "Generating G-code from contours...";

    QFile svgFile(svgPath);
    if (!svgFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open SVG file: " << svgPath;
        return;
    }

    QFile gcodeFile(gcodePath);
    if (!gcodeFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qDebug() << "Failed to open G-code file for writing: " << gcodePath;
        return;
    }

    QTextStream svgStream(&svgFile);
    QTextStream gcodeStream(&gcodeFile);

    // G-code Header
    gcodeStream << "G21\n";  // Set units to mm
    gcodeStream << "G90\n";  // Absolute positioning
    gcodeStream << "M3 S" << spindleSpeed << "\n";  // Spindle on
    gcodeStream << "G0 Z" << safeHeight << "\n";  // Move to safe height

    QRegularExpression pathRegex("<path d=\"([^\"]+)\"");
    QRegularExpression coordRegex("([ML])\\s*(-?\\d+\\.?\\d*)\\s*(-?\\d+\\.?\\d*)");

    QString svgContent = svgStream.readAll();
    QRegularExpressionMatchIterator pathIter = pathRegex.globalMatch(svgContent);

    while (pathIter.hasNext()) {
        QRegularExpressionMatch pathMatch = pathIter.next();
        QString pathData = pathMatch.captured(1);

        QRegularExpressionMatchIterator coordIter = coordRegex.globalMatch(pathData);
        bool firstPoint = true;

        while (coordIter.hasNext()) {
            QRegularExpressionMatch coordMatch = coordIter.next();
            QString command = coordMatch.captured(1);
            double x = coordMatch.captured(2).toDouble();
            double y = coordMatch.captured(3).toDouble();

            if (firstPoint) {
                // Move to start point without cutting
                gcodeStream << "G0 X" << x << " Y" << y << "\n";
                // Lower tool for cutting
                gcodeStream << "G1 Z" << -cutDepth << " F" << feedRate << "\n";
                firstPoint = false;
            } else {
                // Cut to next point
                gcodeStream << "G1 X" << x << " Y" << y << " F" << feedRate << "\n";
            }
        }

        // Lift tool after finishing contour
        gcodeStream << "G0 Z" << safeHeight << "\n";
    }

    // G-code Footer
    gcodeStream << "M5\n";         // Spindle off
    gcodeStream << "G0 Z" << safeHeight << "\n";  // Move to safe height
    gcodeStream << "G0 X0 Y0\n";   // Move to home position
    gcodeStream << "M30\n";        // End program

    svgFile.close();
    gcodeFile.close();

    qDebug() << "G-code successfully generated at: " << gcodePath;
}
