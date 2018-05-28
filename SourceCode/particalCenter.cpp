//-----------------------------------------------------------------------------------------
//パーティクル,道具などの処理[ParticalCenter.cpp]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------------------
#include <stdio.h>
#include <time.h>
#include "main.h"
#include "partical.h"
#include "particalEffect.h"
#include "particalCenter.h"
#include "input.h"

//-----------------------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------------------
#define FRAMEPARTICAL (4)                //1フレーム発射するパーティクルの数

//-----------------------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureParticalCenterPolygon[NUM_PARTICALCENTERTEXTURE] = {};           //テクスチャを操作するためのポインタ(テクスチャバッファインターフェイスにアクセスためのポインタ)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferParticalCenterPolygon = NULL;                             //頂点バッファを操作するためのポインタ(頂点バッファインターフェイスにアクセスためのポインタ)

PARTICALCENTER g_ParticalCenter[NUM_PARTICALCENTERPOLYGON];
CENTER_EFFECT g_ParticalCenterEffect[NUM_PARTICALCENTERPOLYGON];
int g_ParticalFunction = 0;                //パーティクルメニュー階層コントロール変数
EFFECTMODE g_EffectMode = EFFECTMODE_EXPLOSION; //パーティクルのエフェクト選択変数

//-----------------------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------------------
//パーティクル効果
void ParticalBase(PARTICALCENTER *);                  //パーティクル基本
void ParticalExplosion(PARTICALCENTER *);             //パーティクル爆発エフェクト

//-----------------------------------------------------------------------------------------
//重要定番関数達
//-----------------------------------------------------------------------------------------

