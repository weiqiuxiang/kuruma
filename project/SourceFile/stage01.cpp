//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include <Windows.h>
#include "main.h"
#include "stage01.h"
#include "player.h"
#include "enemy01.h"
#include "enemy02.h"
#include "enemyrobot.h"
#include "sound.h"

//-----------------------------------------------------------------------------------------
//マグロ定義
//-----------------------------------------------------------------------------------------
#define STAGE01_WAVE_NUM (2)             //STAGE01のWAVE数

#define WAVE01_POS_SUM (700)             //WAVE01の発生カメラワークの座標
#define WAVE02_POS_SUM (2120)            //WAVE02の発生カメラワークの座標

#define WAVE01_NUM_ZONE (3)             //WAVE01の階層数
#define WAVE02_NUM_ZONE (4)             //WAVE02の階層数

//waveマクロ
#define NUM_SEOP_FLAG (10)

//-----------------------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------------------
//道中敵を設置する関数
void SetEnemyOnPosition01_05(float Pos1,float Pos2,float Pos3,float Pos4,float Pos5);              //引数はカメラの位置の値

//Wave関数
void Wave01Set(void);          //第一波敵のセット
void Wave02Set(void);          //第二波敵のセット
//void Wave03Set(void);          //第三波敵のセット
void WaveZoneClear(void);
bool ZoneRepeatEnemyClear(void);

//-----------------------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------------------
STAGE01 g_Stage01;
bool g_SEOP_Flag[NUM_SEOP_FLAG];                    //道中敵をセット完了フラグ
bool g_Bgm;

////////////////////////////////////////////////
//ポリゴン(頂点)の初期化
////////////////////////////////////////////////
void InitStage01(void)
{
	//--------------------------
	//ステージのパラメータ初期化
	//--------------------------
	g_Stage01.nNumWave = 1;                         //Wave番号
	g_Stage01.bNumWaveClear = false;                //Wave番号クリアフラグ
	g_Stage01.nNumWaveZone = 1;                     //Waveの階層
	g_Stage01.bNumWaveClear = false;                //Waveの階層クリアフラグ

	//他のパラメータ初期化
	g_Stage01.bNumWaveZoneOnceEnemySet = false;
	g_Stage01.bNumWaveZoneRepeatEnemySet = false;
	g_Stage01.nNumWaveZoneRepeatEnemySetTime = 0;

	g_Stage01.bStopCameraMove = false;              //カメラワーク停止フラグ

	//---------------------------
	//道中敵設置初期化
	//---------------------------
	for(int nCnt = 0;nCnt < NUM_SEOP_FLAG;nCnt++)
	{
		g_SEOP_Flag[nCnt] = false;
	}

	g_Bgm = false;

	//設置ポイント初期化
	SetEnemyOnPosition01_05(100,500,900,1300,1700);
}

