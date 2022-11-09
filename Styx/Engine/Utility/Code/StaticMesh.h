#ifndef StaticMesh_h__
#define StaticMesh_h__

#include "Mesh.h"

BEGIN(Engine)

class ENGINE_DLL CStaticMesh : public CMesh
{
private:
	explicit CStaticMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CStaticMesh(const CStaticMesh& rhs);
	virtual ~CStaticMesh(void);

public:
	HRESULT					Ready_Meshe(const _tchar* pFilePath, const _tchar* pFileName);
	void					Render_Meshe(void);
	void					Render_Meshe(LPD3DXEFFECT pEffect, _uint iPassCnt = 0);

public:
	/*  Get  */
	LPD3DXMESH				Get_Mesh(void) { return m_pMesh; }
	const _ulong			Get_VtxNum(void) const { return m_dwVtxNum; }
	const _ulong			Get_Stride(void) const { return m_dwStride; }
	const _vec3*			Get_VtxPos(void) const { return m_pVtxPos; }
	LPDIRECT3DBASETEXTURE9	Get_Texture(_ulong dwMaterialIndex, MESHTEXTURE::TYPE eType);
	_ulong					Get_NumMaterials() const { return m_dwSubsetCnt; }

private:
	LPD3DXMESH				m_pOriMesh;
	LPD3DXMESH				m_pMesh;
	LPD3DXBUFFER			m_pAdjacency;
	LPD3DXBUFFER			m_pSubsetBuffer;
	D3DXMATERIAL*			m_pSubsets;
	//MESHTEXTURE*			m_ppTextures = nullptr;
	_ulong					m_dwSubsetCnt;
	_ulong					m_dwStride;

private:
	LPDIRECT3DTEXTURE9*		m_ppTexture;

private:
	_ulong					m_dwVtxNum;
	_vec3*					m_pVtxPos;




public:
	static CStaticMesh*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
									const _tchar* pFilePath,
									const _tchar* pFileName);
public:
	virtual CResources*		Clone(void);
private:
	virtual void			Free(void);
};

END

#endif // StaticMesh_h__
