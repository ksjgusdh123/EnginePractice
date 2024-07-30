#pragma once

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include "resource.h"

#include <string>
#include <list>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <memory>

// DirectX12 관련 헤더
#include <string>
#include <wrl.h>
#include <shellapi.h>

#include <d3d12.h>
#include <dxgi1_4.h>
#include "d3dx12.h"
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>

#include <D3d12SDKLayers.h>

#include <Mmsystem.h>

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

using namespace DirectX;
using namespace DirectX::PackedVector;

using Microsoft::WRL::ComPtr;

#include "Macro.h"

struct Resolution
{
	int	width;
	int	height;
};

struct ScreenInfo
{
	HWND	hWnd;
	int		width;
	int		height;
	bool	bMsaa4xEnable;
	UINT	nMsaa4xQualityLevels;
};

enum
{
	SWAP_CHAIN_BUFFER_COUNT = 2
};