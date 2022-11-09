
matrix		g_matWorld, g_matView, g_matProj;
texture		g_DiffuseTexture;


sampler DiffuseSampler = sampler_state
{
	//MinFilter = linear;
	//MagFilter = linear;
	//MipFilter = linear;
	Texture = g_DiffuseTexture;
};

struct VS_IN
{
	float3		vPosition : POSITION;	
	float3		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : POSITION;	
	float3		vTexUV : TEXCOORD0;
};


VS_OUT VS_MAIN(VS_IN In) 
{
	VS_OUT	Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);	
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	float4		vPosition : POSITION;	
	float3		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	float4		vColor : COLOR;
};

PS_OUT PS_MAIN(VS_OUT In)
{
	PS_OUT	Out = (PS_OUT)0;

	Out.vColor = texCUBE(DiffuseSampler, In.vTexUV);	

	return Out;
}

technique Default_Technique
{	
	pass Default_Rendering
	{	
		ZWriteEnable = false;
		CullMode = cw;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}	
}