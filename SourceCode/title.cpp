//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
//-----------------------------------------------------------------------------------------
#include <Windows.h>
#include <string.h>
#include "main.h"
#include "title.h"
#include "player.h"
#include "input.h"
#include "fade.h"
#include <math.h>

//-----------------------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureTitlePolygon[NUM_TITLETEXTURE] = {};           //�e�N�X�`���𑀍삷�邽�߂̃|�C���^(�e�N�X�`���o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferTitlePolygon = NULL;    //���_�o�b�t�@�𑀍삷�邽�߂̃|�C���^(���_�o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)
bool g_StickInputYes = false;

////////////////////////////////////////////////
//�|���S��(���_)�̏�����
////////////////////////////////////////////////
HRESULT InitTitle(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_2D *pVtx = NULL;                           //���_�\���̂̃|�C���^
	pD3DDevice = Get_Device();

	//���_�o�b�t�@�̐���
	if(FAILED(pD3DDevice -> CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_TITLEVERTEX*NUM_TITLEPOLYGON,D3DUSAGE_WRITEONLY,FVF_VERTEX2D,D3DPOOL_MANAGED,&g_pVtxBufferTitlePolygon,NULL)))
	{
		return E_FAIL;
	}

	//�^�C�g���e�N�X�`���̎擾(5��)

	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, TITLEPOLYGON00_TEXTURENAME, &g_pTextureTitlePolygon[0])))
	{
		MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂����s���܂���", "�G���[", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, TITLEPOLYGON01_TEXTURENAME, &g_pTextureTitlePolygon[1])))
	{
		MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂����s���܂���", "�G���[", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, TITLEPOLYGON02_TEXTURENAME, &g_pTextureTitlePolygon[2])))
	{
		MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂����s���܂���", "�G���[", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	//�^�C�g���|���S��
	g_pVtxBufferTitlePolygon -> Lock(0,0,(void**)&pVtx,0);                    //���z�A�h���X���擾���o�b�t�@�����b�N����
	MakeVertexPolygonTitle(pD3DDevice,&pVtx[0]);
	pVtx[0].pos = D3DXVECTOR3(0,0,0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH,0,0.0f);
	pVtx[2].pos = D3DXVECTOR3(0,SCREEN_HEIGHT,0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH,SCREEN_HEIGHT,0.0f);

	pVtx += 4;

	//���[�h�̑I���A�C�R��
	pVtx[0].pos = D3DXVECTOR3(ICON_POSX,ICON_POSY,0.0f);
	pVtx[1].pos = D3DXVECTOR3(ICON_POSX+WIDTH_CURSOR,ICON_POSY,0.0f);
	pVtx[2].pos = D3DXVECTOR3(ICON_POSX,ICON_POSY+HEIGHT_CURSOR,0.0f);
	pVtx[3].pos = D3DXVECTOR3(ICON_POSX+WIDTH_CURSOR,ICON_POSY+HEIGHT_CURSOR,0.0f);

	g_pVtxBufferTitlePolygon -> Unlock();                                     //�o�b�t�@�̃A�����b�N

	g_StickInputYes = false;

	return S_OK;
}

//�|���S���̍X�V
void UpdateTitle(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice = Get_Device();
	VERTEX_2D *pVtx = NULL;                           //���_�\���̂̃|�C���^

	//�^�C�g���|���S��
	g_pVtxBufferTitlePolygon -> Lock(0,0,(void**)&pVtx,0);                    //���z�A�h���X���擾���o�b�t�@�����b�N����

	MakeVertexPolygonTitle(pD3DDevice,&pVtx[0]);

	pVtx[0].pos = D3DXVECTOR3(0,0,0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH,0,0.0f);
	pVtx[2].pos = D3DXVECTOR3(0,SCREEN_HEIGHT,0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH,SCREEN_HEIGHT,0.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[1].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[2].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[3].color = D3DCOLOR_RGBA(255,255,255,255);

	pVtx += 4;

	//���[�h�̑I���A�C�R��
	pVtx[0].pos = D3DXVECTOR3(ICON_POSX,ICON_POSY,0.0f);
	pVtx[1].pos = D3DXVECTOR3(ICON_POSX+WIDTH_CURSOR,ICON_POSY,0.0f);
	pVtx[2].pos = D3DXVECTOR3(ICON_POSX,ICON_POSY+HEIGHT_CURSOR,0.0f);
	pVtx[3].pos = D3DXVECTOR3(ICON_POSX+WIDTH_CURSOR,ICON_POSY+HEIGHT_CURSOR,0.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[1].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[2].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[3].color = D3DCOLOR_RGBA(255,255,255,255);

	//test
	DIJOYSTATE *pjs = GetJS();

	//�J�[�\���̃|���S��
	static int nLoop = 0;												//���j���[�I��p�ϐ�

	if (fabs((double)pjs->lY) < 500) { g_StickInputYes = false; }
	if (!g_StickInputYes) {
		if (pjs->lY < -500) { nLoop--; g_StickInputYes = true; }
		if (pjs->lY > 500) { nLoop++; g_StickInputYes = true;}
	}
	if (GetKeyboardTrigger(DIK_W)) nLoop--;
	if (GetKeyboardTrigger(DIK_S)) nLoop++;
	if (nLoop < 0) nLoop = 1;
	nLoop = nLoop % 2;

	if(nLoop == 0)
	{
		pVtx[0].pos = D3DXVECTOR3(START_POSX - DISTANCE_FROM_START,START_POSY,0.0f);
		pVtx[1].pos = D3DXVECTOR3(START_POSX - DISTANCE_FROM_START+WIDTH_CURSOR,START_POSY,0.0f);
		pVtx[2].pos = D3DXVECTOR3(START_POSX - DISTANCE_FROM_START,START_POSY+HEIGHT_CURSOR,0.0f);
		pVtx[3].pos = D3DXVECTOR3(START_POSX - DISTANCE_FROM_START+WIDTH_CURSOR,START_POSY+HEIGHT_CURSOR,0.0f);

		if(GetControllerTrigger(0) || GetKeyboardTrigger(DIK_J))
		{
			SetFade(FADE_OUT,MODE_TUTORIAL);
		}
	}

	else
	{
		pVtx[0].pos = D3DXVECTOR3(EXIT_POSX - DISTANCE_FROM_EXIT,EXIT_POSY,0.0f);
		pVtx[1].pos = D3DXVECTOR3(EXIT_POSX - DISTANCE_FROM_EXIT+WIDTH_CURSOR,EXIT_POSY,0.0f);
		pVtx[2].pos = D3DXVECTOR3(EXIT_POSX - DISTANCE_FROM_EXIT,EXIT_POSY+HEIGHT_CURSOR,0.0f);
		pVtx[3].pos = D3DXVECTOR3(EXIT_POSX - DISTANCE_FROM_EXIT+WIDTH_CURSOR,EXIT_POSY+HEIGHT_CURSOR,0.0f);

		if(GetControllerTrigger(0) || GetKeyboardTrigger(DIK_J))
		{
			Uninit();                     //�I������
			exit(0);
		}
	}

	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[1].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[2].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[3].color = D3DCOLOR_RGBA(255,255,255,255);

	pVtx += 4;

	pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH,0.0f,0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT,0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH,SCREEN_HEIGHT,0.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[1].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[2].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[3].color = D3DCOLOR_RGBA(255,255,255,255);

	g_pVtxBufferTitlePolygon -> Unlock();                                     //�o�b�t�@�̃A�����b�N
}

////////////////////////////////////////////////
//�|���S���̕`��
////////////////////////////////////////////////
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	//���_�t�H�[�}�b�g�ݒ�
	pD3DDevice -> SetStreamSource(0,g_pVtxBufferTitlePolygon,0,sizeof(VERTEX_2D));                   //VRAM����GPU�Ƀf�[�^���X�g���[��
	pD3DDevice -> SetFVF(FVF_VERTEX2D);

	pD3DDevice -> SetTexture(0,g_pTextureTitlePolygon[NUM_TITLETEXTURE-1]);
	pD3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,(NUM_TITLETEXTURE-1)*4,NUM_POLYGON);

	for(int nCnt = 0; nCnt < NUM_TITLETEXTURE-1; nCnt++)
	{
		pD3DDevice -> SetTexture(0,g_pTextureTitlePolygon[nCnt]);
		pD3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,nCnt*4,NUM_POLYGON);                                  //�e�N�X�`���������̊m��
	}
}

//�I���֐�
void UninitTitle(void)
{
	// �e�N�X�`���C���^�[�t�F�C�X�̕Еt��
	for(int nCnt = 0; nCnt < NUM_TITLETEXTURE; nCnt++)
	{
		SAFE_RELEASE(g_pTextureTitlePolygon[nCnt]);
	}

	//���_�o�b�t�@�C���^�[�t�F�C�X�̕Еt��
	SAFE_RELEASE(g_pVtxBufferTitlePolygon);
}

//�w�i�|���S���̕\���ʒuX,�|���S���̕\���ʒuY,�|���S���̕�,�|���S���̍���
void MakeVertexPolygonTitle(LPDIRECT3DDEVICE9 pDevice,VERTEX_2D *pVtx)
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