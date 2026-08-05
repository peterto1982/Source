
#include "Monitor.h"
#include <iostream>
#include <thread>
#include <chrono>

int main(){
    Monitor m;
    m.start();
    for(int i=0;i<30;i++){
        {
            std::lock_guard<std::mutex> l(gMutex);
            std::cout<<"CPU: "<<gInfo.cpu<<"%  "
                     <<"MEM: "<<gInfo.mem<<"%  "
                     <<"DISK: "<<gInfo.disk<<"%  "
                     <<"RX: "<<gInfo.rxKB<<" KB  "
                     <<"TX: "<<gInfo.txKB<<" KB\n";
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    m.stop();
    return 0;
}
