#ifdef _DEBUG
//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
//-----------------------------------------------------------------------------------------
#include <Windows.h>
#include "main.h"
#include "debug_Hitcheck.h"
#include "player.h"
#include "enemy01.h"
#include "enemy02.h"
#include "enemyrobot.h"
#include "bullet.h"
#include "bulletEnemy02.h"
#include "fade.h"
#include "input.h"

//-----------------------------------------------------------------------------------------
//�\���̐錾
//-----------------------------------------------------------------------------------------
typedef struct
{
	PLAYER *pPlayer;
	ENEMY01 *pEnemy01;
	ENEMY02 *pEnemy02;
	ENEMYROBOT *pEnemyRobot;
	BULLET *pBullet;
	BULLET_HITCHECK *pBulletHitcheck;
	BULLETENEMY02 *pEnemyBullet;
}PLAYDATA;

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------------------
void DataSet(void);

//-----------------------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferHitcheckPolygon[NUM_HITCHECKPOLYGON] = {};    //���_�o�b�t�@�𑀍삷�邽�߂̃|�C���^(���_�o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^) 
void MakeVertexPolygonHitcheck(LPDIRECT3DDEVICE9 pDevice,VERTEX_2DF *pVtx);       //�|���S���ݒ�
D3DCOLOR g_ColorHitcheck;                                                         //�F
HITCHECK g_HitCheck[NUM_HITCHECKPOLYGON];
PLAYDATA g_PlayData;

////////////////////////////////////////////////
//�|���S��(���_)�̏�����
////////////////////////////////////////////////
HRESULT InitHitcheck(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_2DF *pVtx = NULL;                           //���_�\���̂̃|�C���^
	pD3DDevice = Get_Device();

	//���_�o�b�t�@�̐���
	if(FAILED(pD3DDevice -> CreateVertexBuffer(sizeof(VERTEX_2DF)*NUM_HITCHECKVERTEX*NUM_HITCHECKPOLYGON,D3DUSAGE_WRITEONLY,FVF_VERTEX2D,D3DPOOL_MANAGED,&g_pVtxBufferHitcheckPolygon[0],NULL)))
	{
		return E_FAIL;
	}

	//�w�i�̋�|���S��
	g_pVtxBufferHitcheckPolygon[0] -> Lock(0,0,(void**)&pVtx,0);                    //���z�A�h���X���擾���o�b�t�@�����b�N����
	MakeVertexPolygonHitcheck(pD3DDevice,pVtx);
	for(int nCnt = 0; nCnt < NUM_HITCHECKPOLYGON;nCnt++)
	{
		g_HitCheck[nCnt].Up = 0.0f;
		g_HitCheck[nCnt].Bottom = 0.0f;
		g_HitCheck[nCnt].Left = 0.0f;
		g_HitCheck[nCnt].Right = 0.0f;
		g_HitCheck[nCnt].bUse = false;

		pVtx[0 + 4*nCnt].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
		pVtx[1 + 4*nCnt].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
		pVtx[2 + 4*nCnt].pos = D3DXVECTOR3(0.0f, 0.0f,0.0f);
		pVtx[3 + 4*nCnt].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	}

	g_pVtxBufferHitcheckPolygon[0] -> Unlock();                                     //�o�b�t�@�̃A�����b�N

	g_ColorHitcheck = D3DXCOLOR(0.0f,0.0f,0.0f,1.0f);                               //�t�F�[�h�J���[�̏����ݒ�

	DataSet();                                                                      //�e�f�[�^�ւ̃����O

	return S_OK;
}

