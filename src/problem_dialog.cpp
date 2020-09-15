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

problem_dialog::problem_dialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::problem_dialog)
{
    ui->setupUi(this);
    ui->stacked_widget->setCurrentIndex(0);
    set_tabs(0);
    ui->back_button->setVisible(false);
    ui->next_button->setVisible(false);
}

problem_dialog::~problem_dialog()
{
    delete ui;
}
void problem_dialog::set_tabs(int i)
{
    ui->browser_tabs->setCurrentIndex(i);
    ui->presentation_tabs->setCurrentIndex(i);
    ui->projector_tabs->setCurrentIndex(i);
    ui->speakers_tabs->setCurrentIndex(i);
}
void problem_dialog::on_back_button_clicked()
{
    if (ui->browser_tabs->currentIndex() == 1 || ui->presentation_tabs->currentIndex() == 1
            || ui->speakers_tabs->currentIndex() == 1 || ui->projector_tabs->currentIndex() == 1)
    {
        set_tabs(0);
    }
    else
    {
        ui->stacked_widget->setCurrentIndex(0);
        ui->back_button->setVisible(false);
        ui->next_button->setVisible(false);
    }
}
void problem_dialog::on_next_button_clicked()
{
    set_tabs(1);
}
void problem_dialog::on_browser_button_clicked()
{
    set_tabs(0);
    ui->stacked_widget->setCurrentIndex(1);
    ui->back_button->setVisible(true);
    ui->next_button->setVisible(true);
}
void problem_dialog::on_projector_button_clicked()
{
    set_tabs(0);
    ui->stacked_widget->setCurrentIndex(2);
    ui->back_button->setVisible(true);
    ui->next_button->setVisible(true);
}
void problem_dialog::on_speakers_button_clicked()
{
    set_tabs(0);
    ui->stacked_widget->setCurrentIndex(3);
    ui->back_button->setVisible(true);
    ui->next_button->setVisible(true);
}
void problem_dialog::on_presentation_button_clicked()
{
    set_tabs(0);
    ui->stacked_widget->setCurrentIndex(4);
    ui->back_button->setVisible(true);
    ui->next_button->setVisible(true);
}

