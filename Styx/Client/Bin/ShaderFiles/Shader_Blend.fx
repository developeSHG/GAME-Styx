
texture			g_DiffuseTexture;

sampler DiffuseSampler = sampler_state
{
	//minfilter = linear;
	//magfilter = linear;
	//mipfilter = linear;
	texture = g_DiffuseTexture;
};

texture			g_ShadeTexture; 

sampler ShadeSampler = sampler_state
{
	//minfilter = linear;
	//magfilter = linear;
	//mipfilter = linear;
	texture = g_ShadeTexture;
};

texture			g_SpecularTexture;

sampler SpecularSampler = sampler_state
{
	//minfilter = linear;
	//magfilter = linear;
	//mipfilter = linear;
	texture = g_SpecularTexture;
};

texture			g_DepthTexture; // Target_Normal

sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;
};

struct PS_IN
{
	float4		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;

	vector	vDiffuse = tex2D(DiffuseSampler, In.vTexUV);
	vector	vShade = tex2D(ShadeSampler, In.vTexUV);	
	vector	vSpecular = tex2D(SpecularSampler, In.vTexUV);
	vector	vDepthInfo = tex2D(DepthSampler, In.vTexUV);

	Out.vColor = vDiffuse * vShade + vSpecular;

	if (vDepthInfo.w == 0.5f)
		Out.vColor.a = 0.2f;

	return Out;
}

technique Default_Technique
{                           
	pass Default_Rendering
	{
		AlphaTestEnable = true;
		AlphaRef = 0;
		AlphaFunc = Greater;

		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_MAIN();
	}
}