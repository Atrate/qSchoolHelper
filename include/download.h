#ifndef DOWNLOAD_H
#define DOWNLOAD_H
#include <curl/curl.h>

    int curl_dl(const char *url, const char *pagefilename);
    static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);
#endif

