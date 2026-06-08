#include "pch.h"
#include "MFCMandelbrot.h"
#include "MandelbrotDoc.h"
#include "MandelbrotView.h"
#include "PropertiesDlg.h"
#include "Properties.h"

#include <string>
#include <format>
#include <limits>
#include <algorithm>
#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern Properties g_props;

IMPLEMENT_DYNCREATE(CMandelbrotView, CView)

BEGIN_MESSAGE_MAP(CMandelbrotView, CView)
    ON_COMMAND(ID_PROPERTIES, &CMandelbrotView::OnProperties)
    ON_COMMAND(ID_VIEW_RESET, &CMandelbrotView::OnViewReset)
    ON_COMMAND(ID_ITER_INC, &CMandelbrotView::OnIterInc)
    ON_COMMAND(ID_ITER_DEC, &CMandelbrotView::OnIterDec)

    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_RBUTTONDOWN()
    ON_WM_RBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSEWHEEL()
    ON_WM_SIZE()
END_MESSAGE_MAP()

CMandelbrotView::CMandelbrotView() noexcept
{
}

CMandelbrotDoc* CMandelbrotView::GetDocument() const
{
    return reinterpret_cast<CMandelbrotDoc*>(m_pDocument);
}

void CMandelbrotView::OnInitialUpdate()
{
    CView::OnInitialUpdate();

    CMandelbrotDoc* pDoc = GetDocument();
    if (!pDoc)
        return;

    CRect rc;
    GetClientRect(&rc);

    pDoc->ResizeBitmap(rc.Width(), rc.Height());
    pDoc->RenderMandelbrot();

    Invalidate();
    UpdateWindow();
}

void CMandelbrotView::OnDraw(CDC* pDC)
{
    CMandelbrotDoc* pDoc = GetDocument();
    if (!pDoc || !pDoc->HasBitmap())
        return;

    CBitmap* pBmp = pDoc->GetBitmap();
    if (!pBmp || !pBmp->GetSafeHandle())
        return;

    BITMAP bm = {};
    pBmp->GetBitmap(&bm);

    CDC memDC;
    memDC.CreateCompatibleDC(pDC);
    CBitmap* pOld = memDC.SelectObject(pBmp);

    pDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, SRCCOPY);

    memDC.SelectObject(pOld);

    // Draw selection rectangle
    if (m_bDragging && !m_bPanning)
    {
        CPen pen(PS_SOLID, 1, RGB(255, 255, 255));
        CPen* oldPen = pDC->SelectObject(&pen);
        CBrush* oldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

        pDC->Rectangle(m_rcCapture);

        pDC->SelectObject(oldPen);
        pDC->SelectObject(oldBrush);
    }

    // Status overlay
    constexpr int D = std::numeric_limits<double>::max_digits10;
    std::string info = std::format("Center: {:.{}g} + {:.{}g}i  Height: {:.{}g}i  Iter: {}",
        pDoc->m_centerX, D,
        pDoc->m_centerY, D,
        pDoc->m_scale, D,
        pDoc->m_maxIter);

    SetTextColor(*pDC, RGB(255, 255, 255));
    SetBkMode(*pDC, TRANSPARENT);
    RECT r = { 8, 32, bm.bmWidth - 8, 64 };
    DrawTextA(pDC->GetSafeHdc(), info.c_str(), static_cast<int>(info.size()), &r,
        DT_LEFT | DT_SINGLELINE | DT_NOPREFIX);
}

void CMandelbrotView::OnSize(UINT nType, int cx, int cy)
{
    CView::OnSize(nType, cx, cy);

    if (cx <= 0 || cy <= 0)
        return;

    CMandelbrotDoc* pDoc = GetDocument();
    if (!pDoc)
        return;

    pDoc->ResizeBitmap(cx, cy);
    pDoc->RenderMandelbrot();
    Invalidate();
}

void CMandelbrotView::OnProperties()
{
    CPropertiesDlg dlg(this);

    CMandelbrotDoc* pDoc = GetDocument();
    if (!pDoc)
        return;

    dlg.m_maxIter = pDoc->m_maxIter;
    dlg.m_centerReal = pDoc->m_centerX;
    dlg.m_centerImag = pDoc->m_centerY;
    dlg.m_height = pDoc->m_scale;

    if (dlg.DoModal() == IDOK)
    {
        pDoc->m_maxIter = g_props.maxIter;
        pDoc->m_centerX = g_props.centerReal;
        pDoc->m_centerY = g_props.centerImag;
        pDoc->m_scale = g_props.height;

        pDoc->RenderMandelbrot();
        Invalidate();
    }
}

void CMandelbrotView::OnViewReset()
{
    g_props = Properties{};

    CMandelbrotDoc* pDoc = GetDocument();
    if (!pDoc)
        return;

    pDoc->m_centerX = -0.5;
    pDoc->m_centerY = 0.0;
    pDoc->m_scale = 4.0;
    pDoc->m_maxIter = 100;

    pDoc->RenderMandelbrot();
    Invalidate();
}

void CMandelbrotView::OnIterInc()
{
    CMandelbrotDoc* pDoc = GetDocument();
    if (!pDoc)
        return;

    pDoc->m_maxIter += 50;
    pDoc->RenderMandelbrot();
    Invalidate();
}

void CMandelbrotView::OnIterDec()
{
    CMandelbrotDoc* pDoc = GetDocument();
    if (!pDoc)
        return;

    if (pDoc->m_maxIter > 50)
        pDoc->m_maxIter -= 50;

    pDoc->RenderMandelbrot();
    Invalidate();
}

