#include "EngineInfo.h"
#include "RootSignature.h"
#include "Engine.h"
#include "GameFramework.h"
#include "Device.h"

void CRootSignature::Init()
{
	CreateRootSignature();
}

void CRootSignature::CreateRootSignature()
{
	//CD3DX12_DESCRIPTOR_RANGE ranges[] =
	//{
	//	CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, CBV_REGISTER_COUNT - 1, 1), // b1~b4
	//	CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, SRV_REGISTER_COUNT, 0), // t0~t4
	//};

	//CD3DX12_ROOT_PARAMETER param[2];
	//param[0].InitAsConstantBufferView(static_cast<uint32>(CBV_REGISTER::b0)); // b0
	//param[1].InitAsDescriptorTable(_countof(ranges), ranges);

	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	d3dRootSignatureDesc.NumParameters = 0;
	d3dRootSignatureDesc.pParameters = NULL;
	d3dRootSignatureDesc.NumStaticSamplers = 0;
	d3dRootSignatureDesc.pStaticSamplers = NULL;
	d3dRootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ID3DBlob* pd3dSignatureBlob = NULL;
	ID3DBlob* pd3dErrorBlob = NULL;
	::D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);
	CEngine::GetInst()->GetFramework()->GetDevice()->GetDevice()->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));
	
	if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
	if (pd3dErrorBlob) pd3dErrorBlob->Release();
}
