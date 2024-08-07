#pragma once
enum class RASTERIZER_TYPE
{
	CULL_NONE,
	CULL_FRONT,	
	CULL_BACK,
	WIREFRAME,
};

enum class DEPTH_STENCIL_TYPE
{
	LESS,
	LESS_EQUAL,
	GREATER,
	GREATER_EQUAL,
};

struct ShaderInfo
{
	RASTERIZER_TYPE rasterizerType = RASTERIZER_TYPE::CULL_BACK;
	DEPTH_STENCIL_TYPE depthStencilType = DEPTH_STENCIL_TYPE::LESS;
};

class CShader
{
public:

public:
	void Init(const std::wstring& path, ShaderInfo info = ShaderInfo());
	void Update();
	void tempDestroy();

private:
	void CreateShader(const std::wstring& path, const std::string& name, const std::string& version, ID3DBlob** blob, D3D12_SHADER_BYTECODE& shaderByteCode);
	void CreateVertexShader(const std::wstring& path, const std::string& name, const std::string& version);
	void CreatePixelShader(const std::wstring& path, const std::string& name, const std::string& version);

private:
	ID3DBlob							*m_vsBlob;
	ID3DBlob							*m_psBlob;
	ID3DBlob							*m_errBlob;

	ID3D12PipelineState					*m_pipelineState;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC  m_pipelineDesc = {};
};

