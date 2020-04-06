#ifndef SHADERHANDLER_H
#define SHADERHANDLER_H

#include <sstream>
#include <map>
#include "../../pch.h"
#include "../Utilities/DeviceResources.h"
#include "../dxc/dxcapi.h"
#include "../Utilities/DXRHelper.h"

struct DxShaderInfo
{
	LPCWSTR		filename = nullptr;
	LPCSTR		entryPoint = nullptr;
	LPCSTR		targetProfile = nullptr;
	LPCSTR*		arguments = nullptr;
	DxcDefine*	defines = nullptr;
	UINT32		argCount = 0;
	UINT32		defineCount = 0;

	DxShaderInfo() {}
	DxShaderInfo(LPCWSTR inFilename, LPCSTR inEntryPoint, LPCSTR inProfile)
	{
		filename = inFilename;
		entryPoint = inEntryPoint;
		targetProfile = inProfile;
	}

	void operator=(DxShaderInfo info_)
	{
		filename = info_.filename;
		entryPoint = info_.entryPoint;
		targetProfile = info_.targetProfile;
		arguments = info_.arguments;
		defines = info_.defines;
		argCount = info_.argCount;
		defineCount = info_.defineCount;
	}
};

struct DxShaderProgram
{
	ID3D12PipelineState*			pipelineState;
	ID3D12RootSignature*			rootSignature;
	ID3D12DescriptorHeap*			descHeap;
	ID3D12GraphicsCommandList4*		commandList;

	void operator=(DxShaderProgram program_)
	{
		pipelineState = program_.pipelineState;
		rootSignature = program_.rootSignature;
		descHeap = program_.descHeap;
		commandList = program_.commandList;
	}

	~DxShaderProgram ()
	{
		SAFE_RELEASE(pipelineState);
		SAFE_RELEASE(rootSignature);
		SAFE_RELEASE(descHeap);
		SAFE_RELEASE(commandList);
	}
};

class ShaderHandler
{
public:
	ShaderHandler(const ShaderHandler&) = delete;
	ShaderHandler(ShaderHandler&&) = delete;
	ShaderHandler& operator=(const ShaderHandler&) = delete;
	ShaderHandler& operator=(ShaderHandler&&) = delete;

	static ShaderHandler* GetInstance();

	void CreateShaderProgram(DeviceResources* device_, std::string name_, DxShaderInfo vsFile_, DxShaderInfo psFile_);
	DxShaderProgram* GetShader(std::string shaderName_);

private:
	ShaderHandler() {}
	~ShaderHandler();

	void Uninitialize();

	void CreateRootSignature(DeviceResources* device_, ID3D12RootSignature*& dstRootSig_, std::string shaderArgs_);
	void CreatePipelineStateObject(DeviceResources* device_, ID3D12PipelineState*& dstPipeline_, ID3DBlob* vsFile_, ID3DBlob* psFile_, ID3D12RootSignature* rootSig_);
	void CreateCommandList(DeviceResources* device_, ID3D12GraphicsCommandList4*& commandList_, ID3D12PipelineState* pipelineState);

	static std::unique_ptr<ShaderHandler> shaderInstance;
	friend std::default_delete<ShaderHandler>;

	static std::map<std::string, DxShaderProgram*> m_shaderPipelines;
};

#endif