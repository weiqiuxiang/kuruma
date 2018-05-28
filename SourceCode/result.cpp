//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include <Windows.h>
#include <string.h>
#include <stdio.h>
#include "main.h"
#include "result.h"
#include "player.h"
#include "input.h"
#include "fade.h"

//-----------------------------------------------------------------------------------------
//マクロ
//-----------------------------------------------------------------------------------------
#define BIG (2)                         //拡大倍数
#define RESULT_POS_SCORE_X (430.0f)
#define RESULT_POS_SCORE_Y (250.0f)
#define RESULT_SCORE_WIDTH (12.0f*BIG)
#define RESULT_SCORE_HEIGHT (24.0f*BIG)
#define RESULT_SCORE_INTERVAL (12.0f*BIG)

#define RESULT_FONT_WIDTH (80.0f*BIG)
#define RESULT_FONT_HEIGHT (24.0f*BIG)
#define RESULT_DISTANCE_FROM_SCORE (125.0f*BIG)

//-----------------------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureResultPolygon[NUM_RESULTTEXTURE] = {};           //テクスチャを操作するためのポインタ(テクスチャバッファインターフェイスにアクセスためのポインタ)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferResultPolygon = NULL;    //頂点バッファを操作するためのポインタ(頂点バッファインターフェイスにアクセスためのポインタ)

