//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include <Windows.h>
#include "main.h"
#include "gameover.h"
#include "player.h"
#include "fade.h"
#include "input.h"
//-----------------------------------------------------------------------------------------
//マクロ
//-----------------------------------------------------------------------------------------
#define GAMEOVER_SHOW_POSX (270)             //フォント表示位置のX軸
#define GAMEOVER_SHOW_POSY (220)             //フォント表示位置のY軸
#define GAMEOVER_SHOW_ALPHA_RATE (1)         //フォント表示のαレート

#define PAUSE_POSX (210.0f)
#define PAUSE_POSY (150.0f) 
#define PAUSE_WIDTH (384.0f)
#define PAUSE_HEIGHT (256.0f) 

#define SELECT_BAR_POSX (210.0f)
#define SELECT_BAR_POSY (200.0f)

#define SELECT_BAR_WIDTH (384.0f)
#define SELECT_BAR_HEIGHT (55.0f) 

#define BAR_MOVE (55.5f)

//-----------------------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------------------
void UpdateGameoverPolygon(void);

//-----------------------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureGameoverPolygon[NUM_GAMEOVERTEXTURE] = {};           //テクスチャを操作するためのポインタ(テクスチャバッファインターフェイスにアクセスためのポインタ)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferGameoverPolygon               = NULL;         //頂点バッファを操作するためのポインタ(頂点バッファインターフェイスにアクセスためのポインタ)
int g_Alpha;                                                                      //α値をコントローラ変数
D3DCOLOR g_Color;                                						          //カラー
int g_nCntBar = 0;																  //pauseの選択バーの位置

////////////////////////////////////////////////
//ポリゴン(頂点)の初期化
////////////////////////////////////////////////
HRESULT InitGameover(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_2D *pVtx = NULL;                           //頂点構造体のポインタ
	pD3DDevice = Get_Device();

	//------------------
	//頂点バッファの生成
	//------------------
	if(FAILED(pD3DDevice -> CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_GAMEOVERVERTEX*NUM_GAMEOVERPOLYGON,D3DUSAGE_WRITEONLY,FVF_VERTEX2D,D3DPOOL_MANAGED,&g_pVtxBufferGameoverPolygon,NULL)))
	{
		return E_FAIL;
	}

	//------------------
	//テクスチャの取得
	//------------------
	if ( FAILED( D3DXCreateTextureFromFile(pD3DDevice, GAMEOVERPOLYGON01_TEXTURENAME,&g_pTextureGameoverPolygon[0]) ) )
	{
		MessageBox(NULL,"テクスチャの読み込みが失敗しました","エラー",MB_OK|MB_ICONHAND);
		return E_FAIL;
	}

	if ( FAILED( D3DXCreateTextureFromFile(pD3DDevice, PAUSEPOLYGON01_TEXTURENAME,&g_pTextureGameoverPolygon[1]) ) )
	{
		MessageBox(NULL,"テクスチャの読み込みが失敗しました","エラー",MB_OK|MB_ICONHAND);
		return E_FAIL;
	}

	if ( FAILED( D3DXCreateTextureFromFile(pD3DDevice, PAUSEPOLYGON02_TEXTURENAME,&g_pTextureGameoverPolygon[2]) ) )
	{
		MessageBox(NULL,"テクスチャの読み込みが失敗しました","エラー",MB_OK|MB_ICONHAND);
		return E_FAIL;
	}

	if ( FAILED( D3DXCreateTextureFromFile(pD3DDevice, PAUSEPOLYGON03_TEXTURENAME,&g_pTextureGameoverPolygon[3]) ) )
	{
		MessageBox(NULL,"テクスチャの読み込みが失敗しました","エラー",MB_OK|MB_ICONHAND);
		return E_FAIL;
	}

	if ( FAILED( D3DXCreateTextureFromFile(pD3DDevice, PAUSEPOLYGON04_TEXTURENAME,&g_pTextureGameoverPolygon[4]) ) )
	{
		MessageBox(NULL,"テクスチャの読み込みが失敗しました","エラー",MB_OK|MB_ICONHAND);
		return E_FAIL;
	}

	//頂点バッファロック
	g_pVtxBufferGameoverPolygon -> Lock(0,0,(void**)&pVtx,0);                    //仮想アドレスを取得しバッファをロックする

	MakeVertexPolygonGameover(pD3DDevice,pVtx);
	pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[1].pos = D3DXVECTOR3(GAMEOVER_WIDTH,0.0f,0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, GAMEOVER_HEIGHT,0.0f);
	pVtx[3].pos = D3DXVECTOR3(GAMEOVER_WIDTH,GAMEOVER_HEIGHT,0.0f);

	g_pVtxBufferGameoverPolygon -> Unlock();                                     //バッファのアンロック

	g_Alpha = 0;                                                                 //α値の初期化
	g_Color = D3DCOLOR_RGBA(255,255,255,255);                                    //カラーの初期化
	g_nCntBar = 0;                                                               //pauseの選択バーの位置

	return S_OK;
}

