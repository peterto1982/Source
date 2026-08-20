
#pragma once
#include <atomic>
#include <mutex>
#include <string>

struct SystemInfo {
    double cpu=0;
    double mem=0;
    double disk=0;
    long rxKB=0;
    long txKB=0;
};

class Monitor {
public:
    void start();
    void stop();
private:
    void cpuThread();
    void memThread();
    void diskThread();
    void netThread();
    std::atomic<bool> running{false};
};

extern SystemInfo gInfo;
extern std::mutex gMutex;
