#include "Serialization.h"

extern DataHandler dataHandler;

Serialization::Serialization(QObject *parent)
    : QObject{parent}
{
    this->serialportid = "/dev/ttyUSB0";

    monitor = "Begin the serialization ❤️__❤️\n";
    connect(&dataHandler, &DataHandler::sendSerialData, this, &Serialization::getSerialPort);
    connect(&serial, &QSerialPort::readyRead, this, &Serialization::GetValue);
}

Serialization::~Serialization()
{
    if (serial.isOpen())
    {
        serial.close();
    }
}

void Serialization::getSerialPort(QString serialportid)
{
    this->serialportid = serialportid;

    if (!serial.isOpen())
    {
        serial.setPortName(serialportid);
        serial.setBaudRate(QSerialPort::Baud115200);
        serial.setDataBits(QSerialPort::Data8);
        serial.setParity(QSerialPort::NoParity);
        serial.setStopBits(QSerialPort::OneStop);
        serial.setFlowControl(QSerialPort::NoFlowControl);

        if (serial.open(QIODevice::ReadWrite))
        {
            connect(&serial, &QSerialPort::readyRead, this, &Serialization::GetValue);
            qDebug() << "Serial port opened successfully on" << serialportid;
        }
        else
        {
            qDebug() << "Failed to open serial port:" << serialportid;
        }
    }
    else
    {
        qDebug() << "Serial port is already open";
    }
}

void Serialization::GetValue()
{
    rxBuffer.append(serial.readAll());

    int newlineIndex = rxBuffer.indexOf('\n');
    while (newlineIndex >= 0)
    {
        QByteArray oneLine = rxBuffer.left(newlineIndex).trimmed();
        rxBuffer.remove(0, newlineIndex + 1);

        monitor += QString(oneLine) + "\n";
        emit monitorUpdated();

        if (oneLine.contains("ok"))
        {
            if (counter < vec.size())
            {
                serial.write(vec[counter].toUtf8() + "\n");
                serial.flush();

                monitor += "Sent: " + vec[counter] + "\n";
                emit monitorUpdated();
                counter++;
            }
        }

        newlineIndex = rxBuffer.indexOf('\n');
    }
}

void Serialization::sendCommand(const QString &command)
{
    if (!serial.isOpen())
    {
        qDebug() << "Serial port is not open!";
        return;
    }

    QString fullCommand = command + "\n";
    serial.write(fullCommand.toUtf8());
    serial.flush();

    monitor += "Sent: " + command + "\n";
    emit monitorUpdated();

    qDebug() << "Sent command:" << command;
}

void Serialization::StartProcess()
{
    if (!serial.isOpen())
    {
        qDebug() << "Serial port is not open!";
        return;
    }

    ReadGCode();
    rxBuffer.clear();
    counter = 0;

    if (!vec.isEmpty())
    {
        serial.write(vec[counter].toUtf8() + "\n");
        serial.flush();

        monitor += "Sent: " + vec[counter] + "\n";
        emit monitorUpdated();

        counter++;
    }
}

void Serialization::ReadGCode()
{
    vec.clear();

    QFile file("file.gcode");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Can't open file.gcode...";
        return;
    }

    QTextStream ss(&file);
    while (!ss.atEnd())
    {
        vec << ss.readLine();
    }
    file.close();
}

QString Serialization::getMonitor()
{
    return monitor;
}
