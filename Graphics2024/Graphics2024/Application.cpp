#include "Application.h"
#include "Canvas.h"
Application* Application::sm_pApp;
Application* Application::GetApplication()
{
  if (!sm_pApp)
  {
    sm_pApp = new Application();
  }
  return sm_pApp;
}
Application::Application()
{
  //Initialize app data!!!
}
Application::~Application()
{
  //Uninitialize app data
}
void Application::Run()
{
  MSG msg;
  if (!Initialize())
  {
    MessageBox(NULL,
      TEXT("Error al inicializar"), 
      TEXT("Application:: Run()"), 
      MB_ICONERROR);
    return;
  }
  ShowWindow(m_hWnd, m_nCmdShow);
  while (1)
  {
    if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
    {
      DispatchMessage(&msg);
      if (msg.message == WM_QUIT)
      {
        break;
      }
    }
    Update();
  }
  Uninitialize();
}
bool Application::Initialize()
{
  m_pAdapter = m_DXGIManager.ChooseAdapter();
  if (m_pAdapter)
  {
    if (!m_DXGIManager.InitializeSwapChain(m_pAdapter))
      return false;
  }
  else
  {
    return false;
  }
  return true;
}
bool Application::Uninitialize()
{
  return true;
}

void ShaderCircle(Canvas::PIXEL* pDest, int i, int j,
  int unused_x, int unused_y)
{
  //r^2 > (yc-y)^2 + (xc-x)^2
  int x, y, r= 100*100;
  x = 400 - i;
  y = 400 - j;
  x *= x;
  y *= y;

  if (r > (x + y))
    *pDest = { 255,255,255,0 };
  else
    *pDest = { 255,0,0,0 };
}
void ShaderChess(Canvas::PIXEL* pDest, int i, int j, int x, int y)
{
  int p, q;
  Canvas::PIXEL Black = { 0,0,0,0 }, White = { 255,255,255,0 };
  p = i >> 7; // i/128
  q = j / 128;
  *pDest = (p & 1) ^ (q & 1) ? Black : White;
}

void ShaderColors(Canvas::PIXEL* pDest,
  int i, int j, int x, int y)
{
  Canvas::PIXEL 
    A {128, 30, 90, 0},
    B{ 0,128,128,0 },
    C{ 226,135,67,0 },
    D{ 218,247,166,0 };

  *pDest = Canvas::Lerp(
    Canvas::Lerp(A, B, x >> 8), 
    Canvas::Lerp(C, D, x >> 8), y >> 8);
}

#include <math.h>
void Application::Update()
{
  auto pSwapChain = m_DXGIManager.GetSwapChain();
  auto pCanvas = m_DXGIManager.GetCanvas();
  static float time = 0.0f;
  float phi = time * 3.141592;

  pCanvas->Clear({ 0,0,0,0 });

  float dTheta = 2.0f * 3.141592f / 15;
  for (int i = 0; i < 15; i++)
  {
    int x0, y0;
    x0 = (int)(150 * cosf(dTheta * i + phi) + 400);
    y0 = (int)(150 * sinf(dTheta * i + phi) + 400); 
    for (int j = 0; j < 15; j++)
    {
      int x1, y1;
      x1 = (int)(150 * cosf(dTheta * j + phi) + 400); 
      y1 = (int)(150 * sinf(dTheta * j + phi) + 400);
      pCanvas->Line(x0, y0, x1, y1, { 0,255,6,0 });
    }
  }

  /*//pCanvas->Line(20, 20, 200, 100, { 255,255,255,0 });
  //pCanvas->Line(20, 20, 100, 200, { 255, 255, 255, 0});
  pCanvas->Line(200, 100, 20, 20, { 255,255,255,0 });
  pCanvas->Line(100, 200, 20, 20, { 255, 255, 255, 0 });
  (*pCanvas) (20, 20) = { 0,255,0,0 };
  (*pCanvas) (200, 100) = { 0,255,0,0 };
  //pCanvas->Shade(ShaderColors);*/

  /*//pCanvas->Clear({ 108,14,14,0 });
  
  Canvas::PIXEL Rojo { 144, 0, 32, 0 };
  Canvas::PIXEL Azul = { 0, 128, 128, 0 };

  Canvas::PIXEL
    A {255, 0, 0, 0},
    B{ 0,255,0,0 },
    C{ 255,255,0,0 },
    D{ 0,0,255,0 };

  // Doble integral dx dy
  long delta = (1<<16) * 1 * 255 / pCanvas->GetSizeX();
  long delta1 = (1 << 16) * 1 * 255 / pCanvas->GetSizeY();
  long q = 0;
  for (int j = 0; j < pCanvas->GetSizeY(); j++)
  {
    //short p = (short)((i * 255) / pCanvas->GetSizeX());
    long p = 0;
    for (int i = 0; i < pCanvas->GetSizeX(); i++)
    {
      /*long p = rand() & 0xff;
      p *= 0x01010101;
      (*pCanvas)(i, j).p = p; //*(/)
      Canvas::PIXEL AB = Canvas::Lerp(A, B, p >> 16);
      Canvas::PIXEL CD = Canvas::Lerp(C, D, p >> 16);
      Canvas::PIXEL ABCD = Canvas::Lerp(AB, CD, q >> 16);
      (*pCanvas) (i, j) = ABCD;
      p += delta;
    }
    q += delta1;
  }

  //(*pCanvas)(100, 100) = { 255,255,255,0 };*/
  m_DXGIManager.SendData(pCanvas->GetBuffer(),
    pCanvas->GetPitch());

  Canvas::DestroyCanvas(pCanvas);
  pSwapChain->Present(1, 0);
  time += 1.0f / 60;
}