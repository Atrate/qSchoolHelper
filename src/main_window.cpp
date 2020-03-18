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
    config_setup();
}

main_window::~main_window()
{
    delete ui;
}
void main_window::config_setup()
{
    // Set-up the config folder
    // ------------------------
    std::string config_folder = "C:\\ProgramData\\qSchoolHelper\\";
    std::string initial_setup_done = config_folder + "initial_setup_done.txt";
    if (!fs::exists(config_folder))
    {
        fs::create_directory(config_folder);
    }

    if (fs::exists(initial_setup_done))
    {
        ui->initial_setup_button->setEnabled(false);
        // Move to initial_setup on button press
        //std::ofstream isdf;
        //isdf.open(initial_setup_done);
        //isdf << std::endl;
        //isdf.close();
    }
}
void main_window::problem_button_clicked()
{
    problem = new problem_dialog(this);
    problem->show();
}
void main_window::initial_setup_triggered()
{
    initial_setup = new initial_setup_dialog(this);
    initial_setup->show();
}
void main_window::about_triggered()
{
    about = new about_dialog(this);
    about->show();
}
void main_window::install_button_clicked()
{
    install = new install_dialog(this);
    install->show();
}
void main_window::cleaning_button_clicked()
{
    cleaning = new cleaning_dialog(this);
    cleaning->show();
}
void main_window::help_triggered()
{
    help = new help_dialog(this);
    help->show();
}
