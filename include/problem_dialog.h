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

#ifndef PROBLEM_DIALOG_H
#define PROBLEM_DIALOG_H

#include <QDialog>

namespace Ui
{
    class problem_dialog;
}

class problem_dialog : public QDialog
{
        Q_OBJECT

    public:
        explicit problem_dialog(QWidget *parent = nullptr);
        ~problem_dialog();

    private:
        Ui::problem_dialog *ui;

    private slots:
        void on_back_button_clicked();
        void on_browser_button_clicked();
        void on_projector_button_clicked();
        void on_speakers_button_clicked();
        void on_presentation_button_clicked();
};

#endif // PROBLEM_DIALOG_H
