//-----------------------------------------------------------------------------------------
//�p�[�e�B�N��,����Ȃǂ̏���[ParticalCenter.cpp]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------------------
#include <stdio.h>
#include <time.h>
#include "main.h"
#include "partical.h"
#include "particalEffect.h"
#include "particalCenter.h"
#include "input.h"

//-----------------------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------------------
#define FRAMEPARTICAL (4)                //1�t���[�����˂���p�[�e�B�N���̐�

//-----------------------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureParticalCenterPolygon[NUM_PARTICALCENTERTEXTURE] = {};           //�e�N�X�`���𑀍삷�邽�߂̃|�C���^(�e�N�X�`���o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferParticalCenterPolygon = NULL;                             //���_�o�b�t�@�𑀍삷�邽�߂̃|�C���^(���_�o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)

PARTICALCENTER g_ParticalCenter[NUM_PARTICALCENTERPOLYGON];
CENTER_EFFECT g_ParticalCenterEffect[NUM_PARTICALCENTERPOLYGON];
int g_ParticalFunction = 0;                //�p�[�e�B�N�����j���[�K�w�R���g���[���ϐ�
EFFECTMODE g_EffectMode = EFFECTMODE_EXPLOSION; //�p�[�e�B�N���̃G�t�F�N�g�I��ϐ�

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------------------
//�p�[�e�B�N������
void ParticalBase(PARTICALCENTER *);                  //�p�[�e�B�N����{
void ParticalExplosion(PARTICALCENTER *);             //�p�[�e�B�N�������G�t�F�N�g

//-----------------------------------------------------------------------------------------
//�d�v��Ԋ֐��B
//-----------------------------------------------------------------------------------------

