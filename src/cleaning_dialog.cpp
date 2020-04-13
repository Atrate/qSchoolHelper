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
#include <QThread>
#include <QtConcurrent/QtConcurrentRun>
#include <QMessageBox>
#include <QCloseEvent>
#include "initial_setup_dialog.h"
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
bool g_cleaning_running {};
void run_clean(const char* cmd)
{
    system(cmd);
}
void cleaning_dialog::clean()
{
    // Begin — set UI element states
    // -----------------------------
    g_cleaning_running = true;
    ui->clean_button->setEnabled(false);
    ui->button_box->setEnabled(false);
    ui->cleaning_progress_label->setEnabled(true);
    ui->progress_bar->setEnabled(true);
    ui->progress_bar->setValue(0);
    ui->cleaning_log->setEnabled(true);
    ui->cleaning_log->clear();
    ui->cleaning_log->append("Starting cleaner…\n—————————————————");
    ui->cleaning_log->append("Removing .bat and .cmd files from the desktop…");

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

    // Uninstall unnecessary software (McAffee etc.)
    // ---------------------------------------------

    // Run BleachBit to clean temporary files
    // --------------------------------------
    ui->progress_bar->setValue(30);
    std::string bb_path = "C:\\Program Files (x86)\\BleachBit\\bleachbit_console.exe";
    if (!fs::exists(bb_path))
    {
        ui->cleaning_log->append("Downloading BleachBit (cleaning engine)…");
        initial_setup_dialog *is = new initial_setup_dialog();
        is->install_bb();
        delete is;
    }
    ui->progress_bar->setValue(40);
    ui->cleaning_log->append("Cleaning temporary files and caches…");
    bb_path = "\"" + bb_path + "\"";
    std::string cmd = bb_path + " --clean adobe_reader.* amule.* chromium.* deepscan.tmp "
                                "filezilla.mru firefox.* flash.* gimp.tmp google_chrome.* google_toolbar.search_history "
                                "internet_explorer.* java.cache libreoffice.* microsoft_office.* openofficeorg.* opera.* "
                                "paint.mru realplayer.* safari.* silverlight.* skype.* smartftp.* system.clipboard "
                                "system.prefetch system.recycle_bin system.tmp vim.* waterfox.* winamp.mru windows_explorer.* "
                                "windows_media_player.* winrar.history winrar.temp winzip.mru wordpad.mru yahoo_messenger.*";

    QFuture<void> bb_clean = QtConcurrent::run(run_clean, cmd.c_str());
    while(bb_clean.isRunning())
    {
        QCoreApplication::processEvents();
    }
    bb_clean.~QFuture();

    // Extended cleaning
    // -----------------
    if (ui->radio_extended->isChecked())
    {
        clean_extended();
    }

    // Clean qSH tmp folder
    // --------------------
    ui->progress_bar->setValue(80);
    ui->cleaning_log->append("Cleaning qSchoolHelper's temporary folder");
    std::string temp_folder = "C:\\ProgramData\\qSchoolHelper\\tmp";
    fs::remove_all(temp_folder);


    // Finalize — set UI element states
    // --------------------------------
    g_cleaning_running = false;
    ui->progress_bar->setValue(100);
    ui->cleaning_log->append("All done!");
    QMessageBox success_box;
    success_box.setText("The cleaning operation completed succesfully!");
    success_box.setModal(true);
    success_box.exec();
    ui->clean_button->setEnabled(true);
    ui->button_box->setEnabled(true);

}
void cleaning_dialog::clean_extended()
{
    std::string bb_path = "\"C:\\Program Files (x86)\\BleachBit\\bleachbit_console.exe\"";
    ui->progress_bar->setValue(60);
    ui->cleaning_log->append("Cleaning temporary files and caches (extended)…");
    std::string cmd = bb_path + " --clean deepscan.ds_store deepscan.thumbs_db system.logs "
                                "system.memory_dump system.muicache system.prefetch system.updates";
    QFuture<void> bb_clean = QtConcurrent::run(run_clean, cmd.c_str());
    while(bb_clean.isRunning())
    {
        QCoreApplication::processEvents();
    }
    bb_clean.~QFuture();
}
void cleaning_dialog::closeEvent(QCloseEvent *event)
{
    if (g_cleaning_running)
    {
        event->ignore();
    }
    else
    {
        event->accept();
    }
}
