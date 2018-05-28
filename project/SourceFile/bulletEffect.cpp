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
//-----------------------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureBulletEffectPolygon[NUM_BULLETEFFECTTEXTURE] = {};           //�e�N�X�`���𑀍삷�邽�߂̃|�C���^(�e�N�X�`���o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferBulletEffectPolygon = NULL;    //���_�o�b�t�@�𑀍삷�邽�߂̃|�C���^(���_�o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)

BULLETEFFECT g_BulletEffect[MAX_BULLETEFFECT];

//-----------------------------------------------------------------------------------------
//�d�v��Ԋ֐��B

//�e�ϐ��̏�����
HRESULT InitBulletEffect(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_2D *pVtx = NULL;                           //���_�\���̂̃|�C���^
	pD3DDevice = Get_Device();
	PLAYER *pPlayer = GetPlayer();
	 
	//���_�o�b�t�@�̐���
	if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_BULLETEFFECTVERTEX*MAX_BULLETEFFECT, D3DUSAGE_WRITEONLY, FVF_VERTEX2D, D3DPOOL_MANAGED, &g_pVtxBufferBulletEffectPolygon, NULL)))
	{
		return E_FAIL;
	}

	//�e�̃e�N�X�`���擾
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, BULLETEFFECTPOLYGON01_TEXTURENAME, &g_pTextureBulletEffectPolygon[0])))
	{
		MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂����s���܂���", "�G���[", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	//�v���[���[�̃|���S��
	g_pVtxBufferBulletEffectPolygon->Lock(0, 0, (void**)&pVtx, 0);                    //���z�A�h���X���擾���o�b�t�@�����b�N����
	MakeVertexPolygonPlayer(pD3DDevice, pVtx);

	for (int nCnt = 0; nCnt < MAX_BULLETEFFECT; nCnt++)
	{
		//�v���[���[�̏��f�[�^�̏����l
		g_BulletEffect[nCnt].pos.x = pPlayer -> fPosX;
		g_BulletEffect[nCnt].pos.y = pPlayer -> fPosY;
		g_BulletEffect[nCnt].color = D3DCOLOR_RGBA(255,255,255,255);
		g_BulletEffect[nCnt].fAngle = 0;
		g_BulletEffect[nCnt].nAlpha = 255;
		g_BulletEffect[nCnt].nLife = BULLETLIFE;
		g_BulletEffect[nCnt].bUse = false;
		g_BulletEffect[nCnt].fRadius = RADIUS;
		g_BulletEffect[nCnt].fRaidusValue = g_BulletEffect[nCnt].fRadius / g_BulletEffect[nCnt].nLife;
		g_BulletEffect[nCnt].fColorAlphaValue = 255 / g_BulletEffect[nCnt].nLife;

		pVtx[0].pos = D3DXVECTOR3(g_BulletEffect[nCnt].pos.x, g_BulletEffect[nCnt].pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_BulletEffect[nCnt].pos.x + BULLETEFFECT_WIDTH, g_BulletEffect[nCnt].pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_BulletEffect[nCnt].pos.x, g_BulletEffect[nCnt].pos.y + BULLETEFFECT_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_BulletEffect[nCnt].pos.x + BULLETEFFECT_WIDTH, g_BulletEffect[nCnt].pos.y + BULLETEFFECT_HEIGHT, 0.0f);

		pVtx += 4;
	}

	g_pVtxBufferBulletEffectPolygon->Unlock();                     //�o�b�t�@�̃A�����b�N
	return S_OK;
}
//�I������
void UninitBulletEffect(void)
{
	// �e�N�X�`���C���^�[�t�F�C�X�̕Еt��
	for (int nCnt = 0; nCnt < NUM_BULLETEFFECTTEXTURE; nCnt++)
	{
		SAFE_RELEASE(g_pTextureBulletEffectPolygon[nCnt]);
	}

	//���_�o�b�t�@�C���^�[�t�F�C�X�̕Еt��
	SAFE_RELEASE(g_pVtxBufferBulletEffectPolygon);
}

//�v���[���[�f�[�^�̍X�V����
void UpdateBulletEffect(void)
{
	for (int nCnt = 0; nCnt < MAX_BULLETEFFECT; nCnt++)
	{
		//�����G�͎g�p�̏�ԂȂ�
		if (g_BulletEffect[nCnt].bUse == true)
		{
			g_BulletEffect[nCnt].nLife--;
			g_BulletEffect[nCnt].fRadius -= g_BulletEffect[nCnt].fRaidusValue;
			g_BulletEffect[nCnt].nAlpha -= g_BulletEffect[nCnt].fColorAlphaValue;

			//���a��0�ȉ��ɂȂ�Ȃ��悤��
			if(g_BulletEffect[nCnt].fRadius < 0)
			{
				g_BulletEffect[nCnt].fRadius = 0.0f;
			}

			//���l��0�ȉ��ɂȂ�Ȃ��悤��
			if(g_BulletEffect[nCnt].nAlpha < 0)
			{
				g_BulletEffect[nCnt].nAlpha = 0;
			}

			g_BulletEffect[nCnt].color = D3DCOLOR_RGBA(255,0,0,255);   //�J���[�l���㏑������

			//life��0�ȉ��ɂȂ����疢�g�p�ɂ���
			if(g_BulletEffect[nCnt].nLife <= 0)
			{
				g_BulletEffect[nCnt].nLife = 0;
				g_BulletEffect[nCnt].bUse = false;
			}
		}
	}

	UpdatePolygonBulletEffect();
}

//�v���[���[�|���S���̃f�[�^�X�V
void UpdatePolygonBulletEffect(void)
{
	static int nRunCnt = 0;                                       //�����j���O�}���e�N�X�`���̐؂�ւ��J�E���^�[
	static int nActionFlame = 0;                                  //�����j���O�}���̓�����x�����邽�߂̃J�E���^�[
	VERTEX_2D *pVtx = NULL;
	LPDIRECT3DDEVICE9 pD3DDevice;
	PLAYER *pPlayer = GetPlayer();
	pD3DDevice = Get_Device();

	g_pVtxBufferBulletEffectPolygon -> Lock(0, 0, (void**)&pVtx, 0);          //���z�A�h���X���擾���o�b�t�@�����b�N����

	MakeVertexPolygonBulletEffect(pD3DDevice, pVtx);

	for (int nCnt = 0; nCnt < MAX_BULLETEFFECT; nCnt++)
	{
		if(g_BulletEffect[nCnt].bUse == true)
		{
			
			SpriteRorateBulletEffect(g_BulletEffect[nCnt],pVtx+nCnt*4);

			pVtx[0+nCnt*4].rhw = 1.0f;
			pVtx[1+nCnt*4].rhw = 1.0f;
			pVtx[2+nCnt*4].rhw = 1.0f;
			pVtx[3+nCnt*4].rhw = 1.0f;

			pVtx[0+nCnt*4].color = g_BulletEffect[nCnt].color;
			pVtx[1+nCnt*4].color = g_BulletEffect[nCnt].color;
			pVtx[2+nCnt*4].color = g_BulletEffect[nCnt].color;
			pVtx[3+nCnt*4].color = g_BulletEffect[nCnt].color;

			pVtx[0+nCnt*4].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1+nCnt*4].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2+nCnt*4].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3+nCnt*4].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
	}

	g_pVtxBufferBulletEffectPolygon -> Unlock();                                     //�o�b�t�@�̃A�����b�N
}

