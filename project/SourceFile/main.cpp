/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
メッセージBOX表示処理[main.cpp]

Author :
Data   :
----------------------------------------------------------------
■Update : 2016/04/18
 ・_tWinMainをWinMainに変更
 ・WinMainのWINAPIをAPIENTRYに変更
 
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

//-----------------------------------------------------------------------------------------
//インクルードヘッド
//-----------------------------------------------------------------------------------------
#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include "main.h"
#include "BGpolygon.h"
#include "tutorial.h"
#include "enemy01.h"
#include "enemy02.h"
#include "enemyrobot.h"
#include "player.h"
#include "stage01.h"
#include "score.h"
#include "title.h"
#include "gameover.h"
#include "result.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "bullet.h"
#include "bulletEffect.h"
#include "bulletEnemy02.h"
#include "debug_hitcheck.h"

#include "particalCenter.h"
#include "partical.h"
#include "particalEffect.h"

//-----------------------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------------------
#define CLASS_NAME      "サンプル"                  //ウィンドウクラスの名前
#define WINDOW_NAME     "ウインドウ表示処理"        //ウィンドウの名前

#define ID_BUTTON000 (101)
#define ID_EDIT000 (201)
#define ID_EDIT001 (202)

#define ID_TIMER (301)
#define TIMER_INTERVAL (1000/60)

//-----------------------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------------------
LPDIRECT3D9 g_pD3D = NULL;
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;            //Direct3Dデバイスインターフェイス
int g_nCountFPS = 0;
MODE g_mode = MODE_TITLE;                         //画面遷移変数の作成
GAMEMANAGER g_GameManager;                        //ゲーム管理

//デバックのみ表示されたパラメータ
#ifdef _DEBUG
LPD3DXFONT g_pFont = NULL;                        //FPSのフォント(インターフェイス)
#endif
LPD3DXFONT g_pGuideFont = NULL;

//-----------------------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
HRESULT Init(HINSTANCE hInstance,HWND hWnd,BOOL bWindow);
void Update(void);                                                             //プロトタイプ宣言の定義
void Draw(void);                                                               //描画処理
void GameManager(void);

