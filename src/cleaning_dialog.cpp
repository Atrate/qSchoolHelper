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
    ui->clean_button->setEnabled(false);
    ui->cleaning_log->clear();
    ui->cleaning_log->append("Starting cleaner…\n-----------------");
    ui->cleaning_log->append("Removing .bat and .cmd files from the desktop…");

    std::string path = "C:\\Users\\";
    for (const auto &entry : fs::directory_iterator(path))
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
            catch (const std::exception)
            {
                // Catch permission denied errors. We can't really do much about them, though,
                // since the application is supposed to run as administrator anyways.
            }

        }
    }
    ui->cleaning_log->append("Cleaning temporary files and caches…");
    // TODO: Verify BleachBit checksum
    // ...
    ui->cleaning_log->append("All done!");
    ui->clean_button->setEnabled(true);

}
