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
    ui->cleaning_log->append("Starting cleaner…\n-----------------");

    ui->cleaning_log->append("Removing .bat files from the desktop…");
    std::string path = "C:\\Users\\";
    for (const auto &entry : fs::directory_iterator(path))
    {
        if (fs::is_directory(entry.path()))
        {
            fs::remove_all(entry.path()/"Desktop"/"*.bat");
            fs::remove_all(entry.path()/"Desktop"/"*.cmd");
            //std::string cmd = "del " + entry.path().u8string() + "\\Desktop\\*.bat";
            //char cmdc[1024];
            //strncpy(cmdc,cmd.c_str(), sizeof(cmdc));
            //cmdc[sizeof(cmdc) - 1] = 0;
            //system(cmdc);
        }
    }

    ui->cleaning_log->append("Cleaning temporary files and caches…");
    // TODO: Verify BleachBit checksum

}
