#include "RootSignature.h"

#include "D3DCore.h"

using namespace DXGraphics;


void RootSignature::Destroy()
{
	if (m_rootSignature)
		m_rootSignature->Release();

	m_rootSignature = nullptr;
}

void RootSignature::Reset(UINT numRootParams_, UINT numStaticSamplers)
{
	if (numRootParams_ > 0)
		m_parameterArray.reset(new RootParameter[numRootParams_]);
	else
		m_parameterArray = nullptr;

	m_numParameters = numRootParams_;

	if (numStaticSamplers > 0)
		m_samplerArray.reset(new D3D12_STATIC_SAMPLER_DESC[numStaticSamplers]);
	else
		m_samplerArray = nullptr;

	m_numStaticSamplers = numStaticSamplers;
	m_numInitializedStaticSamplers = 0;
}

void RootSignature::InitStaticSampler(UINT register_, const D3D12_SAMPLER_DESC& nonStaticSamplerDesc_, D3D12_SHADER_VISIBILITY visibility_)
{
	ASSERT(m_numInitializedStaticSamplers < m_numStaticSamplers, "If we're initializing a static sampler, the number of currently initialized ones should be less than our cap");
	D3D12_STATIC_SAMPLER_DESC& staticSamplerDesc = m_samplerArray[m_numInitializedStaticSamplers++];

	staticSamplerDesc.Filter = nonStaticSamplerDesc_.Filter;
	staticSamplerDesc.AddressU = nonStaticSamplerDesc_.AddressU;
	staticSamplerDesc.AddressV = nonStaticSamplerDesc_.AddressV;
	staticSamplerDesc.AddressW = nonStaticSamplerDesc_.AddressW;
	staticSamplerDesc.MipLODBias = nonStaticSamplerDesc_.MipLODBias;
	staticSamplerDesc.MaxAnisotropy = nonStaticSamplerDesc_.MaxAnisotropy;
	staticSamplerDesc.ComparisonFunc = nonStaticSamplerDesc_.ComparisonFunc;
	staticSamplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
	staticSamplerDesc.MinLOD = nonStaticSamplerDesc_.MinLOD;
	staticSamplerDesc.MaxLOD = nonStaticSamplerDesc_.MaxLOD;
	staticSamplerDesc.ShaderRegister = register_;
	staticSamplerDesc.RegisterSpace = 0;
	staticSamplerDesc.ShaderVisibility = visibility_;

	if (staticSamplerDesc.AddressU == D3D12_TEXTURE_ADDRESS_MODE_BORDER ||
		staticSamplerDesc.AddressV == D3D12_TEXTURE_ADDRESS_MODE_BORDER ||
		staticSamplerDesc.AddressW == D3D12_TEXTURE_ADDRESS_MODE_BORDER)
	{
		if (nonStaticSamplerDesc_.BorderColor[3] == 1.0f)
		{
			if (nonStaticSamplerDesc_.BorderColor[0] == 1.0f)
				staticSamplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
			else
				staticSamplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
		}
		else
			staticSamplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	}
}

void RootSignature::Finalize(const std::wstring& name_, D3D12_ROOT_SIGNATURE_FLAGS flags_)
{
	if (m_finalized)
		return; // Can't finalize a root signature that's already ready for submission

	ASSERT(m_numInitializedStaticSamplers == m_numStaticSamplers, "The number of initialized static samplers should be equal to the total number");

	D3D12_ROOT_SIGNATURE_DESC rootDesc;
	rootDesc.NumParameters = m_numParameters;
	rootDesc.pParameters = (const D3D12_ROOT_PARAMETER*)m_parameterArray.get();
	rootDesc.NumStaticSamplers = m_numStaticSamplers;
	rootDesc.pStaticSamplers = (const D3D12_STATIC_SAMPLER_DESC*)m_samplerArray.get();
	rootDesc.Flags = flags_;

	m_descriptorTableBitMap = 0;
	m_SamplerTableBitMap = 0;

	// For each root parameter, flag entry in bitmap as used (1) or leave unused (0). Also set descriptor table size based on ranges in each param
	for (UINT param = 0; param < m_numParameters; param++) 
	{
		const D3D12_ROOT_PARAMETER& rootParam = rootDesc.pParameters[param];
		m_descriptorTableSize[param] = 0;

		if (rootParam.ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
		{
			ASSERT(rootParam.DescriptorTable.pDescriptorRanges != nullptr, "the descriptor ranges of the given root param's desc table must not be null");

			if (rootParam.DescriptorTable.pDescriptorRanges->RangeType == D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER)
				m_SamplerTableBitMap |= (1 << param);
			else
				m_descriptorTableBitMap |= (1 << param);

			for (UINT tableRange = 0; tableRange < rootParam.DescriptorTable.NumDescriptorRanges; tableRange++)
				m_descriptorTableSize[param] += rootParam.DescriptorTable.pDescriptorRanges[tableRange].NumDescriptors;
		}
	}

	ID3DBlob* pResult;
	ID3DBlob* pError;

	if (FAILED(D3D12SerializeRootSignature(&rootDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		&pResult, &pError)))
		Debug::FatalError("Root Signature could not be serialized!", __FILENAME__, __LINE__);

	if (FAILED(m_device->CreateRootSignature(1, pResult->GetBufferPointer(), pResult->GetBufferSize(),
		IID_PPV_ARGS(&m_rootSignature))))
		Debug::FatalError("Root Signature could not be created!", __FILENAME__, __LINE__);

	m_rootSignature->SetName(name_.c_str());

	m_finalized = true;

	if (pResult) pResult->Release(); pResult = nullptr;
	if (pError) pError->Release(); pError = nullptr;
}
