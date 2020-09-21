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

#ifndef INITIALSETUPDIALOG_H
#define INITIALSETUPDIALOG_H

#include <QDialog>
#include <QStandardPaths>

namespace Ui
{
    class InitialSetupDialog;
}
class InitialSetupDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit InitialSetupDialog(QWidget* parent = nullptr);
        ~InitialSetupDialog();

    protected:
        void closeEvent(QCloseEvent* event);

    private:
        const QString config_folder = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        Ui::InitialSetupDialog* ui;

    private slots:
        void initial_setup();
};

#endif // INITIALSETUPDIALOG_H
