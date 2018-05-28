//-----------------------------------------------------------------------------------------
//�v���[���[,����Ȃǂ̏���[running02.cpp]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------------------
#include <time.h>
#include "main.h"
#include "particalCenter.h"
#include "partical.h"
#include "particalEffect.h"
#include "player.h"

//-----------------------------------------------------------------------------------------
//�}�N��
//-----------------------------------------------------------------------------------------
#define PARTICALRADIUS (10)

//-----------------------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureParticalPolygon[NUM_PARTICALTEXTURE] = {};           //�e�N�X�`���𑀍삷�邽�߂̃|�C���^(�e�N�X�`���o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferParticalPolygon = NULL;    //���_�o�b�t�@�𑀍삷�邽�߂̃|�C���^(���_�o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)

PARTICAL g_Partical[MAX_PARTICAL];

//-----------------------------------------------------------------------------------------
//�d�v��Ԋ֐��B

//�e�ϐ��̏�����
HRESULT InitPartical(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_2D *pVtx = NULL;                           //���_�\���̂̃|�C���^
	pD3DDevice = Get_Device();
	 
	//���_�o�b�t�@�̐���
	if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_PARTICALVERTEX*MAX_PARTICAL, D3DUSAGE_WRITEONLY, FVF_VERTEX2D, D3DPOOL_MANAGED, &g_pVtxBufferParticalPolygon, NULL)))
	{
		return E_FAIL;
	}

	//�p�[�e�B�N���̃e�N�X�`���擾
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, PARTICALPOLYGON01_TEXTURENAME, &g_pTextureParticalPolygon[0])))
	{
		MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂����s���܂���", "�G���[", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	//�v���[���[�̃|���S��
	g_pVtxBufferParticalPolygon->Lock(0, 0, (void**)&pVtx, 0);                    //���z�A�h���X���擾���o�b�t�@�����b�N����
	MakeVertexPolygonPartical(pD3DDevice, pVtx);

	for (int nCnt = 0; nCnt < MAX_PARTICAL; nCnt++)
	{
		//�p�[�e�B�N���̏��f�[�^�̏����l
		g_Partical[nCnt].pos.x = 0;
		g_Partical[nCnt].pos.y = 0;
		g_Partical[nCnt].speed.x = 0;
		g_Partical[nCnt].speed.y = 0;
		g_Partical[nCnt].color = D3DCOLOR_RGBA(255,255,255,255);
		g_Partical[nCnt].nAlpha = 255;
		g_Partical[nCnt].nLife = 20;
		g_Partical[nCnt].bUse = false;
		g_Partical[nCnt].fRadius = 0;
		g_Partical[nCnt].bCamera = false;
		g_Partical[nCnt].fColorAlphaValue = 255 / g_Partical[nCnt].nLife;

		pVtx[0].pos = D3DXVECTOR3(g_Partical[nCnt].pos.x - PARTICALRADIUS, g_Partical[nCnt].pos.y - PARTICALRADIUS, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Partical[nCnt].pos.x + PARTICALRADIUS, g_Partical[nCnt].pos.y - PARTICALRADIUS, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Partical[nCnt].pos.x - PARTICALRADIUS, g_Partical[nCnt].pos.y + PARTICALRADIUS, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Partical[nCnt].pos.x + PARTICALRADIUS, g_Partical[nCnt].pos.y + PARTICALRADIUS, 0.0f);

		pVtx += 4;
	}

	g_pVtxBufferParticalPolygon->Unlock();                     //�o�b�t�@�̃A�����b�N
	return S_OK;
}
//�I������
void UninitPartical(void)
{
	// �e�N�X�`���C���^�[�t�F�C�X�̕Еt��
	for (int nCnt = 0; nCnt < NUM_PARTICALTEXTURE; nCnt++)
	{
		SAFE_RELEASE(g_pTextureParticalPolygon[nCnt]);
	}

	//���_�o�b�t�@�C���^�[�t�F�C�X�̕Еt��
	SAFE_RELEASE(g_pVtxBufferParticalPolygon);
}

//�v���[���[�f�[�^�̍X�V����
void UpdatePartical(void)
{
	//�p�[�e�B�N�����˒��S�f�[�^�擾
	PARTICALCENTER *pParticalCenter;
	pParticalCenter = GetParticalCenter();

	//���݃��[�h�擾
	EFFECTMODE *EffectMode;
	EffectMode = GetEffectMode();

	for (int nCnt = 0; nCnt < MAX_PARTICAL; nCnt++)
	{
		//�����p�[�e�B�N���͎g�p�̏�ԂȂ�
		if (g_Partical[nCnt].bUse == true)
		{
			g_Partical[nCnt].posOld.x = g_Partical[nCnt].pos.x;
			g_Partical[nCnt].posOld.y = g_Partical[nCnt].pos.y;

			//�G�t�F�N�g���X�g
			switch(*EffectMode)
			{
			case EFFECTMODE_BASE:
				{
					//���͌v�Z
					g_Partical[nCnt].speed.x += pParticalCenter -> fWindPower * cos(pParticalCenter -> fWindAngel);
					g_Partical[nCnt].speed.y += pParticalCenter -> fWindPower * sin(pParticalCenter -> fWindAngel);

					//�d�͌v�Z
					g_Partical[nCnt].speed.y += pParticalCenter -> fGravity;
					break;
				}
			}

			//�p�[�e�B�N���ړ�
			g_Partical[nCnt].pos.x += g_Partical[nCnt].speed.x;
			g_Partical[nCnt].pos.y += g_Partical[nCnt].speed.y;

			//�p�[�e�B�N���I������
			g_Partical[nCnt].nLife--;                                   //Life������
			g_Partical[nCnt].nAlpha -= g_Partical[nCnt].fColorAlphaValue; //���l������

			//life��0�ȉ��ɂȂ����疢�g�p�ɂ���
			if(g_Partical[nCnt].nLife <= 0)
			{
				g_Partical[nCnt].bUse = false;
			}
		}
	}

	UpdatePolygonPartical();
}

//�v���[���[�|���S���̃f�[�^�X�V
void UpdatePolygonPartical(void)
{
	VERTEX_2D *pVtx = NULL;
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();
	PLAYER *pPlayer = GetPlayer();

	g_pVtxBufferParticalPolygon -> Lock(0, 0, (void**)&pVtx, 0);          //���z�A�h���X���擾���o�b�t�@�����b�N����
	MakeVertexPolygonPartical(pD3DDevice, pVtx);

	for (int nCnt = 0; nCnt < MAX_PARTICAL; nCnt++)
	{
		if(g_Partical[nCnt].bUse == true)
		{
			//�\���ʒu�̓J�����Ɗ֌W�Ȃ�
			if(g_Partical[nCnt].bCamera == false)
			{
				pVtx[0+nCnt*4].pos = D3DXVECTOR3(g_Partical[nCnt].pos.x - g_Partical[nCnt].fRadius, g_Partical[nCnt].pos.y - g_Partical[nCnt].fRadius, 0.0f);
				pVtx[1+nCnt*4].pos = D3DXVECTOR3(g_Partical[nCnt].pos.x + g_Partical[nCnt].fRadius, g_Partical[nCnt].pos.y - g_Partical[nCnt].fRadius, 0.0f);
				pVtx[2+nCnt*4].pos = D3DXVECTOR3(g_Partical[nCnt].pos.x - g_Partical[nCnt].fRadius, g_Partical[nCnt].pos.y + g_Partical[nCnt].fRadius, 0.0f);
				pVtx[3+nCnt*4].pos = D3DXVECTOR3(g_Partical[nCnt].pos.x + g_Partical[nCnt].fRadius, g_Partical[nCnt].pos.y + g_Partical[nCnt].fRadius, 0.0f);
			}
			
			else
			{
				pVtx[0+nCnt*4].pos = D3DXVECTOR3(g_Partical[nCnt].pos.x - g_Partical[nCnt].fRadius - pPlayer->fPosX_SUM, g_Partical[nCnt].pos.y - g_Partical[nCnt].fRadius, 0.0f);
				pVtx[1+nCnt*4].pos = D3DXVECTOR3(g_Partical[nCnt].pos.x + g_Partical[nCnt].fRadius - pPlayer->fPosX_SUM, g_Partical[nCnt].pos.y - g_Partical[nCnt].fRadius, 0.0f);
				pVtx[2+nCnt*4].pos = D3DXVECTOR3(g_Partical[nCnt].pos.x - g_Partical[nCnt].fRadius - pPlayer->fPosX_SUM, g_Partical[nCnt].pos.y + g_Partical[nCnt].fRadius, 0.0f);
				pVtx[3+nCnt*4].pos = D3DXVECTOR3(g_Partical[nCnt].pos.x + g_Partical[nCnt].fRadius - pPlayer->fPosX_SUM, g_Partical[nCnt].pos.y + g_Partical[nCnt].fRadius, 0.0f);
			}

			pVtx[0+nCnt*4].rhw = 1.0f;
			pVtx[1+nCnt*4].rhw = 1.0f;
			pVtx[2+nCnt*4].rhw = 1.0f;
			pVtx[3+nCnt*4].rhw = 1.0f;

			g_Partical[nCnt].color = D3DCOLOR_RGBA(70,70,0,g_Partical[nCnt].nAlpha);

			pVtx[0+nCnt*4].color = g_Partical[nCnt].color;
			pVtx[1+nCnt*4].color = g_Partical[nCnt].color;
			pVtx[2+nCnt*4].color = g_Partical[nCnt].color;
			pVtx[3+nCnt*4].color = g_Partical[nCnt].color;

			pVtx[0+nCnt*4].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1+nCnt*4].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2+nCnt*4].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3+nCnt*4].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
	}

	g_pVtxBufferParticalPolygon -> Unlock();                                     //�o�b�t�@�̃A�����b�N
}

