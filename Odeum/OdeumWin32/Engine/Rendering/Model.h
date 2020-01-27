#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"

class Model
{
public:
	Model();
	~Model();
	bool Initialize(ID3D12Device*, ID3D12GraphicsCommandList*);
	void Render(ID3D12GraphicsCommandList* commandList_);
	void AddMesh(Mesh* mesh_);
	Mesh* GetMesh() { return m_subMeshes[0]; }
private:
	std::vector<Mesh*> m_subMeshes;
};

#endif