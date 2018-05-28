//-----------------------------------------------------------------------------------------
//�v���[���[,����Ȃǂ̏���[player.cpp]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------------------
#include "main.h"
#include "player.h"
#include "input.h"
#include "enemyrobot.h"
#include "bullet.h"
#include "bulletEnemy02.h"
#include "fade.h"
#include "sound.h"
#include "stage01.h"
#include "partical.h"
#include "particalEffect.h"
#include "particalCenter.h"
#include <stdio.h>

//-----------------------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------------------
#define BARREL_ROTATE_SPEED (0.02*PI)  //�C�g�̉�]�X�s�[�h

//HP�̕`��|�W�V����
#define HP_POSX (30)
#define HP_POSY (50)
#define HP_INTER (40)
#define HP_WIDTH (32)
#define HP_HEIGHT (32)

//�J�����p�����[�^
#define LEFT_END (0)                       //���[
#define RIGHT_END (2520)                   //�E�[
#define LEFT_MOVE_LINE (350)               //���ړ����E��
#define RIGHT_MOVE_LINE (450)              //���ړ����E��

#define PLAYER_INVINCIBLE (120)            //�v���[���[�_���[�W�󂯂���̖��G����

#define PLAYER_PARTICAL_POSX (20.0f)
#define PLAYER_PARTICAL_POSY_D (20.0f)

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------------------
void HitCheckForEnemy(void);                                 //�G���v���C���[�ɑ΂��铖�蔻��֐�

//-----------------------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTexturePlayerPolygon[NUM_PLAYERTEXTURE] = {};           //�e�N�X�`���𑀍삷�邽�߂̃|�C���^(�e�N�X�`���o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferPlayerPolygon = NULL;    //���_�o�b�t�@�𑀍삷�邽�߂̃|�C���^(���_�o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)

PLAYER g_Player;

//-----------------------------------------------------------------------------------------
//�d�v��Ԋ֐��B
//-----------------------------------------------------------------------------------------

