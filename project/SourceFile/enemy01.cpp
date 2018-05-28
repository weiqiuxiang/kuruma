//-----------------------------------------------------------------------------------------
//プレーヤー,道具などの処理[running02.cpp]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------------------
#include <time.h>
#include "main.h"
#include "enemy01.h"
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
#define ENEMY01_INTERATTACK_TIME (10)                  //敵の攻撃準備時間
#define ENEMY01_RECOVERYATTACK_TIME (120)              //敵の弾の装填時間
#define ENEMY01_PREATTACK_TIME (30)                    //敵の攻撃間隔
#define BULLET_SPEED (3.0f)                            //弾のスピード
#define BULLET_ANGLE (1.1)                             //弾の飛び角度

//Enemy01が活動できる範囲
#define ENEMY01_SKY_ZONE_UP (100.0f)
#define ENEMY01_SKY_ZONE_BUTTOM (200.0f)

//敵の砲弾がプレーヤーに着く時間マクロ
#define E1_TIMETOTAL (120.0f)                //弾がプレーヤーの場所に着くまでの時間
#define ATTACK_TIME_MAX (2)                  //Enemy01が次の攻撃移動を行うまでの攻撃回数

//-----------------------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureEnemy01Polygon[NUM_ENEMY01TEXTURE] = {};           //テクスチャを操作するためのポインタ(テクスチャバッファインターフェイスにアクセスためのポインタ)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferEnemy01Polygon = NULL;                      //頂点バッファを操作するためのポインタ(頂点バッファインターフェイスにアクセスためのポインタ)

ENEMY01 g_Enemy01[NUM_ENEMY01POLYGON];

//-----------------------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------------------
void Enemy01AttackMove(ENEMY01 *enemy);                                         //Enemy01の攻撃移動関数

