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

#include "main_window.h"

#include <QApplication>
#include <QTranslator>
#include <QLocale>

void load_translation()
{
    QTranslator qsh_translator;
    QLocale locale = QLocale::system();

    QString bin_path = qApp->applicationDirPath();
    QString src_app_trans_path = bin_path + QLatin1String("/data/translations");
    qsh_translator.load(locale, "translation", "_", src_app_trans_path);

    qApp->installTranslator(&qsh_translator);
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    load_translation();
    main_window w;
    w.show();
    return a.exec();
}

