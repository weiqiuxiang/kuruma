//-----------------------------------------------------------------------------------------
//プレーヤーヘッドファイル[enemy_running02.h]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------------------
#ifndef _PARTICAL_H_
#define _PARTICAL_H_

#include <d3dx9.h>
//-----------------------------------------------------------------------------------------
//頂点フォーマット
//-----------------------------------------------------------------------------------------
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define MAX_PARTICAL (4096)
#define NUM_PARTICALVERTEX (4)                          //頂点数
#define NUM_PARTICALTEXTURE (1)                         //テクスチャ数

//-----------------------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------------------
#define PARTICALPOLYGON01_TEXTURENAME "data/TEXTURE/sozai/partical.png"

//-----------------------------------------------------------------------------------------
//プレーヤーの構造体
//-----------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR2 pos;         //座標
	D3DXVECTOR2 posOld;
	D3DXVECTOR2 speed;       //移動量(速度)
	D3DXCOLOR color;         //カラー
	float fRadius;           //半径
	float fColorAlphaValue;  //色(α)変化量
	int nLife;               //寿命
	int nAlpha;              //α値
	bool bCamera;            //カメラと関係あるか
	bool bUse;               //使用フラグ
}PARTICAL;

//-----------------------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------------------
HRESULT InitPartical(void);
void UninitPartical(void);
void UpdatePartical(void);
void DrawPartical(void);
void UpdatePolygonPartical(void);
void MakeVertexPolygonPartical(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx);
void SetPartical(float fPosX,float fPosY,float fSpeedX,float fSpeedY,float fRadius,float nLife,bool bCamera);
PARTICAL *GetPartical(void);

#endif