//各変数の初期化
HRESULT InitParticalCenter(void)
{
	srand((unsigned)time(NULL));        //乱数の種を生成する

	for(int nCnt = 0; nCnt < NUM_PARTICALCENTERPOLYGON; nCnt++)
	{
		//パーティクルの初期位置
		g_ParticalCenter[nCnt].pos.x = 400;
		g_ParticalCenter[nCnt].pos.y = 320;
		//パーティクルの初期スピード
		g_ParticalCenter[nCnt].speed.x = 10;
		g_ParticalCenter[nCnt].speed.y = 9;
		//パーティクルの古い位置
		g_ParticalCenter[nCnt].posOld.x = g_ParticalCenter[nCnt].pos.x;
		g_ParticalCenter[nCnt].posOld.y = g_ParticalCenter[nCnt].pos.y;
		//パーティクルの集中発射角度と拡散角度
		g_ParticalCenter[nCnt].fShootAngel = PI *0.5;
		g_ParticalCenter[nCnt].fRangeAngel = PI;
		//パーティクルの飛び基準距離と半径
		g_ParticalCenter[nCnt].fDistance = 200;
		g_ParticalCenter[nCnt].fRadius = 0;
		//プレーヤー使用
		g_ParticalCenter[nCnt].nEffectType = 0;
		g_ParticalCenter[nCnt].bUse = false;

		//パーティクル周囲環境
		g_ParticalCenter[nCnt].fGravity = 0;                 //重力
		g_ParticalCenter[nCnt].fWindPower = 0;               //風力
		g_ParticalCenter[nCnt].fWindAngel = 0;               //風が吹く角度

		//パーティクル発射中心エフェクト
		//パーティクルの初期位置
		g_ParticalCenterEffect[nCnt].pos.x = 400;
		g_ParticalCenterEffect[nCnt].pos.y = 320;
		//パーティクルの初期スピード
		g_ParticalCenterEffect[nCnt].speed.x = 10;
		g_ParticalCenterEffect[nCnt].speed.y = 9;
		//パーティクルの古い位置
		g_ParticalCenterEffect[nCnt].posOld.x = g_ParticalCenterEffect[nCnt].pos.x;
		g_ParticalCenterEffect[nCnt].posOld.y = g_ParticalCenterEffect[nCnt].pos.y;
		g_ParticalCenterEffect[nCnt].fRadius = 0;
		g_ParticalCenterEffect[nCnt].bUse = false;
	}

	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_2D *pVtx = NULL;                           //頂点構造体のポインタ
	pD3DDevice = Get_Device();

	//頂点バッファの生成
	if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_PARTICALCENTERVERTEX*NUM_PARTICALCENTERPOLYGON*2, D3DUSAGE_WRITEONLY, FVF_VERTEX2D, D3DPOOL_MANAGED, &g_pVtxBufferParticalCenterPolygon, NULL)))
	{
		return E_FAIL;
	}

	//パーティクルテクスチャの取得
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, PARTICALCENTERPOLYGON01_TEXTURENAME, &g_pTextureParticalCenterPolygon[0])))
	{
		MessageBox(NULL, "テクスチャの読み込みが失敗しました", "エラー", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, PARTICALCENTERPOLYGON02_TEXTURENAME, &g_pTextureParticalCenterPolygon[1])))
	{
		MessageBox(NULL, "テクスチャの読み込みが失敗しました", "エラー", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	//パーティクルのポリゴン
	g_pVtxBufferParticalCenterPolygon->Lock(0, 0, (void**)&pVtx, 0);                    //仮想アドレスを取得しバッファをロックする
	
	for(int nCnt = 0; nCnt < NUM_PARTICALCENTERPOLYGON; nCnt++)
	{
		MakeVertexPolygonParticalCenter(pD3DDevice, pVtx);
		pVtx[0].pos = D3DXVECTOR3(g_ParticalCenter[nCnt].pos.x, g_ParticalCenter[nCnt].pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_ParticalCenter[nCnt].pos.x+PARTICALCENTER_WIDTH, g_ParticalCenter[nCnt].pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_ParticalCenter[nCnt].pos.x, g_ParticalCenter[nCnt].pos.y+PARTICALCENTER_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_ParticalCenter[nCnt].pos.x+PARTICALCENTER_WIDTH, g_ParticalCenter[nCnt].pos.y+PARTICALCENTER_HEIGHT, 0.0f);

		pVtx += 4;

		float fXCenter = g_ParticalCenter[nCnt].pos.x + PARTICALCENTER_WIDTH/2;
		float fYCenter = g_ParticalCenter[nCnt].pos.y + PARTICALCENTER_HEIGHT/2;

		pVtx[0].pos = D3DXVECTOR3(fXCenter - MAIN_RADIUS, fYCenter - MAIN_RADIUS, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fXCenter + MAIN_RADIUS, fYCenter - MAIN_RADIUS, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(fXCenter - MAIN_RADIUS, fYCenter + MAIN_RADIUS, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fXCenter + MAIN_RADIUS, fYCenter + MAIN_RADIUS, 0.0f);
	}

	g_pVtxBufferParticalCenterPolygon->Unlock();                                     //バッファのアンロック

	return S_OK;
}
//終了処理
void UninitParticalCenter(void)
{
	// テクスチャインターフェイスの片付け
	for (int nCnt = 0; nCnt < NUM_PARTICALCENTERTEXTURE; nCnt++)
	{
		SAFE_RELEASE(g_pTextureParticalCenterPolygon[nCnt]);
	}

	//頂点バッファインターフェイスの片付け
	SAFE_RELEASE(g_pVtxBufferParticalCenterPolygon);
}

//パーティクルデータの更新処理
void UpdateParticalCenter(void)
{
	for(int nCnt = 0; nCnt < NUM_PARTICALCENTERPOLYGON;nCnt++)
	{
		//もしパーティクル中心は使用可の状態なら
		if (g_ParticalCenter[nCnt].bUse == true)
		{
			//パーティクルの前の座標を記録
			g_ParticalCenter[nCnt].posOld.x = g_ParticalCenter[nCnt].pos.x;
			g_ParticalCenter[nCnt].posOld.y = g_ParticalCenter[nCnt].pos.y;

			//-----------------------------------------------------------------------------------------
			//パーティクルの移動処理
			//-----------------------------------------------------------------------------------------
			//プレーヤーの位置を受け取る

			//-----------------------------------------------------------------------------------------
			//パーティクルのエフェクトリスト
			//-----------------------------------------------------------------------------------------
			switch(g_ParticalCenter[nCnt].nEffectType)
			{
			case 1:
				//パーティクル:基本
				ParticalBase(&g_ParticalCenter[nCnt]);
				break;
			case 2:
				//パーティクル:爆発
				ParticalExplosion(&g_ParticalCenter[nCnt]);
				break;
			}

			//-----------------------------------------------------------------------------------------
			//パーティクル
			//-----------------------------------------------------------------------------------------
			//移動範囲制限
			if ((g_ParticalCenter[nCnt].pos.x < 0) || ( (g_ParticalCenter[nCnt].pos.x + PARTICALCENTER_WIDTH) > SCREEN_WIDTH))
			{
				g_ParticalCenter[nCnt].pos.x = g_ParticalCenter[nCnt].posOld.x;
			}

			if ((g_ParticalCenter[nCnt].pos.y < 0) || ((g_ParticalCenter[nCnt].pos.y + PARTICALCENTER_HEIGHT) > SCREEN_HEIGHT))
			{
				g_ParticalCenter[nCnt].pos.y = g_ParticalCenter[nCnt].posOld.y;
			}
		}

		UpdatePolygonParticalCenter();
	}
}

//パーティクルポリゴンのデータ更新
void UpdatePolygonParticalCenter(void) 
{
	VERTEX_2D *pVtx = NULL;
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();
																  
	g_pVtxBufferParticalCenterPolygon->Lock(0, 0, (void**)&pVtx, 0);          //仮想アドレスを取得しバッファをロックする

	MakeVertexPolygonParticalCenter(pD3DDevice, pVtx);

	for(int nCnt = 0; nCnt < NUM_PARTICALCENTERPOLYGON; nCnt++)
	{
		//もしパーティクル中心は使用可の状態なら
		if (g_ParticalCenter[nCnt].bUse == true)
		{
			//パーティクル本体のパラメータ
			pVtx[0].pos = D3DXVECTOR3(g_ParticalCenter[nCnt].pos.x, g_ParticalCenter[nCnt].pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_ParticalCenter[nCnt].pos.x + PARTICALCENTER_WIDTH, g_ParticalCenter[nCnt].pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_ParticalCenter[nCnt].pos.x, g_ParticalCenter[nCnt].pos.y + PARTICALCENTER_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_ParticalCenter[nCnt].pos.x + PARTICALCENTER_WIDTH, g_ParticalCenter[nCnt].pos.y + PARTICALCENTER_HEIGHT, 0.0f);

			pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			pVtx[0].color = D3DCOLOR_RGBA(255, 255, 255, 10);
			pVtx[1].color = D3DCOLOR_RGBA(255, 255, 255, 10);
			pVtx[2].color = D3DCOLOR_RGBA(255, 255, 255, 10);
			pVtx[3].color = D3DCOLOR_RGBA(255, 255, 255, 10);

			pVtx += 4;

			//パーティクルのマシンガンのパラメータ

			float fXCenter = g_ParticalCenter[nCnt].pos.x + PARTICALCENTER_WIDTH/2;
			float fYCenter = g_ParticalCenter[nCnt].pos.y + PARTICALCENTER_HEIGHT/2;

			pVtx[0].pos = D3DXVECTOR3(fXCenter - MAIN_RADIUS, fYCenter - MAIN_RADIUS, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(fXCenter + MAIN_RADIUS, fYCenter - MAIN_RADIUS, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(fXCenter - MAIN_RADIUS, fYCenter + MAIN_RADIUS, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(fXCenter + MAIN_RADIUS, fYCenter + MAIN_RADIUS, 0.0f);

			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			pVtx[0].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].color = D3DCOLOR_RGBA(255, 255, 255, 255);

		}
	}

	g_pVtxBufferParticalCenterPolygon->Unlock();                                     //バッファのアンロック
}

void DrawParticalCenter(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	for(int nCnt = 0; nCnt < NUM_PARTICALCENTERPOLYGON; nCnt++)
	{
		if(g_ParticalCenter[nCnt].bUse == true)
		{
			//背景の鉄欄干：頂点フォーマット設定
			pD3DDevice->SetStreamSource(0, g_pVtxBufferParticalCenterPolygon, 0, sizeof(VERTEX_2D));              //VRAMからGPUにデータをストリーム
			pD3DDevice->SetFVF(FVF_VERTEX2D);

			//パーティクル発射中心描画
			pD3DDevice->SetTexture(0, g_pTextureParticalCenterPolygon[0]);				                             //テクスチャの設定
			pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt*4, NUM_POLYGON);                                  //テクスチャメモリの確保

			//パーティクル発射中心のエフェクト描画
			pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);                                       //加算合成を行う!

			pD3DDevice->SetTexture(0, g_pTextureParticalCenterPolygon[1]);				                             //テクスチャの設定
			pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt*4+4, NUM_POLYGON);                                  //テクスチャメモリの確保

			pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);                               //ブレンドモードをアルファブレンドに戻す
		}
	}
}

