//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
//-----------------------------------------------------------------------------------------
#include <Windows.h>
#include "main.h"
#include "fade.h"
#include "player.h"
#include "input.h"

//-----------------------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureFadePolygon[NUM_FADETEXTURE] = {};           //�e�N�X�`���𑀍삷�邽�߂̃|�C���^(�e�N�X�`���o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferFadePolygon[NUM_FADEPOLYGON] = {};    //���_�o�b�t�@�𑀍삷�邽�߂̃|�C���^(���_�o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^) 
D3DCOLOR g_ColorFade;                                                    //�t�F�[�h�F
FADE g_fade;                                                              //�t�F�[�h���
MODE g_modeNext = MODE_TITLE;                                             //���̃��[�h

////////////////////////////////////////////////
//�|���S��(���_)�̏�����
////////////////////////////////////////////////
HRESULT InitFade(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_2DF *pVtx = NULL;                           //���_�\���̂̃|�C���^
	pD3DDevice = Get_Device();

	//���_�o�b�t�@�̐���
	if(FAILED(pD3DDevice -> CreateVertexBuffer(sizeof(VERTEX_2DF)*NUM_FADEVERTEX,D3DUSAGE_WRITEONLY,FVF_VERTEX2D,D3DPOOL_MANAGED,&g_pVtxBufferFadePolygon[0],NULL)))
	{
		return E_FAIL;
	}

	//�w�i�e�N�X�`���̎擾(1��)
	if ( FAILED( D3DXCreateTextureFromFile(pD3DDevice, FADEPOLYGON01_TEXTURENAME,&g_pTextureFadePolygon[0]) ) )
	{
		MessageBox(NULL,"�e�N�X�`���̓ǂݍ��݂����s���܂���","�G���[",MB_OK|MB_ICONHAND);
		return E_FAIL;
	}

	//�w�i�̋�|���S��
	g_pVtxBufferFadePolygon[0] -> Lock(0,0,(void**)&pVtx,0);                    //���z�A�h���X���擾���o�b�t�@�����b�N����

	MakeVertexPolygonFade(pD3DDevice,pVtx);
	pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[1].pos = D3DXVECTOR3(FADE_WIDTH,0.0f,0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, FADE_HEIGHT,0.0f);
	pVtx[3].pos = D3DXVECTOR3(FADE_WIDTH,FADE_HEIGHT,0.0f);

	g_pVtxBufferFadePolygon[0] -> Unlock();                                     //�o�b�t�@�̃A�����b�N

	g_ColorFade = D3DXCOLOR(0.0f,0.0f,0.0f,1.0f);                               //�t�F�[�h�J���[�̏����ݒ�
	g_fade = FADE_IN;

	return S_OK;
}

//�|���S���̍X�V
void UpdateFade(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice = Get_Device();
	PLAYER *pPlayer = GetPlayer();
	VERTEX_2DF *pVtx = NULL;                           //���_�\���̂̃|�C���^
	static int Alpha = 255;

	if(g_fade == FADE_NONE)
	{
		return;
	}

	if(g_fade == FADE_IN)
	{
		Alpha -= FADE_RATE;   //���l�����Z���Č��̂���ʂ𕂂��яオ�点��
		//�t�F�[�h�C���̏I������
		if(Alpha < 0 )
		{
			Alpha = 0.0f;
			g_fade = FADE_NONE;
		}
	}

	else if(g_fade == FADE_OUT)
	{
		Alpha += FADE_RATE;   //���l�����Z���Č��̂���ʂ������Ă���
		//�t�F�[�h�C���̏I������
		if(Alpha > 255)
		{
			Alpha = 255;
			g_fade = FADE_IN;     //�t�F�[�h�C���ɐ؂�ւ�
			SetMode(g_modeNext);
		}
	}
	
	g_ColorFade = D3DCOLOR_RGBA(255,255,255,Alpha);

	//�t�F�[�h�̌v�Z
	g_pVtxBufferFadePolygon[0]->Lock(0, 0, (void**)&pVtx, 0);       //���z�A�h���X���擾���o�b�t�@�����b�N����

	MakeVertexPolygonFade(pD3DDevice, pVtx);
	
	pVtx[0].pos = D3DXVECTOR3(0.0, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(FADE_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0, FADE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(FADE_WIDTH, FADE_HEIGHT, 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].color = g_ColorFade;
	pVtx[1].color = g_ColorFade;
	pVtx[2].color = g_ColorFade;
	pVtx[3].color = g_ColorFade;

	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	g_pVtxBufferFadePolygon[0]->Unlock();                                     //�o�b�t�@�̃A�����b�N�g
}

////////////////////////////////////////////////
//�|���S���̕`��
////////////////////////////////////////////////
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	//�w�i�F���_�t�H�[�}�b�g�ݒ�
	pD3DDevice -> SetStreamSource(0,g_pVtxBufferFadePolygon[0],0,sizeof(VERTEX_2DF));                   //VRAM����GPU�Ƀf�[�^���X�g���[��
	pD3DDevice -> SetFVF(FVF_VERTEX2D);
	pD3DDevice -> SetTexture(0,g_pTextureFadePolygon[0]);												 //�e�N�X�`���̐ݒ�

	//�A���t�@�u�����f�B���O���s��
	pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	//pD3DDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
	//���ߏ����s��
	pD3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//�����������s��
	pD3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	pD3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,0,NUM_POLYGON);                                      //�e�N�X�`���`��

}

void UninitFade(void)
{
		// �e�N�X�`���C���^�[�t�F�C�X�̕Еt��
		for (int nCnt = 0; nCnt < NUM_FADETEXTURE; nCnt++)
		{
			SAFE_RELEASE(g_pTextureFadePolygon[nCnt]);
		}
	
		//���_�o�b�t�@�C���^�[�t�F�C�X�̕Еt��
		for (int nCnt = 0; nCnt < NUM_FADEPOLYGON; nCnt++)
		{
			SAFE_RELEASE(g_pVtxBufferFadePolygon[nCnt]);
		}
}

//�w�i�|���S���̕\���ʒuX,�|���S���̕\���ʒuY,�|���S���̕�,�|���S���̍���
void MakeVertexPolygonFade(LPDIRECT3DDEVICE9 pDevice,VERTEX_2DF *pVtx)
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

//�t�F�[�h��ݒ肷��
void SetFade(FADE fade,MODE modeNext)
{
	g_fade = fade;
	g_modeNext = modeNext;
}

//fade��Ԃ��Q�b�g
FADE *GetFade(void)
{
	return &g_fade;
}
