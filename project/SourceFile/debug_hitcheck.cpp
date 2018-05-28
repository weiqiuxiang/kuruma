#ifdef _DEBUG
//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include <Windows.h>
#include "main.h"
#include "debug_Hitcheck.h"
#include "player.h"
#include "enemy01.h"
#include "enemy02.h"
#include "enemyrobot.h"
#include "bullet.h"
#include "bulletEnemy02.h"
#include "fade.h"
#include "input.h"

//-----------------------------------------------------------------------------------------
//構造体宣言
//-----------------------------------------------------------------------------------------
typedef struct
{
	PLAYER *pPlayer;
	ENEMY01 *pEnemy01;
	ENEMY02 *pEnemy02;
	ENEMYROBOT *pEnemyRobot;
	BULLET *pBullet;
	BULLET_HITCHECK *pBulletHitcheck;
	BULLETENEMY02 *pEnemyBullet;
}PLAYDATA;

//-----------------------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------------------
void DataSet(void);

//-----------------------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferHitcheckPolygon[NUM_HITCHECKPOLYGON] = {};    //頂点バッファを操作するためのポインタ(頂点バッファインターフェイスにアクセスためのポインタ) 
void MakeVertexPolygonHitcheck(LPDIRECT3DDEVICE9 pDevice,VERTEX_2DF *pVtx);       //ポリゴン設定
D3DCOLOR g_ColorHitcheck;                                                         //色
HITCHECK g_HitCheck[NUM_HITCHECKPOLYGON];
PLAYDATA g_PlayData;

////////////////////////////////////////////////
//ポリゴン(頂点)の初期化
////////////////////////////////////////////////
HRESULT InitHitcheck(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_2DF *pVtx = NULL;                           //頂点構造体のポインタ
	pD3DDevice = Get_Device();

	//頂点バッファの生成
	if(FAILED(pD3DDevice -> CreateVertexBuffer(sizeof(VERTEX_2DF)*NUM_HITCHECKVERTEX*NUM_HITCHECKPOLYGON,D3DUSAGE_WRITEONLY,FVF_VERTEX2D,D3DPOOL_MANAGED,&g_pVtxBufferHitcheckPolygon[0],NULL)))
	{
		return E_FAIL;
	}

	//背景の空ポリゴン
	g_pVtxBufferHitcheckPolygon[0] -> Lock(0,0,(void**)&pVtx,0);                    //仮想アドレスを取得しバッファをロックする
	MakeVertexPolygonHitcheck(pD3DDevice,pVtx);
	for(int nCnt = 0; nCnt < NUM_HITCHECKPOLYGON;nCnt++)
	{
		g_HitCheck[nCnt].Up = 0.0f;
		g_HitCheck[nCnt].Bottom = 0.0f;
		g_HitCheck[nCnt].Left = 0.0f;
		g_HitCheck[nCnt].Right = 0.0f;
		g_HitCheck[nCnt].bUse = false;

		pVtx[0 + 4*nCnt].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
		pVtx[1 + 4*nCnt].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
		pVtx[2 + 4*nCnt].pos = D3DXVECTOR3(0.0f, 0.0f,0.0f);
		pVtx[3 + 4*nCnt].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	}

	g_pVtxBufferHitcheckPolygon[0] -> Unlock();                                     //バッファのアンロック

	g_ColorHitcheck = D3DXCOLOR(0.0f,0.0f,0.0f,1.0f);                               //フェードカラーの初期設定

	DataSet();                                                                      //各データへのリング

	return S_OK;
}

