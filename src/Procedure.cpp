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

#include <stdlib.h>
#include <unistd.h>
#include <QDebug>
#include <QFile>
#include <QDirIterator>
#include <QApplication>
#include <QThread>
#include <QTemporaryDir>
#include <QtConcurrent/QtConcurrentRun>
#include <QStandardPaths>
#include "Procedure.h"

// TODO: MAKE UI RESPONSIVE
size_t Procedure::write_data(void* ptr, size_t size, size_t nmemb, void* stream)
{
    size_t written = fwrite(ptr, size, nmemb, (FILE*)stream);
    return written;
}
bool Procedure::qtcurl_dl(const char* url, const char* filename)
{
#ifndef QT_NO_DEBUG
    assert(url[0] != '\0');
    assert(filename[0] != '\0');
#else

    if (url[0] == '\0' || filename[0] == '\0')
    {
        return false;
        qDebug() << "Invalid file name or download link!\n";
    }

#endif

    if (QFile().exists(filename))
    {
        QFile().remove(filename);
    }

    QString ca_path = config_folder + "/curl-ca-bundle.crt";
    qDebug() << "CA Path: " << ca_path << "\n";

    if (!QFile().exists(ca_path))
    {
        QFile::copy(":/data/curl-ca-bundle.crt", ca_path);
    }

    qDebug() << "Downloading: " << url << " to: " << filename << "\n";
    CurlEasy curl;
    curl.set(CURLOPT_URL, QUrl(url));
    curl.set(CURLOPT_FOLLOWLOCATION, long(1)); // Tells libcurl to follow HTTP 3xx redirects
    curl.set(CURLOPT_CAINFO, ca_path);
    curl.set(CURLOPT_FAILONERROR, long(1)); // Do not return CURL_OK in case valid server responses reporting errors.
    curl.set(CURLOPT_WRITEFUNCTION, write_data);
    curl.setHttpHeader("User-Agent", "qSchoolHelper_v" APP_VERSION);
#ifndef QT_NO_DEBUG
    curl.set(CURLOPT_NOPROGRESS, long(0));
#endif
    // Open file for writing and tell cURL to write to it
    // --------------------------------------------------
    FILE* dl_file = nullptr;
    dl_file = fopen(filename, "wb");
    curl.set(CURLOPT_WRITEDATA, dl_file);
    curl.perform();

    while (curl.isRunning())
    {
        QApplication::processEvents();
    }

    fclose(dl_file);
    return (curl.result() == 0 ? true : false);
}
QString Procedure::get_file_info(const int LINE, bool fallback)
{
#ifndef QT_NO_DEBUG
    assert(LINE < 11 && LINE > -1);
#endif
    QString filename = "programlist.txt";

    if (!QFile().exists(filename) && !fallback)
    {
        if (!qtcurl_dl("https://gitlab.com/Atrate/qsh-resources/-/raw/master/programlist.txt", filename.toUtf8()))
        {
            if (!qtcurl_dl("https://raw.githubusercontent.com/Atrate/qsh-resources/master/programlist.txt", filename.toUtf8()))
            {
                fallback = true;
                QFile().remove(filename);
            }
        }
    }

    if (QFile().exists(filename) && !fallback)
    {
        qDebug() << "Reading LINE: " << LINE << " from: " << filename << "\n";
        QFile in(filename);
        QString return_string;
        return_string.reserve(180);

        if (in.open(QIODevice::ReadOnly))
        {
            QTextStream str_in(&filename);

            for (int i = 0; i < LINE; ++i)
            {
                qDebug() << "Skipping line: " << in.readLine() << "\n";
            }

            return_string = in.readLine().replace("\n", "");
            qDebug() << "Grabbed line: " << return_string << "\n";
            in.close();
        }
        else
        {
            qDebug() << "Failed to open file!\n";
        }

        return return_string;
    }
    else
    {
        qDebug() << "Using fallback link for: " << LINE << "\n";

        switch (LINE)
        {
            // Firefox
            // -------
            case 0:
                return "https://download-installer.cdn.mozilla.net/pub/firefox/releases/79.0/win64/en-US/Firefox%20Setup%2079.0.msi";
                break;

            case 1:
                return "C:\\Program Files\\Mozilla Firefox\\firefox.exe";
                break;

            // Reader
            // ------
            case 2:
                return "https://admdownload.adobe.com/bin/live/readerdc_en_a_install.exe";
                break;

            case 3:
                return "C:\\Program Files (x86)\\Adobe\\Acrobat Reader DC\\Reader\\AcroRd32.exe";
                break;

            // LOffice
            // -------
            case 4:
                return "http://ftp.rz.tu-bs.de/pub/mirror/tdf/tdf-pub/libreoffice/stable/7.0.0/win/x86_64/LibreOffice_7.0.0.3_Win_x64.msi";
                break;

            case 5:
                return "C:\\Program Files\\LibreOffice\\program\\soffice.exe";
                break;

            // VLC
            // ---
            case 6:
                return "https://get.videolan.org/vlc/3.0.8/win64/vlc-3.0.8-win64.exe";
                break;

            case 7:
                return "C:\\Program Files\\VideoLAN\\VLC\\vlc.exe";
                break;

            // PPViewer
            // --------
            case 8:
                return "https://gitlab.com/Atrate/qsh-resources/-/raw/master/PowerPointViewer.exe";
                break;

            case 9:
                return "C:\\Program Files (x86)\\Microsoft Office\\Office14\\PPTVIEW.exe";
                break;

            // BleachBit
            // ---------
            case 10:
                return "https://download.bleachbit.org/BleachBit-4.0.0-setup.exe";
                break;

            default:
                return "";
                break;
        }
    }
}
bool Procedure::check_shortcut(QString exe_path, const int NAME_NUM = -1)
{
#ifndef QT_NO_DEBUG
    assert(exe_path != "");
#endif
    qDebug() << "Checking shortcut for: " << exe_path << "\n";
    QString exe_name;

    switch (NAME_NUM)
    {
        case 0:
            exe_name = "Firefox";
            break;

        case 1:
            exe_name = "Adobe Reader";
            break;

        case 2:
            exe_name = "LibreOffice";
            break;

        case 3:
            exe_name = "VLC Media Player";
            break;

        case 4:
            exe_name = "PowerPoint Viewer";
            break;

        default:
            exe_name = QFileInfo(exe_path).baseName();
            break;
    }

    if (exe_path != "" && QFile().exists(exe_path))
    {
        exe_name =  exe_name.replace(0, 1, exe_name[0].toUpper());
        QDirIterator it("C:/Users/");
        qDebug() << "exe_name: " << exe_name << "\n";

        // Iterate through Users' Desktop folders to find whether shortcuts (symlinks) exist, if not, create them
        // ------------------------------------------------------------------------------------------------------
        while (it.hasNext())
        {
            it.next();
            qDebug() << "Iterator path: " << it.filePath() << "\n";
            qDebug() << "Checking for the existence of: " << (it.filePath() + "/Desktop/" + exe_name + ".exe") << "\n";
            qDebug() << "Checking for the existence of: " << (it.filePath() + "/Desktop/" + exe_name + ".lnk") << "\n";
            qDebug() << "Checking for the existence of: " << (it.filePath() + "/Desktop/" + exe_name) << "\n";

            if (QFile().exists(exe_path)
                    && QDir().exists(it.filePath() + "/Desktop")
                    && !((QFile().exists(it.filePath() + "/Desktop/" + exe_name))
                         || (QFile().exists(it.filePath() + "/Desktop/" + exe_name + ".exe"))
                         || (QFile().exists(it.filePath() + "/Desktop/" + exe_name + ".lnk")))
               )
            {
                try
                {
                    emit progress_description(tr("Creating shortcut to: ") + exe_name);
                    QString link_cmd = "mklink ";
                    link_cmd.append("\"" + QDir::toNativeSeparators(it.filePath()) + "\\Desktop\\" + exe_name + "\" \"" + QDir::toNativeSeparators(exe_path) + "\"");
                    qDebug() << "Link CMD: " << link_cmd << "\n";
                    (void) system(link_cmd.toUtf8());
                }
                catch (const std::exception &e)
                {
                    // Catch permission denied errors. We can't really do much about them, though,
                    // since the application is supposed to run as administrator anyways.
                }
            }
        }

        return true;
    }
    else
    {
        return false;
    }
}

