//-----------------------------------------------------------------------------------------
//�v���[���[,����Ȃǂ̏���[running02.cpp]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------------------
#include <time.h>
#include "main.h"
#include "enemy02.h"
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
#define ENEMY02_INTERATTACK_TIME (60)                  //�G�̍U����������
#define ENEMY02_RECOVERYATTACK_TIME (90)              //�G�̒e�̑��U����
#define ENEMY02_PREATTACK_TIME (60)                    //�G�̍U���Ԋu

//�G�̖C�e���v���[���[�ɒ������ԃ}�N��
#define E_TIMETOTAL (120.0f)                 //�e���v���[���[�̏ꏊ�ɒ����܂ł̎���
#define E_TIMEUP (30.0f)                    //�e�̏㏸����(��2-1�̂ݎg�p)

//-----------------------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureEnemy02Polygon[NUM_ENEMY02TEXTURE] = {};           //�e�N�X�`���𑀍삷�邽�߂̃|�C���^(�e�N�X�`���o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferEnemy02Polygon = NULL;                      //���_�o�b�t�@�𑀍삷�邽�߂̃|�C���^(���_�o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)

ENEMY02 g_Enemy02[NUM_ENEMY02POLYGON];
//-----------------------------------------------------------------------------------------
//�d�v��Ԋ֐��B
//-----------------------------------------------------------------------------------------

