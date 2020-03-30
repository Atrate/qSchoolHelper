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
    ui->buttonBox->setEnabled(false);
    ui->progress_bar->setValue(0);
    ui->setup_log->append("Starting initial setup…\n——————————");

    // Disable ads
    //system("RED ADD \"HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\" /v \"ShowSyncProviderNotifications\" /t REG_DWORD /d 0 /f");

    // Disable telemetry
    //system("REG ADD \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Policies\\Microsoft\\Windows\\DataCollection\" /v \"AllowTelemetry\" /t REG_DWORD /d 0 /f");
    ui->setup_log->append("Disabling telemetry service…");
    system("net stop DiagTrack");
    system("sc config DiagTrack start= disabled");

    ui->setup_log->append("Disabling search indexing...\n");
    system("net stop WSearch");
    system("sc config WSearch start= disabled");
    // -------------------
    // More code here soon
    // -------------------

    // Run extended cleaner
    // --------------------

    // Run install (all software)
    // --------------------------

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
