#ifndef _MAIN_H_
#define _MAIN_H_

#pragma warning( disable: 4996 )
#pragma comment (lib,"d3d9.lib")
#pragma comment (lib,"d3dx9.lib")
#pragma comment (lib,"dxguid.lib")
#pragma comment (lib,"winmm.lib")             //システム時刻取得に必要
#pragma comment (lib,"dinput8.lib")

//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#define _USE_VOICECALLBACK_
#define DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>
#include <d3dx9.h>
#include <XAudio2.h>

//-----------------------------------------------------------------------------------------
//マクロ
//-----------------------------------------------------------------------------------------
#define SCREEN_WIDTH (800)                         //ウィンドウの幅
#define SCREEN_HEIGHT (640)                         //ウィンドウの高さ
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(memory) { if(memory != NULL) { memory -> Release(); memory = NULL; } }  //メモリのクリア
#endif
#define NUM_POLYGON (2)                             //ポリゴン数
#define PI (3.1415926535)
#define GRAVITY (0.15f)                             //重力

//-----------------------------------------------------------------------------------------
//構造体宣言
//-----------------------------------------------------------------------------------------
//頂点構造体(↑のファーマットの型に合わせること)
typedef struct
{
	D3DXVECTOR3 pos;                         //
	float rhw;								 //座標変換済み頂点フラグ(必ず1.0f)
	D3DCOLOR color;						 //頂点カラー
	D3DXVECTOR2 tex;                         //テクスチャ座標(UV or ST)
}VERTEX_2D;

typedef enum
{
	MODE_TITLE = 0,
	MODE_TUTORIAL,
	MODE_GAME,
	MODE_RESULT,
	MODE_MAX,
}MODE;

typedef struct
{
	bool bGamePause;
	bool bGameOver;
	int nGameResult;                   //リザルト:1はプレーヤーゲームオーバー 2はゲームクリア 3はゲームポーズ
}GAMEMANAGER;

//-----------------------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------------------
LPDIRECT3DDEVICE9 Get_Device(void);
void Uninit(void);                                                             //終了(後片付け関数)
//デバックの時のみ表示されたパラメータ
#ifdef _DEBUG
void DrawFPS(void);                                 //FPSの状況を表示する関数
#endif
void SetMode(MODE mode);
MODE *GetMode(void);
GAMEMANAGER *GetManager();                          //ゲーム管理フラグを返す

#endif