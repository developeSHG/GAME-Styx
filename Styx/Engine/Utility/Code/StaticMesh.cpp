#include "StaticMesh.h"

USING(Engine)

Engine::CStaticMesh::CStaticMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMesh(pGraphicDev)
{

}

Engine::CStaticMesh::CStaticMesh(const CStaticMesh& rhs)
	: CMesh(rhs)
	, m_pMesh(rhs.m_pMesh)
	, m_pAdjacency(rhs.m_pAdjacency)
	, m_pSubsetBuffer(rhs.m_pSubsetBuffer)
	, m_pSubsets(rhs.m_pSubsets)
	, m_dwSubsetCnt(rhs.m_dwSubsetCnt)
	, m_pOriMesh(rhs.m_pOriMesh)
	, m_pVtxPos(rhs.m_pVtxPos)
	, m_dwVtxNum(rhs.m_dwVtxNum)
	, m_dwStride(rhs.m_dwStride)

{
	/*  메쉬의 텍스쳐는 깊은복사가 이뤄져야한다. (완전한 깊은복사는 아님)  */
	m_ppTexture = new LPDIRECT3DTEXTURE9[rhs.m_dwSubsetCnt];

	for (_ulong i = 0; i < rhs.m_dwSubsetCnt; ++i)
	{
		m_ppTexture[i] = rhs.m_ppTexture[i];
		m_ppTexture[i]->AddRef();
	}
	m_pOriMesh->AddRef();
	m_pMesh->AddRef();
	m_pAdjacency->AddRef();
	m_pSubsetBuffer->AddRef();
}

Engine::CStaticMesh::~CStaticMesh(void)
{

}

HRESULT Engine::CStaticMesh::Ready_Meshe(const _tchar* pFilePath, const _tchar* pFileName)
{
	_tchar szFullPath[128] = L"";

	/*  해당폴더(/) 까지만 복사  */
	lstrcpy(szFullPath, pFilePath);
	/*  폴더 안 파일이름 이어붙이기  */
	lstrcat(szFullPath, pFileName);

	/*  Mesh Load  */
	FAILED_CHECK_RETURN(D3DXLoadMeshFromX(szFullPath,
										D3DXMESH_MANAGED,
										m_pGraphicDev,
										&m_pAdjacency,
										&m_pSubsetBuffer,
										NULL,
										&m_dwSubsetCnt,
										&m_pOriMesh),
										E_FAIL);

	/*  현재 메쉬가 지닌 정점의 속성 값을 얻어오는 함수  */
	_ulong		dwFVF = m_pOriMesh->GetFVF();
	
	/*  Load한 OriMesh에 명암재질이 무조건 있는건 아니기 때문에 있는지 체크후에  */
	/*  m_pMesh에 새로 복제해서 넘겨주는 형식을 쓸 것 이다  */
	/*  FVF 속성 중 NORMAL 정보가 없을 경우 ( 명암재질이 없다는 말)  */
	if (!(dwFVF & D3DFVF_NORMAL))
	{
		/*  기존의 Load한 메쉬의 FVF 정보를 복제하는 함수  */
		m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF | D3DFVF_NORMAL, m_pGraphicDev, &m_pMesh);

		/*  메쉬 정점의 NORMAL 값을 계산하여 넣어주는 함수  */
		D3DXComputeNormals(m_pMesh, (_ulong*)m_pAdjacency->GetBufferPointer());
	}
	else
	{
		m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF, m_pGraphicDev, &m_pMesh);
	}

	void*		pVertex = nullptr;

	/*  메쉬의 총 정점을 얻어오는 함수  */
	m_dwVtxNum = m_pMesh->GetNumVertices();

	m_pVtxPos = new _vec3[m_dwVtxNum];

	/*  정점들의 POS를 알기위해 메쉬를 열어줄 것이다  */
	m_pMesh->LockVertexBuffer(0, &pVertex);

	/*  MAX_FVF_DECL_SIZE = 정점 하나가 지닐 수 있는 최대의 FVF 개수  */
	D3DVERTEXELEMENT9		Decl[MAX_FVF_DECL_SIZE];
	ZeroMemory(Decl, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);

	/*  매쉬 정점의 정보를 얻어온다  */
	m_pMesh->GetDeclaration(Decl);

	/*  OffSet은 FVF정보들 중에서 몇번쨰 주소에 있는지 체크할 용도로 쓸 것이다  */
	_ulong byOffset = 0;

	for (_ulong i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{
		/*  Stream은 현재 구분점으로만 사용 0,1 의외의 숫자면 끝  */
		/*  Usage는 FVF 속성을 의미함 (셰이더에서 사용)  */
		if (Decl[i].Usage == D3DDECLUSAGE_POSITION)
		{
			/*  Decl[i].Offset은 메모리 간격  */
			/*  형번환 하여서 대입  */
			byOffset = (_ulong)Decl[i].Offset;
			break;
		}
	}

	/*  정점의 정보를 알고 있을 때 정점의 최종 크기를 얻어온다. (그냥 정점간의 간격이라고 생각하자)  */
	/*  = 정점의 FVF를 근거로 정점의 크기를 가져온다.  */
	m_dwStride = D3DXGetFVFVertexSize(dwFVF);


	for (_ulong i = 0; i < m_dwVtxNum; ++i)
	{
		/*  pVertex : 맨 앞 주소   */
		/*  byOffset : FVF Pos가 시작되는 주소  */
		/*  m_dwStride : 정점간의 간격  */
		//m_pVtxPos[i] = *(_vec3*)(((_ubyte*)pVertex) + (byOffset + (m_dwStride * i)));
		m_pVtxPos[i] = *((_vec3*)(((_ubyte*)pVertex) + (m_dwStride * i + byOffset)));
	}

	/*  컴객체가 담고 있는 재질 정보 중 가장 앞 주소를 리턴하는 함수  */
	m_pSubsets = (D3DXMATERIAL*)m_pSubsetBuffer->GetBufferPointer();

	m_ppTexture = new LPDIRECT3DTEXTURE9[m_dwSubsetCnt];

	/*  서브셋의 개수는 텍스쳐의 갯수, 재질의 갯수가 같기에  */
	/*  텍스쳐를 로드해준다.  */
	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		_tchar szFileName[128] = L"";

		/*  다시 해당폴더(/) 까지만 복사  */
		lstrcpy(szFullPath, pFilePath);

		/*  ASKI 코드를 UNI 코드로 전환 해주는 함수  */
		MultiByteToWideChar(CP_ACP,
							0,
							m_pSubsets[i].pTextureFilename,
							strlen(m_pSubsets[i].pTextureFilename),
							szFileName,
							128);

		lstrcat(szFullPath, szFileName);

		FAILED_CHECK_RETURN(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &m_ppTexture[i]), E_FAIL);
	}
	
	m_pMesh->UnlockVertexBuffer();

	return S_OK;
}

