#include "Timer.h"

  //--------/
 // Timer  /
//========/

uint64_t Timer::Timer::getElapsedNanoSeconds(bool reset) {
    auto current_time = std::chrono::steady_clock::now();              
    std::chrono::nanoseconds duration = (current_time - start_time);
    if(reset) start_time = current_time;
    return duration.count();
}
double Timer::Timer::getElapsedSeconds(bool bPrint){
    double elapsedSeconds = ((double)getElapsedNanoSeconds() / (double)NanoSecond);
    if(bPrint == 1) printf("Elapsed Seconds = %f",elapsedSeconds );
    return elapsedSeconds;
}
void Timer::Timer::start(){
    start_time = std::chrono::steady_clock::now();
}

  //--------------------/
 // Time Step Manager /
//==================/

void Timer::TimeStepManager::MeasureTimeStep(){      
    FrameTime      = FrameTimer.getElapsedNanoSeconds(true); 
    if (FrameTime >= MaxFrameTime) {          
        FrameTime  = MaxFrameTime;
    }     
    Accumulator += FrameTime;   
}

bool Timer::TimeStepManager::IncTimeStep(){
    if(Accumulator >= TimeStep){
        FrameCount  += 1;
        FixedTime   += TimeStep;
        Accumulator -= TimeStep;
        
        return true;
    } else return false;
}

double Timer::TimeStepManager::UpdateAlpha(){
    Alpha = double(Accumulator)/double(TimeStep);
    return Alpha;
}

double Timer::TimeStepManager::getFixedTimeSeconds(){
    return ((double)FixedTime / (double)NanoSecond);
}

double Timer::TimeStepManager::getFrameTimeSeconds(){
    return ((double)FrameTime / (double)NanoSecond);
}
void Timer::TimeStepManager::start(){
    FrameTimer.start();
    WorldTimer.start();
}

void Timer::TimeStepManager::log(){
    printf("FixedTime:%f\n RealTime:%f\n Alpha:%f\n Accumulator:%llu",
        getFixedTimeSeconds(),
        WorldTimer.getElapsedSeconds(),
        UpdateAlpha(),
        Accumulator
    );
}