#ifndef Struct_h__
#define Struct_h__

typedef struct tagFeature
{
	Engine::MESHTYPE	eMeshType;
	_tchar				szMeshTag[256];
	_bool				bCatsleIn = TRUE;
	_vec3				vScale;
	_vec3				vRotation;
	_vec3				vPosition;
}FEATURE;

typedef struct tagNaviCell
{
	Engine::NAVI_TYPE   eNaviType;
	_vec3				vPointA;
	_vec3				vPointB;
	_vec3				vPointC;
}NAVICELL;

typedef struct tagSpline
{
	_vec3				vPos;
	_vec3				vVirtualStartPos;
	_vec3				vVirtualEndPos;
	_float				fSpeed;
	_float				fIncSpeed;
}SPLINE;

typedef struct tagLight
{
	_vec3				vPosition;
	D3DCOLORVALUE		tDiffuse;
	D3DCOLORVALUE		tSpecular;
	D3DCOLORVALUE		tAmbient;
	_float				fRange;
}LIGHT;

typedef struct tagAniCtrlInfo
{
	_float				fTrackSpeed = 1.f;
	_float				fWeight = 0.1f;
	_float				fDuration = 0.25f;
}ANICTRLINFO;



#endif // Struct_h__
