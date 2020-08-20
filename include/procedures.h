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

#ifndef PROCEDURES_H
#define PROCEDURES_H

#include <curl.h>

int curl_dl(const char *url, const char *pagefilename);
int install_software(const bool INS_FF, const bool INS_RDC, const bool INS_LOF, const bool INS_VLC, const bool INS_PPV);
int clean(const bool EXT);
void install_bb();

#endif // PROCEDURES_H
