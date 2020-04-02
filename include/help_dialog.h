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

#ifndef HELP_DIALOG_H
#define HELP_DIALOG_H

#include <QDialog>

namespace Ui
{
    class help_dialog;
}

class help_dialog : public QDialog
{
        Q_OBJECT

    public:
        explicit help_dialog(QWidget *parent = nullptr);
        ~help_dialog();

    private:
        Ui::help_dialog *ui;
};

#endif // HELP_DIALOG_H
