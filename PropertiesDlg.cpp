#include "pch.h"
#include "MFCMandelbrot.h"
#include "PropertiesDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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
END_MESSAGE_MAP()

void CPropertiesDlg::OnOK()
{
    if (!UpdateData(TRUE))
        return;

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
