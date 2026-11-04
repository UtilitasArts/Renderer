#include "Timer.h"

  //--------/
 // Timer  /
//========/

uint64_t Timer::Timer::GetElapsedNanoSeconds(bool reset) {
    auto current_time = std::chrono::steady_clock::now();              
    std::chrono::nanoseconds duration = (current_time - start_time);
    if(reset) start_time = current_time;
    return duration.count();
}
double Timer::Timer::GetElapsedSeconds(bool bPrint){
    double elapsedSeconds = ((double)GetElapsedNanoSeconds() / (double)NanoSecond);
    if(bPrint == 1) printf("Elapsed Seconds = %f",elapsedSeconds );
    return elapsedSeconds;
}
void Timer::Timer::Start(){
    start_time = std::chrono::steady_clock::now();
}

  //--------------------/
 // Time Step Manager /
//==================/

void Timer::TimeStepManager::MeasureTimeStep(){      
    FrameTime      = FrameTimer.GetElapsedNanoSeconds(true); 
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
    }   else {
        Alpha = double(Accumulator)/double(TimeStep);
        return false;
    } 
}

double Timer::TimeStepManager::GetFixedTimeSeconds(){
    return ((double)FixedTime / (double)NanoSecond);
}

double Timer::TimeStepManager::GetFrameTimeSeconds(){
    return ((double)FrameTime / (double)NanoSecond);
}
void Timer::TimeStepManager::Start(){
    FrameTimer.Start();
    WorldTimer.Start();
}

void Timer::TimeStepManager::log(){
    printf("FixedTime:%f\n RealTime:%f\n Alpha:%f\n Accumulator:%llu",
        GetFixedTimeSeconds(),
        WorldTimer.GetElapsedSeconds(),
        Alpha,
        Accumulator
    );
}