int Procedure::install_software(const bool INS_FF, const bool INS_RDC, const bool INS_LOF, const bool INS_VLC, const bool INS_PPV)
{
    QTemporaryDir temp_folder;

    if (temp_folder.isValid())
    {
        if (chdir(temp_folder.path().toUtf8()) != 0)
        {
            return 3;
        }
    }
    else
    {
        return 3;
    }

    // Declare download links and file names
    // -------------------------------------
    const unsigned int DL_ARRAY_SIZE = 5;
    QString download_array[DL_ARRAY_SIZE][4];

    if (INS_FF)
    {
        download_array[0][0] = get_file_info(0);
        download_array[0][1] = "Firefox_Setup.msi";
        download_array[0][2] = get_file_info(1);
    }
    else
    {
        download_array[0][0] = QString("");
    }

    if (INS_RDC)
    {
        download_array[1][0] = get_file_info(2);
        download_array[1][1] = "readerdc_en_a_install.exe";
        download_array[1][2] = get_file_info(3);
    }
    else
    {
        download_array[1][0] = QString("");
    }

    if (INS_LOF)
    {
        download_array[2][0] = get_file_info(4);
        download_array[2][1] = "LibreOffice_Win_x64.msi";
        download_array[2][2] = get_file_info(5);
    }
    else
    {
        download_array[2][0] = QString("");
    }

    if (INS_VLC)
    {
        download_array[3][0] = get_file_info(6);
        download_array[3][1] = "vlc-win64.exe";
        download_array[3][2] = get_file_info(7);
    }
    else
    {
        download_array[3][0] = QString("");
    }

    if (INS_PPV)
    {
        download_array[4][0] = get_file_info(8);
        download_array[4][1] = "PowerPointViewer.exe";
        download_array[4][2] = get_file_info(9);
    }
    else
    {
        download_array[4][0] = QString("");
    }

    // Download the files
    // ------------------
    bool shortcut_array[DL_ARRAY_SIZE];

    for (unsigned int i = 0; i < DL_ARRAY_SIZE; i++)
    {
        if (download_array[i][2] != "")
        {
            shortcut_array[i] = check_shortcut(download_array[i][2], i);
        }

        if (!(download_array[i][0] == "" || shortcut_array[i]))
        {
            QFile().remove(download_array[i][1]);
            emit progress_description(tr("Downloading ") + download_array[i][1]);

            if (!qtcurl_dl(download_array[i][0].toUtf8(), download_array[i][1].toUtf8()))
            {
                QFile().remove(download_array[i][1]);
                download_array[i][0] = get_file_info(i * 2, true);

                if (!qtcurl_dl(download_array[i][0].toUtf8(), download_array[i][1].toUtf8()))
                {
                    return 1;
                }
            }

            emit progress_changed((i + 1) * 10);
        }
    }

    // Install the applications
    // ------------------------
    for (unsigned int i = 0; i < DL_ARRAY_SIZE; i++)
    {
        if (!(download_array[i][0] == "" || shortcut_array[i]))
        {
            // Check if the same version of the app is already installed. If so, just create a shortcut on the desktop.
            // --------------------------------------------------------------------------------------------------------
            QString cmd = download_array[i][1];
            emit progress_description(tr("Installing: ") + download_array[i][1]);

            switch (i)
            {
                case 0:
                    cmd.append("/S");
                    break;

                case 1:
                    break;

                case 2:
                    break;

                case 3:
                    cmd.append("/S /L=1033");
                    break;

                case 4:
                    break;
            }

            QFuture<int> install = QtConcurrent::run(system, cmd.toUtf8());

            while (install.isRunning())
            {
                QApplication::processEvents();
            }

            QFile().remove(download_array[i][1]);

            if (install != 0)
            {
                return 2;
            }
        }

        emit progress_changed((i + 6) * 10);
    }

    return 0;
}
int Procedure::clean(const bool EXT)
{
    // Find and remove all .bat and .cmd files from all users' desktops
    // ----------------------------------------------------------------
    qInfo() << tr("Starting cleaner…\n—————————————————");
    emit progress_description(tr("Starting cleaner…\n—————————————————"));
    qInfo() << tr("Removing .bat and .cmd files from the desktop…");
    emit progress_description(tr("Removing .bat and .cmd files from the desktop…"));

    try
    {
        QDirIterator it("C:/Users/");

        while (it.hasNext())
        {
            it.next();
            qDebug() << "Iterator path: " << it.filePath() << "\n";

            if (QDir().exists(it.filePath())
                    && QDir().exists(it.filePath() + "/Desktop/"))
            {
                QStringList filters;
                filters << "*.bat" << "*.cmd";
                QDir desktop(it.filePath() + "/Desktop/");
                desktop.setNameFilters(filters);

                for (QFile file : desktop.entryList())
                {
                    qDebug() << "Removing: " << file.fileName() << "\n";
                    file.remove();
                }
            }
        }
    }
    catch (const std::exception &e)
    {
        // Catch permission denied and no such file or directory errors. We can't really do much about them, though,
        // since the application is supposed to run as administrator anyways and we can't account for a lack of C:/Users/
    }

    // Run BleachBit to clean temporary files
    // --------------------------------------
    emit progress_changed(30);
    QString bb_path = "C:\\Program Files (x86)\\BleachBit\\bleachbit_console.exe";

    if (!QFile().exists(bb_path))
    {
        emit progress_description(tr("Downloading BleachBit (cleaning engine)…"));
        install_bb();
    }

    emit progress_changed(40);
    emit progress_description(tr("Cleaning temporary files and caches…"));
    bb_path = "\"" + bb_path + "\"";
    QString cmd = bb_path + " --clean adobe_reader.* amule.* chromium.* deepscan.tmp "
                  "filezilla.mru firefox.* flash.* gimp.tmp google_chrome.* google_toolbar.search_history "
                  "internet_explorer.* java.cache libreoffice.* microsoft_office.* openofficeorg.* opera.* "
                  "paint.mru realplayer.* safari.* silverlight.* skype.* smartftp.* system.clipboard "
                  "system.prefetch system.recycle_bin system.tmp vim.* waterfox.* winamp.mru windows_explorer.* "
                  "windows_media_player.* winrar.history winrar.temp winzip.mru wordpad.mru yahoo_messenger.*";
    QFuture<void> bb_clean = QtConcurrent::run(system, cmd.toUtf8());

    while (bb_clean.isRunning())
    {
        QApplication::processEvents();
    }

    // Extended cleaning
    // -----------------
    if (EXT)
    {
        cmd = bb_path + " --clean deepscan.ds_store deepscan.thumbs_db system.logs "
              "system.memory_dump system.muicache system.prefetch system.updates";
        emit progress_changed(60);
        emit progress_description(tr("Cleaning temporary files and caches (extended)…"));
        QFuture<void> bb_ext_clean = QtConcurrent::run(system, cmd.toUtf8());

        while (bb_ext_clean.isRunning())
        {
            QApplication::processEvents();
        }
    }

    return 0;
}
int Procedure::install_bb()
{
    QTemporaryDir temp_folder;

    if (temp_folder.isValid())
    {
        if (chdir(temp_folder.path().toUtf8()) != 0)
        {
            return 3;
        }
    }
    else
    {
        return 3;
    }

    QString bb_url = get_file_info(10);
    QString bb_exe = "BleachBit-setup.exe";

    if (!qtcurl_dl(bb_url.toUtf8(), bb_exe.toUtf8()))
    {
        QFile().remove(bb_exe);
        bb_url = get_file_info(10, true);

        if (!qtcurl_dl(bb_url.toUtf8(), bb_exe.toUtf8()))
        {
            return 1;
        }
    }

    bb_exe.append("/S /allusers");
    QFuture<int> bb_install = QtConcurrent::run(system, bb_exe.toUtf8());

    while (bb_install.isRunning())
    {
        QApplication::processEvents();
    }

    if (!bb_install)
    {
        return 2;
    }

    return 0;
}
int Procedure::run_install_bb()
{
    return install_bb();
}
int Procedure::run_install_software(const bool INS_FF, const bool INS_RDC, const bool INS_LOF, const bool INS_VLC, const bool INS_PPV)
{
    return install_software(INS_FF, INS_RDC, INS_LOF, INS_VLC, INS_PPV);
}
int Procedure::run_clean(const bool EXT)
{
    return clean(EXT);
}
int Procedure::run_qtcurl_dl(const char* url, const char* filename)
{
    return qtcurl_dl(url, filename);
}