//ポリゴンの更新
void UpdateHitcheck(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice = Get_Device();
	PLAYER *pPlayer = GetPlayer();
	VERTEX_2DF *pVtx = NULL;                           //頂点構造体のポインタ

	g_ColorHitcheck = D3DCOLOR_RGBA(0,255,0,100);

	//----------------
	//敵のデータを取得
	//----------------
	//Enemy01
	for (int nCnt = 0; nCnt < MAX_ENEMY01; nCnt++)
	{
		if ( (g_PlayData.pEnemy01+nCnt) -> bUse == true)
		{
			SetHitCheck( (g_PlayData.pEnemy01+nCnt) -> fPosY + ENEMY01_TOP,										           //↑の座標
				         (g_PlayData.pEnemy01+nCnt) -> fPosY + ENEMY01_HEIGHT - ENEMY01_HEIGHT_CHECK,			           //↓の座標
						 (g_PlayData.pEnemy01+nCnt) -> fPosX - pPlayer->fPosX_SUM + ENEMY01_HORRIZON,					   //←の座標
						 (g_PlayData.pEnemy01+nCnt) -> fPosX - pPlayer->fPosX_SUM + ENEMY01_WIDTH - ENEMY01_HORRIZON);	   //→の座標
		}
	}

	//Enemy02
	for (int nCnt = 0; nCnt < MAX_ENEMY02; nCnt++)
	{
		if ( (g_PlayData.pEnemy02+nCnt) -> bUse == true)
		{
			SetHitCheck( (g_PlayData.pEnemy02+nCnt) -> fPosY + ENEMY02_TOP,										           //↑の座標
				         (g_PlayData.pEnemy02+nCnt) -> fPosY + ENEMY02_HEIGHT - ENEMY02_HEIGHT_CHECK,			           //↓の座標
						 (g_PlayData.pEnemy02+nCnt) -> fPosX - pPlayer->fPosX_SUM + ENEMY02_HORRIZON,					   //←の座標
						 (g_PlayData.pEnemy02+nCnt) -> fPosX - pPlayer->fPosX_SUM + ENEMY02_WIDTH - ENEMY02_HORRIZON);	   //→の座標
		}
	}

	//Bullet
	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		if ( (g_PlayData.pBullet+nCnt) -> bUse == true)
		{
			SetHitCheck( (g_PlayData.pBullet+nCnt) -> pos.y + g_PlayData.pBulletHitcheck -> Up,					   //↑の座標
				         (g_PlayData.pBullet+nCnt) -> pos.y + g_PlayData.pBulletHitcheck -> Bottom,			       //↓の座標
						 (g_PlayData.pBullet+nCnt) -> pos.x + g_PlayData.pBulletHitcheck -> Left,					   //←の座標
						 (g_PlayData.pBullet+nCnt) -> pos.x + g_PlayData.pBulletHitcheck -> Right);	               //→の座標
		}
	}

	//EnemyBullet
	for (int nCnt = 0; nCnt < MAX_BULLETENEMY02; nCnt++)
	{
		if ( (g_PlayData.pEnemyBullet+nCnt) -> bUse == true)
		{
			SetHitCheck( (g_PlayData.pEnemyBullet+nCnt) -> pos.y - g_PlayData.pEnemyBullet -> fRadius,				   //↑の座標
				         (g_PlayData.pEnemyBullet+nCnt) -> pos.y + g_PlayData.pEnemyBullet -> fRadius,			       //↓の座標
						 (g_PlayData.pEnemyBullet+nCnt) -> pos.x - g_PlayData.pEnemyBullet -> fRadius - g_PlayData.pPlayer->fPosX_SUM,				   //←の座標
						 (g_PlayData.pEnemyBullet+nCnt) -> pos.x + g_PlayData.pEnemyBullet -> fRadius - g_PlayData.pPlayer->fPosX_SUM);	               //→の座標
		}
	}

	//player
	if(g_PlayData.pPlayer -> bUse == true)
	{
		SetHitCheck( (g_PlayData.pPlayer) -> fPosY + PLAYER_TOP,										       //↑の座標
			         (g_PlayData.pPlayer) -> fPosY + PLAYER_HEIGHT - PLAYER_HEIGHT_CHECK,			           //↓の座標
			         (g_PlayData.pPlayer) -> fPosX + PLAYER_HORRIZON,					                       //←の座標
			         (g_PlayData.pPlayer) -> fPosX + PLAYER_WIDTH - PLAYER_HORRIZON);	                       //→の座標
	}

	//enemyrobot_knife
	for (int nCnt = 0; nCnt < MAX_ENEMYROBOT; nCnt++)
	{
		if( (g_PlayData.pEnemyRobot+nCnt) -> bUse == true)
		{
			if( (g_PlayData.pEnemyRobot+nCnt) -> nType == 2)
			{
				if((g_PlayData.pEnemyRobot+nCnt) -> fDirect == 0.1f)
				{
					SetHitCheck( (g_PlayData.pEnemyRobot+nCnt)->fPosY+KNIFEENEMYROBOT_DISTANCEY_B,										                                   //↑の座標
								 (g_PlayData.pEnemyRobot+nCnt)->fPosY+KNIFEENEMYROBOT_DISTANCEY_T,			                                                               //↓の座標
								 (g_PlayData.pEnemyRobot+nCnt)->fPosX-KNIFEENEMYROBOT_DISTANCEX_R-g_PlayData.pPlayer->fPosX_SUM+ENEMYROBOT_WIDTH,					       //←の座標
								 (g_PlayData.pEnemyRobot+nCnt)->fPosX-KNIFEENEMYROBOT_DISTANCEX_L-g_PlayData.pPlayer->fPosX_SUM+ENEMYROBOT_WIDTH );	                       //→の座標
				}

				else
				{
					SetHitCheck( (g_PlayData.pEnemyRobot+nCnt)->fPosY+KNIFEENEMYROBOT_DISTANCEY_B,										                                   //↑の座標
								 (g_PlayData.pEnemyRobot+nCnt)->fPosY+KNIFEENEMYROBOT_DISTANCEY_T,			                                                               //↓の座標
								 (g_PlayData.pEnemyRobot+nCnt)->fPosX+KNIFEENEMYROBOT_DISTANCEX_R-g_PlayData.pPlayer->fPosX_SUM,					       //←の座標
								 (g_PlayData.pEnemyRobot+nCnt)->fPosX+KNIFEENEMYROBOT_DISTANCEX_L-g_PlayData.pPlayer->fPosX_SUM);	                       //→の座標
				}
			}
		}
	}

	//フェードの計算
	g_pVtxBufferHitcheckPolygon[0]->Lock(0, 0, (void**)&pVtx, 0);       //仮想アドレスを取得しバッファをロックする

	MakeVertexPolygonHitcheck(pD3DDevice, pVtx);

	for(int nCnt = 0; nCnt < NUM_HITCHECKPOLYGON;nCnt++)
	{
		if(g_HitCheck[nCnt].bUse == true)
		{
			pVtx[0+4*nCnt].pos = D3DXVECTOR3(g_HitCheck[nCnt].Left,g_HitCheck[nCnt].Up,0.0f);
			pVtx[1+4*nCnt].pos = D3DXVECTOR3(g_HitCheck[nCnt].Right,g_HitCheck[nCnt].Up,0.0f);
			pVtx[2+4*nCnt].pos = D3DXVECTOR3(g_HitCheck[nCnt].Left,g_HitCheck[nCnt].Bottom,0.0f);
			pVtx[3+4*nCnt].pos = D3DXVECTOR3(g_HitCheck[nCnt].Right,g_HitCheck[nCnt].Bottom,0.0f);

			pVtx[0+4*nCnt].rhw = 1.0f;
			pVtx[1+4*nCnt].rhw = 1.0f;
			pVtx[2+4*nCnt].rhw = 1.0f;
			pVtx[3+4*nCnt].rhw = 1.0f;

			pVtx[0+4*nCnt].color = g_ColorHitcheck;
			pVtx[1+4*nCnt].color = g_ColorHitcheck;
			pVtx[2+4*nCnt].color = g_ColorHitcheck;
			pVtx[3+4*nCnt].color = g_ColorHitcheck;

			pVtx[0+4*nCnt].tex = D3DXVECTOR2(0.0f,0.0f);
			pVtx[1+4*nCnt].tex = D3DXVECTOR2(1.0f,0.0f);
			pVtx[2+4*nCnt].tex = D3DXVECTOR2(0.0f,1.0f);
			pVtx[3+4*nCnt].tex = D3DXVECTOR2(1.0f,1.0f);
		}
	}
	g_pVtxBufferHitcheckPolygon[0]->Unlock();                                     //バッファのアンロックト
}

