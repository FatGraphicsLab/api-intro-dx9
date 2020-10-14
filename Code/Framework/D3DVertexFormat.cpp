#include "Framework/D3DVertexFormat.h"

IDirect3DVertexDeclaration9 *VertexP::Decl   = nullptr;
IDirect3DVertexDeclaration9 *VertexPC::Decl  = nullptr;
IDirect3DVertexDeclaration9 *VertexPN::Decl  = nullptr;
IDirect3DVertexDeclaration9 *VertexPNT::Decl = nullptr;

void InitAllVertexDeclarations()
{
	D3DVERTEXELEMENT9 VertexPosElems[] = {
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		D3DDECL_END()
	};
	HR(GD3DDevice->CreateVertexDeclaration(VertexPosElems, &VertexP::Decl));

	D3DVERTEXELEMENT9 VertexPosColElems[] = {
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
		D3DDECL_END()
	};
	HR(GD3DDevice->CreateVertexDeclaration(VertexPosColElems, &VertexPC::Decl));

	D3DVERTEXELEMENT9 VertexPNElems[] = {
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		D3DDECL_END()
	};
	HR(GD3DDevice->CreateVertexDeclaration(VertexPNElems, &VertexPN::Decl));

	D3DVERTEXELEMENT9 VertexPNTElems[] = {
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		D3DDECL_END()
	};
	HR(GD3DDevice->CreateVertexDeclaration(VertexPNTElems, &VertexPNT::Decl));
}

void DestroyAllVertexDeclarations()
{
	SafeRelease(VertexP::Decl);
	SafeRelease(VertexPC::Decl);
	SafeRelease(VertexPN::Decl);
	SafeRelease(VertexPNT::Decl);
}
