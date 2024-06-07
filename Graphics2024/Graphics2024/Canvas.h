#pragma once
class Canvas
{
public:
  union PIXEL
  {
    struct
    {
      unsigned char r, g, b, a;
    };
    struct
    {
      unsigned char c, m, y, b;
    };
    long p;
  };
protected:
  int m_nSizeX; // Tamaño horizontal del canvas en píxeles
  int m_nSizeY; // Tamaño vertical del canvas en pixeles PIXEL* m_pBuffer; //Buffer de pixeles
  PIXEL* m_pBuffer;
  Canvas();
  ~Canvas();
public:
  static Canvas* CreateCanvas(int nSizex, int nSizeY);
  static void DestroyCanvas(Canvas* pDestroy);
  PIXEL& operator()(int i, int j);
  void Clear(PIXEL color);
  void* GetBuffer() { return m_pBuffer; };
  unsigned int GetPitch() { return sizeof(PIXEL)* m_nSizeX; }
  int GetSizeX() { return m_nSizeX; }
  int GetSizeY() { return m_nSizeY; }
  static PIXEL Lerp(PIXEL A, PIXEL B, short c);
  typedef void(*SHADER)(PIXEL* dest, int i, int j, int x, int y);
  void Shade(SHADER pShader);
  void Line(int x, int y, int x1, int y1,
    PIXEL Color);
};

