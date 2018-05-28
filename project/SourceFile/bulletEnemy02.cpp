//-----------------------------------------------------------------------------------------
//�v���[���[,����Ȃǂ̏���[running02.cpp]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------------------
#include <time.h>
#include "main.h"
#include "bulletEnemy02.h"
#include "player.h"

//-----------------------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureBulletEnemy02Polygon[NUM_BULLETENEMY02TEXTURE] = {};           //�e�N�X�`���𑀍삷�邽�߂̃|�C���^(�e�N�X�`���o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferBulletEnemy02Polygon = NULL;    //���_�o�b�t�@�𑀍삷�邽�߂̃|�C���^(���_�o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)

BULLETENEMY02 g_BulletEnemy02[MAX_BULLETENEMY02];

//-----------------------------------------------------------------------------------------
//�d�v��Ԋ֐��B

//�e�ϐ��̏�����
HRESULT InitBulletEnemy02(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_2D *pVtx = NULL;                           //���_�\���̂̃|�C���^
	pD3DDevice = Get_Device();
	PLAYER *pPlayer = GetPlayer();
	 
	//���_�o�b�t�@�̐���
	if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_BULLETENEMY02VERTEX*MAX_BULLETENEMY02, D3DUSAGE_WRITEONLY, FVF_VERTEX2D, D3DPOOL_MANAGED, &g_pVtxBufferBulletEnemy02Polygon, NULL)))
	{
		return E_FAIL;
	}

	//�e�̃e�N�X�`���擾
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, BULLETENEMY02POLYGON01_TEXTURENAME, &g_pTextureBulletEnemy02Polygon[0])))
	{
		MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂����s���܂���", "�G���[", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	//�v���[���[�̃|���S��
	g_pVtxBufferBulletEnemy02Polygon->Lock(0, 0, (void**)&pVtx, 0);                    //���z�A�h���X���擾���o�b�t�@�����b�N����
	MakeVertexPolygonPlayer(pD3DDevice, pVtx);

	for (int nCnt = 0; nCnt < MAX_BULLETENEMY02; nCnt++)
	{
		//�v���[���[�̏��f�[�^�̏����l
		g_BulletEnemy02[nCnt].pos.x = pPlayer -> fPosX;
		g_BulletEnemy02[nCnt].pos.y = pPlayer -> fPosY;
		g_BulletEnemy02[nCnt].speed.x = 0;
		g_BulletEnemy02[nCnt].speed.y = 0;
		g_BulletEnemy02[nCnt].fAngle = 0;
		g_BulletEnemy02[nCnt].fRadius = 12;
		g_BulletEnemy02[nCnt].nUnit = 0;
		g_BulletEnemy02[nCnt].bUse = false;

		pVtx[0].pos = D3DXVECTOR3(g_BulletEnemy02[nCnt].pos.x, g_BulletEnemy02[nCnt].pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_BulletEnemy02[nCnt].pos.x + BULLETENEMY02_WIDTH, g_BulletEnemy02[nCnt].pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_BulletEnemy02[nCnt].pos.x, g_BulletEnemy02[nCnt].pos.y + BULLETENEMY02_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_BulletEnemy02[nCnt].pos.x + BULLETENEMY02_WIDTH, g_BulletEnemy02[nCnt].pos.y + BULLETENEMY02_HEIGHT, 0.0f);

		pVtx += 4;
	}

	g_pVtxBufferBulletEnemy02Polygon->Unlock();                     //�o�b�t�@�̃A�����b�N
	return S_OK;
}
//�I������
void UninitBulletEnemy02(void)
{
	// �e�N�X�`���C���^�[�t�F�C�X�̕Еt��
	for (int nCnt = 0; nCnt < NUM_BULLETENEMY02TEXTURE; nCnt++)
	{
		SAFE_RELEASE(g_pTextureBulletEnemy02Polygon[nCnt]);
	}

	//���_�o�b�t�@�C���^�[�t�F�C�X�̕Еt��
	SAFE_RELEASE(g_pVtxBufferBulletEnemy02Polygon);
}

//�v���[���[�f�[�^�̍X�V����
void UpdateBulletEnemy02(void)
{
	PLAYER *pPlayer = GetPlayer();                //�v���[���[�X�e�[�^�X�擾

	for (int nCnt = 0; nCnt < MAX_BULLETENEMY02; nCnt++)
	{
		//�����G�͎g�p�̏�ԂȂ�
		if (g_BulletEnemy02[nCnt].bUse == true)
		{
			//-----------------------------------------------------------
			//�G�̒e�̓���̋��ʕ���
			//-----------------------------------------------------------
			//�X�s�[�h�̂ɂ���č��W�̌v�Z
			g_BulletEnemy02[nCnt].pos.x +=g_BulletEnemy02[nCnt].speed.x; 
			g_BulletEnemy02[nCnt].pos.y +=g_BulletEnemy02[nCnt].speed.y;

			//�G�̒e�̃��C�t�̌v�Z
			g_BulletEnemy02[nCnt].nLife--;

			if(g_BulletEnemy02[nCnt].nLife < 0)
			{
				g_BulletEnemy02[nCnt].nLife = 0;
				g_BulletEnemy02[nCnt].bUse = false;
			}

			//-----------------------------------------------------------
			//Enemy01�̒e�̓���
			//-----------------------------------------------------------
			if(g_BulletEnemy02[nCnt].nUnit == 1)
			{

			}

			//-----------------------------------------------------------
			//Enemy02�̒e�̓���
			//-----------------------------------------------------------
			else if(g_BulletEnemy02[nCnt].nUnit == 2)
			{
				g_BulletEnemy02[nCnt].speed.y += GRAVITY;                  //���t���[���d�͉��Z
			}

			//-----------------------------------------------------------
			//EnemyRobot�̒e�̓���
			//-----------------------------------------------------------
			else
			{
				
			}

			//-----------------------------------------------------------
			//�G�̒e�̓���̋��ʕ���
			//-----------------------------------------------------------
			float nCenterX = g_BulletEnemy02[nCnt].pos.x + BULLETENEMY02_WIDTH - pPlayer -> fPosX_SUM;
			float nCenterY = g_BulletEnemy02[nCnt].pos.y + BULLETENEMY02_HEIGHT;

			//��ʔ͈͊O�o���疢�g�p�ɂ���
			if( (nCenterX < -200) || (nCenterX > 1200) || (nCenterY < -100) || (nCenterY > 900) )
			{
				g_BulletEnemy02[nCnt].nLife = 0;
				g_BulletEnemy02[nCnt].bUse = false;
			}
		}
	}
	UpdatePolygonBulletEnemy02();
}

//�v���[���[�|���S���̃f�[�^�X�V
void UpdatePolygonBulletEnemy02(void)
{
	static int nRunCnt = 0;                                       //�����j���O�}���e�N�X�`���̐؂�ւ��J�E���^�[
	static int nActionFlame = 0;                                  //�����j���O�}���̓�����x�����邽�߂̃J�E���^�[
	VERTEX_2D *pVtx = NULL;
	LPDIRECT3DDEVICE9 pD3DDevice;
	PLAYER *pPlayer = GetPlayer();
	pD3DDevice = Get_Device();

	g_pVtxBufferBulletEnemy02Polygon -> Lock(0, 0, (void**)&pVtx, 0);          //���z�A�h���X���擾���o�b�t�@�����b�N����

	MakeVertexPolygonBulletEnemy02(pD3DDevice, pVtx);
	for (int nCnt = 0; nCnt < MAX_BULLETENEMY02; nCnt++)
	{
		if(g_BulletEnemy02[nCnt].bUse == true)
		{
			pVtx[0+nCnt*4].pos = D3DXVECTOR3(g_BulletEnemy02[nCnt].pos.x - pPlayer -> fPosX_SUM - g_BulletEnemy02[nCnt].fRadius, g_BulletEnemy02[nCnt].pos.y - g_BulletEnemy02[nCnt].fRadius, 0.0f);
			pVtx[1+nCnt*4].pos = D3DXVECTOR3(g_BulletEnemy02[nCnt].pos.x - pPlayer -> fPosX_SUM + g_BulletEnemy02[nCnt].fRadius, g_BulletEnemy02[nCnt].pos.y - g_BulletEnemy02[nCnt].fRadius, 0.0f);
			pVtx[2+nCnt*4].pos = D3DXVECTOR3(g_BulletEnemy02[nCnt].pos.x - pPlayer -> fPosX_SUM - g_BulletEnemy02[nCnt].fRadius, g_BulletEnemy02[nCnt].pos.y + g_BulletEnemy02[nCnt].fRadius, 0.0f);
			pVtx[3+nCnt*4].pos = D3DXVECTOR3(g_BulletEnemy02[nCnt].pos.x - pPlayer -> fPosX_SUM + g_BulletEnemy02[nCnt].fRadius, g_BulletEnemy02[nCnt].pos.y + g_BulletEnemy02[nCnt].fRadius, 0.0f);

			pVtx[0+nCnt*4].rhw = 1.0f;
			pVtx[1+nCnt*4].rhw = 1.0f;
			pVtx[2+nCnt*4].rhw = 1.0f;
			pVtx[3+nCnt*4].rhw = 1.0f;

			pVtx[0+nCnt*4].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1+nCnt*4].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2+nCnt*4].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3+nCnt*4].color = D3DCOLOR_RGBA(255, 255, 255, 255);

			pVtx[0+nCnt*4].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1+nCnt*4].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2+nCnt*4].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3+nCnt*4].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
	}

	g_pVtxBufferBulletEnemy02Polygon -> Unlock();                                     //�o�b�t�@�̃A�����b�N
}

