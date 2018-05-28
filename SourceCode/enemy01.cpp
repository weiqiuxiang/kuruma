//-----------------------------------------------------------------------------------------
//�v���[���[,����Ȃǂ̏���[running02.cpp]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
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
//�}�N����`
//-----------------------------------------------------------------------------------------
#define ENEMY01_INTERATTACK_TIME (10)                  //�G�̍U����������
#define ENEMY01_RECOVERYATTACK_TIME (120)              //�G�̒e�̑��U����
#define ENEMY01_PREATTACK_TIME (30)                    //�G�̍U���Ԋu
#define BULLET_SPEED (3.0f)                            //�e�̃X�s�[�h
#define BULLET_ANGLE (1.1)                             //�e�̔�ъp�x

//Enemy01�������ł���͈�
#define ENEMY01_SKY_ZONE_UP (100.0f)
#define ENEMY01_SKY_ZONE_BUTTOM (200.0f)

//�G�̖C�e���v���[���[�ɒ������ԃ}�N��
#define E1_TIMETOTAL (120.0f)                //�e���v���[���[�̏ꏊ�ɒ����܂ł̎���
#define ATTACK_TIME_MAX (2)                  //Enemy01�����̍U���ړ����s���܂ł̍U����

//-----------------------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureEnemy01Polygon[NUM_ENEMY01TEXTURE] = {};           //�e�N�X�`���𑀍삷�邽�߂̃|�C���^(�e�N�X�`���o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferEnemy01Polygon = NULL;                      //���_�o�b�t�@�𑀍삷�邽�߂̃|�C���^(���_�o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)

ENEMY01 g_Enemy01[NUM_ENEMY01POLYGON];

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------------------
void Enemy01AttackMove(ENEMY01 *enemy);                                         //Enemy01�̍U���ړ��֐�

