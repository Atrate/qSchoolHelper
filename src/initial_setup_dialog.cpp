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
#include "initial_setup_dialog.h"
#include "ui_initial_setup_dialog.h"

initial_setup_dialog::initial_setup_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::initial_setup_dialog)
{
    ui->setupUi(this);
}

initial_setup_dialog::~initial_setup_dialog()
{
    delete ui;
}
void initial_setup_dialog::initial_setup()
{
    // Begin — Declare vars and set UI element states
    // ----------------------------------------------
    std::string config_folder = "C:\\ProgramData\\qSchoolHelper\\";
    std::string initial_setup_done = config_folder + "initial_setup_done.txt";

    ui->start_button->setEnabled(false);
    ui->cancel_button->setEnabled(true);
    ui->setup_label->setEnabled(true);
    ui->setup_log->setEnabled(true);
    ui->progress_bar->setEnabled(true);
    ui->progress_bar->setValue(0);
    ui->setup_log->append("Starting initial setup…\n——————————");

    // -------------------
    // More code here soon
    // -------------------

    // Finalize — Create the initial_setup_done.txt file and set UI element states
    // ---------------------------------------------------------------------------
    std::ofstream isdf;
    isdf.open(initial_setup_done);
    isdf << std::endl;
    isdf.close();

    ui->progress_bar->setValue(100);
    ui->setup_log->append("All done!");
    ui->start_button->setEnabled(true);
    ui->cancel_button->setEnabled(false);

}
