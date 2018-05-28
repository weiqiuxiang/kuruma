//-----------------------------------------------------------------------------------------
//プレーヤー,道具などの処理[player.cpp]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------------------
#include "main.h"
#include "player.h"
#include "input.h"
#include "enemyrobot.h"
#include "bullet.h"
#include "bulletEnemy02.h"
#include "fade.h"
#include "sound.h"
#include "stage01.h"
#include "partical.h"
#include "particalEffect.h"
#include "particalCenter.h"
#include <stdio.h>

//-----------------------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------------------
#define BARREL_ROTATE_SPEED (0.02*PI)  //砲身の回転スピード

//HPの描画ポジション
#define HP_POSX (30)
#define HP_POSY (50)
#define HP_INTER (40)
#define HP_WIDTH (32)
#define HP_HEIGHT (32)

//カメラパラメータ
#define LEFT_END (0)                       //左端
#define RIGHT_END (2520)                   //右端
#define LEFT_MOVE_LINE (350)               //左移動境界線
#define RIGHT_MOVE_LINE (450)              //左移動境界線

#define PLAYER_INVINCIBLE (120)            //プレーヤーダメージ受けた後の無敵時間

#define PLAYER_PARTICAL_POSX (20.0f)
#define PLAYER_PARTICAL_POSY_D (20.0f)

//-----------------------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------------------
void HitCheckForEnemy(void);                                 //敵がプレイヤーに対する当り判定関数

//-----------------------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTexturePlayerPolygon[NUM_PLAYERTEXTURE] = {};           //テクスチャを操作するためのポインタ(テクスチャバッファインターフェイスにアクセスためのポインタ)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferPlayerPolygon = NULL;    //頂点バッファを操作するためのポインタ(頂点バッファインターフェイスにアクセスためのポインタ)

PLAYER g_Player;

//-----------------------------------------------------------------------------------------
//重要定番関数達
//-----------------------------------------------------------------------------------------

