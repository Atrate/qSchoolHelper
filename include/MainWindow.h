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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "AboutDialog.h"
#include "CleaningDialog.h"
#include "HelpDialog.h"
#include "InitialSetupDialog.h"
#include "InstallDialog.h"
#include "ProblemDialog.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        MainWindow(QWidget* parent = nullptr);
        ~MainWindow();

    private:
        Ui::MainWindow* ui;
        AboutDialog* about;
        CleaningDialog* cleaning;
        HelpDialog* help;
        InitialSetupDialog* initial_setup;
        InstallDialog* install;
        ProblemDialog* problem;
        const QString config_folder = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        void window_setup();

    private slots:
        void on_action_about_triggered();
        void on_action_initial_setup_triggered();
        void on_action_open_help_triggered();
        void on_clean_button_clicked();
        void on_initial_setup_button_clicked();
        void on_problem_button_clicked();
        void on_software_button_clicked();
        void on_help_button_clicked();
};
#endif // MAINWINDOW_H