//ポリゴンの更新
void UpdateStage01(void)
{
	//プレーヤーのデータ取得
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	//ゲーム管理
	GAMEMANAGER *pGameManager = GetManager();

	//bgm再生
	if(g_Bgm == false)
	{
		PlaySound(SOUND_LABEL_BGM_STAGE);
		g_Bgm = true;
	}

	//----------
	//Wave部分
	//----------

	//クリアしていないなら
	if(g_Stage01.bNumWaveClear == false)
	{
		//カメラワークを停止していないなら
		if( g_Stage01.bStopCameraMove == false )
		{
			//wave01に入るpPlayer -> fPosX_SUM(カメラの座標)
			switch(g_Stage01.nNumWave)
			{
				//敵出現する座標を監視する
				case 1:
					if(pPlayer -> fPosX_SUM >= WAVE01_POS_SUM)
					{
						g_Stage01.bStopCameraMove = true;
					}
					break;
				case 2:
					if(pPlayer -> fPosX_SUM >= WAVE02_POS_SUM)
					{
						g_Stage01.bStopCameraMove = true;
					}
					break;
			}
		}

		//敵をセットする
		else
		{
			switch(g_Stage01.nNumWave)
			{
			case 1:
				//敵のセット
				Wave01Set();
				break;
			case 2:
				//敵のセット
				Wave02Set();
				break;
			}
		}
	}

	//もしwaveクリアしたら
	else
	{
		g_Stage01.nNumWave++;                             //次のwaveに移る
		g_Stage01.bNumWaveClear = false;                  //waveクリアフラグをfalseに戻らせる
		g_Stage01.nNumWaveZone = 1;                       //Waveの階層
		g_Stage01.bNumWaveZoneClear = false;              //Waveの階層クリアフラグ

		g_Stage01.bNumWaveZoneOnceEnemySet = false;
		g_Stage01.bNumWaveZoneRepeatEnemySet = false;
		g_Stage01.nNumWaveZoneRepeatEnemySetTime = 0;

		g_Stage01.bStopCameraMove = false;                //カメラが動くようになる
	}

	//----------
	//道中部分
	//----------
	SetEnemyOnPosition01_05(100,500,900,1300,1700);

	//stage01がクリアしたのか
	if(g_Stage01.nNumWave > STAGE01_WAVE_NUM)
	{
		pGameManager -> bGameOver = true;

		if(pGameManager -> bGameOver == true)
		{
			pGameManager -> nGameResult = 2;
			StopSound(SOUND_LABEL_BGM_STAGE);
		}
	}
}

void UninitStage01(void)
{
	
}

