//-----------------------------------------------------------------------------------------
//プレーヤー,道具などの処理[running02.cpp]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------------------
#include <time.h>
#include "main.h"
#include "enemy02.h"
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
#define ENEMY02_INTERATTACK_TIME (60)                  //敵の攻撃準備時間
#define ENEMY02_RECOVERYATTACK_TIME (90)              //敵の弾の装填時間
#define ENEMY02_PREATTACK_TIME (60)                    //敵の攻撃間隔

//敵の砲弾がプレーヤーに着く時間マクロ
#define E_TIMETOTAL (120.0f)                 //弾がプレーヤーの場所に着くまでの時間
#define E_TIMEUP (30.0f)                    //弾の上昇時間(状況2-1のみ使用)

//-----------------------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureEnemy02Polygon[NUM_ENEMY02TEXTURE] = {};           //テクスチャを操作するためのポインタ(テクスチャバッファインターフェイスにアクセスためのポインタ)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferEnemy02Polygon = NULL;                      //頂点バッファを操作するためのポインタ(頂点バッファインターフェイスにアクセスためのポインタ)

ENEMY02 g_Enemy02[NUM_ENEMY02POLYGON];
//-----------------------------------------------------------------------------------------
//重要定番関数達
//-----------------------------------------------------------------------------------------

