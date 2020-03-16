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

#include "help_dialog.h"
#include "ui_help_dialog.h"

help_dialog::help_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::help_dialog)
{
    ui->setupUi(this);
}

help_dialog::~help_dialog()
{
    delete ui;
}
