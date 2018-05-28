#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_
//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include <d3dx9.h>

//-----------------------------------------------------------------------------------------
//頂点フォーマット
//-----------------------------------------------------------------------------------------
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define NUM_TUTORIALVERTEX (4)                      //頂点数
#define NUM_TUTORIALPOLYGON (2)						//矩形数
#define NUM_TUTORIALTEXTURE (3)                     //テクスチャ数

//-----------------------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------------------
#define TUTORIALPOLYGON01_TEXTURENAME "data/TEXTURE/sozai/tutorial01.png"
#define TUTORIALPOLYGON02_TEXTURENAME "data/TEXTURE/sozai/tutorial02.png"
#define TUTORIALPOLYGON03_TEXTURENAME "data/TEXTURE/sozai/tutorial_fade.png"

//-----------------------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------------------
HRESULT InitTutorial(void);                                                     //ポリゴン(頂点)の初期化
void DrawTutorial(void);                                                        //ポリゴンの描画
void UpdateTutorial(void);                                                      //ポリゴンの更新
void UpdateTutorialPolygon(void);                                               //ポリゴンの更新
void UninitTutorial(void);                                               //ポリゴンの終了
void MakeVertexPolygonTutorial(LPDIRECT3DDEVICE9 pDevice,VERTEX_2D *pVtx);      //背景ポリゴン設定

#endif