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

#ifndef CLEANINGDIALOG_H
#define CLEANINGDIALOG_H

#include <QDialog>

namespace Ui
{
    class CleaningDialog;
}

class CleaningDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit CleaningDialog(QWidget* parent = nullptr);
        ~CleaningDialog();

    protected:
        void closeEvent(QCloseEvent* event);

    private:
        Ui::CleaningDialog* ui;

    private slots:
        void on_clean_button_clicked();
};

#endif // CLEANINGDIALOG_H
