#ifndef _POLYGON_H_
#define _POLYGON_H_
//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include <d3dx9.h>

//-----------------------------------------------------------------------------------------
//頂点フォーマット
//-----------------------------------------------------------------------------------------
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define NUM_BGVERTEX (4)                        //頂点数
#define NUM_BGPOLYGON (3)						//矩形数
#define NUM_BGTEXTURE (3)                       //テクスチャ数

#define BG_WIDTH (3840.0f)
#define BG_HEIGHT (640.0f)

//-----------------------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------------------
#define BGPOLYGON01_TEXTURENAME "data/TEXTURE/sozai/Background.png"
#define BGPOLYGON02_TEXTURENAME "data/TEXTURE/sozai/Background_sabun01.png"
#define BGPOLYGON03_TEXTURENAME "data/TEXTURE/sozai/Background_Sky.png"


//-----------------------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------------------
HRESULT InitBGPolygon(void);                                                     //ポリゴン(頂点)の初期化
void DrawBGPolygon(void);                                                        //ポリゴンの描画
void UpdateBGPolygon(void);                                                      //ポリゴンの更新
void UninitBGPolygon(void);                                                      //ポリゴンの終了
void MakeVertexPolygonBG(LPDIRECT3DDEVICE9 pDevice,VERTEX_2D *pVtx);           //背景ポリゴン設定

#endif