//パーティクルポリゴンの表示位置X,ポリゴンの表示位置Y,ポリゴンの幅,ポリゴンの高さ
void MakeVertexPolygonParticalCenter(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx)
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

PARTICALCENTER *GetParticalCenter(void)
{
	return &g_ParticalCenter[0];
}

//パーティクル:基本
void ParticalBase(PARTICALCENTER *pParticalCenter)
{
		//発射角度計算
		float fHantenAngle = 2 * PI * ( (pParticalCenter->fShootAngel / (2*PI) ) - (int)(pParticalCenter->fShootAngel / (2*PI) ) );

		//発射角度が逆時計回りに沿いて回る
		if(GetKeyboardPress(DIK_LEFT))
		{
			pParticalCenter->fShootAngel -= 0.01*PI;
		}

		//発射角度が時計回りに沿いて回る
		if(GetKeyboardPress(DIK_RIGHT))
		{
			pParticalCenter->fShootAngel += 0.01*PI;
		}

		//発射角度が0以上確保(最大値は10*PI)
		if(pParticalCenter->fShootAngel <= -10*PI)
		{
			pParticalCenter->fShootAngel = -10*PI;
		}

		if(pParticalCenter->fShootAngel >= 10*PI)
		{
			pParticalCenter->fShootAngel = 10*PI;
		}

		//拡散角度拡大
		if(GetKeyboardPress(DIK_O))
		{
			pParticalCenter->fRangeAngel += 0.01*PI;
		}

		//拡散角度縮小
		if(GetKeyboardPress(DIK_K))
		{
			pParticalCenter->fRangeAngel -= 0.01*PI;
		}

		//拡散角度の範囲を0~PIに収まる
		if(pParticalCenter->fRangeAngel < 0.01)
		{
			pParticalCenter->fRangeAngel = 0.01;
		}

		if(pParticalCenter->fRangeAngel > PI)
		{
			pParticalCenter->fRangeAngel = PI;
		}

		//パーティクルの飛び基準距離をコントロール
		if(GetKeyboardPress(DIK_P))  //飛び距離伸びる
		{
			pParticalCenter->fDistance += 5;
		}

		if(GetKeyboardPress(DIK_L))  //飛び距離縮む
		{
			pParticalCenter->fDistance -= 5;
		}

		//飛び距離範囲制限
		if(pParticalCenter->fDistance < 10)
		{
			pParticalCenter->fDistance = 10;
		}

		if(pParticalCenter->fDistance > 300)
		{
			pParticalCenter->fDistance = 300;
		}

		//-----------------------------------------------------------------------------------------
		//風力計算
		//-----------------------------------------------------------------------------------------
		//風の角度計算
		if(GetKeyboardPress(DIK_I))
		{
			pParticalCenter->fWindAngel += 0.01*PI;
		}

		if(GetKeyboardPress(DIK_J))
		{
			pParticalCenter->fWindAngel -= 0.01*PI;
		}

		//角度範囲制限(-2*PI ~ 2*PI)
		if(pParticalCenter->fWindAngel < -2*PI)
		{
			pParticalCenter->fWindAngel = -2*PI;
		}

		if(pParticalCenter->fWindAngel > 2*PI)
		{
			pParticalCenter->fWindAngel = 2*PI;
		}

		//風力計算
		if(GetKeyboardPress(DIK_U))
		{
			pParticalCenter->fWindPower += 0.001;
		}

		if(GetKeyboardPress(DIK_H))
		{
			pParticalCenter->fWindPower -= 0.001;
		}

		//風力制限(0 ~ 10)
		if(pParticalCenter->fWindPower < 0)
		{
			pParticalCenter->fWindPower = 0;
		}

		if(pParticalCenter->fWindPower > 0.05)
		{
			pParticalCenter->fWindPower = 0.05;
		}

		//-----------------------------------------------------------------------------------------
		//重力計算
		//-----------------------------------------------------------------------------------------
		if(GetKeyboardPress(DIK_Y))
		{
			pParticalCenter->fGravity += 0.001;
		}

		if(GetKeyboardPress(DIK_G))
		{
			pParticalCenter->fGravity -= 0.001;
		}

		//重力制限(0 ~ 15)
		if(pParticalCenter->fGravity < 0)
		{
			pParticalCenter->fGravity = 0;
		}

		if(pParticalCenter->fGravity > 0.05)
		{
			pParticalCenter->fGravity = 0.05;
		}

		//-----------------------------------------------------------------------------------------
		//パーティクルの発射
		//-----------------------------------------------------------------------------------------
		float fDistance = 0;                //パーティクルの飛び距離
		float fAngel = 0;                   //角度
		D3DXVECTOR2 speed;                  //パーティクルのスピード

		fDistance = pParticalCenter->fDistance + rand()%100;       //飛び距離計算
		fAngel = pParticalCenter->fShootAngel + ( rand()%(int)(pParticalCenter->fRangeAngel*2*100) - (int)(pParticalCenter->fRangeAngel*100) ) * 0.01; //飛び角度計算
		speed.x = fDistance*cos(fAngel)/40;        //x方向のスピード
		speed.y = fDistance*sin(fAngel)/40;        //y方向のスピード

		//パーティクルを発射
		SetPartical(pParticalCenter->pos.x + PARTICALCENTER_WIDTH*0.5,pParticalCenter->pos.y + PARTICALCENTER_HEIGHT*0.5,speed.x,speed.y,pParticalCenter->fRadius,40,pParticalCenter->bCamera);
		SetParticalEffect(pParticalCenter->pos.x + PARTICALCENTER_WIDTH*0.5,pParticalCenter->pos.y + PARTICALCENTER_HEIGHT*0.5,speed.x,speed.y,pParticalCenter->fRadius*2,40,pParticalCenter->bCamera);
}

