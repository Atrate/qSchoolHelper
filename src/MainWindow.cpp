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

#include <fstream>
#include <filesystem>
#include <QDebug>
#include "AboutDialog.h"
#include "CleaningDialog.h"
#include "HelpDialog.h"
#include "InitialSetupDialog.h"
#include "InstallDialog.h"
#include "MainWindow.h"
#include "ProblemDialog.h"
#include "ui_main_window.h"

namespace fs = std::filesystem;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    window_setup();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::window_setup()
{
    // Set-up the config folder. Disable initial_setup_button if initial setup has been run.
    // -------------------------------------------------------------------------------------
    std::string config_folder = "C:\\ProgramData\\qSchoolHelper\\";

    if (!fs::exists(config_folder))
    {
        fs::create_directory(config_folder);
    }

#ifdef QT_NO_DEBUG
    std::string initial_setup_done = config_folder + "initial_setup_done.txt";

    if (fs::exists(initial_setup_done))
    {
        ui->software_button->setToolTip(tr("Install missing software"));
        ui->problem_button->setToolTip(tr("Help with common problems"));
        ui->clean_button->setToolTip(tr("Clean junk and temporary files"));
        ui->help_button->setToolTip(tr("Open the application usage guide"));
        ui->initial_setup_button->setEnabled(false);
        ui->initial_setup_button->setToolTip(tr("Initial Setup has already been run on this computer. "
                                                "If you with to run it again, please select it from File -> Initial Setup."));
    }
    else
    {
        QString disabled_tooltip = tr("Please run Initial Setup first!");
        ui->problem_button->setEnabled(false);
        ui->clean_button->setEnabled(false);
        ui->software_button->setEnabled(false);
        ui->help_button->setEnabled(false);
        ui->software_button->setToolTip(disabled_tooltip);
        ui->problem_button->setToolTip(disabled_tooltip);
        ui->clean_button->setToolTip(disabled_tooltip);
        ui->help_button->setToolTip(disabled_tooltip);
    }

#else
    QString debug_tooltip = "All buttons enabled due to running a debug build";
    qDebug() << debug_tooltip;
    ui->software_button->setToolTip(debug_tooltip);
    ui->problem_button->setToolTip(debug_tooltip);
    ui->clean_button->setToolTip(debug_tooltip);
    ui->help_button->setToolTip(debug_tooltip);
    ui->initial_setup_button->setToolTip(debug_tooltip);
#endif
}
void MainWindow::on_problem_button_clicked()
{
    problem = new ProblemDialog(this);
    problem->show();
}
void MainWindow::on_initial_setup_button_clicked()
{
    initial_setup = new InitialSetupDialog(this);
    initial_setup->show();
}
void MainWindow::on_software_button_clicked()
{
    install = new InstallDialog(this);
    install->show();
}
void MainWindow::on_clean_button_clicked()
{
    cleaning = new CleaningDialog(this);
    cleaning->show();
}
void MainWindow::on_action_open_help_triggered()
{
    help = new HelpDialog(this);
    help->show();
}
void MainWindow::on_action_about_triggered()
{
    about = new AboutDialog(this);
    about->show();
}
void MainWindow::on_action_initial_setup_triggered()
{
    on_initial_setup_button_clicked();
}
void MainWindow::on_help_button_clicked()
{
    on_action_open_help_triggered();
}
