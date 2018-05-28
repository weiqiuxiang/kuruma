//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
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
//�}�O����`
//-----------------------------------------------------------------------------------------
#define STAGE01_WAVE_NUM (2)             //STAGE01��WAVE��

#define WAVE01_POS_SUM (700)             //WAVE01�̔����J�������[�N�̍��W
#define WAVE02_POS_SUM (2120)            //WAVE02�̔����J�������[�N�̍��W

#define WAVE01_NUM_ZONE (3)             //WAVE01�̊K�w��
#define WAVE02_NUM_ZONE (4)             //WAVE02�̊K�w��

//wave�}�N��
#define NUM_SEOP_FLAG (10)

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------------------
//�����G��ݒu����֐�
void SetEnemyOnPosition01_05(float Pos1,float Pos2,float Pos3,float Pos4,float Pos5);              //�����̓J�����̈ʒu�̒l

//Wave�֐�
void Wave01Set(void);          //���g�G�̃Z�b�g
void Wave02Set(void);          //���g�G�̃Z�b�g
//void Wave03Set(void);          //��O�g�G�̃Z�b�g
void WaveZoneClear(void);
bool ZoneRepeatEnemyClear(void);

//-----------------------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------------------
STAGE01 g_Stage01;
bool g_SEOP_Flag[NUM_SEOP_FLAG];                    //�����G���Z�b�g�����t���O
bool g_Bgm;

////////////////////////////////////////////////
//�|���S��(���_)�̏�����
////////////////////////////////////////////////
void InitStage01(void)
{
	//--------------------------
	//�X�e�[�W�̃p�����[�^������
	//--------------------------
	g_Stage01.nNumWave = 1;                         //Wave�ԍ�
	g_Stage01.bNumWaveClear = false;                //Wave�ԍ��N���A�t���O
	g_Stage01.nNumWaveZone = 1;                     //Wave�̊K�w
	g_Stage01.bNumWaveClear = false;                //Wave�̊K�w�N���A�t���O

	//���̃p�����[�^������
	g_Stage01.bNumWaveZoneOnceEnemySet = false;
	g_Stage01.bNumWaveZoneRepeatEnemySet = false;
	g_Stage01.nNumWaveZoneRepeatEnemySetTime = 0;

	g_Stage01.bStopCameraMove = false;              //�J�������[�N��~�t���O

	//---------------------------
	//�����G�ݒu������
	//---------------------------
	for(int nCnt = 0;nCnt < NUM_SEOP_FLAG;nCnt++)
	{
		g_SEOP_Flag[nCnt] = false;
	}

	g_Bgm = false;

	//�ݒu�|�C���g������
	SetEnemyOnPosition01_05(100,500,900,1300,1700);
}

