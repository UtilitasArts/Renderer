#pragma once
#include <chrono>

namespace Timer {
static constexpr uint64_t NanoSecond    = 1'000'000'000;
    class Timer{
        public:
            uint64_t  getElapsedNanoSeconds(bool reset = false);
            double    getElapsedSeconds(bool bPrint = false);
            void      start();
            
        private:
            std::chrono::steady_clock::time_point start_time;
    };

    class TimeStepManager{
        public:   
            void   MeasureTimeStep();
            bool   IncTimeStep();               
            double UpdateAlpha();
            double  getFixedTimeSeconds();
            double  getFrameTimeSeconds();
            void   start();
            void   log();

            Timer FrameTimer;
            Timer WorldTimer;
            uint64_t FixedTime   = 0;
            uint64_t Accumulator = 0;
            uint64_t FrameCount  = 0;
            uint64_t FrameTime   = 0;
            double   Alpha       = 0; 

            static constexpr uint64_t FPS           = 60;            
            static constexpr uint64_t TimeStep      = NanoSecond/FPS;  
            static constexpr uint64_t MaxFrameTime  = TimeStep * 25;
            static constexpr double   fixed_dt      = 1.0/(double)FPS;
    };


}
