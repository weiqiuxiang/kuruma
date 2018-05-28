//-----------------------------------------------------------------------------------------
//プレーヤー,道具などの処理[running02.cpp]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------------------
#include <time.h>
#include "main.h"
#include "enemyrobot.h"
#include "player.h"
#include "bullet.h"
#include "fade.h"
#include "bulletEnemy02.h"
#include "stage01.h"
#include "sound.h"
#include "particalCenter.h"

//-----------------------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------------------
#define ENEMYROBOT_RECOVERYATTACK_TIME (120)              //敵の弾の装填時間

//銃タイプ敵
#define EGUN_MOVEMODE_XSPEED (2.0f)                       //普通移動の時の速度
#define EGUN_BULLET_SPEED (2.5f);                         //弾スピード
#define ENEMYROBOT_PREATTACK_TIME (120)                   //敵の攻撃準備時間

//刀タイプ敵
#define EKNIFE_MOVEMODE_XSPEED (2.0f)                     //普通移動をする時の移動スピード
#define MOVE_ACTION_DISTANCE (250)                        //移動状態判定距離
#define RECOVERY_TIME (60)                                //硬直時間
#define E_JUMP_POWER (6.0f)

//-----------------------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------------------
void EnemyRobotMove01(int nCnt);                               //EnemyRobot銃タイプの移動関数
void EnemyRobotMove02(int nCnt);                               //EnemyRobot刀タイプの移動関数
void ATTACK_ACTION(ENEMYROBOT *enemy);                          //EnemyRobot刀タイプの攻撃計算関数

//EnemyRobotのタイプ別ポリゴン更新関数
void UpdatePolygonEnemyRobotTex01(ENEMYROBOT enemy,VERTEX_2D *pVtx);
void UpdatePolygonEnemyRobotTex02(ENEMYROBOT enemy,VERTEX_2D *pVtx);

//-----------------------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureEnemyRobotPolygon[NUM_ENEMYROBOTTEXTURE] = {};           //テクスチャを操作するためのポインタ(テクスチャバッファインターフェイスにアクセスためのポインタ)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferEnemyRobotPolygon = NULL;                      //頂点バッファを操作するためのポインタ(頂点バッファインターフェイスにアクセスためのポインタ)

ENEMYROBOT g_EnemyRobot[NUM_ENEMYROBOTPOLYGON];

