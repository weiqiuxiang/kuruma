//-----------------------------------------------------------------------------------------
//プレーヤーヘッドファイル[enemy_running02.h]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------------------
#ifndef _BULLET_H_
#define _BULLET_H_

#include <d3dx9.h>
//-----------------------------------------------------------------------------------------
//頂点フォーマット
//-----------------------------------------------------------------------------------------
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define MAX_BULLET (256)
#define NUM_BULLETVERTEX (4)                          //頂点数
#define NUM_BULLETTEXTURE (1)                         //テクスチャ数

#define BULLET_WIDTH (24.0f)
#define BULLET_HEIGHT (24.0f)
#define CHECK_RADIUS (6.0f)                           //当り判定の半径
#define BULLET_DISTANCEX (50.0f)
#define BULLET_DISTANCEY (25.0f)

//-----------------------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------------------
#define BULLETPOLYGON01_TEXTURENAME "data/TEXTURE/sozai/bullet.png"

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
}BULLET;

typedef struct
{
	//弾の当り判定の上下左右座標
	float Up;
	float Bottom;
	float Left;
	float Right;
}BULLET_HITCHECK;

//-----------------------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------------------
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void UpdatePolygonBullet(void);
void MakeVertexPolygonBullet(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx);
void SetBullet(float fPosX,float fPosY,float fSpeedX,float fSpeedY,int nLife,float fAngle);
void SpriteRorateBullet (BULLET Bbullet,VERTEX_2D *pVtx);                                              //回転用関数
//弾のデータをゲット
BULLET *GetBullet(void);
BULLET_HITCHECK *GetBulletHitcheck(void);

#endif
