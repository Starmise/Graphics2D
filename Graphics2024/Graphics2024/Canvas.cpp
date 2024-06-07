#include "Canvas.h"

Canvas::Canvas()
{
  m_nSizeX = 0;
  m_nSizeY = 0;
  m_pBuffer = 0;
}
Canvas::~Canvas()
{
}
Canvas* Canvas::CreateCanvas(int nSizeX, int nSizeY)
{
  Canvas* pNewCanvas = new Canvas();
  pNewCanvas->m_pBuffer = new PIXEL[nSizeX * nSizeY];
  pNewCanvas->m_nSizeX = nSizeX;
  pNewCanvas->m_nSizeY = nSizeY;
  return pNewCanvas;
}
void Canvas::DestroyCanvas(Canvas* pDestroy)
{
  delete[] pDestroy->m_pBuffer;
  delete pDestroy;
}
Canvas::PIXEL& Canvas::operator() (int i, int j)
{
  static PIXEL dummy;
  if (i >= 0 && i < m_nSizeX && j >= 0 && j < m_nSizeY)
  {
    return m_pBuffer[i + j * m_nSizeX];
  }
  return dummy;
}

void Canvas::Clear(PIXEL color)
{
  PIXEL* pPixel = m_pBuffer;
  int c = m_nSizeX * m_nSizeY;
  while (c--)
    *pPixel++ = color;
}

Canvas::PIXEL Canvas::Lerp(PIXEL A, PIXEL B, short c)
{
  /*  0 < c < 255 usando 8 bits una mantisa fija de un numero fraccional
      tc / 255 
      c = 255 t*/

  return {
    (unsigned char)(A.r + (unsigned char)((c * ((short)B.r - A.r)) >> 8)),
    (unsigned char)(A.g + (unsigned char)((c * ((short)B.g - A.g)) >> 8)),
    (unsigned char)(A.b + (unsigned char)((c * ((short)B.b - A.b)) >> 8)),
    (unsigned char)(A.a + (unsigned char)((c * ((short)B.a - A.a)) >> 8))
  };
}

void Canvas::Shade(SHADER pShader)
{
  PIXEL* pDest = m_pBuffer;
  int dx, dy;
  dx = (1 << 16) / m_nSizeX;
  dy = (1 << 16) / m_nSizeY;
  int y = 0;
  for (int j = 0; j < m_nSizeY; j++)
  {
    int x = 0;
    for (int i = 0; i < m_nSizeX; i++)
    {
      pShader(pDest++, i, j, x, y);
      x += dx;
    }
    y += dy;
  }
}

void Canvas::Line(int x0, int y0, int x1, int y1, 
  PIXEL Color)
{
  int dx, dy, p, x, y;
  x = x0;
  y = y0;
  dx = x1 - x0;
  dy = y1 - y0;
  int incx = dx >= 0 ? 1 : -1;
  int incy = dy >= 0 ? 1 : -1;
  dx = dx < 0 ? -dx : dx;
  dy = dy < 0 ? -dy : dy;

  //m <= 1
  if (dy <= dx)
  {
    p = dx - 2 * dy;
    int dp0 = -2 * dy;
    int dp1 = 2 * dx - 2 * dy;
    // Para lineas con m<1
    while (dx--)
    {
      (*this) (x, y) = Color;
      if (p > 0)
      {
        p += dp0;
      }
      else
      {
        p += dp1;
        y+= incy;
      }
      x+=incx;
    }
  }
  else //m > 1        x<------>y
  {
    p = dy - 2 * dx;
    int dp0 = -2 * dx;
    int dp1 = 2 * dy - 2 * dx;
    while (dy--)
    {
      (*this) (x, y) = Color;
      if (p > 0)
      {
        p += dp0;
      }
      else
      {
        p += dp1;
        x+=incx;
      }
      y+=incy;
    }
  }
}