//各変数の初期化
HRESULT InitEnemy02(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_2D *pVtx = NULL;                           //頂点構造体のポインタ
	pD3DDevice = Get_Device();

	//頂点バッファの生成
	if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_ENEMY02VERTEX*NUM_ENEMY02POLYGON, D3DUSAGE_WRITEONLY, FVF_VERTEX2D, D3DPOOL_MANAGED, &g_pVtxBufferEnemy02Polygon, NULL)))
	{
		return E_FAIL;
	}

	//ENEMY02テクスチャの取得
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, ENEMY02POLYGON01_TEXTURENAME, &g_pTextureEnemy02Polygon[0])))
	{
		MessageBox(NULL, "テクスチャの読み込みが失敗しました", "エラー", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	//プレーヤーのポリゴン
	g_pVtxBufferEnemy02Polygon -> Lock(0, 0, (void**)&pVtx, 0);                    //仮想アドレスを取得しバッファをロックする
	MakeVertexPolygonPlayer(pD3DDevice, pVtx);

	for (int nCnt = 0; nCnt < MAX_ENEMY02; nCnt++)
	{
		//プレーヤーの諸データの初期値
		g_Enemy02[nCnt].nNumEnemy = 0;                                        //敵の番号
		g_Enemy02[nCnt].fPosX = 0.0f;
		g_Enemy02[nCnt].fPosY = 0.0f;
		g_Enemy02[nCnt].fDirect = -0.1;
		g_Enemy02[nCnt].bHit = false;
		g_Enemy02[nCnt].bUse = false;
		g_Enemy02[nCnt].bPre = false;
		g_Enemy02[nCnt].bInter = false;
		g_Enemy02[nCnt].bRecovery = false;
		g_Enemy02[nCnt].nCntPreAttack = ENEMY02_PREATTACK_TIME;
		g_Enemy02[nCnt].nCntBullet = 3;
		g_Enemy02[nCnt].nCntInterAttack = ENEMY02_INTERATTACK_TIME;
		g_Enemy02[nCnt].nCntRecoveryAttack = ENEMY02_RECOVERYATTACK_TIME;

		pVtx[0+4*nCnt].pos = D3DXVECTOR3(g_Enemy02[nCnt].fPosX, g_Enemy02[nCnt].fPosY, 0.0f);
		pVtx[1+4*nCnt].pos = D3DXVECTOR3(g_Enemy02[nCnt].fPosX + ENEMY02_WIDTH, g_Enemy02[nCnt].fPosY, 0.0f);
		pVtx[2+4*nCnt].pos = D3DXVECTOR3(g_Enemy02[nCnt].fPosX, g_Enemy02[nCnt].fPosY + ENEMY02_WIDTH, 0.0f);
		pVtx[3+4*nCnt].pos = D3DXVECTOR3(g_Enemy02[nCnt].fPosX + ENEMY02_WIDTH, g_Enemy02[nCnt].fPosY + ENEMY02_WIDTH, 0.0f);

	}

	g_pVtxBufferEnemy02Polygon -> Unlock();                                     //バッファのアンロック

	return S_OK;
}
//終了処理
void UninitEnemy02(void)
{
	// テクスチャインターフェイスの片付け
	for (int nCnt = 0; nCnt < NUM_ENEMY02TEXTURE; nCnt++)
	{
		SAFE_RELEASE(g_pTextureEnemy02Polygon[nCnt]);
	}

	//頂点バッファインターフェイスの片付け
	for (int nCnt = 0; nCnt < NUM_ENEMY02POLYGON; nCnt++)
	{
		SAFE_RELEASE(g_pVtxBufferEnemy02Polygon);
	}
}

//プレーヤーデータの更新処理
void UpdateEnemy02(void)
{
	//プレーヤーのデータ取得
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	//プレーヤーの弾データ取得
	BULLET *pBullet;
	pBullet = GetBullet();

	//ステージ01のデータ取得
	STAGE01 *pStage01;
	pStage01 = GetStage01();

	for (int nCnt = 0; nCnt < MAX_ENEMY02; nCnt++)
	{
		//もし敵は使用可の状態なら
		if (g_Enemy02[nCnt].bUse == true)
		{
			//-----------------------------------
			//着弾点計算
			//-----------------------------------
			//前フレームEnemy02の体力が0になったかどうか
			if(g_Enemy02[nCnt].nLife <= 0)
			{
				g_Enemy02[nCnt].bUse = false;         //敵を未使用にする
				pPlayer -> fScore += 100;             //スコア加算
				SetParticalCenter(2,g_Enemy02[nCnt].fPosX+ENEMY02_WIDTH*0.5,g_Enemy02[nCnt].fPosY+ENEMY02_HEIGHT*0.5,10,15,true);
				PlaySound(SOUND_LABEL_SE_EXPLOSION);
			}

			//-----------------------------------------------------------------------------------------
			//Enemy02の移動処理と攻撃処理
			//-----------------------------------------------------------------------------------------
			//硬直状態じゃない
			if(g_Enemy02[nCnt].bRecovery == false)
			{
				//Enemy02はプレーヤーの右側にいる
				if(g_Enemy02[nCnt].fPosX - pPlayer->fPosX_SUM > pPlayer -> fPosX)
				{
					//向き調整
					g_Enemy02[nCnt].fDirect = -0.1f;

					//探知範囲外(移動)
					//条件：攻撃範囲に入るかつ準備状態なし
					if( (pPlayer -> fPosX + PLAYER_WIDTH - g_Enemy02[nCnt].fPosX + pPlayer->fPosX_SUM < -400) && (g_Enemy02[nCnt].bPre == false) )
					{
						g_Enemy02[nCnt].fPosX -= g_Enemy02[nCnt].fMoveX;
						g_Enemy02[nCnt].fDirect = -0.1f;
						g_Enemy02[nCnt].bMove = true;
					}

					//探知範囲内
					else
					{
						float E2_Center= g_Enemy02[nCnt].fPosX + ENEMY02_WIDTH/2 - pPlayer->fPosX_SUM;              //敵の中心座標

						//敵は表示範囲内にいる
						if(E2_Center <= (SCREEN_WIDTH-50) )
						{

							//動きを止め,攻撃準備に入る
							g_Enemy02[nCnt].bMove = false;
							g_Enemy02[nCnt].bPre = true;

							//攻撃状態に入る
							if(g_Enemy02[nCnt].nCntPreAttack == 0)
							{
								//攻撃開始
								if(g_Enemy02[nCnt].bInter == false)
								{
									D3DXVECTOR2 E_BulletSpeed = BulletLanding(g_Enemy02[nCnt]);                  //着弾点計算
									SetBulletEnemy02(2,g_Enemy02[nCnt].fPosX + BULLETENEMY02_DISTANCEX,g_Enemy02[nCnt].fPosY + BULLETENEMY02_DISTANCEY,E_BulletSpeed.x,E_BulletSpeed.y,180,0.0f,12);
									g_Enemy02[nCnt].nCntBullet--;

									//また弾あり
									if(g_Enemy02[nCnt].nCntBullet > 0)
									{
										g_Enemy02[nCnt].bInter = true;
									}

									//弾が打ち切ると硬直状態に入る
									else
									{
										g_Enemy02[nCnt].bRecovery = true;
									}

								}
								//連射間隔中
								else
								{
									g_Enemy02[nCnt].nCntInterAttack--;

									//連射間隔終了
									if(g_Enemy02[nCnt].nCntInterAttack == 0)
									{
										g_Enemy02[nCnt].nCntInterAttack = ENEMY02_INTERATTACK_TIME;
										g_Enemy02[nCnt].bInter = false;
									}
								}
							}

							//攻撃準備段階
							else
							{
								g_Enemy02[nCnt].nCntPreAttack--;
							}
						}

						//敵は表示範囲内にいない(移動)
						else
						{
							g_Enemy02[nCnt].fPosX -= g_Enemy02[nCnt].fMoveX;
							g_Enemy02[nCnt].fDirect = -0.1f;
							g_Enemy02[nCnt].bMove = true;
						}
					}
				}

				//Enemy02はプレーヤーの左側にいる
				else
				{
					//向き調整
					g_Enemy02[nCnt].fDirect = 0.1f;

					//探知範囲外
					if( (pPlayer -> fPosX - (g_Enemy02[nCnt].fPosX + ENEMY02_WIDTH) + pPlayer->fPosX_SUM > 400 ) && (g_Enemy02[nCnt].bPre == false) )
					{
						g_Enemy02[nCnt].fPosX += g_Enemy02[nCnt].fMoveX;
						g_Enemy02[nCnt].fDirect = 0.1f;
						g_Enemy02[nCnt].bMove = true;
					}

					//探知範囲内
					else
					{
						float E2_Center= g_Enemy02[nCnt].fPosX + ENEMY02_WIDTH/2 - pPlayer->fPosX_SUM;              //敵の中心座標

						//敵は表示範囲内にいる
						if(E2_Center >= 50)
						{
							//動きを止め,攻撃準備に入る
							g_Enemy02[nCnt].bMove = false;
							g_Enemy02[nCnt].bPre = true;

							//攻撃状態に入る
							if(g_Enemy02[nCnt].nCntPreAttack == 0)
							{

								//攻撃開始
								if(g_Enemy02[nCnt].bInter == false)
								{
									D3DXVECTOR2 E_BulletSpeed = BulletLanding(g_Enemy02[nCnt]);               //着弾点計算
									SetBulletEnemy02(2,g_Enemy02[nCnt].fPosX - BULLETENEMY02_DISTANCEX + ENEMY02_WIDTH,g_Enemy02[nCnt].fPosY + BULLETENEMY02_DISTANCEY,E_BulletSpeed.x,E_BulletSpeed.y,180,0.0f,12);
									g_Enemy02[nCnt].nCntBullet--;

									//また弾あり
									if(g_Enemy02[nCnt].nCntBullet > 0)
									{
										g_Enemy02[nCnt].bInter = true;
									}

									//弾が打ち切ると硬直状態に入る
									else
									{
										g_Enemy02[nCnt].bRecovery = true;
									}

								}
								//連射間隔中
								else
								{
									g_Enemy02[nCnt].nCntInterAttack--;

									//連射間隔終了
									if(g_Enemy02[nCnt].nCntInterAttack == 0)
									{
										g_Enemy02[nCnt].nCntInterAttack = ENEMY02_INTERATTACK_TIME;
										g_Enemy02[nCnt].bInter = false;
									}
								}
							}

							//攻撃準備段階
							else
							{
								g_Enemy02[nCnt].nCntPreAttack--;
							}
						}

						//敵は表示範囲内にいない(移動)
						else
						{
							g_Enemy02[nCnt].fPosX += g_Enemy02[nCnt].fMoveX;
							g_Enemy02[nCnt].fDirect = 0.1f;
							g_Enemy02[nCnt].bMove = true;
						}
					}
				}
			}


			//硬直状態
			else
			{
				g_Enemy02[nCnt].nCntRecoveryAttack--;

				//硬直状態終了
				if(g_Enemy02[nCnt].nCntRecoveryAttack == 0)
				{
					g_Enemy02[nCnt].nCntRecoveryAttack = ENEMY02_RECOVERYATTACK_TIME;
					g_Enemy02[nCnt].bRecovery = false;

					//弾の補充
					g_Enemy02[nCnt].nCntBullet = 3;

					//プレーヤーが攻撃範囲外にいるか
					if( (pPlayer -> fPosX + PLAYER_WIDTH - g_Enemy02[nCnt].fPosX + pPlayer->fPosX_SUM < -400) || (pPlayer -> fPosX - (g_Enemy02[nCnt].fPosX + ENEMY02_WIDTH) + pPlayer->fPosX_SUM > 400 ) )
					{
						g_Enemy02[nCnt].bPre = false;                     //移動状態に戻る
						g_Enemy02[nCnt].nCntPreAttack = ENEMY02_PREATTACK_TIME;
					}
				}
			}


			//-----------------------------------------------------------------------------------------
			//Enemy02の当り判定処理
			//-----------------------------------------------------------------------------------------

			//Enemy02の重力影響
			g_Enemy02[nCnt].fMoveY += 0.15f;
			g_Enemy02[nCnt].fPosY += g_Enemy02[nCnt].fMoveY;

			if(g_Enemy02[nCnt].fPosY + ENEMY02_HEIGHT > 447.0f + ENEMY02_HEIGHT_ADJUST) 
			{
				g_Enemy02[nCnt].fPosY = 447.0f - ENEMY02_HEIGHT + ENEMY02_HEIGHT_ADJUST;
			}

			float fTop = g_Enemy02[nCnt].fPosY + ENEMY02_TOP;
			float fBottom = g_Enemy02[nCnt].fPosY + ENEMY02_HEIGHT - ENEMY02_HEIGHT_CHECK;
			float fLeft = g_Enemy02[nCnt].fPosX - pPlayer->fPosX_SUM + ENEMY02_HORRIZON;
			float fRight = g_Enemy02[nCnt].fPosX - pPlayer->fPosX_SUM + ENEMY02_WIDTH - ENEMY02_HORRIZON;
			
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
						g_Enemy02[nCnt].nLife--;
						g_Enemy02[nCnt].bHit = true;
						pBullet[nCntBullet].bUse = false;
						PlaySound(SOUND_LABEL_SE_HITENEMY);
						//パーティクル
						SetParticalCenter(2,pBullet[nCntBullet].pos.x+BULLET_WIDTH*0.5,pBullet[nCntBullet].pos.y+BULLET_HEIGHT*0.5,1,4,false);
					}
				}
			}
		}
	}

	UpdatePolygonEnemy02();
}

