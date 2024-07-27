//Trace DirectX Error
#pragma once

#include <Windows.h>

/// @brief 
/// @param strFile File name
/// @param dwLine hlsl line num
/// @param hr HRESULT data
/// @param strMsg String Message
/// @param bPopMsgBox MessageBox
/// @return 
HRESULT WINAPI DXTraceW(_In_z_ const WCHAR* strFile, 
	_In_ DWORD dwLine,
	_In_ HRESULT hr,
	_In_opt_ const WCHAR* strMsg,
	_In_ bool bPopMsgBox);


// ------------------------------
// HR宏
// ------------------------------
// Debug Mode Trace the bug
#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)												    \
    {															\
        HRESULT hr = (x);										\
        if(FAILED(hr))											\
        {														\
            DXTraceW(__FILEW__, (DWORD)__LINE__, hr, L#x, true);\
        }														\
    }
#endif
#else
#ifndef HR
#define HR(x) (x)
#endif 
#endif