//�e�ϐ��̏�����
HRESULT InitPlayer(void)
{
	//�v���[���[�̏��f�[�^�̏����l
	g_Player.fPosX = 100.0f;
	g_Player.fPosY = 300.0f;
	g_Player.fPosXOld = g_Player.fPosX;
	g_Player.fPosYOld = g_Player.fPosY;
	g_Player.fMoveX = 0.1*30;
	g_Player.fDirect = 0.1;
	g_Player.bJump = true;
	g_Player.bJump_Once = false;
	g_Player.bRun = false;
	g_Player.bUse = true;
	g_Player.bHit = false;
	g_Player.nInvinFrame = 0;
	g_Player.nLife = 6;
	g_Player.fScore = 0;
	sprintf(&g_Player.aScore[0],"%f",g_Player.fScore);
	g_Player.aScore[9] = '\0';
	g_Player.fPosY_Move = 0;
	g_Player.fGunAngel = 2*PI;

	//�J�����p�����[�^������
	g_Player.fPosX_SUM = 0;
	g_Player.bCameraRecovery = false;
	g_Player.fRecoveryDistance = 0;

	//�v���[���[�_���[�W
	g_Player.bDamage = false;
	g_Player.nCntInvincible = 0;

	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_2D *pVtx = NULL;                           //���_�\���̂̃|�C���^
	pD3DDevice = Get_Device();

	//���_�o�b�t�@�̐���
	if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_PLAYERVERTEX*NUM_PLAYERPOLYGON, D3DUSAGE_WRITEONLY, FVF_VERTEX2D, D3DPOOL_MANAGED, &g_pVtxBufferPlayerPolygon, NULL)))
	{
		return E_FAIL;
	}

	//�v���[���[�e�N�X�`���̎擾
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, PLAYERPOLYGON01_TEXTURENAME, &g_pTexturePlayerPolygon[0])))
	{
		MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂����s���܂���", "�G���[", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	//�v���[���[�̃}�V���K���e�N�X�`���̎擾
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, PLAYERPOLYGON02_TEXTURENAME, &g_pTexturePlayerPolygon[1])))
	{
		MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂����s���܂���", "�G���[", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	//�v���[���[��HP�e�N�X�`���̎擾
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, PLAYERPOLYGON03_TEXTURENAME, &g_pTexturePlayerPolygon[2])))
	{
		MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂����s���܂���", "�G���[", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	//�v���[���[�̃|���S��
	g_pVtxBufferPlayerPolygon->Lock(0, 0, (void**)&pVtx, 0);                    //���z�A�h���X���擾���o�b�t�@�����b�N����

	MakeVertexPolygonPlayer(pD3DDevice, pVtx);
	pVtx[0].pos = D3DXVECTOR3(g_Player.fPosX, g_Player.fPosY, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Player.fPosX+PLAYER_WIDTH, g_Player.fPosY, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Player.fPosX, g_Player.fPosY+PLAYER_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Player.fPosX+PLAYER_WIDTH, g_Player.fPosY+PLAYER_HEIGHT, 0.0f);

	pVtx += 4;

	pVtx[0].pos = D3DXVECTOR3(g_Player.fPosX+PLAYER_WIDTH+GUN_DISTANCEX, g_Player.fPosY+GUN_DISTANCEY, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Player.fPosX+GUN_WIDTH+PLAYER_WIDTH+GUN_DISTANCEX, g_Player.fPosY+GUN_DISTANCEY, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Player.fPosX+PLAYER_WIDTH+GUN_DISTANCEX, g_Player.fPosY+GUN_HEIGHT+GUN_DISTANCEY, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Player.fPosX+GUN_WIDTH+PLAYER_WIDTH+GUN_DISTANCEX, g_Player.fPosY+GUN_HEIGHT+GUN_DISTANCEY, 0.0f);

	g_pVtxBufferPlayerPolygon->Unlock();                                     //�o�b�t�@�̃A�����b�N

	return S_OK;
}
//�I������
void UninitPlayer(void)
{
	// �e�N�X�`���C���^�[�t�F�C�X�̕Еt��
	for (int nCnt = 0; nCnt < NUM_PLAYERTEXTURE; nCnt++)
	{
		if(g_pTexturePlayerPolygon[nCnt] != NULL)
		{
			g_pTexturePlayerPolygon[nCnt] -> Release();
			g_pTexturePlayerPolygon[nCnt] = NULL;
		}
	}

	//���_�o�b�t�@�C���^�[�t�F�C�X�̕Еt��
	if(g_pVtxBufferPlayerPolygon != NULL)
	{
		g_pVtxBufferPlayerPolygon -> Release();
		g_pVtxBufferPlayerPolygon = NULL;
	}
}

//�v���[���[�f�[�^�̍X�V����
void UpdatePlayer(void)
{
	DIJOYSTATE *pjs = GetJS(); //test

	//�����v���[���[�͎g�p�̏�ԂȂ�
	if (g_Player.bUse == true)
	{
		//�v���[���[�̒��S���W
		float fPosXCenter = 0;
		float fPosXOldCenter = 0;

		//�v���[���[�̑O�̍��W���L�^
		g_Player.fPosXOld = g_Player.fPosX;
		g_Player.fPosYOld = g_Player.fPosY;

		//-----------------------------------------------------------------------------------------
		//�v���[���[�̈ړ�����
		//-----------------------------------------------------------------------------------------
		float fHantenAngle = 2 * PI * ( (g_Player.fGunAngel / (2*PI) ) - (int)(g_Player.fGunAngel / (2*PI) ) );

		//�����Ȃ�,�����F��
		if (pjs->lX < -500 || GetKeyboardPress(DIK_A))
		{
			//���W�݉�&&�O�i�����ۑ�
			g_Player.fPosX -= g_Player.fMoveX;
			g_Player.fDirect = -0.1f;
			g_Player.bRun = true;                                //�v���[���[�������Ă���

			float speedX = rand()%2+1;
			float speedY = -(rand()%200-100)*0.01;

			if(g_Player.bJump == false)
			{
				//�p�[�e�B�N���𔭎�
				SetPartical(g_Player.fPosX + PLAYER_PARTICAL_POSX,g_Player.fPosY + PLAYER_HEIGHT - PLAYER_PARTICAL_POSY_D,speedX,speedY,3,20,false);
				SetParticalEffect(g_Player.fPosX + PLAYER_PARTICAL_POSX,g_Player.fPosY + PLAYER_HEIGHT - PLAYER_PARTICAL_POSY_D,speedX,speedY,3*2,20,false);
			}

		}

		//�����Ȃ�,�����F��
		if (pjs->lX > 500 || GetKeyboardPress(DIK_D))
		{
			//���W�݉�&&�O�i�����ۑ�
			g_Player.fPosX += g_Player.fMoveX;
			g_Player.fDirect = 0.1f;
			g_Player.bRun = true;                                //�v���[���[�������Ă���

			float speedX = -(rand()%2+1);
			float speedY = -(rand()%200-100)*0.01;

			if(g_Player.bJump == false)
			{
				//�p�[�e�B�N���𔭎�
				SetPartical(g_Player.fPosX + PLAYER_PARTICAL_POSX,g_Player.fPosY + PLAYER_HEIGHT - PLAYER_PARTICAL_POSY_D,speedX,speedY,3,20,false);
				SetParticalEffect(g_Player.fPosX + PLAYER_PARTICAL_POSX,g_Player.fPosY + PLAYER_HEIGHT - PLAYER_PARTICAL_POSY_D,speedX,speedY,3*2,20,false);
			}
		}

		//�C�g�̊p�x�v�Z
		if(GetControllerPress(4) || GetKeyboardPress(DIK_U))
		{
			g_Player.fGunAngel -= BARREL_ROTATE_SPEED;
		}

		if(GetControllerPress(5) || GetKeyboardPress(DIK_I))
		{
			g_Player.fGunAngel += BARREL_ROTATE_SPEED;
		}

		////�����F��
		//if (GetKeyboardPress(DIK_W) )
		//{
		//	if( (fHantenAngle > PI*1.5f) || (fHantenAngle < PI*0.5f) )
		//	{
		//		g_Player.fGunAngel -= BARREL_ROTATE_SPEED;
		//	}

		//	else
		//	{
		//		g_Player.fGunAngel += BARREL_ROTATE_SPEED;
		//	}
		//}

		////�����F��
		//if (GetKeyboardPress(DIK_S))
		//{
		//	if( (fHantenAngle > PI*1.5f) || (fHantenAngle < PI*0.5f) )
		//	{
		//		g_Player.fGunAngel += BARREL_ROTATE_SPEED;
		//	}

		//	else
		//	{
		//		g_Player.fGunAngel -= BARREL_ROTATE_SPEED;
		//	}
		//}

		//�p�x��0�ȏ�m��
		if(g_Player.fGunAngel <= 0)
		{
			g_Player.fGunAngel = 2*PI + g_Player.fGunAngel;
		}
		
		/*�W�����v�������������Ɠ��B���Ԃ̌v�Z:�ő卂��:120.0f ���B����:40�t���[��
		�ő卂��: 120 = v*40 - g/2*40^2 �@
		�ő卂���܂ł̎���(�X�s�[�h���[���ɂȂ�):0 = v - g*40 �A
		���@�ƇA��A������,�W�����v�� v = 6.0f �d�� g = 0.15f */

		//�v���[���[�̃W�����v��
		if(GetControllerTrigger(0) || GetKeyboardTrigger(DIK_K))
		{
			if(g_Player.bJump == false)
			{
				g_Player.fPosY_Move -= 6.0f;
				g_Player.bJump = true;
			}
		}

		//Y���̕����d�͉e������
		g_Player.fPosY_Move += GRAVITY;

		//Y���̈ړ��v�Z
		g_Player.fPosY += g_Player.fPosY_Move;

		//Y�����n�ʂɛƂߍ��܂Ȃ��悤��
		if(g_Player.fPosY > 447.0f - PLAYER_HEIGHT + PLAYER_HEIGHT_CHECK)
		{
			g_Player.fPosY = g_Player.fPosYOld;
			g_Player.fPosY_Move = 0;
			g_Player.bJump = false;
		}

		//-----------------------------------------------------------------------------------------
		//�}�b�vX���X�N���[������
		//-----------------------------------------------------------------------------------------
		//(X��)�v���[���[�̍��W�ړ�
		//���S���W������

		//�X�e�[�W�J�������[�N�̃f�[�^���擾
		STAGE01 *pStage01;
		pStage01 = GetStage01();

		//�v���[���[�̒��S���W�ƒ��S�Â����W
		fPosXCenter = g_Player.fPosX + PLAYER_WIDTH/2;
		fPosXOldCenter = g_Player.fPosXOld + PLAYER_WIDTH / 2;

		//�J�������[�N����~���ĂȂ�
		if(pStage01 -> bStopCameraMove == false)
		{
			//�v���[���[���}�b�v�̍��[����o��
			if( 
				(fPosXCenter >= RIGHT_MOVE_LINE && fPosXOldCenter < RIGHT_MOVE_LINE) &&
				(g_Player.fPosX_SUM == 0)
				)
			{
				g_Player.fPosX_SUM += g_Player.fMoveX;
			}

			//�v���[���[���}�b�v�̉E�[����o��
			if (
				(fPosXCenter <= LEFT_MOVE_LINE && fPosXOldCenter > LEFT_MOVE_LINE) &&
				(g_Player.fPosX_SUM == RIGHT_END)
				)
			{
				g_Player.fPosX_SUM -= g_Player.fMoveX;
			}

			//�J�������[�N�ړ����
			if ((fPosXCenter >= RIGHT_MOVE_LINE) || (fPosXCenter <= LEFT_MOVE_LINE))
			{
				//�J�������W���C�����ĂȂ�
				if(g_Player.bCameraRecovery == false)
				{
					//�J�������[�N�ړ����
					if( (g_Player.fPosX_SUM > 0) && (g_Player.fPosX_SUM < RIGHT_END) )
					{
						g_Player.fPosX_SUM += g_Player.fPosX - g_Player.fPosXOld;
						g_Player.fPosX = g_Player.fPosXOld;
					}

					else
					{
						if (g_Player.fPosX_SUM >= RIGHT_END)
						{
							g_Player.fPosX_SUM = RIGHT_END;
						}

						else
						{
							g_Player.fPosX_SUM = 0;
						}
					}
				}

				//�J�������W���C�����Ă���
				else
				{
					//�J�������ړ���
					if( (g_Player.fPosX_SUM > 0) && (g_Player.fPosX_SUM < RIGHT_END) )
					{
						//----------------
						//�Ԉړ������ʗ݉�
						//----------------
						if(g_Player.bRun == true) 
						{
							if(g_Player.fDirect == -0.1f)
							{
								g_Player.fRecoveryDistance += g_Player.fMoveX;
							}

							else
							{
								g_Player.fRecoveryDistance -= g_Player.fMoveX;
							}
						}

						//------------------
						//�J�������W�C���ړ�
						//------------------
						if(g_Player.fRecoveryDistance < 0)
						{
							float fAdjust = 0;
							g_Player.fRecoveryDistance += g_Player.fMoveX*2;

							//�덷�␳����
							if(g_Player.fRecoveryDistance >= 0)
							{
								fAdjust = g_Player.fRecoveryDistance;          //�덷�l���
								g_Player.fRecoveryDistance = 0;                //�C��������0��
								g_Player.bCameraRecovery = false;              //�C�������t���O��߂�
								g_Player.fPosX -= g_Player.fMoveX*2;         //�ی����|����

							}
							g_Player.fPosX_SUM += (g_Player.fMoveX*2 - fAdjust);
							g_Player.fPosX -= (g_Player.fMoveX*2 - fAdjust);
						}

						else
						{
							float fAdjust = 0;
							g_Player.fRecoveryDistance -= g_Player.fMoveX*2;

							//�덷�␳����
							if(g_Player.fRecoveryDistance <= 0)
							{
								fAdjust = g_Player.fRecoveryDistance;          //�덷�l���
								g_Player.fRecoveryDistance = 0;                //�C��������0��
								g_Player.bCameraRecovery = false;              //�C�������t���O��߂�
								g_Player.fPosX += g_Player.fMoveX*2;         //�ی����|����
							}
							g_Player.fPosX_SUM -= (g_Player.fMoveX*2 + fAdjust);
							g_Player.fPosX += (g_Player.fMoveX*2 + fAdjust);
						}
					}
				}
			}

			else
			{
				//�J�������[�N�ړ����
				if( (g_Player.fPosX_SUM > 0) && (g_Player.fPosX_SUM < RIGHT_END) )
				{
					//�J�������W�񕜂��ĂȂ�
					if(g_Player.bCameraRecovery == false)
					{
						//�v���[���[�̓J�����̉E���ړ����E�����E�ɂ���
						if(fPosXCenter > RIGHT_MOVE_LINE)
						{
							g_Player.fRecoveryDistance = RIGHT_MOVE_LINE - fPosXCenter;
						}
						//�v���[���[�̓J�����̍����ړ����E����荶�ɂ���
						if(fPosXCenter < LEFT_MOVE_LINE)
						{
							g_Player.fRecoveryDistance = LEFT_MOVE_LINE - fPosXCenter;
						}

					}

					//�J�������[�N�C����
					else
					{
						//----------------
						//�Ԉړ������ʗ݉�
						//----------------
						if(g_Player.bRun == true) 
						{
							if(g_Player.fDirect == -0.1f)
							{
								g_Player.fRecoveryDistance += g_Player.fMoveX;
							}

							else
							{
								g_Player.fRecoveryDistance -= g_Player.fMoveX;
							}
						}

						//------------------
						//�J�������W�C���ړ�
						//------------------
						if(g_Player.fRecoveryDistance < 0)
						{
							float fAdjust = 0;
							g_Player.fRecoveryDistance += g_Player.fMoveX*2;

							//�덷�␳����
							if(g_Player.fRecoveryDistance >= 0)
							{
								fAdjust = g_Player.fRecoveryDistance;      //�덷�l���
								g_Player.fRecoveryDistance = 0;            //�C��������0��
								g_Player.bCameraRecovery = false;          //�C�������t���O��߂�
								g_Player.fPosX -= g_Player.fMoveX*2;         //�ی����|����
							}
							g_Player.fPosX_SUM += (g_Player.fMoveX*2 - fAdjust);
							g_Player.fPosX -= (g_Player.fMoveX*2 - fAdjust);
						}

						else
						{
							float fAdjust = 0;
							g_Player.fRecoveryDistance -= g_Player.fMoveX*2;

							//�덷�␳����
							if(g_Player.fRecoveryDistance <= 0)
							{
								fAdjust = g_Player.fRecoveryDistance;      //�덷�l���
								g_Player.fRecoveryDistance = 0;            //�C��������0��
								g_Player.bCameraRecovery = false;          //�C�������t���O��߂�
								g_Player.fPosX += g_Player.fMoveX*2;         //�ی����|����
							}
							g_Player.fPosX_SUM -= (g_Player.fMoveX*2 + fAdjust);
							g_Player.fPosX += (g_Player.fMoveX*2 + fAdjust);
						}
					}
				}
			}
		}

		//�J�������[�N�ړ���~
		else
		{
			g_Player.bCameraRecovery = true;               //�J�����C���\��

			//------------------------------
			//�C�������𑪂�
			//------------------------------
			//�v���[���[�̓J�����̉E���ړ����E�����E�ɂ���
			if(fPosXCenter > RIGHT_MOVE_LINE)
			{
				g_Player.fRecoveryDistance = RIGHT_MOVE_LINE - fPosXCenter;
			}
			//�v���[���[�̓J�����̍����ړ����E����荶�ɂ���
			if(fPosXCenter < LEFT_MOVE_LINE)
			{
				g_Player.fRecoveryDistance = LEFT_MOVE_LINE - fPosXCenter;
			}
		}

		//-----------------------------------------------------------------------------------------
		//�v���[���[
		//-----------------------------------------------------------------------------------------

		//�ړ��͈͐���
		if ((g_Player.fPosX < 0) || ( (g_Player.fPosX + PLAYER_WIDTH) > SCREEN_WIDTH))
		{
			g_Player.fPosX = g_Player.fPosXOld;
		}

		if ((g_Player.fPosY < 0) || ((g_Player.fPosY + PLAYER_HEIGHT) > SCREEN_HEIGHT))
		{
			g_Player.fPosY = g_Player.fPosYOld;
		}
		
		//test
		if(GetControllerRepeat(2) || GetKeyboardRepeat(DIK_J))
		{
			SetBullet(g_Player.fPosX, g_Player.fPosY, 10 * cos(g_Player.fGunAngel), 10 * sin(g_Player.fGunAngel), 70,g_Player.fGunAngel);
			PlaySound(SOUND_LABEL_BGM000);
		}

		//���蔻��->���G��ԏ���
		if(g_Player.bDamage == false) HitCheckForEnemy();                             //���蔻��
		else
		{
			g_Player.nCntInvincible--;

			if(g_Player.nCntInvincible <= 0)
			{
				g_Player.bDamage = false;
			}
		}

		UpdatePolygonPlayer();

		//�t���[�����Ƃ�false�ɖ߂�t���O
		g_Player.bRun = false;

	}
}

//�v���[���[�|���S���̃f�[�^�X�V
void UpdatePolygonPlayer(void) 
{
	static int nRunCnt = 0;                                       //�����j���O�}���e�N�X�`���̐؂�ւ��J�E���^�[
	static int nActionFlame = 1;                                  //�����j���O�}���̓�����x�����邽�߂̃J�E���^�[
	VERTEX_2D *pVtx = NULL;
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();
																  
	g_pVtxBufferPlayerPolygon->Lock(0, 0, (void**)&pVtx, 0);          //���z�A�h���X���擾���o�b�t�@�����b�N����

	MakeVertexPolygonPlayer(pD3DDevice, pVtx);

	//�v���[���[�{�̂̃p�����[�^
	pVtx[0].pos = D3DXVECTOR3(g_Player.fPosX, g_Player.fPosY, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Player.fPosX + PLAYER_WIDTH, g_Player.fPosY, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Player.fPosX, g_Player.fPosY + PLAYER_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Player.fPosX + PLAYER_WIDTH, g_Player.fPosY + PLAYER_HEIGHT, 0.0f);

	//�v���[���[���ړ����Ă��邪�Ƃ���
	if(g_Player.bRun == false)
	{
		pVtx[0].tex = D3DXVECTOR2(1.0f*1/3, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f*2/3, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f*1/3, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f*2/3, 1.0f);
	}

	else
	{
		pVtx[0].tex = D3DXVECTOR2(1.0f*1/3+nRunCnt%3*1.0f*1/3, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f*2/3+nRunCnt%3*1.0f*1/3, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f*1/3+nRunCnt%3*1.0f*1/3, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f*2/3+nRunCnt%3*1.0f*1/3, 1.0f);

		nActionFlame++;
		if(nActionFlame % 4 == 0) nRunCnt++;                         //4�t���[�����ƂɃ��[�V������؂�ւ�
	}

	//�ϐ�nRunCnt�̃I�[�o�[�t���[�h�~
	if (nRunCnt == 10000)
	{
		nRunCnt = 0;
	}

	//�ϐ�nActionFlame�̃I�[�o�[�t���[�h�~
	if (nActionFlame >= 10000)
	{
		nActionFlame = 0;
	}

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].color = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx += 4;

	//�v���[���[�̃}�V���K���̃p�����[�^

	SpriteRorate(pVtx);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].color = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx += 4;

	//�v���[���[�̎c�胉�C�t��`��
	for(int nCnt = 0; nCnt < g_Player.nLife ;nCnt++)
	{
		pVtx[0].pos = D3DXVECTOR3(HP_POSX + nCnt*HP_INTER, HP_POSY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(HP_POSX + HP_WIDTH+ nCnt*HP_INTER, HP_POSY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(HP_POSX+ nCnt*HP_INTER, HP_POSY + HP_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(HP_POSX + HP_WIDTH+ nCnt*HP_INTER, HP_POSY + HP_HEIGHT, 0.0f);

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx[0].color = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].color = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].color = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].color = D3DCOLOR_RGBA(255, 255, 255, 255);

		pVtx += 4;
	}

	g_pVtxBufferPlayerPolygon->Unlock();                                     //�o�b�t�@�̃A�����b�N
}

