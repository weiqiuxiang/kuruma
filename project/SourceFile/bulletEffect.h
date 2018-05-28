//-----------------------------------------------------------------------------------------
//プレーヤーヘッドファイル[enemy_running02.h]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------------------
#ifndef _BULLETEFFECT_H_
#define _BULLETEFFECT_H_

#include <d3dx9.h>
//-----------------------------------------------------------------------------------------
//頂点フォーマット
//-----------------------------------------------------------------------------------------
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define MAX_BULLETEFFECT (4096)
#define NUM_BULLETEFFECTVERTEX (4)                          //頂点数
#define NUM_BULLETEFFECTTEXTURE (1)                         //テクスチャ数

#define BULLETEFFECT_WIDTH (24.0f)
#define BULLETEFFECT_HEIGHT (24.0f)
#define BULLETEFFECT_DISTANCEX (50.0f)
#define BULLETEFFECT_DISTANCEY (25.0f)

#define RADIUS (12.0f)
#define BULLETLIFE (10.0f)

//-----------------------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------------------
#define BULLETEFFECTPOLYGON01_TEXTURENAME "data/TEXTURE/sozai/bulletEffect.png"

//-----------------------------------------------------------------------------------------
//プレーヤーの構造体
//-----------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR2 pos;         //ポリゴンの中心座標
	D3DCOLOR color;         //カラー
	float fRadius;           //半径
	float fRaidusValue;      //半径の変化量
	float fColorAlphaValue;  //色(α)変化量
	int nLife;               //寿命
	int nAlpha;              //α値
	bool bUse;               //使用フラグ
	float fAngle;            //弾の角度
}BULLETEFFECT;

//-----------------------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------------------
HRESULT InitBulletEffect(void);
void UninitBulletEffect(void);
void UpdateBulletEffect(void);
void DrawBulletEffect(void);
void UpdatePolygonBulletEffect(void);
void MakeVertexPolygonBulletEffect(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx);
void SetBulletEffect(float fPosX,float fPosY,int nLife,float fAngle);
void SpriteRorateBulletEffect (BULLETEFFECT BBulletEffect,VERTEX_2D *pVtx);                                              //回転用関数
BULLETEFFECT *GetBulletEffect(void);

#endif
