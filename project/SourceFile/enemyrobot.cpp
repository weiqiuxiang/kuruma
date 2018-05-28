//-----------------------------------------------------------------------------------------
//�v���[���[,����Ȃǂ̏���[running02.cpp]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
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
//�}�N����`
//-----------------------------------------------------------------------------------------
#define ENEMYROBOT_RECOVERYATTACK_TIME (120)              //�G�̒e�̑��U����

//�e�^�C�v�G
#define EGUN_MOVEMODE_XSPEED (2.0f)                       //���ʈړ��̎��̑��x
#define EGUN_BULLET_SPEED (2.5f);                         //�e�X�s�[�h
#define ENEMYROBOT_PREATTACK_TIME (120)                   //�G�̍U����������

//���^�C�v�G
#define EKNIFE_MOVEMODE_XSPEED (2.0f)                     //���ʈړ������鎞�̈ړ��X�s�[�h
#define MOVE_ACTION_DISTANCE (250)                        //�ړ���Ԕ��苗��
#define RECOVERY_TIME (60)                                //�d������
#define E_JUMP_POWER (6.0f)

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------------------
void EnemyRobotMove01(int nCnt);                               //EnemyRobot�e�^�C�v�̈ړ��֐�
void EnemyRobotMove02(int nCnt);                               //EnemyRobot���^�C�v�̈ړ��֐�
void ATTACK_ACTION(ENEMYROBOT *enemy);                          //EnemyRobot���^�C�v�̍U���v�Z�֐�

//EnemyRobot�̃^�C�v�ʃ|���S���X�V�֐�
void UpdatePolygonEnemyRobotTex01(ENEMYROBOT enemy,VERTEX_2D *pVtx);
void UpdatePolygonEnemyRobotTex02(ENEMYROBOT enemy,VERTEX_2D *pVtx);

//-----------------------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureEnemyRobotPolygon[NUM_ENEMYROBOTTEXTURE] = {};           //�e�N�X�`���𑀍삷�邽�߂̃|�C���^(�e�N�X�`���o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferEnemyRobotPolygon = NULL;                      //���_�o�b�t�@�𑀍삷�邽�߂̃|�C���^(���_�o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)

ENEMYROBOT g_EnemyRobot[NUM_ENEMYROBOTPOLYGON];