void DrawBulletEffect(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	//�G�t�F�N�g�F���_�t�H�[�}�b�g�ݒ�
	pD3DDevice->SetStreamSource(0, g_pVtxBufferBulletEffectPolygon, 0, sizeof(VERTEX_2D));              //VRAM����GPU�Ƀf�[�^���X�g���[��
	pD3DDevice->SetFVF(FVF_VERTEX2D);
	pD3DDevice->SetTexture(0, g_pTextureBulletEffectPolygon[0]);				 //�e�N�X�`���̐ݒ�

	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);                            //���Z�������s��!

	//�G�t�F�N�g�̕`��
	for (int nCnt = 0; nCnt < MAX_BULLETEFFECT; nCnt++)
	{
		//�����e�͎g�p�̏�ԂȂ�
		if (g_BulletEffect[nCnt].bUse == true)
		{
			pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt*4, NUM_POLYGON);                                            //�e�N�X�`���������̊m��
		}
	}

	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);                   //�u�����h���[�h���A���t�@�u�����h�ɖ߂�
}

//�v���[���[�|���S���̕\���ʒuX,�|���S���̕\���ʒuY,�|���S���̕�,�|���S���̍���
void MakeVertexPolygonBulletEffect(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx)
{
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);   //����1:�ݒ薼�J�����O ����2:�ݒ�l(D3DCULL_CCW:�t���v���𗠖ʂ�����)
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//�|���S���`�悷��O�Ƀe�N�X�`�����Z�b�g
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//�G���擾
BULLETEFFECT *GetBulletEffect(void)
{

	return &g_BulletEffect[0];
}