//-----------------------------------------------------------------------------------------
//メイン関数
// (APIENTRY : Win32API関数を呼び出す時の規約[WINAPIでも良い])
// hInstance     : このアプリケーションのインスタンスハンドル(識別子)
// hPrevInstance : 常にNULL(16bit時代の名残り)
// lpCmdLine     : コマンドラインから受け取った引数へのポインタ
// nCmdShow      : アプリケーションの初期ウインドウ表示方法の指定
//-----------------------------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	//warningを表示させないため、なんも役が立たない
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	RECT cr = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
	AdjustWindowRect(&cr,WS_OVERLAPPEDWINDOW ^ (WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX),FALSE);

	RECT dr;
	GetWindowRect(GetDesktopWindow(),&dr);

	int dw = dr.right -dr.left;
	int dh = dr.bottom -dr.top;

	int ww = cr.right - cr.left;
	int wh = cr.bottom - cr.top;

	int wx = ww > dw ? 0 : (dw-ww)/2;
	int wy = wh > dh ? 0 : (dh-wh)/2;

	WNDCLASSEX wcex = 
	{
		sizeof(WNDCLASSEX),                     //WNDCLASSEXのメモリサイズを指定
		CS_CLASSDC,                             //表示するウィンドウのスタイルを指定
		WndProc,								//ウィンドウプロシージャのアドレス(関数名)を指定
		0,										//通常は使用しないので"0"を指定
		0,										//通常は使用しないので"0"を指定
		hInstance,								//WinMainのパラメータのインスタンスハンドルを指定
		NULL,									//使用するアイコンを指定(Windowsが持っているアイコンを使う)
		LoadCursor(NULL,IDC_ARROW),				//マウスカーソルを指定
		(HBRUSH)(COLOR_WINDOW + 1),				//ウィンドウのクライアント領域の背景色を設定
		NULL,									//Windowにつけるメニューを設定
		CLASS_NAME,								//ウィンドウクラスの名前
		NULL									//小さいアイコンが設定された場合の情報を記述
	};

	//ウィンドウクラスの登録(オリジナルのウィンドウの名前の登録)
	RegisterClassEx(&wcex);

	HWND hWnd;                                  //hWnd:ウィンドウハンドル
	MSG msg;

	//ウィンドウを作成
	hWnd = CreateWindowEx(0,                    //拡張ウィンドウスタイル
		CLASS_NAME,								//ウィンドウクラスの名前
		WINDOW_NAME,							//ウィンドウの名前
		WS_OVERLAPPEDWINDOW ^ (WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX),					//ウィンドウスタイル
		wx,							            //ウィンドウの左上X座標
		wy,							            //ウィンドウの左上Y座標
		ww,							            //ウィンドウの幅
		wh,							            //ウィンドウの高さ
		NULL,									//親ウィンドウのハンドル
		NULL,									//メニューハンドルまたは子ウィンドウID
		hInstance,								//インスタンスハンドル
		NULL);									//ウィンドウ作成データ
	

	//ウィンドウの表示
	ShowWindow(hWnd,nCmdShow);					//指定されたウインドウの表示
	UpdateWindow(hWnd);							//ウィンドウの状態直ちに返す

	//初期化
	Init(hInstance,hWnd,1);

	//分解能の設定
	timeBeginPeriod(1);

	//各カウンターの初期化
	DWORD dwFrameCount = 0;
	DWORD dwCurrentTime = 0;
	DWORD dwExecLastTime = timeGetTime();                  //現在の時刻をミリ秒単位
	DWORD dwFPSLastTime = dwExecLastTime;

	SetFade(FADE_IN,MODE_TITLE);
	//メッセージループ
	for(;;)
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE) != 0 )
		{
			//Windowsの処理
			if(msg.message == WM_QUIT) break;
			else
			{
				TranslateMessage(&msg);               //呼び出し側スレッドのメッセージキューか
				DispatchMessage(&msg);                //メッセージの翻訳と送出
			}
		}

		//ゲームの処理
		else
		{
			dwCurrentTime = timeGetTime();

			if(dwCurrentTime - dwFPSLastTime >= 500)
			{
				g_nCountFPS = ( (dwFrameCount*1000) / (dwCurrentTime - dwFPSLastTime));
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}

			//1フレームの時間が経ったら一回ゲームループ
			if( (dwCurrentTime - dwExecLastTime) >= (1000/60) )
			{
				dwExecLastTime = dwCurrentTime;

				//DirectXの処理
				Update();        //更新処理
				Draw();          //描画処理

				dwFrameCount++;
			}
		}
	}

	//分解能を戻す
	timeEndPeriod(1);

	//終了処理
	Uninit();

	return (int)msg.wParam;
}

//-----------------------------------------------------------------------------------------
//ウィンドウプロシージャ
//(CALLBACK:Win32API関数を呼び出す時の規約)
//hWnd   :ウィンドウのハンドル
//uMsg   :メッセージの識別子
//wParam :メッセージの最初のパラメータ
//LParam :メッセージの2番目のパラメータ
//-----------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	static float x = 0;
	static float y = 0;

	static char buf[256];
	const RECT rect = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};

	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);							//"WM_QUIT"メッセージを返す
		break;
		
	case WM_CREATE:
		
		SetTimer(hWnd,ID_TIMER,TIMER_INTERVAL,NULL);

		break;

	case WM_COMMAND:
		
		break;

	case WM_KEYDOWN:
		break;

	case WM_CLOSE:
		//MessageBox(NULL,"CLOSE","CLOSE",MB_YESNO);
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd,uMsg,wParam,lParam);  //既定の処理を提供(ウィンドウの基本の動きを提供する(これはないと危険！))
}

