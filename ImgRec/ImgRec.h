
// ImgRec.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CImgRecApp: 
// �йش����ʵ�֣������ ImgRec.cpp
//

class CImgRecApp : public CWinApp
{
public:
	CImgRecApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CImgRecApp theApp;