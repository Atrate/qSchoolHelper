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

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include "problem_dialog.h"
#include "about_dialog.h"
#include "install_dialog.h"
#include "initial_setup_dialog.h"
#include "cleaning_dialog.h"
#include "help_dialog.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class main_window;
}
QT_END_NAMESPACE

class main_window : public QMainWindow
{
        Q_OBJECT

    public:
        main_window(QWidget *parent = nullptr);
        ~main_window();

    private:
        Ui::main_window *ui;
        problem_dialog *problem;
        initial_setup_dialog *initial_setup;
        about_dialog *about;
        install_dialog *install;
        cleaning_dialog *cleaning;
        help_dialog *help;

        void window_setup();

    private slots:
        void on_problem_button_clicked();
        void on_initial_setup_button_clicked();
        void on_actionAbout_triggered();
        void on_software_button_clicked();
        void on_clean_button_clicked();
        void on_actionOpen_help_triggered();
        void on_actionInitial_setup_triggered();

};
#endif // MAIN_WINDOW_H