void CMandelbrotView::OnLButtonDown(UINT, CPoint pt)
{
    // If clicking inside rectangle → zoom
    if (m_rcCapture.PtInRect(pt) &&
        m_rcCapture.Width() > 4 &&
        m_rcCapture.Height() > 4)
    {
        CMandelbrotDoc* pDoc = GetDocument();
        if (!pDoc)
            return;

        double pixelAspect = double(pDoc->m_width) / pDoc->m_height;

        double planeH = pDoc->m_scale;
        double planeW = planeH * pixelAspect;

        double left = pDoc->m_centerX - planeW / 2.0;
        double top = pDoc->m_centerY + planeH / 2.0;

        auto mapX = [&](int px)
            {
                return left + (double(px) / (pDoc->m_width - 1)) * planeW;
            };

        auto mapY = [&](int py)
            {
                return top - (double(py) / (pDoc->m_height - 1)) * planeH;
            };

        double x1 = mapX(m_rcCapture.left);
        double x2 = mapX(m_rcCapture.right);
        double y1 = mapY(m_rcCapture.top);
        double y2 = mapY(m_rcCapture.bottom);

        pDoc->m_centerX = (x1 + x2) / 2.0;
        pDoc->m_centerY = (y1 + y2) / 2.0;
        pDoc->m_scale = fabs(y2 - y1);

        m_rcCapture.SetRectEmpty();

        pDoc->RenderMandelbrot();
        Invalidate();
        return;
    }

    // Otherwise begin panning
    m_bDragging = true;
    m_bPanning = true;
    m_rcCapture.SetRectEmpty();

    CMandelbrotDoc* pDoc = GetDocument();
    if (!pDoc)
    {
        m_bDragging = false;
        m_bPanning = false;
        return;
    }

    m_ptAnchor = pt;
    m_aspect = double(pDoc->m_width) / pDoc->m_height;

    SetCapture();
}

void CMandelbrotView::OnLButtonUp(UINT, CPoint)
{
    if (!m_bDragging || !m_bPanning)
        return;

    m_bDragging = false;
    m_bPanning = false;

    if (::GetCapture() == m_hWnd)
        ReleaseCapture();
}

void CMandelbrotView::OnRButtonDown(UINT, CPoint pt)
{
    m_bDragging = true;
    m_bPanning = false;
    m_ptAnchor = pt;

    CRect rc;
    GetClientRect(&rc);
    m_aspect = double(rc.Width()) / rc.Height();

    m_rcCapture.SetRect(pt.x, pt.y, pt.x, pt.y);
    SetCapture();
}

void CMandelbrotView::OnMouseMove(UINT, CPoint pt)
{
    if (!m_bDragging)
        return;

    if (m_bPanning)
    {
        CMandelbrotDoc* pDoc = GetDocument();
        if (!pDoc || pDoc->m_width <= 1 || pDoc->m_height <= 1)
            return;

        double planeH = pDoc->m_scale;
        double planeW = planeH * (double(pDoc->m_width) / pDoc->m_height);

        pDoc->m_centerX -= (double(pt.x - m_ptAnchor.x) / (pDoc->m_width - 1)) * planeW;
        pDoc->m_centerY += (double(pt.y - m_ptAnchor.y) / (pDoc->m_height - 1)) * planeH;
        m_ptAnchor = pt;

        pDoc->RenderMandelbrot();
        Invalidate(FALSE);
        return;
    }

    int dx = pt.x - m_ptAnchor.x;
    int dy = pt.y - m_ptAnchor.y;

    // Aspect ratio lock
    if (abs(dx) > abs(dy))
        dy = int(abs(dx) / m_aspect) * (dy < 0 ? -1 : 1);
    else
        dx = int(abs(dy) * m_aspect) * (dx < 0 ? -1 : 1);

    // Constrain to client area
    CRect client;
    GetClientRect(&client);

    int x2 = std::clamp(m_ptAnchor.x + dx, client.left, client.right);
    int y2 = std::clamp(m_ptAnchor.y + dy, client.top, client.bottom);

    m_rcCapture.SetRect(m_ptAnchor.x, m_ptAnchor.y, x2, y2);
    m_rcCapture.NormalizeRect();

    Invalidate(FALSE);
}

void CMandelbrotView::OnRButtonUp(UINT, CPoint)
{
    if (!m_bDragging || m_bPanning)
        return;

    m_bDragging = false;
    if (::GetCapture() == m_hWnd)
        ReleaseCapture();
}

BOOL CMandelbrotView::OnMouseWheel(UINT, short zDelta, CPoint pt)
{
    CMandelbrotDoc* pDoc = GetDocument();
    if (!pDoc || pDoc->m_width <= 1 || pDoc->m_height <= 1)
        return FALSE;

    ScreenToClient(&pt);

    double pixelAspect = double(pDoc->m_width) / pDoc->m_height;

    double planeH = pDoc->m_scale;
    double planeW = planeH * pixelAspect;

    double left = pDoc->m_centerX - planeW / 2.0;
    double top = pDoc->m_centerY + planeH / 2.0;

    double cx = left + (double(pt.x) / (pDoc->m_width - 1)) * planeW;
    double cy = top - (double(pt.y) / (pDoc->m_height - 1)) * planeH;

    double factor = (zDelta > 0) ? 0.8 : 1.25;
    pDoc->m_scale *= factor;

    planeH = pDoc->m_scale;
    planeW = planeH * pixelAspect;

    double fx = double(pt.x) / (pDoc->m_width - 1);
    double fy = double(pt.y) / (pDoc->m_height - 1);

    pDoc->m_centerX = cx + (0.5 - fx) * planeW;
    pDoc->m_centerY = cy + (fy - 0.5) * planeH;

    pDoc->RenderMandelbrot();
    Invalidate();

    return TRUE;
}
