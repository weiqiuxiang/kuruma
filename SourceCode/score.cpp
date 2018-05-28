//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
//-----------------------------------------------------------------------------------------
#include <Windows.h>
#include <string.h>
#include "main.h"
#include "score.h"
#include "player.h"
#include <stdio.h>

//-----------------------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureScorePolygon[2] = {};           //�e�N�X�`���𑀍삷�邽�߂̃|�C���^(�e�N�X�`���o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferScorePolygon = NULL;    //���_�o�b�t�@�𑀍삷�邽�߂̃|�C���^(���_�o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)
float g_score = 0;                                          //�X�R�A�̍��v

////////////////////////////////////////////////
//�|���S��(���_)�̏�����
////////////////////////////////////////////////
HRESULT InitScorePolygon(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_2D *pVtx = NULL;                           //���_�\���̂̃|�C���^
	pD3DDevice = Get_Device();

	//���_�o�b�t�@�̐���
	if(FAILED(pD3DDevice -> CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_SCOREVERTEX*(NUM_SCOREDIGIT+1),D3DUSAGE_WRITEONLY,FVF_VERTEX2D,D3DPOOL_MANAGED,&g_pVtxBufferScorePolygon,NULL)))
	{
		return E_FAIL;
	}

	//�X�R�A�e�N�X�`���̎擾(2��)
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, SCOREPOLYGON_TEXTURENAME, &g_pTextureScorePolygon[0])))
	{
		MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂����s���܂���", "�G���[", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, SCOREPOLYGON02_TEXTURENAME, &g_pTextureScorePolygon[1])))
	{
		MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂����s���܂���", "�G���[", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	//�X�R�A�̃|���S��
	g_pVtxBufferScorePolygon -> Lock(0,0,(void**)&pVtx,0);                    //���z�A�h���X���擾���o�b�t�@�����b�N����

	for(int nCnt = NUM_SCOREDIGIT - 1; nCnt >= 0; nCnt--)
	{
		MakeVertexPolygonScore(pD3DDevice,&pVtx[nCnt*4+0]);
		pVtx[nCnt*4+0].pos = D3DXVECTOR3(POS_SCORE_X+nCnt*SCORE_INTERVAL,POS_SCORE_Y,0.0f);
		pVtx[nCnt*4+1].pos = D3DXVECTOR3(POS_SCORE_X+WIDTH_SCORE+nCnt*SCORE_INTERVAL,POS_SCORE_Y,0.0f);
		pVtx[nCnt*4+2].pos = D3DXVECTOR3(POS_SCORE_X+nCnt*SCORE_INTERVAL,POS_SCORE_Y+HEIGHT_SCORE,0.0f);
		pVtx[nCnt*4+3].pos = D3DXVECTOR3(POS_SCORE_X+WIDTH_SCORE+nCnt*SCORE_INTERVAL,POS_SCORE_Y+HEIGHT_SCORE,0.0f);
	}

	MakeVertexPolygonScore(pD3DDevice,&pVtx[NUM_SCOREDIGIT*4+0]);
	pVtx[NUM_SCOREDIGIT*4+0].pos = D3DXVECTOR3(POS_SCORE_X-DISTANCE_FROM_SCORE,POS_SCORE_Y,0.0f);
	pVtx[NUM_SCOREDIGIT*4+1].pos = D3DXVECTOR3(POS_SCORE_X-DISTANCE_FROM_SCORE+WIDTH_FONT,POS_SCORE_Y,0.0f);
	pVtx[NUM_SCOREDIGIT*4+2].pos = D3DXVECTOR3(POS_SCORE_X-DISTANCE_FROM_SCORE,POS_SCORE_Y+HEIGHT_FONT,0.0f);
	pVtx[NUM_SCOREDIGIT*4+3].pos = D3DXVECTOR3(POS_SCORE_X-DISTANCE_FROM_SCORE+WIDTH_FONT,POS_SCORE_Y+HEIGHT_FONT,0.0f);

	g_pVtxBufferScorePolygon -> Unlock();                                     //�o�b�t�@�̃A�����b�N

	return S_OK;
}

//�|���S���̍X�V
void UpdateScorePolygon(void)
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

	//�w�i��̌v�Z
	g_pVtxBufferScorePolygon->Lock(0, 0, (void**)&pVtx, 0);       //���z�A�h���X���擾���o�b�t�@�����b�N����

	//�X�R�A�̃p�����[�^����
	for(int nCnt = NUM_SCOREDIGIT - 1; nCnt >= 0; nCnt--)
	{
		//�X�R�A��
		MakeVertexPolygonScore(pD3DDevice,&pVtx[nCnt*4+0]);
		pVtx[nCnt*4+0].pos = D3DXVECTOR3(POS_SCORE_X+nCnt*SCORE_INTERVAL,POS_SCORE_Y,0.0f);
		pVtx[nCnt*4+1].pos = D3DXVECTOR3(POS_SCORE_X+WIDTH_SCORE+nCnt*SCORE_INTERVAL,POS_SCORE_Y,0.0f);
		pVtx[nCnt*4+2].pos = D3DXVECTOR3(POS_SCORE_X+nCnt*SCORE_INTERVAL,POS_SCORE_Y+HEIGHT_SCORE,0.0f);
		pVtx[nCnt*4+3].pos = D3DXVECTOR3(POS_SCORE_X+WIDTH_SCORE+nCnt*SCORE_INTERVAL,POS_SCORE_Y+HEIGHT_SCORE,0.0f);

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

		nDigit--;
	}

	MakeVertexPolygonScore(pD3DDevice,&pVtx[NUM_SCOREDIGIT*4+0]);
	pVtx[NUM_SCOREDIGIT*4+0].pos = D3DXVECTOR3(POS_SCORE_X-DISTANCE_FROM_SCORE,POS_SCORE_Y,0.0f);
	pVtx[NUM_SCOREDIGIT*4+1].pos = D3DXVECTOR3(POS_SCORE_X-DISTANCE_FROM_SCORE+WIDTH_FONT,POS_SCORE_Y,0.0f);
	pVtx[NUM_SCOREDIGIT*4+2].pos = D3DXVECTOR3(POS_SCORE_X-DISTANCE_FROM_SCORE,POS_SCORE_Y+HEIGHT_FONT,0.0f);
	pVtx[NUM_SCOREDIGIT*4+3].pos = D3DXVECTOR3(POS_SCORE_X-DISTANCE_FROM_SCORE+WIDTH_FONT,POS_SCORE_Y+HEIGHT_FONT,0.0f);

	pVtx[NUM_SCOREDIGIT*4+0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[NUM_SCOREDIGIT*4+1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[NUM_SCOREDIGIT*4+2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[NUM_SCOREDIGIT*4+3].tex = D3DXVECTOR2(1.0f,1.0f);

	g_pVtxBufferScorePolygon->Unlock();                                     //�o�b�t�@�̃A�����b�N
}

////////////////////////////////////////////////
//�|���S���̕`��
////////////////////////////////////////////////
void DrawScorePolygon(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	//�w�i�̋�F���_�t�H�[�}�b�g�ݒ�
	pD3DDevice -> SetStreamSource(0,g_pVtxBufferScorePolygon,0,sizeof(VERTEX_2D));                   //VRAM����GPU�Ƀf�[�^���X�g���[��
	pD3DDevice -> SetFVF(FVF_VERTEX2D);
	pD3DDevice -> SetTexture(0,g_pTextureScorePolygon[0]);												 //�e�N�X�`���̐ݒ�
	for(int nCnt = 0; nCnt < NUM_SCOREDIGIT; nCnt++)
	{
		pD3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,nCnt*4,NUM_POLYGON);                                  //�e�N�X�`���������̊m��
	}

	pD3DDevice -> SetTexture(0,g_pTextureScorePolygon[1]);												 //�e�N�X�`���̐ݒ�
	pD3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,NUM_SCOREDIGIT*4,NUM_POLYGON); 
}

void UninitScorePolygon(void)
{
	for(int nCnt = 0; nCnt < 2; nCnt++)
	{
		// �e�N�X�`���C���^�[�t�F�C�X�̕Еt��
		SAFE_RELEASE(g_pTextureScorePolygon[nCnt]);
	}

	//���_�o�b�t�@�C���^�[�t�F�C�X�̕Еt��
	SAFE_RELEASE(g_pVtxBufferScorePolygon);
}

//�w�i�|���S���̕\���ʒuX,�|���S���̕\���ʒuY,�|���S���̕�,�|���S���̍���
void MakeVertexPolygonScore(LPDIRECT3DDEVICE9 pDevice,VERTEX_2D *pVtx)
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