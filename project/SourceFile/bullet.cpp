//-----------------------------------------------------------------------------------------
//�v���[���[,����Ȃǂ̏���[running02.cpp]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------------------
#include <time.h>
#include "main.h"
#include "bullet.h"
#include "bulletEffect.h"
#include "player.h"

//-----------------------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureBulletPolygon[NUM_BULLETTEXTURE] = {};           //�e�N�X�`���𑀍삷�邽�߂̃|�C���^(�e�N�X�`���o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferBulletPolygon = NULL;    //���_�o�b�t�@�𑀍삷�邽�߂̃|�C���^(���_�o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)

BULLET g_Bullet[MAX_BULLET];
BULLET_HITCHECK g_BulletHitcheck;                                            //�e�̕ӂ蔻��͈̓f�[�^
//-----------------------------------------------------------------------------------------
//�d�v��Ԋ֐��B

//�e�ϐ��̏�����
HRESULT InitBullet(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_2D *pVtx = NULL;                           //���_�\���̂̃|�C���^
	pD3DDevice = Get_Device();
	PLAYER *pPlayer = GetPlayer();
	 
	//���_�o�b�t�@�̐���
	if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_BULLETVERTEX*MAX_BULLET, D3DUSAGE_WRITEONLY, FVF_VERTEX2D, D3DPOOL_MANAGED, &g_pVtxBufferBulletPolygon, NULL)))
	{
		return E_FAIL;
	}

	//�e�̃e�N�X�`���擾
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, BULLETPOLYGON01_TEXTURENAME, &g_pTextureBulletPolygon[0])))
	{
		MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂����s���܂���", "�G���[", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	//�v���[���[�̃|���S��
	g_pVtxBufferBulletPolygon->Lock(0, 0, (void**)&pVtx, 0);                    //���z�A�h���X���擾���o�b�t�@�����b�N����
	MakeVertexPolygonPlayer(pD3DDevice, pVtx);

	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		//�v���[���[�̏��f�[�^�̏����l
		g_Bullet[nCnt].pos.x = pPlayer -> fPosX;
		g_Bullet[nCnt].pos.y = pPlayer -> fPosY;
		g_Bullet[nCnt].speed.x = 0;
		g_Bullet[nCnt].speed.y = 0;
		g_Bullet[nCnt].fAngle = 0;
		g_Bullet[nCnt].bUse = false;

		pVtx[0].pos = D3DXVECTOR3(g_Bullet[nCnt].pos.x, g_Bullet[nCnt].pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Bullet[nCnt].pos.x + BULLET_WIDTH, g_Bullet[nCnt].pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Bullet[nCnt].pos.x, g_Bullet[nCnt].pos.y + BULLET_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Bullet[nCnt].pos.x + BULLET_WIDTH, g_Bullet[nCnt].pos.y + BULLET_HEIGHT, 0.0f);

		pVtx += 4;
	}

	g_BulletHitcheck.Up = BULLET_DISTANCEY;
	g_BulletHitcheck.Bottom = BULLET_HEIGHT+BULLET_DISTANCEY;
	g_BulletHitcheck.Left = PLAYER_WIDTH/2-BULLET_DISTANCEX;
	g_BulletHitcheck.Right = BULLET_WIDTH+PLAYER_WIDTH/2-BULLET_DISTANCEX;

	g_pVtxBufferBulletPolygon->Unlock();                     //�o�b�t�@�̃A�����b�N
	return S_OK;
}
//�I������
void UninitBullet(void)
{
	// �e�N�X�`���C���^�[�t�F�C�X�̕Еt��
	for (int nCnt = 0; nCnt < NUM_BULLETTEXTURE; nCnt++)
	{
		SAFE_RELEASE(g_pTextureBulletPolygon[nCnt]);
	}

	//���_�o�b�t�@�C���^�[�t�F�C�X�̕Еt��
	SAFE_RELEASE(g_pVtxBufferBulletPolygon);
}