//各変数の初期化
HRESULT InitEnemy01(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_2D *pVtx = NULL;                           //頂点構造体のポインタ
	pD3DDevice = Get_Device();

	//頂点バッファの生成
	if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_ENEMY01VERTEX*NUM_ENEMY01POLYGON, D3DUSAGE_WRITEONLY, FVF_VERTEX2D, D3DPOOL_MANAGED, &g_pVtxBufferEnemy01Polygon, NULL)))
	{
		return E_FAIL;
	}

	//Enemy01テクスチャの取得
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, Enemy01POLYGON01_TEXTURENAME, &g_pTextureEnemy01Polygon[0])))
	{
		MessageBox(NULL, "テクスチャの読み込みが失敗しました", "エラー", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	//プレーヤーのポリゴン
	g_pVtxBufferEnemy01Polygon -> Lock(0, 0, (void**)&pVtx, 0);                    //仮想アドレスを取得しバッファをロックする
	MakeVertexPolygonPlayer(pD3DDevice, pVtx);
 
	for (int nCnt = 0; nCnt < MAX_ENEMY01; nCnt++)
	{
		//プレーヤーの諸データの初期値
		g_Enemy01[nCnt].nNumEnemy = 0;                                            //敵の番号
		g_Enemy01[nCnt].fPosX = 0.0f;
		g_Enemy01[nCnt].fPosY = 0.0f;
		g_Enemy01[nCnt].fMoveX = 0;
		g_Enemy01[nCnt].fMoveY = 0;
		g_Enemy01[nCnt].fDirect = -0.1;
		g_Enemy01[nCnt].bHit = false;
		g_Enemy01[nCnt].bUse = false;
		g_Enemy01[nCnt].bPre = false;
		g_Enemy01[nCnt].bInter = false;
		g_Enemy01[nCnt].bRecovery = false;
		//攻撃モードのパラメータ
		g_Enemy01[nCnt].bAttackMove = false;                                      //攻撃移動を行うか
		g_Enemy01[nCnt].bAttackMeasure = false;                                   //距離測るフラグ
		g_Enemy01[nCnt].nAttackTime = rand() % ATTACK_TIME_MAX + 1;               //最大攻撃回数
		g_Enemy01[nCnt].nCntPreAttack = ENEMY01_PREATTACK_TIME;
		g_Enemy01[nCnt].nCntBullet = 3;
		g_Enemy01[nCnt].nCntInterAttack = ENEMY01_INTERATTACK_TIME;
		g_Enemy01[nCnt].nCntRecoveryAttack = ENEMY01_RECOVERYATTACK_TIME;

		pVtx[0+4*nCnt].pos = D3DXVECTOR3(g_Enemy01[nCnt].fPosX, g_Enemy01[nCnt].fPosY, 0.0f);
		pVtx[1+4*nCnt].pos = D3DXVECTOR3(g_Enemy01[nCnt].fPosX + ENEMY01_WIDTH, g_Enemy01[nCnt].fPosY, 0.0f);
		pVtx[2+4*nCnt].pos = D3DXVECTOR3(g_Enemy01[nCnt].fPosX, g_Enemy01[nCnt].fPosY + ENEMY01_WIDTH, 0.0f);
		pVtx[3+4*nCnt].pos = D3DXVECTOR3(g_Enemy01[nCnt].fPosX + ENEMY01_WIDTH, g_Enemy01[nCnt].fPosY + ENEMY01_WIDTH, 0.0f);
	}

	g_pVtxBufferEnemy01Polygon -> Unlock();                                     //バッファのアンロック

	return S_OK;
}
//終了処理
void UninitEnemy01(void)
{
	// テクスチャインターフェイスの片付け
	for (int nCnt = 0; nCnt < NUM_ENEMY01TEXTURE; nCnt++)
	{
		SAFE_RELEASE(g_pTextureEnemy01Polygon[nCnt]);
	}

	//頂点バッファインターフェイスの片付け
	for (int nCnt = 0; nCnt < NUM_ENEMY01POLYGON; nCnt++)
	{
		SAFE_RELEASE(g_pVtxBufferEnemy01Polygon);
	}
}

//プレーヤーデータの更新処理
void UpdateEnemy01(void)
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

	for (int nCnt = 0; nCnt < MAX_ENEMY01; nCnt++)
	{
		//もし敵は使用可の状態なら
		if (g_Enemy01[nCnt].bUse == true)
		{
			//前フレームEnemy01の体力が0になったかどうか
			if(g_Enemy01[nCnt].nLife <= 0)
			{
				g_Enemy01[nCnt].bUse = false;          //敵に未使用にする
				pPlayer -> fScore += 100;              //スコア加算
				SetParticalCenter(2,g_Enemy01[nCnt].fPosX+ENEMY01_WIDTH*0.5,g_Enemy01[nCnt].fPosY+ENEMY01_HEIGHT*0.5,10,15,true);
				PlaySound(SOUND_LABEL_SE_EXPLOSION);
			}

			//-----------------------------------------------------------------------------------------
			//Enemy01の移動処理と攻撃処理
			//-----------------------------------------------------------------------------------------
			//硬直状態じゃない
			if(g_Enemy01[nCnt].bRecovery == false)
			{
				//Enemy01はプレーヤーの右側にいる
				if(g_Enemy01[nCnt].fPosX - pPlayer->fPosX_SUM > pPlayer -> fPosX)
				{
					g_Enemy01[nCnt].fDirect = -0.1f;
					//探知範囲外(移動)
					//条件：攻撃範囲に入るかつ準備状態なし
					if( (g_Enemy01[nCnt].fPosX - pPlayer->fPosX_SUM - pPlayer -> fPosX > 200) && (g_Enemy01[nCnt].bPre == false) )
					{
						g_Enemy01[nCnt].fPosX -= g_Enemy01[nCnt].fMoveX;
						g_Enemy01[nCnt].fDirect = -0.1f;
						g_Enemy01[nCnt].bMove = true;
					}

					//探知範囲内
					else
					{
						//動きを止め,攻撃準備に入る
						g_Enemy01[nCnt].bMove = false;
						g_Enemy01[nCnt].bPre = true;

						//攻撃移動を行う
						if(g_Enemy01[nCnt].nAttackTime == 0)
						{
							Enemy01AttackMove(&g_Enemy01[nCnt]);
						}

						//攻撃する
						else
						{
							//攻撃状態に入る
							if(g_Enemy01[nCnt].nCntPreAttack == 0)
							{
								//攻撃開始
								if(g_Enemy01[nCnt].bInter == false)
								{
									SetBulletEnemy02(1,g_Enemy01[nCnt].fPosX + BULLETENEMY01_DISTANCEX,g_Enemy01[nCnt].fPosY + BULLETENEMY01_DISTANCEY,-BULLET_SPEED*cos(BULLET_ANGLE),BULLET_SPEED*sin(BULLET_ANGLE),180,0.0f,6);
									g_Enemy01[nCnt].nCntBullet--;

									//また弾あり
									if(g_Enemy01[nCnt].nCntBullet > 0)
									{
										g_Enemy01[nCnt].bInter = true;
									}

									//弾が打ち切ると硬直状態に入る
									else
									{
										g_Enemy01[nCnt].bRecovery = true;
									}

								}
								//連射間隔中
								else
								{
									g_Enemy01[nCnt].nCntInterAttack--;

									//連射間隔終了
									if(g_Enemy01[nCnt].nCntInterAttack == 0)
									{
										g_Enemy01[nCnt].nCntInterAttack = ENEMY01_INTERATTACK_TIME;
										g_Enemy01[nCnt].bInter = false;
									}
								}
							}

							//攻撃準備段階
							else
							{
								g_Enemy01[nCnt].nCntPreAttack--;
							}
						}
					}
				}

				//Enemy01はプレーヤーの左側にいる
				else
				{
					g_Enemy01[nCnt].fDirect = 0.1f;

					//探知範囲外
					if( (pPlayer -> fPosX - g_Enemy01[nCnt].fPosX + pPlayer->fPosX_SUM > 200) && (g_Enemy01[nCnt].bPre == false) )
					{
						g_Enemy01[nCnt].fPosX += g_Enemy01[nCnt].fMoveX;
						g_Enemy01[nCnt].fDirect = 0.1f;
						g_Enemy01[nCnt].bMove = true;
					}

					//探知範囲内
					else
					{
						//動きを止め,攻撃準備に入る
						g_Enemy01[nCnt].bMove = false;
						g_Enemy01[nCnt].bPre = true;

						//攻撃状態に入る
						if(g_Enemy01[nCnt].nCntPreAttack == 0)
						{
							//攻撃移動を行う
							if(g_Enemy01[nCnt].nAttackTime == 0)
							{
								Enemy01AttackMove(&g_Enemy01[nCnt]);
							}

							else
							{
								//攻撃開始
								if(g_Enemy01[nCnt].bInter == false)
								{
									SetBulletEnemy02(1,g_Enemy01[nCnt].fPosX + BULLETENEMY01_DISTANCEX,g_Enemy01[nCnt].fPosY + BULLETENEMY01_DISTANCEY,BULLET_SPEED*cos(BULLET_ANGLE),BULLET_SPEED*sin(BULLET_ANGLE),180,0.0f,6);
									g_Enemy01[nCnt].nCntBullet--;

									//また弾あり
									if(g_Enemy01[nCnt].nCntBullet > 0)
									{
										g_Enemy01[nCnt].bInter = true;
									}

									//弾が打ち切ると硬直状態に入る
									else
									{
										g_Enemy01[nCnt].bRecovery = true;
									}

								}
								//連射間隔中
								else
								{
									g_Enemy01[nCnt].nCntInterAttack--;

									//連射間隔終了
									if(g_Enemy01[nCnt].nCntInterAttack == 0)
									{
										g_Enemy01[nCnt].nCntInterAttack = ENEMY01_INTERATTACK_TIME;
										g_Enemy01[nCnt].bInter = false;
									}
								}
							}
						}

						//攻撃準備段階
						else
						{
							g_Enemy01[nCnt].nCntPreAttack--;
						}
					}
				}
			}


			//硬直状態
			else
			{
				g_Enemy01[nCnt].nCntRecoveryAttack--;

				//硬直状態終了
				if(g_Enemy01[nCnt].nCntRecoveryAttack == 0)
				{
					g_Enemy01[nCnt].nCntRecoveryAttack = ENEMY01_RECOVERYATTACK_TIME;
					g_Enemy01[nCnt].nAttackTime--;
					g_Enemy01[nCnt].bRecovery = false;

					//弾の補充
					g_Enemy01[nCnt].nCntBullet = 3;

					//プレーヤーが攻撃範囲外にいるか
					if( (pPlayer -> fPosX - g_Enemy01[nCnt].fPosX + pPlayer->fPosX_SUM < -400) || (pPlayer -> fPosX - g_Enemy01[nCnt].fPosX + pPlayer->fPosX_SUM > 400 ) )
					{
						g_Enemy01[nCnt].bPre = false;                     //移動状態に戻る
						g_Enemy01[nCnt].nCntPreAttack = ENEMY01_PREATTACK_TIME;
					}
				}

				//攻撃移動モードに戻る
				if(g_Enemy01[nCnt].nAttackTime == 0)
				{
					g_Enemy01[nCnt].bAttackMeasure = false;
					g_Enemy01[nCnt].bAttackMove = false;
				}
			}

			//-----------------------------------------------------------------------------------------
			//Enemy01の当り判定処理
			//-----------------------------------------------------------------------------------------
			float fTop = g_Enemy01[nCnt].fPosY + ENEMY01_TOP;
			float fBottom = g_Enemy01[nCnt].fPosY + ENEMY01_HEIGHT - ENEMY01_HEIGHT_CHECK;
			float fLeft = g_Enemy01[nCnt].fPosX - pPlayer->fPosX_SUM + ENEMY01_HORRIZON;
			float fRight = g_Enemy01[nCnt].fPosX - pPlayer->fPosX_SUM + ENEMY01_WIDTH - ENEMY01_HORRIZON;

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
						g_Enemy01[nCnt].nLife--;
						PlaySound(SOUND_LABEL_SE_HITENEMY);
						g_Enemy01[nCnt].bHit = true;
						pBullet[nCntBullet].bUse = false;
						//パーティクル
						SetParticalCenter(2,pBullet[nCntBullet].pos.x+BULLET_WIDTH*0.5,pBullet[nCntBullet].pos.y+BULLET_HEIGHT*0.5,1,4,false);
					}
				}
			}
		}
	}

	UpdatePolygonEnemy01();
}