//プレーヤーポリゴンのデータ更新
void UpdatePolygonEnemy02(void)
{
	static int nRunCnt = 0;                                       //ランニングマンテクスチャの切り替えカウンター
	static int nActionFlame = 1;                                  //ランニングマンの動きを遅くするためのカウンター
	VERTEX_2D *pVtx = NULL;
	LPDIRECT3DDEVICE9 pD3DDevice;
	PLAYER *pPlayer = GetPlayer();
	pD3DDevice = Get_Device();

	g_pVtxBufferEnemy02Polygon -> Lock(0, 0, (void**)&pVtx, 0);          //仮想アドレスを取得しバッファをロックする
	MakeVertexPolygonPlayer(pD3DDevice, pVtx);

	for (int nCnt = 0; nCnt < MAX_ENEMY02; nCnt++)
	{
		if (g_Enemy02[nCnt].bUse == true)
		{
			//敵の座標の計算：敵の絶対座標-プレーヤーの移動量 = 敵のプレーヤーに相対する座標 (X軸のみ)
			pVtx[0+4*nCnt].pos = D3DXVECTOR3(g_Enemy02[nCnt].fPosX - pPlayer->fPosX_SUM, g_Enemy02[nCnt].fPosY, 0.0f);
			pVtx[1+4*nCnt].pos = D3DXVECTOR3(g_Enemy02[nCnt].fPosX + ENEMY02_WIDTH - pPlayer->fPosX_SUM, g_Enemy02[nCnt].fPosY, 0.0f);
			pVtx[2+4*nCnt].pos = D3DXVECTOR3(g_Enemy02[nCnt].fPosX - pPlayer->fPosX_SUM, g_Enemy02[nCnt].fPosY + ENEMY02_HEIGHT, 0.0f);
			pVtx[3+4*nCnt].pos = D3DXVECTOR3(g_Enemy02[nCnt].fPosX + ENEMY02_WIDTH - pPlayer->fPosX_SUM, g_Enemy02[nCnt].fPosY + ENEMY02_HEIGHT, 0.0f);

			g_Enemy02[nCnt].fPosX_Relative = g_Enemy02[nCnt].fPosX - pPlayer->fPosX_SUM;                    //敵の相対座標を求める

			//敵が移動状態
			if(g_Enemy02[nCnt].bMove == true)
			{
				if (g_Enemy02[nCnt].fDirect == -0.1f)
				{
					pVtx[0+4*nCnt].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.25), 0.0f);
					pVtx[1+4*nCnt].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.25) + 0.25f, 0.0f);
					pVtx[2+4*nCnt].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.25), 0.5f);
					pVtx[3+4*nCnt].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.25) + 0.25f, 0.5f);
				}

				else
				{
					pVtx[0+4*nCnt].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.25) + 0.25f, 0.0f);
					pVtx[1+4*nCnt].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.25), 0.0f);
					pVtx[2+4*nCnt].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.25) + 0.25f, 0.5f);
					pVtx[3+4*nCnt].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.25), 0.5f);
				}
			}

			//敵が止まっている
			else
			{
				if (g_Enemy02[nCnt].fDirect == -0.1f)
				{
					//攻撃モーション準備
					if(g_Enemy02[nCnt].nCntPreAttack != 0)
					{
						//モーション01
						if(g_Enemy02[nCnt].nCntPreAttack > ENEMY02_PREATTACK_TIME * 0.25 * 3)
						{
							pVtx[0+4*nCnt].tex = D3DXVECTOR2( 0.0f, 0.5f);
							pVtx[1+4*nCnt].tex = D3DXVECTOR2( 0.25f, 0.5f);
							pVtx[2+4*nCnt].tex = D3DXVECTOR2( 0.0f, 1.0f);
							pVtx[3+4*nCnt].tex = D3DXVECTOR2( 0.25f, 1.0f);
						}

						//モーション02
						else if( (g_Enemy02[nCnt].nCntPreAttack <= ENEMY02_PREATTACK_TIME * 0.25 * 3) &&
								 (g_Enemy02[nCnt].nCntPreAttack > ENEMY02_PREATTACK_TIME * 0.25 * 2)
							)
						{
							pVtx[0+4*nCnt].tex = D3DXVECTOR2( 0.25f, 0.5f);
							pVtx[1+4*nCnt].tex = D3DXVECTOR2( 0.5f, 0.5f);
							pVtx[2+4*nCnt].tex = D3DXVECTOR2( 0.25f, 1.0f);
							pVtx[3+4*nCnt].tex = D3DXVECTOR2( 0.5f, 1.0f);
						}

						//モーション03
						else if( (g_Enemy02[nCnt].nCntPreAttack <= ENEMY02_PREATTACK_TIME * 0.25 * 2) &&
								 (g_Enemy02[nCnt].nCntPreAttack > ENEMY02_PREATTACK_TIME * 0.25)
							)
						{
							pVtx[0+4*nCnt].tex = D3DXVECTOR2( 0.5f, 0.5f);
							pVtx[1+4*nCnt].tex = D3DXVECTOR2( 0.75f, 0.5f);
							pVtx[2+4*nCnt].tex = D3DXVECTOR2( 0.5f, 1.0f);
							pVtx[3+4*nCnt].tex = D3DXVECTOR2( 0.75f, 1.0f);
						}

						//モーション04
						else
						{
							pVtx[0+4*nCnt].tex = D3DXVECTOR2( 0.75f, 0.5f);
							pVtx[1+4*nCnt].tex = D3DXVECTOR2( 1.0f, 0.5f);
							pVtx[2+4*nCnt].tex = D3DXVECTOR2( 0.75f, 1.0f);
							pVtx[3+4*nCnt].tex = D3DXVECTOR2( 1.0f, 1.0f);
						}
					}

					//攻撃モーション
					else
					{
						pVtx[0+4*nCnt].tex = D3DXVECTOR2( 0.75f, 0.5f);
						pVtx[1+4*nCnt].tex = D3DXVECTOR2( 1.0f, 0.5f);
						pVtx[2+4*nCnt].tex = D3DXVECTOR2( 0.75f, 1.0f);
						pVtx[3+4*nCnt].tex = D3DXVECTOR2( 1.0f, 1.0f);
					}
				}

				else
				{
					//攻撃モーション準備
					if(g_Enemy02[nCnt].nCntPreAttack != 0)
					{
						//モーション01
						if(g_Enemy02[nCnt].nCntPreAttack > ENEMY02_PREATTACK_TIME * 0.25 * 3)
						{
							pVtx[0+4*nCnt].tex = D3DXVECTOR2( 0.25f, 0.5f);
							pVtx[1+4*nCnt].tex = D3DXVECTOR2( 0.0f, 0.5f);
							pVtx[2+4*nCnt].tex = D3DXVECTOR2( 0.25f, 1.0f);
							pVtx[3+4*nCnt].tex = D3DXVECTOR2( 0.0f, 1.0f);
						}

						//モーション02
						else if( (g_Enemy02[nCnt].nCntPreAttack <= ENEMY02_PREATTACK_TIME * 0.25 * 3) &&
								 (g_Enemy02[nCnt].nCntPreAttack > ENEMY02_PREATTACK_TIME * 0.25 * 2)
							)
						{
							pVtx[0+4*nCnt].tex = D3DXVECTOR2( 0.5f, 0.5f);
							pVtx[1+4*nCnt].tex = D3DXVECTOR2( 0.25f, 0.5f);
							pVtx[2+4*nCnt].tex = D3DXVECTOR2( 0.5f, 1.0f);
							pVtx[3+4*nCnt].tex = D3DXVECTOR2( 0.25f, 1.0f);
						}
						//モーション03
						else if( (g_Enemy02[nCnt].nCntPreAttack <= ENEMY02_PREATTACK_TIME * 0.25 * 2) &&
								 (g_Enemy02[nCnt].nCntPreAttack > ENEMY02_PREATTACK_TIME * 0.25)
							)
						{
							pVtx[0+4*nCnt].tex = D3DXVECTOR2( 0.75f, 0.5f);
							pVtx[1+4*nCnt].tex = D3DXVECTOR2( 0.5f, 0.5f);
							pVtx[2+4*nCnt].tex = D3DXVECTOR2( 0.75f, 1.0f);
							pVtx[3+4*nCnt].tex = D3DXVECTOR2( 0.5f, 1.0f);
						}
						//モーション04
						else
						{
							pVtx[0+4*nCnt].tex = D3DXVECTOR2( 1.0f, 0.5f);
							pVtx[1+4*nCnt].tex = D3DXVECTOR2( 0.75f, 0.5f);
							pVtx[2+4*nCnt].tex = D3DXVECTOR2( 1.0f, 1.0f);
							pVtx[3+4*nCnt].tex = D3DXVECTOR2( 0.75f, 1.0f);
						}
					}

					//攻撃モーション
					else
					{
						pVtx[0+4*nCnt].tex = D3DXVECTOR2( 1.0f, 0.5f);
						pVtx[1+4*nCnt].tex = D3DXVECTOR2( 0.75f, 0.5f);
						pVtx[2+4*nCnt].tex = D3DXVECTOR2( 1.0f, 1.0f);
						pVtx[3+4*nCnt].tex = D3DXVECTOR2( 0.75f, 1.0f);
					}
				}
			}

			pVtx[0+4*nCnt].rhw = 1.0f;
			pVtx[1+4*nCnt].rhw = 1.0f;
			pVtx[2+4*nCnt].rhw = 1.0f;
			pVtx[3+4*nCnt].rhw = 1.0f;

			if(g_Enemy02[nCnt].bHit == false)
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
				g_Enemy02[nCnt].bHit = false;
			}
		}
	}
	g_pVtxBufferEnemy02Polygon->Unlock();                                     //バッファのアンロック

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

