#include "LoadOBJModel.h"

LoadOBJModel::LoadOBJModel()
{
	vertices.reserve(200);
	normals.reserve(200);
	textureCoords.reserve(200);
	indices.reserve(200);
	posIndices.reserve(200);
	normalIndices.reserve(200);
	textureIndices.reserve(200);
	meshVertices.reserve(200);
	subMeshes.reserve(10);
}

LoadOBJModel::~LoadOBJModel()
{
	Shutdown();
}

void LoadOBJModel::LoadModel(const std::string& objFilePath_, const std::string& mtlFilePath_)
{
	LoadMaterialLibrary(mtlFilePath_);
	LoadModel(objFilePath_);
}

void LoadOBJModel::LoadModel(const std::string& filePath_)
{
	totalVerts = 0;

	std::ifstream in(filePath_.c_str(), std::ios::in);
	if (!in) {
		Debug::Error("Object file cannot be opened." + filePath_, __FILENAME__, __LINE__);
		return;
	}
	std::string line;
	while (std::getline(in, line)) {
		//VERTEX DATA
		if (line.substr(0, 2) == "v ") {
			std::stringstream v(line.substr(2));
			float x, y, z;
			v >> x >> y >> z;

			vertices.push_back(DirectX::XMFLOAT3(x, y, z));
		}
		//NORMAL DATA
		if (line.substr(0, 3) == "vn ") {
			std::stringstream vn(line.substr(2));
			float x, y, z;

			vn >> x >> y >> z;
			normals.push_back(DirectX::XMFLOAT3(x, y, z));
		}
		//TEXTURE DATA
		if (line.substr(0, 3) == "vt ") {
			std::stringstream vt(line.substr(2));
			float u, v;

			vt >> u >> v;
			textureCoords.push_back(DirectX::XMFLOAT2(u, v));
		}
		//FACE DATA
		/*if (line.substr(0, 2) == "f ") {
			std::stringstream v(line.substr(2));

			int a, b, c, d, e, f, g, h, i;

			v >> a >> b >> c >> d >> e >> f >> g >> h >> i;
			std::cout << "Face 1: " << a << " " << b << " " << c << " Face 2: " << d << " " << e << " " << f << " Face 3: " << g << " " << h << " " << i << std::endl;
			indices.push_back(a);
			indices.push_back(b);
			indices.push_back(c);

			textureIndices.push_back(d);
			textureIndices.push_back(e);
			textureIndices.push_back(f);

			normalIndices.push_back(g);
			normalIndices.push_back(h);
			normalIndices.push_back(i);
		}*/
		if (line.substr(0, 2) == "f ") {
			int vert, text, norm;

			// Find first number in cluster of 3 for each vertex, texture, and normal indicies
			std::string data = line.substr(2);
			int space = data.find(" ");
			std::string d1 = data.substr(0, space);
			std::stringstream vertText(d1.substr(0, d1.find("/")));
			vertText >> vert;
			d1 = d1.substr(d1.find("/") + 1);
			std::stringstream texText(d1.substr(0, d1.find("/")));
			texText >> text;
			d1 = d1.substr(d1.find("/") + 1);
			std::stringstream normText(d1.substr(0, d1.find("/")));
			normText >> norm;

			vert--;
			text--;
			norm--;
			
			bool vertExists = false;

			if (totalVerts >= 3)    //Make sure we at least have one triangle to check
			{
				//Loop through all the vertices
				for (int iCheck = 0; iCheck < totalVerts; ++iCheck)
				{
					//If the vertex position and texture coordinate in memory are the same
					//As the vertex position and texture coordinate we just now got out
					//of the obj file, we will set this faces vertex index to the vertex's
					//index value in memory. This makes sure we don't create duplicate vertices
					if (vert == posIndices[iCheck] && !vertExists && norm == normalIndices[iCheck] && text == textureIndices[iCheck])
					{
						indices.push_back(iCheck);        //Set index for this vertex
						vertExists = true;        //If we've made it here, the vertex already exists
					}
				}
			}

			//If this vertex is not already in our vertex arrays, put it there
			if (!vertExists)
			{
				posIndices.push_back(vert);
				textureIndices.push_back(text);
				normalIndices.push_back(norm);
				totalVerts++;    //We created a new vertex
				indices.push_back(totalVerts - 1);    //Set index for this vertex
			}

			space = data.find(" ");
			
			// Find the second number in the cluster of 3 between the '/'
			data = data.substr(data.find(" ") + 1);
			std::string d2 = data.substr(0, data.find(" "));
			std::stringstream vertText2(d2.substr(0, d2.find("/")));
			vertText2 >> vert;
			d2 = d2.substr(d2.find("/") + 1);
			std::stringstream texText2(d2.substr(0, d2.find("/")));
			texText2 >> text;
			d2 = d2.substr(d2.find("/") + 1);
			std::stringstream normText2(d2.substr(0, d2.find("/")));
			normText2 >> norm;

			vert--;
			text--;
			norm--;

			vertExists = false;

			if (totalVerts >= 3)    //Make sure we at least have one triangle to check
			{
				//Loop through all the vertices
				for (int iCheck = 0; iCheck < totalVerts; ++iCheck)
				{
					//If the vertex position and texture coordinate in memory are the same
					//As the vertex position and texture coordinate we just now got out
					//of the obj file, we will set this faces vertex index to the vertex's
					//index value in memory. This makes sure we don't create duplicate vertices
					if (vert == posIndices[iCheck] && !vertExists && norm == normalIndices[iCheck] && text == textureIndices[iCheck])
					{
						indices.push_back(iCheck);        //Set index for this vertex
						vertExists = true;        //If we've made it here, the vertex already exists
					}
				}
			}

			//If this vertex is not already in our vertex arrays, put it there
			if (!vertExists)
			{
				posIndices.push_back(vert);
				textureIndices.push_back(text);
				normalIndices.push_back(norm);
				totalVerts++;    //We created a new vertex
				indices.push_back(totalVerts - 1);    //Set index for this vertex
			}

			space = data.find(" ");

			//Find the last number after the 2 '/'
			data = data.substr(data.find(" ") + 1);
			std::string d3 = data.substr(0, data.find(" "));
			std::stringstream vertText3(d3.substr(0, d3.find("/")));
			vertText3 >> vert;
			d3 = d3.substr(d3.find("/") + 1);
			std::stringstream texText3(d3.substr(0, d3.find("/")));
			texText3 >> text;
			d3 = d3.substr(d3.find("/") + 1);
			std::stringstream normText3(d3.substr(0, d3.find("/")));
			normText3 >> norm;

			vert--;
			text--;
			norm--;

			vertExists = false;

			if (totalVerts >= 3)    //Make sure we at least have one triangle to check
			{
				//Loop through all the vertices
				for (int iCheck = 0; iCheck < totalVerts; ++iCheck)
				{
					//If the vertex position and texture coordinate in memory are the same
					//As the vertex position and texture coordinate we just now got out
					//of the obj file, we will set this faces vertex index to the vertex's
					//index value in memory. This makes sure we don't create duplicate vertices
					 if (vert == posIndices[iCheck] && !vertExists && norm == normalIndices[iCheck] && text == textureIndices[iCheck])
					 {
							indices.push_back(iCheck);        //Set index for this vertex
							vertExists = true;        //If we've made it here, the vertex already exists
					 }
				}
			}

			//If this vertex is not already in our vertex arrays, put it there
			if (!vertExists)
			{
				posIndices.push_back(vert);
				textureIndices.push_back(text);
				normalIndices.push_back(norm);
				totalVerts++;    //We created a new vertex
				indices.push_back(totalVerts - 1);    //Set index for this vertex
			}

		}
		//New Mesh
		if (line.substr(0, 7) == "usemtl ") {
			if (indices.size() > 0) {
				PostProcessing();
				totalVerts = 0;
			}
			LoadMaterial(line.substr(7));
		}
	}
	PostProcessing();
}

