#pragma once
#include <string>
#include <unordered_map>

#include "Windows.h"

enum Season
{
	SPRING,
    SUMMER,
    AUTUMN,
    WINTER,
};


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
    //todo:ここの部分をSceneManagerに移行
    int gameMinutesPerSecond = 60;  // 1 second in real-time equals 10 minutes in-game
    int gameHoursPerDay = 24;
    int gameMinutesPerHour = 60;

    int mGameDay = 1;
    int mGameHour = 0;
    int mGameMinute = 0;
    double mAccumulatedRealTime = 0.0;   // Accumulated real-time since last update //todo:RESET FUNCTION
    double mGameHourPassed = 0.0f;    // Accumulated game minute

    int mCurrentSeasonDayCounter = 1; 
    Season mCurrSeason = SPRING;  //今の季節

    using SeasonSet = std::unordered_map<Season, int>;//各季節の日数設定用
    SeasonSet mSeasonSet;
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
    float DeltaTime()const { return static_cast<float>((mDeltaTime)); };

    /// @brief リセット
    void Reset();              

    /// @brief 再開
    void Start();             

    /// @brief 一旦停止
    void Stop();            

    /// @brief フレーム開始時使う
    void Tick();               

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

    /// @brief ゲーム内の季節と日数の初期化
    void InitSeasonSetting();

    /// @brief 季節の日数を設定する
    /// @param _season 季節
    /// @param _days 日数
    void SetDayValueInSeason(Season _season, int _days) noexcept;

    /// @brief 季節の更新状態
    void UpdateSeason();

    Season GetCurrentSeason() noexcept { return mCurrSeason; };

	double GetGameHourPassed() const noexcept { return mGameHourPassed; };
};


