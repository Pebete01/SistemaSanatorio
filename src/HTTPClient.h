//
// Created by Carol on 1/10/2025.
//

#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H


// ⚙️ Evitar conflictos entre std::byte y byte de Windows
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

    // Realizar request GET y devolver respuesta
    std::string get(const std::string& url);

    // Escapar caracteres especiales en URL
    std::string urlEncode(const std::string& text);

private:
    CURL* curl;

    // Callback para escribir la respuesta
    static size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* data);
};

#endif