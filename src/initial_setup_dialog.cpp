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

#include <filesystem>
#include <fstream>
#include <unistd.h>
#include <QThread>
#include <QtConcurrent/QtConcurrentRun>
#include <QCloseEvent>
#include "cleaning_dialog.h"
#include "download.h"
#include "initial_setup_dialog.h"
#include "install_dialog.h"
#include "ui_initial_setup_dialog.h"

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
bool g_setup_running {};
void run_install_bb(const char* bb_exe)
{
    (void) system(bb_exe);
}
void initial_setup_dialog::install_bb()
{
    std::string temp_folder = "C:\\ProgramData\\qSchoolHelper\\tmp";
    if (!fs::exists(temp_folder))
    {
        fs::create_directory(temp_folder);
    }
    chdir(temp_folder.c_str());
    std::string bb_exe = "BleachBit-3.2.0-setup.exe";
    std::string bb_url = "https://download.bleachbit.org/BleachBit-3.2.0-setup.exe";
    fs::remove(bb_exe);
    QFuture<int> bb_dl = QtConcurrent::run(curl_dl, bb_url.c_str(), bb_exe.c_str());
    while(bb_dl.isRunning())
    {
        QApplication::processEvents();
    }
    bb_dl.~QFuture();
    if (fs::file_size(bb_exe) > 1024)
    {
        bb_exe.append("/S /allusers");
        QFuture<void> bb_install = QtConcurrent::run(run_install_bb, bb_exe.c_str());
        while(bb_install.isRunning())
        {
            QApplication::processEvents();
        }
        bb_install.~QFuture();
        fs::remove(bb_exe);
    }
}
void initial_setup_dialog::initial_setup()
{
    // Begin — Declare vars and set UI element states
    // ----------------------------------------------
    g_setup_running = true;
    std::string config_folder = "C:\\ProgramData\\qSchoolHelper\\";
    std::string initial_setup_done = config_folder + "initial_setup_done.txt";

    ui->start_button->setEnabled(false);
    ui->setup_label->setEnabled(true);
    ui->setup_log->setEnabled(true);
    ui->progress_bar->setEnabled(true);
    ui->button_box->setEnabled(false);
    ui->progress_bar->setValue(0);
    ui->setup_log->clear();
    ui->setup_log->append(tr("Starting initial setup…\n——————————"));
    QApplication::processEvents();

    // Disable ads
    // -----------
    ui->setup_log->append(tr("Disabling Windows Explorer ads…\n"));
    QApplication::processEvents();
    (void) system("REG ADD \"HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\" /v \"ShowSyncProviderNotifications\" /t REG_DWORD /d 0 /f");

    // Disable telemetry
    // -----------------
    ui->setup_log->append(tr("Disabling telemetry service…\n"));
    QApplication::processEvents();
    (void) system("REG ADD \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Policies\\Microsoft\\Windows\\DataCollection\" /v \"AllowTelemetry\" /t REG_DWORD /d 0 /f");
    (void) system("net stop DiagTrack");
    (void) system("sc config DiagTrack start= disabled");

    // Disable search indexing
    // -----------------------
    ui->setup_log->append(tr("Disabling search indexing…\n"));
    QApplication::processEvents();
    (void) system("net stop WSearch");
    (void) system("sc config WSearch start= disabled");

    // Disable Windows Visual FX
    // -------------------------
    ui->setup_log->append(tr("Disabling visual effects…\n"));
    QApplication::processEvents();
    (void) system("REG ADD \"HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\VisualEffects\" /v \"VisualFXSetting\" /t REG_DWORD /d 2 /f");

    // Restart explorer.exe to apply changes
    // -------------------------------------
    (void) system("taskkill /F /IM explorer.exe & start explorer");
    ui->progress_bar->setValue(10);
    QApplication::processEvents();


    // Run install (all software)
    // --------------------------
    if(ui->install_check_box->isChecked())
    {
        ui->setup_log->append(tr("Installing required software. This might (will) take a while…\n"));
        QApplication::processEvents();
        install_dialog *id = new install_dialog();
        id->install();
        delete id;
    }

    // Download and install BleachBit
    // ------------------------------
    std::string bb_path = "C:\\Program Files (x86)\\BleachBit\\bleachbit_console.exe";
    if (!fs::exists(bb_path))
    {
        ui->setup_log->append(tr("Installing BleachBit (utility used for computer cleaning)…\n"));
        QApplication::processEvents();
        install_bb();
    }
    // Run extended cleaner
    // --------------------
    if (fs::exists(bb_path))
    {
        ui->setup_log->append(tr("Cleaning temporary files…\n"));
        QApplication::processEvents();
        cleaning_dialog *cl = new cleaning_dialog();
        cl->clean_extended();
        delete cl;
    }

    // Finalize — Create the initial_setup_done.txt file and set UI element states
    // ---------------------------------------------------------------------------
    std::ofstream isdf;
    isdf.open(initial_setup_done);
    isdf << std::endl;
    isdf.close();

    g_setup_running = false;
    ui->progress_bar->setValue(100);
    ui->setup_log->append(tr("All done!"));
    ui->start_button->setEnabled(true);
    ui->button_box->setEnabled(true);
}
void initial_setup_dialog::closeEvent(QCloseEvent *event)
{
    if (g_setup_running)
    {
        event->ignore();
    }
    else
    {
        event->accept();
    }
}
