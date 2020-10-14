#include "Framework/D3DUtil.h"
#include "Framework/D3DVertexFormat.h"
#include <codecvt>

void GenTriGrid(int numVertRows, int numVertCols, float dx, float dz,
	const D3DXVECTOR3 &center, std::vector<D3DXVECTOR3> &verts, std::vector<DWORD> &indices)
{
	int numVertices = numVertRows * numVertCols;
	int numCellRows = numVertRows - 1;
	int numCellCols = numVertCols - 1;

	int numTris = numCellRows * numCellCols * 2;
	float width = (float)numCellCols * dx;
	float depth = (float)numCellRows * dz;

	// vertex generation
	verts.resize(numVertices);

	float xOffset = -width * 0.5f;
	float zOffset = depth * 0.5f;

	int k = 0;
	for (float i = 0; i < numVertRows; ++i)
	{
		for (float j = 0; j < numVertCols; ++j)
		{
			verts[k].x = j * dx + xOffset;
			verts[k].z = -i * dz + zOffset;
			verts[k].y = 0.0f;

			D3DXMATRIX T;
			D3DXMatrixTranslation(&T, center.x, center.y, center.z);
			D3DXVec3TransformCoord(&verts[k], &verts[k], &T);

			++k;
		}
	}

	// index generation
	indices.resize(numTris * 3);

	k = 0;
	for (DWORD i = 0; i < (DWORD)numCellRows; ++i)
	{
		for (DWORD j = 0; j < (DWORD)numCellCols; ++j)
		{
			indices[k] = i * numVertCols + j;
			indices[k + 1] = i * numVertCols + j + 1;
			indices[k + 2] = (i + 1) * numVertCols + j;

			indices[k + 3] = (i + 1) * numVertCols + j;
			indices[k + 4] = i * numVertCols + j + 1;
			indices[k + 5] = (i + 1) * numVertCols + j + 1;

			k += 6;
		}
	}
}

void LoadXFile(
	const std::wstring& filename,
	ID3DXMesh** meshOut,
	std::vector<Material>& materials,
	std::vector<IDirect3DTexture9*>& textures)
{
	// Step 1: Load the .x file from file into a system memory mesh.

	ID3DXMesh* meshSys = nullptr;
	ID3DXBuffer* adjBuffer = nullptr;
	ID3DXBuffer* mtrlBuffer = nullptr;
	DWORD numMtrls = 0;

	HR(D3DXLoadMeshFromX(filename.c_str(), D3DXMESH_SYSTEMMEM, GD3DDevice,
		&adjBuffer, &mtrlBuffer, 0, &numMtrls, &meshSys));


	// Step 2: Find out if the mesh already has normal info?

	D3DVERTEXELEMENT9 elems[MAX_FVF_DECL_SIZE];
	HR(meshSys->GetDeclaration(elems));

	bool hasNormals = false;
	for (int i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{
		// Did we reach D3DDECL_END() {0xFF, 0, D3DDECLTYPE_UNUSED, 0, 0, 0} ?
		if (elems[i].Stream == 0xff)
			break;

		if (elems[i].Type == D3DDECLTYPE_FLOAT3 &&
			elems[i].Usage == D3DDECLUSAGE_NORMAL &&
			elems[i].UsageIndex == 0)
		{
			hasNormals = true;
			break;
		}
	}


	// Step 3: Change vertex format to VertexPNT

	D3DVERTEXELEMENT9 elements[MAX_FVF_DECL_SIZE];
	UINT numElements = 0;
	VertexPNT::Decl->GetDeclaration(elements, &numElements);

	ID3DXMesh* temp = 0;
	HR(meshSys->CloneMesh(D3DXMESH_SYSTEMMEM, elements, GD3DDevice, &temp));
	SafeRelease(meshSys);
	meshSys = temp;


	// Step 4: If the mesh did not have normals, generate them.

	if (hasNormals == false)
		HR(D3DXComputeNormals(meshSys, 0));


	// Step 5: Optimize the mesh.

	HR(meshSys->Optimize(D3DXMESH_MANAGED | D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)adjBuffer->GetBufferPointer(), 0, 0, 0, meshOut));
	SafeRelease(meshSys);
	SafeRelease(adjBuffer);


	// Step 6: Extract the materials and load the textures.
	if (mtrlBuffer != 0 && numMtrls != 0)
	{
		std::wstring basepath;
		auto pos = filename.rfind(L"/");
		if (pos != std::string::npos)
		{
			basepath = filename.substr(0, pos + 1);
		}

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

		D3DXMATERIAL* d3dxmtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();
		for (DWORD i = 0; i < numMtrls; ++i)
		{
			// Save the i-th material. Note that the MatD3D property does not have an ambient
			// value set when its loaded, so just set it to the diffuse value.
			Material m;
			m.Ambient   = d3dxmtrls[i].MatD3D.Diffuse;
			m.Diffuse   = d3dxmtrls[i].MatD3D.Diffuse;
			m.Spec      = d3dxmtrls[i].MatD3D.Specular;
			m.SpecPower = d3dxmtrls[i].MatD3D.Power;
			materials.push_back(m);

			// Check if the i-th material has an associative texture
			if (d3dxmtrls[i].pTextureFilename != 0)
			{
				// Yes, load the texture for the i-th subset
				IDirect3DTexture9* tex = 0;
				std::wstring texFN = basepath + converter.from_bytes(d3dxmtrls[i].pTextureFilename);
				HR(D3DXCreateTextureFromFile(GD3DDevice, texFN.c_str(), &tex));

				textures.push_back(tex);
			}
			else
			{
				textures.push_back(nullptr);
			}
		}
	}

	SafeRelease(mtrlBuffer);
}
