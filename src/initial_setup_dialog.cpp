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
#include <unistd.h>
#include <QThread>
#include <QtConcurrent/QtConcurrentRun>
#include "initial_setup_dialog.h"
#include "ui_initial_setup_dialog.h"
#include "download.h"

namespace fs = std::filesystem;

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
void run_install_bb(const char* bb_exe)
{
    system(bb_exe);
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
    ui->button_box->setEnabled(false);
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

    // Download and install BleachBit
    // ------------------------------
    std::string bb_exe = "BleachBit-3.2.0-setup.exe";
    const char *temp_folder = "C:\\ProgramData\\qSchoolHelper\\tmp";
    if (!fs::exists(temp_folder))
    {
        fs::create_directory(temp_folder);
    }
    chdir(temp_folder);
    curl_dl("https://www.bleachbit.org/download/file/t?file=BleachBit-3.2.0-setup.exe", bb_exe.c_str());
    bb_exe.append("/S /allusers");
    QFuture<void> bb_install = QtConcurrent::run(run_install_bb, bb_exe.c_str());
    while(bb_install.isRunning())
    {
        QCoreApplication::processEvents();
    }

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
    ui->button_box->setEnabled(true);


}
