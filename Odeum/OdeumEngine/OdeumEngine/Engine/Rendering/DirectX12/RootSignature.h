#ifndef ROOTSIGNATURE_H
#define ROOTSIGNATURE_H

// Comments by Aidan Zizys, 2020

#include "D3DIncludes.h"

#define D3D12_ROOT_PARAMETER_NULL (D3D12_ROOT_PARAMETER_TYPE)0xFFFFFFFF

// Root parameter uses functions to initialize root params to certain states. Pretty straightforward
class RootParameter
{
	friend class RootSignature;

public:

	RootParameter()
	{
		m_rootParam.ParameterType = D3D12_ROOT_PARAMETER_NULL;
	}

	~RootParameter()
	{
		Clear();
	}

	void Clear()
	{
		if (m_rootParam.ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
			SAFE_DELETE_ARRAY(m_rootParam.DescriptorTable.pDescriptorRanges);

		m_rootParam.ParameterType = D3D12_ROOT_PARAMETER_NULL;
	}

	void InitAsConstants(UINT register_, UINT numDWords_, D3D12_SHADER_VISIBILITY visibility_ = D3D12_SHADER_VISIBILITY_ALL)
	{
		m_rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		m_rootParam.ShaderVisibility = visibility_;
		m_rootParam.Constants.Num32BitValues = numDWords_;
		m_rootParam.Constants.ShaderRegister = register_;
		m_rootParam.Constants.RegisterSpace = 0;
	}

	void InitAsConstantBuffer(UINT register_, D3D12_SHADER_VISIBILITY visibility_ = D3D12_SHADER_VISIBILITY_ALL)
	{
		m_rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		m_rootParam.ShaderVisibility = visibility_;
		m_rootParam.Descriptor.ShaderRegister = register_;
		m_rootParam.Descriptor.RegisterSpace = 0;
	}

	void InitAsBufferSRV(UINT register_, D3D12_SHADER_VISIBILITY visibility_ = D3D12_SHADER_VISIBILITY_ALL)
	{
		m_rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
		m_rootParam.ShaderVisibility = visibility_;
		m_rootParam.Descriptor.ShaderRegister = register_;
		m_rootParam.Descriptor.RegisterSpace = 0;
	}

	void InitAsBufferUAV(UINT register_, D3D12_SHADER_VISIBILITY visibility_ = D3D12_SHADER_VISIBILITY_ALL)
	{
		m_rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
		m_rootParam.ShaderVisibility = visibility_;
		m_rootParam.Descriptor.ShaderRegister = register_;
		m_rootParam.Descriptor.RegisterSpace = 0;
	}

	void InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE type_, UINT register_, UINT count_, D3D12_SHADER_VISIBILITY visibility_ = D3D12_SHADER_VISIBILITY_ALL)
	{
		InitAsDescriptorTable(1, visibility_);
		SetTableRange(0, type_, register_, count_);
	}

	void InitAsDescriptorTable(UINT rangeCount_, D3D12_SHADER_VISIBILITY visibility_ = D3D12_SHADER_VISIBILITY_ALL)
	{
		m_rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		m_rootParam.ShaderVisibility = visibility_;
		m_rootParam.DescriptorTable.NumDescriptorRanges = rangeCount_;
		m_rootParam.DescriptorTable.pDescriptorRanges = new D3D12_DESCRIPTOR_RANGE[rangeCount_];
	}

	void SetTableRange(UINT rangeIndex_, D3D12_DESCRIPTOR_RANGE_TYPE type_, UINT register_, UINT count_, UINT space_ = 0)
	{
		D3D12_DESCRIPTOR_RANGE* range = const_cast<D3D12_DESCRIPTOR_RANGE*>(m_rootParam.DescriptorTable.pDescriptorRanges + rangeIndex_);
		range->RangeType = type_;
		range->BaseShaderRegister = register_;
		range->NumDescriptors = count_;
		range->RegisterSpace = space_;
		range->OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	}

	const D3D12_ROOT_PARAMETER& operator()() const { return m_rootParam; }

protected:

	D3D12_ROOT_PARAMETER m_rootParam;
};

// Root signature acts as a collection of root parameters, including a bitmap for used parameter indices and size of tables
class RootSignature
{
	friend class DynamicDescriptorHeap;

public:

	RootSignature(UINT numRootParams_ = 0, UINT numStaticSamplers_ = 0) : m_finalized(false), m_numParameters(numRootParams_)
	{
		Reset(numRootParams_, numStaticSamplers_);
	}

	~RootSignature()
	{
	}

	void Destroy()
	{
		if (m_rootSignature) m_rootSignature->Release();  m_rootSignature = nullptr;
	}

	void Reset(UINT numRootParams_, UINT numStaticSamplers = 0);

	void InitStaticSampler(UINT register_, const D3D12_SAMPLER_DESC& nonStaticSamplerDesc_,
		D3D12_SHADER_VISIBILITY visibility_ = D3D12_SHADER_VISIBILITY_ALL);

	void Finalize(const std::wstring& name_, D3D12_ROOT_SIGNATURE_FLAGS flags_ = D3D12_ROOT_SIGNATURE_FLAG_NONE);

	RootParameter& operator[](size_t index_)
	{
		ASSERT(index_ < m_numParameters, "If we're accessing a root parameter, it must be within the range");
		return m_parameterArray.get()[index_];
	}

	const RootParameter& operator[](size_t index_) const
	{
		ASSERT(index_ < m_numParameters, "If we're accessing a root parameter, it must be within the range");
		return m_parameterArray.get()[index_];
	}

	ID3D12RootSignature* GetRootSignature() const { return m_rootSignature; }

protected:

	bool m_finalized;
	UINT m_numParameters;
	UINT m_numStaticSamplers;
	UINT m_numInitializedStaticSamplers;
	uint32_t m_descriptorTableBitMap;
	uint32_t m_SamplerTableBitMap;
	uint32_t m_descriptorTableSize[16];
	std::unique_ptr<RootParameter[]> m_parameterArray;
	std::unique_ptr<D3D12_STATIC_SAMPLER_DESC[]> m_samplerArray;
	ID3D12RootSignature* m_rootSignature;
};

#endif