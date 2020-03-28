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
#include <unistd.h>
#include "cleaning_dialog.h"
#include "ui_cleaning_dialog.h"

namespace fs = std::filesystem;

cleaning_dialog::cleaning_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cleaning_dialog)
{
    ui->setupUi(this);
}

cleaning_dialog::~cleaning_dialog()
{
    delete ui;
}
void cleaning_dialog::clean()
{
    ui->clean_button->setEnabled(false);
    ui->buttonBox->setEnabled(false);
    ui->progress_bar->setValue(0);
    ui->cleaning_log->append("Starting cleaner…\n—————————————————");
    ui->cleaning_log->append("Removing .bat and .cmd files from the desktop…");

    // Find and remove all .bat and .cmd files from all users' desktops
    // ----------------------------------------------------------------
    for (const auto &entry : fs::directory_iterator("C:\\Users\\"))
    {
        if (fs::is_directory(entry.path()) && fs::exists(entry.path()/"Desktop"))
        {
            try
            {
                for (const auto &file : fs::directory_iterator(entry.path()/"Desktop"))
                {
                    if (file.path().extension() == ".cmd" || file.path().extension() == ".bat")
                    {
                        fs::remove(file.path());
                    }
                }
            }
            catch (const std::exception &e)
            {
                // Catch permission denied errors. We can't really do much about them, though,
                // since the application is supposed to run as administrator anyways.
            }

        }
    }
    // Run BleachBit to clean temporary files
    // --------------------------------------
    ui->progress_bar->setValue(10);
    ui->cleaning_log->append("Cleaning temporary files and caches…");
    // DEBUG: Assume BB is DLed to C:\Program Data\qSchoolHelper\utils\BleachBit-Portable
    const char *utils_folder = "C:\\ProgramData\\qSchoolHelper\\utils";
    if (!fs::exists(utils_folder))
    {
        fs::create_directory(utils_folder);
    }
    // TODO: DL and extract BB
    // TODO: Verify BleachBit checksum
    const char *bb_folder = "C:\\ProgramData\\qSchoolHelper\\utils\\BleachBit-Portable";
    chdir(bb_folder);
    system("bleachbit_console.exe --clean adobe_reader.* amule.* chromium.* deepscan.tmp filezilla.mru firefox.* flash.* gimp.tmp google_chrome.* google_toolbar.search_history internet_explorer.* java.cache libreoffice.* microsoft_office.* openofficeorg.* opera.* paint.mru realplayer.* safari.* silverlight.* skype.* smartftp.* system.clipboard system.prefetch system.recycle_bin system.tmp vim.* waterfox.* winamp.mru windows_explorer.* windows_media_player.* winrar.history winrar.temp winzip.mru wordpad.mru yahoo_messenger.*");

    // Uninstall unnecessary software (McAffee etc.)
    // ---------------------------------------------

    // Extended cleaning (repeats some steps from initial setup)
    // ---------------------------------------------------------
    if (ui->radio_extended->isEnabled())
    {
        ui->cleaning_log->append("Cleaning temporary files and caches (extended)…");
        system("bleachbit_console.exe --clean deepscan.ds_store deepscan.thumbs_db system.logs system.memory_dump system.muicache system.prefetch system.updates");

        ui->cleaning_log->append("Disabling telemetry service…");
        system("net stop DiagTrack");
        system("sc config DiagTrack start= disabled");

        ui->cleaning_log->append("Disabling search indexing...\n");
        system("net stop WSearch");
        system("sc config WSearch start= disabled");
    }

    // Finalize — set UI element states
    // --------------------------------
    ui->progress_bar->setValue(100);
    ui->cleaning_log->append("All done!");
    ui->clean_button->setEnabled(true);

}