void DrawBulletEnemy02(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	//�w�i�̓S�����F���_�t�H�[�}�b�g�ݒ�
	pD3DDevice->SetStreamSource(0, g_pVtxBufferBulletEnemy02Polygon, 0, sizeof(VERTEX_2D));              //VRAM����GPU�Ƀf�[�^���X�g���[��
	pD3DDevice->SetFVF(FVF_VERTEX2D);
	pD3DDevice->SetTexture(0, g_pTextureBulletEnemy02Polygon[0]);				 //�e�N�X�`���̐ݒ�

	for (int nCnt = 0; nCnt < MAX_BULLETENEMY02; nCnt++)
	{
		//�����e�͎g�p�̏�ԂȂ�
		if (g_BulletEnemy02[nCnt].bUse == true)
		{
			pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt*4, NUM_POLYGON);                                            //�e�N�X�`���������̊m��
		}
	}
}

//�v���[���[�|���S���̕\���ʒuX,�|���S���̕\���ʒuY,�|���S���̕�,�|���S���̍���
void MakeVertexPolygonBulletEnemy02(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx)
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
BULLETENEMY02 *GetBulletEnemy02(void)
{

	return &g_BulletEnemy02[0];
}

//�e��ݒu
void SetBulletEnemy02(int nUnit,float fPosX,float fPosY,float fSpeedX,float fSpeedY,int nLife,float fAngle,float fRadius)
{
	for(int nCnt = 0; nCnt < MAX_BULLETENEMY02; nCnt++)
	{
		if(g_BulletEnemy02[nCnt].bUse == false)
		{
			g_BulletEnemy02[nCnt].nUnit = nUnit;
			g_BulletEnemy02[nCnt].pos.x = fPosX;
			g_BulletEnemy02[nCnt].pos.y = fPosY;
			g_BulletEnemy02[nCnt].speed.x = fSpeedX;
			g_BulletEnemy02[nCnt].speed.y = fSpeedY;
			g_BulletEnemy02[nCnt].color = D3DXCOLOR(0.0f,0.0f,0.0f,1.0f);
			g_BulletEnemy02[nCnt].nLife = nLife;
			g_BulletEnemy02[nCnt].fAngle = fAngle;
			g_BulletEnemy02[nCnt].fRadius = fRadius;
			g_BulletEnemy02[nCnt].bUse = true;
			break;
		}
	}
}
