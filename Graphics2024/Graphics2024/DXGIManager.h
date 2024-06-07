#pragma once
#include "framework.h"
#include <dxgi.h>
#include <d3d11.h>

class Canvas;

class DXGIManager
{
protected:
  IDXGISwapChain* m_PIDXGISwapChain;      //Cadena de Intercambios
  ID3D11Device* m_pDev;                   //Abstracci�n de dispositivo
  ID3D11DeviceContext* m_pCtx;            //Operaciones sobre un dispositivo
public:
  IDXGIAdapter* ChooseAdapter();
  boolean InitializeSwapChain(IDXGIAdapter* pAdapter);
  IDXGISwapChain* GetSwapChain() 
  {
    return m_PIDXGISwapChain;
  }
  void SendData(void* pBuffer,      //Buffer
    unsigned int pitch);             //Horizontal lenght
    //unsigned int size)              //Buffer size

  Canvas* GetCanvas();
};

