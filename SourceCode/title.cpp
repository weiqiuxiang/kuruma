//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include <Windows.h>
#include <string.h>
#include "main.h"
#include "title.h"
#include "player.h"
#include "input.h"
#include "fade.h"
#include <math.h>

//-----------------------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureTitlePolygon[NUM_TITLETEXTURE] = {};           //テクスチャを操作するためのポインタ(テクスチャバッファインターフェイスにアクセスためのポインタ)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferTitlePolygon = NULL;    //頂点バッファを操作するためのポインタ(頂点バッファインターフェイスにアクセスためのポインタ)
bool g_StickInputYes = false;

////////////////////////////////////////////////
//ポリゴン(頂点)の初期化
////////////////////////////////////////////////
HRESULT InitTitle(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_2D *pVtx = NULL;                           //頂点構造体のポインタ
	pD3DDevice = Get_Device();

	//頂点バッファの生成
	if(FAILED(pD3DDevice -> CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_TITLEVERTEX*NUM_TITLEPOLYGON,D3DUSAGE_WRITEONLY,FVF_VERTEX2D,D3DPOOL_MANAGED,&g_pVtxBufferTitlePolygon,NULL)))
	{
		return E_FAIL;
	}

	//タイトルテクスチャの取得(5枚)

	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, TITLEPOLYGON00_TEXTURENAME, &g_pTextureTitlePolygon[0])))
	{
		MessageBox(NULL, "テクスチャの読み込みが失敗しました", "エラー", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, TITLEPOLYGON01_TEXTURENAME, &g_pTextureTitlePolygon[1])))
	{
		MessageBox(NULL, "テクスチャの読み込みが失敗しました", "エラー", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, TITLEPOLYGON02_TEXTURENAME, &g_pTextureTitlePolygon[2])))
	{
		MessageBox(NULL, "テクスチャの読み込みが失敗しました", "エラー", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	//タイトルポリゴン
	g_pVtxBufferTitlePolygon -> Lock(0,0,(void**)&pVtx,0);                    //仮想アドレスを取得しバッファをロックする
	MakeVertexPolygonTitle(pD3DDevice,&pVtx[0]);
	pVtx[0].pos = D3DXVECTOR3(0,0,0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH,0,0.0f);
	pVtx[2].pos = D3DXVECTOR3(0,SCREEN_HEIGHT,0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH,SCREEN_HEIGHT,0.0f);

	pVtx += 4;

	//モードの選択アイコン
	pVtx[0].pos = D3DXVECTOR3(ICON_POSX,ICON_POSY,0.0f);
	pVtx[1].pos = D3DXVECTOR3(ICON_POSX+WIDTH_CURSOR,ICON_POSY,0.0f);
	pVtx[2].pos = D3DXVECTOR3(ICON_POSX,ICON_POSY+HEIGHT_CURSOR,0.0f);
	pVtx[3].pos = D3DXVECTOR3(ICON_POSX+WIDTH_CURSOR,ICON_POSY+HEIGHT_CURSOR,0.0f);

	g_pVtxBufferTitlePolygon -> Unlock();                                     //バッファのアンロック

	g_StickInputYes = false;

	return S_OK;
}

//ポリゴンの更新
void UpdateTitle(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice = Get_Device();
	VERTEX_2D *pVtx = NULL;                           //頂点構造体のポインタ

	//タイトルポリゴン
	g_pVtxBufferTitlePolygon -> Lock(0,0,(void**)&pVtx,0);                    //仮想アドレスを取得しバッファをロックする

	MakeVertexPolygonTitle(pD3DDevice,&pVtx[0]);

	pVtx[0].pos = D3DXVECTOR3(0,0,0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH,0,0.0f);
	pVtx[2].pos = D3DXVECTOR3(0,SCREEN_HEIGHT,0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH,SCREEN_HEIGHT,0.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[1].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[2].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[3].color = D3DCOLOR_RGBA(255,255,255,255);

	pVtx += 4;

	//モードの選択アイコン
	pVtx[0].pos = D3DXVECTOR3(ICON_POSX,ICON_POSY,0.0f);
	pVtx[1].pos = D3DXVECTOR3(ICON_POSX+WIDTH_CURSOR,ICON_POSY,0.0f);
	pVtx[2].pos = D3DXVECTOR3(ICON_POSX,ICON_POSY+HEIGHT_CURSOR,0.0f);
	pVtx[3].pos = D3DXVECTOR3(ICON_POSX+WIDTH_CURSOR,ICON_POSY+HEIGHT_CURSOR,0.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[1].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[2].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[3].color = D3DCOLOR_RGBA(255,255,255,255);

	//test
	DIJOYSTATE *pjs = GetJS();

	//カーソルのポリゴン
	static int nLoop = 0;												//メニュー選択用変数

	if (fabs((double)pjs->lY) < 500) { g_StickInputYes = false; }
	if (!g_StickInputYes) {
		if (pjs->lY < -500) { nLoop--; g_StickInputYes = true; }
		if (pjs->lY > 500) { nLoop++; g_StickInputYes = true;}
	}
	if (GetKeyboardTrigger(DIK_W)) nLoop--;
	if (GetKeyboardTrigger(DIK_S)) nLoop++;
	if (nLoop < 0) nLoop = 1;
	nLoop = nLoop % 2;

	if(nLoop == 0)
	{
		pVtx[0].pos = D3DXVECTOR3(START_POSX - DISTANCE_FROM_START,START_POSY,0.0f);
		pVtx[1].pos = D3DXVECTOR3(START_POSX - DISTANCE_FROM_START+WIDTH_CURSOR,START_POSY,0.0f);
		pVtx[2].pos = D3DXVECTOR3(START_POSX - DISTANCE_FROM_START,START_POSY+HEIGHT_CURSOR,0.0f);
		pVtx[3].pos = D3DXVECTOR3(START_POSX - DISTANCE_FROM_START+WIDTH_CURSOR,START_POSY+HEIGHT_CURSOR,0.0f);

		if(GetControllerTrigger(0) || GetKeyboardTrigger(DIK_J))
		{
			SetFade(FADE_OUT,MODE_TUTORIAL);
		}
	}

	else
	{
		pVtx[0].pos = D3DXVECTOR3(EXIT_POSX - DISTANCE_FROM_EXIT,EXIT_POSY,0.0f);
		pVtx[1].pos = D3DXVECTOR3(EXIT_POSX - DISTANCE_FROM_EXIT+WIDTH_CURSOR,EXIT_POSY,0.0f);
		pVtx[2].pos = D3DXVECTOR3(EXIT_POSX - DISTANCE_FROM_EXIT,EXIT_POSY+HEIGHT_CURSOR,0.0f);
		pVtx[3].pos = D3DXVECTOR3(EXIT_POSX - DISTANCE_FROM_EXIT+WIDTH_CURSOR,EXIT_POSY+HEIGHT_CURSOR,0.0f);

		if(GetControllerTrigger(0) || GetKeyboardTrigger(DIK_J))
		{
			Uninit();                     //終了処理
			exit(0);
		}
	}

	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[1].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[2].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[3].color = D3DCOLOR_RGBA(255,255,255,255);

	pVtx += 4;

	pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH,0.0f,0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT,0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH,SCREEN_HEIGHT,0.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[1].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[2].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[3].color = D3DCOLOR_RGBA(255,255,255,255);

	g_pVtxBufferTitlePolygon -> Unlock();                                     //バッファのアンロック
}

////////////////////////////////////////////////
//ポリゴンの描画
////////////////////////////////////////////////
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	//頂点フォーマット設定
	pD3DDevice -> SetStreamSource(0,g_pVtxBufferTitlePolygon,0,sizeof(VERTEX_2D));                   //VRAMからGPUにデータをストリーム
	pD3DDevice -> SetFVF(FVF_VERTEX2D);

	pD3DDevice -> SetTexture(0,g_pTextureTitlePolygon[NUM_TITLETEXTURE-1]);
	pD3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,(NUM_TITLETEXTURE-1)*4,NUM_POLYGON);

	for(int nCnt = 0; nCnt < NUM_TITLETEXTURE-1; nCnt++)
	{
		pD3DDevice -> SetTexture(0,g_pTextureTitlePolygon[nCnt]);
		pD3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,nCnt*4,NUM_POLYGON);                                  //テクスチャメモリの確保
	}
}

//終了関数
void UninitTitle(void)
{
	// テクスチャインターフェイスの片付け
	for(int nCnt = 0; nCnt < NUM_TITLETEXTURE; nCnt++)
	{
		SAFE_RELEASE(g_pTextureTitlePolygon[nCnt]);
	}

	//頂点バッファインターフェイスの片付け
	SAFE_RELEASE(g_pVtxBufferTitlePolygon);
}

//背景ポリゴンの表示位置X,ポリゴンの表示位置Y,ポリゴンの幅,ポリゴンの高さ
void MakeVertexPolygonTitle(LPDIRECT3DDEVICE9 pDevice,VERTEX_2D *pVtx)
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