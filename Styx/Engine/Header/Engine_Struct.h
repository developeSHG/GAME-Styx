#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	typedef struct tagVertex_ViewPort
	{
		D3DXVECTOR4		vPosition;
		D3DXVECTOR2		vTexUV;
	}VTXVIEWPORT;

	typedef	struct tagVertexColor
	{
		_vec3			vPos;
		_ulong			dwColor;

	}VTXCOL;

	const _ulong		FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	typedef	struct tagVertexTexture
	{
		_vec3			vPos;
		_vec3			vNormal;
		_vec2		 	vTexUV;

	}VTXTEX;

	typedef struct tagMeshTexture
	{
		enum TYPE { TYPE_DIFFUSE, TYPE_NORMAL, TYPE_SPECULAR, TYPE_EMISSIVE, TYPE_END };

		LPDIRECT3DTEXTURE9		pTextures[TYPE_END];
	}MESHTEXTURE;

	const _ulong		FVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	typedef	struct tagVertexTerrainTexture
	{
		_vec3			vPos;
		_vec2		 	vTexUV;

	}VTXTERRAINTEX;

	const _ulong		FVF_TERRAINTEX = D3DFVF_XYZ | D3DFVF_TEX1;

	typedef	struct tagVertexCubeTexture
	{
		_vec3			vPos;
		_vec3		 	vTex;

	}VTXCUBE;

	const _ulong		FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

	typedef struct tagScreenVertex
	{
		_vec4				vPos;
		_vec2				vTexUV;

	}VTXSCREEN;

	const _ulong		FVF_SCREEN = D3DFVF_XYZRHW | D3DFVF_TEX1;

	typedef	struct tagIndex16
	{
		_ushort	_0, _1, _2;

	}INDEX16;

	typedef	struct tagIndex32
	{
		_ulong	_0, _1, _2;

	}INDEX32;

	typedef struct D3DXFRAME_DERIVED : public D3DXFRAME
	{
		_matrix			CombinedTransformationMatrix;	// 부모 행렬과 최초 뼈대행렬(TransformMatrix)의 곱셈 결과 값을 저장하기 위한 변수

	}D3DXFRAME_DERIVED;

	typedef struct D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER
	{
		LPDIRECT3DTEXTURE9*		ppTexture;

		LPD3DXMESH				pOriMesh;	// 최초 로드 상태의 정보를 가지고 있는 메쉬(불변)

		_ulong					dwNumBones;	// 뼈의 개수, 동적 배열로 할당할 때 사용하는 변수

		_matrix*				pFrameOffSetMatrix; // (애니메이션을 포함한 형태로) 렌더링을 할 때 뼈대들이 가지고 있는 최초의 상태를 저장하기 위한 포인터

		_matrix**				ppFrameCombinedMatrix; // 각 Frame들이 지니고 있는 CombinedTransformationMatrix들의 주소를 한꺼번에 보관하기 위한 포인터
													   // 모든 뼈들이 갱신된 메세지에 따라 행렬의 정보를 동시에 변환하기 위해서 사용

		_matrix*				pRenderingMatrix; // 최종적으로 렌더링을 하기 위한 행렬

	}D3DXMESHCONTAINER_DERIVED;
}

#endif // Engine_Struct_h__
