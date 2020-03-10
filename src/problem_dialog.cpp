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

#include "problem_dialog.h"
#include "ui_problem_dialog.h"

problem_dialog::problem_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::problem_dialog)
{
    ui->setupUi(this);
}

problem_dialog::~problem_dialog()
{
    delete ui;
}
