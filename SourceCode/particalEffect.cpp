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
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureParticalEffectPolygon[NUM_PARTICALEFFECTTEXTURE] = {};           //�e�N�X�`���𑀍삷�邽�߂̃|�C���^(�e�N�X�`���o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferParticalEffectPolygon = NULL;    //���_�o�b�t�@�𑀍삷�邽�߂̃|�C���^(���_�o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)

PARTICALEFFECT g_ParticalEffect[MAX_PARTICALEFFECT];

//-----------------------------------------------------------------------------------------
//�d�v��Ԋ֐��B

//�e�ϐ��̏�����
HRESULT InitParticalEffect(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_2D *pVtx = NULL;                           //���_�\���̂̃|�C���^
	pD3DDevice = Get_Device();
	 
	//���_�o�b�t�@�̐���
	if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_PARTICALEFFECTVERTEX*MAX_PARTICALEFFECT, D3DUSAGE_WRITEONLY, FVF_VERTEX2D, D3DPOOL_MANAGED, &g_pVtxBufferParticalEffectPolygon, NULL)))
	{
		return E_FAIL;
	}

	//�p�[�e�B�N���̃e�N�X�`���擾
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, ParticalEffectPOLYGON01_TEXTURENAME, &g_pTextureParticalEffectPolygon[0])))
	{
		MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂����s���܂���", "�G���[", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	//�v���[���[�̃|���S��
	g_pVtxBufferParticalEffectPolygon->Lock(0, 0, (void**)&pVtx, 0);                    //���z�A�h���X���擾���o�b�t�@�����b�N����
	MakeVertexPolygonParticalEffect(pD3DDevice, pVtx);

	for (int nCnt = 0; nCnt < MAX_PARTICALEFFECT; nCnt++)
	{
		//�p�[�e�B�N���̃G�t�F�N�g�̏��f�[�^�̏����l
		g_ParticalEffect[nCnt].pos.x = 0;
		g_ParticalEffect[nCnt].pos.y = 0;
		g_ParticalEffect[nCnt].speed.x = 0;
		g_ParticalEffect[nCnt].speed.y = 0;
		g_ParticalEffect[nCnt].color = D3DCOLOR_RGBA(255,255,255,255);
		g_ParticalEffect[nCnt].nAlpha = 255;
		g_ParticalEffect[nCnt].nLife = 1;
		g_ParticalEffect[nCnt].bUse = false;
		g_ParticalEffect[nCnt].fRadius = 0;
		g_ParticalEffect[nCnt].fColorAlphaValue = 255 / g_ParticalEffect[nCnt].nLife;

		pVtx[0].pos = D3DXVECTOR3(g_ParticalEffect[nCnt].pos.x , g_ParticalEffect[nCnt].pos.y , 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_ParticalEffect[nCnt].pos.x , g_ParticalEffect[nCnt].pos.y , 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_ParticalEffect[nCnt].pos.x , g_ParticalEffect[nCnt].pos.y , 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_ParticalEffect[nCnt].pos.x , g_ParticalEffect[nCnt].pos.y , 0.0f);

		pVtx += 4;
	}

	g_pVtxBufferParticalEffectPolygon->Unlock();                     //�o�b�t�@�̃A�����b�N
	return S_OK;
}
//�I������
void UninitParticalEffect(void)
{
	// �e�N�X�`���C���^�[�t�F�C�X�̕Еt��
	for (int nCnt = 0; nCnt < NUM_PARTICALEFFECTTEXTURE; nCnt++)
	{
		SAFE_RELEASE(g_pTextureParticalEffectPolygon[nCnt]);
	}

	//���_�o�b�t�@�C���^�[�t�F�C�X�̕Еt��
	SAFE_RELEASE(g_pVtxBufferParticalEffectPolygon);
}

//�v���[���[�f�[�^�̍X�V����
void UpdateParticalEffect(void)
{
	//�p�[�e�B�N�����˒��S�f�[�^�擾
	PARTICALCENTER *pPartical;
	pPartical = GetParticalCenter();

	//���݃��[�h�擾
	EFFECTMODE *EffectMode;
	EffectMode = GetEffectMode();

	for (int nCnt = 0; nCnt < MAX_PARTICALEFFECT; nCnt++)
	{
		//�����p�[�e�B�N���͎g�p�̏�ԂȂ�
		if (g_ParticalEffect[nCnt].bUse == true)
		{
			g_ParticalEffect[nCnt].posOld.x = g_ParticalEffect[nCnt].pos.x;
			g_ParticalEffect[nCnt].posOld.y = g_ParticalEffect[nCnt].pos.y;

			//�G�t�F�N�g���X�g
			switch(*EffectMode)
			{
			case EFFECTMODE_BASE:
				{
					//���͌v�Z
					g_ParticalEffect[nCnt].speed.x += pPartical -> fWindPower * cos(pPartical -> fWindAngel);
					g_ParticalEffect[nCnt].speed.y += pPartical -> fWindPower * sin(pPartical -> fWindAngel);

					//�d�͌v�Z
					g_ParticalEffect[nCnt].speed.y += pPartical -> fGravity;
					break;
				}
			}

			//
			g_ParticalEffect[nCnt].pos.x += g_ParticalEffect[nCnt].speed.x;
			g_ParticalEffect[nCnt].pos.y += g_ParticalEffect[nCnt].speed.y;

			g_ParticalEffect[nCnt].nLife--;                                         //Life������
			g_ParticalEffect[nCnt].nAlpha -= g_ParticalEffect[nCnt].fColorAlphaValue; //���l������

			//life��0�ȉ��ɂȂ����疢�g�p�ɂ���
			if(g_ParticalEffect[nCnt].nLife <= 0)
			{
				g_ParticalEffect[nCnt].bUse = false;
			}
		}
	}

	UpdatePolygonParticalEffect();
}

//�v���[���[�|���S���̃f�[�^�X�V
void UpdatePolygonParticalEffect(void)
{
	static int nRunCnt = 0;                                       //�����j���O�}���e�N�X�`���̐؂�ւ��J�E���^�[
	static int nActionFlame = 0;                                  //�����j���O�}���̓�����x�����邽�߂̃J�E���^�[
	VERTEX_2D *pVtx = NULL;
	PLAYER *pPlayer = GetPlayer();
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	g_pVtxBufferParticalEffectPolygon -> Lock(0, 0, (void**)&pVtx, 0);          //���z�A�h���X���擾���o�b�t�@�����b�N����

	MakeVertexPolygonParticalEffect(pD3DDevice, pVtx);

	for (int nCnt = 0; nCnt < MAX_PARTICALEFFECT; nCnt++)
	{
		if(g_ParticalEffect[nCnt].bUse == true)
		{
			if(g_ParticalEffect[nCnt].bCamera == false)
			{
				pVtx[0+nCnt*4].pos = D3DXVECTOR3(g_ParticalEffect[nCnt].pos.x - g_ParticalEffect[nCnt].fRadius, g_ParticalEffect[nCnt].pos.y - g_ParticalEffect[nCnt].fRadius, 0.0f);
				pVtx[1+nCnt*4].pos = D3DXVECTOR3(g_ParticalEffect[nCnt].pos.x + g_ParticalEffect[nCnt].fRadius, g_ParticalEffect[nCnt].pos.y - g_ParticalEffect[nCnt].fRadius, 0.0f);
				pVtx[2+nCnt*4].pos = D3DXVECTOR3(g_ParticalEffect[nCnt].pos.x - g_ParticalEffect[nCnt].fRadius, g_ParticalEffect[nCnt].pos.y + g_ParticalEffect[nCnt].fRadius, 0.0f);
				pVtx[3+nCnt*4].pos = D3DXVECTOR3(g_ParticalEffect[nCnt].pos.x + g_ParticalEffect[nCnt].fRadius, g_ParticalEffect[nCnt].pos.y + g_ParticalEffect[nCnt].fRadius, 0.0f);
			}

			else
			{
				pVtx[0+nCnt*4].pos = D3DXVECTOR3(g_ParticalEffect[nCnt].pos.x - g_ParticalEffect[nCnt].fRadius - pPlayer->fPosX_SUM, g_ParticalEffect[nCnt].pos.y - g_ParticalEffect[nCnt].fRadius, 0.0f);
				pVtx[1+nCnt*4].pos = D3DXVECTOR3(g_ParticalEffect[nCnt].pos.x + g_ParticalEffect[nCnt].fRadius - pPlayer->fPosX_SUM, g_ParticalEffect[nCnt].pos.y - g_ParticalEffect[nCnt].fRadius, 0.0f);
				pVtx[2+nCnt*4].pos = D3DXVECTOR3(g_ParticalEffect[nCnt].pos.x - g_ParticalEffect[nCnt].fRadius - pPlayer->fPosX_SUM, g_ParticalEffect[nCnt].pos.y + g_ParticalEffect[nCnt].fRadius, 0.0f);
				pVtx[3+nCnt*4].pos = D3DXVECTOR3(g_ParticalEffect[nCnt].pos.x + g_ParticalEffect[nCnt].fRadius - pPlayer->fPosX_SUM, g_ParticalEffect[nCnt].pos.y + g_ParticalEffect[nCnt].fRadius, 0.0f);

			}

			pVtx[0+nCnt*4].rhw = 1.0f;
			pVtx[1+nCnt*4].rhw = 1.0f;
			pVtx[2+nCnt*4].rhw = 1.0f;
			pVtx[3+nCnt*4].rhw = 1.0f;

			g_ParticalEffect[nCnt].color = D3DCOLOR_RGBA(70,70,0,g_ParticalEffect[nCnt].nAlpha);

			pVtx[0+nCnt*4].color = g_ParticalEffect[nCnt].color;
			pVtx[1+nCnt*4].color = g_ParticalEffect[nCnt].color;
			pVtx[2+nCnt*4].color = g_ParticalEffect[nCnt].color;
			pVtx[3+nCnt*4].color = g_ParticalEffect[nCnt].color;

			pVtx[0+nCnt*4].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1+nCnt*4].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2+nCnt*4].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3+nCnt*4].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
	}

	g_pVtxBufferParticalEffectPolygon -> Unlock();                                     //�o�b�t�@�̃A�����b�N
}

