#include "pch.h"
#include "framework.h"
#include "MFCMandelbrot.h"
#include "MandelbrotDoc.h"

#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CMandelbrotDoc, CDocument)

BEGIN_MESSAGE_MAP(CMandelbrotDoc, CDocument)
END_MESSAGE_MAP()

constexpr double initialHeight = 4.0;

CMandelbrotDoc::CMandelbrotDoc() noexcept
    : m_bitmap()
    , m_pBits(nullptr)
    , m_width(1)
    , m_height(1)
    , m_centerX(-0.75)
    , m_centerY(0.0)
    , m_scale(initialHeight)
    , m_maxIter(50)
{
}

BOOL CMandelbrotDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    // Initial bitmap will be sized by view in OnInitialUpdate
    return TRUE;
}

void CMandelbrotDoc::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        ar << m_centerX << m_centerY << m_scale << m_maxIter;
    }
    else
    {
        ar >> m_centerX >> m_centerY >> m_scale >> m_maxIter;
        // Bitmap will be resized by view based on display geometry
        RenderMandelbrot();
    }
}

void CMandelbrotDoc::ResizeBitmap(int width, int height)
{
    if (width <= 0 || height <= 0)
        return;

    if (m_bitmap.GetSafeHandle())
    {
        m_bitmap.DeleteObject();
        m_pBits = nullptr;
    }
    m_hasBitmap = false;

    m_width = width;
    m_height = height;

    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = m_width;
    bmi.bmiHeader.biHeight = -m_height;   // top‑down
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;          // 32‑bit
    bmi.bmiHeader.biCompression = BI_RGB;

    HDC hdc = ::GetDC(nullptr);
    void* pBits = nullptr;
    HBITMAP hBmp = ::CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &pBits, nullptr, 0);
    ::ReleaseDC(nullptr, hdc);

    if (!hBmp || !pBits)
    {
        m_pBits = nullptr;
        m_hasBitmap = false;
        return;
    }

    m_pBits = static_cast<BYTE*>(pBits);
    m_bitmap.Attach(hBmp);
    m_hasBitmap = true;
}

void CMandelbrotDoc::ResizeBitmapForDisplay(HWND, int clientWidth, int clientHeight)
{
    if (clientWidth <= 0 || clientHeight <= 0)
        return;

    ResizeBitmap(clientWidth, clientHeight);
}

void CMandelbrotDoc::RenderMandelbrot()
{
    if (!m_pBits || !m_bitmap.GetSafeHandle() || m_width <= 0 || m_height <= 0)
        return;

    const double pixelAspect = double(m_width) / m_height;
    const double planeH = m_scale;
    const double planeW = planeH * pixelAspect;

    const double left = m_centerX - planeW / 2.0;
    const double top = m_centerY + planeH / 2.0;

    const int stride = m_width * 4;   // 32‑bit, no padding

    for (int y = 0; y < m_height; ++y)
    {
        BYTE* row = m_pBits + y * stride;
        double cy = top - (double(y) / (m_height - 1)) * planeH;

        for (int x = 0; x < m_width; ++x)
        {
            double cx = left + (double(x) / (m_width - 1)) * planeW;

            double zx = 0.0, zy = 0.0;
            int iter = 0;

            while (zx * zx + zy * zy <= 4.0 && iter < m_maxIter)
            {
                double zx2 = zx * zx - zy * zy + cx;
                double zy2 = 2.0 * zx * zy + cy;
                zx = zx2;
                zy = zy2;
                ++iter;
            }

            BYTE r, g, b;

            if (iter == m_maxIter)
            {
                r = g = b = 0;
            }
            else
            {
                r = static_cast<BYTE>(g_state.rmin + (((g_state.rmax - g_state.rmin) * iter) / m_maxIter));
                g = static_cast<BYTE>(g_state.gmin + (((g_state.gmax - g_state.gmin) * iter) / m_maxIter));
                b = static_cast<BYTE>(g_state.bmin + (((g_state.bmax - g_state.bmin) * iter) / m_maxIter));
            }

            int o = x * 4;
            row[o + 0] = b;
            row[o + 1] = g;
            row[o + 2] = r;
            row[o + 3] = 0;   // unused alpha
        }
    }

    SetModifiedFlag(FALSE);
}

bool CMandelbrotDoc::HasBitmap() const
{
    return m_hasBitmap;
}

CBitmap* CMandelbrotDoc::GetBitmap()
{
    return m_hasBitmap ? (CBitmap*)&m_bitmap : nullptr;
}

#ifdef _DEBUG
void CMandelbrotDoc::AssertValid() const
{
    CDocument::AssertValid();
}

void CMandelbrotDoc::Dump(CDumpContext& dc) const
{
    CDocument::Dump(dc);
}
#endif
