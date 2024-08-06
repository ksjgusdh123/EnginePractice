#include "EngineInfo.h"
#include "Shader.h"
#include "Engine.h"
#include "GameFramework.h"
#include "DepthStencilBuffer.h"
#include "Device.h"
#include "RootSignature.h"
#include "CommandQueue.h"

void CShader::Init(const std::wstring& path, ShaderInfo info)
{
	CreateVertexShader(path, "VSMain", "vs_5_1");
	CreatePixelShader(path, "PSMain", "ps_5_1");

	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	m_pipelineDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
	m_pipelineDesc.pRootSignature = CEngine::GetInst()->GetFramework()->GetRootSignature()->GetRootSignature();	
	m_pipelineDesc.VS.pShaderBytecode = m_vsBlob->GetBufferPointer();
	m_pipelineDesc.VS.BytecodeLength = m_vsBlob->GetBufferSize();
	m_pipelineDesc.PS.pShaderBytecode = m_psBlob->GetBufferPointer();
	m_pipelineDesc.PS.BytecodeLength = m_psBlob->GetBufferSize();
	m_pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	m_pipelineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	m_pipelineDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	m_pipelineDesc.SampleMask = UINT_MAX;
	m_pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	m_pipelineDesc.NumRenderTargets = 1;
	m_pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_pipelineDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	m_pipelineDesc.SampleDesc.Count = 1;
	m_pipelineDesc.SampleDesc.Quality = 0;
	m_pipelineDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

	switch (info.rasterizerType)
	{
	case RASTERIZER_TYPE::CULL_BACK:
		m_pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		m_pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
		break;
	case RASTERIZER_TYPE::CULL_FRONT:
		m_pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		m_pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;
		break;
	case RASTERIZER_TYPE::CULL_NONE:
		m_pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		m_pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		break;
	case RASTERIZER_TYPE::WIREFRAME:
		m_pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
		m_pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		break;
	}

	switch (info.depthStencilType)
	{
	case DEPTH_STENCIL_TYPE::LESS:
		m_pipelineDesc.DepthStencilState.DepthEnable = TRUE;
		m_pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		break;
	case DEPTH_STENCIL_TYPE::LESS_EQUAL:
		m_pipelineDesc.DepthStencilState.DepthEnable = TRUE;
		m_pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		break;
	case DEPTH_STENCIL_TYPE::GREATER:
		m_pipelineDesc.DepthStencilState.DepthEnable = TRUE;
		m_pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_GREATER;
		break;
	case DEPTH_STENCIL_TYPE::GREATER_EQUAL:
		m_pipelineDesc.DepthStencilState.DepthEnable = TRUE;
		m_pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_GREATER_EQUAL;
		break;
	}

	CEngine::GetInst()->GetFramework()->GetDevice()->GetDevice()->CreateGraphicsPipelineState(&m_pipelineDesc, IID_PPV_ARGS(&m_pipelineState));
}

void CShader::Update()
{
	CEngine::GetInst()->GetFramework()->GetcmdQueue()->GetCmdList()->SetPipelineState(m_pipelineState);
}

void CShader::CreateShader(const std::wstring& path, const std::string& name, const std::string& version, ID3DBlob** blob, D3D12_SHADER_BYTECODE& shaderByteCode)
{
	UINT32 compileFlag = 0;
#ifdef _DEBUG
	compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	if (FAILED(::D3DCompileFromFile(path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, name.c_str(), version.c_str(), compileFlag, 0, blob, &m_errBlob)))
	{
		::MessageBoxA(nullptr, "Shader Create Failed !", nullptr, MB_OK);
	}

	shaderByteCode = { (*blob)->GetBufferPointer(), (*blob)->GetBufferSize() };
}

void CShader::CreateVertexShader(const std::wstring& path, const std::string& name, const std::string& version)
{
	CreateShader(path, name, version, &m_vsBlob, m_pipelineDesc.VS);
}

void CShader::CreatePixelShader(const std::wstring& path, const std::string& name, const std::string& version)
{
	CreateShader(path, name, version, &m_psBlob, m_pipelineDesc.PS);
}
