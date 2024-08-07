#include "ClientManager.h"
#include "SceneManager.h"
#include "MainScene.h"
#include "GameFramework.h"

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

    CSceneManager::GetInst()->CreateScene<CMainScene>(CEngine::GetInst()->GetFramework()->GetDevice());

	return true;
}

int CClientManager::Run()
{
    return CEngine::GetInst()->Run();
}
