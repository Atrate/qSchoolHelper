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

#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>
#include <QProcess>
#include "CleaningDialog.h"
#include "InitialSetupDialog.h"
#include "InstallDialog.h"
#include "Procedure.h"
#include "ui_InitialSetupDialog.h"

InitialSetupDialog::InitialSetupDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::InitialSetupDialog)
{
    ui->setupUi(this);
}

InitialSetupDialog::~InitialSetupDialog()
{
    delete ui;
}
bool g_setup_running {};

void InitialSetupDialog::initial_setup()
{
    // Begin — Declare vars and set UI element states
    // ----------------------------------------------
    g_setup_running = true;
    Procedure initial_procedures;
    QObject::connect(&initial_procedures, SIGNAL(progress_description(QString)), this->ui->setup_log,
                     SLOT(append(QString)));
    ui->start_button->setEnabled(false);
    ui->setup_label->setEnabled(true);
    ui->setup_log->setEnabled(true);
    ui->progress_bar->setEnabled(true);
    ui->button_box->setEnabled(false);
    ui->install_check_box->setEnabled(false);
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
    (void) QProcess::execute("cmd", QStringList() << "/c" <<
                             "REG ADD \"HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\" /v \"ShowSyncProviderNotifications\" /t REG_DWORD /d 0 /f");
    // Disable telemetry
    // -----------------
    qInfo() << tr("Disabling telemetry service…\n");
    ui->setup_log->append(tr("Disabling telemetry service…\n"));
    QApplication::processEvents();
    (void) QProcess::execute("cmd", QStringList() << "/c" <<
                             "REG ADD \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Policies\\Microsoft\\Windows\\DataCollection\" /v \"AllowTelemetry\" /t REG_DWORD /d 0 /f");
    (void) QProcess::execute("cmd", QStringList() << "/c" << "net stop DiagTrack");
    (void) QProcess::execute("cmd", QStringList() << "/c" << "sc config DiagTrack start= disabled");
    // Disable search indexing
    // -----------------------
    qInfo() << tr("Disabling search indexing…\n");
    ui->setup_log->append(tr("Disabling search indexing…\n"));
    QApplication::processEvents();
    (void) QProcess::execute("cmd", QStringList() << "/c" << "net stop WSearch");
    (void) QProcess::execute("cmd", QStringList() << "/c" << "sc config WSearch start= disabled");
    // Snap desktop icons to grid and enable auto-arrange
    // --------------------------------------------------
    qInfo() << tr("Enabling icon auto-arrange and snap to grid");
    ui->setup_log->append(tr("Enabling icon auto-arrange and snap to grid"));
    QApplication::processEvents();
    (void) QProcess::execute("cmd", QStringList() << "/c" <<
                             "REG ADD \"HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\Shell\\Bags\\1\\Desktop\" /v \"AllowTelemetry\" /t REG_DWORD /d 1075839525 /f");
    // Disable Windows Visual FX
    // -------------------------
    qInfo() << tr("Disabling visual effects…\n");
    ui->setup_log->append(tr("Disabling visual effects…\n"));
    QApplication::processEvents();
    (void) QProcess::execute("cmd", QStringList() << "/c" <<
                             "REG ADD \"HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\VisualEffects\" /v \"VisualFXSetting\" /t REG_DWORD /d 2 /f");
    // Restart explorer.exe to apply changes
    // -------------------------------------
    (void) QProcess::execute("cmd", QStringList() << "/c" << "taskkill /F /IM explorer.exe & start explorer");
    ui->progress_bar->setValue(10);
    QApplication::processEvents();

    // Run install (all software)
    // --------------------------
    if (ui->install_check_box->isChecked())
    {
        qInfo() << tr("Installing required software. This might (will) take a while…\n");
        ui->setup_log->append(tr("Installing required software. This might (will) take a while…\n"));
        QApplication::processEvents();
        int install_result = initial_procedures.run_install_software(true, true, true, true, true);

        if (install_result == 1)
        {
            qCritical() << tr("The download failed! Please check your Internet connectivity!");
            QMessageBox dl_failure_box;
            dl_failure_box.setText(tr("The download failed! Please check your Internet connectivity!"));
            dl_failure_box.setModal(true);
            dl_failure_box.exec();
        }
        else if (install_result == 2)
        {
            qCritical() << tr("The installation failed! Please try installing the program manually!");
            QMessageBox install_failure_box;
            install_failure_box.setText(tr("The installation failed! Please try installing the program manually!"));
            install_failure_box.setModal(true);
            install_failure_box.exec();
        }
    }

    // Download and install BleachBit
    // ------------------------------
    QString bb_path = "C:\\Program Files (x86)\\BleachBit\\bleachbit_console.exe";

    if (!QDir().exists(bb_path))
    {
        qInfo() << tr("Installing BleachBit (utility used for computer cleaning)…\n");
        ui->setup_log->append(tr("Installing BleachBit (utility used for computer cleaning)…\n"));
        QApplication::processEvents();
        int bb_install_result = initial_procedures.run_install_bb();

        if (bb_install_result == 1)
        {
            qCritical() << tr("The download failed! Please check your Internet connectivity!");
            QMessageBox dl_failure_box;
            dl_failure_box.setText(tr("The download failed! Please check your Internet connectivity!"));
            dl_failure_box.setModal(true);
            dl_failure_box.exec();
        }
        else if (bb_install_result == 2)
        {
            qCritical() << tr("The installation failed! Please try installing the program manually!");
            QMessageBox install_failure_box;
            install_failure_box.setText(tr("The installation failed! Please try installing the program manually!"));
            install_failure_box.setModal(true);
            install_failure_box.exec();
        }
    }

    // Run extended cleaner
    // --------------------
    if (QDir().exists(bb_path))
    {
        qInfo() << tr("Cleaning temporary files…\n");
        ui->setup_log->append(tr("Cleaning temporary files…\n"));
        initial_procedures.run_clean(true);
        QApplication::processEvents();
    }

    // Finalize — Create the initial_setup_done.txt file and set UI element states
    // ---------------------------------------------------------------------------
    QString initial_setup_done = config_folder + "initial_setup_done.txt";
    QFile isdf(initial_setup_done);
    isdf.open(QIODevice::WriteOnly);
    isdf.write("");
    isdf.close();
    g_setup_running = false;
    ui->progress_bar->setValue(100);
    qInfo() << tr("All done!");
    ui->setup_log->append(tr("All done!"));
    ui->install_check_box->setEnabled(true);
    ui->start_button->setEnabled(true);
    ui->button_box->setEnabled(true);
}
// Ignore close events if a process is running
// -------------------------------------------
void InitialSetupDialog::closeEvent(QCloseEvent* event)
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
