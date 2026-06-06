#include "pch.h"
#include "framework.h"
#include "MFCMandelbrot.h"

#include "MandelbrotDoc.h"
#include "MandelbrotView.h"

#include <algorithm>
#include <cmath>
#include <format>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CMandelbrotView, CView)

BEGIN_MESSAGE_MAP(CMandelbrotView, CView)
    ON_WM_ERASEBKGND()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_RBUTTONDOWN()
    ON_WM_RBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSEWHEEL()
    ON_WM_SIZE()
END_MESSAGE_MAP()

CMandelbrotView::CMandelbrotView() noexcept
    : m_constrainedRect(0, 0, 0, 0)
    , m_panStartCenterX(0.0)
    , m_panStartCenterY(0.0)
    , m_panning(false)
    , m_selecting(false)
    , m_showConstrainedRect(false)
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

    if (m_showConstrainedRect)
    {
        pDC->DrawFocusRect(m_constrainedRect);
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

void CMandelbrotView::OnLButtonDown(UINT nFlags, CPoint pt)
{
    UNREFERENCED_PARAMETER(nFlags);

    if (m_showConstrainedRect &&
        m_constrainedRect.Width() > 4 &&
        m_constrainedRect.Height() > 4 &&
        m_constrainedRect.PtInRect(pt))
    {
        CMandelbrotDoc* pDoc = GetDocument();

        const double planeH = pDoc->m_scale;
        const double pixelAspect = double(pDoc->m_width) / pDoc->m_height;
        const double planeW = planeH * pixelAspect;
        const CRect constrainedRect = m_constrainedRect;

        const double left = pDoc->m_centerX - planeW / 2.0;
        const double top = pDoc->m_centerY + planeH / 2.0;

        const double x1 = left + (double(constrainedRect.left) / pDoc->m_width) * planeW;
        const double y1 = top - (double(constrainedRect.top) / pDoc->m_height) * planeH;
        const double x2 = left + (double(constrainedRect.right) / pDoc->m_width) * planeW;
        const double y2 = top - (double(constrainedRect.bottom) / pDoc->m_height) * planeH;

        pDoc->m_centerX = (x1 + x2) / 2.0;
        pDoc->m_centerY = (y1 + y2) / 2.0;
        pDoc->m_scale = std::max(std::fabs(x2 - x1), std::fabs(y2 - y1)) / pixelAspect;
        pDoc->RenderMandelbrot();

        m_showConstrainedRect = false;
        Invalidate(FALSE);
        return;
    }

    m_panning = true;
    m_panStart = pt;

    CMandelbrotDoc* pDoc = GetDocument();
    m_panStartCenterX = pDoc->m_centerX;
    m_panStartCenterY = pDoc->m_centerY;

    SetCapture();
}

void CMandelbrotView::OnLButtonUp(UINT nFlags, CPoint pt)
{
    UNREFERENCED_PARAMETER(nFlags);
    UNREFERENCED_PARAMETER(pt);

    if (!m_panning)
        return;

    m_panning = false;
    ReleaseCapture();
}

void CMandelbrotView::OnRButtonDown(UINT nFlags, CPoint pt)
{
    UNREFERENCED_PARAMETER(nFlags);

    m_selecting = true;
    m_selectStart = m_selectEnd = pt;
    m_constrainedRect.SetRect(pt.x, pt.y, pt.x, pt.y);
    m_showConstrainedRect = true;

    SetCapture();
}

void CMandelbrotView::OnRButtonUp(UINT nFlags, CPoint pt)
{
    UNREFERENCED_PARAMETER(nFlags);
    UNREFERENCED_PARAMETER(pt);

    if (!m_selecting)
        return;

    m_selecting = false;
    if (m_constrainedRect.Width() <= 4 || m_constrainedRect.Height() <= 4)
    {
        m_showConstrainedRect = false;
        m_constrainedRect.SetRect(0, 0, 0, 0);
    }

    ReleaseCapture();
    Invalidate(FALSE);
}

void CMandelbrotView::OnMouseMove(UINT nFlags, CPoint pt)
{
    UNREFERENCED_PARAMETER(nFlags);

    if (m_selecting)
    {
        m_selectEnd = pt;
        CRect clientRect;
        GetClientRect(&clientRect);
        const CSize clientSize = clientRect.Size();
        const double clientAspect = (clientSize.cy > 0) ?
            (static_cast<double>(clientSize.cx) / static_cast<double>(clientSize.cy)) : 1.0;

        const int deltaX = pt.x - m_selectStart.x;
        const int deltaY = pt.y - m_selectStart.y;

        double width = static_cast<double>(std::abs(deltaX));
        double height = static_cast<double>(std::abs(deltaY));

        if (width > 0.0 || height > 0.0)
        {
            if (width == 0.0)
            {
                width = height * clientAspect;
            }
            else if (height == 0.0)
            {
                height = width / clientAspect;
            }
            else if ((width / height) > clientAspect)
            {
                height = width / clientAspect;
            }
            else
            {
                width = height * clientAspect;
            }

            const int signX = (deltaX < 0) ? -1 : 1;
            const int signY = (deltaY < 0) ? -1 : 1;

            const double maxWidth = static_cast<double>((signX > 0) ? (clientRect.right - m_selectStart.x) : (m_selectStart.x - clientRect.left));
            const double maxHeight = static_cast<double>((signY > 0) ? (clientRect.bottom - m_selectStart.y) : (m_selectStart.y - clientRect.top));

            const double widthScale = (width > 0.0) ? (maxWidth / width) : 1.0;
            const double heightScale = (height > 0.0) ? (maxHeight / height) : 1.0;
            const double scale = std::min(1.0, std::min(widthScale, heightScale));

            width = std::max(0.0, width * scale);
            height = std::max(0.0, height * scale);

            const int constrainedX = m_selectStart.x + signX * static_cast<int>(std::lround(width));
            const int constrainedY = m_selectStart.y + signY * static_cast<int>(std::lround(height));

            m_constrainedRect.SetRect(m_selectStart.x, m_selectStart.y, constrainedX, constrainedY);
            m_constrainedRect.NormalizeRect();
        }
        else
        {
            m_constrainedRect.SetRect(m_selectStart.x, m_selectStart.y, m_selectStart.x, m_selectStart.y);
        }

        Invalidate(FALSE);
    }
    else if (m_panning)
    {
        CMandelbrotDoc* pDoc = GetDocument();
        if (pDoc->m_width <= 0 || pDoc->m_height <= 0)
            return;

        const int deltaX = pt.x - m_panStart.x;
        const int deltaY = pt.y - m_panStart.y;
        const double planeH = pDoc->m_scale;
        const double planeW = planeH * static_cast<double>(pDoc->m_width) / static_cast<double>(pDoc->m_height);

        pDoc->m_centerX = m_panStartCenterX - (static_cast<double>(deltaX) / pDoc->m_width) * planeW;
        pDoc->m_centerY = m_panStartCenterY + (static_cast<double>(deltaY) / pDoc->m_height) * planeH;
        pDoc->RenderMandelbrot();
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
