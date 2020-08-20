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
#include <stdlib.h>
#include <unistd.h>
#include <QApplication>
#include <QThread>
#include <QtConcurrent/QtConcurrentRun>
#include "procedures.h"

namespace fs = std::filesystem;
// TODO: MAKE UI RESPONSIVE
// Parts of the following two functions have been taken from https://curl.haxx.se/libcurl/c/url2file.html, in accordance with the license.
// ---------------------------------------------------------------------------------------------------------------------------------------

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
    size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
    return written;
}
int curl_dl(const char *url, const char *pagefilename)
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

    /* set certificate bundle path */

    curl_easy_setopt(curl_handle, CURLOPT_CAINFO, "qrc:/../data/curl-ca-bundle.crt");


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

bool check_shortcut(std::string exe_path)
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

int install_software(const bool INS_FF, const bool INS_RDC, const bool INS_LOF, const bool INS_VLC, const bool INS_PPV)
{
    // Declare download links and file names
    // -------------------------------------
    const int DL_ARRAY_SIZE=5;
    std::string download_array[DL_ARRAY_SIZE][4];
    if (INS_FF)
    {
        download_array[0][0]=std::string("https://download-installer.cdn.mozilla.net/pub/firefox/releases/77.0/win64/en-US/Firefox%20Setup%2077.0.msi");
        download_array[0][1]=std::string("Firefox_Setup_77.0.msi");
        download_array[0][2]=std::string("C:\\Program Files\\Mozilla Firefox\\firefox.exe");
    }
    else
    {
        download_array[0][0]=std::string("");
    }
    if (INS_RDC)
    {
        download_array[1][0]=std::string("https://admdownload.adobe.com/bin/live/readerdc_en_a_install.exe");
        download_array[1][1]=std::string("readerdc_en_a_install.exe");
        download_array[1][2]=std::string("C:\\Program Files (x86)\\Adobe\\Acrobat Reader DC\\Reader\\AcroRd32.exe");
    }
    else
    {
        download_array[1][0]=std::string("");
    }
    if (INS_LOF)
    {
        download_array[2][0]=std::string("https://download.documentfoundation.org/libreoffice/stable/6.4.4/win/x86_64/LibreOffice_6.4.4_Win_x64.msi");
        download_array[2][1]=std::string("LibreOffice_6.4.4_Win_x64.msi");
        download_array[2][2]=std::string("");
    }
    else
    {
        download_array[2][0]=std::string("");
    }
    if (INS_VLC)
    {
        download_array[3][0]=std::string("https://get.videolan.org/vlc/3.0.8/win64/vlc-3.0.8-win64.exe");
        download_array[3][1]=std::string("vlc-3.0.8-win64.exe");
        download_array[3][2]=std::string("C:\\Program Files\\VideoLAN\\VLC\\vlc.exe");
    }
    else
    {
        download_array[3][0]=std::string("");
    }
    if (INS_PPV)
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

            if (!(fs::file_size(download_array[i][1]) > 2048) || !(dl == 0))
            {
                dl.~QFuture();
                return 1;
            }
            dl.~QFuture();
            //ui->progress_bar->setValue((i+1)*10);
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
            QFuture<int> install = QtConcurrent::run(system, cmd.c_str());
            while(install.isRunning())
            {
                QApplication::processEvents();
            }
            fs::remove(download_array[i][1]);
            if (!install)
            {
                install.~QFuture();
                return 2;
            }
            install.~QFuture();

        }
        //ui->progress_bar->setValue((i+6)*10);
    }
    return 0;
}
int clean(const bool EXT)
{
    // Find and remove all .bat and .cmd files from all users' desktops
    // ----------------------------------------------------------------
    try
    {
        for (const auto &entry : fs::directory_iterator("C:\\Users\\"))
        {
            if (fs::is_directory(entry.path()) && fs::exists(entry.path()/"Desktop"))
            {

                for (const auto &file : fs::directory_iterator(entry.path()/"Desktop"))
                {
                    if (file.path().extension() == ".cmd" || file.path().extension() == ".bat")
                    {
                        fs::remove(file.path());
                    }
                }
            }
        }
    }
    catch (const std::exception &e)
    {
        // Catch permission denied and no such file or directory errors. We can't really do much about them, though,
        // since the application is supposed to run as administrator anyways and we can't account for a lack of C:/Users/
    }

    // Run BleachBit to clean temporary files
    // --------------------------------------
    //ui->progress_bar->setValue(30);
    std::string bb_path = "C:\\Program Files (x86)\\BleachBit\\bleachbit_console.exe";
    if (!fs::exists(bb_path))
    {
        //ui->cleaning_log->append(tr("Downloading BleachBit (cleaning engine)…"));
        install_bb();
    }
    //ui->progress_bar->setValue(40);
    //ui->cleaning_log->append(tr("Cleaning temporary files and caches…"));
    bb_path = "\"" + bb_path + "\"";
    std::string cmd = bb_path + " --clean adobe_reader.* amule.* chromium.* deepscan.tmp "
                                "filezilla.mru firefox.* flash.* gimp.tmp google_chrome.* google_toolbar.search_history "
                                "internet_explorer.* java.cache libreoffice.* microsoft_office.* openofficeorg.* opera.* "
                                "paint.mru realplayer.* safari.* silverlight.* skype.* smartftp.* system.clipboard "
                                "system.prefetch system.recycle_bin system.tmp vim.* waterfox.* winamp.mru windows_explorer.* "
                                "windows_media_player.* winrar.history winrar.temp winzip.mru wordpad.mru yahoo_messenger.*";

    QFuture<void> bb_clean = QtConcurrent::run(system, cmd.c_str());
    while(bb_clean.isRunning())
    {
        QApplication::processEvents();
    }
    bb_clean.~QFuture();

    // Extended cleaning
    // -----------------
    if (EXT)
    {
        cmd = bb_path + " --clean deepscan.ds_store deepscan.thumbs_db system.logs "
                                    "system.memory_dump system.muicache system.prefetch system.updates";
        //ui->progress_bar->setValue(60);
        //ui->cleaning_log->append(tr("Cleaning temporary files and caches (extended)…"));
        QFuture<void> bb_clean = QtConcurrent::run(system, cmd.c_str());
        while(bb_clean.isRunning())
        {
            QApplication::processEvents();
        }
        bb_clean.~QFuture();
    }

    // Clean qSH tmp folder
    // --------------------
    //ui->progress_bar->setValue(80);
    //ui->cleaning_log->append(tr("Cleaning qSchoolHelper's temporary folder"));
    std::string temp_folder = "C:\\ProgramData\\qSchoolHelper\\tmp";
    fs::remove_all(temp_folder);

    return 0;
}
void install_bb()
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
        QFuture<void> bb_install = QtConcurrent::run(system, bb_exe.c_str());
        while(bb_install.isRunning())
        {
            QApplication::processEvents();
        }
        bb_install.~QFuture();
        fs::remove(bb_exe);
    }
}
