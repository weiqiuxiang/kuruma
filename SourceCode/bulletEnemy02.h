//-----------------------------------------------------------------------------------------
//プレーヤーヘッドファイル[enemy_running02.h]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------------------
#ifndef _BULLETENEMY02_H_
#define _BULLETENEMY02_H_

#include <d3dx9.h>
//-----------------------------------------------------------------------------------------
//頂点フォーマット
//-----------------------------------------------------------------------------------------
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define MAX_BULLETENEMY02 (256)
#define NUM_BULLETENEMY02VERTEX (4)                          //頂点数
#define NUM_BULLETENEMY02TEXTURE (1)                         //テクスチャ数

#define BULLETENEMY02_WIDTH (24.0f)
#define BULLETENEMY02_HEIGHT (24.0f)

//-----------------------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------------------
#define BULLETENEMY02POLYGON01_TEXTURENAME "data/TEXTURE/sozai/BulletEnemy02.png"

//-----------------------------------------------------------------------------------------
//プレーヤーの構造体
//-----------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR2 pos;         //座標
	D3DXVECTOR2 speed;       //移動量(速度)
	D3DXCOLOR color;         //カラー
	int nLife;               //寿命
	bool bUse;               //使用フラグ
	float fAngle;            //弾の角度
	float fRadius;           //弾の半径
	int nUnit;               //どの敵が弾を発射撃つ(違い敵の弾の動作が違い)
}BULLETENEMY02;

//-----------------------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------------------
HRESULT InitBulletEnemy02(void);
void UninitBulletEnemy02(void);
void UpdateBulletEnemy02(void);
void DrawBulletEnemy02(void);
void UpdatePolygonBulletEnemy02(void);
void MakeVertexPolygonBulletEnemy02(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx);
void SetBulletEnemy02(int nUnit,float fPosX,float fPosY,float fSpeedX,float fSpeedY,int nLife,float fAngle,float fRadius);
BULLETENEMY02 *GetBulletEnemy02(void);

#endif
