#include "pch.h"
#include "framework.h"
#include "DBVideoFilesDlg.h"
#include "AboutDlg.h"
#include "Database.h"
#include "Drives.h"
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

CDBVideoFilesDlg::CDBVideoFilesDlg(CWnd* pParent)
    : CDialogEx(IDD_DBVIDEOFILES_DIALOG, pParent)
    , m_pDatabase(make_unique<Database>())
    , m_pDrives(make_unique<Drives>())
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CDBVideoFilesDlg::~CDBVideoFilesDlg(void)
{
}

void CDBVideoFilesDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_DRIVES, m_ListCtrlDrives);
    DDX_Control(pDX, IDC_LIST_FILES, m_ListBoxFiles);
    DDX_Control(pDX, IDC_BUTTON_REGISTER, m_ButtonRegister);
}

BEGIN_MESSAGE_MAP(CDBVideoFilesDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_REGISTER, &CDBVideoFilesDlg::OnBnClickedButtonRegister)
END_MESSAGE_MAP()

BOOL CDBVideoFilesDlg::OnInitDialog(void)
{
    CDialogEx::OnInitDialog();

    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != nullptr) {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if (!strAboutMenu.IsEmpty()) {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    m_ListCtrlDrives.SetExtendedStyle(m_ListCtrlDrives.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
    m_ListCtrlDrives.InsertColumn(0, L"論理ドライブ名", LVCFMT_LEFT, 200);
    m_ListCtrlDrives.InsertColumn(1, L"仮想ドライブ名", LVCFMT_LEFT, 200);
    m_ListCtrlDrives.InsertColumn(2, L"ID", LVCFMT_LEFT, 200);
    m_ListCtrlDrives.InsertColumn(3, L"シリアル番号", LVCFMT_LEFT, 400);

    auto count = m_pDrives->count();
    for (auto i = 0; i < count; ++i) {
        auto [driveLetter, virtualLetter, id, serialNumber] = m_pDrives->at(i);
        auto index = m_ListCtrlDrives.InsertItem(i, driveLetter.c_str());
        m_ListCtrlDrives.SetItemText(index, 1, virtualLetter.c_str());
        m_ListCtrlDrives.SetItemText(index, 2, id.c_str());
        m_ListCtrlDrives.SetItemText(index, 3, serialNumber.c_str());
    }

    return TRUE;
}

void CDBVideoFilesDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    } else {
        CDialogEx::OnSysCommand(nID, lParam);
    }
}

void CDBVideoFilesDlg::OnPaint(void)
{
    if (IsIconic()) {
        CPaintDC dc(this);

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width()  - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        dc.DrawIcon(x, y, m_hIcon);
    } else {
        CDialogEx::OnPaint();
    }
}

HCURSOR CDBVideoFilesDlg::OnQueryDragIcon(void)
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CDBVideoFilesDlg::OnBnClickedButtonRegister(void)
{
    if (m_ListCtrlDrives.GetSelectedCount() == 0) {
        return;
    }

    m_ListBoxFiles.ResetContent();

    auto pos = m_ListCtrlDrives.GetFirstSelectedItemPosition();
    while (pos) {
        auto index = m_ListCtrlDrives.GetNextSelectedItem(pos);

        auto [driveLetter, virtualLetter, id, serialNumber] = m_pDrives->at(index);

        CString strMessage;
        strMessage.Format(L"論理ドライブ名 = %s\n仮想ドライブ名 = %s\nID = %s\nシリアル番号 = %s",
            driveLetter.c_str(),
            virtualLetter.c_str(),
            id.c_str(),
            serialNumber.c_str());
        if (MessageBoxW(strMessage, ::AfxGetAppName(), MB_YESNO | MB_ICONQUESTION) == IDNO) {
            continue;
        }

        CWaitCursor waitCursor;

        auto files = m_pDrives->findFiles(index);
        for (const auto& file : files) {
            m_ListBoxFiles.AddString(file.c_str());
        }

        if (!m_pDatabase->registerFiles(id, files)) {
            MessageBoxW(L"データベースへの登録に失敗しました", ::AfxGetAppName(), MB_ICONERROR);
        }

        waitCursor.Restore();
    }

    MessageBoxW(L"終了しました", ::AfxGetAppName(), MB_ICONINFORMATION);
}
