#include "DebugLog.h"
#include <iostream>


DebugLog::DebugLog()
{
    Init("Debug.txt");
}


DebugLog& DebugLog::GetInstance()
{
    static DebugLog instance;
    return instance;
}

void DebugLog::LogOutput(const std::string& message, Log::Type type)
{
    switch (type)
    {
    case Log::Type::LOG_INFO:
    default:
        ofs << message << std::endl;
        std::cout << message << std::endl;
        break;

    case Log::Type::LOG_WARNING:
        ofs << "WARNING::" << message << std::endl;
        std::cout << "WARNING::" << message << std::endl;
        break;

    case Log::Type::LOG_ERROR:
        ofs << "ERROR::" << message << std::endl;
        std::cout << "ERROR::" << message << std::endl;
        break;
    }
}

bool DebugLog::Init(const char* _fileName)
{
    ofs = std::ofstream(_fileName);
    if (!ofs)
    {
        std::cout << "���O�V�X�e���������Ɏ��s���܂����B" << std::endl;
        return false;
    }

    std::cout << "���O�e�L�X�g����or�㏑������ = " << _fileName << std::endl;
    return true;
}