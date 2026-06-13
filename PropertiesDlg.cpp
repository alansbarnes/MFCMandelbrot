#include "pch.h"
#include "MFCMandelbrot.h"
#include "PropertiesDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{
    constexpr int kColorMin = 0;
    constexpr int kColorMax = 255;

    class CScopedInternalUpdate
    {
    public:
        explicit CScopedInternalUpdate(bool& internalUpdate)
            : m_internalUpdate(internalUpdate)
        {
            m_internalUpdate = true;
        }

        ~CScopedInternalUpdate()
        {
            m_internalUpdate = false;
        }

    private:
        bool& m_internalUpdate;
    };
}

extern Properties g_props;

IMPLEMENT_DYNAMIC(CPropertiesDlg, CDialogEx)

CPropertiesDlg::CPropertiesDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_PROPERTIES, pParent)
{
    m_maxIter = g_props.maxIter;
    m_centerReal = g_props.centerReal;
    m_centerImag = g_props.centerImag;
    m_height = g_props.height;
    m_rmin = g_props.rmin;
    m_rmax = g_props.rmax;
    m_gmin = g_props.gmin;
    m_gmax = g_props.gmax;
    m_bmin = g_props.bmin;
    m_bmax = g_props.bmax;
}

CPropertiesDlg::~CPropertiesDlg()
{
}

BOOL CPropertiesDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    InitSpinControl(IDC_SPIN_RMIN, IDC_EDIT_RMIN, kColorMin, kColorMax);
    InitSpinControl(IDC_SPIN_RMAX, IDC_EDIT_RMAX, kColorMin, kColorMax);
    InitSpinControl(IDC_SPIN_GMIN, IDC_EDIT_GMIN, kColorMin, kColorMax);
    InitSpinControl(IDC_SPIN_GMAX, IDC_EDIT_GMAX, kColorMin, kColorMax);
    InitSpinControl(IDC_SPIN_BMIN, IDC_EDIT_BMIN, kColorMin, kColorMax);
    InitSpinControl(IDC_SPIN_BMAX, IDC_EDIT_BMAX, kColorMin, kColorMax);

    SyncMinMaxPair(IDC_EDIT_RMIN, IDC_EDIT_RMAX);
    SyncMinMaxPair(IDC_EDIT_GMIN, IDC_EDIT_GMAX);
    SyncMinMaxPair(IDC_EDIT_BMIN, IDC_EDIT_BMAX);

    return TRUE;
}

void CPropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);

    DDX_Text(pDX, IDC_EDIT_MAXITER, m_maxIter);
    DDX_Text(pDX, IDC_EDIT_CENTER_REAL, m_centerReal);
    DDX_Text(pDX, IDC_EDIT_CENTER_IMAG, m_centerImag);
    DDX_Text(pDX, IDC_EDIT_HEIGHT, m_height);
    DDX_Text(pDX, IDC_EDIT_RMIN, m_rmin);
    DDX_Text(pDX, IDC_EDIT_RMAX, m_rmax);
    DDX_Text(pDX, IDC_EDIT_GMIN, m_gmin);
    DDX_Text(pDX, IDC_EDIT_GMAX, m_gmax);
    DDX_Text(pDX, IDC_EDIT_BMIN, m_bmin);
    DDX_Text(pDX, IDC_EDIT_BMAX, m_bmax);
}

BEGIN_MESSAGE_MAP(CPropertiesDlg, CDialogEx)
    ON_CONTROL_RANGE(EN_CHANGE, IDC_EDIT_RMIN, IDC_EDIT_BMAX, &CPropertiesDlg::OnColorEditChange)
END_MESSAGE_MAP()

BOOL CPropertiesDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
    NMHDR* pHdr = reinterpret_cast<NMHDR*>(lParam);
    if (pHdr != nullptr && pHdr->code == UDN_DELTAPOS)
    {
        if (HandleSpinDelta(reinterpret_cast<NMUPDOWN*>(lParam)))
        {
            if (pResult != nullptr)
            {
                *pResult = 1;
            }
            return TRUE;
        }
    }

    return CDialogEx::OnNotify(wParam, lParam, pResult);
}

void CPropertiesDlg::InitSpinControl(int spinId, int buddyEditId, int minValue, int maxValue)
{
    CWnd* pSpin = GetDlgItem(spinId);
    CWnd* pBuddy = GetDlgItem(buddyEditId);
    if (pSpin == nullptr)
    {
        return;
    }

    if (pBuddy != nullptr)
    {
        pSpin->SendMessage(UDM_SETBUDDY, reinterpret_cast<WPARAM>(pBuddy->GetSafeHwnd()), 0);
    }
    pSpin->SendMessage(UDM_SETRANGE32, minValue, maxValue);
}

