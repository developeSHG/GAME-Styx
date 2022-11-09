
matrix		g_matWorld, g_matView, g_matProj;
texture		g_DiffuseTexture, g_NormalTexture;

float		g_fAlpha;
bool		g_bCatsleIn = true;
bool		g_bSpecular = false;
bool		g_bIamCatsle = false;
bool		g_bTranslucent = false;

sampler DiffuseSampler = sampler_state
{
	//MinFilter = linear;
	//MagFilter = linear;
	//MipFilter = linear;
	Texture = g_DiffuseTexture;
};

sampler NormalSampler = sampler_state
{
	//MinFilter = linear;
	//MagFilter = linear;
	//MipFilter = linear;
	Texture = g_NormalTexture;
};

struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
};


VS_OUT VS_MAIN(VS_IN In) 
{
	VS_OUT	Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), g_matWorld));
	Out.vTexUV = In.vTexUV;
	Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_matWorld);
	Out.vProjPos = Out.vPosition;
	return Out;
}

struct PS_IN
{
	float4		vPosition : POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
};

struct PS_OUT
{
	vector		vDiffuse : COLOR0;
	vector		vNormal : COLOR1;
	vector		vDepth : COLOR2;
	//vector		vAmbient: COLOR2;
};

PS_OUT PS_MAIN(VS_OUT In)
{
	PS_OUT	Out = (PS_OUT)0;

	if(true == g_bSpecular)
		Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
	else
		Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

	Out.vDiffuse = tex2D(DiffuseSampler, In.vTexUV);
	//Out.vNormal = tex2D(NormalSampler, In.vTexUV);
	if (true == g_bCatsleIn)
	{
		if(true == g_bIamCatsle)
			Out.vDepth = vector(In.vProjPos.w / 500.0f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
		else
		{
			Out.vDepth = vector(In.vProjPos.w / 500.0f, In.vProjPos.z / In.vProjPos.w, 1.f, 0.f);

			if(true == g_bTranslucent)
				Out.vDepth = vector(In.vProjPos.w / 500.0f, In.vProjPos.z / In.vProjPos.w, 1.f, 0.5f);
		}
	}
	else
	{
		if (true == g_bIamCatsle)
			Out.vDepth = vector(In.vProjPos.w / 500.0f, In.vProjPos.z / In.vProjPos.w, 0.f, 1.f);
		else
			Out.vDepth = vector(In.vProjPos.w / 500.0f, In.vProjPos.z / In.vProjPos.w, 1.f, 1.f);
	}

	//Out.vDiffuse = Out.vDiffuse * g_vMtrlDiffuse;
	//Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	//Out.vAmbient = g_vMtrlAmbient;
	//Out.vAmbient.a = 1.f;
	//Out.vNormal.w = 1.f;
	//Out.vDepth = vector(In.vProjPos.w / 500.0f, In.vProjPos.z / In.vProjPos.w, 0.f, 1.f);

	return Out;
}

PS_OUT PS_MAINSPECULAR(VS_OUT In)
{
	PS_OUT	Out = (PS_OUT)0;

	//if (true == g_bSpecular)
		Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);

	Out.vDiffuse = tex2D(DiffuseSampler, In.vTexUV);
	//Out.vNormal = tex2D(NormalSampler, In.vTexUV);
	Out.vDepth = vector(In.vProjPos.w / 500.0f, In.vProjPos.z / In.vProjPos.w, 0.f, 1.f);

	//Out.vDiffuse = Out.vDiffuse * g_vMtrlDiffuse;
	//Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	//Out.vAmbient = g_vMtrlAmbient;
	//Out.vAmbient.a = 1.f;
	//Out.vNormal.w = 1.f;
	//Out.vDepth = vector(In.vProjPos.w / 500.0f, In.vProjPos.z / In.vProjPos.w, 0.f, 1.f);

	return Out;
}

PS_OUT PS_MAINNOSPECULAR(VS_OUT In)
{
	PS_OUT	Out = (PS_OUT)0;

	//if (true == g_bSpecular)
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

	Out.vDiffuse = tex2D(DiffuseSampler, In.vTexUV);
	//Out.vNormal = tex2D(NormalSampler, In.vTexUV);
	Out.vDepth = vector(In.vProjPos.w / 500.0f, In.vProjPos.z / In.vProjPos.w, 0.f, 1.f);

	//Out.vDiffuse = Out.vDiffuse * g_vMtrlDiffuse;
	//Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	//Out.vAmbient = g_vMtrlAmbient;
	//Out.vAmbient.a = 1.f;
	//Out.vNormal.w = 1.f;
	//Out.vDepth = vector(In.vProjPos.w / 500.0f, In.vProjPos.z / In.vProjPos.w, 0.f, 1.f);

	return Out;
}

technique Default_Technique
{
	pass Default_Rendering
	{
		alphatestenable = true;
		alphafunc = Greater;
		alpharef = 0xc0;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}	

	//pass Default_RenderingSpecular
	//{
	//	//alphablendenable = true;
	//	//srcblend = srcalpha;
	//	//destblend = invsrcalpha;
	//	alphatestenable = true;
	//	alphafunc = Greater;
	//	alpharef = 0xc0;

	//	VertexShader = compile vs_3_0 VS_MAIN();
	//	PixelShader = compile ps_3_0 PS_MAINSPECULAR();
	//}

	//pass Default_RenderingNoSpecular
	//{
	//	//alphablendenable = true;
	//	//srcblend = srcalpha;
	//	//destblend = invsrcalpha;
	//	alphatestenable = true;
	//	alphafunc = Greater;
	//	alpharef = 0xc0;

	//	VertexShader = compile vs_3_0 VS_MAIN();
	//	PixelShader = compile ps_3_0 PS_MAINNOSPECULAR();
	//}
}