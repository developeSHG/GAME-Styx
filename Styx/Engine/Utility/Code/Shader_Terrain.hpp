#include "Shader_Base.hpp"

float		g_fDetail;

vector		g_vLightDir = (vector)0.f;

vector		g_vLightDiffuse;
vector		g_vLightAmbient;

vector		g_vMtrlDiffuse;
vector		g_vMtrlAmbient;

struct		VS_IN
{
	vector		vPosition : POSITION;
	vector		vNormal : NORMAL;
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
	//Out.vNormal = mul(vector(In.vNormal.xyz, 0.f), matWVP);

	vector vWorldNormal = mul(vector(In.vNormal.xyz, 0.f), g_matWorld);
	vector vWorldLight = g_vLightDir * -1.f;

	float	fIntensity = saturate(dot(normalize(vWorldNormal), normalize(vWorldLight)));
	//float	fIntensity = max(dot(normalize(vWorldNormal), normalize(vWorldLight)), 0.f);

	//ºûÀÇ ¼¼±â * (L.D * M.D) + (L.A * M.A) + (½ºÆåÅ§·¯ÀÇ ¼¼±â * (L.S * M.S))

	Out.vShade = fIntensity;
	Out.vShade.a = 1.f;

	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	vector		vShade : COLOR0;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector			vColor : COLOR0;
};

PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)Out;

	vector vColor = tex2D(BaseSampler, In.vTexUV * g_fDetail);

	//Out.vColor = (vColor * In.vShade) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient);

	Out.vColor = vColor * (g_vLightDiffuse * g_vMtrlDiffuse) * (In.vShade + (g_vLightAmbient * g_vMtrlAmbient));

	// Out.vColor = vColor * (g_vLightDiffuse * g_vMtrlDiffuse) + (In.vShade * (g_vLightAmbient * g_vMtrlAmbient));

	return Out;
}

technique Default_Device
{
	// ±â´ÉÀÇ Ä¸½¶È­
	pass
	{
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}
};