//�e��ݒu
void SetBulletEffect(float fPosX,float fPosY,int nLife,float fAngle)
{
	for(int nCnt = 0; nCnt < MAX_BULLETEFFECT; nCnt++)
	{
		if(g_BulletEffect[nCnt].bUse == false)
		{
			g_BulletEffect[nCnt].pos.x = fPosX;
			g_BulletEffect[nCnt].pos.y = fPosY;
			g_BulletEffect[nCnt].color = D3DCOLOR_RGBA(255,255,255,255);
			g_BulletEffect[nCnt].nLife = nLife;
			g_BulletEffect[nCnt].nAlpha = 255;
			g_BulletEffect[nCnt].fAngle = fAngle;
			g_BulletEffect[nCnt].bUse = true;
			g_BulletEffect[nCnt].fRadius = RADIUS;
			g_BulletEffect[nCnt].fRaidusValue = g_BulletEffect[nCnt].fRadius / nLife;
			g_BulletEffect[nCnt].fColorAlphaValue = 255 / nLife;
			
			break;
		}
	}
}

void SpriteRorateBulletEffect (BULLETEFFECT BBulletEffect,VERTEX_2D *pVtx)
{
	double X1, Y1, X2, Y2, X3, Y3, X4, Y4;             //����,�E�����W
	double Xc1, Yc1, Xc2, Yc2, Xc3, Yc3, Xc4, Yc4;     //��]������̍��W
	double XC, YC;                                     //���S���W

	X1 = BBulletEffect.pos.x - BBulletEffect.fRadius;
	Y1 = BBulletEffect.pos.y - BBulletEffect.fRadius;
	X2 = BBulletEffect.pos.x + BBulletEffect.fRadius;
	Y2 = BBulletEffect.pos.y - BBulletEffect.fRadius;
	X3 = BBulletEffect.pos.x - BBulletEffect.fRadius;
	Y3 = BBulletEffect.pos.y + BBulletEffect.fRadius;
	X4 = BBulletEffect.pos.x + BBulletEffect.fRadius;
	Y4 = BBulletEffect.pos.y + BBulletEffect.fRadius;

	XC = BBulletEffect.pos.x;
	YC = BBulletEffect.pos.y;

	Xc1 = (X1 - XC)*cos(BBulletEffect.fAngle) - (Y1 - YC)*sin(BBulletEffect.fAngle) + XC;
	Yc1 = (X1 - XC)*sin(BBulletEffect.fAngle) + (Y1 - YC)*cos(BBulletEffect.fAngle) + YC;

	Xc2 = (X2 - XC)*cos(BBulletEffect.fAngle) - (Y2 - YC)*sin(BBulletEffect.fAngle) + XC;
	Yc2 = (X2 - XC)*sin(BBulletEffect.fAngle) + (Y2 - YC)*cos(BBulletEffect.fAngle) + YC;

	Xc3 = (X3 - XC)*cos(BBulletEffect.fAngle) - (Y3 - YC)*sin(BBulletEffect.fAngle) + XC;
	Yc3 = (X3 - XC)*sin(BBulletEffect.fAngle) + (Y3 - YC)*cos(BBulletEffect.fAngle) + YC;

	Xc4 = (X4 - XC)*cos(BBulletEffect.fAngle) - (Y4 - YC)*sin(BBulletEffect.fAngle) + XC;
	Yc4 = (X4 - XC)*sin(BBulletEffect.fAngle) + (Y4 - YC)*cos(BBulletEffect.fAngle) + YC;

	pVtx[0].pos = D3DXVECTOR3(Xc1,Yc1,0.0f);
	pVtx[1].pos = D3DXVECTOR3(Xc2,Yc2,0.0f);
	pVtx[2].pos = D3DXVECTOR3(Xc3,Yc3,0.0f);
	pVtx[3].pos = D3DXVECTOR3(Xc4,Yc4,0.0f);
}