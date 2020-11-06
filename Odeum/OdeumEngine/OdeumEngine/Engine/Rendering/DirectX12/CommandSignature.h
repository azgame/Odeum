#ifndef COMMANDSIGNATURE_H
#define COMMANDSIGNATURE_H

#include "../../../pch.h"
#include "D3DIncludes.h"

#define D3D12_INDIRECT_PARAMETER_NULL 0xFFFFFFFF

class RootSignature;

class IndirectParameter
{
	friend class CommandSignature;

public:
	
	IndirectParameter()
	{
		m_indirectParam.Type = (D3D12_INDIRECT_ARGUMENT_TYPE)D3D12_INDIRECT_PARAMETER_NULL;
	}

	void SetType(D3D12_INDIRECT_ARGUMENT_TYPE Type) { m_indirectParam.Type = Type; }

	void SetAsVertexBufferView(UINT slot)
	{
		m_indirectParam.Type = D3D12_INDIRECT_ARGUMENT_TYPE_VERTEX_BUFFER_VIEW;
		m_indirectParam.VertexBuffer.Slot = slot;
	}

	void SetAsIndexBufferView()
	{
		m_indirectParam.Type = D3D12_INDIRECT_ARGUMENT_TYPE_INDEX_BUFFER_VIEW;
	}

	void SetAsConstant(UINT rootIndex, UINT destOffset, UINT num32BitValues)
	{
		m_indirectParam.Type = D3D12_INDIRECT_ARGUMENT_TYPE_CONSTANT;
		m_indirectParam.Constant.RootParameterIndex = rootIndex;
		m_indirectParam.Constant.Num32BitValuesToSet = num32BitValues;
		m_indirectParam.Constant.DestOffsetIn32BitValues = destOffset;
	}

	void SetAsConstantBufferView(UINT rootIndex)
	{
		m_indirectParam.Type = D3D12_INDIRECT_ARGUMENT_TYPE_CONSTANT_BUFFER_VIEW;
		m_indirectParam.ConstantBufferView.RootParameterIndex = rootIndex;
	}

	void SetAsShaderResourceView(UINT rootIndex)
	{
		m_indirectParam.Type = D3D12_INDIRECT_ARGUMENT_TYPE_SHADER_RESOURCE_VIEW;
		m_indirectParam.ShaderResourceView.RootParameterIndex = rootIndex;
	}

	void SetAsUnorderedAccessView(UINT rootIndex)
	{
		m_indirectParam.Type = D3D12_INDIRECT_ARGUMENT_TYPE_UNORDERED_ACCESS_VIEW;
		m_indirectParam.UnorderedAccessView.RootParameterIndex = rootIndex;
	}

	const D3D12_INDIRECT_ARGUMENT_DESC& Desc() const { return m_indirectParam; }

private:

	D3D12_INDIRECT_ARGUMENT_DESC m_indirectParam;
};

class CommandSignature
{
public:

	CommandSignature(UINT numParams = 0) : m_finalized(false), m_numParams(numParams)
	{
		Reset(numParams);
	}

	void Destroy()
	{
		m_commandSignature = nullptr;
		m_paramArray = nullptr;
	}

	void Reset(UINT numParams)
	{
		if (numParams > 0)
		{
			m_paramArray.reset(new IndirectParameter[numParams]);
		}
		else
			m_paramArray = nullptr;
		
		m_numParams = numParams;
	}

	IndirectParameter& operator[] (size_t index)
	{
		ASSERT(index < m_numParams, "Index out of range violation!");
		return m_paramArray.get()[index];
	}

	const IndirectParameter& operator[] (size_t index) const
	{
		ASSERT(index < m_numParams, "Index out of range violation!");
		return m_paramArray.get()[index];
	}

	void Finalize(const RootSignature* rootSig = nullptr);

	ID3D12CommandSignature* GetSignature() const { return m_commandSignature.Get(); }

protected:

	bool m_finalized;
	UINT m_numParams;
	std::unique_ptr<IndirectParameter[]> m_paramArray;
	Microsoft::WRL::ComPtr<ID3D12CommandSignature> m_commandSignature;
};

#endif