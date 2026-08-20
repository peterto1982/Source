#include "Monitor.h"

#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

int main()
{
    Monitor monitor;

    // Start monitoring threads
    monitor.start();

    // Wait for the first sample to be collected
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Print collected statistics
    {
        std::lock_guard<std::mutex> lock(gMutex);

        std::cout << "=================================\n";
        std::cout << "          SysMon Lite\n";
        std::cout << "=================================\n";
        std::cout << "CPU Usage    : " << gInfo.cpu << " %\n";
        std::cout << "Memory Usage : " << gInfo.mem << " %\n";
        std::cout << "Disk Usage   : " << gInfo.disk << " %\n";
        std::cout << "RX           : " << gInfo.rxKB << " KB\n";
        std::cout << "TX           : " << gInfo.txKB << " KB\n";
        std::cout << "=================================\n";
    }

    // Stop monitoring
    monitor.stop();

    return 0;
}
