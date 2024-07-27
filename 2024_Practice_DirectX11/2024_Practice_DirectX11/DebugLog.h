#pragma once
#include <fstream>
#include <string>
#include <format>

namespace Log
{
	//���b�Z�[�W�̃^�C�v
	enum class Type:int
	{
        LOG_INFO = 0,
        LOG_WARNING = 1,
		LOG_ERROR = 2,
	};
}

/// @brief ���o��
class DebugLog
{
private:
    DebugLog();
    ~DebugLog() = default;

private:
    /// @brief �V���O���g���C���X�^���X�擾
    static DebugLog& GetInstance();

    /// @brief ���b�Z�[�W�o��
    /// @param message ���b�Z�[�W 
    /// @param type ���b�Z�[�W�̎��
    void LogOutput(const std::string& message, Log::Type type);


    std::ofstream ofs;

public:
    /// @brief ������
    /// @param _fileName �o�̓t�@�C���� 
    /// @return �������������ۂ�
    bool Init(const char* _fileName);

    //About template<class... Args>https://conscious-maple-e95.notion.site/Template-class-Args-90f56b52ea7b4259803ffb917cb81547?pvs=74
    template<class... Args>
    static void Log(std::string_view rt_fmt_str, Args && ... args)
    {
        std::string str = std::vformat(rt_fmt_str, std::make_format_args(args...));
        DebugLog::GetInstance().LogOutput(str, Log::Type::LOG_INFO);
    };

    template<class... Args>
    static void LogWarning(std::string_view rt_fmt_str, Args && ... args)
    {
        std::string str = std::vformat(rt_fmt_str, std::make_format_args(args...));
        DebugLog::GetInstance().LogOutput(str, Log::Type::LOG_WARNING);
    };

    template<class... Args>
    static void LogError(std::string_view rt_fmt_str, Args && ... args)
    {
        std::string str = std::vformat(rt_fmt_str, std::make_format_args(args...));
        DebugLog::GetInstance().LogOutput(str, Log::Type::LOG_ERROR);
    };
};