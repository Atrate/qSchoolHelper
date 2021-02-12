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
#include <QSettings>
#include <QStyleFactory>
#include <QTranslator>
#include "MainWindow.h"

void set_theme()
{
// Special thanks to QuantumCD for this theme. See: https://gist.github.com/QuantumCD/6245215
// ------------------------------------------------------------------------------------------
#ifdef Q_OS_WIN
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", QSettings::NativeFormat);

    if (settings.value("AppsUseLightTheme") == 0)
    {
        qApp->setStyle(QStyleFactory::create("Fusion"));
        QPalette darkPalette;
        darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::WindowText, Qt::white);
        darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
        darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
        darkPalette.setColor(QPalette::ToolTipText, Qt::white);
        darkPalette.setColor(QPalette::Text, Qt::white);
        darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::ButtonText, Qt::white);
        darkPalette.setColor(QPalette::BrightText, Qt::red);
        darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::HighlightedText, Qt::black);
        qApp->setPalette(darkPalette);
        qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
    }

#endif
}
int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    set_theme();
    QTranslator qsh_translator;
    QStringList languages;
    languages << "English" << "Polish";
    QString lang = QInputDialog::getItem(NULL, "Select Language", "Language", languages);

    if (lang == "Polish")
    {
        const QString trans_file = ":/i18n/qSchoolHelper_pl_PL.qm";
        qsh_translator.load(trans_file);
        qApp->installTranslator(&qsh_translator);
    }

    MainWindow w;
    w.show();
    return a.exec();
}

