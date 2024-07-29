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

#include "Macro.h"

struct Resolution
{
	int	width;
	int	height;
};