#pragma once
#include <string>
#include "Windows.h"

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

    //===============================================
	//  以下の部分はゲームループ内に使われる変数
    //===============================================
    // Game time related variables
    int gameMinutesPerSecond = 10;  // 1 second in real-time equals 10 minutes in-game
    int gameHoursPerDay = 24;
    int gameMinutesPerHour = 60;

    int gameDay = 1;
    int gameHour = 0;
    int gameMinute = 0;
    double accumulatedRealTime = 0.0;  // Accumulated real-time since last update

public:

    bool isPaused = false;  //ゲーム内Pause
    bool isSkippedToNextDay = false;

    DWORD mNewTime = 0;
    DWORD mOldTime = 0;

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

    /// @brief ゲーム内の時間計算
    void GameTick();

    /// @brief System time related methods
    void UpdateSystemTime();

    /// @brief 今のシステム時間出力
    /// @return システム時間（std::string）
    std::string GetSystemTime() const ;

    /// @brief ゲーム時間計算を停止する
    void Pause();

    /// @brief ゲーム時間計算を再開する
    void Resume();

    /// @brief 一日Skip
    void SkipTime(int dayNum = 1);

    /// @brief スキップ
    void Skip(bool isSkipped) { isSkippedToNextDay = isSkipped; };
};


