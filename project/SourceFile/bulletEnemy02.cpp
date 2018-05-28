//-----------------------------------------------------------------------------------------
//プレーヤー,道具などの処理[running02.cpp]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------------------
#include <time.h>
#include "main.h"
#include "bulletEnemy02.h"
#include "player.h"

//-----------------------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureBulletEnemy02Polygon[NUM_BULLETENEMY02TEXTURE] = {};           //テクスチャを操作するためのポインタ(テクスチャバッファインターフェイスにアクセスためのポインタ)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferBulletEnemy02Polygon = NULL;    //頂点バッファを操作するためのポインタ(頂点バッファインターフェイスにアクセスためのポインタ)

BULLETENEMY02 g_BulletEnemy02[MAX_BULLETENEMY02];

//-----------------------------------------------------------------------------------------
//重要定番関数達

//各変数の初期化
HRESULT InitBulletEnemy02(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_2D *pVtx = NULL;                           //頂点構造体のポインタ
	pD3DDevice = Get_Device();
	PLAYER *pPlayer = GetPlayer();
	 
	//頂点バッファの生成
	if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_BULLETENEMY02VERTEX*MAX_BULLETENEMY02, D3DUSAGE_WRITEONLY, FVF_VERTEX2D, D3DPOOL_MANAGED, &g_pVtxBufferBulletEnemy02Polygon, NULL)))
	{
		return E_FAIL;
	}

	//弾のテクスチャ取得
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, BULLETENEMY02POLYGON01_TEXTURENAME, &g_pTextureBulletEnemy02Polygon[0])))
	{
		MessageBox(NULL, "テクスチャの読み込みが失敗しました", "エラー", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	//プレーヤーのポリゴン
	g_pVtxBufferBulletEnemy02Polygon->Lock(0, 0, (void**)&pVtx, 0);                    //仮想アドレスを取得しバッファをロックする
	MakeVertexPolygonPlayer(pD3DDevice, pVtx);

	for (int nCnt = 0; nCnt < MAX_BULLETENEMY02; nCnt++)
	{
		//プレーヤーの諸データの初期値
		g_BulletEnemy02[nCnt].pos.x = pPlayer -> fPosX;
		g_BulletEnemy02[nCnt].pos.y = pPlayer -> fPosY;
		g_BulletEnemy02[nCnt].speed.x = 0;
		g_BulletEnemy02[nCnt].speed.y = 0;
		g_BulletEnemy02[nCnt].fAngle = 0;
		g_BulletEnemy02[nCnt].fRadius = 12;
		g_BulletEnemy02[nCnt].nUnit = 0;
		g_BulletEnemy02[nCnt].bUse = false;

		pVtx[0].pos = D3DXVECTOR3(g_BulletEnemy02[nCnt].pos.x, g_BulletEnemy02[nCnt].pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_BulletEnemy02[nCnt].pos.x + BULLETENEMY02_WIDTH, g_BulletEnemy02[nCnt].pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_BulletEnemy02[nCnt].pos.x, g_BulletEnemy02[nCnt].pos.y + BULLETENEMY02_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_BulletEnemy02[nCnt].pos.x + BULLETENEMY02_WIDTH, g_BulletEnemy02[nCnt].pos.y + BULLETENEMY02_HEIGHT, 0.0f);

		pVtx += 4;
	}

	g_pVtxBufferBulletEnemy02Polygon->Unlock();                     //バッファのアンロック
	return S_OK;
}
//終了処理
void UninitBulletEnemy02(void)
{
	// テクスチャインターフェイスの片付け
	for (int nCnt = 0; nCnt < NUM_BULLETENEMY02TEXTURE; nCnt++)
	{
		SAFE_RELEASE(g_pTextureBulletEnemy02Polygon[nCnt]);
	}

	//頂点バッファインターフェイスの片付け
	SAFE_RELEASE(g_pVtxBufferBulletEnemy02Polygon);
}

//プレーヤーデータの更新処理
void UpdateBulletEnemy02(void)
{
	PLAYER *pPlayer = GetPlayer();                //プレーヤーステータス取得

	for (int nCnt = 0; nCnt < MAX_BULLETENEMY02; nCnt++)
	{
		//もし敵は使用可の状態なら
		if (g_BulletEnemy02[nCnt].bUse == true)
		{
			//-----------------------------------------------------------
			//敵の弾の動作の共通部分
			//-----------------------------------------------------------
			//スピードのによって座標の計算
			g_BulletEnemy02[nCnt].pos.x +=g_BulletEnemy02[nCnt].speed.x; 
			g_BulletEnemy02[nCnt].pos.y +=g_BulletEnemy02[nCnt].speed.y;

			//敵の弾のライフの計算
			g_BulletEnemy02[nCnt].nLife--;

			if(g_BulletEnemy02[nCnt].nLife < 0)
			{
				g_BulletEnemy02[nCnt].nLife = 0;
				g_BulletEnemy02[nCnt].bUse = false;
			}

			//-----------------------------------------------------------
			//Enemy01の弾の動作
			//-----------------------------------------------------------
			if(g_BulletEnemy02[nCnt].nUnit == 1)
			{

			}

			//-----------------------------------------------------------
			//Enemy02の弾の動作
			//-----------------------------------------------------------
			else if(g_BulletEnemy02[nCnt].nUnit == 2)
			{
				g_BulletEnemy02[nCnt].speed.y += GRAVITY;                  //毎フレーム重力加算
			}

			//-----------------------------------------------------------
			//EnemyRobotの弾の動作
			//-----------------------------------------------------------
			else
			{
				
			}

			//-----------------------------------------------------------
			//敵の弾の動作の共通部分
			//-----------------------------------------------------------
			float nCenterX = g_BulletEnemy02[nCnt].pos.x + BULLETENEMY02_WIDTH - pPlayer -> fPosX_SUM;
			float nCenterY = g_BulletEnemy02[nCnt].pos.y + BULLETENEMY02_HEIGHT;

			//画面範囲外出たら未使用にする
			if( (nCenterX < -200) || (nCenterX > 1200) || (nCenterY < -100) || (nCenterY > 900) )
			{
				g_BulletEnemy02[nCnt].nLife = 0;
				g_BulletEnemy02[nCnt].bUse = false;
			}
		}
	}
	UpdatePolygonBulletEnemy02();
}

//プレーヤーポリゴンのデータ更新
void UpdatePolygonBulletEnemy02(void)
{
	static int nRunCnt = 0;                                       //ランニングマンテクスチャの切り替えカウンター
	static int nActionFlame = 0;                                  //ランニングマンの動きを遅くするためのカウンター
	VERTEX_2D *pVtx = NULL;
	LPDIRECT3DDEVICE9 pD3DDevice;
	PLAYER *pPlayer = GetPlayer();
	pD3DDevice = Get_Device();

	g_pVtxBufferBulletEnemy02Polygon -> Lock(0, 0, (void**)&pVtx, 0);          //仮想アドレスを取得しバッファをロックする

	MakeVertexPolygonBulletEnemy02(pD3DDevice, pVtx);
	for (int nCnt = 0; nCnt < MAX_BULLETENEMY02; nCnt++)
	{
		if(g_BulletEnemy02[nCnt].bUse == true)
		{
			pVtx[0+nCnt*4].pos = D3DXVECTOR3(g_BulletEnemy02[nCnt].pos.x - pPlayer -> fPosX_SUM - g_BulletEnemy02[nCnt].fRadius, g_BulletEnemy02[nCnt].pos.y - g_BulletEnemy02[nCnt].fRadius, 0.0f);
			pVtx[1+nCnt*4].pos = D3DXVECTOR3(g_BulletEnemy02[nCnt].pos.x - pPlayer -> fPosX_SUM + g_BulletEnemy02[nCnt].fRadius, g_BulletEnemy02[nCnt].pos.y - g_BulletEnemy02[nCnt].fRadius, 0.0f);
			pVtx[2+nCnt*4].pos = D3DXVECTOR3(g_BulletEnemy02[nCnt].pos.x - pPlayer -> fPosX_SUM - g_BulletEnemy02[nCnt].fRadius, g_BulletEnemy02[nCnt].pos.y + g_BulletEnemy02[nCnt].fRadius, 0.0f);
			pVtx[3+nCnt*4].pos = D3DXVECTOR3(g_BulletEnemy02[nCnt].pos.x - pPlayer -> fPosX_SUM + g_BulletEnemy02[nCnt].fRadius, g_BulletEnemy02[nCnt].pos.y + g_BulletEnemy02[nCnt].fRadius, 0.0f);

			pVtx[0+nCnt*4].rhw = 1.0f;
			pVtx[1+nCnt*4].rhw = 1.0f;
			pVtx[2+nCnt*4].rhw = 1.0f;
			pVtx[3+nCnt*4].rhw = 1.0f;

			pVtx[0+nCnt*4].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1+nCnt*4].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2+nCnt*4].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3+nCnt*4].color = D3DCOLOR_RGBA(255, 255, 255, 255);

			pVtx[0+nCnt*4].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1+nCnt*4].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2+nCnt*4].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3+nCnt*4].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
	}

	g_pVtxBufferBulletEnemy02Polygon -> Unlock();                                     //バッファのアンロック
}