void DrawEnemy02(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	pD3DDevice->SetStreamSource(0, g_pVtxBufferEnemy02Polygon, 0, sizeof(VERTEX_2D));              //VRAMからGPUにデータをストリーム
	pD3DDevice->SetFVF(FVF_VERTEX2D);

	for (int nCnt = 0; nCnt < MAX_ENEMY02; nCnt++)
	{
		//もし敵は使用可の状態なら
		if (g_Enemy02[nCnt].bUse == true)
		{
			//背景の鉄欄干：頂点フォーマット設定
			pD3DDevice->SetTexture(0, g_pTextureEnemy02Polygon[0]);				 //テクスチャの設定
			pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt*4, NUM_POLYGON);                                            //テクスチャメモリの確保
		}
	}
}

//プレーヤーポリゴンの表示位置X,ポリゴンの表示位置Y,ポリゴンの幅,ポリゴンの高さ
void MakeVertexPolygonEnemy02(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx)
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

//敵情報取得
ENEMY02 *GetEnemy02(void)
{

	return &g_Enemy02[0];
}

//使う敵の数を設定する
void SetEnemy02(float fPosX,float fPosY,float fSpeedX,float fDirect,float nEnemyNum)
{
	for (int nCnt = 0; nCnt < MAX_ENEMY02; nCnt++) 
	{
		if(g_Enemy02[nCnt].bUse == false)
		{
			g_Enemy02[nCnt].nNumEnemy = nEnemyNum;                   //敵の番号を設定
			g_Enemy02[nCnt].fPosX = fPosX;
			g_Enemy02[nCnt].fPosY = fPosY;
			g_Enemy02[nCnt].fMoveX = fSpeedX;
			g_Enemy02[nCnt].nLife = 15;
			g_Enemy02[nCnt].fDirect = fDirect;
			g_Enemy02[nCnt].bUse = true;
			g_Enemy02[nCnt].bPre = false;
			g_Enemy02[nCnt].bInter = false;
			g_Enemy02[nCnt].bRecovery = false;
			g_Enemy02[nCnt].nCntPreAttack = ENEMY02_PREATTACK_TIME;
			g_Enemy02[nCnt].nCntBullet = 3;
			g_Enemy02[nCnt].nCntInterAttack = ENEMY02_INTERATTACK_TIME;
			g_Enemy02[nCnt].nCntRecoveryAttack = ENEMY02_RECOVERYATTACK_TIME;
			break;
		}
	}
}

