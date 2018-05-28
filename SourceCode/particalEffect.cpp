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
//グローバル変数
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureParticalEffectPolygon[NUM_PARTICALEFFECTTEXTURE] = {};           //テクスチャを操作するためのポインタ(テクスチャバッファインターフェイスにアクセスためのポインタ)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferParticalEffectPolygon = NULL;    //頂点バッファを操作するためのポインタ(頂点バッファインターフェイスにアクセスためのポインタ)

PARTICALEFFECT g_ParticalEffect[MAX_PARTICALEFFECT];

//-----------------------------------------------------------------------------------------
//重要定番関数達

//各変数の初期化
HRESULT InitParticalEffect(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_2D *pVtx = NULL;                           //頂点構造体のポインタ
	pD3DDevice = Get_Device();
	 
	//頂点バッファの生成
	if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_PARTICALEFFECTVERTEX*MAX_PARTICALEFFECT, D3DUSAGE_WRITEONLY, FVF_VERTEX2D, D3DPOOL_MANAGED, &g_pVtxBufferParticalEffectPolygon, NULL)))
	{
		return E_FAIL;
	}

	//パーティクルのテクスチャ取得
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, ParticalEffectPOLYGON01_TEXTURENAME, &g_pTextureParticalEffectPolygon[0])))
	{
		MessageBox(NULL, "テクスチャの読み込みが失敗しました", "エラー", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	//プレーヤーのポリゴン
	g_pVtxBufferParticalEffectPolygon->Lock(0, 0, (void**)&pVtx, 0);                    //仮想アドレスを取得しバッファをロックする
	MakeVertexPolygonParticalEffect(pD3DDevice, pVtx);

	for (int nCnt = 0; nCnt < MAX_PARTICALEFFECT; nCnt++)
	{
		//パーティクルのエフェクトの諸データの初期値
		g_ParticalEffect[nCnt].pos.x = 0;
		g_ParticalEffect[nCnt].pos.y = 0;
		g_ParticalEffect[nCnt].speed.x = 0;
		g_ParticalEffect[nCnt].speed.y = 0;
		g_ParticalEffect[nCnt].color = D3DCOLOR_RGBA(255,255,255,255);
		g_ParticalEffect[nCnt].nAlpha = 255;
		g_ParticalEffect[nCnt].nLife = 1;
		g_ParticalEffect[nCnt].bUse = false;
		g_ParticalEffect[nCnt].fRadius = 0;
		g_ParticalEffect[nCnt].fColorAlphaValue = 255 / g_ParticalEffect[nCnt].nLife;

		pVtx[0].pos = D3DXVECTOR3(g_ParticalEffect[nCnt].pos.x , g_ParticalEffect[nCnt].pos.y , 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_ParticalEffect[nCnt].pos.x , g_ParticalEffect[nCnt].pos.y , 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_ParticalEffect[nCnt].pos.x , g_ParticalEffect[nCnt].pos.y , 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_ParticalEffect[nCnt].pos.x , g_ParticalEffect[nCnt].pos.y , 0.0f);

		pVtx += 4;
	}

	g_pVtxBufferParticalEffectPolygon->Unlock();                     //バッファのアンロック
	return S_OK;
}
//終了処理
void UninitParticalEffect(void)
{
	// テクスチャインターフェイスの片付け
	for (int nCnt = 0; nCnt < NUM_PARTICALEFFECTTEXTURE; nCnt++)
	{
		SAFE_RELEASE(g_pTextureParticalEffectPolygon[nCnt]);
	}

	//頂点バッファインターフェイスの片付け
	SAFE_RELEASE(g_pVtxBufferParticalEffectPolygon);
}

//プレーヤーデータの更新処理
void UpdateParticalEffect(void)
{
	//パーティクル発射中心データ取得
	PARTICALCENTER *pPartical;
	pPartical = GetParticalCenter();

	//現在モード取得
	EFFECTMODE *EffectMode;
	EffectMode = GetEffectMode();

	for (int nCnt = 0; nCnt < MAX_PARTICALEFFECT; nCnt++)
	{
		//もしパーティクルは使用可の状態なら
		if (g_ParticalEffect[nCnt].bUse == true)
		{
			g_ParticalEffect[nCnt].posOld.x = g_ParticalEffect[nCnt].pos.x;
			g_ParticalEffect[nCnt].posOld.y = g_ParticalEffect[nCnt].pos.y;

			//エフェクトリスト
			switch(*EffectMode)
			{
			case EFFECTMODE_BASE:
				{
					//風力計算
					g_ParticalEffect[nCnt].speed.x += pPartical -> fWindPower * cos(pPartical -> fWindAngel);
					g_ParticalEffect[nCnt].speed.y += pPartical -> fWindPower * sin(pPartical -> fWindAngel);

					//重力計算
					g_ParticalEffect[nCnt].speed.y += pPartical -> fGravity;
					break;
				}
			}

			//
			g_ParticalEffect[nCnt].pos.x += g_ParticalEffect[nCnt].speed.x;
			g_ParticalEffect[nCnt].pos.y += g_ParticalEffect[nCnt].speed.y;

			g_ParticalEffect[nCnt].nLife--;                                         //Lifeが減る
			g_ParticalEffect[nCnt].nAlpha -= g_ParticalEffect[nCnt].fColorAlphaValue; //α値が減る

			//lifeが0以下になったら未使用にする
			if(g_ParticalEffect[nCnt].nLife <= 0)
			{
				g_ParticalEffect[nCnt].bUse = false;
			}
		}
	}

	UpdatePolygonParticalEffect();
}

//プレーヤーポリゴンのデータ更新
void UpdatePolygonParticalEffect(void)
{
	static int nRunCnt = 0;                                       //ランニングマンテクスチャの切り替えカウンター
	static int nActionFlame = 0;                                  //ランニングマンの動きを遅くするためのカウンター
	VERTEX_2D *pVtx = NULL;
	PLAYER *pPlayer = GetPlayer();
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	g_pVtxBufferParticalEffectPolygon -> Lock(0, 0, (void**)&pVtx, 0);          //仮想アドレスを取得しバッファをロックする

	MakeVertexPolygonParticalEffect(pD3DDevice, pVtx);

	for (int nCnt = 0; nCnt < MAX_PARTICALEFFECT; nCnt++)
	{
		if(g_ParticalEffect[nCnt].bUse == true)
		{
			if(g_ParticalEffect[nCnt].bCamera == false)
			{
				pVtx[0+nCnt*4].pos = D3DXVECTOR3(g_ParticalEffect[nCnt].pos.x - g_ParticalEffect[nCnt].fRadius, g_ParticalEffect[nCnt].pos.y - g_ParticalEffect[nCnt].fRadius, 0.0f);
				pVtx[1+nCnt*4].pos = D3DXVECTOR3(g_ParticalEffect[nCnt].pos.x + g_ParticalEffect[nCnt].fRadius, g_ParticalEffect[nCnt].pos.y - g_ParticalEffect[nCnt].fRadius, 0.0f);
				pVtx[2+nCnt*4].pos = D3DXVECTOR3(g_ParticalEffect[nCnt].pos.x - g_ParticalEffect[nCnt].fRadius, g_ParticalEffect[nCnt].pos.y + g_ParticalEffect[nCnt].fRadius, 0.0f);
				pVtx[3+nCnt*4].pos = D3DXVECTOR3(g_ParticalEffect[nCnt].pos.x + g_ParticalEffect[nCnt].fRadius, g_ParticalEffect[nCnt].pos.y + g_ParticalEffect[nCnt].fRadius, 0.0f);
			}

			else
			{
				pVtx[0+nCnt*4].pos = D3DXVECTOR3(g_ParticalEffect[nCnt].pos.x - g_ParticalEffect[nCnt].fRadius - pPlayer->fPosX_SUM, g_ParticalEffect[nCnt].pos.y - g_ParticalEffect[nCnt].fRadius, 0.0f);
				pVtx[1+nCnt*4].pos = D3DXVECTOR3(g_ParticalEffect[nCnt].pos.x + g_ParticalEffect[nCnt].fRadius - pPlayer->fPosX_SUM, g_ParticalEffect[nCnt].pos.y - g_ParticalEffect[nCnt].fRadius, 0.0f);
				pVtx[2+nCnt*4].pos = D3DXVECTOR3(g_ParticalEffect[nCnt].pos.x - g_ParticalEffect[nCnt].fRadius - pPlayer->fPosX_SUM, g_ParticalEffect[nCnt].pos.y + g_ParticalEffect[nCnt].fRadius, 0.0f);
				pVtx[3+nCnt*4].pos = D3DXVECTOR3(g_ParticalEffect[nCnt].pos.x + g_ParticalEffect[nCnt].fRadius - pPlayer->fPosX_SUM, g_ParticalEffect[nCnt].pos.y + g_ParticalEffect[nCnt].fRadius, 0.0f);

			}

			pVtx[0+nCnt*4].rhw = 1.0f;
			pVtx[1+nCnt*4].rhw = 1.0f;
			pVtx[2+nCnt*4].rhw = 1.0f;
			pVtx[3+nCnt*4].rhw = 1.0f;

			g_ParticalEffect[nCnt].color = D3DCOLOR_RGBA(70,70,0,g_ParticalEffect[nCnt].nAlpha);

			pVtx[0+nCnt*4].color = g_ParticalEffect[nCnt].color;
			pVtx[1+nCnt*4].color = g_ParticalEffect[nCnt].color;
			pVtx[2+nCnt*4].color = g_ParticalEffect[nCnt].color;
			pVtx[3+nCnt*4].color = g_ParticalEffect[nCnt].color;

			pVtx[0+nCnt*4].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1+nCnt*4].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2+nCnt*4].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3+nCnt*4].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
	}

	g_pVtxBufferParticalEffectPolygon -> Unlock();                                     //バッファのアンロック
}

