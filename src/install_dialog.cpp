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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <QThread>
#include <QtConcurrent/QtConcurrentRun>
#include <QMessageBox>
#include <QCloseEvent>
#include "install_dialog.h"
#include "procedures.h"
#include "ui_install_dialog.h"

namespace fs = std::filesystem;

install_dialog::install_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::install_dialog)
{
    ui->setupUi(this);
}
install_dialog::~install_dialog()
{
    delete ui;
}
bool g_install_running {};
bool install_dialog::check_shortcut(std::string exe_path)
{
    if (fs::exists(exe_path))
    {
        std::string exe_name = exe_path.substr((exe_path.find_last_of("\\") + 1),exe_path.length()); // TODO: Convert to title case
        for (const auto &entry : fs::directory_iterator("C:\\Users\\"))
        {
            if (fs::is_directory(entry.path()) && fs::exists(entry.path()/"Desktop") && !fs::exists(entry.path()/"Desktop"/exe_name))
            {
                try
                {
                    std::string link_cmd = "mklink ";
                    link_cmd.append("\"" + entry.path().string() + "\\Desktop\\" + exe_name + "\" \"" + exe_path + "\"");
                    (void) system(link_cmd.c_str());
                }
                catch (const std::exception &e)
                {
                    // Catch permission denied errors. We can't really do much about them, though,
                    // since the application is supposed to run as administrator anyways.
                }
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}
void install_dialog::install()
{
    // Initialize temp folder, set UI elements' states
    // -----------------------------------------------
    g_install_running = true;
    std::string temp_folder = "C:\\ProgramData\\qSchoolHelper\\tmp";
    if (!fs::exists(temp_folder))
    {
        fs::create_directory(temp_folder);
    }
    chdir(temp_folder.c_str());
    ui->button_box->setEnabled(false);
    ui->install_button->setEnabled(false);
    ui->firefox_check_box->setEnabled(false);
    ui->reader_check_box->setEnabled(false);
    ui->libreoffice_check_box->setEnabled(false);
    ui->vlc_check_box->setEnabled(false);
    ui->viewer_check_box->setEnabled(false);

    ui->progress_bar->setValue(0);
    QApplication::processEvents();

    // Actually run the installation
    // -----------------------------
    int install_result = install_software(
                             ui->firefox_check_box->checkState(),
                             ui->reader_check_box->checkState(),
                             ui->libreoffice_check_box->checkState(),
                             ui->vlc_check_box->checkState(),
                             ui->viewer_check_box->checkState()
                             );

    if(install_result == 0)
    {
        QMessageBox success_box;
        success_box.setText(tr("The installation completed succesfully!"));
        success_box.setModal(true);
        success_box.exec();
        ui->progress_bar->setValue(100);
    }
    else if(install_result == 1)
    {
        QMessageBox dl_failure_box;
        dl_failure_box.setText(tr("The download failed! Please check your Internet connectivity!"));
        dl_failure_box.setModal(true);
        dl_failure_box.exec();
        ui->progress_bar->setValue(0);
    }
    else if(install_result == 2)
    {
        QMessageBox install_failure_box;
        install_failure_box.setText(tr("The installation failed! Please try installing the program manually!"));
        install_failure_box.setModal(true);
        install_failure_box.exec();
        ui->progress_bar->setValue(0);
    }
    else
    {
        QMessageBox unknown_failure_box;
        unknown_failure_box.setText(tr("The installation failed!"));
        unknown_failure_box.setModal(true);
        unknown_failure_box.exec();
        ui->progress_bar->setValue(0);
    }
    g_install_running = false;
    

    // Set UI elements' states
    // -----------------------

    ui->button_box->setEnabled(true);
    ui->install_button->setEnabled(true);
    ui->firefox_check_box->setEnabled(true);
    ui->reader_check_box->setEnabled(true);
    ui->libreoffice_check_box->setEnabled(true);
    ui->vlc_check_box->setEnabled(true);
    ui->viewer_check_box->setEnabled(true);
}
void install_dialog::on_install_button_clicked()
{
    // If no program has been selected, display message box
    // ----------------------------------------------------
    if (!(ui->firefox_check_box->isChecked() || ui->reader_check_box->isChecked() || ui->vlc_check_box->isChecked()
          || ui->viewer_check_box->isChecked() || ui->libreoffice_check_box->isChecked()))
    {
        QMessageBox no_program_selected_box;
        no_program_selected_box.setText(tr("Please select at least one program to be installed!"));
        no_program_selected_box.setModal(true);
        no_program_selected_box.exec();
    }
    else
    {
        install();
    }
}
void install_dialog::closeEvent(QCloseEvent *event)
{
    if (g_install_running)
    {
        event->ignore();
    }
    else
    {
        event->accept();
    }
}