//�v���[���[�f�[�^�̍X�V����
void UpdateBullet(void)
{
	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		//�����G�͎g�p�̏�ԂȂ�
		if (g_Bullet[nCnt].bUse == true)
		{
		 	g_Bullet[nCnt].pos.x +=g_Bullet[nCnt].speed.x; 
			g_Bullet[nCnt].pos.y +=g_Bullet[nCnt].speed.y;
			g_Bullet[nCnt].nLife--;

			if(g_Bullet[nCnt].nLife < 0)
			{
				g_Bullet[nCnt].nLife = 0;
				g_Bullet[nCnt].bUse = false;
			}

			float nCenterX = g_Bullet[nCnt].pos.x + BULLET_WIDTH;
			float nCenterY = g_Bullet[nCnt].pos.y + BULLET_HEIGHT;

			//��ʔ͈͊O��苗�����痣���Ɩ��g�p�ɂ���
			if( (nCenterX < -200) || (nCenterX > SCREEN_WIDTH + 200) || (nCenterY < -100) || (nCenterY > SCREEN_HEIGHT + 100) )
			{
				g_Bullet[nCnt].nLife = 0;
				g_Bullet[nCnt].bUse = false;
			}
		}
	}
	UpdatePolygonBullet();
}

//�v���[���[�|���S���̃f�[�^�X�V
void UpdatePolygonBullet(void)
{
	static int nRunCnt = 0;                                       //�����j���O�}���e�N�X�`���̐؂�ւ��J�E���^�[
	static int nActionFlame = 0;                                  //�����j���O�}���̓�����x�����邽�߂̃J�E���^�[
	VERTEX_2D *pVtx = NULL;
	LPDIRECT3DDEVICE9 pD3DDevice;
	PLAYER *pPlayer = GetPlayer();
	pD3DDevice = Get_Device();


	g_pVtxBufferBulletPolygon -> Lock(0, 0, (void**)&pVtx, 0);          //���z�A�h���X���擾���o�b�t�@�����b�N����

	MakeVertexPolygonBullet(pD3DDevice, pVtx);
	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		if(g_Bullet[nCnt].bUse == true)
		{
			
			SpriteRorateBullet(g_Bullet[nCnt],pVtx+nCnt*4);

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

			//�o���b�g�̃G�t�F�N�g�̏���
			SetBulletEffect(g_Bullet[nCnt].pos.x + BULLET_WIDTH/2+PLAYER_WIDTH/2-BULLET_DISTANCEX,g_Bullet[nCnt].pos.y + BULLET_HEIGHT/2+BULLET_DISTANCEY,BULLETLIFE,g_Bullet[nCnt].fAngle);
		}
	}

	g_pVtxBufferBulletPolygon -> Unlock();                                     //�o�b�t�@�̃A�����b�N
}

void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	//�w�i�̓S�����F���_�t�H�[�}�b�g�ݒ�
	pD3DDevice->SetStreamSource(0, g_pVtxBufferBulletPolygon, 0, sizeof(VERTEX_2D));              //VRAM����GPU�Ƀf�[�^���X�g���[��
	pD3DDevice->SetFVF(FVF_VERTEX2D);
	pD3DDevice->SetTexture(0, g_pTextureBulletPolygon[0]);				 //�e�N�X�`���̐ݒ�

	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		//�����e�͎g�p�̏�ԂȂ�
		if (g_Bullet[nCnt].bUse == true)
		{
			pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt*4, NUM_POLYGON);                                            //�e�N�X�`���������̊m��
		}
	}
}

//�v���[���[�|���S���̕\���ʒuX,�|���S���̕\���ʒuY,�|���S���̕�,�|���S���̍���
void MakeVertexPolygonBullet(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx)
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

//�e���擾
BULLET *GetBullet(void)
{

	return &g_Bullet[0];
}

BULLET_HITCHECK *GetBulletHitcheck(void)
{

	return &g_BulletHitcheck;
}

