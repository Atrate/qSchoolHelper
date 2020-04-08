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

#include "main_window.h"
#include "ui_main_window.h"
#include "about_dialog.h"
#include "problem_dialog.h"
#include "install_dialog.h"
#include "initial_setup_dialog.h"
#include "cleaning_dialog.h"
#include "help_dialog.h"

namespace fs = std::filesystem;

main_window::main_window(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::main_window)
{
    ui->setupUi(this);
    window_setup();
}

main_window::~main_window()
{
    delete ui;
}
void main_window::window_setup()
{
    // Set-up the config folder. Disable initial_setup_button if initial setup has been run.
    // -------------------------------------------------------------------------------------
    std::string config_folder = "C:\\ProgramData\\qSchoolHelper\\";
    std::string initial_setup_done = config_folder + "initial_setup_done.txt";
    if (!fs::exists(config_folder))
    {
        fs::create_directory(config_folder);
    }

    if (fs::exists(initial_setup_done))
    {
        ui->software_button->setToolTip("Install missing software");
        ui->problem_button->setToolTip("Help with common problems");
        ui->clean_button->setToolTip("Clean junk and temporary files");
        ui->help_button->setToolTip("");
        ui->initial_setup_button->setEnabled(false);
        ui->initial_setup_button->setToolTip("Initial Setup has already been run on this computer. "
                                             "If you with to run it again, please select it from File -> Initial Setup.");
    }
    else
    {
        QString disabled_tooltip = "Please run Initial Setup first!";
        ui->problem_button->setEnabled(false);
        ui->clean_button->setEnabled(false);
        ui->software_button->setEnabled(false);
        ui->help_button->setEnabled(false);
        ui->software_button->setToolTip(disabled_tooltip);
        ui->problem_button->setToolTip(disabled_tooltip);
        ui->clean_button->setToolTip(disabled_tooltip);
        ui->help_button->setToolTip(disabled_tooltip);
    }
}
void main_window::on_problem_button_clicked()
{
    problem = new problem_dialog(this);
    problem->show();
}
void main_window::on_initial_setup_button_clicked()
{
    initial_setup = new initial_setup_dialog(this);
    initial_setup->show();
}
void main_window::on_software_button_clicked()
{
    install = new install_dialog(this);
    install->show();
}
void main_window::on_clean_button_clicked()
{
    cleaning = new cleaning_dialog(this);
    cleaning->show();
}
void main_window::on_action_open_help_triggered()
{
    help = new help_dialog(this);
    help->show();
}
void main_window::on_action_about_triggered()
{
    about = new about_dialog(this);
    about->show();
}
void main_window::on_action_initial_setup_triggered()
{
    on_initial_setup_button_clicked();
}
void main_window::on_help_button_clicked()
{
    on_action_open_help_triggered();
}
