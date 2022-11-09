#include "Shader_Base.hpp"

struct		VS_IN
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	vector		vPosition : POSITION;
	vector		vShade : COLOR0;
	float2		vTexUV : TEXCOORD0;
};

VS_OUT		VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);

	Out.vShade.a = 1.f;

	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	vector			vShade : COLOR0;
	float2			vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector			vColor : COLOR0;
};

PS_OUT		PS_MAIN_ALPHATEST(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor *= In.vShade + vector(1.f, 1.f, 1.f, 1.f);

	return Out;
}

PS_OUT		PS_MAIN_ALPHABLEND(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	return Out;
}

technique Default_Device
{
	pass	AlphaTesting
	{
		alphatestenable = true;
		alphafunc = Greater;
		alpharef = 0xc0;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN_ALPHATEST();
	}
	pass	AlphaBlending
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN_ALPHABLEND();
	}
	pass	AlphaBlendingBlack
	{
	alphablendenable = true;
	//blendopalpha = add;
	srcblend = srccolor;
	destblend = invsrccolor;

	vertexshader = compile vs_3_0 VS_MAIN();
	pixelshader = compile ps_3_0 PS_MAIN_ALPHABLEND();
	}
};

