//-----------------------------------------------------------------------------------------
//プレーヤーヘッドファイル[player.h]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------------------
#ifndef _PLAYER_H_
#define _PLAYER_H_

//-----------------------------------------------------------------------------------------
//頂点フォーマット
//-----------------------------------------------------------------------------------------
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define NUM_PLAYERVERTEX (4)                            //頂点数
#define NUM_PLAYERPOLYGON (8)						    //矩形数
#define NUM_PLAYERTEXTURE (3)                           //テクスチャ数
#define NUM_SCOREDIGIT (6)					            //桁数

//プレーヤーの幅高さ
#define PLAYER_WIDTH (128.0f)
#define PLAYER_HEIGHT (64.0f)

//プレーヤーのマシンガンの幅高さ
#define GUN_WIDTH (24.0f)
#define GUN_HEIGHT (24.0f)
#define GUN_DISTANCEX (50.0f)
#define GUN_DISTANCEY (25.0f)

//プレーヤー当り判定範囲の調整
#define PLAYER_TOP (10.0f)
#define PLAYER_HORRIZON (16.0f)
#define PLAYER_HEIGHT_CHECK (16.0f)

//-----------------------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------------------
#define PLAYERPOLYGON01_TEXTURENAME "data/TEXTURE/sozai/Player.png"
#define PLAYERPOLYGON02_TEXTURENAME "data/TEXTURE/sozai/MachineGun.png"
#define PLAYERPOLYGON03_TEXTURENAME "data/TEXTURE/sozai/HP.png"

//-----------------------------------------------------------------------------------------
//プレーヤーの構造体
//-----------------------------------------------------------------------------------------
typedef struct
{
	float fPosX;                                   //プレーヤー現在の位置(X座標)
	float fPosY;                                   //プレーヤー現在の位置(Y座標)
	float fPosXOld;                                //前回の位置(X座標)
	float fPosYOld;                                //前回の位置(Y座標)
	float fDirect;                                 //向きのフラグ(-0.1fなら左,0.1fなら右)
	float fMoveX;                                  //プレーヤーのX軸のスピード
	float fPosY_Move;                              //Y軸方向のスピード
	float fScore;                                  //プレーヤーのスコア
	float fGunAngel;                               //マシンガンの回転角度
	char aScore[20];                               //プレーヤーのスコアの文字列
	bool bJump;                                    //ジャンプのフラグ
	bool bJump_Once;                               //ジャンプボタンの押しっぱなし防止
	bool bRun;                                     //ダッシュのフラグ
	bool bUse;                                     //プレーヤーを使うかどうか
	bool bHit;                                     //プレーヤーがダメージを受けるフラグ
	int nLife;                                     //プレーヤーのライフ
	int nInvinFrame;                               //プレーヤーのダメージ受けた後び無敵フレーム

	//カメラ移動のパラメータ
	float fPosX_SUM;                               //X軸移動総量(カメラX座標)
	float fRecoveryDistance;                       //カメラ回復距離
	bool bCameraRecovery;                          //カメラ回復フラグ

	//被弾のパラメータ
	bool bDamage;                                  //ダメージ受けフラグ
	int nCntInvincible;                            //無敵時間
}PLAYER;

//-----------------------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------------------
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void UpdatePolygonPlayer(void);
void MakeVertexPolygonPlayer(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx);
void SpriteRorate (VERTEX_2D *pVtx);                                              //回転用関数
PLAYER *GetPlayer(void);

#endif