//ポリゴンの更新
void UpdateGameover(void)
{
	PLAYER *pPlayer = GetPlayer();
	//データ取得
	GAMEMANAGER *pGameManager = GetManager();

	//----------------
	//ゲームオーバー時
	//----------------
	if(pGameManager -> bGameOver == true)
	{
		//------------------
		//データ部分の更新
		//------------------
		g_Alpha += GAMEOVER_SHOW_ALPHA_RATE;

		if(g_Alpha > 255)
		{
			g_Alpha = 255;
			SetFade(FADE_OUT,MODE_RESULT);
		}
		g_Color = D3DCOLOR_RGBA(255,255,255,g_Alpha);

	}

	//------------
	//ゲーム実行中
	//------------
	else
	{
		//Pause監視
		if(GetControllerTrigger(7))
		{
			//ゲームは非ポーズ状態
			if(pGameManager -> bGamePause == false)
			{
				pGameManager -> bGamePause = true;
			}
			//ポーズ状態
			else
			{
				pGameManager -> bGamePause = false;
			}
		}
	}

	//--------------------
	//選択バーの位置の変更
	//--------------------
	if(pGameManager -> bGamePause == true)
	{
		if(GetControllerTrigger(4))
		{
			g_nCntBar--;
		
		}
		if(g_nCntBar < 0)
		{
			g_nCntBar = 2;
		}

		if(GetControllerTrigger(5))
		{
			g_nCntBar++;
		
		}
		if(g_nCntBar > 2)
		{
			g_nCntBar = 0;
		}

		if(GetControllerTrigger(0))
		{
			switch(g_nCntBar)
			{
			case 0:
				pGameManager -> bGamePause = false;
				break;
			case 1:
				SetFade(FADE_OUT,MODE_TITLE);   //タイトルに遷移
				break;
			case 2:
				exit(0);                        //ゲーム終了
				break;
			}
		}
	}

	else
	{
		g_nCntBar = 0;
	}

	UpdateGameoverPolygon();                         //gameoverポリゴンの更新
}

void UpdateGameoverPolygon(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice = Get_Device();
	VERTEX_2D *pVtx = NULL;                           //頂点構造体のポインタ
	GAMEMANAGER *pGameManager = GetManager();

	//------------------
	//ポリゴン部分の更新
	//------------------
	g_pVtxBufferGameoverPolygon->Lock(0, 0, (void**)&pVtx, 0);       //仮想アドレスを取得しバッファをロックする

	MakeVertexPolygonGameover(pD3DDevice, pVtx);

	pVtx[0].pos = D3DXVECTOR3(GAMEOVER_SHOW_POSX,GAMEOVER_SHOW_POSY,0.0f);
	pVtx[1].pos = D3DXVECTOR3(GAMEOVER_SHOW_POSX+GAMEOVER_WIDTH,GAMEOVER_SHOW_POSY,0.0f);
	pVtx[2].pos = D3DXVECTOR3(GAMEOVER_SHOW_POSX, GAMEOVER_HEIGHT+GAMEOVER_SHOW_POSY,0.0f);
	pVtx[3].pos = D3DXVECTOR3(GAMEOVER_SHOW_POSX+GAMEOVER_WIDTH,GAMEOVER_HEIGHT+GAMEOVER_SHOW_POSY,0.0f);

	//ゲームオーバー
	if(pGameManager -> nGameResult == 1)
	{
		pVtx[0].tex = D3DXVECTOR2(0.0f,0.5f);
		pVtx[1].tex = D3DXVECTOR2(1.0f,0.5f);
		pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);
	}

	//ゲームクリア
	if(pGameManager -> nGameResult == 2)
	{
		pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f,0.5f);
		pVtx[3].tex = D3DXVECTOR2(1.0f,0.5f);
	}

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].color = g_Color;
	pVtx[1].color = g_Color;
	pVtx[2].color = g_Color;
	pVtx[3].color = g_Color;

	pVtx += 4;

	//pause外黒枠
	pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH,0.0f,0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f,SCREEN_HEIGHT,0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH,SCREEN_HEIGHT,0.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].color = D3DCOLOR_RGBA(255,255,255,127);
	pVtx[1].color = D3DCOLOR_RGBA(255,255,255,127);
	pVtx[2].color = D3DCOLOR_RGBA(255,255,255,127);
	pVtx[3].color = D3DCOLOR_RGBA(255,255,255,127);

	pVtx += 4;

	//pause枠
	pVtx[0].pos = D3DXVECTOR3(PAUSE_POSX,PAUSE_POSY,0.0f);
	pVtx[1].pos = D3DXVECTOR3(PAUSE_POSX+PAUSE_WIDTH,PAUSE_POSY,0.0f);
	pVtx[2].pos = D3DXVECTOR3(PAUSE_POSX,PAUSE_POSY+PAUSE_HEIGHT,0.0f);
	pVtx[3].pos = D3DXVECTOR3(PAUSE_POSX+PAUSE_WIDTH,PAUSE_POSY+PAUSE_HEIGHT,0.0f);

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

	//pauseフォント
	pVtx[0].pos = D3DXVECTOR3(PAUSE_POSX,PAUSE_POSY,0.0f);
	pVtx[1].pos = D3DXVECTOR3(PAUSE_POSX+PAUSE_WIDTH,PAUSE_POSY,0.0f);
	pVtx[2].pos = D3DXVECTOR3(PAUSE_POSX,PAUSE_POSY+PAUSE_HEIGHT,0.0f);
	pVtx[3].pos = D3DXVECTOR3(PAUSE_POSX+PAUSE_WIDTH,PAUSE_POSY+PAUSE_HEIGHT,0.0f);

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

	//選択バー
	pVtx[0].pos = D3DXVECTOR3(SELECT_BAR_POSX,SELECT_BAR_POSY+BAR_MOVE*g_nCntBar,0.0f);
	pVtx[1].pos = D3DXVECTOR3(SELECT_BAR_POSX+SELECT_BAR_WIDTH,SELECT_BAR_POSY+BAR_MOVE*g_nCntBar,0.0f);
	pVtx[2].pos = D3DXVECTOR3(SELECT_BAR_POSX,SELECT_BAR_POSY+SELECT_BAR_HEIGHT+BAR_MOVE*g_nCntBar,0.0f);
	pVtx[3].pos = D3DXVECTOR3(SELECT_BAR_POSX+SELECT_BAR_WIDTH,SELECT_BAR_POSY+SELECT_BAR_HEIGHT+BAR_MOVE*g_nCntBar,0.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].color = D3DCOLOR_RGBA(255,255,0,127);
	pVtx[1].color = D3DCOLOR_RGBA(255,255,0,127);
	pVtx[2].color = D3DCOLOR_RGBA(255,255,0,127);
	pVtx[3].color = D3DCOLOR_RGBA(255,255,0,127);

	g_pVtxBufferGameoverPolygon->Unlock();                                     //バッファのアンロックト
}