//プレーヤーポリゴンのデータ更新
void UpdatePolygonEnemy01(void)
{
	static int nRunCnt = 0;                                       //ランニングマンテクスチャの切り替えカウンター
	static int nActionFlame = 1;                                  //ランニングマンの動きを遅くするためのカウンター
	VERTEX_2D *pVtx = NULL;
	LPDIRECT3DDEVICE9 pD3DDevice;
	PLAYER *pPlayer = GetPlayer();
	pD3DDevice = Get_Device();

	g_pVtxBufferEnemy01Polygon -> Lock(0, 0, (void**)&pVtx, 0);          //仮想アドレスを取得しバッファをロックする
	MakeVertexPolygonPlayer(pD3DDevice, pVtx);

	for (int nCnt = 0; nCnt < MAX_ENEMY01; nCnt++)
	{
		if (g_Enemy01[nCnt].bUse == true)
		{
			//敵の座標の計算：敵の絶対座標-プレーヤーの移動量 = 敵のプレーヤーに相対する座標 (X軸のみ)
			pVtx[0+4*nCnt].pos = D3DXVECTOR3(g_Enemy01[nCnt].fPosX - pPlayer->fPosX_SUM, g_Enemy01[nCnt].fPosY, 0.0f);
			pVtx[1+4*nCnt].pos = D3DXVECTOR3(g_Enemy01[nCnt].fPosX + ENEMY01_WIDTH - pPlayer->fPosX_SUM, g_Enemy01[nCnt].fPosY, 0.0f);
			pVtx[2+4*nCnt].pos = D3DXVECTOR3(g_Enemy01[nCnt].fPosX - pPlayer->fPosX_SUM, g_Enemy01[nCnt].fPosY + ENEMY01_HEIGHT, 0.0f);
			pVtx[3+4*nCnt].pos = D3DXVECTOR3(g_Enemy01[nCnt].fPosX + ENEMY01_WIDTH - pPlayer->fPosX_SUM, g_Enemy01[nCnt].fPosY + ENEMY01_HEIGHT, 0.0f);

			g_Enemy01[nCnt].fPosX_Relative = g_Enemy01[nCnt].fPosX - pPlayer->fPosX_SUM;                    //敵の相対座標を求める

			//敵が左向き
			if (g_Enemy01[nCnt].fDirect == -0.1f)
			{
				pVtx[0+4*nCnt].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.5), 0.0f);
				pVtx[1+4*nCnt].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.5) + 0.5f, 0.0f);
				pVtx[2+4*nCnt].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.5), 1.0f);
				pVtx[3+4*nCnt].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.5) + 0.5f, 1.0f);
			}

			//敵が右向き
			else
			{
				pVtx[0+4*nCnt].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.5) + 0.5f, 0.0f);
				pVtx[1+4*nCnt].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.5), 0.0f);
				pVtx[2+4*nCnt].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.5) + 0.5f, 1.0f);
				pVtx[3+4*nCnt].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.5), 1.0f);
			}
			
			pVtx[0+4*nCnt].rhw = 1.0f;
			pVtx[1+4*nCnt].rhw = 1.0f;
			pVtx[2+4*nCnt].rhw = 1.0f;
			pVtx[3+4*nCnt].rhw = 1.0f;

			if(g_Enemy01[nCnt].bHit == false)
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
				g_Enemy01[nCnt].bHit = false;
			}
		}
	}
	g_pVtxBufferEnemy01Polygon->Unlock();                                     //バッファのアンロック

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