//�e�ϐ��̏�����
HRESULT InitEnemy02(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_2D *pVtx = NULL;                           //���_�\���̂̃|�C���^
	pD3DDevice = Get_Device();

	//���_�o�b�t�@�̐���
	if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_ENEMY02VERTEX*NUM_ENEMY02POLYGON, D3DUSAGE_WRITEONLY, FVF_VERTEX2D, D3DPOOL_MANAGED, &g_pVtxBufferEnemy02Polygon, NULL)))
	{
		return E_FAIL;
	}

	//ENEMY02�e�N�X�`���̎擾
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, ENEMY02POLYGON01_TEXTURENAME, &g_pTextureEnemy02Polygon[0])))
	{
		MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂����s���܂���", "�G���[", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	//�v���[���[�̃|���S��
	g_pVtxBufferEnemy02Polygon -> Lock(0, 0, (void**)&pVtx, 0);                    //���z�A�h���X���擾���o�b�t�@�����b�N����
	MakeVertexPolygonPlayer(pD3DDevice, pVtx);

	for (int nCnt = 0; nCnt < MAX_ENEMY02; nCnt++)
	{
		//�v���[���[�̏��f�[�^�̏����l
		g_Enemy02[nCnt].nNumEnemy = 0;                                        //�G�̔ԍ�
		g_Enemy02[nCnt].fPosX = 0.0f;
		g_Enemy02[nCnt].fPosY = 0.0f;
		g_Enemy02[nCnt].fDirect = -0.1;
		g_Enemy02[nCnt].bHit = false;
		g_Enemy02[nCnt].bUse = false;
		g_Enemy02[nCnt].bPre = false;
		g_Enemy02[nCnt].bInter = false;
		g_Enemy02[nCnt].bRecovery = false;
		g_Enemy02[nCnt].nCntPreAttack = ENEMY02_PREATTACK_TIME;
		g_Enemy02[nCnt].nCntBullet = 3;
		g_Enemy02[nCnt].nCntInterAttack = ENEMY02_INTERATTACK_TIME;
		g_Enemy02[nCnt].nCntRecoveryAttack = ENEMY02_RECOVERYATTACK_TIME;

		pVtx[0+4*nCnt].pos = D3DXVECTOR3(g_Enemy02[nCnt].fPosX, g_Enemy02[nCnt].fPosY, 0.0f);
		pVtx[1+4*nCnt].pos = D3DXVECTOR3(g_Enemy02[nCnt].fPosX + ENEMY02_WIDTH, g_Enemy02[nCnt].fPosY, 0.0f);
		pVtx[2+4*nCnt].pos = D3DXVECTOR3(g_Enemy02[nCnt].fPosX, g_Enemy02[nCnt].fPosY + ENEMY02_WIDTH, 0.0f);
		pVtx[3+4*nCnt].pos = D3DXVECTOR3(g_Enemy02[nCnt].fPosX + ENEMY02_WIDTH, g_Enemy02[nCnt].fPosY + ENEMY02_WIDTH, 0.0f);

	}

	g_pVtxBufferEnemy02Polygon -> Unlock();                                     //�o�b�t�@�̃A�����b�N

	return S_OK;
}
//�I������
void UninitEnemy02(void)
{
	// �e�N�X�`���C���^�[�t�F�C�X�̕Еt��
	for (int nCnt = 0; nCnt < NUM_ENEMY02TEXTURE; nCnt++)
	{
		SAFE_RELEASE(g_pTextureEnemy02Polygon[nCnt]);
	}

	//���_�o�b�t�@�C���^�[�t�F�C�X�̕Еt��
	for (int nCnt = 0; nCnt < NUM_ENEMY02POLYGON; nCnt++)
	{
		SAFE_RELEASE(g_pVtxBufferEnemy02Polygon);
	}
}

//�v���[���[�f�[�^�̍X�V����
void UpdateEnemy02(void)
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

	for (int nCnt = 0; nCnt < MAX_ENEMY02; nCnt++)
	{
		//�����G�͎g�p�̏�ԂȂ�
		if (g_Enemy02[nCnt].bUse == true)
		{
			//-----------------------------------
			//���e�_�v�Z
			//-----------------------------------
			//�O�t���[��Enemy02�̗̑͂�0�ɂȂ������ǂ���
			if(g_Enemy02[nCnt].nLife <= 0)
			{
				g_Enemy02[nCnt].bUse = false;         //�G�𖢎g�p�ɂ���
				pPlayer -> fScore += 100;             //�X�R�A���Z
				SetParticalCenter(2,g_Enemy02[nCnt].fPosX+ENEMY02_WIDTH*0.5,g_Enemy02[nCnt].fPosY+ENEMY02_HEIGHT*0.5,10,15,true);
				PlaySound(SOUND_LABEL_SE_EXPLOSION);
			}

			//-----------------------------------------------------------------------------------------
			//Enemy02�̈ړ������ƍU������
			//-----------------------------------------------------------------------------------------
			//�d����Ԃ���Ȃ�
			if(g_Enemy02[nCnt].bRecovery == false)
			{
				//Enemy02�̓v���[���[�̉E���ɂ���
				if(g_Enemy02[nCnt].fPosX - pPlayer->fPosX_SUM > pPlayer -> fPosX)
				{
					//��������
					g_Enemy02[nCnt].fDirect = -0.1f;

					//�T�m�͈͊O(�ړ�)
					//�����F�U���͈͂ɓ��邩������ԂȂ�
					if( (pPlayer -> fPosX + PLAYER_WIDTH - g_Enemy02[nCnt].fPosX + pPlayer->fPosX_SUM < -400) && (g_Enemy02[nCnt].bPre == false) )
					{
						g_Enemy02[nCnt].fPosX -= g_Enemy02[nCnt].fMoveX;
						g_Enemy02[nCnt].fDirect = -0.1f;
						g_Enemy02[nCnt].bMove = true;
					}

					//�T�m�͈͓�
					else
					{
						float E2_Center= g_Enemy02[nCnt].fPosX + ENEMY02_WIDTH/2 - pPlayer->fPosX_SUM;              //�G�̒��S���W

						//�G�͕\���͈͓��ɂ���
						if(E2_Center <= (SCREEN_WIDTH-50) )
						{

							//�������~��,�U�������ɓ���
							g_Enemy02[nCnt].bMove = false;
							g_Enemy02[nCnt].bPre = true;

							//�U����Ԃɓ���
							if(g_Enemy02[nCnt].nCntPreAttack == 0)
							{
								//�U���J�n
								if(g_Enemy02[nCnt].bInter == false)
								{
									D3DXVECTOR2 E_BulletSpeed = BulletLanding(g_Enemy02[nCnt]);                  //���e�_�v�Z
									SetBulletEnemy02(2,g_Enemy02[nCnt].fPosX + BULLETENEMY02_DISTANCEX,g_Enemy02[nCnt].fPosY + BULLETENEMY02_DISTANCEY,E_BulletSpeed.x,E_BulletSpeed.y,180,0.0f,12);
									g_Enemy02[nCnt].nCntBullet--;

									//�܂��e����
									if(g_Enemy02[nCnt].nCntBullet > 0)
									{
										g_Enemy02[nCnt].bInter = true;
									}

									//�e���ł��؂�ƍd����Ԃɓ���
									else
									{
										g_Enemy02[nCnt].bRecovery = true;
									}

								}
								//�A�ˊԊu��
								else
								{
									g_Enemy02[nCnt].nCntInterAttack--;

									//�A�ˊԊu�I��
									if(g_Enemy02[nCnt].nCntInterAttack == 0)
									{
										g_Enemy02[nCnt].nCntInterAttack = ENEMY02_INTERATTACK_TIME;
										g_Enemy02[nCnt].bInter = false;
									}
								}
							}

							//�U�������i�K
							else
							{
								g_Enemy02[nCnt].nCntPreAttack--;
							}
						}

						//�G�͕\���͈͓��ɂ��Ȃ�(�ړ�)
						else
						{
							g_Enemy02[nCnt].fPosX -= g_Enemy02[nCnt].fMoveX;
							g_Enemy02[nCnt].fDirect = -0.1f;
							g_Enemy02[nCnt].bMove = true;
						}
					}
				}

				//Enemy02�̓v���[���[�̍����ɂ���
				else
				{
					//��������
					g_Enemy02[nCnt].fDirect = 0.1f;

					//�T�m�͈͊O
					if( (pPlayer -> fPosX - (g_Enemy02[nCnt].fPosX + ENEMY02_WIDTH) + pPlayer->fPosX_SUM > 400 ) && (g_Enemy02[nCnt].bPre == false) )
					{
						g_Enemy02[nCnt].fPosX += g_Enemy02[nCnt].fMoveX;
						g_Enemy02[nCnt].fDirect = 0.1f;
						g_Enemy02[nCnt].bMove = true;
					}

					//�T�m�͈͓�
					else
					{
						float E2_Center= g_Enemy02[nCnt].fPosX + ENEMY02_WIDTH/2 - pPlayer->fPosX_SUM;              //�G�̒��S���W

						//�G�͕\���͈͓��ɂ���
						if(E2_Center >= 50)
						{
							//�������~��,�U�������ɓ���
							g_Enemy02[nCnt].bMove = false;
							g_Enemy02[nCnt].bPre = true;

							//�U����Ԃɓ���
							if(g_Enemy02[nCnt].nCntPreAttack == 0)
							{

								//�U���J�n
								if(g_Enemy02[nCnt].bInter == false)
								{
									D3DXVECTOR2 E_BulletSpeed = BulletLanding(g_Enemy02[nCnt]);               //���e�_�v�Z
									SetBulletEnemy02(2,g_Enemy02[nCnt].fPosX - BULLETENEMY02_DISTANCEX + ENEMY02_WIDTH,g_Enemy02[nCnt].fPosY + BULLETENEMY02_DISTANCEY,E_BulletSpeed.x,E_BulletSpeed.y,180,0.0f,12);
									g_Enemy02[nCnt].nCntBullet--;

									//�܂��e����
									if(g_Enemy02[nCnt].nCntBullet > 0)
									{
										g_Enemy02[nCnt].bInter = true;
									}

									//�e���ł��؂�ƍd����Ԃɓ���
									else
									{
										g_Enemy02[nCnt].bRecovery = true;
									}

								}
								//�A�ˊԊu��
								else
								{
									g_Enemy02[nCnt].nCntInterAttack--;

									//�A�ˊԊu�I��
									if(g_Enemy02[nCnt].nCntInterAttack == 0)
									{
										g_Enemy02[nCnt].nCntInterAttack = ENEMY02_INTERATTACK_TIME;
										g_Enemy02[nCnt].bInter = false;
									}
								}
							}

							//�U�������i�K
							else
							{
								g_Enemy02[nCnt].nCntPreAttack--;
							}
						}

						//�G�͕\���͈͓��ɂ��Ȃ�(�ړ�)
						else
						{
							g_Enemy02[nCnt].fPosX += g_Enemy02[nCnt].fMoveX;
							g_Enemy02[nCnt].fDirect = 0.1f;
							g_Enemy02[nCnt].bMove = true;
						}
					}
				}
			}


			//�d�����
			else
			{
				g_Enemy02[nCnt].nCntRecoveryAttack--;

				//�d����ԏI��
				if(g_Enemy02[nCnt].nCntRecoveryAttack == 0)
				{
					g_Enemy02[nCnt].nCntRecoveryAttack = ENEMY02_RECOVERYATTACK_TIME;
					g_Enemy02[nCnt].bRecovery = false;

					//�e�̕�[
					g_Enemy02[nCnt].nCntBullet = 3;

					//�v���[���[���U���͈͊O�ɂ��邩
					if( (pPlayer -> fPosX + PLAYER_WIDTH - g_Enemy02[nCnt].fPosX + pPlayer->fPosX_SUM < -400) || (pPlayer -> fPosX - (g_Enemy02[nCnt].fPosX + ENEMY02_WIDTH) + pPlayer->fPosX_SUM > 400 ) )
					{
						g_Enemy02[nCnt].bPre = false;                     //�ړ���Ԃɖ߂�
						g_Enemy02[nCnt].nCntPreAttack = ENEMY02_PREATTACK_TIME;
					}
				}
			}


			//-----------------------------------------------------------------------------------------
			//Enemy02�̓��蔻�菈��
			//-----------------------------------------------------------------------------------------

			//Enemy02�̏d�͉e��
			g_Enemy02[nCnt].fMoveY += 0.15f;
			g_Enemy02[nCnt].fPosY += g_Enemy02[nCnt].fMoveY;

			if(g_Enemy02[nCnt].fPosY + ENEMY02_HEIGHT > 447.0f + ENEMY02_HEIGHT_ADJUST) 
			{
				g_Enemy02[nCnt].fPosY = 447.0f - ENEMY02_HEIGHT + ENEMY02_HEIGHT_ADJUST;
			}

			float fTop = g_Enemy02[nCnt].fPosY + ENEMY02_TOP;
			float fBottom = g_Enemy02[nCnt].fPosY + ENEMY02_HEIGHT - ENEMY02_HEIGHT_CHECK;
			float fLeft = g_Enemy02[nCnt].fPosX - pPlayer->fPosX_SUM + ENEMY02_HORRIZON;
			float fRight = g_Enemy02[nCnt].fPosX - pPlayer->fPosX_SUM + ENEMY02_WIDTH - ENEMY02_HORRIZON;
			
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
						g_Enemy02[nCnt].nLife--;
						g_Enemy02[nCnt].bHit = true;
						pBullet[nCntBullet].bUse = false;
						PlaySound(SOUND_LABEL_SE_HITENEMY);
						//�p�[�e�B�N��
						SetParticalCenter(2,pBullet[nCntBullet].pos.x+BULLET_WIDTH*0.5,pBullet[nCntBullet].pos.y+BULLET_HEIGHT*0.5,1,4,false);
					}
				}
			}
		}
	}

	UpdatePolygonEnemy02();
}

