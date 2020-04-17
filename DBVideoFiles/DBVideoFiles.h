#pragma once

#ifndef __AFXWIN_H__
#error "PCH に対してこのファイルをインクルードする前に 'pch.h' をインクルードしてください"
#endif

#include "Resource.h"

class CDBVideoFilesApp
    : public CWinApp
{
public:
    CDBVideoFilesApp(void);

    BOOL InitInstance(void) override;

protected:
    DECLARE_MESSAGE_MAP()
};

extern CDBVideoFilesApp theApp;