//�|���S���̍X�V
void UpdateHitcheck(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice = Get_Device();
	PLAYER *pPlayer = GetPlayer();
	VERTEX_2DF *pVtx = NULL;                           //���_�\���̂̃|�C���^

	g_ColorHitcheck = D3DCOLOR_RGBA(0,255,0,100);

	//----------------
	//�G�̃f�[�^���擾
	//----------------
	//Enemy01
	for (int nCnt = 0; nCnt < MAX_ENEMY01; nCnt++)
	{
		if ( (g_PlayData.pEnemy01+nCnt) -> bUse == true)
		{
			SetHitCheck( (g_PlayData.pEnemy01+nCnt) -> fPosY + ENEMY01_TOP,										           //���̍��W
				         (g_PlayData.pEnemy01+nCnt) -> fPosY + ENEMY01_HEIGHT - ENEMY01_HEIGHT_CHECK,			           //���̍��W
						 (g_PlayData.pEnemy01+nCnt) -> fPosX - pPlayer->fPosX_SUM + ENEMY01_HORRIZON,					   //���̍��W
						 (g_PlayData.pEnemy01+nCnt) -> fPosX - pPlayer->fPosX_SUM + ENEMY01_WIDTH - ENEMY01_HORRIZON);	   //���̍��W
		}
	}

	//Enemy02
	for (int nCnt = 0; nCnt < MAX_ENEMY02; nCnt++)
	{
		if ( (g_PlayData.pEnemy02+nCnt) -> bUse == true)
		{
			SetHitCheck( (g_PlayData.pEnemy02+nCnt) -> fPosY + ENEMY02_TOP,										           //���̍��W
				         (g_PlayData.pEnemy02+nCnt) -> fPosY + ENEMY02_HEIGHT - ENEMY02_HEIGHT_CHECK,			           //���̍��W
						 (g_PlayData.pEnemy02+nCnt) -> fPosX - pPlayer->fPosX_SUM + ENEMY02_HORRIZON,					   //���̍��W
						 (g_PlayData.pEnemy02+nCnt) -> fPosX - pPlayer->fPosX_SUM + ENEMY02_WIDTH - ENEMY02_HORRIZON);	   //���̍��W
		}
	}

	//Bullet
	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		if ( (g_PlayData.pBullet+nCnt) -> bUse == true)
		{
			SetHitCheck( (g_PlayData.pBullet+nCnt) -> pos.y + g_PlayData.pBulletHitcheck -> Up,					   //���̍��W
				         (g_PlayData.pBullet+nCnt) -> pos.y + g_PlayData.pBulletHitcheck -> Bottom,			       //���̍��W
						 (g_PlayData.pBullet+nCnt) -> pos.x + g_PlayData.pBulletHitcheck -> Left,					   //���̍��W
						 (g_PlayData.pBullet+nCnt) -> pos.x + g_PlayData.pBulletHitcheck -> Right);	               //���̍��W
		}
	}

	//EnemyBullet
	for (int nCnt = 0; nCnt < MAX_BULLETENEMY02; nCnt++)
	{
		if ( (g_PlayData.pEnemyBullet+nCnt) -> bUse == true)
		{
			SetHitCheck( (g_PlayData.pEnemyBullet+nCnt) -> pos.y - g_PlayData.pEnemyBullet -> fRadius,				   //���̍��W
				         (g_PlayData.pEnemyBullet+nCnt) -> pos.y + g_PlayData.pEnemyBullet -> fRadius,			       //���̍��W
						 (g_PlayData.pEnemyBullet+nCnt) -> pos.x - g_PlayData.pEnemyBullet -> fRadius - g_PlayData.pPlayer->fPosX_SUM,				   //���̍��W
						 (g_PlayData.pEnemyBullet+nCnt) -> pos.x + g_PlayData.pEnemyBullet -> fRadius - g_PlayData.pPlayer->fPosX_SUM);	               //���̍��W
		}
	}

	//player
	if(g_PlayData.pPlayer -> bUse == true)
	{
		SetHitCheck( (g_PlayData.pPlayer) -> fPosY + PLAYER_TOP,										       //���̍��W
			         (g_PlayData.pPlayer) -> fPosY + PLAYER_HEIGHT - PLAYER_HEIGHT_CHECK,			           //���̍��W
			         (g_PlayData.pPlayer) -> fPosX + PLAYER_HORRIZON,					                       //���̍��W
			         (g_PlayData.pPlayer) -> fPosX + PLAYER_WIDTH - PLAYER_HORRIZON);	                       //���̍��W
	}

	//enemyrobot_knife
	for (int nCnt = 0; nCnt < MAX_ENEMYROBOT; nCnt++)
	{
		if( (g_PlayData.pEnemyRobot+nCnt) -> bUse == true)
		{
			if( (g_PlayData.pEnemyRobot+nCnt) -> nType == 2)
			{
				if((g_PlayData.pEnemyRobot+nCnt) -> fDirect == 0.1f)
				{
					SetHitCheck( (g_PlayData.pEnemyRobot+nCnt)->fPosY+KNIFEENEMYROBOT_DISTANCEY_B,										                                   //���̍��W
								 (g_PlayData.pEnemyRobot+nCnt)->fPosY+KNIFEENEMYROBOT_DISTANCEY_T,			                                                               //���̍��W
								 (g_PlayData.pEnemyRobot+nCnt)->fPosX-KNIFEENEMYROBOT_DISTANCEX_R-g_PlayData.pPlayer->fPosX_SUM+ENEMYROBOT_WIDTH,					       //���̍��W
								 (g_PlayData.pEnemyRobot+nCnt)->fPosX-KNIFEENEMYROBOT_DISTANCEX_L-g_PlayData.pPlayer->fPosX_SUM+ENEMYROBOT_WIDTH );	                       //���̍��W
				}

				else
				{
					SetHitCheck( (g_PlayData.pEnemyRobot+nCnt)->fPosY+KNIFEENEMYROBOT_DISTANCEY_B,										                                   //���̍��W
								 (g_PlayData.pEnemyRobot+nCnt)->fPosY+KNIFEENEMYROBOT_DISTANCEY_T,			                                                               //���̍��W
								 (g_PlayData.pEnemyRobot+nCnt)->fPosX+KNIFEENEMYROBOT_DISTANCEX_R-g_PlayData.pPlayer->fPosX_SUM,					       //���̍��W
								 (g_PlayData.pEnemyRobot+nCnt)->fPosX+KNIFEENEMYROBOT_DISTANCEX_L-g_PlayData.pPlayer->fPosX_SUM);	                       //���̍��W
				}
			}
		}
	}

	//�t�F�[�h�̌v�Z
	g_pVtxBufferHitcheckPolygon[0]->Lock(0, 0, (void**)&pVtx, 0);       //���z�A�h���X���擾���o�b�t�@�����b�N����

	MakeVertexPolygonHitcheck(pD3DDevice, pVtx);

	for(int nCnt = 0; nCnt < NUM_HITCHECKPOLYGON;nCnt++)
	{
		if(g_HitCheck[nCnt].bUse == true)
		{
			pVtx[0+4*nCnt].pos = D3DXVECTOR3(g_HitCheck[nCnt].Left,g_HitCheck[nCnt].Up,0.0f);
			pVtx[1+4*nCnt].pos = D3DXVECTOR3(g_HitCheck[nCnt].Right,g_HitCheck[nCnt].Up,0.0f);
			pVtx[2+4*nCnt].pos = D3DXVECTOR3(g_HitCheck[nCnt].Left,g_HitCheck[nCnt].Bottom,0.0f);
			pVtx[3+4*nCnt].pos = D3DXVECTOR3(g_HitCheck[nCnt].Right,g_HitCheck[nCnt].Bottom,0.0f);

			pVtx[0+4*nCnt].rhw = 1.0f;
			pVtx[1+4*nCnt].rhw = 1.0f;
			pVtx[2+4*nCnt].rhw = 1.0f;
			pVtx[3+4*nCnt].rhw = 1.0f;

			pVtx[0+4*nCnt].color = g_ColorHitcheck;
			pVtx[1+4*nCnt].color = g_ColorHitcheck;
			pVtx[2+4*nCnt].color = g_ColorHitcheck;
			pVtx[3+4*nCnt].color = g_ColorHitcheck;

			pVtx[0+4*nCnt].tex = D3DXVECTOR2(0.0f,0.0f);
			pVtx[1+4*nCnt].tex = D3DXVECTOR2(1.0f,0.0f);
			pVtx[2+4*nCnt].tex = D3DXVECTOR2(0.0f,1.0f);
			pVtx[3+4*nCnt].tex = D3DXVECTOR2(1.0f,1.0f);
		}
	}
	g_pVtxBufferHitcheckPolygon[0]->Unlock();                                     //�o�b�t�@�̃A�����b�N�g
}

