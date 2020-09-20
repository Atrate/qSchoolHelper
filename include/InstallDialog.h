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

#ifndef INSTALLDIALOG_H
#define INSTALLDIALOG_H

#include <QDialog>

namespace Ui
{
    class InstallDialog;
}
class InstallDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit InstallDialog(QWidget* parent = nullptr);
        ~InstallDialog();

    protected:
        void closeEvent(QCloseEvent* event);

    private:
        Ui::InstallDialog* ui;

    public slots:
        void install();

    private slots:
        void on_install_button_clicked();
};

#endif // INSTALLDIALOG_H
