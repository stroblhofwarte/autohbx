/*
 * This file is part of the AutoHBX software.
 * Copyright (c) 2020 Othmar Ehrhardt
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LX200_H
#define LX200_H

#include <QObject>
#include <QSerialPort>
#include <QThread>
#include <QSerialPortInfo>
#include <QCoreApplication>
#include <QFile>

#include <cstdlib>
#include <iostream>

class LX200 : public QObject
{
    Q_OBJECT
public:
    LX200(QString port);
    ~LX200();
    bool IsOpen();
    void ReadDisplay();
    QString Line1();
    QString Line2();
    QString InfoText();
    void SendKey(QString key);
    void SendOther(QString cmd);
    QString ComPort();

signals:
    void NewData(QString firstLine, QString secondLine, QString infoText);

private:
    void processInputBuffer(QByteArray newData);
    bool OpenAutostar(QString device);
    bool TestAutostar(QString device);
    void CloseAutostar();

    QSerialPort serial;
    bool isOpen;
    QString firstLine;
    QString secondLine;
    QString infoText;
    QString inputBuffer;
    bool volatile receiving;
    QString nextKey;
    QString comPort;


};

#endif // LX200_H