//�v���[���[�|���S���̃f�[�^�X�V
void UpdatePolygonEnemy02(void)
{
	static int nRunCnt = 0;                                       //�����j���O�}���e�N�X�`���̐؂�ւ��J�E���^�[
	static int nActionFlame = 1;                                  //�����j���O�}���̓�����x�����邽�߂̃J�E���^�[
	VERTEX_2D *pVtx = NULL;
	LPDIRECT3DDEVICE9 pD3DDevice;
	PLAYER *pPlayer = GetPlayer();
	pD3DDevice = Get_Device();

	g_pVtxBufferEnemy02Polygon -> Lock(0, 0, (void**)&pVtx, 0);          //���z�A�h���X���擾���o�b�t�@�����b�N����
	MakeVertexPolygonPlayer(pD3DDevice, pVtx);

	for (int nCnt = 0; nCnt < MAX_ENEMY02; nCnt++)
	{
		if (g_Enemy02[nCnt].bUse == true)
		{
			//�G�̍��W�̌v�Z�F�G�̐�΍��W-�v���[���[�̈ړ��� = �G�̃v���[���[�ɑ��΂�����W (X���̂�)
			pVtx[0+4*nCnt].pos = D3DXVECTOR3(g_Enemy02[nCnt].fPosX - pPlayer->fPosX_SUM, g_Enemy02[nCnt].fPosY, 0.0f);
			pVtx[1+4*nCnt].pos = D3DXVECTOR3(g_Enemy02[nCnt].fPosX + ENEMY02_WIDTH - pPlayer->fPosX_SUM, g_Enemy02[nCnt].fPosY, 0.0f);
			pVtx[2+4*nCnt].pos = D3DXVECTOR3(g_Enemy02[nCnt].fPosX - pPlayer->fPosX_SUM, g_Enemy02[nCnt].fPosY + ENEMY02_HEIGHT, 0.0f);
			pVtx[3+4*nCnt].pos = D3DXVECTOR3(g_Enemy02[nCnt].fPosX + ENEMY02_WIDTH - pPlayer->fPosX_SUM, g_Enemy02[nCnt].fPosY + ENEMY02_HEIGHT, 0.0f);

			g_Enemy02[nCnt].fPosX_Relative = g_Enemy02[nCnt].fPosX - pPlayer->fPosX_SUM;                    //�G�̑��΍��W�����߂�

			//�G���ړ����
			if(g_Enemy02[nCnt].bMove == true)
			{
				if (g_Enemy02[nCnt].fDirect == -0.1f)
				{
					pVtx[0+4*nCnt].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.25), 0.0f);
					pVtx[1+4*nCnt].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.25) + 0.25f, 0.0f);
					pVtx[2+4*nCnt].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.25), 0.5f);
					pVtx[3+4*nCnt].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.25) + 0.25f, 0.5f);
				}

				else
				{
					pVtx[0+4*nCnt].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.25) + 0.25f, 0.0f);
					pVtx[1+4*nCnt].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.25), 0.0f);
					pVtx[2+4*nCnt].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.25) + 0.25f, 0.5f);
					pVtx[3+4*nCnt].tex = D3DXVECTOR2((float)(nRunCnt % 2 * 0.25), 0.5f);
				}
			}

			//�G���~�܂��Ă���
			else
			{
				if (g_Enemy02[nCnt].fDirect == -0.1f)
				{
					//�U�����[�V��������
					if(g_Enemy02[nCnt].nCntPreAttack != 0)
					{
						//���[�V����01
						if(g_Enemy02[nCnt].nCntPreAttack > ENEMY02_PREATTACK_TIME * 0.25 * 3)
						{
							pVtx[0+4*nCnt].tex = D3DXVECTOR2( 0.0f, 0.5f);
							pVtx[1+4*nCnt].tex = D3DXVECTOR2( 0.25f, 0.5f);
							pVtx[2+4*nCnt].tex = D3DXVECTOR2( 0.0f, 1.0f);
							pVtx[3+4*nCnt].tex = D3DXVECTOR2( 0.25f, 1.0f);
						}

						//���[�V����02
						else if( (g_Enemy02[nCnt].nCntPreAttack <= ENEMY02_PREATTACK_TIME * 0.25 * 3) &&
								 (g_Enemy02[nCnt].nCntPreAttack > ENEMY02_PREATTACK_TIME * 0.25 * 2)
							)
						{
							pVtx[0+4*nCnt].tex = D3DXVECTOR2( 0.25f, 0.5f);
							pVtx[1+4*nCnt].tex = D3DXVECTOR2( 0.5f, 0.5f);
							pVtx[2+4*nCnt].tex = D3DXVECTOR2( 0.25f, 1.0f);
							pVtx[3+4*nCnt].tex = D3DXVECTOR2( 0.5f, 1.0f);
						}

						//���[�V����03
						else if( (g_Enemy02[nCnt].nCntPreAttack <= ENEMY02_PREATTACK_TIME * 0.25 * 2) &&
								 (g_Enemy02[nCnt].nCntPreAttack > ENEMY02_PREATTACK_TIME * 0.25)
							)
						{
							pVtx[0+4*nCnt].tex = D3DXVECTOR2( 0.5f, 0.5f);
							pVtx[1+4*nCnt].tex = D3DXVECTOR2( 0.75f, 0.5f);
							pVtx[2+4*nCnt].tex = D3DXVECTOR2( 0.5f, 1.0f);
							pVtx[3+4*nCnt].tex = D3DXVECTOR2( 0.75f, 1.0f);
						}

						//���[�V����04
						else
						{
							pVtx[0+4*nCnt].tex = D3DXVECTOR2( 0.75f, 0.5f);
							pVtx[1+4*nCnt].tex = D3DXVECTOR2( 1.0f, 0.5f);
							pVtx[2+4*nCnt].tex = D3DXVECTOR2( 0.75f, 1.0f);
							pVtx[3+4*nCnt].tex = D3DXVECTOR2( 1.0f, 1.0f);
						}
					}

					//�U�����[�V����
					else
					{
						pVtx[0+4*nCnt].tex = D3DXVECTOR2( 0.75f, 0.5f);
						pVtx[1+4*nCnt].tex = D3DXVECTOR2( 1.0f, 0.5f);
						pVtx[2+4*nCnt].tex = D3DXVECTOR2( 0.75f, 1.0f);
						pVtx[3+4*nCnt].tex = D3DXVECTOR2( 1.0f, 1.0f);
					}
				}

				else
				{
					//�U�����[�V��������
					if(g_Enemy02[nCnt].nCntPreAttack != 0)
					{
						//���[�V����01
						if(g_Enemy02[nCnt].nCntPreAttack > ENEMY02_PREATTACK_TIME * 0.25 * 3)
						{
							pVtx[0+4*nCnt].tex = D3DXVECTOR2( 0.25f, 0.5f);
							pVtx[1+4*nCnt].tex = D3DXVECTOR2( 0.0f, 0.5f);
							pVtx[2+4*nCnt].tex = D3DXVECTOR2( 0.25f, 1.0f);
							pVtx[3+4*nCnt].tex = D3DXVECTOR2( 0.0f, 1.0f);
						}

						//���[�V����02
						else if( (g_Enemy02[nCnt].nCntPreAttack <= ENEMY02_PREATTACK_TIME * 0.25 * 3) &&
								 (g_Enemy02[nCnt].nCntPreAttack > ENEMY02_PREATTACK_TIME * 0.25 * 2)
							)
						{
							pVtx[0+4*nCnt].tex = D3DXVECTOR2( 0.5f, 0.5f);
							pVtx[1+4*nCnt].tex = D3DXVECTOR2( 0.25f, 0.5f);
							pVtx[2+4*nCnt].tex = D3DXVECTOR2( 0.5f, 1.0f);
							pVtx[3+4*nCnt].tex = D3DXVECTOR2( 0.25f, 1.0f);
						}
						//���[�V����03
						else if( (g_Enemy02[nCnt].nCntPreAttack <= ENEMY02_PREATTACK_TIME * 0.25 * 2) &&
								 (g_Enemy02[nCnt].nCntPreAttack > ENEMY02_PREATTACK_TIME * 0.25)
							)
						{
							pVtx[0+4*nCnt].tex = D3DXVECTOR2( 0.75f, 0.5f);
							pVtx[1+4*nCnt].tex = D3DXVECTOR2( 0.5f, 0.5f);
							pVtx[2+4*nCnt].tex = D3DXVECTOR2( 0.75f, 1.0f);
							pVtx[3+4*nCnt].tex = D3DXVECTOR2( 0.5f, 1.0f);
						}
						//���[�V����04
						else
						{
							pVtx[0+4*nCnt].tex = D3DXVECTOR2( 1.0f, 0.5f);
							pVtx[1+4*nCnt].tex = D3DXVECTOR2( 0.75f, 0.5f);
							pVtx[2+4*nCnt].tex = D3DXVECTOR2( 1.0f, 1.0f);
							pVtx[3+4*nCnt].tex = D3DXVECTOR2( 0.75f, 1.0f);
						}
					}

					//�U�����[�V����
					else
					{
						pVtx[0+4*nCnt].tex = D3DXVECTOR2( 1.0f, 0.5f);
						pVtx[1+4*nCnt].tex = D3DXVECTOR2( 0.75f, 0.5f);
						pVtx[2+4*nCnt].tex = D3DXVECTOR2( 1.0f, 1.0f);
						pVtx[3+4*nCnt].tex = D3DXVECTOR2( 0.75f, 1.0f);
					}
				}
			}

			pVtx[0+4*nCnt].rhw = 1.0f;
			pVtx[1+4*nCnt].rhw = 1.0f;
			pVtx[2+4*nCnt].rhw = 1.0f;
			pVtx[3+4*nCnt].rhw = 1.0f;

			if(g_Enemy02[nCnt].bHit == false)
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
				g_Enemy02[nCnt].bHit = false;
			}
		}
	}
	g_pVtxBufferEnemy02Polygon->Unlock();                                     //�o�b�t�@�̃A�����b�N

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

