#include "ShaderHandler.h"


std::unique_ptr<ShaderHandler> ShaderHandler::shaderInstance = nullptr;
std::map<std::string, DxShaderProgram*> ShaderHandler::m_shaderPipelines = std::map<std::string, DxShaderProgram*>();

ShaderHandler* ShaderHandler::GetInstance()
{
	if (shaderInstance.get() == nullptr) {
		shaderInstance.reset(new ShaderHandler);
	}
	return shaderInstance.get();
}

ShaderHandler::~ShaderHandler()
{
	Uninitialize();
}

void ShaderHandler::Uninitialize()
{
	/*if (m_shaderPipelines.size() > 0)
	{
		for (auto p : m_shaderPipelines)
			delete p.second;

		m_shaderPipelines.clear();
	}*/ // Need to fix this before using it
}

void ShaderHandler::CreateRootSignature(ID3D12Device* device_, ID3D12RootSignature*& dstRootSig_, std::string shaderArgs_)
{
	// This is hardcoded for now, as building these resources dynamically is too difficult in such a short time span

	HRESULT result; 

	D3D12_ROOT_DESCRIPTOR rootCBVDescriptor;
	rootCBVDescriptor.RegisterSpace = 0;
	rootCBVDescriptor.ShaderRegister = 0;

	D3D12_DESCRIPTOR_RANGE descriptorTableRanges[1];
	descriptorTableRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorTableRanges[0].NumDescriptors = 1;
	descriptorTableRanges[0].BaseShaderRegister = 0;
	descriptorTableRanges[0].RegisterSpace = 0;
	descriptorTableRanges[0].OffsetInDescriptorsFromTableStart = 1;

	D3D12_ROOT_DESCRIPTOR_TABLE descriptorTable;
	descriptorTable.NumDescriptorRanges = _countof(descriptorTableRanges);
	descriptorTable.pDescriptorRanges = descriptorTableRanges;

	D3D12_ROOT_DESCRIPTOR rootSRVDescriptor;
	rootSRVDescriptor.RegisterSpace = 0;
	rootSRVDescriptor.ShaderRegister = 0;

	D3D12_ROOT_PARAMETER rootParameters[2];
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[0].Descriptor = rootCBVDescriptor;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[1].DescriptorTable = descriptorTable;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	// Init Root Signature
	D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | // Only the input assembler stage needs access to the buffer
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;

	D3D12_STATIC_SAMPLER_DESC sampler = {};
	sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
	sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.MipLODBias = 0;
	sampler.MaxAnisotropy = 0;
	sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	sampler.MinLOD = 0.0f;
	sampler.MaxLOD = D3D12_FLOAT32_MAX;
	sampler.ShaderRegister = 0;
	sampler.RegisterSpace = 0;
	sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	D3D12_ROOT_SIGNATURE_DESC rootDesc = {};
	rootDesc.NumParameters = _countof(rootParameters);
	rootDesc.pParameters = rootParameters;
	rootDesc.Flags = rootSignatureFlags;
	rootDesc.NumStaticSamplers = 1;
	rootDesc.pStaticSamplers = &sampler;

	ID3DBlob* signature;
	ID3DBlob* error;

	D3D12SerializeRootSignature(&rootDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);

	if (FAILED(device_->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&dstRootSig_))))
		Debug::Error("Could not create root signature", __FILENAME__, __LINE__);

	if (signature) signature->Release();
	if (error) error->Release();
}

void ShaderHandler::CreatePipelineStateObject(ID3D12Device* device_, ID3D12PipelineState*& dstPipeline_, ID3DBlob* vertexShader_, ID3DBlob* pixelShader_, ID3D12RootSignature* rootSig_)
{
	// This is hardcoded for now, as building these resources dynamically is too difficult in such a short time span

	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	// Init Pipeline Description
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
	psoDesc.pRootSignature = rootSig_;
	psoDesc.VS = { reinterpret_cast<UINT8*>(vertexShader_->GetBufferPointer()), vertexShader_->GetBufferSize() };
	psoDesc.PS = { reinterpret_cast<UINT8*>(pixelShader_->GetBufferPointer()), pixelShader_->GetBufferSize() };
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.SampleDesc.Count = 1;

	if (FAILED(device_->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&dstPipeline_))))
		Debug::Error("Could not create pipeline state", __FILENAME__, __LINE__);
}

void ShaderHandler::CreateShaderProgram(ID3D12Device* device_, std::string name_, DxShaderInfo vsFile_, DxShaderInfo psFile_)
{
	ID3D12RootSignature* rootSig;

	CreateRootSignature(device_, rootSig, "");

	ID3DBlob* vertexShader;
	ID3DBlob* pixelShader;

	if(FAILED(D3DCompileFromFile(vsFile_.filename, NULL, NULL, vsFile_.entryPoint, vsFile_.targetProfile, D3DCOMPILE_ALL_RESOURCES_BOUND, 0, &vertexShader, NULL)))
		Debug::Error("Could not create vertexshader", __FILENAME__, __LINE__);
	if(FAILED(D3DCompileFromFile(psFile_.filename, NULL, NULL, psFile_.entryPoint, psFile_.targetProfile, D3DCOMPILE_ALL_RESOURCES_BOUND, 0, &pixelShader, NULL)))
		Debug::Error("Could not create pixelshader", __FILENAME__, __LINE__);

	ID3D12PipelineState* pipelineState;

	CreatePipelineStateObject(device_, pipelineState, vertexShader, pixelShader, rootSig);

	m_shaderPipelines[name_] = new DxShaderProgram();
	m_shaderPipelines[name_]->pipelineState = pipelineState;
	m_shaderPipelines[name_]->rootSignature = rootSig;

	pipelineState = nullptr;
	rootSig = nullptr;
}

DxShaderProgram* ShaderHandler::GetShader(std::string shaderName_)
{
	if (m_shaderPipelines.find(shaderName_) != m_shaderPipelines.end()) {
		return m_shaderPipelines[shaderName_];
	}
	return nullptr;
}