void DrawPartical(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	//�w�i�̓S�����F���_�t�H�[�}�b�g�ݒ�
	pD3DDevice->SetStreamSource(0, g_pVtxBufferParticalPolygon, 0, sizeof(VERTEX_2D));              //VRAM����GPU�Ƀf�[�^���X�g���[��
	pD3DDevice->SetFVF(FVF_VERTEX2D);
	pD3DDevice->SetTexture(0, g_pTextureParticalPolygon[0]);				 //�e�N�X�`���̐ݒ�

	for (int nCnt = 0; nCnt < MAX_PARTICAL; nCnt++)
	{
		//�����p�[�e�B�N���͎g�p�̏�ԂȂ�
		if (g_Partical[nCnt].bUse == true)
		{
			pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt*4, NUM_POLYGON);                                            //�e�N�X�`���������̊m��
		}
	}
}


//�v���[���[�|���S���̕\���ʒuX,�|���S���̕\���ʒuY,�|���S���̕�,�|���S���̍���
void MakeVertexPolygonPartical(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx)
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
PARTICAL *GetPartical(void)
{

	return &g_Partical[0];
}

//�p�[�e�B�N����ݒu
void SetPartical(float fPosX,float fPosY,float fSpeedX,float fSpeedY,float fRadius,float nLife,bool bCamera)
{
	for(int nCnt = 0; nCnt < MAX_PARTICAL; nCnt++)
	{
		if(g_Partical[nCnt].bUse == false)
		{
			g_Partical[nCnt].pos.x = fPosX;
			g_Partical[nCnt].pos.y = fPosY;
			g_Partical[nCnt].posOld.x = g_Partical[nCnt].pos.x;
			g_Partical[nCnt].posOld.y = g_Partical[nCnt].pos.y;
			g_Partical[nCnt].speed.x = fSpeedX;
			g_Partical[nCnt].speed.y = fSpeedY;
			g_Partical[nCnt].color = D3DCOLOR_RGBA(255,255,255,255);
			g_Partical[nCnt].fRadius = fRadius;
			g_Partical[nCnt].nLife = nLife;
			g_Partical[nCnt].nAlpha = 10;
			g_Partical[nCnt].fColorAlphaValue = g_Partical[nCnt].nAlpha / g_Partical[nCnt].nLife;
			g_Partical[nCnt].bCamera = bCamera;
			g_Partical[nCnt].bUse = true;
			break;
		}
	}
}