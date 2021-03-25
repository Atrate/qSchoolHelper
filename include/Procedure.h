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

#include <QStandardPaths>
#include <QObject>

class Procedure : public QObject
{
        Q_OBJECT

    public:
        bool check_shortcut(const QString &exe_path, const int &NAME_NUM);
        int clean(const bool EXT, const bool BAT);
        int install_bb();
        int install_software(const bool INS_FF, const bool INS_RDC, const bool INS_LOF, const bool INS_VLC, const bool INS_PPV, const bool INS_7Z);
        bool qtcurl_dl(const char* url, const char* filename);

    private:
        const QString config_folder = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        QString get_file_info(const int &LINE, bool fallback = false);
        static size_t write_data(void* ptr, size_t size, size_t nmemb, void* stream);

    signals:
        void progress_changed(int progress);
        void progress_description(QString desc);
};


#endif // PROCEDURE_H
