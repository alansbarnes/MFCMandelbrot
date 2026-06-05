#include "pch.h"
#include "framework.h"
#include "MFCMandelbrot.h"

#include "MandelbrotDoc.h"
#include "MandelbrotView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CMandelbrotView, CView)

BEGIN_MESSAGE_MAP(CMandelbrotView, CView)
    ON_WM_ERASEBKGND()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSEWHEEL()
    ON_WM_SIZE()
#ifdef WM_DPICHANGED
    ON_WM_DPICHANGED()
#endif
END_MESSAGE_MAP()

CMandelbrotView::CMandelbrotView() noexcept
    : m_dragging(false)
{
}

void CMandelbrotView::OnInitialUpdate()
{
    CView::OnInitialUpdate();

    CMandelbrotDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    CRect rcClient;
    GetClientRect(&rcClient);
    pDoc->ResizeBitmapForDisplay(GetSafeHwnd(), rcClient.Width(), rcClient.Height());
    pDoc->RenderMandelbrot();

    CSize bmSize(pDoc->m_width, pDoc->m_height);

    // --- Clamp client area to bitmap dimensions ---
    CFrameWnd* pFrame = GetParentFrame();
    if (pFrame)
    {
        // Current client area
        // How much we need to grow/shrink the frame
        int dx = bmSize.cx - rcClient.Width();
        int dy = bmSize.cy - rcClient.Height();

        if (dx != 0 || dy != 0)
        {
            CRect rcWindow;
            pFrame->GetWindowRect(&rcWindow);

            rcWindow.right += dx;
            rcWindow.bottom += dy;

            pFrame->MoveWindow(&rcWindow, TRUE);
        }
    }

    Invalidate();
}

void CMandelbrotView::OnDraw(CDC* pDC)
{
    CMandelbrotDoc* pDoc = GetDocument();
    if (!pDoc || !pDoc->m_bitmap.GetSafeHandle())
        return;

    CDC memDC;
    memDC.CreateCompatibleDC(pDC);
    CBitmap* pOld = memDC.SelectObject(&pDoc->m_bitmap);

    pDC->BitBlt(0, 0, pDoc->m_width, pDoc->m_height, &memDC, 0, 0, SRCCOPY);

    memDC.SelectObject(pOld);

    if (m_dragging)
    {
        CRect rc(m_dragStart, m_dragEnd);
        rc.NormalizeRect();
        pDC->DrawFocusRect(rc);
    }
}

BOOL CMandelbrotView::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}

void CMandelbrotView::OnLButtonDown(UINT nFlags, CPoint pt)
{
    m_dragging = true;
    m_dragStart = m_dragEnd = pt;
    SetCapture();
}

void CMandelbrotView::OnLButtonUp(UINT nFlags, CPoint pt)
{
    if (!m_dragging)
        return;

    m_dragging = false;
    ReleaseCapture();

    CRect rc(m_dragStart, pt);
    rc.NormalizeRect();

    if (rc.Width() > 4 && rc.Height() > 4)
    {
        CMandelbrotDoc* pDoc = GetDocument();

        double aspect = double(pDoc->m_height) / pDoc->m_width;

        double planeW = pDoc->m_scale;
        double planeH = pDoc->m_scale * aspect;

        double left = pDoc->m_centerX - planeW / 2.0;
        double top = pDoc->m_centerY + planeH / 2.0;

        double x1 = left + (double(rc.left) / pDoc->m_width) * planeW;
        double y1 = top - (double(rc.top) / pDoc->m_height) * planeH;
        double x2 = left + (double(rc.right) / pDoc->m_width) * planeW;
        double y2 = top - (double(rc.bottom) / pDoc->m_height) * planeH;

        pDoc->m_centerX = (x1 + x2) / 2.0;
        pDoc->m_centerY = (y1 + y2) / 2.0;
        pDoc->m_scale = max(fabs(x2 - x1), fabs(y2 - y1));

        pDoc->RenderMandelbrot();
        Invalidate();
    }

    Invalidate();
}

void CMandelbrotView::OnMouseMove(UINT nFlags, CPoint pt)
{
    if (m_dragging)
    {
        m_dragEnd = pt;
        Invalidate(FALSE);
    }
}

BOOL CMandelbrotView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    CMandelbrotDoc* pDoc = GetDocument();

    double factor = (zDelta > 0) ? 0.8 : 1.25;
    pDoc->m_scale *= factor;

    pDoc->RenderMandelbrot();
    Invalidate();

    return TRUE;
}

void CMandelbrotView::OnSize(UINT nType, int cx, int cy)
{
    CView::OnSize(nType, cx, cy);

    if (nType == SIZE_MINIMIZED || cx <= 0 || cy <= 0)
        return;

    CMandelbrotDoc* pDoc = GetDocument();
    if (!pDoc)
        return;

    pDoc->ResizeBitmapForDisplay(GetSafeHwnd(), cx, cy);
    pDoc->RenderMandelbrot();
    Invalidate();
}

#ifdef WM_DPICHANGED
void CMandelbrotView::OnDpiChanged(UINT nDpiX, UINT nDpiY, LPRECT pRect)
{
    CView::OnDpiChanged(nDpiX, nDpiY, pRect);

    CMandelbrotDoc* pDoc = GetDocument();
    if (!pDoc)
        return;

    CRect rcClient;
    GetClientRect(&rcClient);

    pDoc->ResizeBitmapForDisplay(GetSafeHwnd(), rcClient.Width(), rcClient.Height());
    pDoc->RenderMandelbrot();
    Invalidate();
}
#endif

#ifdef _DEBUG
void CMandelbrotView::AssertValid() const
{
    CView::AssertValid();
}

void CMandelbrotView::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}

CMandelbrotDoc* CMandelbrotView::GetDocument() const
{
    return reinterpret_cast<CMandelbrotDoc*>(m_pDocument);
}
#endif