//�e��ݒu
void SetBullet(float fPosX,float fPosY,float fSpeedX,float fSpeedY,int nLife,float fAngle)
{
	for(int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		if(g_Bullet[nCnt].bUse == false)
		{
			g_Bullet[nCnt].pos.x = fPosX;
			g_Bullet[nCnt].pos.y = fPosY;
			g_Bullet[nCnt].speed.x = fSpeedX;
			g_Bullet[nCnt].speed.y = fSpeedY;
			g_Bullet[nCnt].color = D3DXCOLOR(0.0f,0.0f,0.0f,1.0f);
			g_Bullet[nCnt].nLife = nLife;
			g_Bullet[nCnt].fAngle = fAngle;
			g_Bullet[nCnt].bUse = true;
			break;
		}
	}
}

void SpriteRorateBullet (BULLET Bbullet,VERTEX_2D *pVtx)
{
	double X1, Y1, X2, Y2, X3, Y3, X4, Y4;             //����,�E�����W
	double Xc1, Yc1, Xc2, Yc2, Xc3, Yc3, Xc4, Yc4;         //��]������̍��W
	double XC, YC;                                   //���S���W

	X1 = Bbullet.pos.x + PLAYER_WIDTH/2-BULLET_DISTANCEX;
	Y1 = Bbullet.pos.y+BULLET_DISTANCEY;
	X2 = Bbullet.pos.x+BULLET_WIDTH+PLAYER_WIDTH/2-BULLET_DISTANCEX;
	Y2 = Bbullet.pos.y+BULLET_DISTANCEY;
	X3 = Bbullet.pos.x+PLAYER_WIDTH/2-BULLET_DISTANCEX;
	Y3 = Bbullet.pos.y+BULLET_HEIGHT+BULLET_DISTANCEY;
	X4 = Bbullet.pos.x+BULLET_WIDTH+PLAYER_WIDTH/2-BULLET_DISTANCEX;
	Y4 = Bbullet.pos.y+BULLET_HEIGHT+BULLET_DISTANCEY;

	XC = Bbullet.pos.x + BULLET_WIDTH/2+PLAYER_WIDTH/2-BULLET_DISTANCEX;
	YC = Bbullet.pos.y + BULLET_HEIGHT/2+BULLET_DISTANCEY;

	Xc1 = (X1 - XC)*cos(Bbullet.fAngle) - (Y1 - YC)*sin(Bbullet.fAngle) + XC;
	Yc1 = (X1 - XC)*sin(Bbullet.fAngle) + (Y1 - YC)*cos(Bbullet.fAngle) + YC;

	Xc2 = (X2 - XC)*cos(Bbullet.fAngle) - (Y2 - YC)*sin(Bbullet.fAngle) + XC;
	Yc2 = (X2 - XC)*sin(Bbullet.fAngle) + (Y2 - YC)*cos(Bbullet.fAngle) + YC;

	Xc3 = (X3 - XC)*cos(Bbullet.fAngle) - (Y3 - YC)*sin(Bbullet.fAngle) + XC;
	Yc3 = (X3 - XC)*sin(Bbullet.fAngle) + (Y3 - YC)*cos(Bbullet.fAngle) + YC;

	Xc4 = (X4 - XC)*cos(Bbullet.fAngle) - (Y4 - YC)*sin(Bbullet.fAngle) + XC;
	Yc4 = (X4 - XC)*sin(Bbullet.fAngle) + (Y4 - YC)*cos(Bbullet.fAngle) + YC;

	pVtx[0].pos = D3DXVECTOR3(Xc1,Yc1,0.0f);
	pVtx[1].pos = D3DXVECTOR3(Xc2,Yc2,0.0f);
	pVtx[2].pos = D3DXVECTOR3(Xc3,Yc3,0.0f);
	pVtx[3].pos = D3DXVECTOR3(Xc4,Yc4,0.0f);
}