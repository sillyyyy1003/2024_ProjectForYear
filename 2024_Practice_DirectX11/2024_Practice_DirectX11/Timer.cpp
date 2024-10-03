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

    //季節関連の初期化
    InitSeasonSetting();

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


}

void Timer::GameTick()
{
    
    if(!isPaused)
    {
        mAccumulatedRealTime += mDeltaTime;
        UpdateSystemTime();
    }
}




void Timer::UpdateSystemTime()
{

    if(isSkippedToNextDay)
    {
        SkipTime();
        return;
    }

    int realSecondsElapsed = static_cast<int>(mAccumulatedRealTime);

    if (realSecondsElapsed >= 1)  // Update game time every real-time second
    {
        mGameMinute += realSecondsElapsed * gameMinutesPerSecond;
        mAccumulatedRealTime -= realSecondsElapsed; // Subtract the converted time

        if (mGameMinute >= gameMinutesPerHour)
        {
            mGameHour += mGameMinute / gameMinutesPerHour;
            mGameHourPassed += mGameMinute / gameMinutesPerHour;//Accumulate Hours
            mGameMinute %= gameMinutesPerHour;
       

            if (mGameHour >= gameHoursPerDay)
            {
                mGameDay += mGameHour / gameHoursPerDay;
                mCurrentSeasonDayCounter += mGameHour / gameHoursPerDay;
            	mGameHour %= gameHoursPerDay;

                //季節の日数更新
                UpdateSeason();
            }
        }

    }

}

std::string Timer::GetSystemTime() const
{
    std::ostringstream oss;

    std::string seasonStr;
    switch (mCurrSeason)
    {
    case SPRING:
        seasonStr = "Spring";
        break;
    case SUMMER:
        seasonStr = "Summer";
        break;
    case AUTUMN:
        seasonStr = "Autumn";
        break;
    case WINTER:
        seasonStr = "Winter";
        break;
    }

    oss << "Day: " << mGameDay<<"\n"
        << "Time: " << std::setw(2) << std::setfill('0') << mGameHour
        << ":" << std::setw(2) << std::setfill('0') << mGameMinute<<"\n"
        << "Season: " << seasonStr
        << " (Day " << mCurrentSeasonDayCounter << " / " << mSeasonSet.at(mCurrSeason) << ")" << "Accumulate Hour: " << mGameHourPassed;

    return oss.str();
}

void Timer::Pause()
{
    if(!isPaused)
        isPaused = true;
}

void Timer::Resume()
{
    if (isPaused)
		isPaused = false;
}

void Timer::SkipTime(int dayNum)
{
    mGameMinute = 0;
    mGameHour = 0;
    mGameDay += dayNum;
    isSkippedToNextDay = false; // Reset the flag after skipping
    mAccumulatedRealTime = 0.0f;  // Reset accumulated real time
}

void Timer::InitSeasonSetting()
{
    //todo:後でファイルの読み込みで
    mSeasonSet.emplace(SPRING, 4);
    mSeasonSet.emplace(SUMMER, 2);
    mSeasonSet.emplace(AUTUMN, 4);
    mSeasonSet.emplace(WINTER, 2);
}

void Timer::SetDayValueInSeason(Season _season, int _days) noexcept
{

}

void Timer::UpdateSeason()
{
    if (mCurrentSeasonDayCounter > mSeasonSet.at(mCurrSeason))
    {
        mCurrentSeasonDayCounter = 1;  // Reset Day Counter

        // 切换到下一个季节
        switch (mCurrSeason)
        {
        case SPRING:
            mCurrSeason = SUMMER;
            break;
        case SUMMER:
            mCurrSeason = AUTUMN;
            break;
        case AUTUMN:
            mCurrSeason = WINTER;
            break;
        case WINTER:
            mCurrSeason = SPRING;
            break;
        }
    }
}







	