//Wave01のセット関数
void Wave01Set(void)
{
	//プレーヤーデータを取得
	PLAYER *pPlayer = GetPlayer();
	float fRelative = pPlayer ->fPosX_SUM;

	//階層まだクリアしていない
	if(g_Stage01.bNumWaveZoneClear == false)
	{
		switch(g_Stage01.nNumWaveZone)
		{
			//Zoneの判定
			//Zone01
		case 1:
			//---------------------
			//一回切りの敵のセット
			//---------------------
			if(g_Stage01.bNumWaveZoneOnceEnemySet == false)
			{
				//敵のセット
				SetEnemy01(fRelative+900,200,3,3,0.1,2);

				//一回切りの敵のセットフラグをtrueにする
				g_Stage01.bNumWaveZoneOnceEnemySet = true;
			}

			//--------------
			//重複敵のセット
			//--------------
			if(g_Stage01.nNumWaveZoneRepeatEnemySetTime < 0)
			{
				//重複敵セット完了していない
				if(g_Stage01.bNumWaveZoneRepeatEnemySet == false)
				{
					//敵のセット
					SetEnemyRobot(1,fRelative+800,300,2.0f,-0.1,99);

					//一回切りの敵のセットフラグをtrueにする
					g_Stage01.bNumWaveZoneRepeatEnemySet = true;
				}

				//セット完了した
				else
				{
					//重複敵が全滅?
					if(ZoneRepeatEnemyClear() == true)
					{
						g_Stage01.nNumWaveZoneRepeatEnemySetTime = 60;    //全滅したら再セット時間をリセット
					}
				}
			}

			else
			{
				g_Stage01.nNumWaveZoneRepeatEnemySetTime--;              //再セット時間カウントダウン

				//再セット時間は0以下
				if(g_Stage01.nNumWaveZoneRepeatEnemySetTime < 0)
				{
					g_Stage01.bNumWaveZoneRepeatEnemySet = false;        //セットフラグをfalseに戻す
				}
			}

			//--------------
			//ZONEクリア判定
			//--------------
			WaveZoneClear();                 //このZONEがクリアしましたか？

			break;

			//Zone02
		case 2:
			//---------------------
			//一回切りの敵のセット
			//---------------------
			if(g_Stage01.bNumWaveZoneOnceEnemySet == false)
			{
				//敵のセット
				SetEnemy02(fRelative+900,300,5.0f,0.1,2);

				//一回切りの敵のセットフラグをtrueにする
				g_Stage01.bNumWaveZoneOnceEnemySet = true;
			}

			//--------------
			//重複敵のセット
			//--------------
			if(g_Stage01.nNumWaveZoneRepeatEnemySetTime < 0)
			{
				//重複敵セット完了していない
				if(g_Stage01.bNumWaveZoneRepeatEnemySet == false)
				{
					//敵のセット
					SetEnemyRobot(2,fRelative-500,300,5.0f,-0.1,99);

					//一回切りの敵のセットフラグをtrueにする
					g_Stage01.bNumWaveZoneRepeatEnemySet = true;
				}

				//セット完了した
				else
				{
					//重複敵が全滅?
					if(ZoneRepeatEnemyClear() == true)
					{
						g_Stage01.nNumWaveZoneRepeatEnemySetTime = 60;    //全滅したら再セット時間をリセット
					}
				}
			}

			else
			{
				g_Stage01.nNumWaveZoneRepeatEnemySetTime--;              //再セット時間カウントダウン

				//再セット時間は0以下
				if(g_Stage01.nNumWaveZoneRepeatEnemySetTime < 0)
				{
					g_Stage01.bNumWaveZoneRepeatEnemySet = false;        //セットフラグをfalseに戻す
				}
			}

			//--------------
			//ZONEクリア判定
			//--------------
			WaveZoneClear();                 //このZONEがクリアしましたか？
			break;

			//Zone03
		case 3:
			//---------------------
			//一回切りの敵のセット
			//---------------------
			if(g_Stage01.bNumWaveZoneOnceEnemySet == false)
			{
				//敵のセット
				SetEnemy01(fRelative-500,300,2.0f,2.0f,0.1,2);

				//一回切りの敵のセットフラグをtrueにする
				g_Stage01.bNumWaveZoneOnceEnemySet = true;
			}

			//--------------
			//重複敵のセット
			//--------------
			if(g_Stage01.nNumWaveZoneRepeatEnemySetTime < 0)
			{
				//重複敵セット完了していない
				if(g_Stage01.bNumWaveZoneRepeatEnemySet == false)
				{
					//敵のセット
					SetEnemyRobot(1,fRelative+900,300,2.0f,-0.1,99);
					SetEnemyRobot(1,fRelative-500,300,2.0f,0.1,99);

					//一回切りの敵のセットフラグをtrueにする
					g_Stage01.bNumWaveZoneRepeatEnemySet = true;
				}

				//セット完了した
				else
				{
					//重複敵が全滅?
					if(ZoneRepeatEnemyClear() == true)
					{
						g_Stage01.nNumWaveZoneRepeatEnemySetTime = 120;    //全滅したら再セット時間をリセット
					}
				}
			}

			else
			{
				g_Stage01.nNumWaveZoneRepeatEnemySetTime--;              //再セット時間カウントダウン

				//再セット時間は0以下
				if(g_Stage01.nNumWaveZoneRepeatEnemySetTime < 0)
				{
					g_Stage01.bNumWaveZoneRepeatEnemySet = false;        //セットフラグをfalseに戻す
				}
			}

			//--------------
			//ZONEクリア判定
			//--------------
			WaveZoneClear();                 //このZONEがクリアしましたか？
			break;
		}
	}

	//階層クリアする
	else
	{
		g_Stage01.nNumWaveZone++;                            //次の階層に進む
		g_Stage01.bNumWaveZoneClear = false;                 //Waveの階層クリアフラグ

		g_Stage01.bNumWaveZoneOnceEnemySet = false;
		g_Stage01.bNumWaveZoneRepeatEnemySet = false;
		g_Stage01.nNumWaveZoneRepeatEnemySetTime = 0;
	}

	//このwaveがクリアしたの
	if(g_Stage01.nNumWaveZone > WAVE01_NUM_ZONE)
	{
		g_Stage01.bNumWaveClear = true;
	}
	
}