//各変数の初期化
HRESULT InitPlayer(void)
{
	//プレーヤーの諸データの初期値
	g_Player.fPosX = 100.0f;
	g_Player.fPosY = 300.0f;
	g_Player.fPosXOld = g_Player.fPosX;
	g_Player.fPosYOld = g_Player.fPosY;
	g_Player.fMoveX = 0.1*30;
	g_Player.fDirect = 0.1;
	g_Player.bJump = true;
	g_Player.bJump_Once = false;
	g_Player.bRun = false;
	g_Player.bUse = true;
	g_Player.bHit = false;
	g_Player.nInvinFrame = 0;
	g_Player.nLife = 6;
	g_Player.fScore = 0;
	sprintf(&g_Player.aScore[0],"%f",g_Player.fScore);
	g_Player.aScore[9] = '\0';
	g_Player.fPosY_Move = 0;
	g_Player.fGunAngel = 2*PI;

	//カメラパラメータ初期化
	g_Player.fPosX_SUM = 0;
	g_Player.bCameraRecovery = false;
	g_Player.fRecoveryDistance = 0;

	//プレーヤーダメージ
	g_Player.bDamage = false;
	g_Player.nCntInvincible = 0;

	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_2D *pVtx = NULL;                           //頂点構造体のポインタ
	pD3DDevice = Get_Device();

	//頂点バッファの生成
	if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_PLAYERVERTEX*NUM_PLAYERPOLYGON, D3DUSAGE_WRITEONLY, FVF_VERTEX2D, D3DPOOL_MANAGED, &g_pVtxBufferPlayerPolygon, NULL)))
	{
		return E_FAIL;
	}

	//プレーヤーテクスチャの取得
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, PLAYERPOLYGON01_TEXTURENAME, &g_pTexturePlayerPolygon[0])))
	{
		MessageBox(NULL, "テクスチャの読み込みが失敗しました", "エラー", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	//プレーヤーのマシンガンテクスチャの取得
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, PLAYERPOLYGON02_TEXTURENAME, &g_pTexturePlayerPolygon[1])))
	{
		MessageBox(NULL, "テクスチャの読み込みが失敗しました", "エラー", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	//プレーヤーのHPテクスチャの取得
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, PLAYERPOLYGON03_TEXTURENAME, &g_pTexturePlayerPolygon[2])))
	{
		MessageBox(NULL, "テクスチャの読み込みが失敗しました", "エラー", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	//プレーヤーのポリゴン
	g_pVtxBufferPlayerPolygon->Lock(0, 0, (void**)&pVtx, 0);                    //仮想アドレスを取得しバッファをロックする

	MakeVertexPolygonPlayer(pD3DDevice, pVtx);
	pVtx[0].pos = D3DXVECTOR3(g_Player.fPosX, g_Player.fPosY, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Player.fPosX+PLAYER_WIDTH, g_Player.fPosY, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Player.fPosX, g_Player.fPosY+PLAYER_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Player.fPosX+PLAYER_WIDTH, g_Player.fPosY+PLAYER_HEIGHT, 0.0f);

	pVtx += 4;

	pVtx[0].pos = D3DXVECTOR3(g_Player.fPosX+PLAYER_WIDTH+GUN_DISTANCEX, g_Player.fPosY+GUN_DISTANCEY, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Player.fPosX+GUN_WIDTH+PLAYER_WIDTH+GUN_DISTANCEX, g_Player.fPosY+GUN_DISTANCEY, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Player.fPosX+PLAYER_WIDTH+GUN_DISTANCEX, g_Player.fPosY+GUN_HEIGHT+GUN_DISTANCEY, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Player.fPosX+GUN_WIDTH+PLAYER_WIDTH+GUN_DISTANCEX, g_Player.fPosY+GUN_HEIGHT+GUN_DISTANCEY, 0.0f);

	g_pVtxBufferPlayerPolygon->Unlock();                                     //バッファのアンロック

	return S_OK;
}
//終了処理
void UninitPlayer(void)
{
	// テクスチャインターフェイスの片付け
	for (int nCnt = 0; nCnt < NUM_PLAYERTEXTURE; nCnt++)
	{
		if(g_pTexturePlayerPolygon[nCnt] != NULL)
		{
			g_pTexturePlayerPolygon[nCnt] -> Release();
			g_pTexturePlayerPolygon[nCnt] = NULL;
		}
	}

	//頂点バッファインターフェイスの片付け
	if(g_pVtxBufferPlayerPolygon != NULL)
	{
		g_pVtxBufferPlayerPolygon -> Release();
		g_pVtxBufferPlayerPolygon = NULL;
	}
}

//プレーヤーデータの更新処理
void UpdatePlayer(void)
{
	DIJOYSTATE *pjs = GetJS(); //test

	//もしプレーヤーは使用可の状態なら
	if (g_Player.bUse == true)
	{
		//プレーヤーの中心座標
		float fPosXCenter = 0;
		float fPosXOldCenter = 0;

		//プレーヤーの前の座標を記録
		g_Player.fPosXOld = g_Player.fPosX;
		g_Player.fPosYOld = g_Player.fPosY;

		//-----------------------------------------------------------------------------------------
		//プレーヤーの移動処理
		//-----------------------------------------------------------------------------------------
		float fHantenAngle = 2 * PI * ( (g_Player.fGunAngel / (2*PI) ) - (int)(g_Player.fGunAngel / (2*PI) ) );

		//助走なし,方向：←
		if (pjs->lX < -500 || GetKeyboardPress(DIK_A))
		{
			//座標累加&&前進方向保存
			g_Player.fPosX -= g_Player.fMoveX;
			g_Player.fDirect = -0.1f;
			g_Player.bRun = true;                                //プレーヤーが動いている

			float speedX = rand()%2+1;
			float speedY = -(rand()%200-100)*0.01;

			if(g_Player.bJump == false)
			{
				//パーティクルを発射
				SetPartical(g_Player.fPosX + PLAYER_PARTICAL_POSX,g_Player.fPosY + PLAYER_HEIGHT - PLAYER_PARTICAL_POSY_D,speedX,speedY,3,20,false);
				SetParticalEffect(g_Player.fPosX + PLAYER_PARTICAL_POSX,g_Player.fPosY + PLAYER_HEIGHT - PLAYER_PARTICAL_POSY_D,speedX,speedY,3*2,20,false);
			}

		}

		//助走なし,方向：→
		if (pjs->lX > 500 || GetKeyboardPress(DIK_D))
		{
			//座標累加&&前進方向保存
			g_Player.fPosX += g_Player.fMoveX;
			g_Player.fDirect = 0.1f;
			g_Player.bRun = true;                                //プレーヤーが動いている

			float speedX = -(rand()%2+1);
			float speedY = -(rand()%200-100)*0.01;

			if(g_Player.bJump == false)
			{
				//パーティクルを発射
				SetPartical(g_Player.fPosX + PLAYER_PARTICAL_POSX,g_Player.fPosY + PLAYER_HEIGHT - PLAYER_PARTICAL_POSY_D,speedX,speedY,3,20,false);
				SetParticalEffect(g_Player.fPosX + PLAYER_PARTICAL_POSX,g_Player.fPosY + PLAYER_HEIGHT - PLAYER_PARTICAL_POSY_D,speedX,speedY,3*2,20,false);
			}
		}

		//砲身の角度計算
		if(GetControllerPress(4) || GetKeyboardPress(DIK_U))
		{
			g_Player.fGunAngel -= BARREL_ROTATE_SPEED;
		}

		if(GetControllerPress(5) || GetKeyboardPress(DIK_I))
		{
			g_Player.fGunAngel += BARREL_ROTATE_SPEED;
		}

		////方向：↑
		//if (GetKeyboardPress(DIK_W) )
		//{
		//	if( (fHantenAngle > PI*1.5f) || (fHantenAngle < PI*0.5f) )
		//	{
		//		g_Player.fGunAngel -= BARREL_ROTATE_SPEED;
		//	}

		//	else
		//	{
		//		g_Player.fGunAngel += BARREL_ROTATE_SPEED;
		//	}
		//}

		////方向：↓
		//if (GetKeyboardPress(DIK_S))
		//{
		//	if( (fHantenAngle > PI*1.5f) || (fHantenAngle < PI*0.5f) )
		//	{
		//		g_Player.fGunAngel += BARREL_ROTATE_SPEED;
		//	}

		//	else
		//	{
		//		g_Player.fGunAngel -= BARREL_ROTATE_SPEED;
		//	}
		//}

		//角度が0以上確保
		if(g_Player.fGunAngel <= 0)
		{
			g_Player.fGunAngel = 2*PI + g_Player.fGunAngel;
		}
		
		/*ジャンプさせたい高さと到達時間の計算:最大高さ:120.0f 到達時間:40フレーム
		最大高さ: 120 = v*40 - g/2*40^2 ①
		最大高さまでの時間(スピードがゼロになる):0 = v - g*40 ②
		式①と②を連動して,ジャンプ力 v = 6.0f 重力 g = 0.15f */

		//プレーヤーのジャンプ力
		if(GetControllerTrigger(0) || GetKeyboardTrigger(DIK_K))
		{
			if(g_Player.bJump == false)
			{
				g_Player.fPosY_Move -= 6.0f;
				g_Player.bJump = true;
			}
		}

		//Y軸の方が重力影響あり
		g_Player.fPosY_Move += GRAVITY;

		//Y軸の移動計算
		g_Player.fPosY += g_Player.fPosY_Move;

		//Y軸が地面に嵌め込まないように
		if(g_Player.fPosY > 447.0f - PLAYER_HEIGHT + PLAYER_HEIGHT_CHECK)
		{
			g_Player.fPosY = g_Player.fPosYOld;
			g_Player.fPosY_Move = 0;
			g_Player.bJump = false;
		}

		//-----------------------------------------------------------------------------------------
		//マップX軸スクロール処理
		//-----------------------------------------------------------------------------------------
		//(X軸)プレーヤーの座標移動
		//中心座標を求め

		//ステージカメラワークのデータを取得
		STAGE01 *pStage01;
		pStage01 = GetStage01();

		//プレーヤーの中心座標と中心古い座標
		fPosXCenter = g_Player.fPosX + PLAYER_WIDTH/2;
		fPosXOldCenter = g_Player.fPosXOld + PLAYER_WIDTH / 2;

		//カメラワークが停止してない
		if(pStage01 -> bStopCameraMove == false)
		{
			//プレーヤーがマップの左端から出発
			if( 
				(fPosXCenter >= RIGHT_MOVE_LINE && fPosXOldCenter < RIGHT_MOVE_LINE) &&
				(g_Player.fPosX_SUM == 0)
				)
			{
				g_Player.fPosX_SUM += g_Player.fMoveX;
			}

			//プレーヤーがマップの右端から出発
			if (
				(fPosXCenter <= LEFT_MOVE_LINE && fPosXOldCenter > LEFT_MOVE_LINE) &&
				(g_Player.fPosX_SUM == RIGHT_END)
				)
			{
				g_Player.fPosX_SUM -= g_Player.fMoveX;
			}

			//カメラワーク移動区間
			if ((fPosXCenter >= RIGHT_MOVE_LINE) || (fPosXCenter <= LEFT_MOVE_LINE))
			{
				//カメラ座標が修復してない
				if(g_Player.bCameraRecovery == false)
				{
					//カメラワーク移動区間
					if( (g_Player.fPosX_SUM > 0) && (g_Player.fPosX_SUM < RIGHT_END) )
					{
						g_Player.fPosX_SUM += g_Player.fPosX - g_Player.fPosXOld;
						g_Player.fPosX = g_Player.fPosXOld;
					}

					else
					{
						if (g_Player.fPosX_SUM >= RIGHT_END)
						{
							g_Player.fPosX_SUM = RIGHT_END;
						}

						else
						{
							g_Player.fPosX_SUM = 0;
						}
					}
				}

				//カメラ座標が修復している
				else
				{
					//カメラが移動中
					if( (g_Player.fPosX_SUM > 0) && (g_Player.fPosX_SUM < RIGHT_END) )
					{
						//----------------
						//車移動した量累加
						//----------------
						if(g_Player.bRun == true) 
						{
							if(g_Player.fDirect == -0.1f)
							{
								g_Player.fRecoveryDistance += g_Player.fMoveX;
							}

							else
							{
								g_Player.fRecoveryDistance -= g_Player.fMoveX;
							}
						}

						//------------------
						//カメラ座標修復移動
						//------------------
						if(g_Player.fRecoveryDistance < 0)
						{
							float fAdjust = 0;
							g_Player.fRecoveryDistance += g_Player.fMoveX*2;

							//誤差補正部分
							if(g_Player.fRecoveryDistance >= 0)
							{
								fAdjust = g_Player.fRecoveryDistance;          //誤差値代入
								g_Player.fRecoveryDistance = 0;                //修復距離を0に
								g_Player.bCameraRecovery = false;              //修復完了フラグを戻す
								g_Player.fPosX -= g_Player.fMoveX*2;         //保険を掛ける

							}
							g_Player.fPosX_SUM += (g_Player.fMoveX*2 - fAdjust);
							g_Player.fPosX -= (g_Player.fMoveX*2 - fAdjust);
						}

						else
						{
							float fAdjust = 0;
							g_Player.fRecoveryDistance -= g_Player.fMoveX*2;

							//誤差補正部分
							if(g_Player.fRecoveryDistance <= 0)
							{
								fAdjust = g_Player.fRecoveryDistance;          //誤差値代入
								g_Player.fRecoveryDistance = 0;                //修復距離を0に
								g_Player.bCameraRecovery = false;              //修復完了フラグを戻す
								g_Player.fPosX += g_Player.fMoveX*2;         //保険を掛ける
							}
							g_Player.fPosX_SUM -= (g_Player.fMoveX*2 + fAdjust);
							g_Player.fPosX += (g_Player.fMoveX*2 + fAdjust);
						}
					}
				}
			}

			else
			{
				//カメラワーク移動区間
				if( (g_Player.fPosX_SUM > 0) && (g_Player.fPosX_SUM < RIGHT_END) )
				{
					//カメラ座標回復してない
					if(g_Player.bCameraRecovery == false)
					{
						//プレーヤーはカメラの右側移動境界線より右にいる
						if(fPosXCenter > RIGHT_MOVE_LINE)
						{
							g_Player.fRecoveryDistance = RIGHT_MOVE_LINE - fPosXCenter;
						}
						//プレーヤーはカメラの左側移動境界線より左にいる
						if(fPosXCenter < LEFT_MOVE_LINE)
						{
							g_Player.fRecoveryDistance = LEFT_MOVE_LINE - fPosXCenter;
						}

					}

					//カメラワーク修復中
					else
					{
						//----------------
						//車移動した量累加
						//----------------
						if(g_Player.bRun == true) 
						{
							if(g_Player.fDirect == -0.1f)
							{
								g_Player.fRecoveryDistance += g_Player.fMoveX;
							}

							else
							{
								g_Player.fRecoveryDistance -= g_Player.fMoveX;
							}
						}

						//------------------
						//カメラ座標修復移動
						//------------------
						if(g_Player.fRecoveryDistance < 0)
						{
							float fAdjust = 0;
							g_Player.fRecoveryDistance += g_Player.fMoveX*2;

							//誤差補正部分
							if(g_Player.fRecoveryDistance >= 0)
							{
								fAdjust = g_Player.fRecoveryDistance;      //誤差値代入
								g_Player.fRecoveryDistance = 0;            //修復距離を0に
								g_Player.bCameraRecovery = false;          //修復完了フラグを戻す
								g_Player.fPosX -= g_Player.fMoveX*2;         //保険を掛ける
							}
							g_Player.fPosX_SUM += (g_Player.fMoveX*2 - fAdjust);
							g_Player.fPosX -= (g_Player.fMoveX*2 - fAdjust);
						}

						else
						{
							float fAdjust = 0;
							g_Player.fRecoveryDistance -= g_Player.fMoveX*2;

							//誤差補正部分
							if(g_Player.fRecoveryDistance <= 0)
							{
								fAdjust = g_Player.fRecoveryDistance;      //誤差値代入
								g_Player.fRecoveryDistance = 0;            //修復距離を0に
								g_Player.bCameraRecovery = false;          //修復完了フラグを戻す
								g_Player.fPosX += g_Player.fMoveX*2;         //保険を掛ける
							}
							g_Player.fPosX_SUM -= (g_Player.fMoveX*2 + fAdjust);
							g_Player.fPosX += (g_Player.fMoveX*2 + fAdjust);
						}
					}
				}
			}
		}

		//カメラワーク移動停止
		else
		{
			g_Player.bCameraRecovery = true;               //カメラ修復予約

			//------------------------------
			//修復距離を測る
			//------------------------------
			//プレーヤーはカメラの右側移動境界線より右にいる
			if(fPosXCenter > RIGHT_MOVE_LINE)
			{
				g_Player.fRecoveryDistance = RIGHT_MOVE_LINE - fPosXCenter;
			}
			//プレーヤーはカメラの左側移動境界線より左にいる
			if(fPosXCenter < LEFT_MOVE_LINE)
			{
				g_Player.fRecoveryDistance = LEFT_MOVE_LINE - fPosXCenter;
			}
		}

		//-----------------------------------------------------------------------------------------
		//プレーヤー
		//-----------------------------------------------------------------------------------------

		//移動範囲制限
		if ((g_Player.fPosX < 0) || ( (g_Player.fPosX + PLAYER_WIDTH) > SCREEN_WIDTH))
		{
			g_Player.fPosX = g_Player.fPosXOld;
		}

		if ((g_Player.fPosY < 0) || ((g_Player.fPosY + PLAYER_HEIGHT) > SCREEN_HEIGHT))
		{
			g_Player.fPosY = g_Player.fPosYOld;
		}
		
		//test
		if(GetControllerRepeat(2) || GetKeyboardRepeat(DIK_J))
		{
			SetBullet(g_Player.fPosX, g_Player.fPosY, 10 * cos(g_Player.fGunAngel), 10 * sin(g_Player.fGunAngel), 70,g_Player.fGunAngel);
			PlaySound(SOUND_LABEL_BGM000);
		}

		//当り判定->無敵状態処理
		if(g_Player.bDamage == false) HitCheckForEnemy();                             //当り判定
		else
		{
			g_Player.nCntInvincible--;

			if(g_Player.nCntInvincible <= 0)
			{
				g_Player.bDamage = false;
			}
		}

		UpdatePolygonPlayer();

		//フレームごとにfalseに戻るフラグ
		g_Player.bRun = false;

	}
}

//プレーヤーポリゴンのデータ更新
void UpdatePolygonPlayer(void) 
{
	static int nRunCnt = 0;                                       //ランニングマンテクスチャの切り替えカウンター
	static int nActionFlame = 1;                                  //ランニングマンの動きを遅くするためのカウンター
	VERTEX_2D *pVtx = NULL;
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();
																  
	g_pVtxBufferPlayerPolygon->Lock(0, 0, (void**)&pVtx, 0);          //仮想アドレスを取得しバッファをロックする

	MakeVertexPolygonPlayer(pD3DDevice, pVtx);

	//プレーヤー本体のパラメータ
	pVtx[0].pos = D3DXVECTOR3(g_Player.fPosX, g_Player.fPosY, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Player.fPosX + PLAYER_WIDTH, g_Player.fPosY, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Player.fPosX, g_Player.fPosY + PLAYER_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Player.fPosX + PLAYER_WIDTH, g_Player.fPosY + PLAYER_HEIGHT, 0.0f);

	//プレーヤーが移動しているがとうが
	if(g_Player.bRun == false)
	{
		pVtx[0].tex = D3DXVECTOR2(1.0f*1/3, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f*2/3, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f*1/3, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f*2/3, 1.0f);
	}

	else
	{
		pVtx[0].tex = D3DXVECTOR2(1.0f*1/3+nRunCnt%3*1.0f*1/3, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f*2/3+nRunCnt%3*1.0f*1/3, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f*1/3+nRunCnt%3*1.0f*1/3, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f*2/3+nRunCnt%3*1.0f*1/3, 1.0f);

		nActionFlame++;
		if(nActionFlame % 4 == 0) nRunCnt++;                         //4フレームごとにモーションを切り替え
	}

	//変数nRunCntのオーバーフロー防止
	if (nRunCnt == 10000)
	{
		nRunCnt = 0;
	}

	//変数nActionFlameのオーバーフロー防止
	if (nActionFlame >= 10000)
	{
		nActionFlame = 0;
	}

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].color = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx += 4;

	//プレーヤーのマシンガンのパラメータ

	SpriteRorate(pVtx);

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

	pVtx += 4;

	//プレーヤーの残りライフを描画
	for(int nCnt = 0; nCnt < g_Player.nLife ;nCnt++)
	{
		pVtx[0].pos = D3DXVECTOR3(HP_POSX + nCnt*HP_INTER, HP_POSY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(HP_POSX + HP_WIDTH+ nCnt*HP_INTER, HP_POSY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(HP_POSX+ nCnt*HP_INTER, HP_POSY + HP_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(HP_POSX + HP_WIDTH+ nCnt*HP_INTER, HP_POSY + HP_HEIGHT, 0.0f);

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

		pVtx += 4;
	}

	g_pVtxBufferPlayerPolygon->Unlock();                                     //バッファのアンロック
}

//プレーヤー描画
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	//頂点フォーマット設定
	pD3DDevice->SetStreamSource(0, g_pVtxBufferPlayerPolygon, 0, sizeof(VERTEX_2D));              //VRAMからGPUにデータをストリーム
	pD3DDevice->SetFVF(FVF_VERTEX2D);

	if(g_Player.bUse == true)
	{
		//普通状態描画
		if(g_Player.bDamage == false)
		{
			//プレーヤー描画
			pD3DDevice->SetTexture(0, g_pTexturePlayerPolygon[0]);				                             //テクスチャの設定
			pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);                                  //テクスチャメモリの確保

			//砲身描画
			pD3DDevice->SetTexture(0, g_pTexturePlayerPolygon[1]);				                             //テクスチャの設定
			pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, NUM_POLYGON);                                  //テクスチャメモリの確保
		}

		//無敵状態描画
		else
		{
			static int nChange;
			if(nChange % 2 == 0)
			{
				//プレーヤー描画
				pD3DDevice->SetTexture(0, g_pTexturePlayerPolygon[0]);				                             //テクスチャの設定
				pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);                                  //テクスチャメモリの確保

				//砲身描画
				pD3DDevice->SetTexture(0, g_pTexturePlayerPolygon[1]);				                             //テクスチャの設定
				pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, NUM_POLYGON);                                  //テクスチャメモリの確保
			}

			if(g_Player.nCntInvincible % 4 == 0) nChange++;
			if(nChange >= 10000) nChange = 0;
		}

		//プレーヤーHP描画
		for(int nCnt = 0; nCnt < g_Player.nLife ;nCnt++)
		{
			pD3DDevice->SetTexture(0, g_pTexturePlayerPolygon[2]);
			pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 8+nCnt*4, NUM_POLYGON); 
		}
	}
}