//-----------------------------------------------------------------------------------------
//各変数の初期化
//-----------------------------------------------------------------------------------------
HRESULT InitEnemyRobot(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_2D *pVtx = NULL;                           //頂点構造体のポインタ
	pD3DDevice = Get_Device();

	//頂点バッファの生成
	if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_ENEMYROBOTVERTEX*NUM_ENEMYROBOTPOLYGON, D3DUSAGE_WRITEONLY, FVF_VERTEX2D, D3DPOOL_MANAGED, &g_pVtxBufferEnemyRobotPolygon, NULL)))
	{
		return E_FAIL;
	}

	//EnemyRobotテクスチャの取得
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, ENEMYROBOTPOLYGON01_TEXTURENAME, &g_pTextureEnemyRobotPolygon[0])))
	{
		MessageBox(NULL, "テクスチャの読み込みが失敗しました", "エラー", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	//プレーヤーのポリゴン
	g_pVtxBufferEnemyRobotPolygon -> Lock(0, 0, (void**)&pVtx, 0);                    //仮想アドレスを取得しバッファをロックする
	MakeVertexPolygonPlayer(pD3DDevice, pVtx);

	for (int nCnt = 0; nCnt < MAX_ENEMYROBOT; nCnt++)
	{
		//プレーヤーの諸データの初期値
		g_EnemyRobot[nCnt].nNumEnemy = 0;                                        //敵の番号
		g_EnemyRobot[nCnt].fPosX = 0.0f;
		g_EnemyRobot[nCnt].fPosY = 0.0f;
		g_EnemyRobot[nCnt].fDirect = -0.1;
		g_EnemyRobot[nCnt].fBullet_MoveX = EGUN_BULLET_SPEED;
		g_EnemyRobot[nCnt].bHit = false;
		g_EnemyRobot[nCnt].bUse = false;
		g_EnemyRobot[nCnt].nCntPreAttack = ENEMYROBOT_PREATTACK_TIME;
		g_EnemyRobot[nCnt].nCntBullet = 1;                                     //ロボットの弾数一発しかない

		//刀タイプのパラメータ
		g_EnemyRobot[nCnt].bRecoveryFlag = false;                            //硬直状態フラグ
		g_EnemyRobot[nCnt].nCntRecovery = 0;                                 //攻撃後硬直時間
		g_EnemyRobot[nCnt].bAttack = false;                                  //攻撃フラグ
		g_EnemyRobot[nCnt].bJump = false;                                    //ジャンプフラグ
		g_EnemyRobot[nCnt].bAttackDropFlag = false;                          //ジャンプの落下した時の攻撃判定有効フラグ

		pVtx[0+4*nCnt].pos = D3DXVECTOR3(g_EnemyRobot[nCnt].fPosX, g_EnemyRobot[nCnt].fPosY, 0.0f);
		pVtx[1+4*nCnt].pos = D3DXVECTOR3(g_EnemyRobot[nCnt].fPosX + ENEMYROBOT_WIDTH, g_EnemyRobot[nCnt].fPosY, 0.0f);
		pVtx[2+4*nCnt].pos = D3DXVECTOR3(g_EnemyRobot[nCnt].fPosX, g_EnemyRobot[nCnt].fPosY + ENEMYROBOT_HEIGHT, 0.0f);
		pVtx[3+4*nCnt].pos = D3DXVECTOR3(g_EnemyRobot[nCnt].fPosX + ENEMYROBOT_WIDTH, g_EnemyRobot[nCnt].fPosY + ENEMYROBOT_HEIGHT, 0.0f);

	}

	g_pVtxBufferEnemyRobotPolygon -> Unlock();                                     //バッファのアンロック

	return S_OK;
}

//-----------------------------------------------------------------------------------------
//終了処理
//-----------------------------------------------------------------------------------------
void UninitEnemyRobot(void)
{
	// テクスチャインターフェイスの片付け
	for (int nCnt = 0; nCnt < NUM_ENEMYROBOTTEXTURE; nCnt++)
	{
		//NULLではなかったら解放
		if(g_pTextureEnemyRobotPolygon[nCnt] != NULL)
		{
			g_pTextureEnemyRobotPolygon[nCnt] -> Release(); 
			g_pTextureEnemyRobotPolygon[nCnt] = NULL; 
		}
	}

	//頂点バッファインターフェイスの片付け
	if(g_pVtxBufferEnemyRobotPolygon != NULL)
	{
		g_pVtxBufferEnemyRobotPolygon -> Release(); 
		g_pVtxBufferEnemyRobotPolygon = NULL; 
	}
}

//-----------------------------------------------------------------------------------------
//プレーヤーデータの更新処理
//-----------------------------------------------------------------------------------------
void UpdateEnemyRobot(void)
{
	//ステージ01のデータ取得
	STAGE01 *pStage01;
	pStage01 = GetStage01();

	for (int nCnt = 0; nCnt < MAX_ENEMYROBOT; nCnt++)
	{
		//もし敵は使用可の状態なら
		if (g_EnemyRobot[nCnt].bUse == true)
		{
			//座標保存
			g_EnemyRobot[nCnt].fPosXOld = g_EnemyRobot[nCnt].fPosX;
			g_EnemyRobot[nCnt].fPosYOld = g_EnemyRobot[nCnt].fPosY;

			//-----------------------------------------------------------------------------------------
			//EnemyRobotの移動&攻撃処理
			//-----------------------------------------------------------------------------------------
			if(g_EnemyRobot[nCnt].nType == 1) EnemyRobotMove01(nCnt);                         //敵の移動パターン
			if(g_EnemyRobot[nCnt].nType == 2) EnemyRobotMove02(nCnt);                         //敵の移動パターン

			//移動
			g_EnemyRobot[nCnt].fPosX += g_EnemyRobot[nCnt].fMoveX;
			g_EnemyRobot[nCnt].fPosY += g_EnemyRobot[nCnt].fMoveY;

			//-----------------------------------------------------------------------------------------
			//EnemyRobotの当り判定処理
			//-----------------------------------------------------------------------------------------
			HitCheckForBullet(nCnt);                        //プレーヤーの弾に対する当り判定

			//EnemyRobotの重力影響
			g_EnemyRobot[nCnt].fMoveY += 0.15f;

			if(g_EnemyRobot[nCnt].fPosY + ENEMYROBOT_HEIGHT > 447.0f + ENEMYROBOT_HEIGHT_ADJUST)
			{
				g_EnemyRobot[nCnt].fPosY = 447.0f - ENEMYROBOT_HEIGHT + ENEMYROBOT_HEIGHT_ADJUST;    //座標戻り
				g_EnemyRobot[nCnt].fMoveY = 0;                               //Y軸スピード0

				//刀タイプのパラメータ
				if(g_EnemyRobot[nCnt].nType == 2)
				{
					//ジャンプ攻撃の後
					if(g_EnemyRobot[nCnt].bJump == true)
					{
						g_EnemyRobot[nCnt].bJump = false;                          //ジャンプフラグを戻す
						g_EnemyRobot[nCnt].nCntRecovery = RECOVERY_TIME;           //硬直時間セット
						g_EnemyRobot[nCnt].bRecoveryFlag = true;                   //硬直モードに入る
						g_EnemyRobot[nCnt].bAttack = false;
						g_EnemyRobot[nCnt].fMoveX = 0;
						g_EnemyRobot[nCnt].bAttackDropFlag = false;
					}
				}
			}
		}

		UpdatePolygonEnemyRobot();
	}
}

//-----------------------------------------------------------------------------------------
//敵の移動方法01
//-----------------------------------------------------------------------------------------
void EnemyRobotMove01(int nCnt)
{
	//プレーヤーのデータ取得
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	//-----------------------------------
	//着弾点計算
	//-----------------------------------
	//前フレームEnemyRobotの体力が0になったかどうか
	if(g_EnemyRobot[nCnt].nLife <= 0)
	{
		g_EnemyRobot[nCnt].bUse = false;         //敵を未使用にする
		pPlayer -> fScore += 100;                //スコア加算
		SetParticalCenter(2,g_EnemyRobot[nCnt].fPosX+ENEMYROBOT_WIDTH*0.5,g_EnemyRobot[nCnt].fPosY+ENEMYROBOT_HEIGHT*0.5,10,5,true);
		PlaySound(SOUND_LABEL_SE_EXPLOSION);
	}

	//-----------------------------------------------------------------------------------------
	//EnemyRobotの移動処理と攻撃処理
	//-----------------------------------------------------------------------------------------
	//硬直状態じゃない
	//EnemyRobotはプレーヤーの右側にいる
	if(g_EnemyRobot[nCnt].fPosX - pPlayer->fPosX_SUM > pPlayer -> fPosX)
	{
		//向き調整
		g_EnemyRobot[nCnt].fDirect = -0.1f;

		//探知範囲外(移動)
		//条件：攻撃範囲に入るかつ準備状態なし
		if(g_EnemyRobot[nCnt].bAttack == false)
		{
			if( (g_EnemyRobot[nCnt].fPosX - pPlayer->fPosX_SUM - pPlayer -> fPosX) > 250 + rand()%100 )
			{
				g_EnemyRobot[nCnt].fMoveX = -EGUN_MOVEMODE_XSPEED;
				g_EnemyRobot[nCnt].fDirect = -0.1f;
				g_EnemyRobot[nCnt].bMove = true;
			}

			else
			{
				g_EnemyRobot[nCnt].bAttack = true;
			}
		}

		//探知範囲内
		else
		{
			float ER_Center = g_EnemyRobot[nCnt].fPosX + ENEMYROBOT_WIDTH/2 - pPlayer->fPosX_SUM;    //敵の中心座標

			//敵がスクリーンの表示範囲内
			if( ER_Center <= SCREEN_WIDTH )
			{
				//動きを止め,攻撃準備に入る
				g_EnemyRobot[nCnt].bMove = false;
				g_EnemyRobot[nCnt].fMoveX = 0;                            //移動スピードを0にする
				g_EnemyRobot[nCnt].nCntPreAttack--;                       //攻撃準備時間カウントダウン

				//攻撃状態に入る
				if(g_EnemyRobot[nCnt].nCntPreAttack <= 0)
				{
					//攻撃開始
					SetBulletEnemy02(3,g_EnemyRobot[nCnt].fPosX + BULLETENEMYROBOT_DISTANCEX,g_EnemyRobot[nCnt].fPosY + BULLETENEMYROBOT_DISTANCEY,-g_EnemyRobot[nCnt].fBullet_MoveX,0,240,0.0f,6);
					g_EnemyRobot[nCnt].nCntBullet--;

					//弾切れ
					if(g_EnemyRobot[nCnt].nCntBullet <= 0)
					{
						g_EnemyRobot[nCnt].nCntBullet = 1;
						g_EnemyRobot[nCnt].nCntPreAttack = ENEMYROBOT_PREATTACK_TIME;
					}
				}
			}

			//敵がスクリーンの表示範囲外(移動)
			else
			{
				g_EnemyRobot[nCnt].fMoveX = -EGUN_MOVEMODE_XSPEED;
				g_EnemyRobot[nCnt].fDirect = -0.1f;
				g_EnemyRobot[nCnt].bMove = true;
			}
		}
	}

	//EnemyRobotはプレーヤーの左側にいる
	else
	{
		//向き調整
		g_EnemyRobot[nCnt].fDirect = 0.1f;

		//探知範囲外
		if(g_EnemyRobot[nCnt].bAttack == false)
		{
			if( (pPlayer -> fPosX - g_EnemyRobot[nCnt].fPosX + pPlayer->fPosX_SUM) > 250 + rand()%100)
			{
				g_EnemyRobot[nCnt].fMoveX = EGUN_MOVEMODE_XSPEED;
				g_EnemyRobot[nCnt].fDirect = 0.1f;
				g_EnemyRobot[nCnt].bMove = true;                           //描画用フラグ
			}

			else
			{
				g_EnemyRobot[nCnt].bAttack = true;
			}
		}

		//探知範囲内
		else
		{
			float ER_Center = g_EnemyRobot[nCnt].fPosX + ENEMYROBOT_WIDTH/2 - pPlayer->fPosX_SUM;    //敵の中心座標
			
			//敵がスクリーンの表示範囲内
			if( ER_Center >= 0 )
			{
				//動きを止め,攻撃準備に入る
				g_EnemyRobot[nCnt].bMove = false;                         //描画用フラグ
				g_EnemyRobot[nCnt].fMoveX = 0;                            //移動スピードを0にする
				g_EnemyRobot[nCnt].nCntPreAttack--;                       //攻撃準備時間カウントダウン

				//攻撃状態に入る
				if(g_EnemyRobot[nCnt].nCntPreAttack <= 0)
				{
					//攻撃開始
					SetBulletEnemy02(3,g_EnemyRobot[nCnt].fPosX - BULLETENEMYROBOT_DISTANCEX + ENEMYROBOT_WIDTH,g_EnemyRobot[nCnt].fPosY + BULLETENEMYROBOT_DISTANCEY,g_EnemyRobot[nCnt].fBullet_MoveX,0,240,0.0f,6);
					g_EnemyRobot[nCnt].nCntBullet--;

					//弾切れ
					if(g_EnemyRobot[nCnt].nCntBullet <= 0)
					{
						g_EnemyRobot[nCnt].nCntBullet = 1;
						g_EnemyRobot[nCnt].nCntPreAttack = ENEMYROBOT_PREATTACK_TIME;
					}
				}
			}

			//敵がスクリーンの表示範囲外(移動)
			else
			{
				g_EnemyRobot[nCnt].fMoveX = EGUN_MOVEMODE_XSPEED;
				g_EnemyRobot[nCnt].fDirect = 0.1f;
				g_EnemyRobot[nCnt].bMove = true;                           //描画用フラグ
			}
		}
	}

	//プレーヤーが攻撃範囲外にいるか
	if( (pPlayer -> fPosX - g_EnemyRobot[nCnt].fPosX + pPlayer->fPosX_SUM < -500) || (pPlayer -> fPosX - g_EnemyRobot[nCnt].fPosX + pPlayer->fPosX_SUM > 500 ) )
	{
		g_EnemyRobot[nCnt].bAttack = false;                     //移動状態に戻る
	}
}

//-----------------------------------------------------------------------------------------
//敵の移動方法02
//-----------------------------------------------------------------------------------------
void EnemyRobotMove02(int nCnt)
{
	//プレーヤーのデータ取得
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	//-----------------------------------
	//着弾点計算
	//-----------------------------------
	//前フレームEnemyRobotの体力が0になったかどうか
	if(g_EnemyRobot[nCnt].nLife <= 0)
	{
		g_EnemyRobot[nCnt].bUse = false;         //敵を未使用にする
		pPlayer -> fScore += 100;                //スコア加算
		SetParticalCenter(2,g_EnemyRobot[nCnt].fPosX+ENEMYROBOT_WIDTH*0.5,g_EnemyRobot[nCnt].fPosY+ENEMYROBOT_HEIGHT*0.5,10,5,true);
		PlaySound(SOUND_LABEL_SE_EXPLOSION);
	}

	//-----------------------------------------------------------------------------------------
	//EnemyRobotの移動処理と攻撃処理
	//-----------------------------------------------------------------------------------------
	//硬直状態じゃない
	if(g_EnemyRobot[nCnt].bRecoveryFlag == false)
	{
		//移動状態確認
		if( ( (g_EnemyRobot[nCnt].fPosX - pPlayer->fPosX_SUM - pPlayer -> fPosX) > MOVE_ACTION_DISTANCE ) || ( (g_EnemyRobot[nCnt].fPosX - pPlayer->fPosX_SUM - pPlayer -> fPosX) < -MOVE_ACTION_DISTANCE ))
		{
			g_EnemyRobot[nCnt].bMove = true;
		}

		//攻撃状態に入る
		else
		{
			g_EnemyRobot[nCnt].bMove = false;
		}

		//探知範囲外(移動)
		if( (g_EnemyRobot[nCnt].bMove == true) && (g_EnemyRobot[nCnt].bAttack == false) )
		{
			//enemyrobotはプレーヤーの右側にいる
			if((g_EnemyRobot[nCnt].fPosX - pPlayer->fPosX_SUM) > pPlayer -> fPosX)
			{
				g_EnemyRobot[nCnt].fMoveX = -EKNIFE_MOVEMODE_XSPEED;
				g_EnemyRobot[nCnt].fDirect = -0.1f;
			}

			//enemyrobotはプレーヤーの左側にいる
			else
			{
				g_EnemyRobot[nCnt].fMoveX = EKNIFE_MOVEMODE_XSPEED;
				g_EnemyRobot[nCnt].fDirect = 0.1f;
			}
		}
				

		//探知範囲内
		else
		{
			float ER_Center = g_EnemyRobot[nCnt].fPosX + ENEMYROBOT_WIDTH/2 - pPlayer->fPosX_SUM;    //敵の中心座標

			//敵の中心座標は画面表示範囲内
			if( (ER_Center >= 0) && (ER_Center <= SCREEN_WIDTH) )
			{
				//スピード計算
				if(g_EnemyRobot[nCnt].bAttack == false)
				{
					ATTACK_ACTION(&g_EnemyRobot[nCnt]);         //敵の攻撃時の移動速度の計算
					g_EnemyRobot[nCnt].bAttack = true;
				}

				else
				{
					if(g_EnemyRobot[nCnt].bJump == true)
					{
						//落下していない
						if(g_EnemyRobot[nCnt].fMoveY < 0)
						{
							g_EnemyRobot[nCnt].bAttackDropFlag = false;
						}
						//落下状態
						else
						{
							g_EnemyRobot[nCnt].bAttackDropFlag = true;
						}
					}
				}
			}

			//敵の中心座標は画面表示範囲内
			else
			{
				//enemyrobotはプレーヤーの右側にいる
				if((g_EnemyRobot[nCnt].fPosX - pPlayer->fPosX_SUM) > pPlayer -> fPosX)
				{
					g_EnemyRobot[nCnt].fMoveX = -EKNIFE_MOVEMODE_XSPEED;
					g_EnemyRobot[nCnt].fDirect = -0.1f;
					g_EnemyRobot[nCnt].bMove = true;
				}

				//enemyrobotはプレーヤーの左側にいる
				else
				{
					g_EnemyRobot[nCnt].fMoveX = EKNIFE_MOVEMODE_XSPEED;
					g_EnemyRobot[nCnt].fDirect = 0.1f;
					g_EnemyRobot[nCnt].bMove = true;
				}
			}
		}
	}

	//硬直状態
	else
	{
		g_EnemyRobot[nCnt].nCntRecovery--;    //準備時間のカウントダウン

		//硬直状態終了
		if(g_EnemyRobot[nCnt].nCntRecovery <= 0)
		{
			g_EnemyRobot[nCnt].bRecoveryFlag = false;
		}
	}
}

//-----------------------------------------------------------------------------------------
//刀タイプの敵の攻撃計算
//-----------------------------------------------------------------------------------------
void ATTACK_ACTION(ENEMYROBOT *enemy)
{
	float fTime = 0;                //敵がプレーヤーの位置に着く時間
	float fAdjustX = (KNIFEENEMYROBOT_DISTANCEX_L + KNIFEENEMYROBOT_DISTANCEX_R)*0.5;             //X軸距離の補正

	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	if(enemy->bJump == false)
	{
		enemy->fMoveY = -E_JUMP_POWER;    //敵のジャンプ力
		enemy->bJump = true;              //ジャンプフラグオン
	}

	fTime = (E_JUMP_POWER / GRAVITY)*2; //敵の空中時間

	//enemy02はプレーヤーの右側にいる
	if(enemy->fPosX - pPlayer->fPosX_SUM > pPlayer -> fPosX)
	{
		enemy->fMoveX = ( (pPlayer -> fPosX + PLAYER_WIDTH*0.5) - (enemy->fPosX - pPlayer -> fPosX_SUM + fAdjustX) ) / fTime;     //砲弾X軸スピード = ( プレーヤーX座標 - 砲弾X座標 ) / 総時間
		enemy->fDirect = -0.1f;
	}

	//enemy02はプレーヤーの左側にいる
	else
	{
		enemy->fMoveX = ( (pPlayer -> fPosX + PLAYER_WIDTH*0.5) - (enemy->fPosX - pPlayer -> fPosX_SUM - fAdjustX + ENEMYROBOT_WIDTH) ) / fTime;     //砲弾X軸スピード = ( プレーヤーX座標 - 砲弾X座標 ) / 総時間
		enemy->fDirect = 0.1f;
	}
}

//-----------------------------------------------------------------------------------------
//プレーヤーの弾に対する当たり判定
//-----------------------------------------------------------------------------------------
void HitCheckForBullet(int nCnt)
{
	//プレーヤーのデータ取得
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	//プレーヤーの弾データ取得
	BULLET *pBullet;
	pBullet = GetBullet();

	float fTop = g_EnemyRobot[nCnt].fPosY + ENEMYROBOT_TOP;
	float fBottom = g_EnemyRobot[nCnt].fPosY + ENEMYROBOT_HEIGHT - ENEMYROBOT_HEIGHT_CHECK;
	float fLeft = g_EnemyRobot[nCnt].fPosX - pPlayer->fPosX_SUM + ENEMYROBOT_HORRIZON;
	float fRight = g_EnemyRobot[nCnt].fPosX - pPlayer->fPosX_SUM + ENEMYROBOT_WIDTH - ENEMYROBOT_HORRIZON;

	BULLET_HITCHECK *pBulletHitcheck = GetBulletHitcheck();                 //弾の当り判定範囲をゲット

	for(int nCntBullet = 0; nCntBullet <= MAX_BULLET; nCntBullet++)
	{
		//弾が使われているか
		if( pBullet[nCntBullet].bUse == true)
		{
			//当り判定
			if(
				(fTop <= pBullet[nCntBullet].pos.y + pBulletHitcheck -> Up) &&
				(fBottom >= pBullet[nCntBullet].pos.y + pBulletHitcheck -> Bottom) &&
				(fLeft <= pBullet[nCntBullet].pos.x + pBulletHitcheck -> Left) &&
				(fRight >= pBullet[nCntBullet].pos.x + pBulletHitcheck -> Right)
				)
			{
				g_EnemyRobot[nCnt].nLife--;
				g_EnemyRobot[nCnt].bHit = true;
				pBullet[nCntBullet].bUse = false;
				PlaySound(SOUND_LABEL_SE_HITENEMY);
				//パーティクル
				SetParticalCenter(2,pBullet[nCntBullet].pos.x+BULLET_WIDTH*0.5,pBullet[nCntBullet].pos.y+BULLET_HEIGHT*0.5,1,4,false);
			}
		}
	}
}

//-----------------------------------------------------------------------------------------
//ロボット描画
//-----------------------------------------------------------------------------------------
void DrawEnemyRobot(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	pD3DDevice->SetStreamSource(0, g_pVtxBufferEnemyRobotPolygon, 0, sizeof(VERTEX_2D));              //VRAMからGPUにデータをストリーム
	pD3DDevice->SetFVF(FVF_VERTEX2D);

	for (int nCnt = 0; nCnt < MAX_ENEMYROBOT; nCnt++)
	{
		//もし敵は使用可の状態なら
		if (g_EnemyRobot[nCnt].bUse == true)
		{
			//背景の鉄欄干：頂点フォーマット設定
			pD3DDevice->SetTexture(0, g_pTextureEnemyRobotPolygon[0]);				 //テクスチャの設定
			pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt*4, NUM_POLYGON);                                            //テクスチャメモリの確保
		}
	}
}