//�v���[���[�`��
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	//���_�t�H�[�}�b�g�ݒ�
	pD3DDevice->SetStreamSource(0, g_pVtxBufferPlayerPolygon, 0, sizeof(VERTEX_2D));              //VRAM����GPU�Ƀf�[�^���X�g���[��
	pD3DDevice->SetFVF(FVF_VERTEX2D);

	if(g_Player.bUse == true)
	{
		//���ʏ�ԕ`��
		if(g_Player.bDamage == false)
		{
			//�v���[���[�`��
			pD3DDevice->SetTexture(0, g_pTexturePlayerPolygon[0]);				                             //�e�N�X�`���̐ݒ�
			pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);                                  //�e�N�X�`���������̊m��

			//�C�g�`��
			pD3DDevice->SetTexture(0, g_pTexturePlayerPolygon[1]);				                             //�e�N�X�`���̐ݒ�
			pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, NUM_POLYGON);                                  //�e�N�X�`���������̊m��
		}

		//���G��ԕ`��
		else
		{
			static int nChange;
			if(nChange % 2 == 0)
			{
				//�v���[���[�`��
				pD3DDevice->SetTexture(0, g_pTexturePlayerPolygon[0]);				                             //�e�N�X�`���̐ݒ�
				pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);                                  //�e�N�X�`���������̊m��

				//�C�g�`��
				pD3DDevice->SetTexture(0, g_pTexturePlayerPolygon[1]);				                             //�e�N�X�`���̐ݒ�
				pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, NUM_POLYGON);                                  //�e�N�X�`���������̊m��
			}

			if(g_Player.nCntInvincible % 4 == 0) nChange++;
			if(nChange >= 10000) nChange = 0;
		}

		//�v���[���[HP�`��
		for(int nCnt = 0; nCnt < g_Player.nLife ;nCnt++)
		{
			pD3DDevice->SetTexture(0, g_pTexturePlayerPolygon[2]);
			pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 8+nCnt*4, NUM_POLYGON); 
		}
	}
}

