#pragma once

class CMandelbrotView : public CView
{
protected:
    CMandelbrotView() noexcept;
    DECLARE_DYNCREATE(CMandelbrotView)

public:
    CMandelbrotDoc* GetDocument() const;

protected:
    CPoint m_panStart;
    double m_panStartCenterX;
    double m_panStartCenterY;
    CPoint m_selectStart;
    CPoint m_selectEnd;
    CRect  m_constrainedRect;
    bool   m_panning;
    bool   m_selecting;
    bool   m_showConstrainedRect;

public:
    virtual void OnDraw(CDC* pDC) override;
    virtual void OnInitialUpdate() override;

protected:
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint pt);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint pt);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint pt);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint pt);
    afx_msg void OnMouseMove(UINT nFlags, CPoint pt);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg void OnSize(UINT nType, int cx, int cy);

    DECLARE_MESSAGE_MAP()

#ifdef _DEBUG
    virtual void AssertValid() const override;
    virtual void Dump(CDumpContext& dc) const override;
#endif
};
