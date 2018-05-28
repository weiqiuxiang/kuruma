//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include <Windows.h>
#include <string.h>
#include "main.h"
#include "score.h"
#include "player.h"
#include <stdio.h>

//-----------------------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureScorePolygon[2] = {};           //テクスチャを操作するためのポインタ(テクスチャバッファインターフェイスにアクセスためのポインタ)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferScorePolygon = NULL;    //頂点バッファを操作するためのポインタ(頂点バッファインターフェイスにアクセスためのポインタ)
float g_score = 0;                                          //スコアの合計

////////////////////////////////////////////////
//ポリゴン(頂点)の初期化
////////////////////////////////////////////////
HRESULT InitScorePolygon(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_2D *pVtx = NULL;                           //頂点構造体のポインタ
	pD3DDevice = Get_Device();

	//頂点バッファの生成
	if(FAILED(pD3DDevice -> CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_SCOREVERTEX*(NUM_SCOREDIGIT+1),D3DUSAGE_WRITEONLY,FVF_VERTEX2D,D3DPOOL_MANAGED,&g_pVtxBufferScorePolygon,NULL)))
	{
		return E_FAIL;
	}

	//スコアテクスチャの取得(2枚)
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, SCOREPOLYGON_TEXTURENAME, &g_pTextureScorePolygon[0])))
	{
		MessageBox(NULL, "テクスチャの読み込みが失敗しました", "エラー", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, SCOREPOLYGON02_TEXTURENAME, &g_pTextureScorePolygon[1])))
	{
		MessageBox(NULL, "テクスチャの読み込みが失敗しました", "エラー", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	//スコアのポリゴン
	g_pVtxBufferScorePolygon -> Lock(0,0,(void**)&pVtx,0);                    //仮想アドレスを取得しバッファをロックする

	for(int nCnt = NUM_SCOREDIGIT - 1; nCnt >= 0; nCnt--)
	{
		MakeVertexPolygonScore(pD3DDevice,&pVtx[nCnt*4+0]);
		pVtx[nCnt*4+0].pos = D3DXVECTOR3(POS_SCORE_X+nCnt*SCORE_INTERVAL,POS_SCORE_Y,0.0f);
		pVtx[nCnt*4+1].pos = D3DXVECTOR3(POS_SCORE_X+WIDTH_SCORE+nCnt*SCORE_INTERVAL,POS_SCORE_Y,0.0f);
		pVtx[nCnt*4+2].pos = D3DXVECTOR3(POS_SCORE_X+nCnt*SCORE_INTERVAL,POS_SCORE_Y+HEIGHT_SCORE,0.0f);
		pVtx[nCnt*4+3].pos = D3DXVECTOR3(POS_SCORE_X+WIDTH_SCORE+nCnt*SCORE_INTERVAL,POS_SCORE_Y+HEIGHT_SCORE,0.0f);
	}

	MakeVertexPolygonScore(pD3DDevice,&pVtx[NUM_SCOREDIGIT*4+0]);
	pVtx[NUM_SCOREDIGIT*4+0].pos = D3DXVECTOR3(POS_SCORE_X-DISTANCE_FROM_SCORE,POS_SCORE_Y,0.0f);
	pVtx[NUM_SCOREDIGIT*4+1].pos = D3DXVECTOR3(POS_SCORE_X-DISTANCE_FROM_SCORE+WIDTH_FONT,POS_SCORE_Y,0.0f);
	pVtx[NUM_SCOREDIGIT*4+2].pos = D3DXVECTOR3(POS_SCORE_X-DISTANCE_FROM_SCORE,POS_SCORE_Y+HEIGHT_FONT,0.0f);
	pVtx[NUM_SCOREDIGIT*4+3].pos = D3DXVECTOR3(POS_SCORE_X-DISTANCE_FROM_SCORE+WIDTH_FONT,POS_SCORE_Y+HEIGHT_FONT,0.0f);

	g_pVtxBufferScorePolygon -> Unlock();                                     //バッファのアンロック

	return S_OK;
}

//ポリゴンの更新
void UpdateScorePolygon(void)
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

	//背景空の計算
	g_pVtxBufferScorePolygon->Lock(0, 0, (void**)&pVtx, 0);       //仮想アドレスを取得しバッファをロックする

	//スコアのパラメータ調整
	for(int nCnt = NUM_SCOREDIGIT - 1; nCnt >= 0; nCnt--)
	{
		//スコアの
		MakeVertexPolygonScore(pD3DDevice,&pVtx[nCnt*4+0]);
		pVtx[nCnt*4+0].pos = D3DXVECTOR3(POS_SCORE_X+nCnt*SCORE_INTERVAL,POS_SCORE_Y,0.0f);
		pVtx[nCnt*4+1].pos = D3DXVECTOR3(POS_SCORE_X+WIDTH_SCORE+nCnt*SCORE_INTERVAL,POS_SCORE_Y,0.0f);
		pVtx[nCnt*4+2].pos = D3DXVECTOR3(POS_SCORE_X+nCnt*SCORE_INTERVAL,POS_SCORE_Y+HEIGHT_SCORE,0.0f);
		pVtx[nCnt*4+3].pos = D3DXVECTOR3(POS_SCORE_X+WIDTH_SCORE+nCnt*SCORE_INTERVAL,POS_SCORE_Y+HEIGHT_SCORE,0.0f);

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

		nDigit--;
	}

	MakeVertexPolygonScore(pD3DDevice,&pVtx[NUM_SCOREDIGIT*4+0]);
	pVtx[NUM_SCOREDIGIT*4+0].pos = D3DXVECTOR3(POS_SCORE_X-DISTANCE_FROM_SCORE,POS_SCORE_Y,0.0f);
	pVtx[NUM_SCOREDIGIT*4+1].pos = D3DXVECTOR3(POS_SCORE_X-DISTANCE_FROM_SCORE+WIDTH_FONT,POS_SCORE_Y,0.0f);
	pVtx[NUM_SCOREDIGIT*4+2].pos = D3DXVECTOR3(POS_SCORE_X-DISTANCE_FROM_SCORE,POS_SCORE_Y+HEIGHT_FONT,0.0f);
	pVtx[NUM_SCOREDIGIT*4+3].pos = D3DXVECTOR3(POS_SCORE_X-DISTANCE_FROM_SCORE+WIDTH_FONT,POS_SCORE_Y+HEIGHT_FONT,0.0f);

	pVtx[NUM_SCOREDIGIT*4+0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[NUM_SCOREDIGIT*4+1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[NUM_SCOREDIGIT*4+2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[NUM_SCOREDIGIT*4+3].tex = D3DXVECTOR2(1.0f,1.0f);

	g_pVtxBufferScorePolygon->Unlock();                                     //バッファのアンロック
}

////////////////////////////////////////////////
//ポリゴンの描画
////////////////////////////////////////////////
void DrawScorePolygon(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	//背景の空：頂点フォーマット設定
	pD3DDevice -> SetStreamSource(0,g_pVtxBufferScorePolygon,0,sizeof(VERTEX_2D));                   //VRAMからGPUにデータをストリーム
	pD3DDevice -> SetFVF(FVF_VERTEX2D);
	pD3DDevice -> SetTexture(0,g_pTextureScorePolygon[0]);												 //テクスチャの設定
	for(int nCnt = 0; nCnt < NUM_SCOREDIGIT; nCnt++)
	{
		pD3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,nCnt*4,NUM_POLYGON);                                  //テクスチャメモリの確保
	}

	pD3DDevice -> SetTexture(0,g_pTextureScorePolygon[1]);												 //テクスチャの設定
	pD3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,NUM_SCOREDIGIT*4,NUM_POLYGON); 
}

void UninitScorePolygon(void)
{
	for(int nCnt = 0; nCnt < 2; nCnt++)
	{
		// テクスチャインターフェイスの片付け
		SAFE_RELEASE(g_pTextureScorePolygon[nCnt]);
	}

	//頂点バッファインターフェイスの片付け
	SAFE_RELEASE(g_pVtxBufferScorePolygon);
}

//背景ポリゴンの表示位置X,ポリゴンの表示位置Y,ポリゴンの幅,ポリゴンの高さ
void MakeVertexPolygonScore(LPDIRECT3DDEVICE9 pDevice,VERTEX_2D *pVtx)
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