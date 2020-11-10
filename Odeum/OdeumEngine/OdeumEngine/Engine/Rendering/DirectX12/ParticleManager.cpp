#include "ParticleManager.h"
#include "D3DCore.h"
#include "CommandContext.h"
#include "Buffers/ColourBuffer.h"
#include "Buffers/DepthBuffer.h"
#include "RootSignature.h"
#include "../../Core/Camera.h"

#define MAX_PARTICLES 0x40000

ParticleManager* ParticleManager::sm_instance = nullptr;
std::mutex ParticleManager::sm_mutex;
std::vector<ParticleEffect*> ParticleManager::sm_activeEffects;

ParticleManager::ParticleManager()
{
}

ParticleManager::~ParticleManager()
{
	SAFE_DELETE(sm_instance);
	readBack.UnMap();
}

void ParticleManager::Initialize(uint32_t Width, uint32_t Height)
{
	// initialize root signature
	m_rootSignature.Reset(5);
	m_rootSignature[0].InitAsConstants(0, 1);
	m_rootSignature[1].InitAsConstantBuffer(1);
	m_rootSignature[2].InitAsConstantBuffer(2);
	m_rootSignature[3].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 0, 3);
	m_rootSignature[4].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 3);
	m_rootSignature.Finalize(L"Particle Effects");

#define CreatePSO(PSOName, ComputeShader) \
		PSOName.SetRootSignature(m_rootSignature); \
		PSOName.CompileComputeShader(ComputeShader, "main", "cs_5_0"); \
		PSOName.Finalize();

	CreatePSO(sm_particleSpawnCS, L"Engine/Shaders/ParticleSpawnCS.hlsl");
	CreatePSO(sm_particleUpdateCS, L"Engine/Shaders/ParticleUpdateCS.hlsl");
	CreatePSO(sm_particleDispatchIndirectArgsCS, L"Engine/Shaders/ParticleDispatchArgs.hlsl");
	CreatePSO(sm_particleFinalDispatchIndirectArgsCS, L"Engine/Shaders/ParticleFinalDispatchArgs.hlsl");

#undef CreatePSO

	// initialize psos
	sm_drawParticlesNoTileNoSort.SetRootSignature(m_rootSignature);
	sm_drawParticlesNoTileNoSort.SetRasterizerState(DXGraphics::rasterTwoSided);
	sm_drawParticlesNoTileNoSort.SetDepthStencilState(DXGraphics::depthReadOnly);
	sm_drawParticlesNoTileNoSort.SetBlendState(DXGraphics::blendPreMultiplied);
	sm_drawParticlesNoTileNoSort.SetInputLayout(0, nullptr);
	sm_drawParticlesNoTileNoSort.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	sm_drawParticlesNoTileNoSort.SetRenderTargetFormat(DXGraphics::m_presentBuffer.GetFormat(), DXGraphics::m_sceneDepthBuffer.GetFormat());
	sm_drawParticlesNoTileNoSort.CompileVertexShader(L"Engine/Shaders/ParticleVS.hlsl", "main", "vs_5_0");
	sm_drawParticlesNoTileNoSort.CompilePixelShader(L"Engine/Shaders/ParticlePS.hlsl", "main", "ps_5_0");
	sm_drawParticlesNoTileNoSort.Finalize();

	// create buffers
	__declspec(align(16)) UINT drawIndirectArgs[4] = { 4, 0, 0, 0 };
	m_drawIndirectArgs.Create("DrawIndirectArgs", 1, sizeof(D3D12_DRAW_ARGUMENTS), drawIndirectArgs);
	__declspec(align(16)) UINT dispatchIndirectArgs[6] = { 0, 1, 1, 0, 1, 1 };
	m_particleFinalDispatchIndirectArgs.Create("DispatchIndirectArgs", 1, sizeof(D3D12_DISPATCH_ARGUMENTS), dispatchIndirectArgs);
	m_vertexBuffer.Create("ParticleVertexBuffer", MAX_PARTICLES, sizeof(ParticleVertexData));
	m_indexBuffer.Create("ParticleIndexBuffer", MAX_PARTICLES, sizeof(UINT));
	readBack.Create(1, 4);
	

	// create texture array -- later
}

void ParticleManager::CreateEffect(ParticleInitProperties Properties)
{
	std::lock_guard<std::mutex> lg(sm_mutex);

	ParticleEffect* effect = new ParticleEffect(Properties);
	effect->Initialize();
	sm_activeEffects.push_back(effect);
}

