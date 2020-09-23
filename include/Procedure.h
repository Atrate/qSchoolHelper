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

#ifndef PROCEDURE_H
#define PROCEDURE_H

#include <CurlEasy.h>
#include <QStandardPaths>
#include <QDir>
#include <QObject>

class Procedure : public QObject
{
        Q_OBJECT

    private:
        const QString config_folder = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

        int qtcurl_dl(const char* url, const char* filename);
        int install_software(const bool INS_FF, const bool INS_RDC, const bool INS_LOF, const bool INS_VLC, const bool INS_PPV);
        int clean(const bool EXT);
        int install_bb();
        bool check_shortcut(QString exe_path, const int NAME_NUM);
        QString get_file_info(const int LINE, bool fallback = false);
        static size_t write_data(void* ptr, size_t size, size_t nmemb, void* stream);

    public:
        int run_install_bb();
        int run_install_software(const bool INS_FF, const bool INS_RDC, const bool INS_LOF, const bool INS_VLC, const bool INS_PPV);
        int run_clean(const bool EXT);
        int run_qtcurl_dl(const char* url, const char* filename);

    signals:
        void progress_changed(int progress);
        void progress_description(QString desc);
};


#endif // PROCEDURE_H
