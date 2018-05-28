//-----------------------------------------------------------------------------------------
//プレーヤー,道具などの処理[running02.cpp]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------------------
#include <time.h>
#include "main.h"
#include "bullet.h"
#include "bulletEffect.h"
#include "player.h"

//-----------------------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureBulletEffectPolygon[NUM_BULLETEFFECTTEXTURE] = {};           //テクスチャを操作するためのポインタ(テクスチャバッファインターフェイスにアクセスためのポインタ)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferBulletEffectPolygon = NULL;    //頂点バッファを操作するためのポインタ(頂点バッファインターフェイスにアクセスためのポインタ)

BULLETEFFECT g_BulletEffect[MAX_BULLETEFFECT];

//-----------------------------------------------------------------------------------------
//重要定番関数達

//各変数の初期化
HRESULT InitBulletEffect(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_2D *pVtx = NULL;                           //頂点構造体のポインタ
	pD3DDevice = Get_Device();
	PLAYER *pPlayer = GetPlayer();
	 
	//頂点バッファの生成
	if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_BULLETEFFECTVERTEX*MAX_BULLETEFFECT, D3DUSAGE_WRITEONLY, FVF_VERTEX2D, D3DPOOL_MANAGED, &g_pVtxBufferBulletEffectPolygon, NULL)))
	{
		return E_FAIL;
	}

	//弾のテクスチャ取得
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, BULLETEFFECTPOLYGON01_TEXTURENAME, &g_pTextureBulletEffectPolygon[0])))
	{
		MessageBox(NULL, "テクスチャの読み込みが失敗しました", "エラー", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	//プレーヤーのポリゴン
	g_pVtxBufferBulletEffectPolygon->Lock(0, 0, (void**)&pVtx, 0);                    //仮想アドレスを取得しバッファをロックする
	MakeVertexPolygonPlayer(pD3DDevice, pVtx);

	for (int nCnt = 0; nCnt < MAX_BULLETEFFECT; nCnt++)
	{
		//プレーヤーの諸データの初期値
		g_BulletEffect[nCnt].pos.x = pPlayer -> fPosX;
		g_BulletEffect[nCnt].pos.y = pPlayer -> fPosY;
		g_BulletEffect[nCnt].color = D3DCOLOR_RGBA(255,255,255,255);
		g_BulletEffect[nCnt].fAngle = 0;
		g_BulletEffect[nCnt].nAlpha = 255;
		g_BulletEffect[nCnt].nLife = BULLETLIFE;
		g_BulletEffect[nCnt].bUse = false;
		g_BulletEffect[nCnt].fRadius = RADIUS;
		g_BulletEffect[nCnt].fRaidusValue = g_BulletEffect[nCnt].fRadius / g_BulletEffect[nCnt].nLife;
		g_BulletEffect[nCnt].fColorAlphaValue = 255 / g_BulletEffect[nCnt].nLife;

		pVtx[0].pos = D3DXVECTOR3(g_BulletEffect[nCnt].pos.x, g_BulletEffect[nCnt].pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_BulletEffect[nCnt].pos.x + BULLETEFFECT_WIDTH, g_BulletEffect[nCnt].pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_BulletEffect[nCnt].pos.x, g_BulletEffect[nCnt].pos.y + BULLETEFFECT_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_BulletEffect[nCnt].pos.x + BULLETEFFECT_WIDTH, g_BulletEffect[nCnt].pos.y + BULLETEFFECT_HEIGHT, 0.0f);

		pVtx += 4;
	}

	g_pVtxBufferBulletEffectPolygon->Unlock();                     //バッファのアンロック
	return S_OK;
}
//終了処理
void UninitBulletEffect(void)
{
	// テクスチャインターフェイスの片付け
	for (int nCnt = 0; nCnt < NUM_BULLETEFFECTTEXTURE; nCnt++)
	{
		SAFE_RELEASE(g_pTextureBulletEffectPolygon[nCnt]);
	}

	//頂点バッファインターフェイスの片付け
	SAFE_RELEASE(g_pVtxBufferBulletEffectPolygon);
}

//プレーヤーデータの更新処理
void UpdateBulletEffect(void)
{
	for (int nCnt = 0; nCnt < MAX_BULLETEFFECT; nCnt++)
	{
		//もし敵は使用可の状態なら
		if (g_BulletEffect[nCnt].bUse == true)
		{
			g_BulletEffect[nCnt].nLife--;
			g_BulletEffect[nCnt].fRadius -= g_BulletEffect[nCnt].fRaidusValue;
			g_BulletEffect[nCnt].nAlpha -= g_BulletEffect[nCnt].fColorAlphaValue;

			//半径が0以下にならないように
			if(g_BulletEffect[nCnt].fRadius < 0)
			{
				g_BulletEffect[nCnt].fRadius = 0.0f;
			}

			//α値が0以下にならないように
			if(g_BulletEffect[nCnt].nAlpha < 0)
			{
				g_BulletEffect[nCnt].nAlpha = 0;
			}

			g_BulletEffect[nCnt].color = D3DCOLOR_RGBA(255,0,0,255);   //カラー値を上書きする

			//lifeが0以下になったら未使用にする
			if(g_BulletEffect[nCnt].nLife <= 0)
			{
				g_BulletEffect[nCnt].nLife = 0;
				g_BulletEffect[nCnt].bUse = false;
			}
		}
	}

	UpdatePolygonBulletEffect();
}

//プレーヤーポリゴンのデータ更新
void UpdatePolygonBulletEffect(void)
{
	static int nRunCnt = 0;                                       //ランニングマンテクスチャの切り替えカウンター
	static int nActionFlame = 0;                                  //ランニングマンの動きを遅くするためのカウンター
	VERTEX_2D *pVtx = NULL;
	LPDIRECT3DDEVICE9 pD3DDevice;
	PLAYER *pPlayer = GetPlayer();
	pD3DDevice = Get_Device();

	g_pVtxBufferBulletEffectPolygon -> Lock(0, 0, (void**)&pVtx, 0);          //仮想アドレスを取得しバッファをロックする

	MakeVertexPolygonBulletEffect(pD3DDevice, pVtx);

	for (int nCnt = 0; nCnt < MAX_BULLETEFFECT; nCnt++)
	{
		if(g_BulletEffect[nCnt].bUse == true)
		{
			
			SpriteRorateBulletEffect(g_BulletEffect[nCnt],pVtx+nCnt*4);

			pVtx[0+nCnt*4].rhw = 1.0f;
			pVtx[1+nCnt*4].rhw = 1.0f;
			pVtx[2+nCnt*4].rhw = 1.0f;
			pVtx[3+nCnt*4].rhw = 1.0f;

			pVtx[0+nCnt*4].color = g_BulletEffect[nCnt].color;
			pVtx[1+nCnt*4].color = g_BulletEffect[nCnt].color;
			pVtx[2+nCnt*4].color = g_BulletEffect[nCnt].color;
			pVtx[3+nCnt*4].color = g_BulletEffect[nCnt].color;

			pVtx[0+nCnt*4].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1+nCnt*4].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2+nCnt*4].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3+nCnt*4].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
	}

	g_pVtxBufferBulletEffectPolygon -> Unlock();                                     //バッファのアンロック
}