void ParticleManager::Update(ComputeContext& Compute, float deltaTime)
{
	Compute.ResetCounter(m_vertexBuffer);

	if (sm_activeEffects.size() == 0)
		return;

	Compute.SetRootSignature(m_rootSignature);
	Compute.SetConstants(0, deltaTime);
	Compute.TransitionResource(m_vertexBuffer, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	Compute.SetDynamicDescriptor(3, 0, m_vertexBuffer.UnorderedAccessView());

	for (int i = 0; i < sm_activeEffects.size(); i++)
	{
		sm_activeEffects[i]->Update(Compute, deltaTime);

		if (sm_activeEffects[i]->GetLifeTime() <= sm_activeEffects[i]->GetElapsedTime())
		{
			auto iter = sm_activeEffects.begin() + i;
			sm_mutex.lock();
			sm_activeEffects.erase(iter);
			sm_mutex.unlock();
		}
	}

	Compute.SetPipelineState(sm_particleFinalDispatchIndirectArgsCS);
	Compute.TransitionResource(m_vertexBuffer, D3D12_RESOURCE_STATE_GENERIC_READ);
	Compute.TransitionResource(m_particleFinalDispatchIndirectArgs, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	Compute.TransitionResource(m_drawIndirectArgs, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	Compute.SetDynamicDescriptor(3, 0, m_particleFinalDispatchIndirectArgs.UnorderedAccessView());
	Compute.SetDynamicDescriptor(3, 1, m_drawIndirectArgs.UnorderedAccessView());
	Compute.SetDynamicDescriptor(4, 0, m_vertexBuffer.GetCounterSRV(Compute));
	Compute.Dispatch(1, 1, 1);

	Compute.TransitionResource(m_vertexBuffer.GetCounterBuffer(), D3D12_RESOURCE_STATE_COPY_SOURCE);
	Compute.CopyBufferRegion(readBack, 0, m_vertexBuffer.GetCounterBuffer(), 0, 4);

	particleCount = (UINT*)readBack.Map();
	//if (particleCount != nullptr)
	//	std::cout << "Number of particles: " << *particleCount << std::endl;
	readBack.UnMap();
}
	
void ParticleManager::Render(CommandContext& Context, Camera& Camera, ColourBuffer& renderTarget, DepthBuffer& depthTarget)
{
	uint32_t width = renderTarget.GetWidth();
	uint32_t height = renderTarget.GetHeight();

	ASSERT(width == depthTarget.GetWidth() && height == depthTarget.GetHeight(), "Mismatch in size of render and depth targets");

	m_CBperFrameData.viewProjMatrix = Camera.GetViewProjMatrix();
	m_CBperFrameData.invView = Matrix4(DirectX::XMMatrixInverse(nullptr, Camera.GetViewMatrix()));

	D3D12_RECT scissor;
	scissor.left = 0;
	scissor.top = 0;
	scissor.right = (LONG)width;
	scissor.bottom = (LONG)height;

	D3D12_VIEWPORT viewport;
	viewport.TopLeftX = 0.0;
	viewport.TopLeftY = 0.0;
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = -1.0;
	viewport.MaxDepth = 0.0;

	GraphicsContext& Graphics = Context.GetGraphicsContext();

	Graphics.SetRootSignature(m_rootSignature);
	Graphics.SetDynamicConstantBufferView(1, sizeof(ParticleSharedConstantBuffer), &m_CBperFrameData);
	Graphics.SetPipelineState(sm_drawParticlesNoTileNoSort);
	Graphics.TransitionResource(m_vertexBuffer, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
	Graphics.TransitionResource(m_drawIndirectArgs, D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT);
	Graphics.TransitionResource(m_indexBuffer, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
	Graphics.SetDynamicDescriptor(4, 1, m_vertexBuffer.ShaderResourceView());
	Graphics.SetDynamicDescriptor(4, 2, m_indexBuffer.ShaderResourceView());
	Graphics.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	Graphics.TransitionResource(renderTarget, D3D12_RESOURCE_STATE_RENDER_TARGET);
	Graphics.TransitionResource(depthTarget, D3D12_RESOURCE_STATE_DEPTH_READ);
	Graphics.SetRenderTarget(renderTarget.GetRTV(), depthTarget.GetDSV_DepthReadOnly());
	Graphics.SetViewportAndScissor(viewport, scissor);
	Graphics.DrawIndirect(m_drawIndirectArgs);
}

void ParticleManager::Uninitialize()
{
}