void DrawEnemy01(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	pD3DDevice->SetStreamSource(0, g_pVtxBufferEnemy01Polygon, 0, sizeof(VERTEX_2D));              //VRAMからGPUにデータをストリーム
	pD3DDevice->SetFVF(FVF_VERTEX2D);

	for (int nCnt = 0; nCnt < MAX_ENEMY01; nCnt++)
	{
		//もし敵は使用可の状態なら
		if (g_Enemy01[nCnt].bUse == true)
		{
			//背景の鉄欄干：頂点フォーマット設定
			pD3DDevice->SetTexture(0, g_pTextureEnemy01Polygon[0]);				 //テクスチャの設定
			pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt*4, NUM_POLYGON);                                            //テクスチャメモリの確保
		}
	}
}


//プレーヤーポリゴンの表示位置X,ポリゴンの表示位置Y,ポリゴンの幅,ポリゴンの高さ
void MakeVertexPolygonEnemy01(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx)
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
ENEMY01 *GetEnemy01(void)
{
	return &g_Enemy01[0];
}

//使う敵の数を設定する
void SetEnemy01(float fPosX,float fPosY,float fSpeedX,float fSpeedY,float fDirect,float nEnemyNum)
{
	for (int nCnt = 0; nCnt < MAX_ENEMY01; nCnt++) 
	{
		if(g_Enemy01[nCnt].bUse == false)
		{
			g_Enemy01[nCnt].nNumEnemy = nEnemyNum;                   //敵の番号を設定
			g_Enemy01[nCnt].fPosX = fPosX;
			g_Enemy01[nCnt].fPosY = fPosY;
			g_Enemy01[nCnt].fMoveX = fSpeedX;
			g_Enemy01[nCnt].fMoveY = fSpeedY;
			g_Enemy01[nCnt].nLife = 15;
			g_Enemy01[nCnt].fDirect = fDirect;
			g_Enemy01[nCnt].bUse = true;
			g_Enemy01[nCnt].bPre = false;
			g_Enemy01[nCnt].bInter = false;
			g_Enemy01[nCnt].bRecovery = false;
			g_Enemy01[nCnt].nCntPreAttack = ENEMY01_PREATTACK_TIME;
			g_Enemy01[nCnt].nCntBullet = 3;
			g_Enemy01[nCnt].nCntInterAttack = ENEMY01_INTERATTACK_TIME;
			g_Enemy01[nCnt].nCntRecoveryAttack = ENEMY01_RECOVERYATTACK_TIME;

			//攻撃モードのフラグ設定
			g_Enemy01[nCnt].bAttackMeasure = false;
			g_Enemy01[nCnt].bAttackMove = false;
			g_Enemy01[nCnt].nAttackTime = 0;

			break;
		}
	}
}