//失敗した時の内容を格納させていく
HRESULT Init(HINSTANCE hInstance,HWND hWnd,BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;         //デバイスの動作を設定する構造体
	D3DDISPLAYMODE d3ddm;                //デバイスのIDirect3Device9インターフェイス

	//D3D9インターフェイスの取得
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	
	//テクスチャの読みごみ
	if(g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//現在のディスプレイモードの取得
	//FAILEDマクロ:DIRECTXプログラムが失敗か？というマクロ
	if ( FAILED( g_pD3D -> GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&d3ddm) ) )
	{
		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメーターの設定
	ZeroMemory(&d3dpp,sizeof(d3dpp));
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	d3dpp.EnableAutoDepthStencil = TRUE;                           //DepthStencilを使えます
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;                     //デブスバッファ16ビット
	d3dpp.Windowed = bWindow;                                      //ウィンドウモードかフルスクリーンモードを選択
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;    //フルスクリーンモードにおいて
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;      //フルスクリーンモードにおいて

	//Direct3Dデバイスの作成
	if(FAILED(g_pD3D -> CreateDevice(
		D3DADAPTER_DEFAULT,                                    //どの表示ハードウェアにしますか？(defaultなら自分のパソコンのスクリーン)
		D3DDEVTYPE_HAL,                                        //描画処理タイプ(ハードウェアでやるかソフトウェアでやるか？_HALならハードウェア)
		hWnd,                                                  //ウィンドウハンドル
		D3DCREATE_HARDWARE_VERTEXPROCESSING,                   //デバイス作成オプションフラグ(この場合:ハードウェアで頂点を処理する)
		&d3dpp,                                                //その他の設定
		&g_pD3DDevice)))                                       //Direct3Dデバイスインターフェイスのポインタのポインタ(間接かな)
	{
		if(FAILED(g_pD3D -> CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&d3dpp,&g_pD3DDevice)))
		{
			if(FAILED(g_pD3D -> CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_REF,hWnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&d3dpp,&g_pD3DDevice)))
			{
				MessageBox(NULL,"デバイスの取得が失敗しました","エラー",MB_OK|MB_ICONHAND);
				return E_FAIL;
			}
		}
	}

#ifdef _DEBUG
	D3DXCreateFont(g_pD3DDevice,18,0,0,0,FALSE,SHIFTJIS_CHARSET,OUT_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,"Terminal",&g_pFont);
#endif //_DEBUG
	D3DXCreateFont(g_pD3DDevice,18,0,0,0,FALSE,SHIFTJIS_CHARSET,OUT_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,"Terminal",&g_pGuideFont);

	//サンプラーステートの設定
	// レンダーステート設定
	
	g_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
	g_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	g_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_CURRENT );

	InitSound(hWnd);
	InitKeyboard(hInstance,hWnd);
	InitController(hInstance,hWnd);
	InitBGPolygon();
	InitPlayer();
	InitStage01();
	InitEnemy01();
	InitEnemy02();
	InitEnemyRobot();
	InitScorePolygon();
	InitTitle();
	InitFade();
	InitBullet();
	InitBulletEffect();
	InitBulletEnemy02();
	InitGameover();
	InitTutorial();

	InitParticalCenter();
	InitPartical();
	InitParticalEffect();

	SetMode(g_mode);
	g_GameManager.bGameOver = false;
	g_GameManager.bGamePause = false;
	g_GameManager.nGameResult = 0;

#ifdef _DEBUG
	InitHitcheck();
