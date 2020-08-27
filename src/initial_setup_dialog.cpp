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
#include <QMessageBox>
#include <QtConcurrent/QtConcurrentRun>
#include <QCloseEvent>
#include "cleaning_dialog.h"
#include "initial_setup_dialog.h"
#include "install_dialog.h"
#include "procedures.h"
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


void initial_setup_dialog::initial_setup()
{
    // Begin — Declare vars and set UI element states
    // ----------------------------------------------
    g_setup_running = true;
    std::string config_folder = "C:\\ProgramData\\qSchoolHelper\\";
    std::string initial_setup_done = config_folder + "initial_setup_done.txt";

    procedures* initial_procedures = new procedures();

    ui->start_button->setEnabled(false);
    ui->setup_label->setEnabled(true);
    ui->setup_log->setEnabled(true);
    ui->progress_bar->setEnabled(true);
    ui->button_box->setEnabled(false);
    ui->progress_bar->setValue(0);
    ui->setup_log->clear();
    qInfo() << tr("Starting initial setup…\n——————————");
    ui->setup_log->append(tr("Starting initial setup…\n——————————"));
    QApplication::processEvents();

    // Disable ads
    // -----------
    qInfo() << tr("Disabling Windows Explorer ads…\n");
    ui->setup_log->append(tr("Disabling Windows Explorer ads…\n"));
    QApplication::processEvents();
    (void) system("REG ADD \"HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\" /v \"ShowSyncProviderNotifications\" /t REG_DWORD /d 0 /f");

    // Disable telemetry
    // -----------------
    qInfo() << tr("Disabling telemetry service…\n");
    ui->setup_log->append(tr("Disabling telemetry service…\n"));
    QApplication::processEvents();
    (void) system("REG ADD \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Policies\\Microsoft\\Windows\\DataCollection\" /v \"AllowTelemetry\" /t REG_DWORD /d 0 /f");
    (void) system("net stop DiagTrack");
    (void) system("sc config DiagTrack start= disabled");

    // Disable search indexing
    // -----------------------
    qInfo() << tr("Disabling search indexing…\n");
    ui->setup_log->append(tr("Disabling search indexing…\n"));
    QApplication::processEvents();
    (void) system("net stop WSearch");
    (void) system("sc config WSearch start= disabled");

    // Disable Windows Visual FX
    // -------------------------
    qInfo() << tr("Disabling visual effects…\n");
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
        qInfo() << tr("Installing required software. This might (will) take a while…\n");
        ui->setup_log->append(tr("Installing required software. This might (will) take a while…\n"));
        QApplication::processEvents();
        int install_result = initial_procedures->run_install_software(true,true,true,true,true);
        if(install_result == 1)
        {
            qCritical() << tr("The download failed! Please check your Internet connectivity!");
            QMessageBox dl_failure_box;
            dl_failure_box.setText(tr("The download failed! Please check your Internet connectivity!"));
            dl_failure_box.setModal(true);
            dl_failure_box.exec();
        }
        else if(install_result == 2)
        {
            qCritical() << tr("The installation failed! Please try installing the program manually!");
            QMessageBox install_failure_box;
            install_failure_box.setText(tr("The installation failed! Please try installing the program manually!"));
            install_failure_box.setModal(true);
            install_failure_box.exec();        }
        else
        {
            qCritical() << tr("The installation failed!");
            QMessageBox unknown_failure_box;
            unknown_failure_box.setText(tr("The installation failed!"));
            unknown_failure_box.setModal(true);
            unknown_failure_box.exec();
        }
    }

    // Download and install BleachBit
    // ------------------------------
    std::string bb_path = "C:\\Program Files (x86)\\BleachBit\\bleachbit_console.exe";
    if (!fs::exists(bb_path))
    {
        qInfo() << tr("Installing BleachBit (utility used for computer cleaning)…\n");
        ui->setup_log->append(tr("Installing BleachBit (utility used for computer cleaning)…\n"));
        QApplication::processEvents();
        int bb_install_result = initial_procedures->run_install_bb();
        if(bb_install_result == 1)
        {
            qCritical() << tr("The download failed! Please check your Internet connectivity!");
            QMessageBox dl_failure_box;
            dl_failure_box.setText(tr("The download failed! Please check your Internet connectivity!"));
            dl_failure_box.setModal(true);
            dl_failure_box.exec();
        }
        else if(bb_install_result == 2)
        {
            qCritical() << tr("The installation failed! Please try installing the program manually!");
            QMessageBox install_failure_box;
            install_failure_box.setText(tr("The installation failed! Please try installing the program manually!"));
            install_failure_box.setModal(true);
            install_failure_box.exec();
        }
        else
        {
            qCritical() << tr("The installation failed!");
            QMessageBox unknown_failure_box;
            unknown_failure_box.setText(tr("The installation failed!"));
            unknown_failure_box.setModal(true);
            unknown_failure_box.exec();
        }
    }
    // Run extended cleaner
    // --------------------
    if (fs::exists(bb_path))
    {
        qInfo() << tr("Cleaning temporary files…\n");
        ui->setup_log->append(tr("Cleaning temporary files…\n"));
        initial_procedures->run_clean(true);
        QApplication::processEvents();
    }

    // Finalize — Create the initial_setup_done.txt file and set UI element states
    // ---------------------------------------------------------------------------
    std::ofstream isdf;
    isdf.open(initial_setup_done);
    isdf << std::endl;
    isdf.close();

    g_setup_running = false;
    ui->progress_bar->setValue(100);
    qInfo() << tr("All done!");
    ui->setup_log->append(tr("All done!"));
    ui->start_button->setEnabled(true);
    ui->button_box->setEnabled(true);
}
// Ignore close events if a process is running
// -------------------------------------------
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