////////////////////////////////////////////////
//�|���S���̕`��
////////////////////////////////////////////////
void DrawHitcheck(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	//�w�i�F���_�t�H�[�}�b�g�ݒ�
	pD3DDevice -> SetStreamSource(0,g_pVtxBufferHitcheckPolygon[0],0,sizeof(VERTEX_2DF));                   //VRAM����GPU�Ƀf�[�^���X�g���[��
	pD3DDevice -> SetFVF(FVF_VERTEX2D);
	pD3DDevice -> SetTexture(0,NULL);												                       //�e�N�X�`���̐ݒ�

	//�A���t�@�u�����f�B���O���s��
	pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	//���ߏ����s��
	pD3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//�����������s��
	pD3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	//�`��
	for(int nCnt = 0; nCnt < NUM_HITCHECKPOLYGON;nCnt++)
	{
		if(g_HitCheck[nCnt].bUse == true)
		{
			pD3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,4*nCnt,NUM_POLYGON);                                      //�e�N�X�`���`��
			g_HitCheck[nCnt].bUse = false;
		}
	}

}

void UninitHitcheck(void)
{	
	//���_�o�b�t�@�C���^�[�t�F�C�X�̕Еt��
	for (int nCnt = 0; nCnt < NUM_HITCHECKPOLYGON; nCnt++)
	{
		SAFE_RELEASE(g_pVtxBufferHitcheckPolygon[nCnt]);
	}
}

