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

#ifndef INITIAL_SETUP_DIALOG_H
#define INITIAL_SETUP_DIALOG_H

#include <QDialog>

namespace Ui {
    class initial_setup_dialog;
}

class initial_setup_dialog : public QDialog
{
        Q_OBJECT
    public:
        explicit initial_setup_dialog(QWidget *parent = nullptr);
        ~initial_setup_dialog();
    private:
        Ui::initial_setup_dialog *ui;

};

#endif // INITIAL_SETUP_DIALOG_H