void DrawBulletEffect(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	//エフェクト：頂点フォーマット設定
	pD3DDevice->SetStreamSource(0, g_pVtxBufferBulletEffectPolygon, 0, sizeof(VERTEX_2D));              //VRAMからGPUにデータをストリーム
	pD3DDevice->SetFVF(FVF_VERTEX2D);
	pD3DDevice->SetTexture(0, g_pTextureBulletEffectPolygon[0]);				 //テクスチャの設定

	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);                            //加算合成を行う!

	//エフェクトの描画
	for (int nCnt = 0; nCnt < MAX_BULLETEFFECT; nCnt++)
	{
		//もし弾は使用可の状態なら
		if (g_BulletEffect[nCnt].bUse == true)
		{
			pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt*4, NUM_POLYGON);                                            //テクスチャメモリの確保
		}
	}

	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);                   //ブレンドモードをアルファブレンドに戻す
}

//プレーヤーポリゴンの表示位置X,ポリゴンの表示位置Y,ポリゴンの幅,ポリゴンの高さ
void MakeVertexPolygonBulletEffect(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx)
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
BULLETEFFECT *GetBulletEffect(void)
{

	return &g_BulletEffect[0];
}

//弾を設置
void SetBulletEffect(float fPosX,float fPosY,int nLife,float fAngle)
{
	for(int nCnt = 0; nCnt < MAX_BULLETEFFECT; nCnt++)
	{
		if(g_BulletEffect[nCnt].bUse == false)
		{
			g_BulletEffect[nCnt].pos.x = fPosX;
			g_BulletEffect[nCnt].pos.y = fPosY;
			g_BulletEffect[nCnt].color = D3DCOLOR_RGBA(255,255,255,255);
			g_BulletEffect[nCnt].nLife = nLife;
			g_BulletEffect[nCnt].nAlpha = 255;
			g_BulletEffect[nCnt].fAngle = fAngle;
			g_BulletEffect[nCnt].bUse = true;
			g_BulletEffect[nCnt].fRadius = RADIUS;
			g_BulletEffect[nCnt].fRaidusValue = g_BulletEffect[nCnt].fRadius / nLife;
			g_BulletEffect[nCnt].fColorAlphaValue = 255 / nLife;
			
			break;
		}
	}
}

