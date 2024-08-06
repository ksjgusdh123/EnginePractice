#pragma once
class CRootSignature
{
public:
	ID3D12RootSignature* GetRootSignature() { return m_rootSignature; }

public:
	void Init();
	void CreateRootSignature();


private:
	ID3D12RootSignature* m_rootSignature;
};