void ParticalExplosion(PARTICALCENTER *pParticalCenter)
{
	float fDistance = 0;                //パーティクルの飛び距離
	float fAngel = 0;                   //角度
	D3DXVECTOR2 speed;                  //パーティクルのスピード
	D3DXVECTOR2 test;
	//飛び距離範囲制限
	if(pParticalCenter->fDistance < 0)
	{
		pParticalCenter->fDistance = 0;
	}

	if(pParticalCenter->fDistance > 20)
	{
		pParticalCenter->fDistance = 20;
	}

	
	for(int nCnt = 0; nCnt < 120; nCnt++)
	{
		//-----------------------------------------------------------------------------------------
		//パーティクルの発射
		//-----------------------------------------------------------------------------------------
		fDistance = pParticalCenter->fDistance + rand()%20;       //飛び距離計算
		fAngel = pParticalCenter->fShootAngel + ( rand()%(int)(pParticalCenter->fRangeAngel*2*100) - (int)(pParticalCenter->fRangeAngel*100) ) * 0.01; //飛び角度計算
		speed.x = fDistance*cos(fAngel)/20;        //x方向のスピード
		speed.y = fDistance*sin(fAngel)/20;        //y方向のスピード

		test.x = (fDistance+10)*cos(fAngel)/20;        //x方向のスピード
		test.y = (fDistance+10)*sin(fAngel)/20;        //y方向のスピード

		//パーティクルを発射
		SetPartical(pParticalCenter->pos.x + PARTICALCENTER_WIDTH*0.5,pParticalCenter->pos.y + PARTICALCENTER_HEIGHT*0.5,speed.x,speed.y,pParticalCenter->fRadius,20,pParticalCenter->bCamera);
		SetParticalEffect(pParticalCenter->pos.x + PARTICALCENTER_WIDTH*0.5,pParticalCenter->pos.y + PARTICALCENTER_HEIGHT*0.5,speed.x,speed.y,pParticalCenter->fRadius*2,20,pParticalCenter->bCamera);
		SetPartical(pParticalCenter->pos.x + PARTICALCENTER_WIDTH*0.5,pParticalCenter->pos.y + PARTICALCENTER_HEIGHT*0.5,test.x,test.y,pParticalCenter->fRadius*2,20,pParticalCenter->bCamera);
	}

	pParticalCenter -> bUse = false;
}