//プレーヤーポリゴンの表示位置X,ポリゴンの表示位置Y,ポリゴンの幅,ポリゴンの高さ
void MakeVertexPolygonPlayer(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx)
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

PLAYER *GetPlayer(void)
{
	return &g_Player;
}

void SpriteRorate (VERTEX_2D *pVtx)
{
	double X1, Y1, X2, Y2, X3, Y3, X4, Y4;             //左上,右下座標
	double Xc1, Yc1, Xc2, Yc2, Xc3, Yc3, Xc4, Yc4;     //回転した後の座標
	double XC, YC;                                     //中心座標

	X1 = g_Player.fPosX+PLAYER_WIDTH/2-GUN_DISTANCEX;
	Y1 = g_Player.fPosY+GUN_DISTANCEY;
	X2 = g_Player.fPosX+GUN_WIDTH+PLAYER_WIDTH/2-GUN_DISTANCEX;
	Y2 = g_Player.fPosY+GUN_DISTANCEY;
	X3 = g_Player.fPosX+PLAYER_WIDTH/2-GUN_DISTANCEX;
	Y3 = g_Player.fPosY+GUN_HEIGHT+GUN_DISTANCEY;
	X4 = g_Player.fPosX+GUN_WIDTH+PLAYER_WIDTH/2-GUN_DISTANCEX;
	Y4 = g_Player.fPosY+GUN_HEIGHT+GUN_DISTANCEY;

	XC = g_Player.fPosX+GUN_WIDTH/2+PLAYER_WIDTH/2-GUN_DISTANCEX;
	YC = g_Player.fPosY+GUN_HEIGHT/2+GUN_DISTANCEY;

	Xc1 = (X1 - XC)*cos(g_Player.fGunAngel) - (Y1 - YC)*sin(g_Player.fGunAngel) + XC;
	Yc1 = (X1 - XC)*sin(g_Player.fGunAngel) + (Y1 - YC)*cos(g_Player.fGunAngel) + YC;

	Xc2 = (X2 - XC)*cos(g_Player.fGunAngel) - (Y2 - YC)*sin(g_Player.fGunAngel) + XC;
	Yc2 = (X2 - XC)*sin(g_Player.fGunAngel) + (Y2 - YC)*cos(g_Player.fGunAngel) + YC;

	Xc3 = (X3 - XC)*cos(g_Player.fGunAngel) - (Y3 - YC)*sin(g_Player.fGunAngel) + XC;
	Yc3 = (X3 - XC)*sin(g_Player.fGunAngel) + (Y3 - YC)*cos(g_Player.fGunAngel) + YC;

	Xc4 = (X4 - XC)*cos(g_Player.fGunAngel) - (Y4 - YC)*sin(g_Player.fGunAngel) + XC;
	Yc4 = (X4 - XC)*sin(g_Player.fGunAngel) + (Y4 - YC)*cos(g_Player.fGunAngel) + YC;

	pVtx[0].pos = D3DXVECTOR3(Xc1,Yc1,0.0f);
	pVtx[1].pos = D3DXVECTOR3(Xc2,Yc2,0.0f);
	pVtx[2].pos = D3DXVECTOR3(Xc3,Yc3,0.0f);
	pVtx[3].pos = D3DXVECTOR3(Xc4,Yc4,0.0f);
}