void DrawParticalEffect(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	//�G�t�F�N�g�F���_�t�H�[�}�b�g�ݒ�
	pD3DDevice->SetStreamSource(0, g_pVtxBufferParticalEffectPolygon, 0, sizeof(VERTEX_2D));              //VRAM����GPU�Ƀf�[�^���X�g���[��
	pD3DDevice->SetFVF(FVF_VERTEX2D);
	pD3DDevice->SetTexture(0, g_pTextureParticalEffectPolygon[0]);				                        //�e�N�X�`���̐ݒ�

	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);                                             //���Z�������s��!

	for (int nCnt = 0; nCnt < MAX_PARTICALEFFECT; nCnt++)
	{
		//�����p�[�e�B�N���͎g�p�̏�ԂȂ�
		if (g_ParticalEffect[nCnt].bUse == true)
		{
			pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt*4, NUM_POLYGON);                        //�e�N�X�`���������̊m��
		}
	}

	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);                   //�u�����h���[�h���A���t�@�u�����h�ɖ߂�
}


//�v���[���[�|���S���̕\���ʒuX,�|���S���̕\���ʒuY,�|���S���̕�,�|���S���̍���
void MakeVertexPolygonParticalEffect(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx)
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
PARTICALEFFECT *GetParticalEffect(void)
{
	return &g_ParticalEffect[0];
}

