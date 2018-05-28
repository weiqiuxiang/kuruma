#ifndef _FADE_H_
#define _FADE_H_
//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include <d3dx9.h>

//-----------------------------------------------------------------------------------------
//頂点フォーマット
//-----------------------------------------------------------------------------------------
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define NUM_FADEVERTEX (4)                        //頂点数
#define NUM_FADEPOLYGON (1)						//矩形数
#define NUM_FADETEXTURE (1)                       //テクスチャ数

#define FADE_WIDTH (1000.0f)
#define FADE_HEIGHT (800.0f)
#define FADE_RATE (5)                 //faderateを定義

//-----------------------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------------------
#define FADEPOLYGON01_TEXTURENAME "data/TEXTURE/sozai/fade.png"

//-----------------------------------------------------------------------------------------
//フェードモードの定義
//-----------------------------------------------------------------------------------------
typedef enum
{
	FADE_NONE = 0,
	FADE_IN,
	FADE_OUT,
	FADE_MAX,
}FADE;

typedef struct
{
	D3DXVECTOR3 pos;                         //
	float rhw;								 //座標変換済み頂点フラグ(必ず1.0f)
	D3DCOLOR color;						 //頂点カラー
	D3DXVECTOR2 tex;                         //テクスチャ座標(UV or ST)
}VERTEX_2DF;

//-----------------------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------------------
HRESULT InitFade(void);                                                     //ポリゴン(頂点)の初期化
void DrawFade(void);                                                        //ポリゴンの描画
void UpdateFade(void);                                                      //ポリゴンの更新
void UninitFade(void);                                                      //ポリゴンの終了
void MakeVertexPolygonFade(LPDIRECT3DDEVICE9 pDevice,VERTEX_2DF *pVtx);           //背景ポリゴン設定
void SetFade(FADE fade,MODE modeNext);                                      //フェード設定
FADE *GetFade(void);

#endif