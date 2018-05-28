//-----------------------------------------------------------------------------------------
//プレーヤーヘッドファイル[enemy_running02.h]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------------------
#ifndef _ENEMYROBOT_H_
#define _ENEMYROBOT_H_

//-----------------------------------------------------------------------------------------
//頂点フォーマット
//-----------------------------------------------------------------------------------------
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define NUM_ENEMYROBOTVERTEX (4)                            //頂点数
#define NUM_ENEMYROBOTPOLYGON (50)						    //矩形数
#define NUM_ENEMYROBOTTEXTURE (1)                           //テクスチャ数

#define ENEMYROBOT_WIDTH (96.0f)
#define ENEMYROBOT_HEIGHT (96.0f)

//当り範囲の修正用
#define ENEMYROBOT_TOP (7.5f)
#define ENEMYROBOT_HORRIZON (15.0f)
#define ENEMYROBOT_HEIGHT_CHECK (2.0f)

//弾発射位置の調整
#define BULLETENEMYROBOT_DISTANCEX (12.0f)
#define BULLETENEMYROBOT_DISTANCEY (48.0f)

//敵のナイフの判定範囲フラグ
#define KNIFEENEMYROBOT_DISTANCEX_L (12.0f)
#define KNIFEENEMYROBOT_DISTANCEX_R (31.5f)
#define KNIFEENEMYROBOT_DISTANCEY_T (70.5f)
#define KNIFEENEMYROBOT_DISTANCEY_B (90.0f)

//地面との当り判定
#define ENEMYROBOT_HEIGHT_ADJUST (12.0f)

//敵の最大数
#define MAX_ENEMYROBOT (50)

//-----------------------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------------------
#define ENEMYROBOTPOLYGON01_TEXTURENAME "data/TEXTURE/sozai/EnemyRobot.png"

//-----------------------------------------------------------------------------------------
//プレーヤーの構造体
//-----------------------------------------------------------------------------------------
typedef struct
{
	int nType;                                     //EnemyRobotのタイプ : 1.銃タイプ 2.刀タイプ
	float nNumEnemy;                               //敵の番号
	float fPosX;                                   //EnemyRobotの座標(X座標)
	float fPosY;                                   //EnemyRobotの座標(Y座標)
	float fPosXOld;                                //EnemyRobotの古い座標(X座標)
	float fPosYOld;                                //EnemyRobotの古い座標(X座標)
	float fMoveX;                                  //敵X軸の移動スピード
	float fMoveY;                                  //敵Y軸の移動スピード
	float fDirect;                                 //向きのフラグ(-0.1fなら左,0.1fなら右)
	bool bUse;                                     //プレーヤーを使うかどうか
	bool bMove;                                    //敵が動いているか
	bool bHit;                                     //攻撃を受けるフラグ
	int nLife;                                     //敵の体力
	int nCntPreAttack;                             //弾発射までの時間
	int nCntBullet;                                //一回連射できる弾の数
	float fBullet_MoveX;                           //弾の移動速度

	//刀タイプのパラメータ
	bool bRecoveryFlag;                            //硬直状態フラグ
	int nCntRecovery;                              //攻撃後硬直時間
	bool bAttack;                                  //攻撃フラグ
	bool bJump;                                    //ジャンプフラグ
	bool bAttackDropFlag;                          //ジャンプの落下した時の攻撃判定有効フラグ
}ENEMYROBOT;

//-----------------------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------------------
HRESULT InitEnemyRobot(void);
void UninitEnemyRobot(void);
void UpdateEnemyRobot(void);
void DrawEnemyRobot(void);
void UpdatePolygonEnemyRobot(void);
void MakeVertexPolygonEnemyRobot(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx);
void SetEnemyRobot(int nType,float fPosX,float fPosY,float fMoveX,float fDirect,float nEnemyNum);    //EnemyRobotのセット関数
ENEMYROBOT *GetEnemyRobot(void);
void HitCheckForBullet(int nCnt);                              //プレーヤーの弾に対する当たり判定確認

#endif
