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
#include <curl/curl.h>
#include <QThread>
#include <QtConcurrent/QtConcurrentRun>
#include <QMessageBox>
#include "install_dialog.h"
#include "ui_install_dialog.h"
#include "download.h"

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
bool install_dialog::check_shortcut(std::string exe_path)
{
    if (fs::exists(exe_path))
    {
        std::string exe_name = exe_path.substr(exe_path.find_last_of("\\"),exe_path.length()); // TODO: Convert to title case
        for (const auto &entry : fs::directory_iterator("C:\\Users\\"))
        {

            if (fs::is_directory(entry.path()) && fs::exists(entry.path()/"Desktop") && !fs::exists(entry.path()/"Desktop"/exe_name))
            {
                try
                {
                    std::string link_cmd = "mklink ";
                    link_cmd.append(entry.path()/"Desktop"/exe_name);
                    link_cmd.append(exe_path);

                    system(link_cmd.c_str());
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
void run_install(const char* cmd, const char* filename)
{
    if (system(cmd))
    {
        fs::remove(filename); // Remove installation files after successful installation
    }
    else
    {
        // TODO: Handle installation errors properly
        QMessageBox failure_box;
        failure_box.setText("The installation failed!");
        failure_box.setModal(true);
        failure_box.exec();
    }
    // Reference commands
    ///system("installers\\vlc-3.0.8-win64.exe /L=1033 /S");
    ///system("installers\\\"Firefox Installer.exe\" /S");
    // TODO: SILENT INSTALL
    ///system("installers\\PowerPointViewer.exe /S");
    ///system("installers\\LibreOffice_6.3.4_Win_x64.msi");
    ///system("installers\\readerdc_uk_xa_install.exe");


}
void install_dialog::install()
{
    const char *temp_folder = "C:\\ProgramData\\qSchoolHelper\\tmp";
    if (!fs::exists(temp_folder))
    {
        fs::create_directory(temp_folder);
    }
    chdir(temp_folder);
    ui->button_box->setEnabled(false);
    ui->install_button->setEnabled(false);
    ui->progress_bar->setValue(0);

    // Declare download links and file names
    // -------------------------------------
    const int DL_ARRAY_SIZE=5;
    const char *download_array[DL_ARRAY_SIZE][4];
    if (ui->firefox_check_box->isChecked())
    {
        download_array[0][0]="https://download-installer.cdn.mozilla.net/pub/firefox/releases/74.0/win64/en-US/Firefox%20Setup%2074.0.msi";
        download_array[0][1]="Firefox_Setup_74.0.msi";
        download_array[0][2]="C:\\Program Files\\Mozilla Firefox\\firefox.exe";
    }
    else
    {
        download_array[0][0]=nullptr;
    }
    if (ui->reader_check_box->isChecked())
    {
        download_array[1][0]="https://admdownload.adobe.com/bin/live/readerdc_en_a_install.exe";
        download_array[1][1]="readerdc_en_a_install.exe";
        download_array[1][2]="C:\\Program Files (x86)\\Adobe\\Acrobat Reader DC\\Reader\\AcroRd32.exe";
    }
    else
    {
        download_array[1][0]=nullptr;
    }
    if (ui->libreoffice_check_box->isChecked())
    {
        download_array[2][0]="https://download.documentfoundation.org/libreoffice/stable/6.4.2/win/x86_64/LibreOffice_6.4.2_Win_x64.msi";
        download_array[2][1]="LibreOffice_6.4.2_Win_x64.msi";
        download_array[2][2]=nullptr;
    }
    else
    {
        download_array[2][0]=nullptr;
    }
    if (ui->vlc_check_box->isChecked())
    {
        download_array[3][0]="https://get.videolan.org/vlc/3.0.8/win64/vlc-3.0.8-win64.exe";
        download_array[3][1]="vlc-3.0.8-win64.exe";
        download_array[3][2]="C:\\Program Files\\VideoLAN\\VLC\\vlc.exe";
    }
    else
    {
        download_array[3][0]=nullptr;
    }
    if (ui->viewer_check_box->isChecked())
    {
        download_array[4][0]=nullptr;
        download_array[4][1]=nullptr;
        download_array[4][2]=nullptr;
    }
    else
    {
        download_array[4][0]=nullptr;
    }
    // Download the files
    // ------------------
    bool shortcut_array[DL_ARRAY_SIZE];
    for (int i = 0; i < DL_ARRAY_SIZE; i++)
    {
        shortcut_array[i] = check_shortcut(download_array[i][2]);
        if (download_array[i][0] != nullptr && !shortcut_array[i])
        {
            QFuture<void> dl = QtConcurrent::run(curl_dl, download_array[i][0],download_array[i][1]);
            while(dl.isRunning())
            {
                QCoreApplication::processEvents();
            }
            ui->progress_bar->setValue((i+1)*10);
        }
    }

    // Install the applications
    // ------------------------
    for (int i = 0; i < DL_ARRAY_SIZE; i++)
    {
        if (download_array[i][0] != nullptr && !shortcut_array[i])
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
                case 5:
                    break;
            }
            QFuture<void> install = QtConcurrent::run(run_install, cmd.c_str(), download_array[i][1]);
            while(install.isRunning())
            {
                QCoreApplication::processEvents();
            }



        }
        ui->progress_bar->setValue((i+6)*10);
    }
    QMessageBox success_box;
    success_box.setText("The installation completed succesfully!");
    success_box.setModal(true);
    success_box.exec();
    ui->button_box->setEnabled(true);
    ui->install_button->setEnabled(true);
}