//Wave02のセット関数
void Wave02Set(void)
{
	//プレーヤーデータを取得
	PLAYER *pPlayer = GetPlayer();
	float fRelative = pPlayer ->fPosX_SUM;

	//階層まだクリアしていない
	if(g_Stage01.bNumWaveZoneClear == false)
	{
		switch(g_Stage01.nNumWaveZone)
		{
			//Zoneの判定
			//Zone01
		case 1:
			//---------------------
			//一回切りの敵のセット
			//---------------------
			if(g_Stage01.bNumWaveZoneOnceEnemySet == false)
			{
				//敵のセット
				SetEnemy02(fRelative-500,300,5.0f,0.1,2);
				SetEnemy02(fRelative+900,300,5.0f,0.1,2);

				//一回切りの敵のセットフラグをtrueにする
				g_Stage01.bNumWaveZoneOnceEnemySet = true;
			}

			//--------------
			//重複敵のセット
			//--------------
			if(g_Stage01.nNumWaveZoneRepeatEnemySetTime < 0)
			{
				//重複敵セット完了していない
				if(g_Stage01.bNumWaveZoneRepeatEnemySet == false)
				{
					//敵のセット
					SetEnemyRobot(1,fRelative+900,300,2.0f,-0.1,99);

					//一回切りの敵のセットフラグをtrueにする
					g_Stage01.bNumWaveZoneRepeatEnemySet = true;
				}

				//セット完了した
				else
				{
					//重複敵が全滅?
					if(ZoneRepeatEnemyClear() == true)
					{
						g_Stage01.nNumWaveZoneRepeatEnemySetTime = 60;    //全滅したら再セット時間をリセット
					}
				}
			}

			else
			{
				g_Stage01.nNumWaveZoneRepeatEnemySetTime--;              //再セット時間カウントダウン

				//再セット時間は0以下
				if(g_Stage01.nNumWaveZoneRepeatEnemySetTime < 0)
				{
					g_Stage01.bNumWaveZoneRepeatEnemySet = false;        //セットフラグをfalseに戻す
				}
			}

			//--------------
			//ZONEクリア判定
			//--------------
			WaveZoneClear();                 //このZONEがクリアしましたか？

			break;

			//Zone02
		case 2:
			//---------------------
			//一回切りの敵のセット
			//---------------------
			if(g_Stage01.bNumWaveZoneOnceEnemySet == false)
			{
				//敵のセット
				SetEnemy01(fRelative+900,200,3.0f,3.0f,0.1,2);
				SetEnemy01(fRelative-500,200,3.0f,3.0f,0.1,2);

				//一回切りの敵のセットフラグをtrueにする
				g_Stage01.bNumWaveZoneOnceEnemySet = true;
			}

			//--------------
			//重複敵のセット
			//--------------
			if(g_Stage01.nNumWaveZoneRepeatEnemySetTime < 0)
			{
				//重複敵セット完了していない
				if(g_Stage01.bNumWaveZoneRepeatEnemySet == false)
				{
					//敵のセット
					SetEnemyRobot(2,fRelative+900,300,5.0f,-0.1,99);

					//一回切りの敵のセットフラグをtrueにする
					g_Stage01.bNumWaveZoneRepeatEnemySet = true;
				}

				//セット完了した
				else
				{
					//重複敵が全滅?
					if(ZoneRepeatEnemyClear() == true)
					{
						g_Stage01.nNumWaveZoneRepeatEnemySetTime = 60;    //全滅したら再セット時間をリセット
					}
				}
			}

			else
			{
				g_Stage01.nNumWaveZoneRepeatEnemySetTime--;              //再セット時間カウントダウン

				//再セット時間は0以下
				if(g_Stage01.nNumWaveZoneRepeatEnemySetTime < 0)
				{
					g_Stage01.bNumWaveZoneRepeatEnemySet = false;        //セットフラグをfalseに戻す
				}
			}

			//--------------
			//ZONEクリア判定
			//--------------
			WaveZoneClear();                 //このZONEがクリアしましたか？
			break;

			//Zone03
		case 3:
			//---------------------
			//一回切りの敵のセット
			//---------------------
			if(g_Stage01.bNumWaveZoneOnceEnemySet == false)
			{
				//敵のセット
				SetEnemy01(fRelative-500,300,3.0f,3.0f,0.1,2);
				SetEnemy02(fRelative+900,300,5.0f,0.1,2);

				//一回切りの敵のセットフラグをtrueにする
				g_Stage01.bNumWaveZoneOnceEnemySet = true;
			}

			//--------------
			//重複敵のセット
			//--------------
			if(g_Stage01.nNumWaveZoneRepeatEnemySetTime < 0)
			{
				//重複敵セット完了していない
				if(g_Stage01.bNumWaveZoneRepeatEnemySet == false)
				{
					//敵のセット
					SetEnemyRobot(1,fRelative-500,300,2.0f,0.1,99);

					//一回切りの敵のセットフラグをtrueにする
					g_Stage01.bNumWaveZoneRepeatEnemySet = true;
				}

				//セット完了した
				else
				{
					//重複敵が全滅?
					if(ZoneRepeatEnemyClear() == true)
					{
						g_Stage01.nNumWaveZoneRepeatEnemySetTime = 120;    //全滅したら再セット時間をリセット
					}
				}
			}

			else
			{
				g_Stage01.nNumWaveZoneRepeatEnemySetTime--;              //再セット時間カウントダウン

				//再セット時間は0以下
				if(g_Stage01.nNumWaveZoneRepeatEnemySetTime < 0)
				{
					g_Stage01.bNumWaveZoneRepeatEnemySet = false;        //セットフラグをfalseに戻す
				}
			}

			//--------------
			//ZONEクリア判定
			//--------------
			WaveZoneClear();                 //このZONEがクリアしましたか？
			break;

			case 4:
			//---------------------
			//一回切りの敵のセット
			//---------------------
			if(g_Stage01.bNumWaveZoneOnceEnemySet == false)
			{
				//敵のセット
				SetEnemy01(fRelative+900,300,3.0f,3.0f,0.1,2);
				SetEnemy01(fRelative+900,300,3.0f,3.0f,0.1,2);

				//一回切りの敵のセットフラグをtrueにする
				g_Stage01.bNumWaveZoneOnceEnemySet = true;
			}

			//--------------
			//重複敵のセット
			//--------------
			if(g_Stage01.nNumWaveZoneRepeatEnemySetTime < 0)
			{
				//重複敵セット完了していない
				if(g_Stage01.bNumWaveZoneRepeatEnemySet == false)
				{
					//敵のセット
					SetEnemyRobot(2,fRelative-500,300,5.0f,0.1,99);
					SetEnemyRobot(2,fRelative+900,300,5.0f,0.1,99);

					//一回切りの敵のセットフラグをtrueにする
					g_Stage01.bNumWaveZoneRepeatEnemySet = true;
				}

				//セット完了した
				else
				{
					//重複敵が全滅?
					if(ZoneRepeatEnemyClear() == true)
					{
						g_Stage01.nNumWaveZoneRepeatEnemySetTime = 120;    //全滅したら再セット時間をリセット
					}
				}
			}

			else
			{
				g_Stage01.nNumWaveZoneRepeatEnemySetTime--;              //再セット時間カウントダウン

				//再セット時間は0以下
				if(g_Stage01.nNumWaveZoneRepeatEnemySetTime < 0)
				{
					g_Stage01.bNumWaveZoneRepeatEnemySet = false;        //セットフラグをfalseに戻す
				}
			}

			//--------------
			//ZONEクリア判定
			//--------------
			WaveZoneClear();                 //このZONEがクリアしましたか？
			break;
		}
	}

	//階層クリアする
	else
	{
		g_Stage01.nNumWaveZone++;                            //次の階層に進む
		g_Stage01.bNumWaveZoneClear = false;                 //Waveの階層クリアフラグ

		g_Stage01.bNumWaveZoneOnceEnemySet = false;
		g_Stage01.bNumWaveZoneRepeatEnemySet = false;
		g_Stage01.nNumWaveZoneRepeatEnemySetTime = 0;
	}

	//このwaveがクリアしたの
	if(g_Stage01.nNumWaveZone > WAVE01_NUM_ZONE)
	{
		g_Stage01.bNumWaveClear = true;
	}
	
}

