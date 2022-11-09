#include "Shader.h"

USING(Engine)

Engine::CShader::CShader(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_pEffect(nullptr)
{
	m_pGraphicDev->AddRef();
}

Engine::CShader::CShader(const CShader& rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev)
	, m_pEffect(rhs.m_pEffect)
{
	m_pGraphicDev->AddRef();
	m_pEffect->AddRef();
}

Engine::CShader::~CShader(void)
{

}

HRESULT Engine::CShader::Ready_Shader(const _tchar* pFilePath)
{
	/*m_pEffect == nullptr ������ ���

	1.���� m_pEffect�� ���� �ְ� m_pErrMsg�� ���� NULL�� ��찡 ����
	2.���� m_pEffect ���� �ְ� m_pErrMsg ���� �ִ� ���, ��� �޼����� ���� �ִ� ��Ȳ*/

	if(FAILED(D3DXCreateEffectFromFile(m_pGraphicDev, 
										pFilePath,
										NULL,	// ���̴� ��� �� ������ ��ũ������
										NULL,	// ���̴� ��� �� ������ �������
										D3DXSHADER_DEBUG, // ���̴� ����� �ɼ� �÷���(�ܺζ��̺귯�� ������ �����ؾ������� ���� ����ؼ� �ɼ� ����)
										NULL,	// ���� ���ҽ� ������ ��� ������ ���ΰ�(null�� �־��ָ� �⺻ ������ ��ġ�� ���� : �׷��� ī��)
										&m_pEffect,	
										&m_pErrMsg)))	// ������� �Ұ��������� ���� �� ��� �߻����� ��� ���ڿ� ���·� ���� ��Ȳ�� ������ �� �ִ�.
	{
		MessageBoxA(NULL, (char*)m_pErrMsg->GetBufferPointer(), "Shader_Error", MB_OK);
		return E_FAIL;
	}

	else if (nullptr != m_pErrMsg)
	{
		MessageBoxA(NULL, (char*)m_pErrMsg->GetBufferPointer(), "Shader_warning", MB_OK);
	}
	
	return S_OK;
}

CShader* Engine::CShader::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath)
{
	CShader*	pInstance = new CShader(pGraphicDev);

	if (FAILED(pInstance->Ready_Shader(pFilePath)))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* Engine::CShader::Clone_Component(void)
{
	return new CShader(*this);
}

void Engine::CShader::Free(void)
{
	Safe_Release(m_pEffect);
	Safe_Release(m_pGraphicDev);
}

HRESULT Engine::CShader::Set_Value(D3DXHANDLE hParameter, void* pValue, size_t iSize)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	return m_pEffect->SetValue(hParameter, pValue, iSize);
}

HRESULT Engine::CShader::Set_Texture(D3DXHANDLE hParameter, LPDIRECT3DBASETEXTURE9 pTexture)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	return m_pEffect->SetTexture(hParameter, pTexture);
}

HRESULT Engine::CShader::Set_Bool(D3DXHANDLE hParameter, _bool Value)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	return m_pEffect->SetBool(hParameter, Value);
}

