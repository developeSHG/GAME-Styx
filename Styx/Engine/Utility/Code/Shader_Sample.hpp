matrix  g_matWorld;		// ��� ���̺� : Ŭ���̾�Ʈ���� ���� �ڵ忡 �����ϱ� ���� ��¡���� ��Ī
matrix  g_matView;		// matrix == float4x4
matrix  g_matProj;

texture g_BaseTexture;	// ��� ���̺�

sampler BaseSampler = sampler_state	// ���� ���� ������ ��� �ִ� ����Ʈ ������ �ؽ��ĸ� ���̴������� sampler��� �θ���.
{									// = sampler_state�� ���۸� �����Ͽ� ���¸� �����ߴٰ� ǥ���ϴ� ����(�ݵ�� �־�� ��)
	texture = g_BaseTexture;
	
	minfilter = linear;
	magfilter = linear;
};

struct VS_IN
{
	vector		vPosition : POSITION;		// �빮��(Symantic) : ����� �Ӽ��� �����ϴ� ����, vector == float4 ����

	float2		vTexUV	: TEXCOORD0;
};

struct VS_OUT
{
	vector		vPosition : POSITION;		

	float2		vTexUV : TEXCOORD0;
};

// ���ؽ� ���̵�
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV  = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(In.vPosition, matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

// �ȼ� ���̵�

struct PS_IN	// �ȼ� ���̴� ����ü�� POSITION ���� ���� ���� ����.
{
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor : COLOR0;	
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);		// 2���� ������ �ؽ��ķκ��� ���� �����Ͽ� ������ �Լ�(��ȯŸ���� vectorŸ��)

	Out.vColor.r = 0.5f;
	Out.vColor.g = 0.7f;

	return Out;
}

technique Default_Device			// �ϳ��� ���̴� ������ �ϳ��� ��ũ��ũ�� ���� �� �ִ�. ��� �������� pass�� ���� ���� �ִ�.
{
	// ����� ĸ��ȭ
	pass Test
	{
		alphaBlendEnAbLe = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader =  compile ps_3_0 PS_MAIN();

	}

	/*pass Sour
	{
		vertexshader = compile vs_3_0 VS_Temp();
		pixelshader = compile ps_3_0 PS_Sour();
	}*/
};