//WaveZoneクリア関数
void WaveZoneClear(void)
{
	bool bWaveEnemy01,bWaveEnemy02,bWaveEnemyRobot;
	int nCnt;

	//Enemy01のデータの取得
	ENEMY01 *pEnemy01;
	pEnemy01 = GetEnemy01();

	//Enemy02のデータの取得
	ENEMY02 *pEnemy02;
	pEnemy02 = GetEnemy02();

	//EnemyRobotのデータの取得
	ENEMYROBOT *pEnemyRobot;
	pEnemyRobot = GetEnemyRobot();

	//-----------------------
	//特定番号の敵を倒されたら
	//-----------------------
	//Enemy01
	for(nCnt = 0; nCnt < MAX_ENEMY01;nCnt++)
	{
		//敵の番号の値は0より大きい、3より小さい
		if( ((pEnemy01+nCnt) -> nNumEnemy > 0) && ((pEnemy01+nCnt) -> nNumEnemy < 99) )
		{
			if( (pEnemy01+nCnt) -> bUse == true)
			{
				bWaveEnemy01 = false;
				break;
			}
		}
	}

	if(nCnt == MAX_ENEMY01) bWaveEnemy01 = true;                    //waveの倒す必要な敵を全滅させると,クリアフラグ立つ

	//Enemy02
	for(nCnt = 0; nCnt < MAX_ENEMY02;nCnt++)
	{
		//敵の番号の値は0より大きい、3より小さい
		if( ((pEnemy02+nCnt) -> nNumEnemy > 0) && ((pEnemy02+nCnt) -> nNumEnemy < 99) )
		{
			if( (pEnemy02+nCnt) -> bUse == true)
			{
				bWaveEnemy02 = false;
				break;
			}
		}
	}

	if(nCnt == MAX_ENEMY02) bWaveEnemy02 = true;                    //waveの倒す必要な敵を全滅させると,クリアフラグ立つ

	//EnemyRobot
	for(nCnt = 0; nCnt < MAX_ENEMYROBOT;nCnt++)
	{
		//敵の番号の値は0より大きい、3より小さい
		if( ((pEnemyRobot+nCnt) -> nNumEnemy > 0) && ((pEnemyRobot+nCnt) -> nNumEnemy < 99) )
		{
			if( (pEnemyRobot+nCnt) -> bUse == true)
			{
				bWaveEnemyRobot = false;
				break;
			}
		}
	}

	if(nCnt == MAX_ENEMYROBOT) bWaveEnemyRobot = true;                    //waveの倒す必要な敵を全滅させると,クリアフラグ立つ

	//倒す必要がある敵を全滅するとクリアフラグ立つ
	if( (bWaveEnemy01 == true) &&
		(bWaveEnemy02 == true) &&
		(bWaveEnemyRobot == true)
		)
	{
		g_Stage01.bNumWaveZoneClear = true;
	}
}

