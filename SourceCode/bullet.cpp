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
LPDIRECT3DTEXTURE9 g_pTextureBulletPolygon[NUM_BULLETTEXTURE] = {};           //テクスチャを操作するためのポインタ(テクスチャバッファインターフェイスにアクセスためのポインタ)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferBulletPolygon = NULL;    //頂点バッファを操作するためのポインタ(頂点バッファインターフェイスにアクセスためのポインタ)

BULLET g_Bullet[MAX_BULLET];
BULLET_HITCHECK g_BulletHitcheck;                                            //弾の辺り判定範囲データ
//-----------------------------------------------------------------------------------------
//重要定番関数達

//各変数の初期化
HRESULT InitBullet(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_2D *pVtx = NULL;                           //頂点構造体のポインタ
	pD3DDevice = Get_Device();
	PLAYER *pPlayer = GetPlayer();
	 
	//頂点バッファの生成
	if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_BULLETVERTEX*MAX_BULLET, D3DUSAGE_WRITEONLY, FVF_VERTEX2D, D3DPOOL_MANAGED, &g_pVtxBufferBulletPolygon, NULL)))
	{
		return E_FAIL;
	}

	//弾のテクスチャ取得
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, BULLETPOLYGON01_TEXTURENAME, &g_pTextureBulletPolygon[0])))
	{
		MessageBox(NULL, "テクスチャの読み込みが失敗しました", "エラー", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	//プレーヤーのポリゴン
	g_pVtxBufferBulletPolygon->Lock(0, 0, (void**)&pVtx, 0);                    //仮想アドレスを取得しバッファをロックする
	MakeVertexPolygonPlayer(pD3DDevice, pVtx);

	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		//プレーヤーの諸データの初期値
		g_Bullet[nCnt].pos.x = pPlayer -> fPosX;
		g_Bullet[nCnt].pos.y = pPlayer -> fPosY;
		g_Bullet[nCnt].speed.x = 0;
		g_Bullet[nCnt].speed.y = 0;
		g_Bullet[nCnt].fAngle = 0;
		g_Bullet[nCnt].bUse = false;

		pVtx[0].pos = D3DXVECTOR3(g_Bullet[nCnt].pos.x, g_Bullet[nCnt].pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Bullet[nCnt].pos.x + BULLET_WIDTH, g_Bullet[nCnt].pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Bullet[nCnt].pos.x, g_Bullet[nCnt].pos.y + BULLET_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Bullet[nCnt].pos.x + BULLET_WIDTH, g_Bullet[nCnt].pos.y + BULLET_HEIGHT, 0.0f);

		pVtx += 4;
	}

	g_BulletHitcheck.Up = BULLET_DISTANCEY;
	g_BulletHitcheck.Bottom = BULLET_HEIGHT+BULLET_DISTANCEY;
	g_BulletHitcheck.Left = PLAYER_WIDTH/2-BULLET_DISTANCEX;
	g_BulletHitcheck.Right = BULLET_WIDTH+PLAYER_WIDTH/2-BULLET_DISTANCEX;

	g_pVtxBufferBulletPolygon->Unlock();                     //バッファのアンロック
	return S_OK;
}
//終了処理
void UninitBullet(void)
{
	// テクスチャインターフェイスの片付け
	for (int nCnt = 0; nCnt < NUM_BULLETTEXTURE; nCnt++)
	{
		SAFE_RELEASE(g_pTextureBulletPolygon[nCnt]);
	}

	//頂点バッファインターフェイスの片付け
	SAFE_RELEASE(g_pVtxBufferBulletPolygon);
}

//プレーヤーデータの更新処理
void UpdateBullet(void)
{
	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		//もし敵は使用可の状態なら
		if (g_Bullet[nCnt].bUse == true)
		{
		 	g_Bullet[nCnt].pos.x +=g_Bullet[nCnt].speed.x; 
			g_Bullet[nCnt].pos.y +=g_Bullet[nCnt].speed.y;
			g_Bullet[nCnt].nLife--;

			if(g_Bullet[nCnt].nLife < 0)
			{
				g_Bullet[nCnt].nLife = 0;
				g_Bullet[nCnt].bUse = false;
			}

			float nCenterX = g_Bullet[nCnt].pos.x + BULLET_WIDTH;
			float nCenterY = g_Bullet[nCnt].pos.y + BULLET_HEIGHT;

			//画面範囲外一定距離から離れると未使用にする
			if( (nCenterX < -200) || (nCenterX > SCREEN_WIDTH + 200) || (nCenterY < -100) || (nCenterY > SCREEN_HEIGHT + 100) )
			{
				g_Bullet[nCnt].nLife = 0;
				g_Bullet[nCnt].bUse = false;
			}
		}
	}
	UpdatePolygonBullet();
}

//プレーヤーポリゴンのデータ更新
void UpdatePolygonBullet(void)
{
	static int nRunCnt = 0;                                       //ランニングマンテクスチャの切り替えカウンター
	static int nActionFlame = 0;                                  //ランニングマンの動きを遅くするためのカウンター
	VERTEX_2D *pVtx = NULL;
	LPDIRECT3DDEVICE9 pD3DDevice;
	PLAYER *pPlayer = GetPlayer();
	pD3DDevice = Get_Device();


	g_pVtxBufferBulletPolygon -> Lock(0, 0, (void**)&pVtx, 0);          //仮想アドレスを取得しバッファをロックする

	MakeVertexPolygonBullet(pD3DDevice, pVtx);
	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		if(g_Bullet[nCnt].bUse == true)
		{
			
			SpriteRorateBullet(g_Bullet[nCnt],pVtx+nCnt*4);

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

			//バレットのエフェクトの召喚
			SetBulletEffect(g_Bullet[nCnt].pos.x + BULLET_WIDTH/2+PLAYER_WIDTH/2-BULLET_DISTANCEX,g_Bullet[nCnt].pos.y + BULLET_HEIGHT/2+BULLET_DISTANCEY,BULLETLIFE,g_Bullet[nCnt].fAngle);
		}
	}

	g_pVtxBufferBulletPolygon -> Unlock();                                     //バッファのアンロック
}