void SpriteRorateBulletEffect (BULLETEFFECT BBulletEffect,VERTEX_2D *pVtx)
{
	double X1, Y1, X2, Y2, X3, Y3, X4, Y4;             //左上,右下座標
	double Xc1, Yc1, Xc2, Yc2, Xc3, Yc3, Xc4, Yc4;     //回転した後の座標
	double XC, YC;                                     //中心座標

	X1 = BBulletEffect.pos.x - BBulletEffect.fRadius;
	Y1 = BBulletEffect.pos.y - BBulletEffect.fRadius;
	X2 = BBulletEffect.pos.x + BBulletEffect.fRadius;
	Y2 = BBulletEffect.pos.y - BBulletEffect.fRadius;
	X3 = BBulletEffect.pos.x - BBulletEffect.fRadius;
	Y3 = BBulletEffect.pos.y + BBulletEffect.fRadius;
	X4 = BBulletEffect.pos.x + BBulletEffect.fRadius;
	Y4 = BBulletEffect.pos.y + BBulletEffect.fRadius;

	XC = BBulletEffect.pos.x;
	YC = BBulletEffect.pos.y;

	Xc1 = (X1 - XC)*cos(BBulletEffect.fAngle) - (Y1 - YC)*sin(BBulletEffect.fAngle) + XC;
	Yc1 = (X1 - XC)*sin(BBulletEffect.fAngle) + (Y1 - YC)*cos(BBulletEffect.fAngle) + YC;

	Xc2 = (X2 - XC)*cos(BBulletEffect.fAngle) - (Y2 - YC)*sin(BBulletEffect.fAngle) + XC;
	Yc2 = (X2 - XC)*sin(BBulletEffect.fAngle) + (Y2 - YC)*cos(BBulletEffect.fAngle) + YC;

	Xc3 = (X3 - XC)*cos(BBulletEffect.fAngle) - (Y3 - YC)*sin(BBulletEffect.fAngle) + XC;
	Yc3 = (X3 - XC)*sin(BBulletEffect.fAngle) + (Y3 - YC)*cos(BBulletEffect.fAngle) + YC;

	Xc4 = (X4 - XC)*cos(BBulletEffect.fAngle) - (Y4 - YC)*sin(BBulletEffect.fAngle) + XC;
	Yc4 = (X4 - XC)*sin(BBulletEffect.fAngle) + (Y4 - YC)*cos(BBulletEffect.fAngle) + YC;

	pVtx[0].pos = D3DXVECTOR3(Xc1,Yc1,0.0f);
	pVtx[1].pos = D3DXVECTOR3(Xc2,Yc2,0.0f);
	pVtx[2].pos = D3DXVECTOR3(Xc3,Yc3,0.0f);
	pVtx[3].pos = D3DXVECTOR3(Xc4,Yc4,0.0f);
}