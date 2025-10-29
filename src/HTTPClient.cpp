
#include "HTTPClient.h"
#include <iostream>

#ifdef _WIN32
#pragma comment(lib, "ws2_32.lib")
#endif

HTTPClient::HTTPClient() {
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {

    } else {

    }
}

HTTPClient::~HTTPClient() {
    if (curl) {
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

#ifdef _WIN32
    WSACleanup();
#endif
}

std::string HTTPClient::get(const std::string& url) {
    std::string response;

    if (curl) {

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

        curl_easy_setopt(curl, CURLOPT_USERAGENT, "SanatorioApp/1.0");

        CURLcode res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            return "";
        }

        long response_code;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

        if (response_code != 200) {
            return "";
        }
    }

    return response;
}

std::string HTTPClient::urlEncode(const std::string& text) {
    if (curl) {
        char* encoded = curl_easy_escape(curl, text.c_str(), text.length());
        if (encoded) {
            std::string result(encoded);
            curl_free(encoded);
            return result;
        }
    }
    return text;
}

size_t HTTPClient::writeCallback(void* contents, size_t size, size_t nmemb, std::string* data) {
    size_t totalSize = size * nmemb;
    data->append((char*)contents, totalSize);
    return totalSize;
}
