//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include <Windows.h>
#include "main.h"
#include "tutorial.h"
#include "player.h"
#include "fade.h"
#include "input.h"

//-----------------------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureTutorialPolygon[NUM_TUTORIALTEXTURE] = {};           //テクスチャを操作するためのポインタ(テクスチャバッファインターフェイスにアクセスためのポインタ)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferTutorialPolygon = NULL;                       //頂点バッファを操作するためのポインタ(頂点バッファインターフェイスにアクセスためのポインタ)
int g_nCntChange;
FADE g_TutorialFade;
int g_TutorialAlpha;

bool UpdateTutorialFade(void);                                                    //チュートリアルfadeの更新

////////////////////////////////////////////////
//ポリゴン(頂点)の初期化
////////////////////////////////////////////////
HRESULT InitTutorial(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_2D *pVtx = NULL;                           //頂点構造体のポインタ
	pD3DDevice = Get_Device();

	//頂点バッファの生成
	if(FAILED(pD3DDevice -> CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_TUTORIALVERTEX*NUM_TUTORIALPOLYGON,D3DUSAGE_WRITEONLY,FVF_VERTEX2D,D3DPOOL_MANAGED,&g_pVtxBufferTutorialPolygon,NULL)))
	{
		return E_FAIL;
	}

	//チュートリアル
	if ( FAILED( D3DXCreateTextureFromFile(pD3DDevice, TUTORIALPOLYGON01_TEXTURENAME,&g_pTextureTutorialPolygon[0]) ) )
	{
		MessageBox(NULL,"テクスチャの読み込みが失敗しました","エラー",MB_OK|MB_ICONHAND);
		return E_FAIL;
	}

	if ( FAILED( D3DXCreateTextureFromFile(pD3DDevice, TUTORIALPOLYGON02_TEXTURENAME,&g_pTextureTutorialPolygon[1]) ) )
	{
		MessageBox(NULL,"テクスチャの読み込みが失敗しました","エラー",MB_OK|MB_ICONHAND);
		return E_FAIL;
	}

	if ( FAILED( D3DXCreateTextureFromFile(pD3DDevice, TUTORIALPOLYGON03_TEXTURENAME,&g_pTextureTutorialPolygon[2]) ) )
	{
		MessageBox(NULL,"テクスチャの読み込みが失敗しました","エラー",MB_OK|MB_ICONHAND);
		return E_FAIL;
	}

	//ロック
	g_pVtxBufferTutorialPolygon -> Lock(0,0,(void**)&pVtx,0);                    //仮想アドレスを取得しバッファをロックする

	MakeVertexPolygonTutorial(pD3DDevice,pVtx);
	pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH,0.0f,0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT,0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH,SCREEN_HEIGHT,0.0f);

	pVtx += 4;

	pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH,0.0f,0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT,0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH,SCREEN_HEIGHT,0.0f);

	g_pVtxBufferTutorialPolygon -> Unlock();                                     //バッファのアンロック

	g_nCntChange = 0;
	g_TutorialFade = FADE_NONE;

	return S_OK;
}

void UpdateTutorial(void)
{
	FADE *pFade = GetFade();                              //フェードの状態をゲット
	static int nCntChange = 0;
	
	if(nCntChange >= 120)
	{
		//Xボタンで画面遷移
		if(GetControllerTrigger(0) || GetKeyboardTrigger(DIK_J))
		{
			g_TutorialFade = FADE_OUT;
			nCntChange = 0;
		}
	}

	if(*pFade == FADE_NONE) nCntChange++;               //画面遷移完了後からカウントアップ

	if(UpdateTutorialFade() == true) g_nCntChange++;    //チュートリアル自分の画面遷移

	//ゲームに遷移
	if(g_nCntChange >= 2)
	{
		SetFade(FADE_OUT,MODE_GAME);
	}

	UpdateTutorialPolygon();                             //ポリゴンテクスチャの更新
}