void DrawEnemy02(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	pD3DDevice->SetStreamSource(0, g_pVtxBufferEnemy02Polygon, 0, sizeof(VERTEX_2D));              //VRAM����GPU�Ƀf�[�^���X�g���[��
	pD3DDevice->SetFVF(FVF_VERTEX2D);

	for (int nCnt = 0; nCnt < MAX_ENEMY02; nCnt++)
	{
		//�����G�͎g�p�̏�ԂȂ�
		if (g_Enemy02[nCnt].bUse == true)
		{
			//�w�i�̓S�����F���_�t�H�[�}�b�g�ݒ�
			pD3DDevice->SetTexture(0, g_pTextureEnemy02Polygon[0]);				 //�e�N�X�`���̐ݒ�
			pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt*4, NUM_POLYGON);                                            //�e�N�X�`���������̊m��
		}
	}
}

//�v���[���[�|���S���̕\���ʒuX,�|���S���̕\���ʒuY,�|���S���̕�,�|���S���̍���
void MakeVertexPolygonEnemy02(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx)
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
ENEMY02 *GetEnemy02(void)
{

	return &g_Enemy02[0];
}

//�g���G�̐���ݒ肷��
void SetEnemy02(float fPosX,float fPosY,float fSpeedX,float fDirect,float nEnemyNum)
{
	for (int nCnt = 0; nCnt < MAX_ENEMY02; nCnt++) 
	{
		if(g_Enemy02[nCnt].bUse == false)
		{
			g_Enemy02[nCnt].nNumEnemy = nEnemyNum;                   //�G�̔ԍ���ݒ�
			g_Enemy02[nCnt].fPosX = fPosX;
			g_Enemy02[nCnt].fPosY = fPosY;
			g_Enemy02[nCnt].fMoveX = fSpeedX;
			g_Enemy02[nCnt].nLife = 15;
			g_Enemy02[nCnt].fDirect = fDirect;
			g_Enemy02[nCnt].bUse = true;
			g_Enemy02[nCnt].bPre = false;
			g_Enemy02[nCnt].bInter = false;
			g_Enemy02[nCnt].bRecovery = false;
			g_Enemy02[nCnt].nCntPreAttack = ENEMY02_PREATTACK_TIME;
			g_Enemy02[nCnt].nCntBullet = 3;
			g_Enemy02[nCnt].nCntInterAttack = ENEMY02_INTERATTACK_TIME;
			g_Enemy02[nCnt].nCntRecoveryAttack = ENEMY02_RECOVERYATTACK_TIME;
			break;
		}
	}
}

