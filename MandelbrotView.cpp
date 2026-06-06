#include "pch.h"
#include "framework.h"
#include "MFCMandelbrot.h"

#include "MandelbrotDoc.h"
#include "MandelbrotView.h"

#include <format>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CMandelbrotView, CView)

BEGIN_MESSAGE_MAP(CMandelbrotView, CView)
    ON_WM_ERASEBKGND()
    ON_WM_RBUTTONDOWN()
    ON_WM_RBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSEWHEEL()
    ON_WM_SIZE()
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

    // Get the work area (excluding taskbar)
    RECT workArea;
    ::SystemParametersInfoA(SPI_GETWORKAREA, 0, &workArea, 0);
    int screenWidth = workArea.right - workArea.left;
    int screenHeight = workArea.bottom - workArea.top;

    // Use 90% of available screen space as target client area
    const int targetClientWidth = static_cast<int>(screenWidth * 0.9);
    const int targetClientHeight = static_cast<int>(screenHeight * 0.9);

    // Get current frame window
    CFrameWnd* pFrame = GetParentFrame();
    if (pFrame)
    {
        // Get frame decorations BEFORE resizing
        CRect rcFrame;
        pFrame->GetWindowRect(&rcFrame);
        CRect rcClient;
        GetClientRect(&rcClient);
        int decorationWidth = rcFrame.Width() - rcClient.Width();
        int decorationHeight = rcFrame.Height() - rcClient.Height();

        // Calculate new frame size to achieve target client area
        int newFrameWidth = targetClientWidth + decorationWidth;
        int newFrameHeight = targetClientHeight + decorationHeight;

        // Clamp to screen area
        if (newFrameWidth > screenWidth)
            newFrameWidth = screenWidth;
        if (newFrameHeight > screenHeight)
            newFrameHeight = screenHeight;

        // FIRST: Resize the frame to the target size
        rcFrame.right = rcFrame.left + newFrameWidth;
        rcFrame.bottom = rcFrame.top + newFrameHeight;
        pFrame->MoveWindow(&rcFrame, TRUE);

        // SECOND: Now get the actual client area after resize
        GetClientRect(&rcClient);
        int actualClientWidth = rcClient.Width();
        int actualClientHeight = rcClient.Height();

        // THIRD: Resize bitmap to EXACTLY match the client area
        // (not constrained to 4-inch height for initial sizing)
        pDoc->ResizeBitmap(actualClientWidth, actualClientHeight);
        pDoc->RenderMandelbrot();
    }
    else
    {
        // Fallback if no frame available
        pDoc->ResizeBitmap(targetClientWidth, targetClientHeight);
        pDoc->RenderMandelbrot();
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

    constexpr int D = std::numeric_limits<double>::max_digits10;
    // m_scale IS the imaginary plane height, no multiplication needed
    std::string info =
        "Center: " + std::format("{:.{}g}", pDoc->m_centerX, D) +
        " + " + std::format("{:.{}g}", pDoc->m_centerY, D) + "i" +
        "  Height: " + std::format("{:.{}g}", pDoc->m_scale, D) + "i" +
        "  Iter: " + std::to_string(pDoc->m_maxIter);

    SetTextColor(*pDC, RGB(255, 255, 255));
    SetBkMode(*pDC, TRANSPARENT);
    RECT r = { 8, 8, pDoc->m_width - 8, 40 };
    DrawTextA(*pDC, info.c_str(), static_cast<int>(info.size()), &r, DT_LEFT | DT_SINGLELINE | DT_NOPREFIX);
}

BOOL CMandelbrotView::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}

void CMandelbrotView::OnRButtonDown(UINT nFlags, CPoint pt)
{
    m_dragging = true;
    m_dragStart = m_dragEnd = pt;
    SetCapture();
}

void CMandelbrotView::OnRButtonUp(UINT nFlags, CPoint pt)
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

        // m_scale is the imaginary plane height
        const double planeH = pDoc->m_scale;
        const double pixelAspect = double(pDoc->m_width) / pDoc->m_height;
        const double planeW = planeH * pixelAspect;

        double left = pDoc->m_centerX - planeW / 2.0;
        double top = pDoc->m_centerY + planeH / 2.0;

        double x1 = left + (double(rc.left) / pDoc->m_width) * planeW;
        double y1 = top - (double(rc.top) / pDoc->m_height) * planeH;
        double x2 = left + (double(rc.right) / pDoc->m_width) * planeW;
        double y2 = top - (double(rc.bottom) / pDoc->m_height) * planeH;

        pDoc->m_centerX = (x1 + x2) / 2.0;
        pDoc->m_centerY = (y1 + y2) / 2.0;
        pDoc->m_scale = max(fabs(x2 - x1), fabs(y2 - y1)) / pixelAspect;  // Preserve height semantics

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

    CMandelbrotDoc* pDoc = GetDocument();
    if (!pDoc)
        return;

    // Recompute bitmap dimensions based on new client area
    pDoc->ResizeBitmap(cx, cy);
    pDoc->RenderMandelbrot();
    Invalidate();
}

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
