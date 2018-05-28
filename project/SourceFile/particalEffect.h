//-----------------------------------------------------------------------------------------
//プレーヤーヘッドファイル[enemy_running02.h]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------------------
#ifndef _PARTICALEFFECT_H_
#define _PARTICALEFFECT_H_

#include <d3dx9.h>
//-----------------------------------------------------------------------------------------
//頂点フォーマット
//-----------------------------------------------------------------------------------------
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define MAX_PARTICALEFFECT (4096)
#define NUM_PARTICALEFFECTVERTEX (4)                          //頂点数
#define NUM_PARTICALEFFECTTEXTURE (1)                         //テクスチャ数

//-----------------------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------------------
#define ParticalEffectPOLYGON01_TEXTURENAME "data/TEXTURE/sozai/particalEffect.jpg"

//-----------------------------------------------------------------------------------------
//プレーヤーの構造体
//-----------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR2 pos;         //中心座標
	D3DXVECTOR2 posOld;      //
	D3DXVECTOR2 speed;       //速度
	D3DCOLOR color;          //カラー
	float fRadius;           //半径
	float fColorAlphaValue;  //色(α)変化量
	int nLife;               //寿命
	int nAlpha;              //α値
	bool bCamera;             //
	bool bUse;               //使用フラグ
}PARTICALEFFECT;

//-----------------------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------------------
HRESULT InitParticalEffect(void);
void UninitParticalEffect(void);
void UpdateParticalEffect(void);
void DrawParticalEffect(void);
void UpdatePolygonParticalEffect(void);
void MakeVertexPolygonParticalEffect(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx);
void SetParticalEffect(float fPosX,float fPosY,float fSpeedX,float fSpeedY,float fRadius,int nLife,bool bCamera);
void SpriteRorateParticalEffect (PARTICALEFFECT PParticalEffect,VERTEX_2D *pVtx);                                              //回転用関数
PARTICALEFFECT *GetParticalEffect(void);

#endif