//�e�ϐ��̏�����
HRESULT InitEnemy01(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_2D *pVtx = NULL;                           //���_�\���̂̃|�C���^
	pD3DDevice = Get_Device();

	//���_�o�b�t�@�̐���
	if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_ENEMY01VERTEX*NUM_ENEMY01POLYGON, D3DUSAGE_WRITEONLY, FVF_VERTEX2D, D3DPOOL_MANAGED, &g_pVtxBufferEnemy01Polygon, NULL)))
	{
		return E_FAIL;
	}

	//Enemy01�e�N�X�`���̎擾
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, Enemy01POLYGON01_TEXTURENAME, &g_pTextureEnemy01Polygon[0])))
	{
		MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂����s���܂���", "�G���[", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	//�v���[���[�̃|���S��
	g_pVtxBufferEnemy01Polygon -> Lock(0, 0, (void**)&pVtx, 0);                    //���z�A�h���X���擾���o�b�t�@�����b�N����
	MakeVertexPolygonPlayer(pD3DDevice, pVtx);
 
	for (int nCnt = 0; nCnt < MAX_ENEMY01; nCnt++)
	{
		//�v���[���[�̏��f�[�^�̏����l
		g_Enemy01[nCnt].nNumEnemy = 0;                                            //�G�̔ԍ�
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
		//�U�����[�h�̃p�����[�^
		g_Enemy01[nCnt].bAttackMove = false;                                      //�U���ړ����s����
		g_Enemy01[nCnt].bAttackMeasure = false;                                   //��������t���O
		g_Enemy01[nCnt].nAttackTime = rand() % ATTACK_TIME_MAX + 1;               //�ő�U����
		g_Enemy01[nCnt].nCntPreAttack = ENEMY01_PREATTACK_TIME;
		g_Enemy01[nCnt].nCntBullet = 3;
		g_Enemy01[nCnt].nCntInterAttack = ENEMY01_INTERATTACK_TIME;
		g_Enemy01[nCnt].nCntRecoveryAttack = ENEMY01_RECOVERYATTACK_TIME;

		pVtx[0+4*nCnt].pos = D3DXVECTOR3(g_Enemy01[nCnt].fPosX, g_Enemy01[nCnt].fPosY, 0.0f);
		pVtx[1+4*nCnt].pos = D3DXVECTOR3(g_Enemy01[nCnt].fPosX + ENEMY01_WIDTH, g_Enemy01[nCnt].fPosY, 0.0f);
		pVtx[2+4*nCnt].pos = D3DXVECTOR3(g_Enemy01[nCnt].fPosX, g_Enemy01[nCnt].fPosY + ENEMY01_WIDTH, 0.0f);
		pVtx[3+4*nCnt].pos = D3DXVECTOR3(g_Enemy01[nCnt].fPosX + ENEMY01_WIDTH, g_Enemy01[nCnt].fPosY + ENEMY01_WIDTH, 0.0f);
	}

	g_pVtxBufferEnemy01Polygon -> Unlock();                                     //�o�b�t�@�̃A�����b�N

	return S_OK;
}
//�I������
void UninitEnemy01(void)
{
	// �e�N�X�`���C���^�[�t�F�C�X�̕Еt��
	for (int nCnt = 0; nCnt < NUM_ENEMY01TEXTURE; nCnt++)
	{
		SAFE_RELEASE(g_pTextureEnemy01Polygon[nCnt]);
	}

	//���_�o�b�t�@�C���^�[�t�F�C�X�̕Еt��
	for (int nCnt = 0; nCnt < NUM_ENEMY01POLYGON; nCnt++)
	{
		SAFE_RELEASE(g_pVtxBufferEnemy01Polygon);
	}
}

//�v���[���[�f�[�^�̍X�V����
void UpdateEnemy01(void)
{
	//�v���[���[�̃f�[�^�擾
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	//�v���[���[�̒e�f�[�^�擾
	BULLET *pBullet;
	pBullet = GetBullet();

	//�X�e�[�W01�̃f�[�^�擾
	STAGE01 *pStage01;
	pStage01 = GetStage01();

	for (int nCnt = 0; nCnt < MAX_ENEMY01; nCnt++)
	{
		//�����G�͎g�p�̏�ԂȂ�
		if (g_Enemy01[nCnt].bUse == true)
		{
			//�O�t���[��Enemy01�̗̑͂�0�ɂȂ������ǂ���
			if(g_Enemy01[nCnt].nLife <= 0)
			{
				g_Enemy01[nCnt].bUse = false;          //�G�ɖ��g�p�ɂ���
				pPlayer -> fScore += 100;              //�X�R�A���Z
				SetParticalCenter(2,g_Enemy01[nCnt].fPosX+ENEMY01_WIDTH*0.5,g_Enemy01[nCnt].fPosY+ENEMY01_HEIGHT*0.5,10,15,true);
				PlaySound(SOUND_LABEL_SE_EXPLOSION);
			}

			//-----------------------------------------------------------------------------------------
			//Enemy01�̈ړ������ƍU������
			//-----------------------------------------------------------------------------------------
			//�d����Ԃ���Ȃ�
			if(g_Enemy01[nCnt].bRecovery == false)
			{
				//Enemy01�̓v���[���[�̉E���ɂ���
				if(g_Enemy01[nCnt].fPosX - pPlayer->fPosX_SUM > pPlayer -> fPosX)
				{
					g_Enemy01[nCnt].fDirect = -0.1f;
					//�T�m�͈͊O(�ړ�)
					//�����F�U���͈͂ɓ��邩������ԂȂ�
					if( (g_Enemy01[nCnt].fPosX - pPlayer->fPosX_SUM - pPlayer -> fPosX > 200) && (g_Enemy01[nCnt].bPre == false) )
					{
						g_Enemy01[nCnt].fPosX -= g_Enemy01[nCnt].fMoveX;
						g_Enemy01[nCnt].fDirect = -0.1f;
						g_Enemy01[nCnt].bMove = true;
					}

					//�T�m�͈͓�
					else
					{
						//�������~��,�U�������ɓ���
						g_Enemy01[nCnt].bMove = false;
						g_Enemy01[nCnt].bPre = true;

						//�U���ړ����s��
						if(g_Enemy01[nCnt].nAttackTime == 0)
						{
							Enemy01AttackMove(&g_Enemy01[nCnt]);
						}

						//�U������
						else
						{
							//�U����Ԃɓ���
							if(g_Enemy01[nCnt].nCntPreAttack == 0)
							{
								//�U���J�n
								if(g_Enemy01[nCnt].bInter == false)
								{
									SetBulletEnemy02(1,g_Enemy01[nCnt].fPosX + BULLETENEMY01_DISTANCEX,g_Enemy01[nCnt].fPosY + BULLETENEMY01_DISTANCEY,-BULLET_SPEED*cos(BULLET_ANGLE),BULLET_SPEED*sin(BULLET_ANGLE),180,0.0f,6);
									g_Enemy01[nCnt].nCntBullet--;

									//�܂��e����
									if(g_Enemy01[nCnt].nCntBullet > 0)
									{
										g_Enemy01[nCnt].bInter = true;
									}

									//�e���ł��؂�ƍd����Ԃɓ���
									else
									{
										g_Enemy01[nCnt].bRecovery = true;
									}

								}
								//�A�ˊԊu��
								else
								{
									g_Enemy01[nCnt].nCntInterAttack--;

									//�A�ˊԊu�I��
									if(g_Enemy01[nCnt].nCntInterAttack == 0)
									{
										g_Enemy01[nCnt].nCntInterAttack = ENEMY01_INTERATTACK_TIME;
										g_Enemy01[nCnt].bInter = false;
									}
								}
							}

							//�U�������i�K
							else
							{
								g_Enemy01[nCnt].nCntPreAttack--;
							}
						}
					}
				}

				//Enemy01�̓v���[���[�̍����ɂ���
				else
				{
					g_Enemy01[nCnt].fDirect = 0.1f;

					//�T�m�͈͊O
					if( (pPlayer -> fPosX - g_Enemy01[nCnt].fPosX + pPlayer->fPosX_SUM > 200) && (g_Enemy01[nCnt].bPre == false) )
					{
						g_Enemy01[nCnt].fPosX += g_Enemy01[nCnt].fMoveX;
						g_Enemy01[nCnt].fDirect = 0.1f;
						g_Enemy01[nCnt].bMove = true;
					}

					//�T�m�͈͓�
					else
					{
						//�������~��,�U�������ɓ���
						g_Enemy01[nCnt].bMove = false;
						g_Enemy01[nCnt].bPre = true;

						//�U����Ԃɓ���
						if(g_Enemy01[nCnt].nCntPreAttack == 0)
						{
							//�U���ړ����s��
							if(g_Enemy01[nCnt].nAttackTime == 0)
							{
								Enemy01AttackMove(&g_Enemy01[nCnt]);
							}

							else
							{
								//�U���J�n
								if(g_Enemy01[nCnt].bInter == false)
								{
									SetBulletEnemy02(1,g_Enemy01[nCnt].fPosX + BULLETENEMY01_DISTANCEX,g_Enemy01[nCnt].fPosY + BULLETENEMY01_DISTANCEY,BULLET_SPEED*cos(BULLET_ANGLE),BULLET_SPEED*sin(BULLET_ANGLE),180,0.0f,6);
									g_Enemy01[nCnt].nCntBullet--;

									//�܂��e����
									if(g_Enemy01[nCnt].nCntBullet > 0)
									{
										g_Enemy01[nCnt].bInter = true;
									}

									//�e���ł��؂�ƍd����Ԃɓ���
									else
									{
										g_Enemy01[nCnt].bRecovery = true;
									}

								}
								//�A�ˊԊu��
								else
								{
									g_Enemy01[nCnt].nCntInterAttack--;

									//�A�ˊԊu�I��
									if(g_Enemy01[nCnt].nCntInterAttack == 0)
									{
										g_Enemy01[nCnt].nCntInterAttack = ENEMY01_INTERATTACK_TIME;
										g_Enemy01[nCnt].bInter = false;
									}
								}
							}
						}

						//�U�������i�K
						else
						{
							g_Enemy01[nCnt].nCntPreAttack--;
						}
					}
				}
			}


			//�d�����
			else
			{
				g_Enemy01[nCnt].nCntRecoveryAttack--;

				//�d����ԏI��
				if(g_Enemy01[nCnt].nCntRecoveryAttack == 0)
				{
					g_Enemy01[nCnt].nCntRecoveryAttack = ENEMY01_RECOVERYATTACK_TIME;
					g_Enemy01[nCnt].nAttackTime--;
					g_Enemy01[nCnt].bRecovery = false;

					//�e�̕�[
					g_Enemy01[nCnt].nCntBullet = 3;

					//�v���[���[���U���͈͊O�ɂ��邩
					if( (pPlayer -> fPosX - g_Enemy01[nCnt].fPosX + pPlayer->fPosX_SUM < -400) || (pPlayer -> fPosX - g_Enemy01[nCnt].fPosX + pPlayer->fPosX_SUM > 400 ) )
					{
						g_Enemy01[nCnt].bPre = false;                     //�ړ���Ԃɖ߂�
						g_Enemy01[nCnt].nCntPreAttack = ENEMY01_PREATTACK_TIME;
					}
				}

				//�U���ړ����[�h�ɖ߂�
				if(g_Enemy01[nCnt].nAttackTime == 0)
				{
					g_Enemy01[nCnt].bAttackMeasure = false;
					g_Enemy01[nCnt].bAttackMove = false;
				}
			}

			//-----------------------------------------------------------------------------------------
			//Enemy01�̓��蔻�菈��
			//-----------------------------------------------------------------------------------------
			float fTop = g_Enemy01[nCnt].fPosY + ENEMY01_TOP;
			float fBottom = g_Enemy01[nCnt].fPosY + ENEMY01_HEIGHT - ENEMY01_HEIGHT_CHECK;
			float fLeft = g_Enemy01[nCnt].fPosX - pPlayer->fPosX_SUM + ENEMY01_HORRIZON;
			float fRight = g_Enemy01[nCnt].fPosX - pPlayer->fPosX_SUM + ENEMY01_WIDTH - ENEMY01_HORRIZON;

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
						g_Enemy01[nCnt].nLife--;
						PlaySound(SOUND_LABEL_SE_HITENEMY);
						g_Enemy01[nCnt].bHit = true;
						pBullet[nCntBullet].bUse = false;
						//�p�[�e�B�N��
						SetParticalCenter(2,pBullet[nCntBullet].pos.x+BULLET_WIDTH*0.5,pBullet[nCntBullet].pos.y+BULLET_HEIGHT*0.5,1,4,false);
					}
				}
			}
		}
	}

	UpdatePolygonEnemy01();
}

