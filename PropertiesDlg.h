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
    virtual BOOL OnInitDialog() override;
    virtual void DoDataExchange(CDataExchange* pDX) override;
    virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) override;
    virtual void OnOK() override;

    void InitSpinControl(int spinId, int buddyEditId, int minValue, int maxValue);
    bool HandleSpinDelta(NMUPDOWN* pUpDown);
    void SyncMinMaxPair(int minEditId, int maxEditId);
    void SyncMaxMinPair(int minEditId, int maxEditId);
    int GetDlgItemIntOrDefault(int id, int defaultValue) const;
    static int ClampInt(int value, int minValue, int maxValue);

    afx_msg void OnColorEditChange(UINT nID);

    DECLARE_MESSAGE_MAP()

public:
    int     m_maxIter;
    double  m_centerReal;
    double  m_centerImag;
    double  m_height;

private:
    bool    m_internalUpdate = false;
    int     m_rmin, m_rmax;
    int     m_gmin, m_gmax;
    int     m_bmin, m_bmax;
};
