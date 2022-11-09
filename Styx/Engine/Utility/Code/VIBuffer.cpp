#include "VIBuffer.h"

Engine::CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CResources(_pGraphicDev)
	, m_pVB(nullptr)
	, m_pIB(nullptr)
	, m_dwVtxSize(0)
	, m_dwVtxCnt(0)
	, m_dwVtxFVF(0)
	, m_dwTriCnt(0)
	, m_dwIdxSize(0)
{

}

Engine::CVIBuffer::CVIBuffer(const CVIBuffer& rhs)
	: CResources(rhs)
	, m_pVB(rhs.m_pVB)
	, m_pIB(rhs.m_pIB)
	, m_dwVtxSize(rhs.m_dwVtxSize)
	, m_dwVtxCnt(rhs.m_dwVtxCnt)
	, m_dwTriCnt(rhs.m_dwTriCnt)
	, m_dwVtxFVF(rhs.m_dwVtxFVF)
	, m_dwIdxSize(rhs.m_dwIdxSize)
	, m_IdxFmt(rhs.m_IdxFmt)
{
	m_pVB->AddRef();
	m_pIB->AddRef();
}

Engine::CVIBuffer::~CVIBuffer(void)
{
	
}

HRESULT Engine::CVIBuffer::Ready_Buffer(void)
{
	m_pGraphicDev->CreateVertexBuffer(m_dwVtxSize * m_dwVtxCnt, // ���� ������ ������
										0,					  // ������ �����(0�� ��� ���� ���۸� ����ϰڴٴ� �ǹ�)
										m_dwVtxFVF,		  // ������ ��Ÿ�� ���� ���ڰ�	
										D3DPOOL_MANAGED,	  // �޸� Ǯ ��Ÿ��	
										&m_pVB,			  // �ƿ�ǲ ��ǥ��ü�� �ּ� 	
										NULL);			      // ���� ���� ���� ���� ���� ���� NULL�� ����


	m_pGraphicDev->CreateIndexBuffer(m_dwIdxSize * m_dwTriCnt,
										0,
										m_IdxFmt,
										D3DPOOL_MANAGED,
										&m_pIB,
										NULL);

	return S_OK;
}

void Engine::CVIBuffer::Render_Buffer(void)
{
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	// ����̽� ��ġ���� ���� �׸����� ���۸� ��ũ�����ִ� �Լ�

	// 1. �� �� ���Կ� ������ ���ΰ�
	// 2. ������ �Ѱ��� ���ΰ�
	// 3. ��𼭺��� �׸� ���ΰ�, 0�� ��� ���۰� �����ϴ� ó������ �׸���� �ǹ�
	// 4. � ������ �׸��⸦ ǥ���� ���ΰ�

	m_pGraphicDev->SetFVF(m_dwVtxFVF);

	m_pGraphicDev->SetIndices(m_pIB);
	// ����̽� ��ġ���� ���� �׸����� ������ �ε����� ��ũ�����ִ� �Լ�

	//m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_dwTriCnt);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
	// �̰��� �ε����� �̿��� �׸��� �Լ��̴�.
}

void Engine::CVIBuffer::Free(void)
{
	CResources::Free();

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
}




