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

#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QDialog>

namespace Ui
{
    class HelpDialog;
}

class HelpDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit HelpDialog(QWidget* parent = nullptr);
        ~HelpDialog();

    private:
        Ui::HelpDialog* ui;
};

#endif // HELPDIALOG_H
