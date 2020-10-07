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

#include <QMessageBox>
#include <QCloseEvent>
#include <QDebug>
#include "CleaningDialog.h"
#include "InitialSetupDialog.h"
#include "Procedure.h"
#include "ui_CleaningDialog.h"

CleaningDialog::CleaningDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::CleaningDialog)
{
    ui->setupUi(this);
}
CleaningDialog::~CleaningDialog()
{
    delete ui;
}
bool g_cleaning_running {};

void CleaningDialog::on_clean_button_clicked()
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
    ui->radio_simple->setEnabled(false);
    ui->radio_extended->setEnabled(false);
    QApplication::processEvents();
    // Actually run the cleaning process
    // ---------------------------------
    Procedure cleaning;
    QObject::connect(&cleaning, SIGNAL(progress_description(QString)), this->ui->cleaning_log, SLOT(append(QString)));
    QObject::connect(&cleaning, SIGNAL(progress_changed(int)), this->ui->progress_bar, SLOT(setValue(int)));
    cleaning.clean(ui->radio_extended->isChecked());
    // Finalize — set UI element states
    // --------------------------------
    g_cleaning_running = false;
    ui->progress_bar->setValue(100);
    qInfo() << tr("All done!");
    ui->cleaning_log->append(tr("All done!"));
    QMessageBox success_box;
    success_box.setText(tr("The cleaning operation completed succesfully!"));
    success_box.setModal(true);
    success_box.exec();
    ui->radio_simple->setEnabled(true);
    ui->radio_extended->setEnabled(true);
    ui->clean_button->setEnabled(true);
    ui->button_box->setEnabled(true);
}
// Ignore close events if a process is running
// -------------------------------------------
void CleaningDialog::closeEvent(QCloseEvent* event)
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
