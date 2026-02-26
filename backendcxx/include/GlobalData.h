#pragma once
#include <mutex>
#include <string>
struct GpsInfo {
    float lat = 0.0f;
    float lon = 0.0f;
    std::string text = "waiting...";
    bool isConnected = false;
};
class GlobalData {
    std::mutex mtx;
    GpsInfo info;
public:
    void update(float lat, float lon, std::string msg) {
        std::lock_guard<std::mutex> lock(mtx);
        info.lat = lat;
        info.lon = lon;
        info.text = msg;
        info.isConnected = true; 
    }
    GpsInfo get() {
        std::lock_guard<std::mutex> lock(mtx);
        return info;
    }
};