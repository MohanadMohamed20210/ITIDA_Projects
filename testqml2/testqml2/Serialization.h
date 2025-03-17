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
    Q_PROPERTY(QString monitor READ getMonitor NOTIFY monitorUpdated)

public:
    explicit Serialization(QObject *parent = nullptr);
    ~Serialization();

    Q_INVOKABLE void StartProcess();
    Q_INVOKABLE void sendCommand(const QString &command);

public slots:
    void getSerialPort(QString serialportid);
    Q_INVOKABLE QString getMonitor();
    void GetValue();

signals:
    void monitorUpdated();

private:
    void ReadGCode();

    QSerialPort serial;
    QString monitor;
    QVector<QString> vec;
    int counter = 0;
    QString serialportid;
    QByteArray rxBuffer;
};

#endif // SERIALIZATION_H
