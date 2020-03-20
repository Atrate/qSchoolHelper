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

namespace Ui {
    class install_dialog;
}

class install_dialog : public QDialog
{
        Q_OBJECT

    public:
        explicit install_dialog(QWidget *parent = nullptr);
        ~install_dialog();

    private:
        Ui::install_dialog *ui;

    private slots:
        void install();
        void download(const char *url, const char *pagefilename);
};

#endif // INSTALL_DIALOG_H
