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

/* This software use the Famirids font from Axel Lymphos. This font
 * is free regading the information on https://www.freefontspro.com/de/15361/famirids.ttf:
 *
 * 14.6.2020:
 * Schriftname: 	    Famirids
 * Schriftdesigner: 	Axel Lymphos
 * Schriftlizenz: 	    Free(Commercial Use Allowed)
 *
 */

#include "hbx.h"
#include "ui_hbx.h"

hbx::hbx(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::hbx)
{
    ui->setupUi(this);
    QFontDatabase fontDB;
    int id = fontDB.addApplicationFont(":Famirids..ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont Famirids(family);
    Famirids.setPointSize(28);
    ui->displayL1->setFont(Famirids);
    ui->displayL2->setFont(Famirids);
    normalheight = this->height();
    extendedWidth = this->width();
    normalWidth = extendedWidth/2;
    this->setFixedSize(normalWidth, normalheight);
}

hbx::~hbx()
{
    delete lx200;
    delete ui;
}

void hbx::Init(QString port)
{
    QPixmap pixmap(":/splash.png");
    QSplashScreen splash(pixmap);
    QCoreApplication::processEvents();
    splash.show();
    splash.showMessage("Check RS232 ports");

    // Show the splash screen for sure 0,5 sec:
    for(int i = 0; i < 50; i++)
    {
        QCoreApplication::processEvents();
        QThread::msleep(10);
    }

    lx200 = new LX200(port);

    if(lx200->IsOpen())
    {
        connect(lx200, &LX200::NewData, this, &hbx::DisplayData);
        timer = new QTimer(this);

        // setup signal and slot
        connect(timer, SIGNAL(timeout()), this, SLOT(MyTimerSlot()));
        // msec
        timer->start(350);
    }
    splash.hide();
}

void hbx::MyTimerSlot()
{
    lx200->ReadDisplay();
}

void hbx::DisplayData(QString firstLine, QString secondLine, QString infoText)
{
    ui->displayL1->setText(firstLine);
    ui->displayL2->setText(secondLine);
    if(infoText.length() > 0)
    {
        this->setFixedSize(extendedWidth, normalheight);
        ui->txtInstruction->setText(infoText);
    }
    else
        this->setFixedSize(normalWidth, normalheight);
}

void hbx::on_pbEnter_pressed()
{
    enterTimer.start();
}

void hbx::on_pbEnter_released()
{
    if(enterTimer.elapsed() > 3000)
    {
        // This was a long press
        lx200->SendKey("10");
    }
    else
    {
        // this was a short press
        lx200->SendKey("13");
    }
}

void hbx::on_pbMode_pressed()
{
    modeTimer.start();
}

void hbx::on_pbMode_released()
{
    if(modeTimer.elapsed() > 3000)
    {
        // This was a long press
        lx200->SendKey("11");
    }
    else
    {
        // this was a short press
        lx200->SendKey("9");
    }
}

void hbx::on_pb1_clicked()
{
    lx200->SendKey("49");
}

void hbx::on_pb2_clicked()
{
    lx200->SendKey("50");
}

void hbx::on_pb3_clicked()
{
    lx200->SendKey("51");
}

void hbx::on_pb4_clicked()
{
    lx200->SendKey("52");
}

void hbx::on_pb5_clicked()
{
    lx200->SendKey("53");
}

void hbx::on_pb6_clicked()
{
    lx200->SendKey("54");
}

void hbx::on_pb7_clicked()
{
    lx200->SendKey("55");
}

void hbx::on_pb8_clicked()
{
    lx200->SendKey("56");
}

void hbx::on_pb9_clicked()
{
    lx200->SendKey("57");
}

void hbx::on_pb0_clicked()
{
    lx200->SendKey("48");
}

void hbx::on_pbUp_clicked()
{
    lx200->SendKey("94");
}

void hbx::on_pbDown_clicked()
{
    lx200->SendKey("118");
}

void hbx::on_pbQuest_clicked()
{
    lx200->SendKey("63");
}

void hbx::on_pbN_pressed()
{
    lx200->SendOther(":Mn#");
}

void hbx::on_pbN_released()
{
    lx200->SendOther(":Qn#");
}

void hbx::on_pbW_pressed()
{
    lx200->SendOther(":Mw#");
}

void hbx::on_pbW_released()
{
    lx200->SendOther(":Qw#");
}

void hbx::on_pbS_pressed()
{
    lx200->SendOther(":Ms#");
}

void hbx::on_pbS_released()
{
    lx200->SendOther(":Qs#");
}

void hbx::on_pbE_pressed()
{
    lx200->SendOther(":Me#");
}

void hbx::on_pbE_released()
{
    lx200->SendOther(":Qe#");
}

void hbx::on_pbCentering_clicked()
{
    lx200->SendOther(":RC#");
    ui->pbCentering->setStyleSheet("background-color:#510202;");
    ui->pbGuiding->setStyleSheet("background-color: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #161616, stop: 1 #323030);");
    ui->pbSlew->setStyleSheet("background-color: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #161616, stop: 1 #323030);");
    ui->pbMax->setStyleSheet("background-color: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #161616, stop: 1 #323030);");
}

void hbx::on_pbGuiding_clicked()
{
    lx200->SendOther(":RG#");
    ui->pbCentering->setStyleSheet("background-color: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #161616, stop: 1 #323030);");
    ui->pbGuiding->setStyleSheet("background-color:#510202;");
    ui->pbSlew->setStyleSheet("background-color: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #161616, stop: 1 #323030);");
    ui->pbMax->setStyleSheet("background-color: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #161616, stop: 1 #323030);");
}

void hbx::on_pbSlew_clicked()
{
    lx200->SendOther(":RM#");
    ui->pbCentering->setStyleSheet("background-color: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #161616, stop: 1 #323030);");
    ui->pbGuiding->setStyleSheet("background-color: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #161616, stop: 1 #323030);");
    ui->pbSlew->setStyleSheet("background-color:#510202;");
    ui->pbMax->setStyleSheet("background-color: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #161616, stop: 1 #323030);");
}

void hbx::on_pbMax_clicked()
{
    lx200->SendOther(":RS#");
    ui->pbCentering->setStyleSheet("background-color: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #161616, stop: 1 #323030);");
    ui->pbGuiding->setStyleSheet("background-color: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #161616, stop: 1 #323030);");
    ui->pbSlew->setStyleSheet("background-color: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #161616, stop: 1 #323030);");
    ui->pbMax->setStyleSheet("background-color:#510202;");
}

void hbx::on_pbFw_clicked()
{
    lx200->SendKey("87");
}

void hbx::on_pbBw_clicked()
{
    lx200->SendKey("69");
}

void hbx::on_pbSetTime_clicked()
{
    lx200->SendOther(":SL16:31:30#");
}
