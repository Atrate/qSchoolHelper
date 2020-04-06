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
#include <QTranslator>
#include <QInputDialog>
#include "main_window.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator qsh_translator;
    QString bin_path = qApp->applicationDirPath();
    QString trans_path = bin_path + QString("/data/translations/");
    QStringList languages;
    languages << "English" << "Polish";

    QString lang = QInputDialog::getItem(NULL, "Select Language", "Language", languages);
    if (lang == "Polish")
    {
        trans_path.append("qSchoolHelper_pl_PL.qm");
        qsh_translator.load(trans_path);
        qApp->installTranslator(&qsh_translator);
    }

    main_window w;
    w.show();
    return a.exec();
}

