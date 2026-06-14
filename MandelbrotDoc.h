#pragma once

class CMandelbrotDoc : public CDocument
{
protected:
    CMandelbrotDoc() noexcept;
    DECLARE_DYNCREATE(CMandelbrotDoc)

public:
    bool HasBitmap() const;
    CBitmap* GetBitmap();

private:
    bool m_hasBitmap = false;

public:
    CBitmap m_bitmap;
    BYTE* m_pBits;

    int     m_width;
    int     m_height;

    double  m_centerX;
    double  m_centerY;
    double  m_scale;
    int     m_maxIter;
    
    int     m_rmin;
    int     m_rmax;
    int     m_gmin;
    int     m_gmax;
    int     m_bmin;
    int     m_bmax;

public:
    virtual BOOL OnNewDocument() override;
    virtual void Serialize(CArchive& ar) override;

    void ResizeBitmap(int width, int height);
    void ResizeBitmapForDisplay(HWND hWnd, int clientWidth, int clientHeight);
    void RenderMandelbrot();

    DECLARE_MESSAGE_MAP()

public:
#ifdef _DEBUG
    virtual void AssertValid() const override;
    virtual void Dump(CDumpContext& dc) const override;
#endif
};