bool CPropertiesDlg::HandleSpinDelta(NMUPDOWN* pUpDown)
{
    if (pUpDown == nullptr)
    {
        return false;
    }

    int buddyId = 0;
    int pairedId = 0;
    bool isMinControl = false;

    switch (pUpDown->hdr.idFrom)
    {
    case IDC_SPIN_RMIN:
        buddyId = IDC_EDIT_RMIN;
        pairedId = IDC_EDIT_RMAX;
        isMinControl = true;
        break;
    case IDC_SPIN_RMAX:
        buddyId = IDC_EDIT_RMAX;
        pairedId = IDC_EDIT_RMIN;
        break;
    case IDC_SPIN_GMIN:
        buddyId = IDC_EDIT_GMIN;
        pairedId = IDC_EDIT_GMAX;
        isMinControl = true;
        break;
    case IDC_SPIN_GMAX:
        buddyId = IDC_EDIT_GMAX;
        pairedId = IDC_EDIT_GMIN;
        break;
    case IDC_SPIN_BMIN:
        buddyId = IDC_EDIT_BMIN;
        pairedId = IDC_EDIT_BMAX;
        isMinControl = true;
        break;
    case IDC_SPIN_BMAX:
        buddyId = IDC_EDIT_BMAX;
        pairedId = IDC_EDIT_BMIN;
        break;
    default:
        return false;
    }

    const int newValue = ClampInt(pUpDown->iPos + pUpDown->iDelta, kColorMin, kColorMax);

    CScopedInternalUpdate guard(m_internalUpdate);
    SetDlgItemInt(buddyId, newValue, TRUE);

    if (isMinControl)
    {
        const int maxValue = ClampInt(GetDlgItemIntOrDefault(pairedId, kColorMax), kColorMin, kColorMax);
        if (newValue > maxValue)
        {
            SetDlgItemInt(pairedId, newValue, TRUE);
        }
    }
    else
    {
        const int minValue = ClampInt(GetDlgItemIntOrDefault(pairedId, kColorMin), kColorMin, kColorMax);
        if (newValue < minValue)
        {
            SetDlgItemInt(pairedId, newValue, TRUE);
        }
    }

    return true;
}

void CPropertiesDlg::SyncMinMaxPair(int minEditId, int maxEditId)
{
    int minValue = ClampInt(GetDlgItemIntOrDefault(minEditId, kColorMin), kColorMin, kColorMax);
    int maxValue = ClampInt(GetDlgItemIntOrDefault(maxEditId, kColorMax), kColorMin, kColorMax);

    if (minValue > maxValue)
    {
        maxValue = minValue;
    }

    CScopedInternalUpdate guard(m_internalUpdate);
    SetDlgItemInt(minEditId, minValue, TRUE);
    SetDlgItemInt(maxEditId, maxValue, TRUE);
}

void CPropertiesDlg::SyncMaxMinPair(int minEditId, int maxEditId)
{
    int minValue = ClampInt(GetDlgItemIntOrDefault(minEditId, kColorMin), kColorMin, kColorMax);
    int maxValue = ClampInt(GetDlgItemIntOrDefault(maxEditId, kColorMax), kColorMin, kColorMax);

    if (maxValue < minValue)
    {
        minValue = maxValue;
    }

    CScopedInternalUpdate guard(m_internalUpdate);
    SetDlgItemInt(minEditId, minValue, TRUE);
    SetDlgItemInt(maxEditId, maxValue, TRUE);
}

int CPropertiesDlg::GetDlgItemIntOrDefault(int id, int defaultValue) const
{
    BOOL translated = FALSE;
    const UINT value = CDialogEx::GetDlgItemInt(id, &translated, TRUE);
    return translated ? static_cast<int>(value) : defaultValue;
}

int CPropertiesDlg::ClampInt(int value, int minValue, int maxValue)
{
    if (value < minValue)
    {
        return minValue;
    }
    if (value > maxValue)
    {
        return maxValue;
    }
    return value;
}

void CPropertiesDlg::OnColorEditChange(UINT nID)
{
    if (m_internalUpdate)
    {
        return;
    }

    switch (nID)
    {
    case IDC_EDIT_RMIN:
        SyncMinMaxPair(IDC_EDIT_RMIN, IDC_EDIT_RMAX);
        break;
    case IDC_EDIT_RMAX:
        SyncMaxMinPair(IDC_EDIT_RMIN, IDC_EDIT_RMAX);
        break;
    case IDC_EDIT_GMIN:
        SyncMinMaxPair(IDC_EDIT_GMIN, IDC_EDIT_GMAX);
        break;
    case IDC_EDIT_GMAX:
        SyncMaxMinPair(IDC_EDIT_GMIN, IDC_EDIT_GMAX);
        break;
    case IDC_EDIT_BMIN:
        SyncMinMaxPair(IDC_EDIT_BMIN, IDC_EDIT_BMAX);
        break;
    case IDC_EDIT_BMAX:
        SyncMaxMinPair(IDC_EDIT_BMIN, IDC_EDIT_BMAX);
        break;
    default:
        break;
    }
}

void CPropertiesDlg::OnOK()
{
    SyncMinMaxPair(IDC_EDIT_RMIN, IDC_EDIT_RMAX);
    SyncMinMaxPair(IDC_EDIT_GMIN, IDC_EDIT_GMAX);
    SyncMinMaxPair(IDC_EDIT_BMIN, IDC_EDIT_BMAX);

    if (!UpdateData(TRUE))
        return;

    m_rmin = ClampInt(m_rmin, kColorMin, kColorMax);
    m_rmax = ClampInt(m_rmax, m_rmin, kColorMax);
    m_gmin = ClampInt(m_gmin, kColorMin, kColorMax);
    m_gmax = ClampInt(m_gmax, m_gmin, kColorMax);
    m_bmin = ClampInt(m_bmin, kColorMin, kColorMax);
    m_bmax = ClampInt(m_bmax, m_bmin, kColorMax);

    g_props.maxIter = m_maxIter;
    g_props.centerReal = m_centerReal;
    g_props.centerImag = m_centerImag;
    g_props.height = m_height;
    g_props.rmin = m_rmin;
    g_props.rmax = m_rmax;
    g_props.gmin = m_gmin;
    g_props.gmax = m_gmax;
    g_props.bmin = m_bmin;
    g_props.bmax = m_bmax;

    CDialogEx::OnOK();
}
