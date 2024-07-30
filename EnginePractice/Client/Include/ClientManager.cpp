#include "ClientManager.h"

CClientManager::CClientManager()
{
}
 
CClientManager::~CClientManager()
{
    CEngine::GetInst()->DestroyInst();
}

bool CClientManager::Init(HINSTANCE hInst)
{
    if (!CEngine::GetInst()->Init(hInst, 960, 640))
        return false;

	return true;
}

int CClientManager::Run()
{
    return CEngine::GetInst()->Run();
}