//-----------------------------------------------------------------------------------------
//プレーヤーポリゴンのデータ更新
//-----------------------------------------------------------------------------------------
void UpdatePolygonEnemyRobot(void)
{
	VERTEX_2D *pVtx = NULL;
	LPDIRECT3DDEVICE9 pD3DDevice;
	PLAYER *pPlayer = GetPlayer();
	pD3DDevice = Get_Device();

	g_pVtxBufferEnemyRobotPolygon -> Lock(0, 0, (void**)&pVtx, 0);          //仮想アドレスを取得しバッファをロックする
	MakeVertexPolygonPlayer(pD3DDevice, pVtx);

	for (int nCnt = 0; nCnt < MAX_ENEMYROBOT; nCnt++)
	{
		if (g_EnemyRobot[nCnt].bUse == true)
		{
			//敵の座標の計算：敵の絶対座標-プレーヤーの移動量 = 敵のプレーヤーに相対する座標 (X軸のみ)
			pVtx[0+4*nCnt].pos = D3DXVECTOR3(g_EnemyRobot[nCnt].fPosX - pPlayer->fPosX_SUM, g_EnemyRobot[nCnt].fPosY, 0.0f);
			pVtx[1+4*nCnt].pos = D3DXVECTOR3(g_EnemyRobot[nCnt].fPosX + ENEMYROBOT_WIDTH - pPlayer->fPosX_SUM, g_EnemyRobot[nCnt].fPosY, 0.0f);
			pVtx[2+4*nCnt].pos = D3DXVECTOR3(g_EnemyRobot[nCnt].fPosX - pPlayer->fPosX_SUM, g_EnemyRobot[nCnt].fPosY + ENEMYROBOT_HEIGHT, 0.0f);
			pVtx[3+4*nCnt].pos = D3DXVECTOR3(g_EnemyRobot[nCnt].fPosX + ENEMYROBOT_WIDTH - pPlayer->fPosX_SUM, g_EnemyRobot[nCnt].fPosY + ENEMYROBOT_HEIGHT, 0.0f);

			//敵のテクスチャ設定
			if(g_EnemyRobot[nCnt].nType == 1) UpdatePolygonEnemyRobotTex01(g_EnemyRobot[nCnt],pVtx+nCnt*4);
			if(g_EnemyRobot[nCnt].nType == 2) UpdatePolygonEnemyRobotTex02(g_EnemyRobot[nCnt],pVtx+nCnt*4);

			pVtx[0+4*nCnt].rhw = 1.0f;
			pVtx[1+4*nCnt].rhw = 1.0f;
			pVtx[2+4*nCnt].rhw = 1.0f;
			pVtx[3+4*nCnt].rhw = 1.0f;

			if(g_EnemyRobot[nCnt].bHit == false)
			{
				pVtx[0+4*nCnt].color = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[1+4*nCnt].color = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[2+4*nCnt].color = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[3+4*nCnt].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			}

			else
			{
				pVtx[0+4*nCnt].color = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[1+4*nCnt].color = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[2+4*nCnt].color = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[3+4*nCnt].color = D3DCOLOR_RGBA(255, 0, 0, 255);
				g_EnemyRobot[nCnt].bHit = false;
			}
		}
	}
	g_pVtxBufferEnemyRobotPolygon->Unlock();                                     //バッファのアンロック
}