std::vector<Vertex> LoadOBJModel::GetVerts()
{
	return meshVertices;
}

std::vector<UINT16> LoadOBJModel::GetIndices()
{
	return indices;
}

std::vector<SubMesh> LoadOBJModel::GetSubMeshes()
{
	return subMeshes;
}

void LoadOBJModel::Shutdown()
{
	vertices.clear();
	normals.clear();
	textureCoords.clear();
	indices.clear();
	normalIndices.clear();
	textureIndices.clear();
	meshVertices.clear();
	subMeshes.clear();
}

void LoadOBJModel::PostProcessing() {
	for (int i = 0; i < totalVerts; i++) {

		Vertex vert;
		vert.position = vertices[posIndices[i]];
		vert.normal = normals[normalIndices[i]];
		vert.uv = textureCoords[textureIndices[i]];

		meshVertices.push_back(vert);
	}

	SubMesh subMesh;
	subMesh.vertexList = meshVertices;
	subMesh.meshIndices = indices;
	subMesh.m_texture = currentTexture;

	subMeshes.push_back(subMesh);

	indices.clear();
	posIndices.clear();
	normalIndices.clear();
	textureIndices.clear();
	meshVertices.clear();
}

void LoadOBJModel::LoadMaterial(const std::string& matName_) {
	currentTexture = TextureHandler::GetInstance()->GetTexture(matName_);
	if (currentTexture.width == 0)
	{
		TextureHandler::GetInstance()->CreateTexture(matName_, "Engine/Resources/Textures/" + matName_ + ".JPG");
		currentTexture = TextureHandler::GetInstance()->GetTexture(matName_);
	}
}

void LoadOBJModel::LoadMaterialLibrary(const std::string& matFilePath_) {
	std::ifstream in(matFilePath_.c_str(), std::ios::in);
	if (!in) {
		Debug::Error("Material file cannot be opened." + matFilePath_, __FILENAME__, __LINE__);
		return;
	}
	std::string line;
	while (std::getline(in, line)) {
		if (line.substr(0, 7) == "newmtl ") {
			LoadMaterial(line.substr(7));
		}
	}
}