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

#ifndef HBX_H
#define HBX_H

#include <QMainWindow>
#include <QTimer>
#include <QFontDatabase>
#include <QElapsedTimer>
#include <QPixmap>
#include <QSplashScreen>
#include <QCoreApplication>
#include <QThread>

#include "lx200.h"

namespace Ui {
class hbx;
}

class hbx : public QMainWindow
{
    Q_OBJECT

public:
    explicit hbx(QWidget *parent = 0);
    ~hbx();

    void Init(QString port);

private:
    Ui::hbx *ui;
    LX200 *lx200;
    QTimer *timer;
    int normalheight;
    int normalWidth;
    int extendedWidth;

    QElapsedTimer enterTimer;
    QElapsedTimer modeTimer;
    QElapsedTimer gotoTimer;

public slots:
        void MyTimerSlot();
        void DisplayData(QString firstLine, QString secondLine, QString infoText);

private slots:
        void on_pbEnter_pressed();
        void on_pbEnter_released();
        void on_pbMode_pressed();
        void on_pbMode_released();
        void on_pb1_clicked();
        void on_pb2_clicked();
        void on_pb3_clicked();
        void on_pb4_clicked();
        void on_pb5_clicked();
        void on_pb6_clicked();
        void on_pb7_clicked();
        void on_pb8_clicked();
        void on_pb9_clicked();
        void on_pb0_clicked();
        void on_pbUp_clicked();
        void on_pbDown_clicked();
        void on_pbQuest_clicked();
        void on_pbN_pressed();
        void on_pbN_released();
        void on_pbW_pressed();
        void on_pbW_released();
        void on_pbS_pressed();
        void on_pbS_released();
        void on_pbE_pressed();
        void on_pbE_released();
        void on_pbCentering_clicked();
        void on_pbGuiding_clicked();
        void on_pbSlew_clicked();
        void on_pbMax_clicked();
        void on_pbFw_clicked();
        void on_pbBw_clicked();
        void on_pbSetTime_clicked();
};

#endif // HBX_H