void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	//背景の鉄欄干：頂点フォーマット設定
	pD3DDevice->SetStreamSource(0, g_pVtxBufferBulletPolygon, 0, sizeof(VERTEX_2D));              //VRAMからGPUにデータをストリーム
	pD3DDevice->SetFVF(FVF_VERTEX2D);
	pD3DDevice->SetTexture(0, g_pTextureBulletPolygon[0]);				 //テクスチャの設定

	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		//もし弾は使用可の状態なら
		if (g_Bullet[nCnt].bUse == true)
		{
			pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt*4, NUM_POLYGON);                                            //テクスチャメモリの確保
		}
	}
}

//プレーヤーポリゴンの表示位置X,ポリゴンの表示位置Y,ポリゴンの幅,ポリゴンの高さ
void MakeVertexPolygonBullet(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx)
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

//弾情報取得
BULLET *GetBullet(void)
{

	return &g_Bullet[0];
}

BULLET_HITCHECK *GetBulletHitcheck(void)
{

	return &g_BulletHitcheck;
}

//弾を設置
void SetBullet(float fPosX,float fPosY,float fSpeedX,float fSpeedY,int nLife,float fAngle)
{
	for(int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		if(g_Bullet[nCnt].bUse == false)
		{
			g_Bullet[nCnt].pos.x = fPosX;
			g_Bullet[nCnt].pos.y = fPosY;
			g_Bullet[nCnt].speed.x = fSpeedX;
			g_Bullet[nCnt].speed.y = fSpeedY;
			g_Bullet[nCnt].color = D3DXCOLOR(0.0f,0.0f,0.0f,1.0f);
			g_Bullet[nCnt].nLife = nLife;
			g_Bullet[nCnt].fAngle = fAngle;
			g_Bullet[nCnt].bUse = true;
			break;
		}
	}
}

void SpriteRorateBullet (BULLET Bbullet,VERTEX_2D *pVtx)
{
	double X1, Y1, X2, Y2, X3, Y3, X4, Y4;             //左上,右下座標
	double Xc1, Yc1, Xc2, Yc2, Xc3, Yc3, Xc4, Yc4;         //回転した後の座標
	double XC, YC;                                   //中心座標

	X1 = Bbullet.pos.x + PLAYER_WIDTH/2-BULLET_DISTANCEX;
	Y1 = Bbullet.pos.y+BULLET_DISTANCEY;
	X2 = Bbullet.pos.x+BULLET_WIDTH+PLAYER_WIDTH/2-BULLET_DISTANCEX;
	Y2 = Bbullet.pos.y+BULLET_DISTANCEY;
	X3 = Bbullet.pos.x+PLAYER_WIDTH/2-BULLET_DISTANCEX;
	Y3 = Bbullet.pos.y+BULLET_HEIGHT+BULLET_DISTANCEY;
	X4 = Bbullet.pos.x+BULLET_WIDTH+PLAYER_WIDTH/2-BULLET_DISTANCEX;
	Y4 = Bbullet.pos.y+BULLET_HEIGHT+BULLET_DISTANCEY;

	XC = Bbullet.pos.x + BULLET_WIDTH/2+PLAYER_WIDTH/2-BULLET_DISTANCEX;
	YC = Bbullet.pos.y + BULLET_HEIGHT/2+BULLET_DISTANCEY;

	Xc1 = (X1 - XC)*cos(Bbullet.fAngle) - (Y1 - YC)*sin(Bbullet.fAngle) + XC;
	Yc1 = (X1 - XC)*sin(Bbullet.fAngle) + (Y1 - YC)*cos(Bbullet.fAngle) + YC;

	Xc2 = (X2 - XC)*cos(Bbullet.fAngle) - (Y2 - YC)*sin(Bbullet.fAngle) + XC;
	Yc2 = (X2 - XC)*sin(Bbullet.fAngle) + (Y2 - YC)*cos(Bbullet.fAngle) + YC;

	Xc3 = (X3 - XC)*cos(Bbullet.fAngle) - (Y3 - YC)*sin(Bbullet.fAngle) + XC;
	Yc3 = (X3 - XC)*sin(Bbullet.fAngle) + (Y3 - YC)*cos(Bbullet.fAngle) + YC;

	Xc4 = (X4 - XC)*cos(Bbullet.fAngle) - (Y4 - YC)*sin(Bbullet.fAngle) + XC;
	Yc4 = (X4 - XC)*sin(Bbullet.fAngle) + (Y4 - YC)*cos(Bbullet.fAngle) + YC;

	pVtx[0].pos = D3DXVECTOR3(Xc1,Yc1,0.0f);
	pVtx[1].pos = D3DXVECTOR3(Xc2,Yc2,0.0f);
	pVtx[2].pos = D3DXVECTOR3(Xc3,Yc3,0.0f);
	pVtx[3].pos = D3DXVECTOR3(Xc4,Yc4,0.0f);
}