//�e�ϐ��̏�����
HRESULT InitParticalCenter(void)
{
	srand((unsigned)time(NULL));        //�����̎�𐶐�����

	for(int nCnt = 0; nCnt < NUM_PARTICALCENTERPOLYGON; nCnt++)
	{
		//�p�[�e�B�N���̏����ʒu
		g_ParticalCenter[nCnt].pos.x = 400;
		g_ParticalCenter[nCnt].pos.y = 320;
		//�p�[�e�B�N���̏����X�s�[�h
		g_ParticalCenter[nCnt].speed.x = 10;
		g_ParticalCenter[nCnt].speed.y = 9;
		//�p�[�e�B�N���̌Â��ʒu
		g_ParticalCenter[nCnt].posOld.x = g_ParticalCenter[nCnt].pos.x;
		g_ParticalCenter[nCnt].posOld.y = g_ParticalCenter[nCnt].pos.y;
		//�p�[�e�B�N���̏W�����ˊp�x�Ɗg�U�p�x
		g_ParticalCenter[nCnt].fShootAngel = PI *0.5;
		g_ParticalCenter[nCnt].fRangeAngel = PI;
		//�p�[�e�B�N���̔�ъ�����Ɣ��a
		g_ParticalCenter[nCnt].fDistance = 200;
		g_ParticalCenter[nCnt].fRadius = 0;
		//�v���[���[�g�p
		g_ParticalCenter[nCnt].nEffectType = 0;
		g_ParticalCenter[nCnt].bUse = false;

		//�p�[�e�B�N�����͊�
		g_ParticalCenter[nCnt].fGravity = 0;                 //�d��
		g_ParticalCenter[nCnt].fWindPower = 0;               //����
		g_ParticalCenter[nCnt].fWindAngel = 0;               //���������p�x

		//�p�[�e�B�N�����˒��S�G�t�F�N�g
		//�p�[�e�B�N���̏����ʒu
		g_ParticalCenterEffect[nCnt].pos.x = 400;
		g_ParticalCenterEffect[nCnt].pos.y = 320;
		//�p�[�e�B�N���̏����X�s�[�h
		g_ParticalCenterEffect[nCnt].speed.x = 10;
		g_ParticalCenterEffect[nCnt].speed.y = 9;
		//�p�[�e�B�N���̌Â��ʒu
		g_ParticalCenterEffect[nCnt].posOld.x = g_ParticalCenterEffect[nCnt].pos.x;
		g_ParticalCenterEffect[nCnt].posOld.y = g_ParticalCenterEffect[nCnt].pos.y;
		g_ParticalCenterEffect[nCnt].fRadius = 0;
		g_ParticalCenterEffect[nCnt].bUse = false;
	}

	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_2D *pVtx = NULL;                           //���_�\���̂̃|�C���^
	pD3DDevice = Get_Device();

	//���_�o�b�t�@�̐���
	if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_PARTICALCENTERVERTEX*NUM_PARTICALCENTERPOLYGON*2, D3DUSAGE_WRITEONLY, FVF_VERTEX2D, D3DPOOL_MANAGED, &g_pVtxBufferParticalCenterPolygon, NULL)))
	{
		return E_FAIL;
	}

	//�p�[�e�B�N���e�N�X�`���̎擾
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, PARTICALCENTERPOLYGON01_TEXTURENAME, &g_pTextureParticalCenterPolygon[0])))
	{
		MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂����s���܂���", "�G���[", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, PARTICALCENTERPOLYGON02_TEXTURENAME, &g_pTextureParticalCenterPolygon[1])))
	{
		MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂����s���܂���", "�G���[", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	//�p�[�e�B�N���̃|���S��
	g_pVtxBufferParticalCenterPolygon->Lock(0, 0, (void**)&pVtx, 0);                    //���z�A�h���X���擾���o�b�t�@�����b�N����
	
	for(int nCnt = 0; nCnt < NUM_PARTICALCENTERPOLYGON; nCnt++)
	{
		MakeVertexPolygonParticalCenter(pD3DDevice, pVtx);
		pVtx[0].pos = D3DXVECTOR3(g_ParticalCenter[nCnt].pos.x, g_ParticalCenter[nCnt].pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_ParticalCenter[nCnt].pos.x+PARTICALCENTER_WIDTH, g_ParticalCenter[nCnt].pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_ParticalCenter[nCnt].pos.x, g_ParticalCenter[nCnt].pos.y+PARTICALCENTER_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_ParticalCenter[nCnt].pos.x+PARTICALCENTER_WIDTH, g_ParticalCenter[nCnt].pos.y+PARTICALCENTER_HEIGHT, 0.0f);

		pVtx += 4;

		float fXCenter = g_ParticalCenter[nCnt].pos.x + PARTICALCENTER_WIDTH/2;
		float fYCenter = g_ParticalCenter[nCnt].pos.y + PARTICALCENTER_HEIGHT/2;

		pVtx[0].pos = D3DXVECTOR3(fXCenter - MAIN_RADIUS, fYCenter - MAIN_RADIUS, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fXCenter + MAIN_RADIUS, fYCenter - MAIN_RADIUS, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(fXCenter - MAIN_RADIUS, fYCenter + MAIN_RADIUS, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fXCenter + MAIN_RADIUS, fYCenter + MAIN_RADIUS, 0.0f);
	}

	g_pVtxBufferParticalCenterPolygon->Unlock();                                     //�o�b�t�@�̃A�����b�N

	return S_OK;
}
//�I������
void UninitParticalCenter(void)
{
	// �e�N�X�`���C���^�[�t�F�C�X�̕Еt��
	for (int nCnt = 0; nCnt < NUM_PARTICALCENTERTEXTURE; nCnt++)
	{
		SAFE_RELEASE(g_pTextureParticalCenterPolygon[nCnt]);
	}

	//���_�o�b�t�@�C���^�[�t�F�C�X�̕Еt��
	SAFE_RELEASE(g_pVtxBufferParticalCenterPolygon);
}

//�p�[�e�B�N���f�[�^�̍X�V����
void UpdateParticalCenter(void)
{
	for(int nCnt = 0; nCnt < NUM_PARTICALCENTERPOLYGON;nCnt++)
	{
		//�����p�[�e�B�N�����S�͎g�p�̏�ԂȂ�
		if (g_ParticalCenter[nCnt].bUse == true)
		{
			//�p�[�e�B�N���̑O�̍��W���L�^
			g_ParticalCenter[nCnt].posOld.x = g_ParticalCenter[nCnt].pos.x;
			g_ParticalCenter[nCnt].posOld.y = g_ParticalCenter[nCnt].pos.y;

			//-----------------------------------------------------------------------------------------
			//�p�[�e�B�N���̈ړ�����
			//-----------------------------------------------------------------------------------------
			//�v���[���[�̈ʒu���󂯎��

			//-----------------------------------------------------------------------------------------
			//�p�[�e�B�N���̃G�t�F�N�g���X�g
			//-----------------------------------------------------------------------------------------
			switch(g_ParticalCenter[nCnt].nEffectType)
			{
			case 1:
				//�p�[�e�B�N��:��{
				ParticalBase(&g_ParticalCenter[nCnt]);
				break;
			case 2:
				//�p�[�e�B�N��:����
				ParticalExplosion(&g_ParticalCenter[nCnt]);
				break;
			}

			//-----------------------------------------------------------------------------------------
			//�p�[�e�B�N��
			//-----------------------------------------------------------------------------------------
			//�ړ��͈͐���
			if ((g_ParticalCenter[nCnt].pos.x < 0) || ( (g_ParticalCenter[nCnt].pos.x + PARTICALCENTER_WIDTH) > SCREEN_WIDTH))
			{
				g_ParticalCenter[nCnt].pos.x = g_ParticalCenter[nCnt].posOld.x;
			}

			if ((g_ParticalCenter[nCnt].pos.y < 0) || ((g_ParticalCenter[nCnt].pos.y + PARTICALCENTER_HEIGHT) > SCREEN_HEIGHT))
			{
				g_ParticalCenter[nCnt].pos.y = g_ParticalCenter[nCnt].posOld.y;
			}
		}

		UpdatePolygonParticalCenter();
	}
}

//�p�[�e�B�N���|���S���̃f�[�^�X�V
void UpdatePolygonParticalCenter(void) 
{
	VERTEX_2D *pVtx = NULL;
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();
																  
	g_pVtxBufferParticalCenterPolygon->Lock(0, 0, (void**)&pVtx, 0);          //���z�A�h���X���擾���o�b�t�@�����b�N����

	MakeVertexPolygonParticalCenter(pD3DDevice, pVtx);

	for(int nCnt = 0; nCnt < NUM_PARTICALCENTERPOLYGON; nCnt++)
	{
		//�����p�[�e�B�N�����S�͎g�p�̏�ԂȂ�
		if (g_ParticalCenter[nCnt].bUse == true)
		{
			//�p�[�e�B�N���{�̂̃p�����[�^
			pVtx[0].pos = D3DXVECTOR3(g_ParticalCenter[nCnt].pos.x, g_ParticalCenter[nCnt].pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_ParticalCenter[nCnt].pos.x + PARTICALCENTER_WIDTH, g_ParticalCenter[nCnt].pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_ParticalCenter[nCnt].pos.x, g_ParticalCenter[nCnt].pos.y + PARTICALCENTER_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_ParticalCenter[nCnt].pos.x + PARTICALCENTER_WIDTH, g_ParticalCenter[nCnt].pos.y + PARTICALCENTER_HEIGHT, 0.0f);

			pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			pVtx[0].color = D3DCOLOR_RGBA(255, 255, 255, 10);
			pVtx[1].color = D3DCOLOR_RGBA(255, 255, 255, 10);
			pVtx[2].color = D3DCOLOR_RGBA(255, 255, 255, 10);
			pVtx[3].color = D3DCOLOR_RGBA(255, 255, 255, 10);

			pVtx += 4;

			//�p�[�e�B�N���̃}�V���K���̃p�����[�^

			float fXCenter = g_ParticalCenter[nCnt].pos.x + PARTICALCENTER_WIDTH/2;
			float fYCenter = g_ParticalCenter[nCnt].pos.y + PARTICALCENTER_HEIGHT/2;

			pVtx[0].pos = D3DXVECTOR3(fXCenter - MAIN_RADIUS, fYCenter - MAIN_RADIUS, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(fXCenter + MAIN_RADIUS, fYCenter - MAIN_RADIUS, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(fXCenter - MAIN_RADIUS, fYCenter + MAIN_RADIUS, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(fXCenter + MAIN_RADIUS, fYCenter + MAIN_RADIUS, 0.0f);

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

		}
	}

	g_pVtxBufferParticalCenterPolygon->Unlock();                                     //�o�b�t�@�̃A�����b�N
}

void DrawParticalCenter(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	for(int nCnt = 0; nCnt < NUM_PARTICALCENTERPOLYGON; nCnt++)
	{
		if(g_ParticalCenter[nCnt].bUse == true)
		{
			//�w�i�̓S�����F���_�t�H�[�}�b�g�ݒ�
			pD3DDevice->SetStreamSource(0, g_pVtxBufferParticalCenterPolygon, 0, sizeof(VERTEX_2D));              //VRAM����GPU�Ƀf�[�^���X�g���[��
			pD3DDevice->SetFVF(FVF_VERTEX2D);

			//�p�[�e�B�N�����˒��S�`��
			pD3DDevice->SetTexture(0, g_pTextureParticalCenterPolygon[0]);				                             //�e�N�X�`���̐ݒ�
			pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt*4, NUM_POLYGON);                                  //�e�N�X�`���������̊m��

			//�p�[�e�B�N�����˒��S�̃G�t�F�N�g�`��
			pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);                                       //���Z�������s��!

			pD3DDevice->SetTexture(0, g_pTextureParticalCenterPolygon[1]);				                             //�e�N�X�`���̐ݒ�
			pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt*4+4, NUM_POLYGON);                                  //�e�N�X�`���������̊m��

			pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);                               //�u�����h���[�h���A���t�@�u�����h�ɖ߂�
		}
	}
}

//�p�[�e�B�N���|���S���̕\���ʒuX,�|���S���̕\���ʒuY,�|���S���̕�,�|���S���̍���
void MakeVertexPolygonParticalCenter(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx)
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

PARTICALCENTER *GetParticalCenter(void)
{
	return &g_ParticalCenter[0];
}

//�p�[�e�B�N��:��{
void ParticalBase(PARTICALCENTER *pParticalCenter)
{
		//���ˊp�x�v�Z
		float fHantenAngle = 2 * PI * ( (pParticalCenter->fShootAngel / (2*PI) ) - (int)(pParticalCenter->fShootAngel / (2*PI) ) );

		//���ˊp�x���t���v���ɉ����ĉ��
		if(GetKeyboardPress(DIK_LEFT))
		{
			pParticalCenter->fShootAngel -= 0.01*PI;
		}

		//���ˊp�x�����v���ɉ����ĉ��
		if(GetKeyboardPress(DIK_RIGHT))
		{
			pParticalCenter->fShootAngel += 0.01*PI;
		}

		//���ˊp�x��0�ȏ�m��(�ő�l��10*PI)
		if(pParticalCenter->fShootAngel <= -10*PI)
		{
			pParticalCenter->fShootAngel = -10*PI;
		}

		if(pParticalCenter->fShootAngel >= 10*PI)
		{
			pParticalCenter->fShootAngel = 10*PI;
		}

		//�g�U�p�x�g��
		if(GetKeyboardPress(DIK_O))
		{
			pParticalCenter->fRangeAngel += 0.01*PI;
		}

		//�g�U�p�x�k��
		if(GetKeyboardPress(DIK_K))
		{
			pParticalCenter->fRangeAngel -= 0.01*PI;
		}

		//�g�U�p�x�͈̔͂�0~PI�Ɏ��܂�
		if(pParticalCenter->fRangeAngel < 0.01)
		{
			pParticalCenter->fRangeAngel = 0.01;
		}

		if(pParticalCenter->fRangeAngel > PI)
		{
			pParticalCenter->fRangeAngel = PI;
		}

		//�p�[�e�B�N���̔�ъ�������R���g���[��
		if(GetKeyboardPress(DIK_P))  //��ы����L�т�
		{
			pParticalCenter->fDistance += 5;
		}

		if(GetKeyboardPress(DIK_L))  //��ы����k��
		{
			pParticalCenter->fDistance -= 5;
		}

		//��ы����͈͐���
		if(pParticalCenter->fDistance < 10)
		{
			pParticalCenter->fDistance = 10;
		}

		if(pParticalCenter->fDistance > 300)
		{
			pParticalCenter->fDistance = 300;
		}

		//-----------------------------------------------------------------------------------------
		//���͌v�Z
		//-----------------------------------------------------------------------------------------
		//���̊p�x�v�Z
		if(GetKeyboardPress(DIK_I))
		{
			pParticalCenter->fWindAngel += 0.01*PI;
		}

		if(GetKeyboardPress(DIK_J))
		{
			pParticalCenter->fWindAngel -= 0.01*PI;
		}

		//�p�x�͈͐���(-2*PI ~ 2*PI)
		if(pParticalCenter->fWindAngel < -2*PI)
		{
			pParticalCenter->fWindAngel = -2*PI;
		}

		if(pParticalCenter->fWindAngel > 2*PI)
		{
			pParticalCenter->fWindAngel = 2*PI;
		}

		//���͌v�Z
		if(GetKeyboardPress(DIK_U))
		{
			pParticalCenter->fWindPower += 0.001;
		}

		if(GetKeyboardPress(DIK_H))
		{
			pParticalCenter->fWindPower -= 0.001;
		}

		//���͐���(0 ~ 10)
		if(pParticalCenter->fWindPower < 0)
		{
			pParticalCenter->fWindPower = 0;
		}

		if(pParticalCenter->fWindPower > 0.05)
		{
			pParticalCenter->fWindPower = 0.05;
		}

		//-----------------------------------------------------------------------------------------
		//�d�͌v�Z
		//-----------------------------------------------------------------------------------------
		if(GetKeyboardPress(DIK_Y))
		{
			pParticalCenter->fGravity += 0.001;
		}

		if(GetKeyboardPress(DIK_G))
		{
			pParticalCenter->fGravity -= 0.001;
		}

		//�d�͐���(0 ~ 15)
		if(pParticalCenter->fGravity < 0)
		{
			pParticalCenter->fGravity = 0;
		}

		if(pParticalCenter->fGravity > 0.05)
		{
			pParticalCenter->fGravity = 0.05;
		}

		//-----------------------------------------------------------------------------------------
		//�p�[�e�B�N���̔���
		//-----------------------------------------------------------------------------------------
		float fDistance = 0;                //�p�[�e�B�N���̔�ы���
		float fAngel = 0;                   //�p�x
		D3DXVECTOR2 speed;                  //�p�[�e�B�N���̃X�s�[�h

		fDistance = pParticalCenter->fDistance + rand()%100;       //��ы����v�Z
		fAngel = pParticalCenter->fShootAngel + ( rand()%(int)(pParticalCenter->fRangeAngel*2*100) - (int)(pParticalCenter->fRangeAngel*100) ) * 0.01; //��ъp�x�v�Z
		speed.x = fDistance*cos(fAngel)/40;        //x�����̃X�s�[�h
		speed.y = fDistance*sin(fAngel)/40;        //y�����̃X�s�[�h

		//�p�[�e�B�N���𔭎�
		SetPartical(pParticalCenter->pos.x + PARTICALCENTER_WIDTH*0.5,pParticalCenter->pos.y + PARTICALCENTER_HEIGHT*0.5,speed.x,speed.y,pParticalCenter->fRadius,40,pParticalCenter->bCamera);
		SetParticalEffect(pParticalCenter->pos.x + PARTICALCENTER_WIDTH*0.5,pParticalCenter->pos.y + PARTICALCENTER_HEIGHT*0.5,speed.x,speed.y,pParticalCenter->fRadius*2,40,pParticalCenter->bCamera);
}

void ParticalExplosion(PARTICALCENTER *pParticalCenter)
{
	float fDistance = 0;                //�p�[�e�B�N���̔�ы���
	float fAngel = 0;                   //�p�x
	D3DXVECTOR2 speed;                  //�p�[�e�B�N���̃X�s�[�h
	D3DXVECTOR2 test;
	//��ы����͈͐���
	if(pParticalCenter->fDistance < 0)
	{
		pParticalCenter->fDistance = 0;
	}

	if(pParticalCenter->fDistance > 20)
	{
		pParticalCenter->fDistance = 20;
	}

	
	for(int nCnt = 0; nCnt < 120; nCnt++)
	{
		//-----------------------------------------------------------------------------------------
		//�p�[�e�B�N���̔���
		//-----------------------------------------------------------------------------------------
		fDistance = pParticalCenter->fDistance + rand()%20;       //��ы����v�Z
		fAngel = pParticalCenter->fShootAngel + ( rand()%(int)(pParticalCenter->fRangeAngel*2*100) - (int)(pParticalCenter->fRangeAngel*100) ) * 0.01; //��ъp�x�v�Z
		speed.x = fDistance*cos(fAngel)/20;        //x�����̃X�s�[�h
		speed.y = fDistance*sin(fAngel)/20;        //y�����̃X�s�[�h

		test.x = (fDistance+10)*cos(fAngel)/20;        //x�����̃X�s�[�h
		test.y = (fDistance+10)*sin(fAngel)/20;        //y�����̃X�s�[�h

		//�p�[�e�B�N���𔭎�
		SetPartical(pParticalCenter->pos.x + PARTICALCENTER_WIDTH*0.5,pParticalCenter->pos.y + PARTICALCENTER_HEIGHT*0.5,speed.x,speed.y,pParticalCenter->fRadius,20,pParticalCenter->bCamera);
		SetParticalEffect(pParticalCenter->pos.x + PARTICALCENTER_WIDTH*0.5,pParticalCenter->pos.y + PARTICALCENTER_HEIGHT*0.5,speed.x,speed.y,pParticalCenter->fRadius*2,20,pParticalCenter->bCamera);
		SetPartical(pParticalCenter->pos.x + PARTICALCENTER_WIDTH*0.5,pParticalCenter->pos.y + PARTICALCENTER_HEIGHT*0.5,test.x,test.y,pParticalCenter->fRadius*2,20,pParticalCenter->bCamera);
	}

	pParticalCenter -> bUse = false;
}

EFFECTMODE *GetEffectMode(void)
{
	return &g_EffectMode;
}

//�p�[�e�B�N�����S�Z�b�g
void SetParticalCenter(int nEffectType,float fposX,float fposY,float fRadius,float fDistance,bool bCamera)
{
	for(int nCnt = 0; nCnt < NUM_PARTICALCENTERPOLYGON; nCnt++)
	{
		if(g_ParticalCenter[nCnt].bUse == false)
		{
			//�p�[�e�B�N���̏����ʒu
			g_ParticalCenter[nCnt].pos.x = fposX;
			g_ParticalCenter[nCnt].pos.y = fposY;
			//�p�[�e�B�N���̏����X�s�[�h
			g_ParticalCenter[nCnt].speed.x = 1;
			g_ParticalCenter[nCnt].speed.y = 1;
			//�p�[�e�B�N���̌Â��ʒu
			g_ParticalCenter[nCnt].posOld.x = g_ParticalCenter[nCnt].pos.x;
			g_ParticalCenter[nCnt].posOld.y = g_ParticalCenter[nCnt].pos.y;
			//�p�[�e�B�N���̏W�����ˊp�x�Ɗg�U�p�x
			g_ParticalCenter[nCnt].fShootAngel = PI *0.5;
			g_ParticalCenter[nCnt].fRangeAngel = PI;
			//�p�[�e�B�N���̔�ъ�����Ɣ��a
			g_ParticalCenter[nCnt].fDistance = fDistance;
			g_ParticalCenter[nCnt].fRadius = fRadius;
			//�v���[���[�g�p
			g_ParticalCenter[nCnt].nEffectType = nEffectType;
			g_ParticalCenter[nCnt].bCamera = bCamera;
			g_ParticalCenter[nCnt].bUse = true;
			break;
		}
	}
}