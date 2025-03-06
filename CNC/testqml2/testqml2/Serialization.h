#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include <QObject>
#include <QSerialPort>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QVector>
#include <DataHandler.h>

class Serialization : public QObject
{
    Q_OBJECT
public:
    explicit Serialization(QObject *parent = nullptr);
    ~Serialization();
    void GetValue();
    Q_INVOKABLE void StartProcess();
public slots:
    void getSerialPort(QString serialportid);
    Q_INVOKABLE QByteArray SendToSerialMonitor();
private:
    void ReadGCode();

    QSerialPort serial;
    QByteArray monitor;
    QVector<QString> vec;
    int counter = 0;
    QString serialportid;

    QByteArray rxBuffer;
};

#endif // SERIALIZATION_H