EFFECTMODE *GetEffectMode(void)
{
	return &g_EffectMode;
}

//パーティクル中心セット
void SetParticalCenter(int nEffectType,float fposX,float fposY,float fRadius,float fDistance,bool bCamera)
{
	for(int nCnt = 0; nCnt < NUM_PARTICALCENTERPOLYGON; nCnt++)
	{
		if(g_ParticalCenter[nCnt].bUse == false)
		{
			//パーティクルの初期位置
			g_ParticalCenter[nCnt].pos.x = fposX;
			g_ParticalCenter[nCnt].pos.y = fposY;
			//パーティクルの初期スピード
			g_ParticalCenter[nCnt].speed.x = 1;
			g_ParticalCenter[nCnt].speed.y = 1;
			//パーティクルの古い位置
			g_ParticalCenter[nCnt].posOld.x = g_ParticalCenter[nCnt].pos.x;
			g_ParticalCenter[nCnt].posOld.y = g_ParticalCenter[nCnt].pos.y;
			//パーティクルの集中発射角度と拡散角度
			g_ParticalCenter[nCnt].fShootAngel = PI *0.5;
			g_ParticalCenter[nCnt].fRangeAngel = PI;
			//パーティクルの飛び基準距離と半径
			g_ParticalCenter[nCnt].fDistance = fDistance;
			g_ParticalCenter[nCnt].fRadius = fRadius;
			//プレーヤー使用
			g_ParticalCenter[nCnt].nEffectType = nEffectType;
			g_ParticalCenter[nCnt].bCamera = bCamera;
			g_ParticalCenter[nCnt].bUse = true;
			break;
		}
	}
}