//重複セット敵のクリア判定関数
bool ZoneRepeatEnemyClear(void)
{
	bool bWaveEnemy01,bWaveEnemy02,bWaveEnemyRobot;
	int nCnt;

	//Enemy01のデータの取得
	ENEMY01 *pEnemy01;
	pEnemy01 = GetEnemy01();

	//Enemy02のデータの取得
	ENEMY02 *pEnemy02;
	pEnemy02 = GetEnemy02();

	//EnemyRobotのデータの取得
	ENEMYROBOT *pEnemyRobot;
	pEnemyRobot = GetEnemyRobot();

	//-----------------------
	//特定番号の敵を倒されたら
	//-----------------------
	//Enemy01
	for(nCnt = 0; nCnt < MAX_ENEMY01;nCnt++)
	{
		//敵の番号の値は0より大きい、3より小さい
		if( (pEnemy01+nCnt) -> nNumEnemy == 99 )
		{
			if( (pEnemy01+nCnt) -> bUse == true)
			{
				bWaveEnemy01 = false;
				break;
			}
		}
	}

	if(nCnt == MAX_ENEMY01) bWaveEnemy01 = true;                    //waveの倒す必要な敵を全滅させると,クリアフラグ立つ

	//Enemy02
	for(nCnt = 0; nCnt < MAX_ENEMY02;nCnt++)
	{
		//敵の番号の値は0より大きい、3より小さい
		if( (pEnemy02+nCnt) -> nNumEnemy == 99 )
		{
			if( (pEnemy02+nCnt) -> bUse == true)
			{
				bWaveEnemy02 = false;
				break;
			}
		}
	}

	if(nCnt == MAX_ENEMY02) bWaveEnemy02 = true;                    //waveの倒す必要な敵を全滅させると,クリアフラグ立つ

	//EnemyRobot
	for(nCnt = 0; nCnt < MAX_ENEMYROBOT;nCnt++)
	{
		//敵の番号の値は0より大きい、3より小さい
		if( (pEnemyRobot+nCnt) -> nNumEnemy == 99 )
		{
			if( (pEnemyRobot+nCnt) -> bUse == true)
			{
				bWaveEnemyRobot = false;
				break;
			}
		}
	}

	if(nCnt == MAX_ENEMYROBOT) bWaveEnemyRobot = true;                    //waveの倒す必要な敵を全滅させると,クリアフラグ立つ

	//倒す必要がある敵を全滅するとクリアフラグ立つ
	if( (bWaveEnemy01 == true) &&
		(bWaveEnemy02 == true) &&
		(bWaveEnemyRobot == true)
		)
	{
		return true;
	}

	//全滅していない
	else return false;
}

