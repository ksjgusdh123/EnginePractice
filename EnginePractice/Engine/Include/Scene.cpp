#include "Scene.h"

CScene::CScene()
{
}

CScene::~CScene()
{
}

bool CScene::Init(ID3D12Device* device)
{
    return true;
}

void CScene::Update(float elapsedTime)
{
}

void CScene::PostUpdate(float elapsedTime)
{
}

void CScene::Render(ID3D12GraphicsCommandList* cmdList, float elapsedTime)
{
}