//-----------------------------------------------------------------------------------------
//プレーヤーポリゴンの表示位置X,ポリゴンの表示位置Y,ポリゴンの幅,ポリゴンの高さ
//-----------------------------------------------------------------------------------------
void MakeVertexPolygonEnemyRobot(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx)
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

//-----------------------------------------------------------------------------------------
//敵銃タイプのテクスチャ設定
//-----------------------------------------------------------------------------------------
void UpdatePolygonEnemyRobotTex01(ENEMYROBOT enemy,VERTEX_2D *pVtx)
{
	static int nRunCnt = 0;                                       //ランニングマンテクスチャの切り替えカウンター
	static int nActionFlame = 1;                                  //ランニングマンの動きを遅くするためのカウンター
	//敵が移動状態
	if(enemy.bMove == true)
	{
		if (enemy.fDirect == -0.1f)
		{
			pVtx[0].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.25), 0.0f);
			pVtx[1].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.25) + 0.25f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.25), 0.5f);
			pVtx[3].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.25) + 0.25f, 0.5f);
		}

		else
		{
			pVtx[0].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.25) + 0.25f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.25), 0.0f);
			pVtx[2].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.25) + 0.25f, 0.5f);
			pVtx[3].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.25), 0.5f);
		}
	}

	//敵が止まっている
	else
	{
		if (enemy.fDirect == -0.1f)
		{
			//モーション01
			if(enemy.nCntPreAttack > ENEMYROBOT_PREATTACK_TIME * 0.5)
			{
				pVtx[0].tex = D3DXVECTOR2( 0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2( 0.25f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2( 0.0f, 0.5f);
				pVtx[3].tex = D3DXVECTOR2( 0.25f, 0.5f);
			}

			//モーション02
			else 
			{
				pVtx[0].tex = D3DXVECTOR2( 0.5f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2( 0.75f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2( 0.5f, 0.5f);
				pVtx[3].tex = D3DXVECTOR2( 0.75f, 0.5f);
			}
		}

		else
		{
			//モーション01
			if(enemy.nCntPreAttack > ENEMYROBOT_PREATTACK_TIME * 0.5)
			{
				pVtx[0].tex = D3DXVECTOR2( 0.25f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2( 0.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2( 0.25f, 0.5f);
				pVtx[3].tex = D3DXVECTOR2( 0.0f, 0.5f);
			}

			//モーション02
			else 
			{
				pVtx[0].tex = D3DXVECTOR2( 0.75f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2( 0.5f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2( 0.75f, 0.5f);
				pVtx[3].tex = D3DXVECTOR2( 0.5f, 0.5f);
			}
		}
	}

	//テクスチャの切り替えは4フレームごとに行う
	if ((nActionFlame % 8) == 0)
	{
		nRunCnt++;
	}

	//変数nRunCntのオーバーフロー防止
	if (nRunCnt == 10000)
	{
		nRunCnt = 0;
	}

	//アクションフレームのインクリメント
	nActionFlame++;

	//変数nActionFlameのオーバーフロー防止
	if (nActionFlame >= 10000)
	{
		nActionFlame = 0;
	}
}
//-----------------------------------------------------------------------------------------
//敵刀タイプのテクスチャ設定
//-----------------------------------------------------------------------------------------
void UpdatePolygonEnemyRobotTex02(ENEMYROBOT enemy,VERTEX_2D *pVtx)
{
	static int nRunCnt = 0;                                       //ランニングマンテクスチャの切り替えカウンター
	static int nActionFlame = 1;                                  //ランニングマンの動きを遅くするためのカウンター

	if(enemy.bRecoveryFlag == false)
	{
		//敵が移動状態
		if(enemy.bMove == true)
		{
			if (enemy.fDirect == -0.1f)
			{
				pVtx[0].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.25), 0.5f);
				pVtx[1].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.25) + 0.25f, 0.5f);
				pVtx[2].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.25), 1.0f);
				pVtx[3].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.25) + 0.25f, 1.0f);
			}

			else
			{
				pVtx[0].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.25) + 0.25f, 0.5f);
				pVtx[1].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.25), 0.5f);
				pVtx[2].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.25) + 0.25f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.25), 1.0f);
			}
		}

		//敵が攻撃状態
		else
		{
			if (enemy.fDirect == -0.1f)
			{
				if(enemy.bJump == true)
				{
					//上昇
					if(enemy.bAttackDropFlag == false)
					{

						pVtx[0].tex = D3DXVECTOR2( 0.5f, 0.5f);
						pVtx[1].tex = D3DXVECTOR2( 0.75f, 0.5f);
						pVtx[2].tex = D3DXVECTOR2( 0.5f, 1.0f);
						pVtx[3].tex = D3DXVECTOR2( 0.75f, 1.0f);
					}

					//降下
					else 
					{
						pVtx[0].tex = D3DXVECTOR2( 0.75f, 0.5f);
						pVtx[1].tex = D3DXVECTOR2( 1.0f, 0.5f);
						pVtx[2].tex = D3DXVECTOR2( 0.75f, 1.0f);
						pVtx[3].tex = D3DXVECTOR2( 1.0f, 1.0f);
					}
				}

				else
				{
					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.5f);
					pVtx[1].tex = D3DXVECTOR2(0.25f, 0.5f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(0.25f, 1.0f);
				}
			}

			else
			{
				if(enemy.bJump == true)
				{
					//上昇
					if(enemy.bAttackDropFlag == false)
					{

						pVtx[0].tex = D3DXVECTOR2( 0.75f, 0.5f);
						pVtx[1].tex = D3DXVECTOR2( 0.5f, 0.5f);
						pVtx[2].tex = D3DXVECTOR2( 0.75f, 1.0f);
						pVtx[3].tex = D3DXVECTOR2( 0.5f, 1.0f);
					}

					//降下
					else 
					{
						pVtx[0].tex = D3DXVECTOR2( 1.0f, 0.5f);
						pVtx[1].tex = D3DXVECTOR2( 0.75f, 0.5f);
						pVtx[2].tex = D3DXVECTOR2( 1.0f, 1.0f);
						pVtx[3].tex = D3DXVECTOR2( 0.75f, 1.0f);
					}
				}

				else
				{
					pVtx[0].tex = D3DXVECTOR2(0.25f, 0.5f);
					pVtx[1].tex = D3DXVECTOR2(0.0f, 0.5f);
					pVtx[2].tex = D3DXVECTOR2(0.25f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
				}
			}
		}
	}

	//敵は硬直状態
	else
	{
		if (enemy.fDirect == -0.1f)
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.5f);
			pVtx[1].tex = D3DXVECTOR2(0.25f, 0.5f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.25f, 1.0f);
		}

		else
		{
			pVtx[0].tex = D3DXVECTOR2(0.25f, 0.5f);
			pVtx[1].tex = D3DXVECTOR2(0.0f, 0.5f);
			pVtx[2].tex = D3DXVECTOR2(0.25f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
		}
	}
}