//�p�[�e�B�N����ݒu
void SetParticalEffect(float fPosX,float fPosY,float fSpeedX,float fSpeedY,float fRadius,int nLife,bool bCamera)
{
	for(int nCnt = 0; nCnt < MAX_PARTICALEFFECT; nCnt++)
	{
		if(g_ParticalEffect[nCnt].bUse == false)
		{
			g_ParticalEffect[nCnt].pos.x = fPosX;
			g_ParticalEffect[nCnt].pos.y = fPosY;
			g_ParticalEffect[nCnt].posOld.x = g_ParticalEffect[nCnt].pos.x;
			g_ParticalEffect[nCnt].posOld.y = g_ParticalEffect[nCnt].pos.y;
			g_ParticalEffect[nCnt].speed.x = fSpeedX;
			g_ParticalEffect[nCnt].speed.y = fSpeedY;
			g_ParticalEffect[nCnt].color = D3DCOLOR_RGBA(255,255,255,255);
			g_ParticalEffect[nCnt].fRadius = fRadius;
			g_ParticalEffect[nCnt].nLife = nLife;
			g_ParticalEffect[nCnt].nAlpha = 255;
			g_ParticalEffect[nCnt].bCamera = bCamera;
			g_ParticalEffect[nCnt].fColorAlphaValue = g_ParticalEffect[nCnt].nAlpha / g_ParticalEffect[nCnt].nLife;
			g_ParticalEffect[nCnt].bUse = true;
			
			break;
		}
	}
}