void DrawBulletEnemy02(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	//背景の鉄欄干：頂点フォーマット設定
	pD3DDevice->SetStreamSource(0, g_pVtxBufferBulletEnemy02Polygon, 0, sizeof(VERTEX_2D));              //VRAMからGPUにデータをストリーム
	pD3DDevice->SetFVF(FVF_VERTEX2D);
	pD3DDevice->SetTexture(0, g_pTextureBulletEnemy02Polygon[0]);				 //テクスチャの設定

	for (int nCnt = 0; nCnt < MAX_BULLETENEMY02; nCnt++)
	{
		//もし弾は使用可の状態なら
		if (g_BulletEnemy02[nCnt].bUse == true)
		{
			pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt*4, NUM_POLYGON);                                            //テクスチャメモリの確保
		}
	}
}

//プレーヤーポリゴンの表示位置X,ポリゴンの表示位置Y,ポリゴンの幅,ポリゴンの高さ
void MakeVertexPolygonBulletEnemy02(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx)
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
BULLETENEMY02 *GetBulletEnemy02(void)
{

	return &g_BulletEnemy02[0];
}

//弾を設置
void SetBulletEnemy02(int nUnit,float fPosX,float fPosY,float fSpeedX,float fSpeedY,int nLife,float fAngle,float fRadius)
{
	for(int nCnt = 0; nCnt < MAX_BULLETENEMY02; nCnt++)
	{
		if(g_BulletEnemy02[nCnt].bUse == false)
		{
			g_BulletEnemy02[nCnt].nUnit = nUnit;
			g_BulletEnemy02[nCnt].pos.x = fPosX;
			g_BulletEnemy02[nCnt].pos.y = fPosY;
			g_BulletEnemy02[nCnt].speed.x = fSpeedX;
			g_BulletEnemy02[nCnt].speed.y = fSpeedY;
			g_BulletEnemy02[nCnt].color = D3DXCOLOR(0.0f,0.0f,0.0f,1.0f);
			g_BulletEnemy02[nCnt].nLife = nLife;
			g_BulletEnemy02[nCnt].fAngle = fAngle;
			g_BulletEnemy02[nCnt].fRadius = fRadius;
			g_BulletEnemy02[nCnt].bUse = true;
			break;
		}
	}
}
