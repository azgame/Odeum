#include "ParticleManager.h"
#include "D3DCore.h"
#include "Buffers/ColourBuffer.h"
#include "Buffers/DepthBuffer.h"

#define MAX_PARTICLES 0x40000

ParticleManager* ParticleManager::sm_instance = nullptr;
std::vector<ParticleEffect*> ParticleManager::sm_activeEffects;

ParticleManager::ParticleManager()
{
}

ParticleManager::~ParticleManager()
{
}

void ParticleManager::Initialize(uint32_t Width, uint32_t Height)
{
	// initialize root signature
	m_rootSignature.Reset(5);
	m_rootSignature[0].InitAsConstants(0, 3);
	m_rootSignature[1].InitAsConstantBuffer(1);
	m_rootSignature[2].InitAsConstantBuffer(2);
	m_rootSignature[3].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 0, 3);
	m_rootSignature[4].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 4);
	m_rootSignature.Finalize(L"Particle Effects");

#define CreatePSO(PSOName, ComputeShader) \
		PSOName.SetRootSignature(m_rootSignature); \
		PSOName.CompileComputeShader(ComputeShader, "main", "cs_5_0"); \
		PSOName.Finalize();

	CreatePSO(sm_particleSpawnCS, L"Engine/Resources/Shaders/ParticleSpawnCS.hlsl");
	CreatePSO(sm_particleUpdateCS, L"Engine/Resources/Shaders/ParticleUpdateCS.hlsl");
	CreatePSO(sm_particleDispatchIndirectArgsCS, L"Engine/Resources/Shaders/ParticleIndirectArgsCS.hlsl");
	CreatePSO(sm_particleFinalDispatchIndirectArgsCS, L"Engine/Resources/Shaders/ParticleFinalIndirectArgsCS.hlsl");

#undef CreatePSO

	// initialize psos
	sm_drawParticlesNoTileNoSort.SetRootSignature(m_rootSignature);
	sm_drawParticlesNoTileNoSort.SetRasterizerState(DXGraphics::rasterTwoSided);
	sm_drawParticlesNoTileNoSort.SetDepthStencilState(DXGraphics::depthReadOnly);
	sm_drawParticlesNoTileNoSort.SetBlendState(DXGraphics::blendPreMultiplied);
	sm_drawParticlesNoTileNoSort.SetInputLayout(0, nullptr);
	sm_drawParticlesNoTileNoSort.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	sm_drawParticlesNoTileNoSort.SetRenderTargetFormat(DXGraphics::m_presentBuffer.GetFormat(), DXGraphics::m_sceneDepthBuffer.GetFormat());
	sm_drawParticlesNoTileNoSort.CompileVertexShader(L"Engine/Resources/Shaders/ParticleVS.hlsl", "main", "vs_5_0");
	sm_drawParticlesNoTileNoSort.CompilePixelShader(L"Engine/Resources/Shaders/ParticlePS.hlsl", "main", "ps_5_0");
	sm_drawParticlesNoTileNoSort.Finalize();

	// create buffers
	__declspec(align(16)) UINT drawIndirectArgs[4] = { 4, 0, 0, 0 };
	m_drawIndirectArgs.Create("DrawIndirectArgs", 1, sizeof(D3D12_DRAW_ARGUMENTS), drawIndirectArgs);
	__declspec(align(16)) UINT dispatchIndirectArgs[6] = { 0, 1, 1, 0, 1, 1 };
	m_particleFinalDispatchIndirectArgs.Create("DispatchIndirectArgs", 1, sizeof(D3D12_DISPATCH_ARGUMENTS), dispatchIndirectArgs);
	m_vertexBuffer.Create("ParticleVertexBuffer", MAX_PARTICLES, sizeof(ParticleVertexData));
	m_indexBuffer.Create("ParticleIndexBuffer", MAX_PARTICLES, sizeof(UINT));

	// create texture array -- later
}

void ParticleManager::CreateEffect(ParticleInitProperties Properties)
{
	std::lock_guard<std::mutex> lg(sm_mutex);

	ParticleEffect* effect = new ParticleEffect(Properties);
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
	Compute.SetDynamicDescriptor(3, 0, m_vertexBuffer.GetCounterSRV(Compute));
	Compute.Dispatch(1, 1, 1);
}

void ParticleManager::Render(CommandContext& Context, const Camera& Camera, ColourBuffer& renderTarget, DepthBuffer& depthTarget)
{

}

void ParticleManager::Uninitialize()
{
}