//�e�̒��e�_�v�Z��
D3DXVECTOR2 BulletLanding(ENEMY02 enemy)
{
	float fYDistance = 0;               //�C�e�ƃv���[���[��Y���̋���
	D3DXVECTOR2 E_BulletSpeed;          //�G�̒e�̃X�s�[�h
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	fYDistance =  (pPlayer -> fPosY + PLAYER_HEIGHT*0.5) - (enemy.fPosY + BULLETENEMY02_DISTANCEY);                   //�v���[���[�ƖC�e��Y������ = �v���[���[Y���W - �C�eY���W

	//enemy02�̓v���[���[�̉E���ɂ���
	if(enemy.fPosX - pPlayer->fPosX_SUM > pPlayer -> fPosX)
	{
		E_BulletSpeed.x = ( (pPlayer -> fPosX + PLAYER_WIDTH*0.5) - (enemy.fPosX - pPlayer -> fPosX_SUM + BULLETENEMY02_DISTANCEX) ) / E_TIMETOTAL;     //�C�eX���X�s�[�h = ( �v���[���[X���W - �C�eX���W ) / ������
	}

	//enemy02�̓v���[���[�̍����ɂ���
	else
	{
		E_BulletSpeed.x = ( (pPlayer -> fPosX + PLAYER_WIDTH*0.5) - (enemy.fPosX - pPlayer -> fPosX_SUM - BULLETENEMY02_DISTANCEX + ENEMY02_WIDTH) ) / E_TIMETOTAL;     //�C�eX���X�s�[�h = ( �v���[���[X���W - �C�eX���W ) / ������
	}

	E_BulletSpeed.y = (fYDistance - 0.5*GRAVITY*E_TIMETOTAL*E_TIMETOTAL) /E_TIMETOTAL;                                                              //�C�eY���X�s�[�h�̌v�Z����: Y�������� = �C�eY���X�s�[�h*������ + 0.5*�d��*������^2           

	return E_BulletSpeed;
}
