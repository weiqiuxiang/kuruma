//-----------------------------------------------------------------------------------------
//プレーヤー,道具などの処理[running02.cpp]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------------------
#include <time.h>
#include "main.h"
#include "particalCenter.h"
#include "partical.h"
#include "particalEffect.h"
#include "player.h"

//-----------------------------------------------------------------------------------------
//マクロ
//-----------------------------------------------------------------------------------------
#define PARTICALRADIUS (10)

//-----------------------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureParticalPolygon[NUM_PARTICALTEXTURE] = {};           //テクスチャを操作するためのポインタ(テクスチャバッファインターフェイスにアクセスためのポインタ)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferParticalPolygon = NULL;    //頂点バッファを操作するためのポインタ(頂点バッファインターフェイスにアクセスためのポインタ)

PARTICAL g_Partical[MAX_PARTICAL];

//-----------------------------------------------------------------------------------------
//重要定番関数達

//各変数の初期化
HRESULT InitPartical(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_2D *pVtx = NULL;                           //頂点構造体のポインタ
	pD3DDevice = Get_Device();
	 
	//頂点バッファの生成
	if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_PARTICALVERTEX*MAX_PARTICAL, D3DUSAGE_WRITEONLY, FVF_VERTEX2D, D3DPOOL_MANAGED, &g_pVtxBufferParticalPolygon, NULL)))
	{
		return E_FAIL;
	}

	//パーティクルのテクスチャ取得
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, PARTICALPOLYGON01_TEXTURENAME, &g_pTextureParticalPolygon[0])))
	{
		MessageBox(NULL, "テクスチャの読み込みが失敗しました", "エラー", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	//プレーヤーのポリゴン
	g_pVtxBufferParticalPolygon->Lock(0, 0, (void**)&pVtx, 0);                    //仮想アドレスを取得しバッファをロックする
	MakeVertexPolygonPartical(pD3DDevice, pVtx);

	for (int nCnt = 0; nCnt < MAX_PARTICAL; nCnt++)
	{
		//パーティクルの諸データの初期値
		g_Partical[nCnt].pos.x = 0;
		g_Partical[nCnt].pos.y = 0;
		g_Partical[nCnt].speed.x = 0;
		g_Partical[nCnt].speed.y = 0;
		g_Partical[nCnt].color = D3DCOLOR_RGBA(255,255,255,255);
		g_Partical[nCnt].nAlpha = 255;
		g_Partical[nCnt].nLife = 20;
		g_Partical[nCnt].bUse = false;
		g_Partical[nCnt].fRadius = 0;
		g_Partical[nCnt].bCamera = false;
		g_Partical[nCnt].fColorAlphaValue = 255 / g_Partical[nCnt].nLife;

		pVtx[0].pos = D3DXVECTOR3(g_Partical[nCnt].pos.x - PARTICALRADIUS, g_Partical[nCnt].pos.y - PARTICALRADIUS, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Partical[nCnt].pos.x + PARTICALRADIUS, g_Partical[nCnt].pos.y - PARTICALRADIUS, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Partical[nCnt].pos.x - PARTICALRADIUS, g_Partical[nCnt].pos.y + PARTICALRADIUS, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Partical[nCnt].pos.x + PARTICALRADIUS, g_Partical[nCnt].pos.y + PARTICALRADIUS, 0.0f);

		pVtx += 4;
	}

	g_pVtxBufferParticalPolygon->Unlock();                     //バッファのアンロック
	return S_OK;
}
//終了処理
void UninitPartical(void)
{
	// テクスチャインターフェイスの片付け
	for (int nCnt = 0; nCnt < NUM_PARTICALTEXTURE; nCnt++)
	{
		SAFE_RELEASE(g_pTextureParticalPolygon[nCnt]);
	}

	//頂点バッファインターフェイスの片付け
	SAFE_RELEASE(g_pVtxBufferParticalPolygon);
}

//プレーヤーデータの更新処理
void UpdatePartical(void)
{
	//パーティクル発射中心データ取得
	PARTICALCENTER *pParticalCenter;
	pParticalCenter = GetParticalCenter();

	//現在モード取得
	EFFECTMODE *EffectMode;
	EffectMode = GetEffectMode();

	for (int nCnt = 0; nCnt < MAX_PARTICAL; nCnt++)
	{
		//もしパーティクルは使用可の状態なら
		if (g_Partical[nCnt].bUse == true)
		{
			g_Partical[nCnt].posOld.x = g_Partical[nCnt].pos.x;
			g_Partical[nCnt].posOld.y = g_Partical[nCnt].pos.y;

			//エフェクトリスト
			switch(*EffectMode)
			{
			case EFFECTMODE_BASE:
				{
					//風力計算
					g_Partical[nCnt].speed.x += pParticalCenter -> fWindPower * cos(pParticalCenter -> fWindAngel);
					g_Partical[nCnt].speed.y += pParticalCenter -> fWindPower * sin(pParticalCenter -> fWindAngel);

					//重力計算
					g_Partical[nCnt].speed.y += pParticalCenter -> fGravity;
					break;
				}
			}

			//パーティクル移動
			g_Partical[nCnt].pos.x += g_Partical[nCnt].speed.x;
			g_Partical[nCnt].pos.y += g_Partical[nCnt].speed.y;

			//パーティクル終了処理
			g_Partical[nCnt].nLife--;                                   //Lifeが減る
			g_Partical[nCnt].nAlpha -= g_Partical[nCnt].fColorAlphaValue; //α値が減る

			//lifeが0以下になったら未使用にする
			if(g_Partical[nCnt].nLife <= 0)
			{
				g_Partical[nCnt].bUse = false;
			}
		}
	}

	UpdatePolygonPartical();
}

//プレーヤーポリゴンのデータ更新
void UpdatePolygonPartical(void)
{
	VERTEX_2D *pVtx = NULL;
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();
	PLAYER *pPlayer = GetPlayer();

	g_pVtxBufferParticalPolygon -> Lock(0, 0, (void**)&pVtx, 0);          //仮想アドレスを取得しバッファをロックする
	MakeVertexPolygonPartical(pD3DDevice, pVtx);

	for (int nCnt = 0; nCnt < MAX_PARTICAL; nCnt++)
	{
		if(g_Partical[nCnt].bUse == true)
		{
			//表示位置はカメラと関係ない
			if(g_Partical[nCnt].bCamera == false)
			{
				pVtx[0+nCnt*4].pos = D3DXVECTOR3(g_Partical[nCnt].pos.x - g_Partical[nCnt].fRadius, g_Partical[nCnt].pos.y - g_Partical[nCnt].fRadius, 0.0f);
				pVtx[1+nCnt*4].pos = D3DXVECTOR3(g_Partical[nCnt].pos.x + g_Partical[nCnt].fRadius, g_Partical[nCnt].pos.y - g_Partical[nCnt].fRadius, 0.0f);
				pVtx[2+nCnt*4].pos = D3DXVECTOR3(g_Partical[nCnt].pos.x - g_Partical[nCnt].fRadius, g_Partical[nCnt].pos.y + g_Partical[nCnt].fRadius, 0.0f);
				pVtx[3+nCnt*4].pos = D3DXVECTOR3(g_Partical[nCnt].pos.x + g_Partical[nCnt].fRadius, g_Partical[nCnt].pos.y + g_Partical[nCnt].fRadius, 0.0f);
			}
			
			else
			{
				pVtx[0+nCnt*4].pos = D3DXVECTOR3(g_Partical[nCnt].pos.x - g_Partical[nCnt].fRadius - pPlayer->fPosX_SUM, g_Partical[nCnt].pos.y - g_Partical[nCnt].fRadius, 0.0f);
				pVtx[1+nCnt*4].pos = D3DXVECTOR3(g_Partical[nCnt].pos.x + g_Partical[nCnt].fRadius - pPlayer->fPosX_SUM, g_Partical[nCnt].pos.y - g_Partical[nCnt].fRadius, 0.0f);
				pVtx[2+nCnt*4].pos = D3DXVECTOR3(g_Partical[nCnt].pos.x - g_Partical[nCnt].fRadius - pPlayer->fPosX_SUM, g_Partical[nCnt].pos.y + g_Partical[nCnt].fRadius, 0.0f);
				pVtx[3+nCnt*4].pos = D3DXVECTOR3(g_Partical[nCnt].pos.x + g_Partical[nCnt].fRadius - pPlayer->fPosX_SUM, g_Partical[nCnt].pos.y + g_Partical[nCnt].fRadius, 0.0f);
			}

			pVtx[0+nCnt*4].rhw = 1.0f;
			pVtx[1+nCnt*4].rhw = 1.0f;
			pVtx[2+nCnt*4].rhw = 1.0f;
			pVtx[3+nCnt*4].rhw = 1.0f;

			g_Partical[nCnt].color = D3DCOLOR_RGBA(70,70,0,g_Partical[nCnt].nAlpha);

			pVtx[0+nCnt*4].color = g_Partical[nCnt].color;
			pVtx[1+nCnt*4].color = g_Partical[nCnt].color;
			pVtx[2+nCnt*4].color = g_Partical[nCnt].color;
			pVtx[3+nCnt*4].color = g_Partical[nCnt].color;

			pVtx[0+nCnt*4].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1+nCnt*4].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2+nCnt*4].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3+nCnt*4].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
	}

	g_pVtxBufferParticalPolygon -> Unlock();                                     //バッファのアンロック
}

