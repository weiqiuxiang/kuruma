//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
//-----------------------------------------------------------------------------------------
#include <Windows.h>
#include <string.h>
#include <stdio.h>
#include "main.h"
#include "result.h"
#include "player.h"
#include "input.h"
#include "fade.h"

//-----------------------------------------------------------------------------------------
//�}�N��
//-----------------------------------------------------------------------------------------
#define BIG (2)                         //�g��{��
#define RESULT_POS_SCORE_X (430.0f)
#define RESULT_POS_SCORE_Y (250.0f)
#define RESULT_SCORE_WIDTH (12.0f*BIG)
#define RESULT_SCORE_HEIGHT (24.0f*BIG)
#define RESULT_SCORE_INTERVAL (12.0f*BIG)

#define RESULT_FONT_WIDTH (80.0f*BIG)
#define RESULT_FONT_HEIGHT (24.0f*BIG)
#define RESULT_DISTANCE_FROM_SCORE (125.0f*BIG)

//-----------------------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureResultPolygon[NUM_RESULTTEXTURE] = {};           //�e�N�X�`���𑀍삷�邽�߂̃|�C���^(�e�N�X�`���o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferResultPolygon = NULL;    //���_�o�b�t�@�𑀍삷�邽�߂̃|�C���^(���_�o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)

