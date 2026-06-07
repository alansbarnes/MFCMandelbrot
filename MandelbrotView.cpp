#include "pch.h"
#include "MFCMandelbrot.h"
#include "MandelbrotDoc.h"
#include "MandelbrotView.h"
#include "PropertiesDlg.h"
#include "Properties.h"

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
    ON_WM_MOUSEMOVE()
    ON_WM_RBUTTONDOWN()
    ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

CMandelbrotView::CMandelbrotView() noexcept
{
}

void CMandelbrotView::OnSize(UINT nType, int cx, int cy)
{
    CView::OnSize(nType, cx, cy);

    if (cx <= 0 || cy <= 0)
        return;

    CMandelbrotDoc* pDoc = static_cast<CMandelbrotDoc*>(GetDocument());
    if (!pDoc)
        return;

    pDoc->ResizeBitmap(cx, cy);
    pDoc->RenderMandelbrot();

    Invalidate();
}

void CMandelbrotView::OnProperties()
{
    CPropertiesDlg dlg(this);

    // Initialize dialog from current doc state
    CMandelbrotDoc* pDoc = static_cast<CMandelbrotDoc*>(GetDocument());
    if (!pDoc)
        return;

    dlg.m_maxIter = pDoc->m_maxIter;
    dlg.m_centerReal = pDoc->m_centerX;
    dlg.m_centerImag = pDoc->m_centerY;
    dlg.m_height = pDoc->m_scale;

    if (dlg.DoModal() == IDOK)
    {
        // Apply dialog values to the document
        pDoc->m_maxIter = g_props.maxIter;
        pDoc->m_centerX = g_props.centerReal;
        pDoc->m_centerY = g_props.centerImag;
        pDoc->m_scale = g_props.height;

        // Re-render and refresh
        pDoc->RenderMandelbrot();
        Invalidate();
    }
}

void CMandelbrotView::OnViewReset()
{
    g_props = Properties{};

    CMandelbrotDoc* pDoc = static_cast<CMandelbrotDoc*>(GetDocument());
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
    CMandelbrotDoc* pDoc = static_cast<CMandelbrotDoc*>(GetDocument());
    if (!pDoc)
        return;

    pDoc->m_maxIter += 50;
    pDoc->RenderMandelbrot();
    Invalidate();
}

void CMandelbrotView::OnIterDec()
{
    CMandelbrotDoc* pDoc = static_cast<CMandelbrotDoc*>(GetDocument());
    if (!pDoc)
        return;

    if (pDoc->m_maxIter > 50)
        pDoc->m_maxIter -= 50;

    pDoc->RenderMandelbrot();
    Invalidate();
}

void CMandelbrotView::OnInitialUpdate()
{
    CView::OnInitialUpdate();

    CMandelbrotDoc* pDoc = static_cast<CMandelbrotDoc*>(GetDocument());
    if (!pDoc)
        return;

    CRect rc;
    GetClientRect(&rc);

    pDoc->ResizeBitmapForDisplay(m_hWnd, rc.Width(), rc.Height());
    pDoc->RenderMandelbrot();

    Invalidate();
    UpdateWindow();
}

void CMandelbrotView::OnDraw(CDC* pDC)
{
    CMandelbrotDoc* pDoc = static_cast<CMandelbrotDoc*>(GetDocument());
    if (!pDoc || !pDoc->HasBitmap())
        return;

    CBitmap* pBmp = pDoc->GetBitmap();
    if (!pBmp || !pBmp->GetSafeHandle())
        return;

    BITMAP bm = {};
    pBmp->GetBitmap(&bm);
    if (bm.bmWidth <= 0 || bm.bmHeight <= 0)
        return;

    CDC memDC;
    if (!memDC.CreateCompatibleDC(pDC))
        return;

    CBitmap* pOld = memDC.SelectObject(pBmp);
    if (!pOld)
        return;

    pDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, SRCCOPY);

    memDC.SelectObject(pOld);

    if (m_bDragging)
    {
        CPen pen(PS_SOLID, 1, RGB(255, 255, 255));
        CPen* oldPen = pDC->SelectObject(&pen);
        CBrush* oldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

        pDC->Rectangle(m_rcCapture);

        pDC->SelectObject(oldPen);
        pDC->SelectObject(oldBrush);
    }
}

void CMandelbrotView::OnLButtonDown(UINT, CPoint pt)
{
    if (m_bDragging)
        return; // ignore if dragging

    // Only zoom if click is inside the rectangle
    if (!m_rcCapture.PtInRect(pt))
        return;

    CMandelbrotDoc* pDoc = static_cast<CMandelbrotDoc*>(GetDocument());
    if (!pDoc)
        return;

    // Convert pixel rectangle → complex plane rectangle
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

    // New center
    pDoc->m_centerX = (x1 + x2) / 2.0;
    pDoc->m_centerY = (y1 + y2) / 2.0;

    // New scale (height of selected region)
    pDoc->m_scale = fabs(y2 - y1);

    // Clear rectangle
    m_rcCapture.SetRectEmpty();

    // Re-render
    pDoc->RenderMandelbrot();
    Invalidate();
}

void CMandelbrotView::OnRButtonDown(UINT, CPoint pt)
{
    m_bDragging = true;
    m_ptAnchor = pt;

    // Compute aspect ratio of the view
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

    int dx = pt.x - m_ptAnchor.x;
    int dy = pt.y - m_ptAnchor.y;

    // Enforce aspect ratio
    if (abs(dx) > abs(dy))
        dy = int(abs(dx) / m_aspect) * (dy < 0 ? -1 : 1);
    else
        dx = int(abs(dy) * m_aspect) * (dx < 0 ? -1 : 1);

    m_rcCapture.SetRect(m_ptAnchor.x, m_ptAnchor.y,
        m_ptAnchor.x + dx, m_ptAnchor.y + dy);
    m_rcCapture.NormalizeRect();

    Invalidate(FALSE);
}

void CMandelbrotView::OnRButtonUp(UINT, CPoint)
{
    if (!m_bDragging)
        return;

    m_bDragging = false;
    ReleaseCapture();
}

void CMandelbrotView::UpdateCaptureRect(CPoint pt)
{
    m_rcCapture.SetRect(m_ptAnchor.x, m_ptAnchor.y, pt.x, pt.y);
    m_rcCapture.NormalizeRect();
}
