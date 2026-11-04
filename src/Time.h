#pragma once
#include <chrono>

namespace Time {

static constexpr uint64_t FPS = 60;    
static constexpr uint64_t NanoSecond    = 1'000'000'000;
static constexpr uint64_t TimeStep      = NanoSecond/FPS;  
static constexpr uint64_t MaxFrameTime  = NanoSecond/4;

class Timer {
public:
    Timer() : start_time(std::chrono::steady_clock::now()) {}    
    
    uint64_t getElapsedNanoSeconds() {
        auto current_time = std::chrono::steady_clock::now();              
        std::chrono::nanoseconds duration = (current_time - start_time);
        return duration.count();
    } 

    float getElapsedSeconds() {
        return ((float)getElapsedNanoSeconds() / (float)NanoSecond);
    } 
    
    void Reset() {
        start_time = std::chrono::steady_clock::now();
    }

    private:
	std::chrono::steady_clock::time_point start_time;  
};

class TimeStepManager{
public:
    TimeStepManager() {}

    void TakeTimeStep() {      
        FrameTime     = GlobalTimer.getElapsedNanoSeconds(); 
        if (FrameTime >= MaxFrameTime) {          
            FrameTime = MaxFrameTime;
        }  

        Accumulator += FrameTime; 
        GlobalTimer.Reset();
    }

    bool ShouldUpdate() {          
        return (Accumulator >= TimeStep);           
    }

    void IncTimeStep() {
        FrameCount++;
        Time        += TimeStep;
        Accumulator -= TimeStep;  
    }    
    
    void UpdateAlpha() {
        Alpha = Accumulator/TimeStep;
    }

    float getTimeSeconds(){
        return ((float)Time / (float)NanoSecond);
    }    

    Timer GlobalTimer;
    Timer ActualTimer;
	uint64_t Time        = 0;
    uint64_t Accumulator = 0;
    uint64_t FrameCount  = 0;
    uint64_t FrameTime   = 0;
    uint64_t Alpha       = 0; 
};

 class TimeStepManager Manager;

}