//Enemy01の攻撃移動関数
void Enemy01AttackMove(ENEMY01 *enemy)
{
	PLAYER *pPlayer;
	pPlayer = GetPlayer();
	float fXDistance;                      //プレーヤーと敵のX軸距離
	float fYDistance;                      //プレーヤーと敵のY軸距離
	static float fXMovePos;               //敵の必要なX軸の座標
	static float fYMovePos;               //敵の必要なY軸の座標
	static bool bXMove = false;
	static bool bYMove = false;
	float fRandPosY;                       //活動ゾンの移動位置


	//プレーヤーとの距離が測りましたか?
	if( enemy -> bAttackMeasure == false)
	{
		//X軸距離計算
		fXDistance = (pPlayer -> fPosX + PLAYER_WIDTH*0.5) - (enemy -> fPosX + ENEMY01_WIDTH*0.5 - pPlayer -> fPosX_SUM);   //プレーヤーとのX軸距離計算
		fYDistance = (pPlayer -> fPosY + PLAYER_HEIGHT*0.5) - (enemy -> fPosY + ENEMY01_HEIGHT*0.5);                        //プレーヤーとのY軸距離計算
		fRandPosY = rand()%(int)(ENEMY01_SKY_ZONE_BUTTOM- ENEMY01_SKY_ZONE_UP)  + ENEMY01_SKY_ZONE_UP;
		fYMovePos = fRandPosY;

		//Enemy01は右側
		if(fXDistance < 0)
		{
			fXMovePos = (pPlayer -> fPosX + PLAYER_WIDTH*0.5) + ( (pPlayer -> fPosY + PLAYER_HEIGHT*0.5) - fRandPosY) / tan(BULLET_ANGLE);
		}

		else
		{
			fXMovePos = (pPlayer -> fPosX + PLAYER_WIDTH*0.5) - ( (pPlayer -> fPosY + PLAYER_HEIGHT*0.5) - fRandPosY) / tan(BULLET_ANGLE);
		}

		enemy -> bAttackMeasure = true;             //フラグをtrueにし,次の段階を行う
	}

	//距離測り終えて攻撃移動を行う
	if(enemy -> bAttackMeasure == true)
	{
		//移動途中
		if(enemy -> bAttackMove == false)
		{
			//------------------------------------------------
			//X軸の移動
			//------------------------------------------------
			if(bXMove == false)
			{
				if( (enemy -> fPosX + ENEMY01_WIDTH*0.5 - pPlayer -> fPosX_SUM) < fXMovePos)
				{
					enemy -> fPosX += enemy -> fMoveX;
					if( (enemy -> fPosX + ENEMY01_WIDTH*0.5 - pPlayer -> fPosX_SUM) >= fXMovePos)
					{
						bXMove = true;
					}
				}

				else
				{
					enemy -> fPosX -= enemy -> fMoveX;
					if( (enemy -> fPosX + ENEMY01_WIDTH*0.5 - pPlayer -> fPosX_SUM) < fXMovePos)
					{
						bXMove = true;
					}
				}
			}
			//------------------------------------------------
			//Y軸の移動
			//------------------------------------------------
			if(bYMove == false)
			{
				if( (enemy -> fPosY + ENEMY01_HEIGHT*0.5) < fYMovePos)
				{
					enemy -> fPosY += enemy -> fMoveY;
					if( (enemy -> fPosY + ENEMY01_HEIGHT*0.5) >= fYMovePos)
					{
						bYMove = true;
					}
				}

				else
				{
					enemy -> fPosY -= enemy -> fMoveY;
					if( (enemy -> fPosY + ENEMY01_HEIGHT*0.5) < fYMovePos)
					{
						bYMove = true;
					}
				}
			}
		}

		//移動終了
		if( (bXMove == true) && (bYMove == true) )
		{
			bXMove = false;
			bYMove = false;
			enemy -> nAttackTime = rand() % ATTACK_TIME_MAX + 1;               //最大攻撃回数設定
		}
	}
}