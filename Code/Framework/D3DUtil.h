#pragma once

#include "Framework/D3DHeaders.h"
#include <vector>

//===============================================================
// Geometry generation.

void GenTriGrid(int numVertRows, int numVertCols, float dx, float dz,
	const D3DXVECTOR3 &center, std::vector<D3DXVECTOR3> &verts, std::vector<DWORD> &indices);

//===============================================================
// Colors & Materials

const D3DXCOLOR WHITE(1.0f, 1.0f, 1.0f, 1.0f);
const D3DXCOLOR BLACK(0.0f, 0.0f, 0.0f, 1.0f);
const D3DXCOLOR RED(1.0f, 0.0f, 0.0f, 1.0f);
const D3DXCOLOR GREEN(0.0f, 1.0f, 0.0f, 1.0f);
const D3DXCOLOR BLUE(0.0f, 0.0f, 1.0f, 1.0f);

struct Material
{
	Material() : Ambient(WHITE), Diffuse(WHITE), Spec(WHITE), SpecPower(8.0f) {}
	Material(const D3DXCOLOR& a, const D3DXCOLOR& d, const D3DXCOLOR& s, float power) :
		Ambient(a), Diffuse(d), Spec(s), SpecPower(power) {}

	D3DXCOLOR Ambient;
	D3DXCOLOR Diffuse;
	D3DXCOLOR Spec;
	float SpecPower;
};

struct DirLight
{
	D3DXCOLOR Ambient;
	D3DXCOLOR Diffuse;
	D3DXCOLOR Spec;
	D3DXVECTOR3 DirW;
};

//===============================================================
// .X Files

void LoadXFile(
	const std::wstring& filename,
	ID3DXMesh** meshOut,
	std::vector<Material>& materials,
	std::vector<IDirect3DTexture9*>& textures);

//===============================================================
// Math Constants

const float MY_INFINITY = FLT_MAX;
const float MY_EPSILON  = 0.001f;

//===============================================================
// Bounding Volumes

struct AABB
{
	AABB() : MinPt(MY_INFINITY, MY_INFINITY, MY_INFINITY), MaxPt(-MY_INFINITY, -MY_INFINITY, -MY_INFINITY) {}

	D3DXVECTOR3 Center()
	{
		return 0.5f*(MinPt + MaxPt);
	}

	D3DXVECTOR3 MinPt;
	D3DXVECTOR3 MaxPt;
};

struct BoundingSphere
{
	BoundingSphere() : Pos(0.0f, 0.0f, 0.0f), Radius(0.0f) {}

	D3DXVECTOR3 Pos;
	float Radius;
};
