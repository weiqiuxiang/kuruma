//-----------------------------------------------------------------------------------------
//パーティクルヘッドファイル[ParticalCenter.h]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

#include "main.h"

//-----------------------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------------------
#ifndef _PARTICALCENTER_H_
#define _PARTICALCENTER_H_

//-----------------------------------------------------------------------------------------
//頂点フォーマット
//-----------------------------------------------------------------------------------------
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define NUM_PARTICALCENTERVERTEX (4)                            //頂点数
#define NUM_PARTICALCENTERPOLYGON (128)						    //矩形数
#define NUM_PARTICALCENTERTEXTURE (2)                           //テクスチャ数

//パーティクルの幅高さ
#define PARTICALCENTER_WIDTH (20.0f)
#define PARTICALCENTER_HEIGHT (20.0f)

//パーティクルのエフェクトの半径
#define MAIN_RADIUS (20.0f)

//-----------------------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------------------
#define PARTICALCENTERPOLYGON01_TEXTURENAME "data/TEXTURE/sozai/partical.png"
#define PARTICALCENTERPOLYGON02_TEXTURENAME "data/TEXTURE/sozai/particalEffect.jpg"

//-----------------------------------------------------------------------------------------
//パーティクルの構造体
//-----------------------------------------------------------------------------------------
typedef struct
{
	int nEffectType;                               //エフェクトのタイプ
	D3DXVECTOR2 pos;                               //パーティクルのポジション
	D3DXVECTOR2 speed;                             //パーティクルのスピード
	D3DXVECTOR2 posOld;                            //パーティクルのポジション
	float fRangeAngel;                             //パーティクルの拡散角度の大きさ
	float fShootAngel;                             //パーティクルの発射角度
	float fRadius;                                 //パーティクルの半径
	float fDistance;                               //パーティクルの飛び基準距離
	bool bCamera;
	bool bUse;                                     //パーティクルを使うかどうか

	//パーティクル周囲環境
	float fGravity;                //重力
	float fWindPower;              //風力
	float fWindAngel;              //風が吹く角度
}PARTICALCENTER;

typedef struct
{
	D3DXVECTOR2 pos;                               //パーティクルのポジション
	D3DXVECTOR2 speed;                             //パーティクルのスピード
	D3DXVECTOR2 posOld;                            //パーティクルのポジション
	float fRadius;
	bool bUse;
}CENTER_EFFECT;

//パーティクルのエフェクト選択
typedef enum
{
	EFFECTMODE_BASE = 0,
	EFFECTMODE_EXPLOSION,
	EFFECTMODE_MAX,
}EFFECTMODE;

//-----------------------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------------------
HRESULT InitParticalCenter(void);
void UninitParticalCenter(void);
void UpdateParticalCenter(void);
void DrawParticalCenter(void);
void UpdatePolygonParticalCenter(void);
void MakeVertexPolygonParticalCenter(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx);
PARTICALCENTER *GetParticalCenter(void);

void SetParticalCenter(int nEffectType,float fposX,float fposY,float fRadius,float fDistance,bool bCamera);  //パーティクルセット

EFFECTMODE *GetEffectMode(void);

#endif