void DrawParticalEffect(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	//エフェクト：頂点フォーマット設定
	pD3DDevice->SetStreamSource(0, g_pVtxBufferParticalEffectPolygon, 0, sizeof(VERTEX_2D));              //VRAMからGPUにデータをストリーム
	pD3DDevice->SetFVF(FVF_VERTEX2D);
	pD3DDevice->SetTexture(0, g_pTextureParticalEffectPolygon[0]);				                        //テクスチャの設定

	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);                                             //加算合成を行う!

	for (int nCnt = 0; nCnt < MAX_PARTICALEFFECT; nCnt++)
	{
		//もしパーティクルは使用可の状態なら
		if (g_ParticalEffect[nCnt].bUse == true)
		{
			pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt*4, NUM_POLYGON);                        //テクスチャメモリの確保
		}
	}

	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);                   //ブレンドモードをアルファブレンドに戻す
}


//プレーヤーポリゴンの表示位置X,ポリゴンの表示位置Y,ポリゴンの幅,ポリゴンの高さ
void MakeVertexPolygonParticalEffect(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx)
{
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);   //引数1:設定名カリング 引数2:設定値(D3DCULL_CCW:逆時計回りを裏面をする)
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//ポリゴン描画する前にテクスチャをセット
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//敵情報取得
PARTICALEFFECT *GetParticalEffect(void)
{
	return &g_ParticalEffect[0];
}

