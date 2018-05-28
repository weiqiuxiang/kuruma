#ifndef _SCORE_H_
#define _SCORE_H_
//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include <d3dx9.h>

//-----------------------------------------------------------------------------------------
//頂点フォーマット
//-----------------------------------------------------------------------------------------
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define NUM_SCOREVERTEX (4)                     //頂点数
#define POS_SCORE_X (100)                       //スコアの初期X座標
#define POS_SCORE_Y (20)                       //スコアの初期Y座標
#define WIDTH_SCORE (12)                       //スコアの1桁の幅
#define HEIGHT_SCORE (24)                      //スコアの1桁の高さ
#define SCORE_INTERVAL (12)                    //桁と桁の間隔
#define WIDTH_FONT (80)
#define HEIGHT_FONT (24)
#define DISTANCE_FROM_SCORE (95)

//-----------------------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------------------
#define SCOREPOLYGON_TEXTURENAME "data/TEXTURE/sozai/score.png"
#define SCOREPOLYGON02_TEXTURENAME "data/TEXTURE/sozai/score_font.png"

//-----------------------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------------------
HRESULT InitScorePolygon(void);                                                      //ポリゴン(頂点)の初期化
void DrawScorePolygon(void);                                                         //ポリゴンの描画
void UpdateScorePolygon(void);                                                       //ポリゴンの更新
void UninitScorePolygon(void);                                                       //ポリゴンの終了
void MakeVertexPolygonScore(LPDIRECT3DDEVICE9 pDevice,VERTEX_2D *pVtx);              //背景ポリゴン設定

#endif