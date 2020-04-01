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
    ui->stacked_widget->setCurrentIndex(0);
}

problem_dialog::~problem_dialog()
{
    delete ui;
}
void problem_dialog::on_back_button_clicked()
{
    ui->stacked_widget->setCurrentIndex(0);
}
void problem_dialog::on_browser_button_clicked()
{
    ui->stacked_widget->setCurrentIndex(1);
}
void problem_dialog::on_projector_button_clicked()
{
    ui->stacked_widget->setCurrentIndex(2);
}
void problem_dialog::on_speakers_button_clicked()
{
    ui->stacked_widget->setCurrentIndex(3);
}
void problem_dialog::on_presentation_button_clicked()
{
    ui->stacked_widget->setCurrentIndex(4);
}