void Engine::CStaticMesh::Render_Meshe(void)
{
	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		m_pGraphicDev->SetMaterial(&m_pSubsets[i].MatD3D);
		m_pGraphicDev->SetTexture(0, m_ppTexture[i]);
		m_pMesh->DrawSubset(i);
	}
}

void Engine::CStaticMesh::Render_Meshe(LPD3DXEFFECT pEffect, _uint iPassCnt /*= 0*/)
{
	pEffect->AddRef();

	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		_bool	bAlpha = false;
		//_uint iPassCnt = 0;

		//if (bAlpha = Find_Alpha(m_pSubsets[i].pTextureFilename))
		//	iPassCnt = 1;

		pEffect->SetTexture("g_BaseTexture", m_ppTexture[i]);   
		pEffect->SetTexture("g_DiffuseTexture", m_ppTexture[i]);
		                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          pEffect->SetTexture("g_NormalTexture", m_ppTexture[i]);
		//if(1 <= i)
			//pEffect->SetTexture("g_NormalTexture", m_ppTexture[i]);
		//if (2 <= i)
		//	pEffect->SetTexture("g_NormalTexture", m_ppTexture[MESHTEXTURE::TYPE_SPECULAR]);
		//if (3 <= i)
		//	pEffect->SetTexture("g_NormalTexture", m_ppTexture[MESHTEXTURE::TYPE_EMISSIVE]);

		pEffect->CommitChanges();	// 변경된 텍스처 정보를 정리해주는 함수

		pEffect->BeginPass(iPassCnt);

		m_pMesh->DrawSubset(i);

		pEffect->EndPass();
	}

	Safe_Release(pEffect);
}

CStaticMesh* Engine::CStaticMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName)
{
	CStaticMesh*		pInstance = new CStaticMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_Meshe(pFilePath, pFileName)))
		Safe_Release(pInstance);

	return pInstance;
}

CResources* Engine::CStaticMesh::Clone(void)
{
	return new CStaticMesh(*this);
}

void Engine::CStaticMesh::Free(void)
{
	CMesh::Free();

	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		Safe_Release(m_ppTexture[i]);
	}

	Safe_Release(m_pOriMesh);
	Safe_Release(m_pMesh);
	Safe_Release(m_pAdjacency);
	Safe_Release(m_pSubsetBuffer);

	Safe_Delete_Array(m_ppTexture);

	if (false == m_bClone)
		Safe_Delete_Array(m_pVtxPos);
}

LPDIRECT3DBASETEXTURE9 Engine::CStaticMesh::Get_Texture(_ulong dwMaterialIndex, MESHTEXTURE::TYPE eType)
{
	if (nullptr == m_ppTexture ||
		m_dwSubsetCnt <= dwMaterialIndex)
		return nullptr;

	return m_ppTexture[eType];
}

