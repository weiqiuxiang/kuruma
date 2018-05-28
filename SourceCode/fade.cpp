//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include <Windows.h>
#include "main.h"
#include "fade.h"
#include "player.h"
#include "input.h"

//-----------------------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureFadePolygon[NUM_FADETEXTURE] = {};           //テクスチャを操作するためのポインタ(テクスチャバッファインターフェイスにアクセスためのポインタ)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferFadePolygon[NUM_FADEPOLYGON] = {};    //頂点バッファを操作するためのポインタ(頂点バッファインターフェイスにアクセスためのポインタ) 
D3DCOLOR g_ColorFade;                                                    //フェード色
FADE g_fade;                                                              //フェード状態
MODE g_modeNext = MODE_TITLE;                                             //次のモード

////////////////////////////////////////////////
//ポリゴン(頂点)の初期化
////////////////////////////////////////////////
HRESULT InitFade(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_2DF *pVtx = NULL;                           //頂点構造体のポインタ
	pD3DDevice = Get_Device();

	//頂点バッファの生成
	if(FAILED(pD3DDevice -> CreateVertexBuffer(sizeof(VERTEX_2DF)*NUM_FADEVERTEX,D3DUSAGE_WRITEONLY,FVF_VERTEX2D,D3DPOOL_MANAGED,&g_pVtxBufferFadePolygon[0],NULL)))
	{
		return E_FAIL;
	}

	//背景テクスチャの取得(1枚)
	if ( FAILED( D3DXCreateTextureFromFile(pD3DDevice, FADEPOLYGON01_TEXTURENAME,&g_pTextureFadePolygon[0]) ) )
	{
		MessageBox(NULL,"テクスチャの読み込みが失敗しました","エラー",MB_OK|MB_ICONHAND);
		return E_FAIL;
	}

	//背景の空ポリゴン
	g_pVtxBufferFadePolygon[0] -> Lock(0,0,(void**)&pVtx,0);                    //仮想アドレスを取得しバッファをロックする

	MakeVertexPolygonFade(pD3DDevice,pVtx);
	pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[1].pos = D3DXVECTOR3(FADE_WIDTH,0.0f,0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, FADE_HEIGHT,0.0f);
	pVtx[3].pos = D3DXVECTOR3(FADE_WIDTH,FADE_HEIGHT,0.0f);

	g_pVtxBufferFadePolygon[0] -> Unlock();                                     //バッファのアンロック

	g_ColorFade = D3DXCOLOR(0.0f,0.0f,0.0f,1.0f);                               //フェードカラーの初期設定
	g_fade = FADE_IN;

	return S_OK;
}

//ポリゴンの更新
void UpdateFade(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice = Get_Device();
	PLAYER *pPlayer = GetPlayer();
	VERTEX_2DF *pVtx = NULL;                           //頂点構造体のポインタ
	static int Alpha = 255;

	if(g_fade == FADE_NONE)
	{
		return;
	}

	if(g_fade == FADE_IN)
	{
		Alpha -= FADE_RATE;   //α値を減算して後ろのお画面を浮かび上がらせる
		//フェードインの終了判定
		if(Alpha < 0 )
		{
			Alpha = 0.0f;
			g_fade = FADE_NONE;
		}
	}

	else if(g_fade == FADE_OUT)
	{
		Alpha += FADE_RATE;   //α値を加算して後ろのお画面を消していく
		//フェードインの終了判定
		if(Alpha > 255)
		{
			Alpha = 255;
			g_fade = FADE_IN;     //フェードインに切り替え
			SetMode(g_modeNext);
		}
	}
	
	g_ColorFade = D3DCOLOR_RGBA(255,255,255,Alpha);

	//フェードの計算
	g_pVtxBufferFadePolygon[0]->Lock(0, 0, (void**)&pVtx, 0);       //仮想アドレスを取得しバッファをロックする

	MakeVertexPolygonFade(pD3DDevice, pVtx);
	
	pVtx[0].pos = D3DXVECTOR3(0.0, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(FADE_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0, FADE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(FADE_WIDTH, FADE_HEIGHT, 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].color = g_ColorFade;
	pVtx[1].color = g_ColorFade;
	pVtx[2].color = g_ColorFade;
	pVtx[3].color = g_ColorFade;

	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	g_pVtxBufferFadePolygon[0]->Unlock();                                     //バッファのアンロックト
}

////////////////////////////////////////////////
//ポリゴンの描画
////////////////////////////////////////////////
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	//背景：頂点フォーマット設定
	pD3DDevice -> SetStreamSource(0,g_pVtxBufferFadePolygon[0],0,sizeof(VERTEX_2DF));                   //VRAMからGPUにデータをストリーム
	pD3DDevice -> SetFVF(FVF_VERTEX2D);
	pD3DDevice -> SetTexture(0,g_pTextureFadePolygon[0]);												 //テクスチャの設定

	//アルファブレンディングを行う
	pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	//pD3DDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
	//透過処理行う
	pD3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//半透明処理行う
	pD3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	pD3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,0,NUM_POLYGON);                                      //テクスチャ描画

}

void UninitFade(void)
{
		// テクスチャインターフェイスの片付け
		for (int nCnt = 0; nCnt < NUM_FADETEXTURE; nCnt++)
		{
			SAFE_RELEASE(g_pTextureFadePolygon[nCnt]);
		}
	
		//頂点バッファインターフェイスの片付け
		for (int nCnt = 0; nCnt < NUM_FADEPOLYGON; nCnt++)
		{
			SAFE_RELEASE(g_pVtxBufferFadePolygon[nCnt]);
		}
}

//背景ポリゴンの表示位置X,ポリゴンの表示位置Y,ポリゴンの幅,ポリゴンの高さ
void MakeVertexPolygonFade(LPDIRECT3DDEVICE9 pDevice,VERTEX_2DF *pVtx)
{
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

//フェードを設定する
void SetFade(FADE fade,MODE modeNext)
{
	g_fade = fade;
	g_modeNext = modeNext;
}

//fade状態をゲット
FADE *GetFade(void)
{
	return &g_fade;
}
