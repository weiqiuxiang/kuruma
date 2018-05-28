#ifndef _TITLE_H_
#define _TITLE_H_
//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include <d3dx9.h>

//-----------------------------------------------------------------------------------------
//頂点フォーマット
//-----------------------------------------------------------------------------------------
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define NUM_TITLEVERTEX (4)                     //頂点数
#define NUM_TITLETEXTURE (3)                    //テクスチャ数
#define NUM_TITLEPOLYGON (3)					//矩形数

//タイトルの幅、高さ
#define WIDTH_TITLE (640) 
#define HEIGHT_TITLE (640)  

//選択カーソルの幅、高さ
#define WIDTH_CURSOR (128)
#define HEIGHT_CURSOR (64)
#define ICON_POSX (239)
#define ICON_POSY (398)
#define DISTANCE_FROM_START (130)                   //文字STARTとの距離
#define DISTANCE_FROM_EXIT (130)                    //文字EXITとの距離

//startとexitの位置
#define START_POSX (239)
#define START_POSY (398)
#define EXIT_POSX (267)
#define EXIT_POSY (502)

//-----------------------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------------------
#define TITLEPOLYGON00_TEXTURENAME "data/TEXTURE/sozai/title.png"
#define TITLEPOLYGON01_TEXTURENAME "data/TEXTURE/sozai/SelectIcon.png"
#define TITLEPOLYGON02_TEXTURENAME "data/TEXTURE/sozai/normal_bg.png"

//-----------------------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------------------
HRESULT InitTitle(void);                                                      //ポリゴン(頂点)の初期化
void DrawTitle(void);                                                         //ポリゴンの描画
void UpdateTitle(void);                                                       //ポリゴンの更新
void UninitTitle(void);                                                       //ポリゴンの終了
void MakeVertexPolygonTitle(LPDIRECT3DDEVICE9 pDevice,VERTEX_2D *pVtx);              //背景ポリゴン設定

#endif