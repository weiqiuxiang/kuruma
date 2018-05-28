//-----------------------------------------------------------------------------------------
//プレーヤーヘッドファイル[enemy_running02.h]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------------------
#ifndef _ENEMY01_H_
#define _ENEMY01_H_

//-----------------------------------------------------------------------------------------
//頂点フォーマット
//-----------------------------------------------------------------------------------------
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define NUM_ENEMY01VERTEX (4)                            //頂点数
#define NUM_ENEMY01POLYGON (10)						  //矩形数
#define NUM_ENEMY01TEXTURE (1)                           //テクスチャ数

#define ENEMY01_WIDTH (96.0f*1.5)
#define ENEMY01_HEIGHT (64.0f*1.5)

//判定範囲の修正用
#define ENEMY01_TOP (17.0f*1.5)
#define ENEMY01_HORRIZON (10.0f*1.5)
#define ENEMY01_HEIGHT_CHECK (3.0f*1.5)

//弾発射位置の調整
#define BULLETENEMY01_DISTANCEX (50.0f*1.5)
#define BULLETENEMY01_DISTANCEY (46.0f*1.5)

//敵の最大数
#define MAX_ENEMY01 (10)

//-----------------------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------------------
#define Enemy01POLYGON01_TEXTURENAME "data/TEXTURE/sozai/Enemy01.png"

//-----------------------------------------------------------------------------------------
//プレーヤーの構造体
//-----------------------------------------------------------------------------------------
typedef struct
{
	float nNumEnemy;                               //敵の番号
	float fPosX;                                   //Enemy01の座標(X座標)
	float fPosY;                                   //Enemy01の座標(Y座標)
	float fPosXOld;                                //Enemy01の古い座標(X座標)
	float fPosYOld;                                //Enemy01の古い座標(X座標)
	float fPosX_Relative;                          //敵のX軸の相対座標
	float fMoveX;                                  //敵X軸の移動スピード
	float fMoveY;                                  //敵Y軸の移動スピード
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

	//Enemy01が攻撃状態に入る時の移動
	//流れ:プレーヤーとの距離を測る -> 移動 -> 攻撃(回数あり,最大2回) -> 弾装填
	//ループ: ↑-----------------------------------------------------------↓

	//bAttackMeasure通常はflase,攻撃モードに入った時,プレーヤーとの距離を測ってtrueにする
	bool bAttackMeasure;
	//bAttackMove,通常はflase,攻撃モードに入った時trueにする,trueの時,攻撃移動を行う,移動終了後false
	bool bAttackMove;
	//攻撃回数(最大2回),攻撃回数が0になると,bAttackMeasureをfalseにし,ループ
	int nAttackTime; 
}ENEMY01;

//-----------------------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------------------
HRESULT InitEnemy01(void);
void UninitEnemy01(void);
void UpdateEnemy01(void);
void DrawEnemy01(void);
void UpdatePolygonEnemy01(void);
void MakeVertexPolygonEnemy01(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx);
void SetEnemy01(float fPosX,float fPosY,float fSpeedX,float fSpeedY,float fDirect,float nEnemyNum);                          //Enemy01のセット関数
ENEMY01 *GetEnemy01(void);

#endif
