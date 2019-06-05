#pragma once

#include <windows.h> 
#include <d3d11.h>
#include <d2d1_1.h>
#include <dxgi1_2.h>
#include <dwrite.h>
#include <wincodec.h>
#include <DirectXMath.h>
#include <dwmapi.h>
#include <windowsx.h>
#include <map>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"D2d1.lib")
#pragma comment(lib,"DWrite.lib")
#pragma comment(lib,"Dwmapi.lib")


template<class Interface>
inline void SafeRelease(Interface **ppInterfaceToRelease){
	if (*ppInterfaceToRelease != NULL){
		(*ppInterfaceToRelease)->Release();
		(*ppInterfaceToRelease) = NULL;
	}
}

typedef struct _VERTEX
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT2 TexCoord;
} VERTEX;

#define NUMVERTICES 6
#define BPP         4

#define OCCLUSION_STATUS_MSG WM_USER
#define WM_BTNCLICK		WM_USER + 0x0000001
#define WM_STARTRECORD  WM_USER + 0x0000002
#define WM_STOPRECORD   WM_USER + 0x0000003