//道中敵のセット
void SetEnemyOnPosition01_05(float Pos1,float Pos2,float Pos3,float Pos4,float Pos5)
{
	PLAYER *pPlayer = GetPlayer();
	float fRelative = pPlayer ->fPosX_SUM;

	//pos1
	if(g_SEOP_Flag[0] == false)
	{
		if(pPlayer ->fPosX_SUM >= Pos1)
		{
			SetEnemyRobot(1,fRelative+900,300,2.0f,-0.1,99);
			SetEnemyRobot(2,fRelative+900,300,4.0f,-0.1,99);
			SetEnemyRobot(1,fRelative-500,300,2.0f,-0.1,99);
			SetEnemyRobot(2,fRelative-500,300,4.0f,-0.1,99);
			g_SEOP_Flag[0] = true;
		}
	}

	//pos2
	if(g_SEOP_Flag[1] == false)
	{
		if(pPlayer ->fPosX_SUM >= Pos2)
		{
			SetEnemy01(fRelative+900,300,3,3,-0.1,99);
			SetEnemy01(fRelative-500,300,3,3,-0.1,99);
			g_SEOP_Flag[1] = true;
		}
	}

	//pos3
	if(g_SEOP_Flag[2] == false)
	{
		if(pPlayer ->fPosX_SUM >= Pos3)
		{
			SetEnemy02(fRelative+900,400,5,-0.1,99);
			SetEnemy02(fRelative-500,400,5,0.1,99);
			g_SEOP_Flag[2] = true;
		}
	}

	//pos4
	if(g_SEOP_Flag[3] == false)
	{
		if(pPlayer ->fPosX_SUM >= Pos4)
		{
			SetEnemy01(fRelative+900,300,3,3,-0.1,99);
			SetEnemy01(fRelative-500,300,3,3,-0.1,99);
			SetEnemy02(fRelative-500,400,5,0.1,99);
			g_SEOP_Flag[3] = true;
		}
	}

	//pos5
	if(g_SEOP_Flag[4] == false)
	{
		if(pPlayer ->fPosX_SUM >= Pos5)
		{
			SetEnemy02(fRelative+900,400,3,-0.1,99);
			SetEnemy02(fRelative-500,400,3,0.1,99);
			SetEnemy01(fRelative+900,300,5,3,-0.1,99);
			g_SEOP_Flag[4] = true;
		}
	}
}


//ステージのデータを取得する関数
STAGE01 *GetStage01(void)
{
	return &g_Stage01;
}