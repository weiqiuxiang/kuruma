#ifndef _GAMEOVER_H_
#define _GAMEOVER_H_
//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include <d3dx9.h>

//-----------------------------------------------------------------------------------------
//頂点フォーマット
//-----------------------------------------------------------------------------------------
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define NUM_GAMEOVERVERTEX (4)                      //頂点数
#define NUM_GAMEOVERPOLYGON (5)						//矩形数
#define NUM_GAMEOVERTEXTURE (5)                     //テクスチャ数

#define GAMEOVER_WIDTH (256.0f)
#define GAMEOVER_HEIGHT (64.0f)

//-----------------------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------------------
#define GAMEOVERPOLYGON01_TEXTURENAME "data/TEXTURE/sozai/GameOverFont.png"
#define PAUSEPOLYGON01_TEXTURENAME "data/TEXTURE/sozai/pause.png"
#define PAUSEPOLYGON02_TEXTURENAME "data/TEXTURE/sozai/pause_frame.png"
#define PAUSEPOLYGON03_TEXTURENAME "data/TEXTURE/sozai/pause_font.png"
#define PAUSEPOLYGON04_TEXTURENAME "data/TEXTURE/sozai/pause_font_color.png"

//-----------------------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------------------
HRESULT InitGameover(void);                                                    //初期化関数
void DrawGameover(void);                                                       //描画関数
void UpdateGameover(void);                                                     //更新関数
void UninitGameover(void);                                                     //メモリ解放
void MakeVertexPolygonGameover(LPDIRECT3DDEVICE9 pDevice,VERTEX_2D *pVtx);     //ポリゴン設定

#endif