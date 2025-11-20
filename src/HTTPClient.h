//
// Created by Carol on 1/10/2025.
//

#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H


#ifndef NOMINMAX
#define NOMINMAX
#endif

#define byte win_byte_override

#include <curl/curl.h>

#undef byte

#include <string>

// Para Windows
#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#endif


class HTTPClient {
public:
    HTTPClient();
    ~HTTPClient();

    std::string get(const std::string& url);
    std::string post(const std::string& url, const std::string& jsonData);
    std::string urlEncode(const std::string& text);

private:
    CURL* curl;

    static size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* data);
};

#endif