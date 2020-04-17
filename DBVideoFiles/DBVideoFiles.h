
// DBVideoFiles.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです
//

#pragma once

#ifndef __AFXWIN_H__
#error "PCH に対してこのファイルをインクルードする前に 'pch.h' をインクルードしてください"
#endif

#include "resource.h"       // メイン シンボル


// CDBVideoFilesApp:
// このクラスの実装については、DBVideoFiles.cpp を参照してください
//

class CDBVideoFilesApp : public CWinApp
{
public:
    CDBVideoFilesApp();

    // オーバーライド
public:
    virtual BOOL InitInstance();

    // 実装

    DECLARE_MESSAGE_MAP()
};

extern CDBVideoFilesApp theApp;