//弾の着弾点計算式
D3DXVECTOR2 BulletLanding(ENEMY02 enemy)
{
	float fYDistance = 0;               //砲弾とプレーヤーのY軸の距離
	D3DXVECTOR2 E_BulletSpeed;          //敵の弾のスピード
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	fYDistance =  (pPlayer -> fPosY + PLAYER_HEIGHT*0.5) - (enemy.fPosY + BULLETENEMY02_DISTANCEY);                   //プレーヤーと砲弾のY距離差 = プレーヤーY座標 - 砲弾Y座標

	//enemy02はプレーヤーの右側にいる
	if(enemy.fPosX - pPlayer->fPosX_SUM > pPlayer -> fPosX)
	{
		E_BulletSpeed.x = ( (pPlayer -> fPosX + PLAYER_WIDTH*0.5) - (enemy.fPosX - pPlayer -> fPosX_SUM + BULLETENEMY02_DISTANCEX) ) / E_TIMETOTAL;     //砲弾X軸スピード = ( プレーヤーX座標 - 砲弾X座標 ) / 総時間
	}

	//enemy02はプレーヤーの左側にいる
	else
	{
		E_BulletSpeed.x = ( (pPlayer -> fPosX + PLAYER_WIDTH*0.5) - (enemy.fPosX - pPlayer -> fPosX_SUM - BULLETENEMY02_DISTANCEX + ENEMY02_WIDTH) ) / E_TIMETOTAL;     //砲弾X軸スピード = ( プレーヤーX座標 - 砲弾X座標 ) / 総時間
	}

	E_BulletSpeed.y = (fYDistance - 0.5*GRAVITY*E_TIMETOTAL*E_TIMETOTAL) /E_TIMETOTAL;                                                              //砲弾Y軸スピードの計算公式: Y軸距離差 = 砲弾Y軸スピード*総時間 + 0.5*重力*総時間^2           

	return E_BulletSpeed;
}