#endif

	srand((unsigned)time(NULL));                                //乱数の種をセット

	return S_OK;
}
//終了関数(後片づけ)
void Uninit(void)
{
	UninitKeyboard();
	UninitController();
	UninitSound();
	UninitBGPolygon();
	UninitPlayer();
	UninitStage01();
	UninitEnemy01();
	UninitEnemy02();
	UninitEnemyRobot();
	UninitScorePolygon();
	UninitTitle();
	UninitResult();
	UninitFade();
	UninitBullet();
	UninitBulletEffect();
	UninitBulletEnemy02();
	UninitGameover();
	UninitTutorial();

	UninitParticalCenter();
	UninitPartical();
	UninitParticalEffect();

#ifdef _DEBUG
	UninitHitcheck();
#endif
	
	SAFE_RELEASE (g_pD3DDevice);
	SAFE_RELEASE (g_pD3D);

#ifdef _DEBUG 
	SAFE_RELEASE(g_pFont);
#endif
	SAFE_RELEASE(g_pGuideFont);
}

////////////////////////////////////////////////
//プロトタイプ宣言の定義
////////////////////////////////////////////////
void Update(void)
{
	//キーボードによる更新処理
	UpdateKeyboard();
	UpdateController();
	UpdateFade();
	//更新処理
	switch(g_mode)
	{
	case MODE_TITLE:
		UpdateTitle();                                //タイトル
		break;
	case MODE_TUTORIAL:
		UpdateTutorial();
		break;
	case MODE_GAME:
		if( (g_GameManager.bGameOver == false) && (g_GameManager.bGamePause == false) )
		{
			UpdatePlayer();
			UpdateStage01();
			UpdateBullet();
			UpdateBulletEffect();
			UpdateEnemy01();
			UpdateEnemy02();
			UpdateEnemyRobot();
			UpdateBulletEnemy02();
			UpdateBGPolygon();
			UpdateScorePolygon();

			UpdateParticalCenter();
			UpdatePartical();
			UpdateParticalEffect();
		}
		UpdateGameover();
#ifdef _DEBUG
		UpdateHitcheck();
#endif
		break;
	case MODE_RESULT:
		UpdateResult();                              //ゲーム結果の表示
		break;
	}
}

////////////////////////////////////////////////
//描画処理
////////////////////////////////////////////////
void Draw(void)
{
	//バックバッファとZバッファのクリア
	g_pD3DDevice -> Clear(
		0,
		NULL,
		D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,          //色|深度情報
		D3DCOLOR_RGBA(0,0,0,0),                    //クリア色
		1.0f,                                      //クリア深度値(0:手前 1:一番後ろ)
		0                                          //ステンシル値(0~255)
		);

	//描画の開始
	if(SUCCEEDED(g_pD3DDevice -> BeginScene()))
	{
		switch(g_mode)
		{
		case MODE_TITLE:
			DrawTitle();                                //タイトル
			break;
		case MODE_TUTORIAL:
			DrawTutorial();
			break;
		case MODE_GAME:
			{

				DrawBGPolygon();
				DrawPlayer();
				DrawBullet();                              //弾の描画
				DrawBulletEffect();                        //弾のエフェクトの描画
				//敵の描画
				DrawEnemy01();
				DrawEnemy02();
				DrawEnemyRobot();
				DrawBulletEnemy02();                       //敵の弾の描画
				DrawScorePolygon();

				DrawPartical();
				DrawParticalEffect();

//////////////////////デバックメッセージの表示////////////////////////////
#ifdef _DEBUG
				static bool DrawHitCheckFlag = true;
				if(GetKeyboardTrigger(DIK_F1))
				{

					if(DrawHitCheckFlag == false) DrawHitCheckFlag = true;
					else DrawHitCheckFlag = false;
				}
				if(DrawHitCheckFlag == true)
				{
					DrawHitcheck();
				}
#endif
//////////////////////デバックメッセージの表示////////////////////////////

				//ゲームオーバー,クリアもしくはポーズの時の描画
				if( (g_GameManager.bGameOver == true) || (g_GameManager.bGamePause == true) )
				{
					DrawGameover();
				}
				break;
			}

		case MODE_RESULT:
			DrawResult();                              //ゲーム結果の表示
			break;
		}

		DrawFade();                                   //fadeデース
		
#ifdef _DEBUG
		DrawFPS();
#endif //_DEBUG
		//描画終了
		g_pD3DDevice -> EndScene();
	}

	g_pD3DDevice -> Present(NULL,NULL,NULL,NULL);
}