bool UpdateTutorialFade(void)
{
	static int Alpha = 255;
	bool flag = false;

	if(g_TutorialFade == FADE_NONE)
	{
		return flag;
	}

	if(g_TutorialFade == FADE_IN)
	{
		Alpha -= FADE_RATE;   //α値を減算して後ろのお画面を浮かび上がらせる
		//フェードインの終了判定
		if(Alpha < 0 )
		{
			Alpha = 0.0f;
			g_TutorialFade = FADE_NONE;
		}
	}

	else if(g_TutorialFade == FADE_OUT)
	{
		Alpha += FADE_RATE;   //α値を加算して後ろのお画面を消していく
		//フェードインの終了判定
		if(Alpha > 255)
		{
			Alpha = 255;
			g_TutorialFade = FADE_IN;     //フェードインに切り替え
			flag = true;
		}
	}

	g_TutorialAlpha = Alpha;         //今のα値を渡す

	return flag;
}

//ポリゴンの更新
void UpdateTutorialPolygon(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice = Get_Device();
	PLAYER *pPlayer = GetPlayer();
	VERTEX_2D *pVtx = NULL;                           //頂点構造体のポインタ
	static float fMoveCloud = 0;                                  //背景の空の移動増量

	//背景空の計算
	g_pVtxBufferTutorialPolygon->Lock(0, 0, (void**)&pVtx, 0);       //仮想アドレスを取得しバッファをロックする

	MakeVertexPolygonTutorial(pD3DDevice, pVtx);

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

	pVtx[0].color = D3DCOLOR_RGBA(255,255,255,g_TutorialAlpha);
	pVtx[1].color = D3DCOLOR_RGBA(255,255,255,g_TutorialAlpha);
	pVtx[2].color = D3DCOLOR_RGBA(255,255,255,g_TutorialAlpha);
	pVtx[3].color = D3DCOLOR_RGBA(255,255,255,g_TutorialAlpha);

	g_pVtxBufferTutorialPolygon->Unlock();                                     //バッファのアンロックト
}

////////////////////////////////////////////////
//ポリゴンの描画
////////////////////////////////////////////////
void DrawTutorial(void)
{
	static int nCnt_TutorialChange = 0;
	static int nCnt_FrameChange = 1;

	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	//背景：頂点フォーマット設定
	pD3DDevice -> SetStreamSource(0,g_pVtxBufferTutorialPolygon,0,sizeof(VERTEX_2D));                   //VRAMからGPUにデータをストリーム
	pD3DDevice -> SetFVF(FVF_VERTEX2D);

	if(g_nCntChange == 0)
	{
		pD3DDevice -> SetTexture(0,g_pTextureTutorialPolygon[0]);												 //テクスチャの設定
		pD3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,0,NUM_POLYGON);                              //テクスチャメモリの確保
	}

	if(g_nCntChange == 1)
	{
		pD3DDevice -> SetTexture(0,g_pTextureTutorialPolygon[1]);												 //テクスチャの設定
		pD3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,0,NUM_POLYGON);                              //テクスチャメモリの確保
	}

	//フェード
	pD3DDevice -> SetTexture(0,g_pTextureTutorialPolygon[2]);												 //テクスチャの設定
	pD3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,4,NUM_POLYGON);                                 //テクスチャメモリの確保
}

void UninitTutorial(void)
{
		// テクスチャインターフェイスの片付け
		for (int nCnt = 0; nCnt < NUM_TUTORIALTEXTURE; nCnt++)
		{
			SAFE_RELEASE(g_pTextureTutorialPolygon[nCnt]);
		}
	
		//頂点バッファインターフェイスの片付け
		SAFE_RELEASE(g_pVtxBufferTutorialPolygon);
}

//背景ポリゴンの表示位置X,ポリゴンの表示位置Y,ポリゴンの幅,ポリゴンの高さ
void MakeVertexPolygonTutorial(LPDIRECT3DDEVICE9 pDevice,VERTEX_2D *pVtx)
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