////////////////////////////////////////////////
//ポリゴンの描画
////////////////////////////////////////////////
void DrawGameover(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	//背景：頂点フォーマット設定
	pD3DDevice -> SetStreamSource(0,g_pVtxBufferGameoverPolygon,0,sizeof(VERTEX_2D));                   //VRAMからGPUにデータをストリーム
	pD3DDevice -> SetFVF(FVF_VERTEX2D);

	pD3DDevice -> SetTexture(0,g_pTextureGameoverPolygon[0]);												 //テクスチャの設定
	pD3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,0,NUM_POLYGON);                                 //テクスチャメモリの確保

	//データ取得
	GAMEMANAGER *pGameManager = GetManager();

	//PAUSE画面表示
	if(pGameManager -> bGamePause == true)
	{
		//PAUSE外黒枠
		pD3DDevice -> SetTexture(0,g_pTextureGameoverPolygon[1]);
		pD3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,4,NUM_POLYGON);

		//PAUSE枠
		pD3DDevice -> SetTexture(0,g_pTextureGameoverPolygon[2]);
		pD3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,8,NUM_POLYGON);

		//PAUSEフォント
		pD3DDevice -> SetTexture(0,g_pTextureGameoverPolygon[3]);
		pD3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,12,NUM_POLYGON);

		pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);                                       //加算合成を行う!

		//選択バー
		pD3DDevice -> SetTexture(0,g_pTextureGameoverPolygon[4]);
		pD3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,16,NUM_POLYGON);

		pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);                               //ブレンドモードをアルファブレンドに戻す
	}
}

void UninitGameover(void)
{
		// テクスチャインターフェイスの片付け
		for (int nCnt = 0; nCnt < NUM_GAMEOVERTEXTURE; nCnt++)
		{
			if(g_pTextureGameoverPolygon[nCnt] != NULL)
			{
				g_pTextureGameoverPolygon[nCnt]->Release();
				g_pTextureGameoverPolygon[nCnt] = NULL;
			}
		}
	
		//頂点バッファインターフェイスの片付け
		if( g_pVtxBufferGameoverPolygon != NULL)
		{
			g_pVtxBufferGameoverPolygon -> Release();
			g_pVtxBufferGameoverPolygon = NULL;
		}
}

//背景ポリゴンの表示位置X,ポリゴンの表示位置Y,ポリゴンの幅,ポリゴンの高さ
void MakeVertexPolygonGameover(LPDIRECT3DDEVICE9 pDevice,VERTEX_2D *pVtx)
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