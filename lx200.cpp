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

#include "lx200.h"

LX200::LX200(QString port)
{
    if(port.length() > 0)
    {
        // Port given, use this one:
        isOpen = OpenAutostar(comPort);
    }
    else
    {
        QCoreApplication::processEvents();
        std::cout << "Check for autostar:" << std::endl;
        // Autodetect the Autostar
        QList<QSerialPortInfo> comports;
        comports = QSerialPortInfo::availablePorts();

        // CHecking for the port can be so slow!
        // Simpl hack: Store the last port and try this first!
        QFile fileCheck("~.autostar.port");
        if (fileCheck.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QString lastPort = fileCheck.readLine();
            fileCheck.close();
        }

        for( int i=0; i<comports.count(); ++i )
        {
            if(TestAutostar(lastPort))
            {
                comPort = lastPort;
                break;
            }
            QCoreApplication::processEvents();
            std::cout << "Check " << comports[i].portName().toStdString().c_str() << std::endl;
            bool ret = TestAutostar(comports[i].portName());
            if(ret)
            {
                comPort = comports[i].portName();
                QFile fileOut("~.autostar.port");
                if (fileOut.open(QIODevice::WriteOnly | QIODevice::Text))
                {
                    QTextStream out(&fileOut);
                    out << comPort << "\n";
                    fileOut.close();
                }
                break;
            }
        }
        QCoreApplication::processEvents();
        std::cout << "Use port: " << comPort.toStdString().c_str() << std::endl;
        isOpen = OpenAutostar(comPort);
    }
}

LX200::~LX200()
{
    CloseAutostar();
}

void LX200::CloseAutostar()
{
    if(serial.isOpen())
        serial.close();
}

bool LX200::TestAutostar(QString device)
{
    serial.setPortName("/dev/" + device);
    serial.setBaudRate(QSerialPort::Baud9600);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);

    if (serial.open(QIODevice::ReadWrite))
    {
        QString info = "";

        serial.write(":GVP#");
        serial.waitForBytesWritten();
        // The autostar is slow, the raspberry pi also!
        serial.waitForReadyRead(2000);
        info.append(serial.readAll());
        while (serial.waitForReadyRead(100))
        {
            // If we catch the interface of a GPS receiver, this
            // thing will not stop to send something!
            if(info.length() > 16) break;
            info.append(serial.readAll());
        }

        serial.close();
        std::cout << "Device answer: " << info.toStdString().c_str() << std::endl;
        if(info.contains("Autostar")) return true;
        if(info.contains("Audiostar")) return true; // NOT TESTED!
        std::cout << "NOT A AUTOSTAR!" << std::endl;
        return false;
    }
    std::cout << "Port could not be open" << std::endl;
    return false;
}

bool LX200::OpenAutostar(QString device)
{
    serial.setPortName("/dev/" + device);
    serial.setBaudRate(QSerialPort::Baud9600);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);

    if (serial.open(QIODevice::ReadWrite))
    {
        QObject::connect(&serial, &QSerialPort::readyRead, [&]
        {
            QByteArray data =  serial.readAll();
            processInputBuffer(data);
        });
        QObject::connect(&serial,
                             static_cast<void(QSerialPort::*)(QSerialPort::SerialPortError)>
                             (&QSerialPort::error),
                             [&](QSerialPort::SerialPortError error)
        {
            //this is called when a serial communication error occurs

        });
        return true;
    }
    return false;
}

void LX200::processInputBuffer(QByteArray newData)
{
    inputBuffer.append(newData);
    int idx = inputBuffer.indexOf('#');
    if(idx < 0) return;
    QString displayString = inputBuffer.left(idx);
    inputBuffer.remove(0,idx+1);
    if(displayString.at(0).toLatin1() == 0x00)
    {
        // For some reasons the hbx sends a Zero byte at the beginning
        // of a normal display message. Perhaps 'clear'?
        displayString.remove(0,1);
    }

    if(displayString.length() > 15)
    {
        // Seems to be a complete display string
        // The first line seems always 16 bytes
        firstLine = displayString.left(16);
        displayString.remove(0,16);
        if(displayString.length() > 16)
        {
            // The rest is more than one line, this is a info text
            secondLine = "(see info)";
            infoText = displayString;
        }
        else
        {
            infoText = "";
            secondLine = displayString;
        }
        emit NewData(firstLine, secondLine, infoText);
    }
    receiving = false;
}

bool LX200::IsOpen()
{
    return isOpen;
}

void LX200::ReadDisplay()
{
    if(receiving) return;
    if(nextKey.length() > 0)
    {
        serial.write(nextKey.toStdString().c_str());
        serial.waitForBytesWritten();
        nextKey = "";
    }
    serial.write(":ED#");
    serial.waitForBytesWritten();
    receiving = true;
}

QString LX200::ComPort()
{
    return comPort;
}

QString LX200::Line1()
{
    return firstLine;
}

QString LX200::Line2()
{
    return secondLine;
}

QString LX200::InfoText()
{
    return infoText;
}

void LX200::SendKey(QString key)
{
    nextKey = ":EK" + key + "#";
}

void LX200::SendOther(QString cmd)
{
    nextKey = cmd;
}