//パーティクルを設置
void SetParticalEffect(float fPosX,float fPosY,float fSpeedX,float fSpeedY,float fRadius,int nLife,bool bCamera)
{
	for(int nCnt = 0; nCnt < MAX_PARTICALEFFECT; nCnt++)
	{
		if(g_ParticalEffect[nCnt].bUse == false)
		{
			g_ParticalEffect[nCnt].pos.x = fPosX;
			g_ParticalEffect[nCnt].pos.y = fPosY;
			g_ParticalEffect[nCnt].posOld.x = g_ParticalEffect[nCnt].pos.x;
			g_ParticalEffect[nCnt].posOld.y = g_ParticalEffect[nCnt].pos.y;
			g_ParticalEffect[nCnt].speed.x = fSpeedX;
			g_ParticalEffect[nCnt].speed.y = fSpeedY;
			g_ParticalEffect[nCnt].color = D3DCOLOR_RGBA(255,255,255,255);
			g_ParticalEffect[nCnt].fRadius = fRadius;
			g_ParticalEffect[nCnt].nLife = nLife;
			g_ParticalEffect[nCnt].nAlpha = 255;
			g_ParticalEffect[nCnt].bCamera = bCamera;
			g_ParticalEffect[nCnt].fColorAlphaValue = g_ParticalEffect[nCnt].nAlpha / g_ParticalEffect[nCnt].nLife;
			g_ParticalEffect[nCnt].bUse = true;
			
			break;
		}
	}
}