////////////////////////////////////////////////
//�|���S��(���_)�̏�����
////////////////////////////////////////////////
HRESULT InitResult(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_2D *pVtx = NULL;                           //���_�\���̂̃|�C���^
	pD3DDevice = Get_Device();

	//���_�o�b�t�@�̐���
	if(FAILED(pD3DDevice -> CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_RESULTVERTEX*NUM_RESULTPOLYGON,D3DUSAGE_WRITEONLY,FVF_VERTEX2D,D3DPOOL_MANAGED,&g_pVtxBufferResultPolygon,NULL)))
	{
		return E_FAIL;
	}

	//result�e�N�X�`���̎擾(3��)
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, RESULTPOLYGON_TEXTURENAME, &g_pTextureResultPolygon[0])))
	{
		MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂����s���܂���", "�G���[", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, RESULTPOLYGON02_TEXTURENAME, &g_pTextureResultPolygon[1])))
	{
		MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂����s���܂���", "�G���[", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, RESULTPOLYGON03_TEXTURENAME, &g_pTextureResultPolygon[2])))
	{
		MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂����s���܂���", "�G���[", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, RESULTPOLYGON04_TEXTURENAME, &g_pTextureResultPolygon[3])))
	{
		MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂����s���܂���", "�G���[", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	//�X�R�A�̃|���S��
	g_pVtxBufferResultPolygon -> Lock(0,0,(void**)&pVtx,0);                    //���z�A�h���X���擾���o�b�t�@�����b�N����
	MakeVertexPolygonResult(pD3DDevice,&pVtx[0]);

	pVtx[0].pos = D3DXVECTOR3(275,200,0.0f);
	pVtx[1].pos = D3DXVECTOR3(275+WIDTH_RESULT,200,0.0f);
	pVtx[2].pos = D3DXVECTOR3(275,200+HEIGHT_RESULT,0.0f);
	pVtx[3].pos = D3DXVECTOR3(275+WIDTH_RESULT,200+HEIGHT_RESULT,0.0f);

	pVtx += 4;

	pVtx[0].pos = D3DXVECTOR3(0,0,0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH,0,0.0f);
	pVtx[2].pos = D3DXVECTOR3(0,SCREEN_HEIGHT,0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH,SCREEN_HEIGHT,0.0f);

	g_pVtxBufferResultPolygon -> Unlock();                                     //�o�b�t�@�̃A�����b�N

	return S_OK;
}

//�|���S���̍X�V
void UpdateResult(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice = Get_Device();
	VERTEX_2D *pVtx = NULL;                           //���_�\���̂̃|�C���^
	PLAYER *pPlayer = GetPlayer();                    //�v���[���[�̃X�e�[�^�X�̎擾

	sprintf( &(pPlayer -> aScore[0]),"%f",pPlayer -> fScore);
	pPlayer -> aScore[9] = '\0';

	char aScore[20];
	int nDigit = 0;                                   //����
	int score;                                        //�X�R�A���i�[����ϐ�
	strcpy(&aScore[0],&pPlayer -> aScore[0]);

	for(;nDigit < 8;nDigit++)
	{
		if(aScore[nDigit] == 0x2e) break;
	}

	//result�|���S���̕\��
	g_pVtxBufferResultPolygon->Lock(0, 0, (void**)&pVtx, 0);       //���z�A�h���X���擾���o�b�t�@�����b�N����

	MakeVertexPolygonResult(pD3DDevice,&pVtx[0]);

	//----------------
	//���U���g�t�H���g
	//----------------
	pVtx[0].pos = D3DXVECTOR3(RESULT_POSX,RESULT_POSY,0.0f);
	pVtx[1].pos = D3DXVECTOR3(RESULT_POSX+WIDTH_RESULT,RESULT_POSY,0.0f);
	pVtx[2].pos = D3DXVECTOR3(RESULT_POSX,RESULT_POSY+HEIGHT_RESULT,0.0f);
	pVtx[3].pos = D3DXVECTOR3(RESULT_POSX+WIDTH_RESULT,RESULT_POSY+HEIGHT_RESULT,0.0f);

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
	
	pVtx += 4;

	//----------------------
	//�X�R�A�̃p�����[�^����
	//----------------------
	for(int nCnt = NUM_SCOREDIGIT - 1; nCnt >= 0; nCnt--)
	{
		//�X�R�A��
		pVtx[nCnt*4+0].pos = D3DXVECTOR3(RESULT_POS_SCORE_X+nCnt*RESULT_SCORE_INTERVAL,RESULT_POS_SCORE_Y,0.0f);
		pVtx[nCnt*4+1].pos = D3DXVECTOR3(RESULT_POS_SCORE_X+RESULT_SCORE_WIDTH+nCnt*RESULT_SCORE_INTERVAL,RESULT_POS_SCORE_Y,0.0f);
		pVtx[nCnt*4+2].pos = D3DXVECTOR3(RESULT_POS_SCORE_X+nCnt*RESULT_SCORE_INTERVAL,RESULT_POS_SCORE_Y+RESULT_SCORE_HEIGHT,0.0f);
		pVtx[nCnt*4+3].pos = D3DXVECTOR3(RESULT_POS_SCORE_X+RESULT_SCORE_WIDTH+nCnt*RESULT_SCORE_INTERVAL,RESULT_POS_SCORE_Y+RESULT_SCORE_HEIGHT,0.0f);

		//�X�R�A�̌�������0�ɂȂ邩�ǂ���
		if(nDigit > 0)
		{
			score = aScore[nDigit-1] - 48;
		}

		else
		{
			score = 0;
		}

		pVtx[nCnt*4+0].tex = D3DXVECTOR2(0.1*score,0.0f);
		pVtx[nCnt*4+1].tex = D3DXVECTOR2(0.1*score+0.1f,0.0f);
		pVtx[nCnt*4+2].tex = D3DXVECTOR2(0.1*score,1.0f);
		pVtx[nCnt*4+3].tex = D3DXVECTOR2(0.1*score+0.1f,1.0f);

		pVtx[nCnt*4+0].rhw = 1.0f;
		pVtx[nCnt*4+1].rhw = 1.0f;
		pVtx[nCnt*4+2].rhw = 1.0f;
		pVtx[nCnt*4+3].rhw = 1.0f;

		pVtx[nCnt*4+0].color = D3DCOLOR_RGBA(255,255,255,255);
		pVtx[nCnt*4+1].color = D3DCOLOR_RGBA(255,255,255,255);
		pVtx[nCnt*4+2].color = D3DCOLOR_RGBA(255,255,255,255);
		pVtx[nCnt*4+3].color = D3DCOLOR_RGBA(255,255,255,255);

		nDigit--;
	}

	//----------------------
	//�X�R�A�t�H���g
	//----------------------
	pVtx[NUM_SCOREDIGIT*4+0].pos = D3DXVECTOR3(RESULT_POS_SCORE_X-RESULT_DISTANCE_FROM_SCORE,RESULT_POS_SCORE_Y,0.0f);
	pVtx[NUM_SCOREDIGIT*4+1].pos = D3DXVECTOR3(RESULT_POS_SCORE_X-RESULT_DISTANCE_FROM_SCORE+RESULT_FONT_WIDTH,RESULT_POS_SCORE_Y,0.0f);
	pVtx[NUM_SCOREDIGIT*4+2].pos = D3DXVECTOR3(RESULT_POS_SCORE_X-RESULT_DISTANCE_FROM_SCORE,RESULT_POS_SCORE_Y+RESULT_FONT_HEIGHT,0.0f);
	pVtx[NUM_SCOREDIGIT*4+3].pos = D3DXVECTOR3(RESULT_POS_SCORE_X-RESULT_DISTANCE_FROM_SCORE+RESULT_FONT_WIDTH,RESULT_POS_SCORE_Y+RESULT_FONT_HEIGHT,0.0f);

	pVtx[NUM_SCOREDIGIT*4+0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[NUM_SCOREDIGIT*4+1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[NUM_SCOREDIGIT*4+2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[NUM_SCOREDIGIT*4+3].tex = D3DXVECTOR2(1.0f,1.0f);

	pVtx[NUM_SCOREDIGIT*4+0].rhw = 1.0f;
	pVtx[NUM_SCOREDIGIT*4+1].rhw = 1.0f;
	pVtx[NUM_SCOREDIGIT*4+2].rhw = 1.0f;
	pVtx[NUM_SCOREDIGIT*4+3].rhw = 1.0f;

	pVtx[NUM_SCOREDIGIT*4+0].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[NUM_SCOREDIGIT*4+1].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[NUM_SCOREDIGIT*4+2].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[NUM_SCOREDIGIT*4+3].color = D3DCOLOR_RGBA(255,255,255,255);

	//�w�i
	pVtx[(NUM_SCOREDIGIT+1)*4+0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[(NUM_SCOREDIGIT+1)*4+1].pos = D3DXVECTOR3(SCREEN_WIDTH,0.0f,0.0f);
	pVtx[(NUM_SCOREDIGIT+1)*4+2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT,0.0f);
	pVtx[(NUM_SCOREDIGIT+1)*4+3].pos = D3DXVECTOR3(SCREEN_WIDTH,SCREEN_HEIGHT,0.0f);

	pVtx[(NUM_SCOREDIGIT+1)*4+0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[(NUM_SCOREDIGIT+1)*4+1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[(NUM_SCOREDIGIT+1)*4+2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[(NUM_SCOREDIGIT+1)*4+3].tex = D3DXVECTOR2(1.0f,1.0f);

	pVtx[(NUM_SCOREDIGIT+1)*4+0].rhw = 1.0f;
	pVtx[(NUM_SCOREDIGIT+1)*4+1].rhw = 1.0f;
	pVtx[(NUM_SCOREDIGIT+1)*4+2].rhw = 1.0f;
	pVtx[(NUM_SCOREDIGIT+1)*4+3].rhw = 1.0f;

	pVtx[(NUM_SCOREDIGIT+1)*4+0].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[(NUM_SCOREDIGIT+1)*4+1].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[(NUM_SCOREDIGIT+1)*4+2].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[(NUM_SCOREDIGIT+1)*4+3].color = D3DCOLOR_RGBA(255,255,255,255);

	g_pVtxBufferResultPolygon->Unlock();                                     //�o�b�t�@�̃A�����b�N

	if(GetControllerTrigger(0) || GetKeyboardTrigger(DIK_J)) 
	{
		SetFade(FADE_OUT,MODE_TITLE);
	}
}

////////////////////////////////////////////////
//�|���S���̕`��
////////////////////////////////////////////////
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	//���_�t�H�[�}�b�g�ݒ�
	pD3DDevice -> SetStreamSource(0,g_pVtxBufferResultPolygon,0,sizeof(VERTEX_2D));                   //VRAM����GPU�Ƀf�[�^���X�g���[��
	pD3DDevice -> SetFVF(FVF_VERTEX2D);

	//�w�i
	pD3DDevice -> SetTexture(0,g_pTextureResultPolygon[3]);												 //�e�N�X�`���̐ݒ�
	pD3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,(NUM_SCOREDIGIT+2)*4,NUM_POLYGON);

	//���U���g�t�H���g
	pD3DDevice -> SetTexture(0,g_pTextureResultPolygon[0]);												 //�e�N�X�`���̐ݒ�
	pD3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,0,NUM_POLYGON);                                  //�e�N�X�`���������̊m��

	//�X�R�A
	pD3DDevice -> SetTexture(0,g_pTextureResultPolygon[1]);												 //�e�N�X�`���̐ݒ�
	for(int nCnt = 1; nCnt < NUM_SCOREDIGIT+1; nCnt++)
	{
		pD3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,nCnt*4,NUM_POLYGON);                                  //�e�N�X�`���������̊m��
	}

	//�X�R�A�t�H���g
	pD3DDevice -> SetTexture(0,g_pTextureResultPolygon[2]);												 //�e�N�X�`���̐ݒ�
	pD3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,(NUM_SCOREDIGIT+1)*4,NUM_POLYGON); 
}

void UninitResult(void)
{
	// �e�N�X�`���C���^�[�t�F�C�X�̕Еt��
	for(int nCnt = 0; nCnt < NUM_RESULTTEXTURE; nCnt++)
	{
		SAFE_RELEASE(g_pTextureResultPolygon[nCnt]);
	}

	//���_�o�b�t�@�C���^�[�t�F�C�X�̕Еt��
	SAFE_RELEASE(g_pVtxBufferResultPolygon);
}

//�w�i�|���S���̕\���ʒuX,�|���S���̕\���ʒuY,�|���S���̕�,�|���S���̍���
void MakeVertexPolygonResult(LPDIRECT3DDEVICE9 pDevice,VERTEX_2D *pVtx)
{
	pDevice -> SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);   //����1:�ݒ薼�J�����O ����2:�ݒ�l(D3DCULL_CCW:�t���v���𗠖ʂ�����)
	pDevice -> SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	pDevice -> SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	pDevice -> SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

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