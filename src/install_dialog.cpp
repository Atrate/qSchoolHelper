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
#include "download.h"
#include "install_dialog.h"
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
int run_install(const char* cmd)
{
    return(system(cmd));
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
    ui->progress_bar->setValue(0);
    QApplication::processEvents();

    // Declare download links and file names
    // -------------------------------------
    const int DL_ARRAY_SIZE=5;
    std::string download_array[DL_ARRAY_SIZE][4];
    if (ui->firefox_check_box->isChecked())
    {
        download_array[0][0]=std::string("https://download-installer.cdn.mozilla.net/pub/firefox/releases/77.0/win64/en-US/Firefox%20Setup%2077.0.msi");
        download_array[0][1]=std::string("Firefox_Setup_77.0.msi");
        download_array[0][2]=std::string("C:\\Program Files\\Mozilla Firefox\\firefox.exe");
    }
    else
    {
        download_array[0][0]=std::string("");
    }
    if (ui->reader_check_box->isChecked())
    {
        download_array[1][0]=std::string("https://admdownload.adobe.com/bin/live/readerdc_en_a_install.exe");
        download_array[1][1]=std::string("readerdc_en_a_install.exe");
        download_array[1][2]=std::string("C:\\Program Files (x86)\\Adobe\\Acrobat Reader DC\\Reader\\AcroRd32.exe");
    }
    else
    {
        download_array[1][0]=std::string("");
    }
    if (ui->libreoffice_check_box->isChecked())
    {
        download_array[2][0]=std::string("https://download.documentfoundation.org/libreoffice/stable/6.4.4/win/x86_64/LibreOffice_6.4.4_Win_x64.msi");
        download_array[2][1]=std::string("LibreOffice_6.4.4_Win_x64.msi");
        download_array[2][2]=std::string("");
    }
    else
    {
        download_array[2][0]=std::string("");
    }
    if (ui->vlc_check_box->isChecked())
    {
        download_array[3][0]=std::string("https://get.videolan.org/vlc/3.0.8/win64/vlc-3.0.8-win64.exe");
        download_array[3][1]=std::string("vlc-3.0.8-win64.exe");
        download_array[3][2]=std::string("C:\\Program Files\\VideoLAN\\VLC\\vlc.exe");
    }
    else
    {
        download_array[3][0]=std::string("");
    }
    if (ui->viewer_check_box->isChecked())
    {
        download_array[4][0]=std::string("https://gitlab.com/Atrate/powerpoint-viewer/-/raw/803209ecc2e0f773f6fe15410ad7e1bc1a51c0c7/PowerPointViewer.exe?inline=false");
        download_array[4][1]=std::string("PowerPointViewer.exe");
        download_array[4][2]=std::string("C:\\Program Files (x86)\\Microsoft Office\\Office14\\PPTVIEW.exe");
    }
    else
    {
        download_array[4][0]=std::string("");
    }

    // Download the files
    // ------------------
    bool shortcut_array[DL_ARRAY_SIZE];
    for (int i = 0; i < DL_ARRAY_SIZE; i++)
    {
        shortcut_array[i] = check_shortcut(download_array[i][2]);
        if (!(download_array[i][0] == "" || shortcut_array[i]))
        {
            fs::remove(download_array[i][1]);
            QFuture<int> dl = QtConcurrent::run(curl_dl, download_array[i][0].c_str(), download_array[i][1].c_str());
            while(dl.isRunning())
            {
                QApplication::processEvents();
            }
            dl.~QFuture();
            if (!(fs::file_size(download_array[i][1]) > 2048))
            {
                g_install_running = false;
                QMessageBox dl_failure_box;
                dl_failure_box.setText(tr("The download failed! Please check your Internet connectivity!"));
                dl_failure_box.setModal(true);
                dl_failure_box.exec();
                ui->progress_bar->setValue(0);
                ui->button_box->setEnabled(true);
                ui->install_button->setEnabled(true);
                return;
            }
            ui->progress_bar->setValue((i+1)*10);
            QApplication::processEvents();
        }
    }

    // Install the applications
    // ------------------------
    for (int i = 0; i < DL_ARRAY_SIZE; i++)
    {
        if (!(download_array[i][0] == "" || shortcut_array[i]))
        {
            // Check if the same version of the app is already installed. If so, just create a shortcut on the desktop.
            // --------------------------------------------------------------------------------------------------------
            std::string cmd = download_array[i][1];
            switch (i)
            {
                case 0:
                    cmd.append("/S");
                    break;
                case 1:
                    break;
                case 2:
                    break;
                case 3:
                    cmd.append("/S","/L=1033");
                    break;
                case 4:
                    break;
            }
            QFuture<int> install = QtConcurrent::run(run_install, cmd.c_str());
            while(install.isRunning())
            {
                QApplication::processEvents();
            }
            fs::remove(download_array[i][1]);
            if (!install)
            {
                install.~QFuture();
                g_install_running = false;
                QMessageBox install_failure_box;
                install_failure_box.setText(tr("The installation failed! Please try installing the program manually!"));
                install_failure_box.setModal(true);
                install_failure_box.exec();
                ui->progress_bar->setValue(0);
                ui->button_box->setEnabled(true);
                ui->install_button->setEnabled(true);
                return;
            }
            install.~QFuture();

        }
        ui->progress_bar->setValue((i+6)*10);
    }

    // Show success message, set UI elements' states
    // ---------------------------------------------
    g_install_running = false;
    QMessageBox success_box;
    success_box.setText(tr("The installation completed succesfully!"));
    success_box.setModal(true);
    success_box.exec();
    ui->button_box->setEnabled(true);
    ui->install_button->setEnabled(true);
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
