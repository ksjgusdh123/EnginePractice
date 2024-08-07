#include "MainScene.h"
#include "Mesh.h"

bool CMainScene::Init(ID3D12Device* device)
{
	m_tempMesh = new CMesh;
	{
		std::vector<Vertex> triangleVertices =
		{
			{ { 0.0f, 0.25f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
			{ { 0.25f, -0.25f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
			{ { -0.25f, -0.25f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
		};

		std::vector<UINT32> triangleIndeces =
		{
			{0, 1, 2}
		};

		m_tempMesh->Init(device, triangleVertices, triangleIndeces);
	}

    return true;
}

void CMainScene::Update(float elapsedTime)
{

}

void CMainScene::PostUpdate(float elapsedTime)
{
}

void CMainScene::Render(ID3D12GraphicsCommandList* cmdList, float elapsedTime)
{
	m_tempMesh->Render(cmdList);
}