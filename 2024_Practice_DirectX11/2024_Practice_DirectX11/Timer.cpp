#include "Timer.h"
#include <iomanip>
#include <sstream>
#include "D3DApp.h"

Timer::Timer()
{
    __int64 countsPerSec{};
    QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
    mSecondsPerCount = 1.0 / (double)countsPerSec;

	__int64 currTime{};
    QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

    

}

float Timer::TotalTime() const
{
    /*
    // 如果调用了Stop()，暂停中的这段时间我们不需要计入。此外
    // mStopTime - mBaseTime可能会包含之前的暂停时间，为
    // 此我们可以从mStopTime减去之前累积的暂停的时间
    //
    //                     |<--Pause time -->|
    // ----*---------------*-----------------*------------*------------*------> time
    //  mBaseTime       mStopTime        startTime     mStopTime    mCurrTime
    */
    if (isStopped)
    {
        return (float)(((mStopTime - mPausedTime) - mBaseTime) * mSecondsPerCount);
    }
    /*
    // mCurrTime - mBaseTime包含暂停时间，但我们不想将它计入。
    // 为此我们可以从mCurrTime减去之前累积的暂停的时间
    //
    //  (mCurrTime - mausedTime) - mBaseTime 
    //
    //                     |<--Pause time -->|
    // ----*---------------*-----------------*------------*------> time
    //  mBaseTime       mStopTime        startTime     mCurrTime
     */
    else
    {
        return (float)(((mCurrTime - mPausedTime) - mBaseTime) * mSecondsPerCount);
    }
}

void Timer::Reset()
{
    __int64 currTime{};
    QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

    mBaseTime = currTime;
    mPrevTime = currTime;
    mStopTime = 0;
    mPausedTime = 0;  
    isStopped = false;
}

void Timer::Start()
{
    __int64 startTime{};
    QueryPerformanceCounter((LARGE_INTEGER*)&startTime);


    // 累积暂停开始到暂停结束的这段时间
    //
    //                     |<-------d------->|
    // ----*---------------*-----------------*------------> time
    //  mBaseTime       mStopTime        startTime     

    if (isStopped)
    {
        mPausedTime += (startTime - mStopTime);

        mPrevTime = startTime;
        mStopTime = 0;
        isStopped = false;
    }
}

void Timer::Stop()
{
    if (!isStopped)
    {
        __int64 currTime{};
        QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

        mStopTime = currTime;
        isStopped = true;
    }
}

void Timer::Tick()
{
    if (isStopped)
    {
        mDeltaTime = 0.0;
        return;
    }

    __int64 currTime{};
    QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
    mCurrTime = currTime;

	mDeltaTime = (mCurrTime - mPrevTime) * mSecondsPerCount;

	mPrevTime = mCurrTime;

    //DeltaTime
    if (mDeltaTime < 0.0)
    {
        mDeltaTime = 0.0;
    }

    // Update system time based on delta time
    if(!isStopped)
    {
        accumulatedRealTime += mDeltaTime;
        UpdateSystemTime();
    }
  
}

void Timer::UpdateSystemTime()
{
    int realSecondsElapsed = static_cast<int>(accumulatedRealTime);

    if (realSecondsElapsed >= 1)  // Update game time every real-time second
    {
        gameMinute += realSecondsElapsed * gameMinutesPerSecond;
        accumulatedRealTime -= realSecondsElapsed; // Subtract the converted time

        if (gameMinute >= gameMinutesPerHour)
        {
            gameHour += gameMinute / gameMinutesPerHour;
            gameMinute %= gameMinutesPerHour;

            if (gameHour >= gameHoursPerDay)
            {
                gameDay += gameHour / gameHoursPerDay;
                gameHour %= gameHoursPerDay;
            }
        }
    }
}

std::string Timer::GetSystemTime() const
{
    std::ostringstream oss;
    oss << "Day: " << gameDay
        << ", Time: " << std::setw(2) << std::setfill('0') << gameHour
        << ":" << std::setw(2) << std::setfill('0') << gameMinute;
    return oss.str();
 
}







