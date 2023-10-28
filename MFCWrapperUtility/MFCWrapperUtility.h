// MFCWrapperUtility.h : MFCWrapperUtility DLL의 주 헤더 파일
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'pch.h'를 포함합니다."
#endif

#include "resource.h"		// 주 기호입니다.
#include "CppVectorInternal.h"

// MFCWrapperUtilityApp
// 이 클래스 구현에 대해서는 MFCWrapperUtility.cpp를 참조하세요.
//

class MFCWrapperUtilityApp : public CWinApp
{
public:
	MFCWrapperUtilityApp();

// 재정의입니다.
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};


extern "C" __declspec(dllexport) void ShowMessageBox(LPCTSTR strText);

extern "C" __declspec(dllexport) int GetSum(int a, int b);

extern "C" __declspec(dllexport) void GetNativeStructs(CppVectorInternal<NativeStruct>*out);

extern "C" __declspec(dllexport) BSTR AsyncThreadTest();

extern "C" __declspec(dllexport) unsigned int AsyncThreadTest_2();
extern "C" __declspec(dllexport) void AsyncThreadTest_3();