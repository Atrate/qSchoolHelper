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

#ifndef INSTALL_DIALOG_H
#define INSTALL_DIALOG_H

#include <QDialog>

namespace Ui
{
    class install_dialog;
}
class install_dialog : public QDialog
{
        Q_OBJECT

    public:
        explicit install_dialog(QWidget *parent = nullptr);
        ~install_dialog();

    protected:
        void closeEvent(QCloseEvent *event);

    private:
        Ui::install_dialog *ui;
        bool check_shortcut(std::string exe_path);

    public slots:
        void install();
};

#endif // INSTALL_DIALOG_H
