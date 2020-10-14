#pragma once

#include "Framework/D3DHeaders.h"

// P - Position
// C - Color
// N - Normal
// T - Texcoord

struct VertexP
{
	VertexP() : Pos(0.f, 0.f, 0.f) {}
	VertexP(float x, float y, float z) : Pos(x, y, z) {}
	VertexP(const D3DXVECTOR3& v) : Pos(v) {}

	D3DXVECTOR3 Pos;
	static IDirect3DVertexDeclaration9 *Decl;
};

struct VertexPC
{
	VertexPC() : Pos(0.f, 0.f, 0.f), Color(0x00000000) {}
	VertexPC(float x, float y, float z, D3DCOLOR c) : Pos(x, y, z), Color(c) {}
	VertexPC(const D3DXVECTOR3& v, D3DCOLOR c) : Pos(v), Color(c) {}

	D3DXVECTOR3 Pos;
	D3DCOLOR    Color;
	static IDirect3DVertexDeclaration9 *Decl;
};

struct VertexPN
{
	VertexPN() : Pos(0.0f, 0.0f, 0.0f), Normal(0.0f, 0.0f, 0.0f) {}
	VertexPN(float x, float y, float z, float nx, float ny, float nz) : Pos(x, y, z), Normal(nx, ny, nz) {}
	VertexPN(const D3DXVECTOR3 &v, const D3DXVECTOR3 &n) : Pos(v), Normal(n) {}

	D3DXVECTOR3 Pos;
	D3DXVECTOR3 Normal;
	static IDirect3DVertexDeclaration9 *Decl;
};

struct VertexPNT
{
	VertexPNT() : Pos(0.0f, 0.0f, 0.0f), Normal(0.0f, 0.0f, 0.0f), Tex0(0.0f, 0.0f) {}
	VertexPNT(float x, float y, float z, float nx, float ny, float nz, float u, float v) : Pos(x, y, z), Normal(nx, ny, nz), Tex0(u, v) {}
	VertexPNT(const D3DXVECTOR3 &v, const D3DXVECTOR3 &n, const D3DXVECTOR3 &uv) : Pos(v), Normal(n), Tex0(uv) {}

	D3DXVECTOR3 Pos;
	D3DXVECTOR3 Normal;
	D3DXVECTOR2 Tex0;
	static IDirect3DVertexDeclaration9 *Decl;
};

void InitAllVertexDeclarations();
void DestroyAllVertexDeclarations();
