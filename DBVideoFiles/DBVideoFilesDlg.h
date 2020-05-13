#pragma once

class Database;
class Drives;

class CDBVideoFilesDlg
    : public CDialogEx
{
public:
    CDBVideoFilesDlg(CWnd* pParent = nullptr);
    ~CDBVideoFilesDlg(void);

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DBVIDEOFILES_DIALOG };
#endif

protected:
    HICON m_hIcon;

    void DoDataExchange(CDataExchange* pDX) override;
    BOOL OnInitDialog(void) override;

    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint(void);
    afx_msg HCURSOR OnQueryDragIcon(void);
    afx_msg void OnBnClickedButtonRegister(void);

    DECLARE_MESSAGE_MAP()

private:
    CListCtrl m_ListCtrlDrives;
    CListBox m_ListBoxFiles;
    CButton m_ButtonRegister;
    std::unique_ptr<Database> m_pDatabase;
    std::unique_ptr<Drives> m_pDrives;
};
