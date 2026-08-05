
#include "Monitor.h"
#include <thread>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <sstream>
using namespace std::chrono_literals;
SystemInfo gInfo;
std::mutex gMutex;

static void sleep(){std::this_thread::sleep_for(1s);}

void Monitor::cpuThread(){
    long long u,n,s,i,pu=0,pn=0,ps=0,pi=0;
    while(running){
        std::ifstream f("/proc/stat");
        std::string cpu;
        f>>cpu>>u>>n>>s>>i;
        long long idle=i-pi, total=(u-pu)+(n-pn)+(s-ps)+idle;
        if(total>0){
            std::lock_guard<std::mutex> l(gMutex);
            gInfo.cpu=100.0*(total-idle)/total;
        }
        pu=u;pn=n;ps=s;pi=i;
        sleep();
    }
}
void Monitor::memThread(){
    while(running){
        std::ifstream f("/proc/meminfo");
        std::string k; long t=0,a=0,v;
        while(f>>k>>v){
            if(k=="MemTotal:") t=v;
            if(k=="MemAvailable:"){a=v;break;}
            f.ignore(1000,'\n');
        }
        std::lock_guard<std::mutex> l(gMutex);
        gInfo.mem=100.0*(t-a)/t;
        sleep();
    }
}
void Monitor::diskThread(){
    while(running){
        auto s=std::filesystem::space("/");
        std::lock_guard<std::mutex> l(gMutex);
        gInfo.disk=100.0*(double)(s.capacity-s.available)/s.capacity;
        sleep();
    }
}
void Monitor::netThread(){
    while(running){
        std::ifstream f("/proc/net/dev");
        std::string line;
        std::getline(f,line); std::getline(f,line);
        long rx=0,tx=0;
        while(std::getline(f,line)){
            if(line.find("lo:")!=std::string::npos) continue;
            auto c=line.find(':');
            std::string rest=line.substr(c+1);
            std::istringstream iss(rest);
            long vals[16]={0};
            for(int i=0;i<16 && iss>>vals[i];++i){}
            rx+=vals[0]/1024;
            tx+=vals[8]/1024;
        }
        std::lock_guard<std::mutex> l(gMutex);
        gInfo.rxKB=rx; gInfo.txKB=tx;
        sleep();
    }
}
void Monitor::start(){
    running=true;
    std::thread(&Monitor::cpuThread,this).detach();
    std::thread(&Monitor::memThread,this).detach();
    std::thread(&Monitor::diskThread,this).detach();
    std::thread(&Monitor::netThread,this).detach();
}
void Monitor::stop(){running=false;}
