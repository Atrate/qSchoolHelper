/*
 * Copyright (C) 2020 Atrate <atrate@protonmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <QApplication>
#include <QInputDialog>
#include <QTranslator>
#include "MainWindow.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    QTranslator qsh_translator;
    QStringList languages;
    languages << "English" << "Polish";
    QString lang = QInputDialog::getItem(NULL, "Select Language", "Language", languages);

    if (lang == "Polish")
    {
        QString trans_file = ":/i18n/qSchoolHelper_pl_PL.qm";
        qsh_translator.load(trans_file);
        qApp->installTranslator(&qsh_translator);
    }

    MainWindow w;
    w.show();
    return a.exec();
}

