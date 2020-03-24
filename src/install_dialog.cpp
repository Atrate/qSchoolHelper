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
// Parts of the following two functions have been taken from https://curl.haxx.se/libcurl/c/url2file.html, in accordance with the license.
// ---------------------------------------------------------------------------------------------------------------------------------------
size_t install_dialog::write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
    size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
    return written;
}
int install_dialog::curl_dl(const char *url, const char *pagefilename)
{
    CURL *curl_handle;

    FILE *pagefile = nullptr;
    curl_global_init(CURL_GLOBAL_ALL);

    /* init the curl session */
    curl_handle = curl_easy_init();

    /* set URL to get here */
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);

    /* Switch on full protocol/debug output while testing */
    curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);

    /* disable progress meter, set to 0L to enable it */
    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 0L);

    /* follow redirects, to select best mirror */
    curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);

    /* send all data to this function  */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);

    /* open the file */
    pagefile = fopen(pagefilename, "wb");
    if (pagefile)
    {

        /* write the page body to this file handle */
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, pagefile);

        /* get it! */
        curl_easy_perform(curl_handle);

        /* close the header file */
        fclose(pagefile);
    }

    /* cleanup curl stuff */
    curl_easy_cleanup(curl_handle);

    curl_global_cleanup();

    return 0;
}
void install_dialog::install()
{
    const char *temp_folder = "C:\\ProgramData\\qSchoolHelper\\tmp";
    if (!fs::exists(temp_folder))
    {
        fs::create_directory(temp_folder);
    }
    chdir(temp_folder);
    ui->install_button->setEnabled(false);
    ui->progress_bar->setValue(0);

    // Declare download links and file names
    // -------------------------------------
    const int DL_ARRAY_SIZE=5;
    const char *download_array[DL_ARRAY_SIZE][2];
    download_array[0][0]="https://download-installer.cdn.mozilla.net/pub/firefox/releases/74.0/win64/en-US/Firefox%20Setup%2074.0.msi";
    download_array[0][1]="Firefox_Setup_74.0.msi";
    // DEBUG: Download only Firefox, no need to DL everything in early debug builds
    ///download_array[1][0]="https://admdownload.adobe.com/bin/live/readerdc_en_a_install.exe";
    ///download_array[1][1]="readerdc_en_a_install.exe";
    ///download_array[2][0]="https://download.documentfoundation.org/libreoffice/stable/6.4.2/win/x86_64/LibreOffice_6.4.2_Win_x64.msi";
    ///download_array[2][1]="LibreOffice_6.4.2_Win_x64.msi";
    ///download_array[3][0]="https://get.videolan.org/vlc/3.0.8/win64/vlc-3.0.8-win64.exe";
    ///download_array[3][1]="vlc-3.0.8-win64.exe";
    ///download_array[4][0]=nullptr;
    ///download_array[4][1]=nullptr;

    // Download the files
    // ------------------
    for (int i = 0; i < DL_ARRAY_SIZE; i++)
    {
        if (download_array[i][0] != nullptr) // DEBUG: Do not download nullptr. Nullptr will be removed from here in the future.
        {
            curl_dl(download_array[i][0],download_array[i][1]);
            ui->progress_bar->setValue((i+1)*10);
        }
    }

    // Install
    // -------
    for (int i = 0; i < DL_ARRAY_SIZE; i++)
    {
        if (download_array[i][0] != nullptr) // DEBUG: Do not download nullptr. Nullptr will be removed from here in the future.
        {
            // TODO: Add a check, if the same version of the app is already installed, just create a shortcut on the desktop.
            std::string cmd = download_array[i][1];
            if (i == 0)
            {
                cmd.append("/S");
            }
            else if (i == 3)
            {
                cmd.append("/L=1033");
            }
            if (system(cmd.c_str()))
            {
                fs::remove(download_array[i][1]); // Remove installation files after successful installation
            }
            else
            {
                // TODO: Handle installation errors
            }
            // Reference commands
            ///system("installers\\vlc-3.0.8-win64.exe /L=1033 /S");
            ///system("installers\\\"Firefox Installer.exe\" /S");
            // TODO: SILENT INSTALL
            ///system("installers\\PowerPointViewer.exe /S");
            ///system("installers\\LibreOffice_6.3.4_Win_x64.msi");
            ///system("installers\\readerdc_uk_xa_install.exe");


        }
    }

    ui->install_button->setEnabled(true);

}
