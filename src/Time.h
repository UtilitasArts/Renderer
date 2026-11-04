#pragma once
#include <chrono>

namespace Time {

    static constexpr uint64_t FPS           = 60;    
    static constexpr uint64_t NanoSecond    = 1'000'000'000;
    static constexpr uint64_t TimeStep      = NanoSecond/FPS;  
    static constexpr uint64_t MaxFrameTime  = TimeStep * 25;
    static constexpr double   fixed_dt      = 1.0/(double)FPS;

    class Timer {
    public:
        uint64_t getElapsedNanoSeconds(bool reset = false) {
            auto current_time = std::chrono::steady_clock::now();              
            std::chrono::nanoseconds duration = (current_time - start_time);
            if(reset) start_time = current_time;
            return duration.count();
        } 

        float getElapsedSeconds() {
            return ((float)getElapsedNanoSeconds() / (float)NanoSecond);
        }
        
        void Start() {
            start_time = std::chrono::steady_clock::now();
        }

        private:
        std::chrono::steady_clock::time_point start_time;
    };

    class TimeStepManager{
    public:
        TimeStepManager() {}

        bool TakeTimeStep() {      
            FrameTime     = FrameTimer.getElapsedNanoSeconds(true); 
            if (FrameTime >= MaxFrameTime) {          
                FrameTime = MaxFrameTime;
            }
            
            Accumulator += FrameTime;            
            return (Accumulator >= TimeStep); 
        }

        void IncTimeStep() {
            FrameCount++;
            FixedTime   += TimeStep;
            Accumulator -= TimeStep;  
        }    
        
        double UpdateAlpha() {
            Alpha = double(Accumulator)/double(TimeStep);
            return Alpha;
        }

        float getFixedTimeSeconds(){
            return ((float)FixedTime / (float)NanoSecond);
        }    

        void Start(){
            FrameTimer.Start();
            WorldTimer.Start();
        }

        void Log() {
            printf("FixedTime:%f\n RealTime:%f\n Alpha:%f\n Accumulator:%llu",
                getFixedTimeSeconds(),
                WorldTimer.getElapsedSeconds(),
                UpdateAlpha(),
                Accumulator
            );
        }

        Timer FrameTimer;
        Timer WorldTimer;
        uint64_t FixedTime   = 0;
        uint64_t Accumulator = 0;
        uint64_t FrameCount  = 0;
        uint64_t FrameTime   = 0;
        double   Alpha       = 0; 
    };

    class TimeStepManager Manager;

}
