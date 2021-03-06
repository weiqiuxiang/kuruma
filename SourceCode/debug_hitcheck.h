#ifdef _DEBUG
#ifndef _HITCHECK_H_
#define _HITCHECK_H_
//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include <d3dx9.h>

//-----------------------------------------------------------------------------------------
//頂点フォーマット
//-----------------------------------------------------------------------------------------
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define NUM_HITCHECKVERTEX (4)                        //頂点数
#define NUM_HITCHECKPOLYGON (4096)					  //矩形数

#define HITCHECKPOLYGON01_TEXTURENAME "data/TEXTURE/sozai/checkbox.png"

//-----------------------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//当り判定範囲チェックの定義
//-----------------------------------------------------------------------------------------
typedef struct
{   
	//上下左右の座標
	float Up;
	float Bottom;
	float Left;
	float Right;
	bool bUse;               //使っているか
}HITCHECK;

//-----------------------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------------------
HRESULT InitHitcheck(void);                                                     //ポリゴン(頂点)の初期化
void DrawHitcheck(void);                                                        //ポリゴンの描画
void UpdateHitcheck(void);                                                      //ポリゴンの更新
void UninitHitcheck(void);                                                      //ポリゴンの終了
void SetHitCheck(float fPosX,float fPosY,float fWidth,float fHeight);           //チェックボックスの設置

#endif
#endif