//�v���[���[�|���S���̃f�[�^�X�V
void UpdatePolygonEnemy01(void)
{
	static int nRunCnt = 0;                                       //�����j���O�}���e�N�X�`���̐؂�ւ��J�E���^�[
	static int nActionFlame = 1;                                  //�����j���O�}���̓�����x�����邽�߂̃J�E���^�[
	VERTEX_2D *pVtx = NULL;
	LPDIRECT3DDEVICE9 pD3DDevice;
	PLAYER *pPlayer = GetPlayer();
	pD3DDevice = Get_Device();

	g_pVtxBufferEnemy01Polygon -> Lock(0, 0, (void**)&pVtx, 0);          //���z�A�h���X���擾���o�b�t�@�����b�N����
	MakeVertexPolygonPlayer(pD3DDevice, pVtx);

	for (int nCnt = 0; nCnt < MAX_ENEMY01; nCnt++)
	{
		if (g_Enemy01[nCnt].bUse == true)
		{
			//�G�̍��W�̌v�Z�F�G�̐�΍��W-�v���[���[�̈ړ��� = �G�̃v���[���[�ɑ��΂�����W (X���̂�)
			pVtx[0+4*nCnt].pos = D3DXVECTOR3(g_Enemy01[nCnt].fPosX - pPlayer->fPosX_SUM, g_Enemy01[nCnt].fPosY, 0.0f);
			pVtx[1+4*nCnt].pos = D3DXVECTOR3(g_Enemy01[nCnt].fPosX + ENEMY01_WIDTH - pPlayer->fPosX_SUM, g_Enemy01[nCnt].fPosY, 0.0f);
			pVtx[2+4*nCnt].pos = D3DXVECTOR3(g_Enemy01[nCnt].fPosX - pPlayer->fPosX_SUM, g_Enemy01[nCnt].fPosY + ENEMY01_HEIGHT, 0.0f);
			pVtx[3+4*nCnt].pos = D3DXVECTOR3(g_Enemy01[nCnt].fPosX + ENEMY01_WIDTH - pPlayer->fPosX_SUM, g_Enemy01[nCnt].fPosY + ENEMY01_HEIGHT, 0.0f);

			g_Enemy01[nCnt].fPosX_Relative = g_Enemy01[nCnt].fPosX - pPlayer->fPosX_SUM;                    //�G�̑��΍��W�����߂�

			//�G��������
			if (g_Enemy01[nCnt].fDirect == -0.1f)
			{
				pVtx[0+4*nCnt].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.5), 0.0f);
				pVtx[1+4*nCnt].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.5) + 0.5f, 0.0f);
				pVtx[2+4*nCnt].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.5), 1.0f);
				pVtx[3+4*nCnt].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.5) + 0.5f, 1.0f);
			}

			//�G���E����
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
	g_pVtxBufferEnemy01Polygon->Unlock();                                     //�o�b�t�@�̃A�����b�N

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