//-----------------------------------------------------------------------------------------
//敵情報取得
//-----------------------------------------------------------------------------------------
ENEMYROBOT *GetEnemyRobot(void)
{

	return &g_EnemyRobot[0];
}

//-----------------------------------------------------------------------------------------
//敵セット
//-----------------------------------------------------------------------------------------
void SetEnemyRobot(int nType,float fPosX,float fPosY,float fMoveX,float fDirect,float nEnemyNum)
{
	for (int nCnt = 0; nCnt < MAX_ENEMYROBOT; nCnt++) 
	{
		if(g_EnemyRobot[nCnt].bUse == false)
		{
			g_EnemyRobot[nCnt].nType = nType;                                     //EnemyRobotのタイプ : 1.銃タイプ 2.刀タイプ
			g_EnemyRobot[nCnt].nNumEnemy = nEnemyNum;                               //敵の番号
			g_EnemyRobot[nCnt].fPosX = fPosX;                                   //EnemyRobotの座標(X座標)
			g_EnemyRobot[nCnt].fPosY = fPosY;                                   //EnemyRobotの座標(Y座標)
			g_EnemyRobot[nCnt].fPosXOld = g_EnemyRobot[nCnt].fPosX;                                //EnemyRobotの古い座標(X座標)
			g_EnemyRobot[nCnt].fPosYOld = g_EnemyRobot[nCnt].fPosY;                                //EnemyRobotの古い座標(X座標)
			g_EnemyRobot[nCnt].fMoveX = fMoveX;                                  //敵X軸の移動スピード
			g_EnemyRobot[nCnt].fMoveY = 0;                                  //敵Y軸の移動スピード
			g_EnemyRobot[nCnt].fDirect = fDirect;                                 //向きのフラグ(-0.1fなら左,0.1fなら右)
			g_EnemyRobot[nCnt].bMove = true;                                    //敵が動いているか
			g_EnemyRobot[nCnt].bHit = false;                                     //攻撃を受けるフラグ
			g_EnemyRobot[nCnt].nLife = 1;                                     //敵の体力
			g_EnemyRobot[nCnt].nCntPreAttack = ENEMYROBOT_PREATTACK_TIME;        //弾発射までの時間
			g_EnemyRobot[nCnt].nCntBullet = 1;                                //一回連射できる弾の数
			g_EnemyRobot[nCnt].fBullet_MoveX = EGUN_BULLET_SPEED;                           //弾の移動速度

			//刀タイプのパラメータ
			g_EnemyRobot[nCnt].bRecoveryFlag = false;                            //硬直状態フラグ
			g_EnemyRobot[nCnt].nCntRecovery = RECOVERY_TIME;                     //攻撃後硬直時間
			g_EnemyRobot[nCnt].bAttack = false;                                  //攻撃フラグ
			g_EnemyRobot[nCnt].bJump = false;                                    //ジャンプフラグ
			g_EnemyRobot[nCnt].bAttackDropFlag = false;                          //ジャンプの落下した時の攻撃判定有効フラグ

			g_EnemyRobot[nCnt].bUse = true;                                     //使うかどうか
			break;
		}
	}
}
