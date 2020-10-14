#include "CommandSignature.h"
#include "RootSignature.h"
#include "D3DCore.h"

void CommandSignature::Finalize(const RootSignature* rootSig)
{
	if (m_finalized)
		return;

	UINT byteStride = 0;
	bool requiresRootSignature = false;

	for (UINT i = 0; i < m_numParams; i++)
	{
		switch (m_paramArray[i].Desc().Type)
		{
		case D3D12_INDIRECT_ARGUMENT_TYPE_DRAW:
			byteStride += sizeof(D3D12_DRAW_ARGUMENTS);
			break;
		case D3D12_INDIRECT_ARGUMENT_TYPE_DRAW_INDEXED:
			byteStride += sizeof(D3D12_DRAW_INDEXED_ARGUMENTS);
			break;
		case D3D12_INDIRECT_ARGUMENT_TYPE_DISPATCH:
			byteStride += sizeof(D3D12_DISPATCH_ARGUMENTS);
			break;
		case D3D12_INDIRECT_ARGUMENT_TYPE_CONSTANT:
			byteStride += sizeof(m_paramArray[i].Desc().Constant.Num32BitValuesToSet * 4);
			requiresRootSignature = true;
			break;
		case D3D12_INDIRECT_ARGUMENT_TYPE_VERTEX_BUFFER_VIEW:
			byteStride += sizeof(D3D12_VERTEX_BUFFER_VIEW);
			break;
		case D3D12_INDIRECT_ARGUMENT_TYPE_INDEX_BUFFER_VIEW:
			byteStride += sizeof(D3D12_INDEX_BUFFER_VIEW);
			break;
		case D3D12_INDIRECT_ARGUMENT_TYPE_CONSTANT_BUFFER_VIEW:
		case D3D12_INDIRECT_ARGUMENT_TYPE_SHADER_RESOURCE_VIEW:
		case D3D12_INDIRECT_ARGUMENT_TYPE_UNORDERED_ACCESS_VIEW:
			byteStride += 8;
			requiresRootSignature = true;
			break;
		}
	}

	D3D12_COMMAND_SIGNATURE_DESC desc;
	desc.ByteStride = byteStride;
	desc.NumArgumentDescs = m_numParams;
	desc.pArgumentDescs = (const D3D12_INDIRECT_ARGUMENT_DESC*)m_paramArray.data();
	desc.NodeMask = 1;

	Microsoft::WRL::ComPtr<ID3DBlob> result, error;

	ID3D12RootSignature* pRootSignature = nullptr;
	if (requiresRootSignature)
	{
		if (rootSig)
			pRootSignature = rootSig->GetRootSignature();

		ASSERT(pRootSignature != nullptr, "If we require a root signature, one must be provided!");
	}

	if (FAILED(DXGraphics::m_device->CreateCommandSignature(&desc, pRootSignature, IID_PPV_ARGS(&m_commandSignature))))
	{
		ERROR("Could not create root signature.");
		throw std::runtime_error("Could not create root signature.");
	}

	m_commandSignature->SetName(L"Command Signature");

	m_finalized = true;
}