////////////////////////////////////////////////
//DIRECT3Dデバイスをゲット
////////////////////////////////////////////////
LPDIRECT3DDEVICE9 Get_Device(void)
{
	return g_pD3DDevice;
}

#ifdef _DEBUG
//フォント表示
void DrawFPS(void)
{
	char aStr[256];
	RECT rect = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
	sprintf(&aStr[0],"FPS:%d\n",g_nCountFPS);
	g_pFont -> DrawText(NULL,&aStr[0],-1,&rect,DT_LEFT,D3DCOLOR_RGBA(0,255,0,255));                  //DT_LEFTは左詰め(同類のやつと一緒に使う時,符号 | を使え,)
}
#endif  //_DEBUG

////////////////////////////////////////////////
//現在の画面モードをゲット
////////////////////////////////////////////////
MODE *GetMode(void)
{
	return &g_mode;
}

////////////////////////////////////////////////
//画面モードをセット
////////////////////////////////////////////////
void SetMode(MODE mode)
{
	//終了処理
	switch(mode)
	{
	case MODE_TITLE:
		UninitTitle();                                //タイトル
		break;

	case MODE_TUTORIAL:
		UninitTutorial();
		break;

	case MODE_GAME:
		UninitPlayer();
		UninitStage01();
		UninitEnemy01();
		UninitEnemy02();
		UninitEnemyRobot();
		UninitBullet();
		UninitBulletEffect();
		UninitBulletEnemy02();
		UninitBGPolygon();
		UninitScorePolygon();
		UninitParticalCenter();
		UninitPartical();
		UninitParticalEffect();
#ifdef _DEBUG
		UninitHitcheck();
#endif
		break;
	case MODE_RESULT:
		UninitResult();                                //タイトル
		break;
	}

	//初期化処理
	switch(mode)
	{
	case MODE_TITLE:
		InitTitle();                                //タイトル
		break;
	case MODE_TUTORIAL:
		InitTutorial();
		break;
	case MODE_GAME:
		InitPlayer();
		InitStage01();
		InitEnemy01();
		InitEnemy02();
		InitEnemyRobot();
		InitBullet();
		InitBulletEffect();
		InitBulletEnemy02();
		InitBGPolygon();
		InitScorePolygon();
		InitGameover();

		InitParticalCenter();
		InitPartical();
		InitParticalEffect();

		g_GameManager.bGameOver = false;
		g_GameManager.bGamePause = false;
		g_GameManager.nGameResult = 0;
#ifdef _DEBUG
		InitHitcheck();
#endif
		break;
	case MODE_RESULT:
		InitResult();                                //タイトル
		break;
	}

	//モード変換とフラグを立たせる
	g_mode = mode;
}

////////////////////////////////////////////////
//ゲーム管理
////////////////////////////////////////////////
void GameManager(void)
{
	if(g_mode == MODE_GAME)
	{
		if(g_GameManager.bGameOver == false)
		{
			//ゲームを一時停止
			if( GetKeyboardTrigger(DIK_ESCAPE) )
			{
				if(g_GameManager.bGamePause == false)
				{
					g_GameManager.bGamePause = true;
				}

				else
				{
					g_GameManager.bGamePause = false;
				}
			}
		}

		//ゲーム終了の知らせ
		else
		{
			if(g_GameManager.bGameOver == true)
			{
				PLAYER *pPlayer = GetPlayer();

				//プレーヤーが死んだ
				if(pPlayer -> nLife <=0)
				{
					
				}
				//敵の全滅確認
				else
				{
				
				}
			}
		}
	}
}

GAMEMANAGER *GetManager()
{
	return &g_GameManager;
}
