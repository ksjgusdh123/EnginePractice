#pragma once
// ������ �տ� p���̴��� �ʺ��̴��� �𸣱⿡ �ϴ� �Ⱥ���
class CDevice
{
public:
	IDXGIFactory4* GetDXGIFactory() { return m_factory; }
	ID3D12Device* GetDevice() { return m_device; }

public:
	void Init();

private:
	IDXGIFactory4*	m_factory;
	ID3D12Device*	m_device;
};