//�|���S���̍X�V
void UpdateStage01(void)
{
	//�v���[���[�̃f�[�^�擾
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	//�Q�[���Ǘ�
	GAMEMANAGER *pGameManager = GetManager();

	//bgm�Đ�
	if(g_Bgm == false)
	{
		PlaySound(SOUND_LABEL_BGM_STAGE);
		g_Bgm = true;
	}

	//----------
	//Wave����
	//----------

	//�N���A���Ă��Ȃ��Ȃ�
	if(g_Stage01.bNumWaveClear == false)
	{
		//�J�������[�N���~���Ă��Ȃ��Ȃ�
		if( g_Stage01.bStopCameraMove == false )
		{
			//wave01�ɓ���pPlayer -> fPosX_SUM(�J�����̍��W)
			switch(g_Stage01.nNumWave)
			{
				//�G�o��������W���Ď�����
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

		//�G���Z�b�g����
		else
		{
			switch(g_Stage01.nNumWave)
			{
			case 1:
				//�G�̃Z�b�g
				Wave01Set();
				break;
			case 2:
				//�G�̃Z�b�g
				Wave02Set();
				break;
			}
		}
	}

	//����wave�N���A������
	else
	{
		g_Stage01.nNumWave++;                             //����wave�Ɉڂ�
		g_Stage01.bNumWaveClear = false;                  //wave�N���A�t���O��false�ɖ߂点��
		g_Stage01.nNumWaveZone = 1;                       //Wave�̊K�w
		g_Stage01.bNumWaveZoneClear = false;              //Wave�̊K�w�N���A�t���O

		g_Stage01.bNumWaveZoneOnceEnemySet = false;
		g_Stage01.bNumWaveZoneRepeatEnemySet = false;
		g_Stage01.nNumWaveZoneRepeatEnemySetTime = 0;

		g_Stage01.bStopCameraMove = false;                //�J�����������悤�ɂȂ�
	}

	//----------
	//��������
	//----------
	SetEnemyOnPosition01_05(100,500,900,1300,1700);

	//stage01���N���A�����̂�
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

//Wave01�̃Z�b�g�֐�
void Wave01Set(void)
{
	//�v���[���[�f�[�^���擾
	PLAYER *pPlayer = GetPlayer();
	float fRelative = pPlayer ->fPosX_SUM;

	//�K�w�܂��N���A���Ă��Ȃ�
	if(g_Stage01.bNumWaveZoneClear == false)
	{
		switch(g_Stage01.nNumWaveZone)
		{
			//Zone�̔���
			//Zone01
		case 1:
			//---------------------
			//���؂�̓G�̃Z�b�g
			//---------------------
			if(g_Stage01.bNumWaveZoneOnceEnemySet == false)
			{
				//�G�̃Z�b�g
				SetEnemy01(fRelative+900,200,3,3,0.1,2);

				//���؂�̓G�̃Z�b�g�t���O��true�ɂ���
				g_Stage01.bNumWaveZoneOnceEnemySet = true;
			}

			//--------------
			//�d���G�̃Z�b�g
			//--------------
			if(g_Stage01.nNumWaveZoneRepeatEnemySetTime < 0)
			{
				//�d���G�Z�b�g�������Ă��Ȃ�
				if(g_Stage01.bNumWaveZoneRepeatEnemySet == false)
				{
					//�G�̃Z�b�g
					SetEnemyRobot(1,fRelative+800,300,2.0f,-0.1,99);

					//���؂�̓G�̃Z�b�g�t���O��true�ɂ���
					g_Stage01.bNumWaveZoneRepeatEnemySet = true;
				}

				//�Z�b�g��������
				else
				{
					//�d���G���S��?
					if(ZoneRepeatEnemyClear() == true)
					{
						g_Stage01.nNumWaveZoneRepeatEnemySetTime = 60;    //�S�ł�����ăZ�b�g���Ԃ����Z�b�g
					}
				}
			}

			else
			{
				g_Stage01.nNumWaveZoneRepeatEnemySetTime--;              //�ăZ�b�g���ԃJ�E���g�_�E��

				//�ăZ�b�g���Ԃ�0�ȉ�
				if(g_Stage01.nNumWaveZoneRepeatEnemySetTime < 0)
				{
					g_Stage01.bNumWaveZoneRepeatEnemySet = false;        //�Z�b�g�t���O��false�ɖ߂�
				}
			}

			//--------------
			//ZONE�N���A����
			//--------------
			WaveZoneClear();                 //����ZONE���N���A���܂������H

			break;

			//Zone02
		case 2:
			//---------------------
			//���؂�̓G�̃Z�b�g
			//---------------------
			if(g_Stage01.bNumWaveZoneOnceEnemySet == false)
			{
				//�G�̃Z�b�g
				SetEnemy02(fRelative+900,300,5.0f,0.1,2);

				//���؂�̓G�̃Z�b�g�t���O��true�ɂ���
				g_Stage01.bNumWaveZoneOnceEnemySet = true;
			}

			//--------------
			//�d���G�̃Z�b�g
			//--------------
			if(g_Stage01.nNumWaveZoneRepeatEnemySetTime < 0)
			{
				//�d���G�Z�b�g�������Ă��Ȃ�
				if(g_Stage01.bNumWaveZoneRepeatEnemySet == false)
				{
					//�G�̃Z�b�g
					SetEnemyRobot(2,fRelative-500,300,5.0f,-0.1,99);

					//���؂�̓G�̃Z�b�g�t���O��true�ɂ���
					g_Stage01.bNumWaveZoneRepeatEnemySet = true;
				}

				//�Z�b�g��������
				else
				{
					//�d���G���S��?
					if(ZoneRepeatEnemyClear() == true)
					{
						g_Stage01.nNumWaveZoneRepeatEnemySetTime = 60;    //�S�ł�����ăZ�b�g���Ԃ����Z�b�g
					}
				}
			}

			else
			{
				g_Stage01.nNumWaveZoneRepeatEnemySetTime--;              //�ăZ�b�g���ԃJ�E���g�_�E��

				//�ăZ�b�g���Ԃ�0�ȉ�
				if(g_Stage01.nNumWaveZoneRepeatEnemySetTime < 0)
				{
					g_Stage01.bNumWaveZoneRepeatEnemySet = false;        //�Z�b�g�t���O��false�ɖ߂�
				}
			}

			//--------------
			//ZONE�N���A����
			//--------------
			WaveZoneClear();                 //����ZONE���N���A���܂������H
			break;

			//Zone03
		case 3:
			//---------------------
			//���؂�̓G�̃Z�b�g
			//---------------------
			if(g_Stage01.bNumWaveZoneOnceEnemySet == false)
			{
				//�G�̃Z�b�g
				SetEnemy01(fRelative-500,300,2.0f,2.0f,0.1,2);

				//���؂�̓G�̃Z�b�g�t���O��true�ɂ���
				g_Stage01.bNumWaveZoneOnceEnemySet = true;
			}

			//--------------
			//�d���G�̃Z�b�g
			//--------------
			if(g_Stage01.nNumWaveZoneRepeatEnemySetTime < 0)
			{
				//�d���G�Z�b�g�������Ă��Ȃ�
				if(g_Stage01.bNumWaveZoneRepeatEnemySet == false)
				{
					//�G�̃Z�b�g
					SetEnemyRobot(1,fRelative+900,300,2.0f,-0.1,99);
					SetEnemyRobot(1,fRelative-500,300,2.0f,0.1,99);

					//���؂�̓G�̃Z�b�g�t���O��true�ɂ���
					g_Stage01.bNumWaveZoneRepeatEnemySet = true;
				}

				//�Z�b�g��������
				else
				{
					//�d���G���S��?
					if(ZoneRepeatEnemyClear() == true)
					{
						g_Stage01.nNumWaveZoneRepeatEnemySetTime = 120;    //�S�ł�����ăZ�b�g���Ԃ����Z�b�g
					}
				}
			}

			else
			{
				g_Stage01.nNumWaveZoneRepeatEnemySetTime--;              //�ăZ�b�g���ԃJ�E���g�_�E��

				//�ăZ�b�g���Ԃ�0�ȉ�
				if(g_Stage01.nNumWaveZoneRepeatEnemySetTime < 0)
				{
					g_Stage01.bNumWaveZoneRepeatEnemySet = false;        //�Z�b�g�t���O��false�ɖ߂�
				}
			}

			//--------------
			//ZONE�N���A����
			//--------------
			WaveZoneClear();                 //����ZONE���N���A���܂������H
			break;
		}
	}

	//�K�w�N���A����
	else
	{
		g_Stage01.nNumWaveZone++;                            //���̊K�w�ɐi��
		g_Stage01.bNumWaveZoneClear = false;                 //Wave�̊K�w�N���A�t���O

		g_Stage01.bNumWaveZoneOnceEnemySet = false;
		g_Stage01.bNumWaveZoneRepeatEnemySet = false;
		g_Stage01.nNumWaveZoneRepeatEnemySetTime = 0;
	}

	//����wave���N���A������
	if(g_Stage01.nNumWaveZone > WAVE01_NUM_ZONE)
	{
		g_Stage01.bNumWaveClear = true;
	}
	
}

//Wave02�̃Z�b�g�֐�
void Wave02Set(void)
{
	//�v���[���[�f�[�^���擾
	PLAYER *pPlayer = GetPlayer();
	float fRelative = pPlayer ->fPosX_SUM;

	//�K�w�܂��N���A���Ă��Ȃ�
	if(g_Stage01.bNumWaveZoneClear == false)
	{
		switch(g_Stage01.nNumWaveZone)
		{
			//Zone�̔���
			//Zone01
		case 1:
			//---------------------
			//���؂�̓G�̃Z�b�g
			//---------------------
			if(g_Stage01.bNumWaveZoneOnceEnemySet == false)
			{
				//�G�̃Z�b�g
				SetEnemy02(fRelative-500,300,5.0f,0.1,2);
				SetEnemy02(fRelative+900,300,5.0f,0.1,2);

				//���؂�̓G�̃Z�b�g�t���O��true�ɂ���
				g_Stage01.bNumWaveZoneOnceEnemySet = true;
			}

			//--------------
			//�d���G�̃Z�b�g
			//--------------
			if(g_Stage01.nNumWaveZoneRepeatEnemySetTime < 0)
			{
				//�d���G�Z�b�g�������Ă��Ȃ�
				if(g_Stage01.bNumWaveZoneRepeatEnemySet == false)
				{
					//�G�̃Z�b�g
					SetEnemyRobot(1,fRelative+900,300,2.0f,-0.1,99);

					//���؂�̓G�̃Z�b�g�t���O��true�ɂ���
					g_Stage01.bNumWaveZoneRepeatEnemySet = true;
				}

				//�Z�b�g��������
				else
				{
					//�d���G���S��?
					if(ZoneRepeatEnemyClear() == true)
					{
						g_Stage01.nNumWaveZoneRepeatEnemySetTime = 60;    //�S�ł�����ăZ�b�g���Ԃ����Z�b�g
					}
				}
			}

			else
			{
				g_Stage01.nNumWaveZoneRepeatEnemySetTime--;              //�ăZ�b�g���ԃJ�E���g�_�E��

				//�ăZ�b�g���Ԃ�0�ȉ�
				if(g_Stage01.nNumWaveZoneRepeatEnemySetTime < 0)
				{
					g_Stage01.bNumWaveZoneRepeatEnemySet = false;        //�Z�b�g�t���O��false�ɖ߂�
				}
			}

			//--------------
			//ZONE�N���A����
			//--------------
			WaveZoneClear();                 //����ZONE���N���A���܂������H

			break;

			//Zone02
		case 2:
			//---------------------
			//���؂�̓G�̃Z�b�g
			//---------------------
			if(g_Stage01.bNumWaveZoneOnceEnemySet == false)
			{
				//�G�̃Z�b�g
				SetEnemy01(fRelative+900,200,3.0f,3.0f,0.1,2);
				SetEnemy01(fRelative-500,200,3.0f,3.0f,0.1,2);

				//���؂�̓G�̃Z�b�g�t���O��true�ɂ���
				g_Stage01.bNumWaveZoneOnceEnemySet = true;
			}

			//--------------
			//�d���G�̃Z�b�g
			//--------------
			if(g_Stage01.nNumWaveZoneRepeatEnemySetTime < 0)
			{
				//�d���G�Z�b�g�������Ă��Ȃ�
				if(g_Stage01.bNumWaveZoneRepeatEnemySet == false)
				{
					//�G�̃Z�b�g
					SetEnemyRobot(2,fRelative+900,300,5.0f,-0.1,99);

					//���؂�̓G�̃Z�b�g�t���O��true�ɂ���
					g_Stage01.bNumWaveZoneRepeatEnemySet = true;
				}

				//�Z�b�g��������
				else
				{
					//�d���G���S��?
					if(ZoneRepeatEnemyClear() == true)
					{
						g_Stage01.nNumWaveZoneRepeatEnemySetTime = 60;    //�S�ł�����ăZ�b�g���Ԃ����Z�b�g
					}
				}
			}

			else
			{
				g_Stage01.nNumWaveZoneRepeatEnemySetTime--;              //�ăZ�b�g���ԃJ�E���g�_�E��

				//�ăZ�b�g���Ԃ�0�ȉ�
				if(g_Stage01.nNumWaveZoneRepeatEnemySetTime < 0)
				{
					g_Stage01.bNumWaveZoneRepeatEnemySet = false;        //�Z�b�g�t���O��false�ɖ߂�
				}
			}

			//--------------
			//ZONE�N���A����
			//--------------
			WaveZoneClear();                 //����ZONE���N���A���܂������H
			break;

			//Zone03
		case 3:
			//---------------------
			//���؂�̓G�̃Z�b�g
			//---------------------
			if(g_Stage01.bNumWaveZoneOnceEnemySet == false)
			{
				//�G�̃Z�b�g
				SetEnemy01(fRelative-500,300,3.0f,3.0f,0.1,2);
				SetEnemy02(fRelative+900,300,5.0f,0.1,2);

				//���؂�̓G�̃Z�b�g�t���O��true�ɂ���
				g_Stage01.bNumWaveZoneOnceEnemySet = true;
			}

			//--------------
			//�d���G�̃Z�b�g
			//--------------
			if(g_Stage01.nNumWaveZoneRepeatEnemySetTime < 0)
			{
				//�d���G�Z�b�g�������Ă��Ȃ�
				if(g_Stage01.bNumWaveZoneRepeatEnemySet == false)
				{
					//�G�̃Z�b�g
					SetEnemyRobot(1,fRelative-500,300,2.0f,0.1,99);

					//���؂�̓G�̃Z�b�g�t���O��true�ɂ���
					g_Stage01.bNumWaveZoneRepeatEnemySet = true;
				}

				//�Z�b�g��������
				else
				{
					//�d���G���S��?
					if(ZoneRepeatEnemyClear() == true)
					{
						g_Stage01.nNumWaveZoneRepeatEnemySetTime = 120;    //�S�ł�����ăZ�b�g���Ԃ����Z�b�g
					}
				}
			}

			else
			{
				g_Stage01.nNumWaveZoneRepeatEnemySetTime--;              //�ăZ�b�g���ԃJ�E���g�_�E��

				//�ăZ�b�g���Ԃ�0�ȉ�
				if(g_Stage01.nNumWaveZoneRepeatEnemySetTime < 0)
				{
					g_Stage01.bNumWaveZoneRepeatEnemySet = false;        //�Z�b�g�t���O��false�ɖ߂�
				}
			}

			//--------------
			//ZONE�N���A����
			//--------------
			WaveZoneClear();                 //����ZONE���N���A���܂������H
			break;

			case 4:
			//---------------------
			//���؂�̓G�̃Z�b�g
			//---------------------
			if(g_Stage01.bNumWaveZoneOnceEnemySet == false)
			{
				//�G�̃Z�b�g
				SetEnemy01(fRelative+900,300,3.0f,3.0f,0.1,2);
				SetEnemy01(fRelative+900,300,3.0f,3.0f,0.1,2);

				//���؂�̓G�̃Z�b�g�t���O��true�ɂ���
				g_Stage01.bNumWaveZoneOnceEnemySet = true;
			}

			//--------------
			//�d���G�̃Z�b�g
			//--------------
			if(g_Stage01.nNumWaveZoneRepeatEnemySetTime < 0)
			{
				//�d���G�Z�b�g�������Ă��Ȃ�
				if(g_Stage01.bNumWaveZoneRepeatEnemySet == false)
				{
					//�G�̃Z�b�g
					SetEnemyRobot(2,fRelative-500,300,5.0f,0.1,99);
					SetEnemyRobot(2,fRelative+900,300,5.0f,0.1,99);

					//���؂�̓G�̃Z�b�g�t���O��true�ɂ���
					g_Stage01.bNumWaveZoneRepeatEnemySet = true;
				}

				//�Z�b�g��������
				else
				{
					//�d���G���S��?
					if(ZoneRepeatEnemyClear() == true)
					{
						g_Stage01.nNumWaveZoneRepeatEnemySetTime = 120;    //�S�ł�����ăZ�b�g���Ԃ����Z�b�g
					}
				}
			}

			else
			{
				g_Stage01.nNumWaveZoneRepeatEnemySetTime--;              //�ăZ�b�g���ԃJ�E���g�_�E��

				//�ăZ�b�g���Ԃ�0�ȉ�
				if(g_Stage01.nNumWaveZoneRepeatEnemySetTime < 0)
				{
					g_Stage01.bNumWaveZoneRepeatEnemySet = false;        //�Z�b�g�t���O��false�ɖ߂�
				}
			}

			//--------------
			//ZONE�N���A����
			//--------------
			WaveZoneClear();                 //����ZONE���N���A���܂������H
			break;
		}
	}

	//�K�w�N���A����
	else
	{
		g_Stage01.nNumWaveZone++;                            //���̊K�w�ɐi��
		g_Stage01.bNumWaveZoneClear = false;                 //Wave�̊K�w�N���A�t���O

		g_Stage01.bNumWaveZoneOnceEnemySet = false;
		g_Stage01.bNumWaveZoneRepeatEnemySet = false;
		g_Stage01.nNumWaveZoneRepeatEnemySetTime = 0;
	}

	//����wave���N���A������
	if(g_Stage01.nNumWaveZone > WAVE01_NUM_ZONE)
	{
		g_Stage01.bNumWaveClear = true;
	}
	
}

//WaveZone�N���A�֐�
void WaveZoneClear(void)
{
	bool bWaveEnemy01,bWaveEnemy02,bWaveEnemyRobot;
	int nCnt;

	//Enemy01�̃f�[�^�̎擾
	ENEMY01 *pEnemy01;
	pEnemy01 = GetEnemy01();

	//Enemy02�̃f�[�^�̎擾
	ENEMY02 *pEnemy02;
	pEnemy02 = GetEnemy02();

	//EnemyRobot�̃f�[�^�̎擾
	ENEMYROBOT *pEnemyRobot;
	pEnemyRobot = GetEnemyRobot();

	//-----------------------
	//����ԍ��̓G��|���ꂽ��
	//-----------------------
	//Enemy01
	for(nCnt = 0; nCnt < MAX_ENEMY01;nCnt++)
	{
		//�G�̔ԍ��̒l��0���傫���A3��菬����
		if( ((pEnemy01+nCnt) -> nNumEnemy > 0) && ((pEnemy01+nCnt) -> nNumEnemy < 99) )
		{
			if( (pEnemy01+nCnt) -> bUse == true)
			{
				bWaveEnemy01 = false;
				break;
			}
		}
	}

	if(nCnt == MAX_ENEMY01) bWaveEnemy01 = true;                    //wave�̓|���K�v�ȓG��S�ł������,�N���A�t���O����

	//Enemy02
	for(nCnt = 0; nCnt < MAX_ENEMY02;nCnt++)
	{
		//�G�̔ԍ��̒l��0���傫���A3��菬����
		if( ((pEnemy02+nCnt) -> nNumEnemy > 0) && ((pEnemy02+nCnt) -> nNumEnemy < 99) )
		{
			if( (pEnemy02+nCnt) -> bUse == true)
			{
				bWaveEnemy02 = false;
				break;
			}
		}
	}

	if(nCnt == MAX_ENEMY02) bWaveEnemy02 = true;                    //wave�̓|���K�v�ȓG��S�ł������,�N���A�t���O����

	//EnemyRobot
	for(nCnt = 0; nCnt < MAX_ENEMYROBOT;nCnt++)
	{
		//�G�̔ԍ��̒l��0���傫���A3��菬����
		if( ((pEnemyRobot+nCnt) -> nNumEnemy > 0) && ((pEnemyRobot+nCnt) -> nNumEnemy < 99) )
		{
			if( (pEnemyRobot+nCnt) -> bUse == true)
			{
				bWaveEnemyRobot = false;
				break;
			}
		}
	}

	if(nCnt == MAX_ENEMYROBOT) bWaveEnemyRobot = true;                    //wave�̓|���K�v�ȓG��S�ł������,�N���A�t���O����

	//�|���K�v������G��S�ł���ƃN���A�t���O����
	if( (bWaveEnemy01 == true) &&
		(bWaveEnemy02 == true) &&
		(bWaveEnemyRobot == true)
		)
	{
		g_Stage01.bNumWaveZoneClear = true;
	}
}

//�d���Z�b�g�G�̃N���A����֐�
bool ZoneRepeatEnemyClear(void)
{
	bool bWaveEnemy01,bWaveEnemy02,bWaveEnemyRobot;
	int nCnt;

	//Enemy01�̃f�[�^�̎擾
	ENEMY01 *pEnemy01;
	pEnemy01 = GetEnemy01();

	//Enemy02�̃f�[�^�̎擾
	ENEMY02 *pEnemy02;
	pEnemy02 = GetEnemy02();

	//EnemyRobot�̃f�[�^�̎擾
	ENEMYROBOT *pEnemyRobot;
	pEnemyRobot = GetEnemyRobot();

	//-----------------------
	//����ԍ��̓G��|���ꂽ��
	//-----------------------
	//Enemy01
	for(nCnt = 0; nCnt < MAX_ENEMY01;nCnt++)
	{
		//�G�̔ԍ��̒l��0���傫���A3��菬����
		if( (pEnemy01+nCnt) -> nNumEnemy == 99 )
		{
			if( (pEnemy01+nCnt) -> bUse == true)
			{
				bWaveEnemy01 = false;
				break;
			}
		}
	}

	if(nCnt == MAX_ENEMY01) bWaveEnemy01 = true;                    //wave�̓|���K�v�ȓG��S�ł������,�N���A�t���O����

	//Enemy02
	for(nCnt = 0; nCnt < MAX_ENEMY02;nCnt++)
	{
		//�G�̔ԍ��̒l��0���傫���A3��菬����
		if( (pEnemy02+nCnt) -> nNumEnemy == 99 )
		{
			if( (pEnemy02+nCnt) -> bUse == true)
			{
				bWaveEnemy02 = false;
				break;
			}
		}
	}

	if(nCnt == MAX_ENEMY02) bWaveEnemy02 = true;                    //wave�̓|���K�v�ȓG��S�ł������,�N���A�t���O����

	//EnemyRobot
	for(nCnt = 0; nCnt < MAX_ENEMYROBOT;nCnt++)
	{
		//�G�̔ԍ��̒l��0���傫���A3��菬����
		if( (pEnemyRobot+nCnt) -> nNumEnemy == 99 )
		{
			if( (pEnemyRobot+nCnt) -> bUse == true)
			{
				bWaveEnemyRobot = false;
				break;
			}
		}
	}

	if(nCnt == MAX_ENEMYROBOT) bWaveEnemyRobot = true;                    //wave�̓|���K�v�ȓG��S�ł������,�N���A�t���O����

	//�|���K�v������G��S�ł���ƃN���A�t���O����
	if( (bWaveEnemy01 == true) &&
		(bWaveEnemy02 == true) &&
		(bWaveEnemyRobot == true)
		)
	{
		return true;
	}

	//�S�ł��Ă��Ȃ�
	else return false;
}

//�����G�̃Z�b�g
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


//�X�e�[�W�̃f�[�^���擾����֐�
STAGE01 *GetStage01(void)
{
	return &g_Stage01;
}