//�w�i�|���S���̕\���ʒuX,�|���S���̕\���ʒuY,�|���S���̕�,�|���S���̍���
void MakeVertexPolygonHitcheck(LPDIRECT3DDEVICE9 pDevice,VERTEX_2DF *pVtx)
{
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[1].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[2].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[3].color = D3DCOLOR_RGBA(255,255,255,255);


	//�|���S���`�悷��O�Ƀe�N�X�`�����Z�b�g
	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);
}

//�`�F�b�N�{�b�N�X�̐ݒu
void SetHitCheck(float Up,float Bottom,float Left,float Right)
{
	for(int nCnt = 0; nCnt < NUM_HITCHECKPOLYGON;nCnt++)
	{
		if(g_HitCheck[nCnt].bUse == false)
		{
			g_HitCheck[nCnt].Up = Up;
			g_HitCheck[nCnt].Bottom = Bottom;
			g_HitCheck[nCnt].Left = Left;
			g_HitCheck[nCnt].Right = Right;
			g_HitCheck[nCnt].bUse = true;
			break;
		}
	}
}

//�f�[�^���Z�b�g
void DataSet(void)
{
	g_PlayData.pPlayer = GetPlayer();                         //�v���[���[�f�[�^�Q�b�g
	g_PlayData.pEnemy01 = GetEnemy01();                       //Enemy01�̃f�[�^�Q�b�g
	g_PlayData.pEnemy02 = GetEnemy02();                       //Enemy02�̃f�[�^�Q�b�g
	g_PlayData.pBullet = GetBullet();                         //�e�̃f�[�^���Q�b�g
	g_PlayData.pBulletHitcheck = GetBulletHitcheck();         //�e�̓��蔻��͈�
	g_PlayData.pEnemyBullet = GetBulletEnemy02();             //�G�̒e�̃f�[�^���Q�b�g
	g_PlayData.pEnemyRobot = GetEnemyRobot();
}

#endif
