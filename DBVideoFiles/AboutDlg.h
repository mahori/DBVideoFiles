#pragma once

class CAboutDlg
    : public CDialogEx
{
public:
    CAboutDlg(void);

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_ABOUTBOX };
#endif

protected:
    void DoDataExchange(CDataExchange* pDX) override;

    DECLARE_MESSAGE_MAP()
};
