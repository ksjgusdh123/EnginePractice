#pragma once
// 포인터 앞에 p붙이는지 않붙이는지 모르기에 일단 안붙임
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

