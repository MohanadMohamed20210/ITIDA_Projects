#include "Serialization.h"

extern DataHandler dataHandler;

Serialization::Serialization(QObject *parent)
    : QObject{parent}
{
    connect(&dataHandler, &DataHandler::sendSerialData, this, &Serialization::getSerialPort);

    monitor = "Begin the serialization ❤️__❤️\n";
}

Serialization::~Serialization()
{
    if (serial.isOpen())
    {
        serial.close();
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
        monitor += oneLine + "\n";

        if (oneLine.contains("ok"))
        {
            if (counter < vec.size())
            {
                serial.write(vec[counter].toUtf8() + "\n");
                serial.flush();

                monitor += vec[counter].toUtf8() + "\n";
                counter++;
            }
        }

        newlineIndex = rxBuffer.indexOf('\n');
    }
}

void Serialization::StartProcess()
{
    static bool flag = false;

    if (!flag)
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
            flag = true;
        }
        else
        {
            qDebug() << "Failed to open port:" << serialportid;
            flag = false;
            return;
        }
    }

    if (serial.isOpen() && flag)
    {
        ReadGCode();
        monitor.clear();
        rxBuffer.clear();
        counter = 0;
        if (!vec.isEmpty())
        {
            serial.write(vec[counter].toUtf8() + "\n");
            serial.flush();
            monitor += vec[counter].toUtf8() + "\n";
            counter++;
        }
    }
    else
    {
        flag = false;
    }
}

void Serialization::getSerialPort(QString serialportid)
{
    this->serialportid = serialportid;
}

QByteArray Serialization::SendToSerialMonitor()
{
    return monitor;
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
