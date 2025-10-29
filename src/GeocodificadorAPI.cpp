//
// Created by Carol on 1/10/2025.
//

#include "GeocodificadorAPI.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include <sstream>

std::string leerAPIKeyDesdeEnv() {
    std::ifstream file(".env");
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo .env" << std::endl;
        return "";
    }

    std::string line;
    while (std::getline(file, line)) {
        // Buscar línea que empiece con "API_KEY="
        if (line.find("API_KEY=") == 0) {
            return line.substr(8);  // Extraer después de "API_KEY="
        }
    }

    std::cerr << "Error: API_KEY no encontrada en .env" << std::endl;
    return "";
}

GeocodificadorAPI::GeocodificadorAPI() : regionPorDefecto("AR") {
    //std::cout << "Geocodificador API inicializado\n";
}

void GeocodificadorAPI::configurarAPIKey(const std::string& apiKey) {
    this->apiKey = apiKey;
}

void GeocodificadorAPI::configurarRegion(const std::string& pais) {
    this->regionPorDefecto = pais;
}

std::pair<double, double> GeocodificadorAPI::obtenerCoordenadas(const std::string& direccion) {

    auto resultado = usarNominatim(direccion);
    if (coordenadasValidas(resultado.first, resultado.second)) {
        return resultado;
    }

    if (!apiKey.empty()) {
        resultado = usarGoogleMaps(direccion);
        if (coordenadasValidas(resultado.first, resultado.second)) {

            return resultado;
        }
    }

    resultado = usarPositionStack(direccion);
    if (coordenadasValidas(resultado.first, resultado.second)) {
        return resultado;
    }

    return {0.0, 0.0};
}

std::pair<double, double> GeocodificadorAPI::usarNominatim(const std::string& direccion) {
    try {
        std::string url = "https://nominatim.openstreetmap.org/search?format=json&limit=1&q=" +
                          httpClient.urlEncode(direccion);


        if (!regionPorDefecto.empty()) {
            url += "&countrycodes=" + regionPorDefecto;
        }

        std::string respuesta = httpClient.get(url);
        if (respuesta.empty()) {
            return {0.0, 0.0};
        }

        json jsonRespuesta = json::parse(respuesta);
        return parsearNominatim(jsonRespuesta);

    } catch (const std::exception& e) {
        return {0.0, 0.0};
    }
}

std::pair<double, double> GeocodificadorAPI::usarGoogleMaps(const std::string& direccion) {
    if (apiKey.empty()) {

        return {0.0, 0.0};
    }

    try {
        std::string url = "https://maps.googleapis.com/maps/api/geocode/json?address=" +
                          httpClient.urlEncode(direccion) +
                          "&key=" + apiKey;

        if (!regionPorDefecto.empty()) {
            url += "&region=" + regionPorDefecto;
        }

        std::string respuesta = httpClient.get(url);
        if (respuesta.empty()) {
            return {0.0, 0.0};
        }

        json jsonRespuesta = json::parse(respuesta);
        return parsearGoogleMaps(jsonRespuesta);

    } catch (const std::exception& e) {
        return {0.0, 0.0};
    }
}

std::pair<double, double> GeocodificadorAPI::usarPositionStack(const std::string& direccion) {
    try {
        std::string API_KEY = leerAPIKeyDesdeEnv();
        if (API_KEY.empty()) {
            return {0.0, 0.0};
        }
        std::string url = "https://api.positionstack.com/v1/forward?access_key=" + API_KEY + "&query=" +
                          httpClient.urlEncode(direccion);


        std::string respuesta = httpClient.get(url);
        if (respuesta.empty()) {
            return {0.0, 0.0};
        }

        json jsonRespuesta = json::parse(respuesta);
        return parsearPositionStack(jsonRespuesta);

    } catch (const std::exception& e) {
        return {0.0, 0.0};
    }
}

std::pair<double, double> GeocodificadorAPI::parsearNominatim(const json& respuesta) {
    if (respuesta.is_array() && !respuesta.empty()) {
        auto primer_resultado = respuesta[0];

        if (primer_resultado.contains("lat") && primer_resultado.contains("lon")) {
            double lat = std::stod(primer_resultado["lat"].get<std::string>());
            double lon = std::stod(primer_resultado["lon"].get<std::string>());


            return {lat, lon};
        }
    }
    return {0.0, 0.0};
}

std::pair<double, double> GeocodificadorAPI::parsearGoogleMaps(const json& respuesta) {
    if (respuesta.contains("status") && respuesta["status"] == "OK" &&
        respuesta.contains("results") && !respuesta["results"].empty()) {

        auto primer_resultado = respuesta["results"][0];

        if (primer_resultado.contains("geometry") &&
            primer_resultado["geometry"].contains("location")) {

            auto location = primer_resultado["geometry"]["location"];
            double lat = location["lat"];
            double lon = location["lng"];

            return {lat, lon};
        }
    }
    return {0.0, 0.0};
}

std::pair<double, double> GeocodificadorAPI::parsearPositionStack(const json& respuesta) {
    if (respuesta.contains("data") && !respuesta["data"].empty()) {
        auto primer_resultado = respuesta["data"][0];

        if (primer_resultado.contains("latitude") && primer_resultado.contains("longitude")) {
            double lat = primer_resultado["latitude"];
            double lon = primer_resultado["longitude"];

            return {lat, lon};
        }
    }
    return {0.0, 0.0};
}

bool GeocodificadorAPI::coordenadasValidas(double lat, double lon) {
    return (lat != 0.0 || lon != 0.0) &&
           lat >= -90.0 && lat <= 90.0 &&
           lon >= -180.0 && lon <= 180.0;
}

void GeocodificadorAPI::logError(const std::string& mensaje) {
    std::cerr << "✖ " << mensaje << std::endl;
}