void DrawEnemy01(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	pD3DDevice->SetStreamSource(0, g_pVtxBufferEnemy01Polygon, 0, sizeof(VERTEX_2D));              //VRAM����GPU�Ƀf�[�^���X�g���[��
	pD3DDevice->SetFVF(FVF_VERTEX2D);

	for (int nCnt = 0; nCnt < MAX_ENEMY01; nCnt++)
	{
		//�����G�͎g�p�̏�ԂȂ�
		if (g_Enemy01[nCnt].bUse == true)
		{
			//�w�i�̓S�����F���_�t�H�[�}�b�g�ݒ�
			pD3DDevice->SetTexture(0, g_pTextureEnemy01Polygon[0]);				 //�e�N�X�`���̐ݒ�
			pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt*4, NUM_POLYGON);                                            //�e�N�X�`���������̊m��
		}
	}
}


//�v���[���[�|���S���̕\���ʒuX,�|���S���̕\���ʒuY,�|���S���̕�,�|���S���̍���
void MakeVertexPolygonEnemy01(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx)
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

//�G���擾
ENEMY01 *GetEnemy01(void)
{
	return &g_Enemy01[0];
}

//�g���G�̐���ݒ肷��
void SetEnemy01(float fPosX,float fPosY,float fSpeedX,float fSpeedY,float fDirect,float nEnemyNum)
{
	for (int nCnt = 0; nCnt < MAX_ENEMY01; nCnt++) 
	{
		if(g_Enemy01[nCnt].bUse == false)
		{
			g_Enemy01[nCnt].nNumEnemy = nEnemyNum;                   //�G�̔ԍ���ݒ�
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

			//�U�����[�h�̃t���O�ݒ�
			g_Enemy01[nCnt].bAttackMeasure = false;
			g_Enemy01[nCnt].bAttackMove = false;
			g_Enemy01[nCnt].nAttackTime = 0;

			break;
		}
	}
}

