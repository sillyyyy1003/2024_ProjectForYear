#pragma once


class Timer
{
private:
    bool isStopped = false;

    double mSecondsPerCount = 0.0;
    double mDeltaTime = -1.0;

    __int64 mBaseTime = 0;
    __int64 mPausedTime = 0;
    __int64 mStopTime = 0;
    __int64 mPrevTime = 0;
    __int64 mCurrTime = 0;

public:
    __int64 mOldTime = 0;
    __int64 mNewTime = 0;
    
public:
    Timer();

    /// @brief トータルタイム
    /// @return Total Time from Reset
    float TotalTime()const;

    /// @brief Delta Time
    /// @return 
    float DeltaTime()const { return (float)mDeltaTime; };

    /// @brief リセット
    void Reset();               // 计时开始前或者需要重置时调用

    /// @brief 再開
    void Start();               // 在开始计时或取消暂停的时候调用

    /// @brief 一旦停止
    void Stop();                // 在需要暂停的时候调用

    /// @brief フレーム開始時使う
    void Tick();                // 在每一帧开始的时候调用

    /// @brief タイマーが停止しているかどうか
    /// @return 
    bool IsStopped() const { return isStopped; };



    void SetPrevTime()  { mPrevTime = mCurrTime; };

    void SetCurrTime();

    bool isSet();
};


