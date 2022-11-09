matrix  g_matWorld;		// 상수 테이블 : 클라이언트에서 얻어와 코드에 대입하기 위한 상징적인 명칭
matrix  g_matView;		// matrix == float4x4
matrix  g_matProj;

texture g_BaseTexture;	// 상수 테이블

sampler BaseSampler = sampler_state	// 여러 색상 정보를 담고 있는 팔테트 형태의 텍스쳐를 쉐이더에서는 sampler라고 부른다.
{									// = sampler_state는 샘퍼를 정의하여 상태를 정의했다고 표시하는 문법(반드시 있어야 함)
	texture = g_BaseTexture;
	
	minfilter = linear;
	magfilter = linear;
};

struct VS_IN
{
	vector		vPosition : POSITION;		// 대문자(Symantic) : 멤버의 속성을 지정하는 문법, vector == float4 같음

	float2		vTexUV	: TEXCOORD0;
};

struct VS_OUT
{
	vector		vPosition : POSITION;		

	float2		vTexUV : TEXCOORD0;
};

// 버텍스 쉐이딩
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

// 픽셀 쉐이딩

struct PS_IN	// 픽셀 쉐이더 구조체는 POSITION 값을 가질 수가 없다.
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

	Out.vColor = tex2D(BaseSampler, In.vTexUV);		// 2차원 형태의 텍스쳐로부터 색을 추출하여 얻어오는 함수(반환타입은 vector타입)

	Out.vColor.r = 0.5f;
	Out.vColor.g = 0.7f;

	return Out;
}

technique Default_Device			// 하나의 쉐이더 파일은 하나의 테크니크만 가질 수 있다. 대신 여러개의 pass를 가질 수도 있다.
{
	// 기능의 캡슐화
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
