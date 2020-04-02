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

#ifndef CLEANING_DIALOG_H
#define CLEANING_DIALOG_H

#include <QDialog>

namespace Ui
{
    class cleaning_dialog;
}

class cleaning_dialog : public QDialog
{
        Q_OBJECT

    public:
        explicit cleaning_dialog(QWidget *parent = nullptr);
        ~cleaning_dialog();

    private:
        Ui::cleaning_dialog *ui;

    public slots:
        void clean_extended();

    private slots:
        void clean();

};

#endif // CLEANING_DIALOG_H
