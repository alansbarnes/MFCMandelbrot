#pragma once

#include "afxwin.h"
#include "Properties.h"

class CPropertiesDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CPropertiesDlg)

public:
    CPropertiesDlg(CWnd* pParent = nullptr);
    virtual ~CPropertiesDlg();

    enum { IDD = IDD_PROPERTIES };

protected:
    virtual void DoDataExchange(CDataExchange* pDX) override;
    virtual void OnOK() override;

    DECLARE_MESSAGE_MAP()

public:
    int     m_maxIter;
    double  m_centerReal;
    double  m_centerImag;
    double  m_height;

private:
    int     m_rmin, m_rmax;
    int     m_gmin, m_gmax;
    int     m_bmin, m_bmax;
};
