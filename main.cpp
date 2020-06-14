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

#include "hbx.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    // The port as commandline argument was never tested!
    QString argPort = "";
    QApplication a(argc, argv);

    if(argc == 2)
    {
        // Port given (the old way...)
        argPort = QString(argv[1]);
    }
    hbx w;
    w.Init(argPort);
    w.show();

    return a.exec();
}
