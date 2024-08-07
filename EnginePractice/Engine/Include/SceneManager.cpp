#include "EngineInfo.h"
#include "SceneManager.h"
#include "Scene.h"
#include "DefaultScene.h"

DEFINITION_SINGLE(CSceneManager)

CSceneManager::CSceneManager()
{
}

CSceneManager::~CSceneManager()
{
}

bool CSceneManager::Init(ID3D12Device* device)
{
	CreateScene<CDefaultScene>(device);

	return true;
}

bool CSceneManager::Update(float elapsedTime)
{
	m_scene->Update(elapsedTime);

	return ChangeScene();
}

bool CSceneManager::PostUpdate(float elapsedTime)
{
	m_scene->PostUpdate(elapsedTime);

	return ChangeScene();
}

void CSceneManager::Render(ID3D12GraphicsCommandList* cmdList, float elapsedTime)
{
	m_scene->Render(cmdList, elapsedTime);
}

bool CSceneManager::ChangeScene()
{
	if (m_nextScene)
	{
		m_scene.swap(m_nextScene);
		m_nextScene.reset();

		return true;
	}

	return false;
}