//�v���[���[�|���S���̕\���ʒuX,�|���S���̕\���ʒuY,�|���S���̕�,�|���S���̍���
void MakeVertexPolygonPlayer(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx)
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

PLAYER *GetPlayer(void)
{
	return &g_Player;
}

void SpriteRorate (VERTEX_2D *pVtx)
{
	double X1, Y1, X2, Y2, X3, Y3, X4, Y4;             //����,�E�����W
	double Xc1, Yc1, Xc2, Yc2, Xc3, Yc3, Xc4, Yc4;     //��]������̍��W
	double XC, YC;                                     //���S���W

	X1 = g_Player.fPosX+PLAYER_WIDTH/2-GUN_DISTANCEX;
	Y1 = g_Player.fPosY+GUN_DISTANCEY;
	X2 = g_Player.fPosX+GUN_WIDTH+PLAYER_WIDTH/2-GUN_DISTANCEX;
	Y2 = g_Player.fPosY+GUN_DISTANCEY;
	X3 = g_Player.fPosX+PLAYER_WIDTH/2-GUN_DISTANCEX;
	Y3 = g_Player.fPosY+GUN_HEIGHT+GUN_DISTANCEY;
	X4 = g_Player.fPosX+GUN_WIDTH+PLAYER_WIDTH/2-GUN_DISTANCEX;
	Y4 = g_Player.fPosY+GUN_HEIGHT+GUN_DISTANCEY;

	XC = g_Player.fPosX+GUN_WIDTH/2+PLAYER_WIDTH/2-GUN_DISTANCEX;
	YC = g_Player.fPosY+GUN_HEIGHT/2+GUN_DISTANCEY;

	Xc1 = (X1 - XC)*cos(g_Player.fGunAngel) - (Y1 - YC)*sin(g_Player.fGunAngel) + XC;
	Yc1 = (X1 - XC)*sin(g_Player.fGunAngel) + (Y1 - YC)*cos(g_Player.fGunAngel) + YC;

	Xc2 = (X2 - XC)*cos(g_Player.fGunAngel) - (Y2 - YC)*sin(g_Player.fGunAngel) + XC;
	Yc2 = (X2 - XC)*sin(g_Player.fGunAngel) + (Y2 - YC)*cos(g_Player.fGunAngel) + YC;

	Xc3 = (X3 - XC)*cos(g_Player.fGunAngel) - (Y3 - YC)*sin(g_Player.fGunAngel) + XC;
	Yc3 = (X3 - XC)*sin(g_Player.fGunAngel) + (Y3 - YC)*cos(g_Player.fGunAngel) + YC;

	Xc4 = (X4 - XC)*cos(g_Player.fGunAngel) - (Y4 - YC)*sin(g_Player.fGunAngel) + XC;
	Yc4 = (X4 - XC)*sin(g_Player.fGunAngel) + (Y4 - YC)*cos(g_Player.fGunAngel) + YC;

	pVtx[0].pos = D3DXVECTOR3(Xc1,Yc1,0.0f);
	pVtx[1].pos = D3DXVECTOR3(Xc2,Yc2,0.0f);
	pVtx[2].pos = D3DXVECTOR3(Xc3,Yc3,0.0f);
	pVtx[3].pos = D3DXVECTOR3(Xc4,Yc4,0.0f);
}