//プレーヤーに対する当り判定
void HitCheckForEnemy(void)
{
	//敵の弾の情報ゲット
	BULLETENEMY02  *pEnemyBullet = GetBulletEnemy02();
	GAMEMANAGER *pGameManager = GetManager();
	ENEMYROBOT *pEnemyRobot = GetEnemyRobot();               //敵に対する当り判定

	float top = g_Player.fPosY + PLAYER_TOP;
	float bottom = g_Player.fPosY + PLAYER_HEIGHT - PLAYER_HEIGHT_CHECK;
	float left = g_Player.fPosX + PLAYER_HORRIZON;
	float right = g_Player.fPosX + PLAYER_WIDTH - PLAYER_HORRIZON;

	//敵の弾に対する当り判定
	for (int nCnt = 0; nCnt < MAX_BULLETENEMY02; nCnt++)
	{
		//弾が使用中
		if( (pEnemyBullet+nCnt) -> bUse == true)
		{
			if(
				( top <= ((pEnemyBullet+nCnt)->pos.y+(pEnemyBullet+nCnt)->fRadius) ) &&
				( bottom >= ((pEnemyBullet+nCnt)->pos.y-(pEnemyBullet+nCnt)->fRadius) ) &&
				( left <= ((pEnemyBullet+nCnt)->pos.x+(pEnemyBullet+nCnt)->fRadius-g_Player.fPosX_SUM) ) &&
				( right >= ((pEnemyBullet+nCnt)->pos.x-(pEnemyBullet+nCnt)->fRadius-g_Player.fPosX_SUM) )
				)
			{
				g_Player.nLife--;
				(pEnemyBullet+nCnt) -> bUse = false;
				g_Player.bDamage = true;
				g_Player.nCntInvincible = PLAYER_INVINCIBLE;
			}
		}
	}
	//敵ロボット刀タイプに対する当り判定
	for (int nCnt = 0; nCnt < NUM_ENEMYROBOTPOLYGON; nCnt++)
	{
		//敵が使用中
		if( (pEnemyRobot+nCnt) -> bUse == true)
		{
			//敵の向き方向は←
			if( (pEnemyRobot+nCnt) -> fDirect == -0.1f)
			{
				//敵は落下攻撃の時
				if( (pEnemyRobot+nCnt) -> bAttackDropFlag == true)
				{
					if(
						( top <= ((pEnemyRobot+nCnt)->fPosY+KNIFEENEMYROBOT_DISTANCEY_B ) ) &&
						( bottom >= ((pEnemyRobot+nCnt)->fPosY+KNIFEENEMYROBOT_DISTANCEY_T) )&&
						( left <= ((pEnemyRobot+nCnt)->fPosX+KNIFEENEMYROBOT_DISTANCEX_L-g_Player.fPosX_SUM) ) &&
						( right >= ((pEnemyRobot+nCnt)->fPosX+KNIFEENEMYROBOT_DISTANCEX_R-g_Player.fPosX_SUM) )
						)

					{
						g_Player.nLife--;
						(pEnemyBullet+nCnt) -> bUse = false;
						g_Player.bDamage = true;
						g_Player.nCntInvincible = PLAYER_INVINCIBLE;
					}
				}
			}

			//敵の向き方向は→
			else
			{
				//敵は落下攻撃の時
				if( (pEnemyRobot+nCnt) -> bAttackDropFlag == true)
				{
					if(
						( top <= ((pEnemyRobot+nCnt)->fPosY+KNIFEENEMYROBOT_DISTANCEY_B ) ) &&
						( bottom >= ((pEnemyRobot+nCnt)->fPosY+KNIFEENEMYROBOT_DISTANCEY_T) )&&
						( left <= ((pEnemyRobot+nCnt)->fPosX-KNIFEENEMYROBOT_DISTANCEX_R-g_Player.fPosX_SUM+ENEMYROBOT_WIDTH) ) &&
						( right >= ((pEnemyRobot+nCnt)->fPosX-KNIFEENEMYROBOT_DISTANCEX_L-g_Player.fPosX_SUM+ENEMYROBOT_WIDTH) )
						)
					{
						g_Player.nLife--;
						(pEnemyBullet+nCnt) -> bUse = false;
						g_Player.bDamage = true;
						g_Player.nCntInvincible = PLAYER_INVINCIBLE;
					}
				}
			}
		}
	}

	//プレーヤーが死んだかどうか
	if(g_Player.nLife <= 0)
	{
		g_Player.bUse = false;
		PlaySound(SOUND_LABEL_SE_EXPLOSION);
		pGameManager -> bGameOver = true;
		pGameManager -> nGameResult = 1;
		StopSound(SOUND_LABEL_BGM_STAGE);
		SetParticalCenter(2,g_Player.fPosX+PLAYER_WIDTH*0.5,g_Player.fPosY+PLAYER_HEIGHT*0.5,10,15,false);
	}
}