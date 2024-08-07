#pragma once
#include "Scene.h"

class CMainScene : public CScene
{
public:
	virtual bool Init(ID3D12Device* device) override;
	virtual void Update(float elapsedTime) override;
	virtual void PostUpdate(float elapsedTime) override;
	virtual void Render(ID3D12GraphicsCommandList* cmdList, float elapsedTime) override;

private:
	class CMesh	*m_tempMesh;
};

