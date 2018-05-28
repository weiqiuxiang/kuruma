//-----------------------------------------------------------------------------------------
//プレーヤーヘッドファイル[enemy_running02.h]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------------------
#ifndef _ENEMY02_H_
#define _ENEMY02_H_

//-----------------------------------------------------------------------------------------
//頂点フォーマット
//-----------------------------------------------------------------------------------------
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define NUM_ENEMY02VERTEX (4)                            //頂点数
#define NUM_ENEMY02POLYGON (10)						  //矩形数
#define NUM_ENEMY02TEXTURE (1)                           //テクスチャ数

#define ENEMY02_WIDTH (192.0f)
#define ENEMY02_HEIGHT (128.0f)

//当り範囲の修正用
#define ENEMY02_TOP (20.0f)
#define ENEMY02_HORRIZON (30.0f)
#define ENEMY02_HEIGHT_CHECK (10.0f)

#define ENEMY02_HEIGHT_ADJUST (22.0f)                  //ポリゴンの表示調整

//弾発射位置の調整
#define BULLETENEMY02_DISTANCEX (128.0f)
#define BULLETENEMY02_DISTANCEY (20.0f)

//敵の最大数
#define MAX_ENEMY02 (10)

//-----------------------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------------------
#define ENEMY02POLYGON01_TEXTURENAME "data/TEXTURE/sozai/Enemy02.png"

//-----------------------------------------------------------------------------------------
//プレーヤーの構造体
//-----------------------------------------------------------------------------------------
typedef struct
{
	float nNumEnemy;                               //敵の番号
	float fPosX;                                   //Enemy02の座標(X座標)
	float fPosY;                                   //Enemy02の座標(Y座標)
	float fPosXOld;                                //Enemy02の古い座標(X座標)
	float fPosYOld;                                //Enemy02の古い座標(X座標)
	float fPosX_Relative;                          //敵のX軸の相対座標
	float fMoveX;                                  //敵の移動スピード
	float fMoveY;                                  //敵の移動スピード
	float fDirect;                                 //向きのフラグ(-0.1fなら左,0.1fなら右)
	bool bUse;                                     //プレーヤーを使うかどうか
	bool bMove;                                    //敵が動いているか
	bool bHit;                                     //攻撃を受けるフラグ
	bool bPre;                                     //準備状態に戻るフラグ(bMoveがtrueの時,bPreがfalse)
	bool bInter;                                   //連射間隔状態か
	bool bRecovery;                                //硬直状態か
	int nLife;                                     //敵の体力
	int nCntPreAttack;                             //弾発射までの時間
	int nCntBullet;                                //一回連射できる弾の数
	int nCntInterAttack;                           //連射間隔
	int nCntRecoveryAttack;                        //攻撃後硬直時間
}ENEMY02;

//-----------------------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------------------
HRESULT InitEnemy02(void);
void UninitEnemy02(void);
void UpdateEnemy02(void);
void DrawEnemy02(void);
void UpdatePolygonEnemy02(void);
void MakeVertexPolygonEnemy02(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx);
void SetEnemy02(float fPosX,float fPosY,float fSpeedX,float fDirect,float nEnemyNum);                          //Enemy02のセット関数
ENEMY02 *GetEnemy02(void);
D3DXVECTOR2 BulletLanding(ENEMY02 enemy);                 //弾の着弾点計算式

#endif
