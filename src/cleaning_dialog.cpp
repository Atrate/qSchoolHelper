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
#include <QDebug>
#include "cleaning_dialog.h"
#include "initial_setup_dialog.h"
#include "procedures.h"
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

void cleaning_dialog::on_clean_button_clicked()
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
    qInfo() << tr("Starting cleaner…\n—————————————————");
    ui->cleaning_log->append(tr("Starting cleaner…\n—————————————————"));
    qInfo() << tr("Removing .bat and .cmd files from the desktop…");
    ui->cleaning_log->append(tr("Removing .bat and .cmd files from the desktop…"));
    QApplication::processEvents();

    // Actually run the cleaning process
    // ---------------------------------
    procedures* cleaning = new procedures;
    cleaning->run_clean(ui->radio_extended->isChecked());
    delete cleaning;

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
    ui->clean_button->setEnabled(true);
    ui->button_box->setEnabled(true);

}
// Ignore close events if a process is running
// -------------------------------------------
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