////////////////////////////////////////////////
//ポリゴン(頂点)の初期化
////////////////////////////////////////////////
HRESULT InitResult(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_2D *pVtx = NULL;                           //頂点構造体のポインタ
	pD3DDevice = Get_Device();

	//頂点バッファの生成
	if(FAILED(pD3DDevice -> CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_RESULTVERTEX*NUM_RESULTPOLYGON,D3DUSAGE_WRITEONLY,FVF_VERTEX2D,D3DPOOL_MANAGED,&g_pVtxBufferResultPolygon,NULL)))
	{
		return E_FAIL;
	}

	//resultテクスチャの取得(3枚)
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, RESULTPOLYGON_TEXTURENAME, &g_pTextureResultPolygon[0])))
	{
		MessageBox(NULL, "テクスチャの読み込みが失敗しました", "エラー", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, RESULTPOLYGON02_TEXTURENAME, &g_pTextureResultPolygon[1])))
	{
		MessageBox(NULL, "テクスチャの読み込みが失敗しました", "エラー", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, RESULTPOLYGON03_TEXTURENAME, &g_pTextureResultPolygon[2])))
	{
		MessageBox(NULL, "テクスチャの読み込みが失敗しました", "エラー", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, RESULTPOLYGON04_TEXTURENAME, &g_pTextureResultPolygon[3])))
	{
		MessageBox(NULL, "テクスチャの読み込みが失敗しました", "エラー", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	//スコアのポリゴン
	g_pVtxBufferResultPolygon -> Lock(0,0,(void**)&pVtx,0);                    //仮想アドレスを取得しバッファをロックする
	MakeVertexPolygonResult(pD3DDevice,&pVtx[0]);

	pVtx[0].pos = D3DXVECTOR3(275,200,0.0f);
	pVtx[1].pos = D3DXVECTOR3(275+WIDTH_RESULT,200,0.0f);
	pVtx[2].pos = D3DXVECTOR3(275,200+HEIGHT_RESULT,0.0f);
	pVtx[3].pos = D3DXVECTOR3(275+WIDTH_RESULT,200+HEIGHT_RESULT,0.0f);

	pVtx += 4;

	pVtx[0].pos = D3DXVECTOR3(0,0,0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH,0,0.0f);
	pVtx[2].pos = D3DXVECTOR3(0,SCREEN_HEIGHT,0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH,SCREEN_HEIGHT,0.0f);

	g_pVtxBufferResultPolygon -> Unlock();                                     //バッファのアンロック

	return S_OK;
}

//ポリゴンの更新
void UpdateResult(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice = Get_Device();
	VERTEX_2D *pVtx = NULL;                           //頂点構造体のポインタ
	PLAYER *pPlayer = GetPlayer();                    //プレーヤーのステータスの取得

	sprintf( &(pPlayer -> aScore[0]),"%f",pPlayer -> fScore);
	pPlayer -> aScore[9] = '\0';

	char aScore[20];
	int nDigit = 0;                                   //桁数
	int score;                                        //スコアを格納する変数
	strcpy(&aScore[0],&pPlayer -> aScore[0]);

	for(;nDigit < 8;nDigit++)
	{
		if(aScore[nDigit] == 0x2e) break;
	}

	//resultポリゴンの表示
	g_pVtxBufferResultPolygon->Lock(0, 0, (void**)&pVtx, 0);       //仮想アドレスを取得しバッファをロックする

	MakeVertexPolygonResult(pD3DDevice,&pVtx[0]);

	//----------------
	//リザルトフォント
	//----------------
	pVtx[0].pos = D3DXVECTOR3(RESULT_POSX,RESULT_POSY,0.0f);
	pVtx[1].pos = D3DXVECTOR3(RESULT_POSX+WIDTH_RESULT,RESULT_POSY,0.0f);
	pVtx[2].pos = D3DXVECTOR3(RESULT_POSX,RESULT_POSY+HEIGHT_RESULT,0.0f);
	pVtx[3].pos = D3DXVECTOR3(RESULT_POSX+WIDTH_RESULT,RESULT_POSY+HEIGHT_RESULT,0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[1].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[2].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[3].color = D3DCOLOR_RGBA(255,255,255,255);

	//ポリゴン描画する前にテクスチャをセット
	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);
	
	pVtx += 4;

	//----------------------
	//スコアのパラメータ調整
	//----------------------
	for(int nCnt = NUM_SCOREDIGIT - 1; nCnt >= 0; nCnt--)
	{
		//スコアの
		pVtx[nCnt*4+0].pos = D3DXVECTOR3(RESULT_POS_SCORE_X+nCnt*RESULT_SCORE_INTERVAL,RESULT_POS_SCORE_Y,0.0f);
		pVtx[nCnt*4+1].pos = D3DXVECTOR3(RESULT_POS_SCORE_X+RESULT_SCORE_WIDTH+nCnt*RESULT_SCORE_INTERVAL,RESULT_POS_SCORE_Y,0.0f);
		pVtx[nCnt*4+2].pos = D3DXVECTOR3(RESULT_POS_SCORE_X+nCnt*RESULT_SCORE_INTERVAL,RESULT_POS_SCORE_Y+RESULT_SCORE_HEIGHT,0.0f);
		pVtx[nCnt*4+3].pos = D3DXVECTOR3(RESULT_POS_SCORE_X+RESULT_SCORE_WIDTH+nCnt*RESULT_SCORE_INTERVAL,RESULT_POS_SCORE_Y+RESULT_SCORE_HEIGHT,0.0f);

		//スコアの桁数分が0になるかどうか
		if(nDigit > 0)
		{
			score = aScore[nDigit-1] - 48;
		}

		else
		{
			score = 0;
		}

		pVtx[nCnt*4+0].tex = D3DXVECTOR2(0.1*score,0.0f);
		pVtx[nCnt*4+1].tex = D3DXVECTOR2(0.1*score+0.1f,0.0f);
		pVtx[nCnt*4+2].tex = D3DXVECTOR2(0.1*score,1.0f);
		pVtx[nCnt*4+3].tex = D3DXVECTOR2(0.1*score+0.1f,1.0f);

		pVtx[nCnt*4+0].rhw = 1.0f;
		pVtx[nCnt*4+1].rhw = 1.0f;
		pVtx[nCnt*4+2].rhw = 1.0f;
		pVtx[nCnt*4+3].rhw = 1.0f;

		pVtx[nCnt*4+0].color = D3DCOLOR_RGBA(255,255,255,255);
		pVtx[nCnt*4+1].color = D3DCOLOR_RGBA(255,255,255,255);
		pVtx[nCnt*4+2].color = D3DCOLOR_RGBA(255,255,255,255);
		pVtx[nCnt*4+3].color = D3DCOLOR_RGBA(255,255,255,255);

		nDigit--;
	}

	//----------------------
	//スコアフォント
	//----------------------
	pVtx[NUM_SCOREDIGIT*4+0].pos = D3DXVECTOR3(RESULT_POS_SCORE_X-RESULT_DISTANCE_FROM_SCORE,RESULT_POS_SCORE_Y,0.0f);
	pVtx[NUM_SCOREDIGIT*4+1].pos = D3DXVECTOR3(RESULT_POS_SCORE_X-RESULT_DISTANCE_FROM_SCORE+RESULT_FONT_WIDTH,RESULT_POS_SCORE_Y,0.0f);
	pVtx[NUM_SCOREDIGIT*4+2].pos = D3DXVECTOR3(RESULT_POS_SCORE_X-RESULT_DISTANCE_FROM_SCORE,RESULT_POS_SCORE_Y+RESULT_FONT_HEIGHT,0.0f);
	pVtx[NUM_SCOREDIGIT*4+3].pos = D3DXVECTOR3(RESULT_POS_SCORE_X-RESULT_DISTANCE_FROM_SCORE+RESULT_FONT_WIDTH,RESULT_POS_SCORE_Y+RESULT_FONT_HEIGHT,0.0f);

	pVtx[NUM_SCOREDIGIT*4+0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[NUM_SCOREDIGIT*4+1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[NUM_SCOREDIGIT*4+2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[NUM_SCOREDIGIT*4+3].tex = D3DXVECTOR2(1.0f,1.0f);

	pVtx[NUM_SCOREDIGIT*4+0].rhw = 1.0f;
	pVtx[NUM_SCOREDIGIT*4+1].rhw = 1.0f;
	pVtx[NUM_SCOREDIGIT*4+2].rhw = 1.0f;
	pVtx[NUM_SCOREDIGIT*4+3].rhw = 1.0f;

	pVtx[NUM_SCOREDIGIT*4+0].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[NUM_SCOREDIGIT*4+1].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[NUM_SCOREDIGIT*4+2].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[NUM_SCOREDIGIT*4+3].color = D3DCOLOR_RGBA(255,255,255,255);

	//背景
	pVtx[(NUM_SCOREDIGIT+1)*4+0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[(NUM_SCOREDIGIT+1)*4+1].pos = D3DXVECTOR3(SCREEN_WIDTH,0.0f,0.0f);
	pVtx[(NUM_SCOREDIGIT+1)*4+2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT,0.0f);
	pVtx[(NUM_SCOREDIGIT+1)*4+3].pos = D3DXVECTOR3(SCREEN_WIDTH,SCREEN_HEIGHT,0.0f);

	pVtx[(NUM_SCOREDIGIT+1)*4+0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[(NUM_SCOREDIGIT+1)*4+1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[(NUM_SCOREDIGIT+1)*4+2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[(NUM_SCOREDIGIT+1)*4+3].tex = D3DXVECTOR2(1.0f,1.0f);

	pVtx[(NUM_SCOREDIGIT+1)*4+0].rhw = 1.0f;
	pVtx[(NUM_SCOREDIGIT+1)*4+1].rhw = 1.0f;
	pVtx[(NUM_SCOREDIGIT+1)*4+2].rhw = 1.0f;
	pVtx[(NUM_SCOREDIGIT+1)*4+3].rhw = 1.0f;

	pVtx[(NUM_SCOREDIGIT+1)*4+0].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[(NUM_SCOREDIGIT+1)*4+1].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[(NUM_SCOREDIGIT+1)*4+2].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[(NUM_SCOREDIGIT+1)*4+3].color = D3DCOLOR_RGBA(255,255,255,255);

	g_pVtxBufferResultPolygon->Unlock();                                     //バッファのアンロック

	if(GetControllerTrigger(0) || GetKeyboardTrigger(DIK_J)) 
	{
		SetFade(FADE_OUT,MODE_TITLE);
	}
}

////////////////////////////////////////////////
//ポリゴンの描画
////////////////////////////////////////////////
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	//頂点フォーマット設定
	pD3DDevice -> SetStreamSource(0,g_pVtxBufferResultPolygon,0,sizeof(VERTEX_2D));                   //VRAMからGPUにデータをストリーム
	pD3DDevice -> SetFVF(FVF_VERTEX2D);

	//背景
	pD3DDevice -> SetTexture(0,g_pTextureResultPolygon[3]);												 //テクスチャの設定
	pD3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,(NUM_SCOREDIGIT+2)*4,NUM_POLYGON);

	//リザルトフォント
	pD3DDevice -> SetTexture(0,g_pTextureResultPolygon[0]);												 //テクスチャの設定
	pD3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,0,NUM_POLYGON);                                  //テクスチャメモリの確保

	//スコア
	pD3DDevice -> SetTexture(0,g_pTextureResultPolygon[1]);												 //テクスチャの設定
	for(int nCnt = 1; nCnt < NUM_SCOREDIGIT+1; nCnt++)
	{
		pD3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,nCnt*4,NUM_POLYGON);                                  //テクスチャメモリの確保
	}

	//スコアフォント
	pD3DDevice -> SetTexture(0,g_pTextureResultPolygon[2]);												 //テクスチャの設定
	pD3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,(NUM_SCOREDIGIT+1)*4,NUM_POLYGON); 
}

void UninitResult(void)
{
	// テクスチャインターフェイスの片付け
	for(int nCnt = 0; nCnt < NUM_RESULTTEXTURE; nCnt++)
	{
		SAFE_RELEASE(g_pTextureResultPolygon[nCnt]);
	}

	//頂点バッファインターフェイスの片付け
	SAFE_RELEASE(g_pVtxBufferResultPolygon);
}

//背景ポリゴンの表示位置X,ポリゴンの表示位置Y,ポリゴンの幅,ポリゴンの高さ
void MakeVertexPolygonResult(LPDIRECT3DDEVICE9 pDevice,VERTEX_2D *pVtx)
{
	pDevice -> SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);   //引数1:設定名カリング 引数2:設定値(D3DCULL_CCW:逆時計回りを裏面をする)
	pDevice -> SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	pDevice -> SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	pDevice -> SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[1].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[2].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[3].color = D3DCOLOR_RGBA(255,255,255,255);

	//ポリゴン描画する前にテクスチャをセット
	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);
}