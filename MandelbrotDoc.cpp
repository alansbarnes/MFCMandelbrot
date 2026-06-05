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

    // Reset bitmap state before allocation
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
    bmi.bmiHeader.biHeight = -m_height;  // Top-down DIB
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 24;
    bmi.bmiHeader.biCompression = BI_RGB;

    HDC hdc = ::GetDC(nullptr);
    void* pBits = nullptr;
    HBITMAP hBmp = ::CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &pBits, nullptr, 0);
    ::ReleaseDC(nullptr, hdc);

    if (!hBmp)
    {
        // Allocation failed; ensure cleanup
        m_pBits = nullptr;
        m_hasBitmap = false;
        return;
    }

    m_pBits = static_cast<BYTE*>(pBits);
    m_bitmap.Attach(hBmp);
    m_hasBitmap = true;
}

void CMandelbrotDoc::ResizeBitmapForDisplay(HWND hWnd, int clientWidth, int clientHeight)
{
    if (clientWidth <= 0 || clientHeight <= 0)
        return;

    // Get DPI for the window
    UINT dpiY = 96;  // Default fallback

    // Try GetDpiForWindow if available (Windows 10+)
    typedef UINT(WINAPI* GetDpiForWindowPtr)(HWND);
    HMODULE hUser32 = ::GetModuleHandleA("user32.dll");
    if (hUser32)
    {
        GetDpiForWindowPtr pGetDpiForWindow = reinterpret_cast<GetDpiForWindowPtr>(
            ::GetProcAddress(hUser32, "GetDpiForWindow"));
        if (pGetDpiForWindow)
        {
            dpiY = pGetDpiForWindow(hWnd);
        }
        else
        {
            // Fallback to GetDeviceCaps with LOGPIXELSY
            HDC hdc = ::GetDC(hWnd);
            if (hdc)
            {
                dpiY = ::GetDeviceCaps(hdc, LOGPIXELSY);
                ::ReleaseDC(hWnd, hdc);
            }
        }
    }

    // Compute height in pixels to maintain 4-inch physical height
    const int heightPx = static_cast<int>(std::lround(4.0 * static_cast<double>(dpiY)));

    // Compute width preserving client aspect ratio
    const double clientAspect = (clientWidth > 0 && clientHeight > 0) ?
        (static_cast<double>(clientWidth) / static_cast<double>(clientHeight)) : 1.0;
    const int widthPx = static_cast<int>(std::lround(heightPx * clientAspect));

    // Allocate bitmap with DPI-aware dimensions
    int finalWidth = widthPx > 0 ? widthPx : 1;
    int finalHeight = heightPx > 0 ? heightPx : 1;
    ResizeBitmap(finalWidth, finalHeight);
}

void CMandelbrotDoc::RenderMandelbrot()
{
    if (!m_pBits || !m_bitmap.GetSafeHandle() || m_width <= 0 || m_height <= 0)
        return;

    // m_scale represents the imaginary plane HEIGHT (e.g., 4.0 initially)
    // Compute the real plane width based on pixel aspect ratio
    const double pixelAspect = double(m_width) / m_height;
    const double planeH = m_scale;
    const double planeW = planeH * pixelAspect;

    const double left = m_centerX - planeW / 2.0;
    const double top = m_centerY + planeH / 2.0;

    // DIB rows must be 4-byte aligned
    const int stride = ((m_width * 3 + 3) / 4) * 4;

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
                r = static_cast<uint8_t>(g_state.rmin + (((g_state.rmax - g_state.rmin) * iter) / m_maxIter));
                g = static_cast<uint8_t>(g_state.gmin + (((g_state.gmax - g_state.gmin) * iter) / m_maxIter));
                b = static_cast<uint8_t>(g_state.bmin + (((g_state.bmax - g_state.bmin) * iter) / m_maxIter));
            }

            int o = x * 3;
            row[o + 0] = b;
            row[o + 1] = g;
            row[o + 2] = r;
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