//�v���[���[�ɑ΂��铖�蔻��
void HitCheckForEnemy(void)
{
	//�G�̒e�̏��Q�b�g
	BULLETENEMY02  *pEnemyBullet = GetBulletEnemy02();
	GAMEMANAGER *pGameManager = GetManager();
	ENEMYROBOT *pEnemyRobot = GetEnemyRobot();               //�G�ɑ΂��铖�蔻��

	float top = g_Player.fPosY + PLAYER_TOP;
	float bottom = g_Player.fPosY + PLAYER_HEIGHT - PLAYER_HEIGHT_CHECK;
	float left = g_Player.fPosX + PLAYER_HORRIZON;
	float right = g_Player.fPosX + PLAYER_WIDTH - PLAYER_HORRIZON;

	//�G�̒e�ɑ΂��铖�蔻��
	for (int nCnt = 0; nCnt < MAX_BULLETENEMY02; nCnt++)
	{
		//�e���g�p��
		if( (pEnemyBullet+nCnt) -> bUse == true)
		{
			if(
				( top <= ((pEnemyBullet+nCnt)->pos.y+(pEnemyBullet+nCnt)->fRadius) ) &&
				( bottom >= ((pEnemyBullet+nCnt)->pos.y-(pEnemyBullet+nCnt)->fRadius) ) &&
				( left <= ((pEnemyBullet+nCnt)->pos.x+(pEnemyBullet+nCnt)->fRadius-g_Player.fPosX_SUM) ) &&
				( right >= ((pEnemyBullet+nCnt)->pos.x-(pEnemyBullet+nCnt)->fRadius-g_Player.fPosX_SUM) )
				)
			{
				g_Player.nLife--;
				(pEnemyBullet+nCnt) -> bUse = false;
				g_Player.bDamage = true;
				g_Player.nCntInvincible = PLAYER_INVINCIBLE;
			}
		}
	}
	//�G���{�b�g���^�C�v�ɑ΂��铖�蔻��
	for (int nCnt = 0; nCnt < NUM_ENEMYROBOTPOLYGON; nCnt++)
	{
		//�G���g�p��
		if( (pEnemyRobot+nCnt) -> bUse == true)
		{
			//�G�̌��������́�
			if( (pEnemyRobot+nCnt) -> fDirect == -0.1f)
			{
				//�G�͗����U���̎�
				if( (pEnemyRobot+nCnt) -> bAttackDropFlag == true)
				{
					if(
						( top <= ((pEnemyRobot+nCnt)->fPosY+KNIFEENEMYROBOT_DISTANCEY_B ) ) &&
						( bottom >= ((pEnemyRobot+nCnt)->fPosY+KNIFEENEMYROBOT_DISTANCEY_T) )&&
						( left <= ((pEnemyRobot+nCnt)->fPosX+KNIFEENEMYROBOT_DISTANCEX_L-g_Player.fPosX_SUM) ) &&
						( right >= ((pEnemyRobot+nCnt)->fPosX+KNIFEENEMYROBOT_DISTANCEX_R-g_Player.fPosX_SUM) )
						)

					{
						g_Player.nLife--;
						(pEnemyBullet+nCnt) -> bUse = false;
						g_Player.bDamage = true;
						g_Player.nCntInvincible = PLAYER_INVINCIBLE;
					}
				}
			}

			//�G�̌��������́�
			else
			{
				//�G�͗����U���̎�
				if( (pEnemyRobot+nCnt) -> bAttackDropFlag == true)
				{
					if(
						( top <= ((pEnemyRobot+nCnt)->fPosY+KNIFEENEMYROBOT_DISTANCEY_B ) ) &&
						( bottom >= ((pEnemyRobot+nCnt)->fPosY+KNIFEENEMYROBOT_DISTANCEY_T) )&&
						( left <= ((pEnemyRobot+nCnt)->fPosX-KNIFEENEMYROBOT_DISTANCEX_R-g_Player.fPosX_SUM+ENEMYROBOT_WIDTH) ) &&
						( right >= ((pEnemyRobot+nCnt)->fPosX-KNIFEENEMYROBOT_DISTANCEX_L-g_Player.fPosX_SUM+ENEMYROBOT_WIDTH) )
						)
					{
						g_Player.nLife--;
						(pEnemyBullet+nCnt) -> bUse = false;
						g_Player.bDamage = true;
						g_Player.nCntInvincible = PLAYER_INVINCIBLE;
					}
				}
			}
		}
	}

	//�v���[���[�����񂾂��ǂ���
	if(g_Player.nLife <= 0)
	{
		g_Player.bUse = false;
		PlaySound(SOUND_LABEL_SE_EXPLOSION);
		pGameManager -> bGameOver = true;
		pGameManager -> nGameResult = 1;
		StopSound(SOUND_LABEL_BGM_STAGE);
		SetParticalCenter(2,g_Player.fPosX+PLAYER_WIDTH*0.5,g_Player.fPosY+PLAYER_HEIGHT*0.5,10,15,false);
	}
}