//-----------------------------------------------------------------------------------------
//�e�ϐ��̏�����
//-----------------------------------------------------------------------------------------
HRESULT InitEnemyRobot(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_2D *pVtx = NULL;                           //���_�\���̂̃|�C���^
	pD3DDevice = Get_Device();

	//���_�o�b�t�@�̐���
	if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_ENEMYROBOTVERTEX*NUM_ENEMYROBOTPOLYGON, D3DUSAGE_WRITEONLY, FVF_VERTEX2D, D3DPOOL_MANAGED, &g_pVtxBufferEnemyRobotPolygon, NULL)))
	{
		return E_FAIL;
	}

	//EnemyRobot�e�N�X�`���̎擾
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, ENEMYROBOTPOLYGON01_TEXTURENAME, &g_pTextureEnemyRobotPolygon[0])))
	{
		MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂����s���܂���", "�G���[", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	//�v���[���[�̃|���S��
	g_pVtxBufferEnemyRobotPolygon -> Lock(0, 0, (void**)&pVtx, 0);                    //���z�A�h���X���擾���o�b�t�@�����b�N����
	MakeVertexPolygonPlayer(pD3DDevice, pVtx);

	for (int nCnt = 0; nCnt < MAX_ENEMYROBOT; nCnt++)
	{
		//�v���[���[�̏��f�[�^�̏����l
		g_EnemyRobot[nCnt].nNumEnemy = 0;                                        //�G�̔ԍ�
		g_EnemyRobot[nCnt].fPosX = 0.0f;
		g_EnemyRobot[nCnt].fPosY = 0.0f;
		g_EnemyRobot[nCnt].fDirect = -0.1;
		g_EnemyRobot[nCnt].fBullet_MoveX = EGUN_BULLET_SPEED;
		g_EnemyRobot[nCnt].bHit = false;
		g_EnemyRobot[nCnt].bUse = false;
		g_EnemyRobot[nCnt].nCntPreAttack = ENEMYROBOT_PREATTACK_TIME;
		g_EnemyRobot[nCnt].nCntBullet = 1;                                     //���{�b�g�̒e���ꔭ�����Ȃ�

		//���^�C�v�̃p�����[�^
		g_EnemyRobot[nCnt].bRecoveryFlag = false;                            //�d����ԃt���O
		g_EnemyRobot[nCnt].nCntRecovery = 0;                                 //�U����d������
		g_EnemyRobot[nCnt].bAttack = false;                                  //�U���t���O
		g_EnemyRobot[nCnt].bJump = false;                                    //�W�����v�t���O
		g_EnemyRobot[nCnt].bAttackDropFlag = false;                          //�W�����v�̗����������̍U������L���t���O

		pVtx[0+4*nCnt].pos = D3DXVECTOR3(g_EnemyRobot[nCnt].fPosX, g_EnemyRobot[nCnt].fPosY, 0.0f);
		pVtx[1+4*nCnt].pos = D3DXVECTOR3(g_EnemyRobot[nCnt].fPosX + ENEMYROBOT_WIDTH, g_EnemyRobot[nCnt].fPosY, 0.0f);
		pVtx[2+4*nCnt].pos = D3DXVECTOR3(g_EnemyRobot[nCnt].fPosX, g_EnemyRobot[nCnt].fPosY + ENEMYROBOT_HEIGHT, 0.0f);
		pVtx[3+4*nCnt].pos = D3DXVECTOR3(g_EnemyRobot[nCnt].fPosX + ENEMYROBOT_WIDTH, g_EnemyRobot[nCnt].fPosY + ENEMYROBOT_HEIGHT, 0.0f);

	}

	g_pVtxBufferEnemyRobotPolygon -> Unlock();                                     //�o�b�t�@�̃A�����b�N

	return S_OK;
}

//-----------------------------------------------------------------------------------------
//�I������
//-----------------------------------------------------------------------------------------
void UninitEnemyRobot(void)
{
	// �e�N�X�`���C���^�[�t�F�C�X�̕Еt��
	for (int nCnt = 0; nCnt < NUM_ENEMYROBOTTEXTURE; nCnt++)
	{
		//NULL�ł͂Ȃ���������
		if(g_pTextureEnemyRobotPolygon[nCnt] != NULL)
		{
			g_pTextureEnemyRobotPolygon[nCnt] -> Release(); 
			g_pTextureEnemyRobotPolygon[nCnt] = NULL; 
		}
	}

	//���_�o�b�t�@�C���^�[�t�F�C�X�̕Еt��
	if(g_pVtxBufferEnemyRobotPolygon != NULL)
	{
		g_pVtxBufferEnemyRobotPolygon -> Release(); 
		g_pVtxBufferEnemyRobotPolygon = NULL; 
	}
}

//-----------------------------------------------------------------------------------------
//�v���[���[�f�[�^�̍X�V����
//-----------------------------------------------------------------------------------------
void UpdateEnemyRobot(void)
{
	//�X�e�[�W01�̃f�[�^�擾
	STAGE01 *pStage01;
	pStage01 = GetStage01();

	for (int nCnt = 0; nCnt < MAX_ENEMYROBOT; nCnt++)
	{
		//�����G�͎g�p�̏�ԂȂ�
		if (g_EnemyRobot[nCnt].bUse == true)
		{
			//���W�ۑ�
			g_EnemyRobot[nCnt].fPosXOld = g_EnemyRobot[nCnt].fPosX;
			g_EnemyRobot[nCnt].fPosYOld = g_EnemyRobot[nCnt].fPosY;

			//-----------------------------------------------------------------------------------------
			//EnemyRobot�̈ړ�&�U������
			//-----------------------------------------------------------------------------------------
			if(g_EnemyRobot[nCnt].nType == 1) EnemyRobotMove01(nCnt);                         //�G�̈ړ��p�^�[��
			if(g_EnemyRobot[nCnt].nType == 2) EnemyRobotMove02(nCnt);                         //�G�̈ړ��p�^�[��

			//�ړ�
			g_EnemyRobot[nCnt].fPosX += g_EnemyRobot[nCnt].fMoveX;
			g_EnemyRobot[nCnt].fPosY += g_EnemyRobot[nCnt].fMoveY;

			//-----------------------------------------------------------------------------------------
			//EnemyRobot�̓��蔻�菈��
			//-----------------------------------------------------------------------------------------
			HitCheckForBullet(nCnt);                        //�v���[���[�̒e�ɑ΂��铖�蔻��

			//EnemyRobot�̏d�͉e��
			g_EnemyRobot[nCnt].fMoveY += 0.15f;

			if(g_EnemyRobot[nCnt].fPosY + ENEMYROBOT_HEIGHT > 447.0f + ENEMYROBOT_HEIGHT_ADJUST)
			{
				g_EnemyRobot[nCnt].fPosY = 447.0f - ENEMYROBOT_HEIGHT + ENEMYROBOT_HEIGHT_ADJUST;    //���W�߂�
				g_EnemyRobot[nCnt].fMoveY = 0;                               //Y���X�s�[�h0

				//���^�C�v�̃p�����[�^
				if(g_EnemyRobot[nCnt].nType == 2)
				{
					//�W�����v�U���̌�
					if(g_EnemyRobot[nCnt].bJump == true)
					{
						g_EnemyRobot[nCnt].bJump = false;                          //�W�����v�t���O��߂�
						g_EnemyRobot[nCnt].nCntRecovery = RECOVERY_TIME;           //�d�����ԃZ�b�g
						g_EnemyRobot[nCnt].bRecoveryFlag = true;                   //�d�����[�h�ɓ���
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
//�G�̈ړ����@01
//-----------------------------------------------------------------------------------------
void EnemyRobotMove01(int nCnt)
{
	//�v���[���[�̃f�[�^�擾
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	//-----------------------------------
	//���e�_�v�Z
	//-----------------------------------
	//�O�t���[��EnemyRobot�̗̑͂�0�ɂȂ������ǂ���
	if(g_EnemyRobot[nCnt].nLife <= 0)
	{
		g_EnemyRobot[nCnt].bUse = false;         //�G�𖢎g�p�ɂ���
		pPlayer -> fScore += 100;                //�X�R�A���Z
		SetParticalCenter(2,g_EnemyRobot[nCnt].fPosX+ENEMYROBOT_WIDTH*0.5,g_EnemyRobot[nCnt].fPosY+ENEMYROBOT_HEIGHT*0.5,10,5,true);
		PlaySound(SOUND_LABEL_SE_EXPLOSION);
	}

	//-----------------------------------------------------------------------------------------
	//EnemyRobot�̈ړ������ƍU������
	//-----------------------------------------------------------------------------------------
	//�d����Ԃ���Ȃ�
	//EnemyRobot�̓v���[���[�̉E���ɂ���
	if(g_EnemyRobot[nCnt].fPosX - pPlayer->fPosX_SUM > pPlayer -> fPosX)
	{
		//��������
		g_EnemyRobot[nCnt].fDirect = -0.1f;

		//�T�m�͈͊O(�ړ�)
		//�����F�U���͈͂ɓ��邩������ԂȂ�
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

		//�T�m�͈͓�
		else
		{
			float ER_Center = g_EnemyRobot[nCnt].fPosX + ENEMYROBOT_WIDTH/2 - pPlayer->fPosX_SUM;    //�G�̒��S���W

			//�G���X�N���[���̕\���͈͓�
			if( ER_Center <= SCREEN_WIDTH )
			{
				//�������~��,�U�������ɓ���
				g_EnemyRobot[nCnt].bMove = false;
				g_EnemyRobot[nCnt].fMoveX = 0;                            //�ړ��X�s�[�h��0�ɂ���
				g_EnemyRobot[nCnt].nCntPreAttack--;                       //�U���������ԃJ�E���g�_�E��

				//�U����Ԃɓ���
				if(g_EnemyRobot[nCnt].nCntPreAttack <= 0)
				{
					//�U���J�n
					SetBulletEnemy02(3,g_EnemyRobot[nCnt].fPosX + BULLETENEMYROBOT_DISTANCEX,g_EnemyRobot[nCnt].fPosY + BULLETENEMYROBOT_DISTANCEY,-g_EnemyRobot[nCnt].fBullet_MoveX,0,240,0.0f,6);
					g_EnemyRobot[nCnt].nCntBullet--;

					//�e�؂�
					if(g_EnemyRobot[nCnt].nCntBullet <= 0)
					{
						g_EnemyRobot[nCnt].nCntBullet = 1;
						g_EnemyRobot[nCnt].nCntPreAttack = ENEMYROBOT_PREATTACK_TIME;
					}
				}
			}

			//�G���X�N���[���̕\���͈͊O(�ړ�)
			else
			{
				g_EnemyRobot[nCnt].fMoveX = -EGUN_MOVEMODE_XSPEED;
				g_EnemyRobot[nCnt].fDirect = -0.1f;
				g_EnemyRobot[nCnt].bMove = true;
			}
		}
	}

	//EnemyRobot�̓v���[���[�̍����ɂ���
	else
	{
		//��������
		g_EnemyRobot[nCnt].fDirect = 0.1f;

		//�T�m�͈͊O
		if(g_EnemyRobot[nCnt].bAttack == false)
		{
			if( (pPlayer -> fPosX - g_EnemyRobot[nCnt].fPosX + pPlayer->fPosX_SUM) > 250 + rand()%100)
			{
				g_EnemyRobot[nCnt].fMoveX = EGUN_MOVEMODE_XSPEED;
				g_EnemyRobot[nCnt].fDirect = 0.1f;
				g_EnemyRobot[nCnt].bMove = true;                           //�`��p�t���O
			}

			else
			{
				g_EnemyRobot[nCnt].bAttack = true;
			}
		}

		//�T�m�͈͓�
		else
		{
			float ER_Center = g_EnemyRobot[nCnt].fPosX + ENEMYROBOT_WIDTH/2 - pPlayer->fPosX_SUM;    //�G�̒��S���W
			
			//�G���X�N���[���̕\���͈͓�
			if( ER_Center >= 0 )
			{
				//�������~��,�U�������ɓ���
				g_EnemyRobot[nCnt].bMove = false;                         //�`��p�t���O
				g_EnemyRobot[nCnt].fMoveX = 0;                            //�ړ��X�s�[�h��0�ɂ���
				g_EnemyRobot[nCnt].nCntPreAttack--;                       //�U���������ԃJ�E���g�_�E��

				//�U����Ԃɓ���
				if(g_EnemyRobot[nCnt].nCntPreAttack <= 0)
				{
					//�U���J�n
					SetBulletEnemy02(3,g_EnemyRobot[nCnt].fPosX - BULLETENEMYROBOT_DISTANCEX + ENEMYROBOT_WIDTH,g_EnemyRobot[nCnt].fPosY + BULLETENEMYROBOT_DISTANCEY,g_EnemyRobot[nCnt].fBullet_MoveX,0,240,0.0f,6);
					g_EnemyRobot[nCnt].nCntBullet--;

					//�e�؂�
					if(g_EnemyRobot[nCnt].nCntBullet <= 0)
					{
						g_EnemyRobot[nCnt].nCntBullet = 1;
						g_EnemyRobot[nCnt].nCntPreAttack = ENEMYROBOT_PREATTACK_TIME;
					}
				}
			}

			//�G���X�N���[���̕\���͈͊O(�ړ�)
			else
			{
				g_EnemyRobot[nCnt].fMoveX = EGUN_MOVEMODE_XSPEED;
				g_EnemyRobot[nCnt].fDirect = 0.1f;
				g_EnemyRobot[nCnt].bMove = true;                           //�`��p�t���O
			}
		}
	}

	//�v���[���[���U���͈͊O�ɂ��邩
	if( (pPlayer -> fPosX - g_EnemyRobot[nCnt].fPosX + pPlayer->fPosX_SUM < -500) || (pPlayer -> fPosX - g_EnemyRobot[nCnt].fPosX + pPlayer->fPosX_SUM > 500 ) )
	{
		g_EnemyRobot[nCnt].bAttack = false;                     //�ړ���Ԃɖ߂�
	}
}

//-----------------------------------------------------------------------------------------
//�G�̈ړ����@02
//-----------------------------------------------------------------------------------------
void EnemyRobotMove02(int nCnt)
{
	//�v���[���[�̃f�[�^�擾
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	//-----------------------------------
	//���e�_�v�Z
	//-----------------------------------
	//�O�t���[��EnemyRobot�̗̑͂�0�ɂȂ������ǂ���
	if(g_EnemyRobot[nCnt].nLife <= 0)
	{
		g_EnemyRobot[nCnt].bUse = false;         //�G�𖢎g�p�ɂ���
		pPlayer -> fScore += 100;                //�X�R�A���Z
		SetParticalCenter(2,g_EnemyRobot[nCnt].fPosX+ENEMYROBOT_WIDTH*0.5,g_EnemyRobot[nCnt].fPosY+ENEMYROBOT_HEIGHT*0.5,10,5,true);
		PlaySound(SOUND_LABEL_SE_EXPLOSION);
	}

	//-----------------------------------------------------------------------------------------
	//EnemyRobot�̈ړ������ƍU������
	//-----------------------------------------------------------------------------------------
	//�d����Ԃ���Ȃ�
	if(g_EnemyRobot[nCnt].bRecoveryFlag == false)
	{
		//�ړ���Ԋm�F
		if( ( (g_EnemyRobot[nCnt].fPosX - pPlayer->fPosX_SUM - pPlayer -> fPosX) > MOVE_ACTION_DISTANCE ) || ( (g_EnemyRobot[nCnt].fPosX - pPlayer->fPosX_SUM - pPlayer -> fPosX) < -MOVE_ACTION_DISTANCE ))
		{
			g_EnemyRobot[nCnt].bMove = true;
		}

		//�U����Ԃɓ���
		else
		{
			g_EnemyRobot[nCnt].bMove = false;
		}

		//�T�m�͈͊O(�ړ�)
		if( (g_EnemyRobot[nCnt].bMove == true) && (g_EnemyRobot[nCnt].bAttack == false) )
		{
			//enemyrobot�̓v���[���[�̉E���ɂ���
			if((g_EnemyRobot[nCnt].fPosX - pPlayer->fPosX_SUM) > pPlayer -> fPosX)
			{
				g_EnemyRobot[nCnt].fMoveX = -EKNIFE_MOVEMODE_XSPEED;
				g_EnemyRobot[nCnt].fDirect = -0.1f;
			}

			//enemyrobot�̓v���[���[�̍����ɂ���
			else
			{
				g_EnemyRobot[nCnt].fMoveX = EKNIFE_MOVEMODE_XSPEED;
				g_EnemyRobot[nCnt].fDirect = 0.1f;
			}
		}
				

		//�T�m�͈͓�
		else
		{
			float ER_Center = g_EnemyRobot[nCnt].fPosX + ENEMYROBOT_WIDTH/2 - pPlayer->fPosX_SUM;    //�G�̒��S���W

			//�G�̒��S���W�͉�ʕ\���͈͓�
			if( (ER_Center >= 0) && (ER_Center <= SCREEN_WIDTH) )
			{
				//�X�s�[�h�v�Z
				if(g_EnemyRobot[nCnt].bAttack == false)
				{
					ATTACK_ACTION(&g_EnemyRobot[nCnt]);         //�G�̍U�����̈ړ����x�̌v�Z
					g_EnemyRobot[nCnt].bAttack = true;
				}

				else
				{
					if(g_EnemyRobot[nCnt].bJump == true)
					{
						//�������Ă��Ȃ�
						if(g_EnemyRobot[nCnt].fMoveY < 0)
						{
							g_EnemyRobot[nCnt].bAttackDropFlag = false;
						}
						//�������
						else
						{
							g_EnemyRobot[nCnt].bAttackDropFlag = true;
						}
					}
				}
			}

			//�G�̒��S���W�͉�ʕ\���͈͓�
			else
			{
				//enemyrobot�̓v���[���[�̉E���ɂ���
				if((g_EnemyRobot[nCnt].fPosX - pPlayer->fPosX_SUM) > pPlayer -> fPosX)
				{
					g_EnemyRobot[nCnt].fMoveX = -EKNIFE_MOVEMODE_XSPEED;
					g_EnemyRobot[nCnt].fDirect = -0.1f;
					g_EnemyRobot[nCnt].bMove = true;
				}

				//enemyrobot�̓v���[���[�̍����ɂ���
				else
				{
					g_EnemyRobot[nCnt].fMoveX = EKNIFE_MOVEMODE_XSPEED;
					g_EnemyRobot[nCnt].fDirect = 0.1f;
					g_EnemyRobot[nCnt].bMove = true;
				}
			}
		}
	}

	//�d�����
	else
	{
		g_EnemyRobot[nCnt].nCntRecovery--;    //�������Ԃ̃J�E���g�_�E��

		//�d����ԏI��
		if(g_EnemyRobot[nCnt].nCntRecovery <= 0)
		{
			g_EnemyRobot[nCnt].bRecoveryFlag = false;
		}
	}
}

//-----------------------------------------------------------------------------------------
//���^�C�v�̓G�̍U���v�Z
//-----------------------------------------------------------------------------------------
void ATTACK_ACTION(ENEMYROBOT *enemy)
{
	float fTime = 0;                //�G���v���[���[�̈ʒu�ɒ�������
	float fAdjustX = (KNIFEENEMYROBOT_DISTANCEX_L + KNIFEENEMYROBOT_DISTANCEX_R)*0.5;             //X�������̕␳

	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	if(enemy->bJump == false)
	{
		enemy->fMoveY = -E_JUMP_POWER;    //�G�̃W�����v��
		enemy->bJump = true;              //�W�����v�t���O�I��
	}

	fTime = (E_JUMP_POWER / GRAVITY)*2; //�G�̋󒆎���

	//enemy02�̓v���[���[�̉E���ɂ���
	if(enemy->fPosX - pPlayer->fPosX_SUM > pPlayer -> fPosX)
	{
		enemy->fMoveX = ( (pPlayer -> fPosX + PLAYER_WIDTH*0.5) - (enemy->fPosX - pPlayer -> fPosX_SUM + fAdjustX) ) / fTime;     //�C�eX���X�s�[�h = ( �v���[���[X���W - �C�eX���W ) / ������
		enemy->fDirect = -0.1f;
	}

	//enemy02�̓v���[���[�̍����ɂ���
	else
	{
		enemy->fMoveX = ( (pPlayer -> fPosX + PLAYER_WIDTH*0.5) - (enemy->fPosX - pPlayer -> fPosX_SUM - fAdjustX + ENEMYROBOT_WIDTH) ) / fTime;     //�C�eX���X�s�[�h = ( �v���[���[X���W - �C�eX���W ) / ������
		enemy->fDirect = 0.1f;
	}
}

//-----------------------------------------------------------------------------------------
//�v���[���[�̒e�ɑ΂��铖���蔻��
//-----------------------------------------------------------------------------------------
void HitCheckForBullet(int nCnt)
{
	//�v���[���[�̃f�[�^�擾
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	//�v���[���[�̒e�f�[�^�擾
	BULLET *pBullet;
	pBullet = GetBullet();

	float fTop = g_EnemyRobot[nCnt].fPosY + ENEMYROBOT_TOP;
	float fBottom = g_EnemyRobot[nCnt].fPosY + ENEMYROBOT_HEIGHT - ENEMYROBOT_HEIGHT_CHECK;
	float fLeft = g_EnemyRobot[nCnt].fPosX - pPlayer->fPosX_SUM + ENEMYROBOT_HORRIZON;
	float fRight = g_EnemyRobot[nCnt].fPosX - pPlayer->fPosX_SUM + ENEMYROBOT_WIDTH - ENEMYROBOT_HORRIZON;

	BULLET_HITCHECK *pBulletHitcheck = GetBulletHitcheck();                 //�e�̓��蔻��͈͂��Q�b�g

	for(int nCntBullet = 0; nCntBullet <= MAX_BULLET; nCntBullet++)
	{
		//�e���g���Ă��邩
		if( pBullet[nCntBullet].bUse == true)
		{
			//���蔻��
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
				//�p�[�e�B�N��
				SetParticalCenter(2,pBullet[nCntBullet].pos.x+BULLET_WIDTH*0.5,pBullet[nCntBullet].pos.y+BULLET_HEIGHT*0.5,1,4,false);
			}
		}
	}
}

//-----------------------------------------------------------------------------------------
//���{�b�g�`��
//-----------------------------------------------------------------------------------------
void DrawEnemyRobot(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	pD3DDevice->SetStreamSource(0, g_pVtxBufferEnemyRobotPolygon, 0, sizeof(VERTEX_2D));              //VRAM����GPU�Ƀf�[�^���X�g���[��
	pD3DDevice->SetFVF(FVF_VERTEX2D);

	for (int nCnt = 0; nCnt < MAX_ENEMYROBOT; nCnt++)
	{
		//�����G�͎g�p�̏�ԂȂ�
		if (g_EnemyRobot[nCnt].bUse == true)
		{
			//�w�i�̓S�����F���_�t�H�[�}�b�g�ݒ�
			pD3DDevice->SetTexture(0, g_pTextureEnemyRobotPolygon[0]);				 //�e�N�X�`���̐ݒ�
			pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt*4, NUM_POLYGON);                                            //�e�N�X�`���������̊m��
		}
	}
}

//-----------------------------------------------------------------------------------------
//�v���[���[�|���S���̃f�[�^�X�V
//-----------------------------------------------------------------------------------------
void UpdatePolygonEnemyRobot(void)
{
	VERTEX_2D *pVtx = NULL;
	LPDIRECT3DDEVICE9 pD3DDevice;
	PLAYER *pPlayer = GetPlayer();
	pD3DDevice = Get_Device();

	g_pVtxBufferEnemyRobotPolygon -> Lock(0, 0, (void**)&pVtx, 0);          //���z�A�h���X���擾���o�b�t�@�����b�N����
	MakeVertexPolygonPlayer(pD3DDevice, pVtx);

	for (int nCnt = 0; nCnt < MAX_ENEMYROBOT; nCnt++)
	{
		if (g_EnemyRobot[nCnt].bUse == true)
		{
			//�G�̍��W�̌v�Z�F�G�̐�΍��W-�v���[���[�̈ړ��� = �G�̃v���[���[�ɑ��΂�����W (X���̂�)
			pVtx[0+4*nCnt].pos = D3DXVECTOR3(g_EnemyRobot[nCnt].fPosX - pPlayer->fPosX_SUM, g_EnemyRobot[nCnt].fPosY, 0.0f);
			pVtx[1+4*nCnt].pos = D3DXVECTOR3(g_EnemyRobot[nCnt].fPosX + ENEMYROBOT_WIDTH - pPlayer->fPosX_SUM, g_EnemyRobot[nCnt].fPosY, 0.0f);
			pVtx[2+4*nCnt].pos = D3DXVECTOR3(g_EnemyRobot[nCnt].fPosX - pPlayer->fPosX_SUM, g_EnemyRobot[nCnt].fPosY + ENEMYROBOT_HEIGHT, 0.0f);
			pVtx[3+4*nCnt].pos = D3DXVECTOR3(g_EnemyRobot[nCnt].fPosX + ENEMYROBOT_WIDTH - pPlayer->fPosX_SUM, g_EnemyRobot[nCnt].fPosY + ENEMYROBOT_HEIGHT, 0.0f);

			//�G�̃e�N�X�`���ݒ�
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
	g_pVtxBufferEnemyRobotPolygon->Unlock();                                     //�o�b�t�@�̃A�����b�N
}

//-----------------------------------------------------------------------------------------
//�v���[���[�|���S���̕\���ʒuX,�|���S���̕\���ʒuY,�|���S���̕�,�|���S���̍���
//-----------------------------------------------------------------------------------------
void MakeVertexPolygonEnemyRobot(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx)
{
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);   //����1:�ݒ薼�J�����O ����2:�ݒ�l(D3DCULL_CCW:�t���v���𗠖ʂ�����)
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

	//�|���S���`�悷��O�Ƀe�N�X�`�����Z�b�g
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//-----------------------------------------------------------------------------------------
//�G�e�^�C�v�̃e�N�X�`���ݒ�
//-----------------------------------------------------------------------------------------
void UpdatePolygonEnemyRobotTex01(ENEMYROBOT enemy,VERTEX_2D *pVtx)
{
	static int nRunCnt = 0;                                       //�����j���O�}���e�N�X�`���̐؂�ւ��J�E���^�[
	static int nActionFlame = 1;                                  //�����j���O�}���̓�����x�����邽�߂̃J�E���^�[
	//�G���ړ����
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

	//�G���~�܂��Ă���
	else
	{
		if (enemy.fDirect == -0.1f)
		{
			//���[�V����01
			if(enemy.nCntPreAttack > ENEMYROBOT_PREATTACK_TIME * 0.5)
			{
				pVtx[0].tex = D3DXVECTOR2( 0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2( 0.25f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2( 0.0f, 0.5f);
				pVtx[3].tex = D3DXVECTOR2( 0.25f, 0.5f);
			}

			//���[�V����02
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
			//���[�V����01
			if(enemy.nCntPreAttack > ENEMYROBOT_PREATTACK_TIME * 0.5)
			{
				pVtx[0].tex = D3DXVECTOR2( 0.25f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2( 0.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2( 0.25f, 0.5f);
				pVtx[3].tex = D3DXVECTOR2( 0.0f, 0.5f);
			}

			//���[�V����02
			else 
			{
				pVtx[0].tex = D3DXVECTOR2( 0.75f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2( 0.5f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2( 0.75f, 0.5f);
				pVtx[3].tex = D3DXVECTOR2( 0.5f, 0.5f);
			}
		}
	}

	//�e�N�X�`���̐؂�ւ���4�t���[�����Ƃɍs��
	if ((nActionFlame % 8) == 0)
	{
		nRunCnt++;
	}

	//�ϐ�nRunCnt�̃I�[�o�[�t���[�h�~
	if (nRunCnt == 10000)
	{
		nRunCnt = 0;
	}

	//�A�N�V�����t���[���̃C���N�������g
	nActionFlame++;

	//�ϐ�nActionFlame�̃I�[�o�[�t���[�h�~
	if (nActionFlame >= 10000)
	{
		nActionFlame = 0;
	}
}
//-----------------------------------------------------------------------------------------
//�G���^�C�v�̃e�N�X�`���ݒ�
//-----------------------------------------------------------------------------------------
void UpdatePolygonEnemyRobotTex02(ENEMYROBOT enemy,VERTEX_2D *pVtx)
{
	static int nRunCnt = 0;                                       //�����j���O�}���e�N�X�`���̐؂�ւ��J�E���^�[
	static int nActionFlame = 1;                                  //�����j���O�}���̓�����x�����邽�߂̃J�E���^�[

	if(enemy.bRecoveryFlag == false)
	{
		//�G���ړ����
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

		//�G���U�����
		else
		{
			if (enemy.fDirect == -0.1f)
			{
				if(enemy.bJump == true)
				{
					//�㏸
					if(enemy.bAttackDropFlag == false)
					{

						pVtx[0].tex = D3DXVECTOR2( 0.5f, 0.5f);
						pVtx[1].tex = D3DXVECTOR2( 0.75f, 0.5f);
						pVtx[2].tex = D3DXVECTOR2( 0.5f, 1.0f);
						pVtx[3].tex = D3DXVECTOR2( 0.75f, 1.0f);
					}

					//�~��
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
					//�㏸
					if(enemy.bAttackDropFlag == false)
					{

						pVtx[0].tex = D3DXVECTOR2( 0.75f, 0.5f);
						pVtx[1].tex = D3DXVECTOR2( 0.5f, 0.5f);
						pVtx[2].tex = D3DXVECTOR2( 0.75f, 1.0f);
						pVtx[3].tex = D3DXVECTOR2( 0.5f, 1.0f);
					}

					//�~��
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

	//�G�͍d�����
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
//�G���擾
//-----------------------------------------------------------------------------------------
ENEMYROBOT *GetEnemyRobot(void)
{

	return &g_EnemyRobot[0];
}

//-----------------------------------------------------------------------------------------
//�G�Z�b�g
//-----------------------------------------------------------------------------------------
void SetEnemyRobot(int nType,float fPosX,float fPosY,float fMoveX,float fDirect,float nEnemyNum)
{
	for (int nCnt = 0; nCnt < MAX_ENEMYROBOT; nCnt++) 
	{
		if(g_EnemyRobot[nCnt].bUse == false)
		{
			g_EnemyRobot[nCnt].nType = nType;                                     //EnemyRobot�̃^�C�v : 1.�e�^�C�v 2.���^�C�v
			g_EnemyRobot[nCnt].nNumEnemy = nEnemyNum;                               //�G�̔ԍ�
			g_EnemyRobot[nCnt].fPosX = fPosX;                                   //EnemyRobot�̍��W(X���W)
			g_EnemyRobot[nCnt].fPosY = fPosY;                                   //EnemyRobot�̍��W(Y���W)
			g_EnemyRobot[nCnt].fPosXOld = g_EnemyRobot[nCnt].fPosX;                                //EnemyRobot�̌Â����W(X���W)
			g_EnemyRobot[nCnt].fPosYOld = g_EnemyRobot[nCnt].fPosY;                                //EnemyRobot�̌Â����W(X���W)
			g_EnemyRobot[nCnt].fMoveX = fMoveX;                                  //�GX���̈ړ��X�s�[�h
			g_EnemyRobot[nCnt].fMoveY = 0;                                  //�GY���̈ړ��X�s�[�h
			g_EnemyRobot[nCnt].fDirect = fDirect;                                 //�����̃t���O(-0.1f�Ȃ獶,0.1f�Ȃ�E)
			g_EnemyRobot[nCnt].bMove = true;                                    //�G�������Ă��邩
			g_EnemyRobot[nCnt].bHit = false;                                     //�U�����󂯂�t���O
			g_EnemyRobot[nCnt].nLife = 1;                                     //�G�̗̑�
			g_EnemyRobot[nCnt].nCntPreAttack = ENEMYROBOT_PREATTACK_TIME;        //�e���˂܂ł̎���
			g_EnemyRobot[nCnt].nCntBullet = 1;                                //���A�˂ł���e�̐�
			g_EnemyRobot[nCnt].fBullet_MoveX = EGUN_BULLET_SPEED;                           //�e�̈ړ����x

			//���^�C�v�̃p�����[�^
			g_EnemyRobot[nCnt].bRecoveryFlag = false;                            //�d����ԃt���O
			g_EnemyRobot[nCnt].nCntRecovery = RECOVERY_TIME;                     //�U����d������
			g_EnemyRobot[nCnt].bAttack = false;                                  //�U���t���O
			g_EnemyRobot[nCnt].bJump = false;                                    //�W�����v�t���O
			g_EnemyRobot[nCnt].bAttackDropFlag = false;                          //�W�����v�̗����������̍U������L���t���O

			g_EnemyRobot[nCnt].bUse = true;                                     //�g�����ǂ���
			break;
		}
	}
}
