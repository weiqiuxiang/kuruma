#ifndef _RESULT_H_
#define _RESULT_H_
//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include <d3dx9.h>

//-----------------------------------------------------------------------------------------
//頂点フォーマット
//-----------------------------------------------------------------------------------------
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define NUM_RESULTVERTEX (4)                     //頂点数
#define RESULT_POSX (280)
#define RESULT_POSY (120)
#define WIDTH_RESULT (225)                       //タイトルの幅
#define HEIGHT_RESULT (64)                      //タイトルの高さ
#define NUM_RESULTTEXTURE (4)                    //テクスチャ数
#define NUM_RESULTPOLYGON (10)					 //矩形数

//-----------------------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------------------
#define RESULTPOLYGON_TEXTURENAME "data/TEXTURE/sozai/result.png"
#define RESULTPOLYGON02_TEXTURENAME "data/TEXTURE/sozai/score.png"
#define RESULTPOLYGON03_TEXTURENAME "data/TEXTURE/sozai/score_font.png"
#define RESULTPOLYGON04_TEXTURENAME "data/TEXTURE/sozai/normal_bg.png"

//-----------------------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------------------
HRESULT InitResult(void);                                                      //ポリゴン(頂点)の初期化
void DrawResult(void);                                                         //ポリゴンの描画
void UpdateResult(void);                                                       //ポリゴンの更新
void UninitResult(void);                                                        //ポリゴンの終了
void MakeVertexPolygonResult(LPDIRECT3DDEVICE9 pDevice,VERTEX_2D *pVtx);              //背景ポリゴン設定

#endif