void DrawPartical(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	//背景の鉄欄干：頂点フォーマット設定
	pD3DDevice->SetStreamSource(0, g_pVtxBufferParticalPolygon, 0, sizeof(VERTEX_2D));              //VRAMからGPUにデータをストリーム
	pD3DDevice->SetFVF(FVF_VERTEX2D);
	pD3DDevice->SetTexture(0, g_pTextureParticalPolygon[0]);				 //テクスチャの設定

	for (int nCnt = 0; nCnt < MAX_PARTICAL; nCnt++)
	{
		//もしパーティクルは使用可の状態なら
		if (g_Partical[nCnt].bUse == true)
		{
			pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt*4, NUM_POLYGON);                                            //テクスチャメモリの確保
		}
	}
}


//プレーヤーポリゴンの表示位置X,ポリゴンの表示位置Y,ポリゴンの幅,ポリゴンの高さ
void MakeVertexPolygonPartical(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx)
{
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);   //引数1:設定名カリング 引数2:設定値(D3DCULL_CCW:逆時計回りを裏面をする)
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].color = D3DCOLOR_RGBA(255, 255, 255, 255);

	//ポリゴン描画する前にテクスチャをセット
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//敵情報取得
PARTICAL *GetPartical(void)
{

	return &g_Partical[0];
}

//パーティクルを設置
void SetPartical(float fPosX,float fPosY,float fSpeedX,float fSpeedY,float fRadius,float nLife,bool bCamera)
{
	for(int nCnt = 0; nCnt < MAX_PARTICAL; nCnt++)
	{
		if(g_Partical[nCnt].bUse == false)
		{
			g_Partical[nCnt].pos.x = fPosX;
			g_Partical[nCnt].pos.y = fPosY;
			g_Partical[nCnt].posOld.x = g_Partical[nCnt].pos.x;
			g_Partical[nCnt].posOld.y = g_Partical[nCnt].pos.y;
			g_Partical[nCnt].speed.x = fSpeedX;
			g_Partical[nCnt].speed.y = fSpeedY;
			g_Partical[nCnt].color = D3DCOLOR_RGBA(255,255,255,255);
			g_Partical[nCnt].fRadius = fRadius;
			g_Partical[nCnt].nLife = nLife;
			g_Partical[nCnt].nAlpha = 10;
			g_Partical[nCnt].fColorAlphaValue = g_Partical[nCnt].nAlpha / g_Partical[nCnt].nLife;
			g_Partical[nCnt].bCamera = bCamera;
			g_Partical[nCnt].bUse = true;
			break;
		}
	}
}