////////////////////////////////////////////////
//ポリゴンの描画
////////////////////////////////////////////////
void DrawHitcheck(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	//背景：頂点フォーマット設定
	pD3DDevice -> SetStreamSource(0,g_pVtxBufferHitcheckPolygon[0],0,sizeof(VERTEX_2DF));                   //VRAMからGPUにデータをストリーム
	pD3DDevice -> SetFVF(FVF_VERTEX2D);
	pD3DDevice -> SetTexture(0,NULL);												                       //テクスチャの設定

	//アルファブレンディングを行う
	pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	//透過処理行う
	pD3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//半透明処理行う
	pD3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	//描画
	for(int nCnt = 0; nCnt < NUM_HITCHECKPOLYGON;nCnt++)
	{
		if(g_HitCheck[nCnt].bUse == true)
		{
			pD3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,4*nCnt,NUM_POLYGON);                                      //テクスチャ描画
			g_HitCheck[nCnt].bUse = false;
		}
	}

}

void UninitHitcheck(void)
{	
	//頂点バッファインターフェイスの片付け
	for (int nCnt = 0; nCnt < NUM_HITCHECKPOLYGON; nCnt++)
	{
		SAFE_RELEASE(g_pVtxBufferHitcheckPolygon[nCnt]);
	}
}

//背景ポリゴンの表示位置X,ポリゴンの表示位置Y,ポリゴンの幅,ポリゴンの高さ
void MakeVertexPolygonHitcheck(LPDIRECT3DDEVICE9 pDevice,VERTEX_2DF *pVtx)
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

//チェックボックスの設置
void SetHitCheck(float Up,float Bottom,float Left,float Right)
{
	for(int nCnt = 0; nCnt < NUM_HITCHECKPOLYGON;nCnt++)
	{
		if(g_HitCheck[nCnt].bUse == false)
		{
			g_HitCheck[nCnt].Up = Up;
			g_HitCheck[nCnt].Bottom = Bottom;
			g_HitCheck[nCnt].Left = Left;
			g_HitCheck[nCnt].Right = Right;
			g_HitCheck[nCnt].bUse = true;
			break;
		}
	}
}

//データをセット
void DataSet(void)
{
	g_PlayData.pPlayer = GetPlayer();                         //プレーヤーデータゲット
	g_PlayData.pEnemy01 = GetEnemy01();                       //Enemy01のデータゲット
	g_PlayData.pEnemy02 = GetEnemy02();                       //Enemy02のデータゲット
	g_PlayData.pBullet = GetBullet();                         //弾のデータをゲット
	g_PlayData.pBulletHitcheck = GetBulletHitcheck();         //弾の当り判定範囲
	g_PlayData.pEnemyBullet = GetBulletEnemy02();             //敵の弾のデータをゲット
	g_PlayData.pEnemyRobot = GetEnemyRobot();
}

#endif