//Enemy01�̍U���ړ��֐�
void Enemy01AttackMove(ENEMY01 *enemy)
{
	PLAYER *pPlayer;
	pPlayer = GetPlayer();
	float fXDistance;                      //�v���[���[�ƓG��X������
	float fYDistance;                      //�v���[���[�ƓG��Y������
	static float fXMovePos;               //�G�̕K�v��X���̍��W
	static float fYMovePos;               //�G�̕K�v��Y���̍��W
	static bool bXMove = false;
	static bool bYMove = false;
	float fRandPosY;                       //�����]���̈ړ��ʒu


	//�v���[���[�Ƃ̋���������܂�����?
	if( enemy -> bAttackMeasure == false)
	{
		//X�������v�Z
		fXDistance = (pPlayer -> fPosX + PLAYER_WIDTH*0.5) - (enemy -> fPosX + ENEMY01_WIDTH*0.5 - pPlayer -> fPosX_SUM);   //�v���[���[�Ƃ�X�������v�Z
		fYDistance = (pPlayer -> fPosY + PLAYER_HEIGHT*0.5) - (enemy -> fPosY + ENEMY01_HEIGHT*0.5);                        //�v���[���[�Ƃ�Y�������v�Z
		fRandPosY = rand()%(int)(ENEMY01_SKY_ZONE_BUTTOM- ENEMY01_SKY_ZONE_UP)  + ENEMY01_SKY_ZONE_UP;
		fYMovePos = fRandPosY;

		//Enemy01�͉E��
		if(fXDistance < 0)
		{
			fXMovePos = (pPlayer -> fPosX + PLAYER_WIDTH*0.5) + ( (pPlayer -> fPosY + PLAYER_HEIGHT*0.5) - fRandPosY) / tan(BULLET_ANGLE);
		}

		else
		{
			fXMovePos = (pPlayer -> fPosX + PLAYER_WIDTH*0.5) - ( (pPlayer -> fPosY + PLAYER_HEIGHT*0.5) - fRandPosY) / tan(BULLET_ANGLE);
		}

		enemy -> bAttackMeasure = true;             //�t���O��true�ɂ�,���̒i�K���s��
	}

	//��������I���čU���ړ����s��
	if(enemy -> bAttackMeasure == true)
	{
		//�ړ��r��
		if(enemy -> bAttackMove == false)
		{
			//------------------------------------------------
			//X���̈ړ�
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
			//Y���̈ړ�
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

		//�ړ��I��
		if( (bXMove == true) && (bYMove == true) )
		{
			bXMove = false;
			bYMove = false;
			enemy -> nAttackTime = rand() % ATTACK_TIME_MAX + 1;               //�ő�U���񐔐ݒ�
		}
	}
}