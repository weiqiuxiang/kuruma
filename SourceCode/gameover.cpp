//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
//-----------------------------------------------------------------------------------------
#include <Windows.h>
#include "main.h"
#include "gameover.h"
#include "player.h"
#include "fade.h"
#include "input.h"
//-----------------------------------------------------------------------------------------
//�}�N��
//-----------------------------------------------------------------------------------------
#define GAMEOVER_SHOW_POSX (270)             //�t�H���g�\���ʒu��X��
#define GAMEOVER_SHOW_POSY (220)             //�t�H���g�\���ʒu��Y��
#define GAMEOVER_SHOW_ALPHA_RATE (1)         //�t�H���g�\���̃����[�g

#define PAUSE_POSX (210.0f)
#define PAUSE_POSY (150.0f) 
#define PAUSE_WIDTH (384.0f)
#define PAUSE_HEIGHT (256.0f) 

#define SELECT_BAR_POSX (210.0f)
#define SELECT_BAR_POSY (200.0f)

#define SELECT_BAR_WIDTH (384.0f)
#define SELECT_BAR_HEIGHT (55.0f) 

#define BAR_MOVE (55.5f)

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------------------
void UpdateGameoverPolygon(void);

//-----------------------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureGameoverPolygon[NUM_GAMEOVERTEXTURE] = {};           //�e�N�X�`���𑀍삷�邽�߂̃|�C���^(�e�N�X�`���o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferGameoverPolygon               = NULL;         //���_�o�b�t�@�𑀍삷�邽�߂̃|�C���^(���_�o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)
int g_Alpha;                                                                      //���l���R���g���[���ϐ�
D3DCOLOR g_Color;                                						          //�J���[
int g_nCntBar = 0;																  //pause�̑I���o�[�̈ʒu

////////////////////////////////////////////////
//�|���S��(���_)�̏�����
////////////////////////////////////////////////
HRESULT InitGameover(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_2D *pVtx = NULL;                           //���_�\���̂̃|�C���^
	pD3DDevice = Get_Device();

	//------------------
	//���_�o�b�t�@�̐���
	//------------------
	if(FAILED(pD3DDevice -> CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_GAMEOVERVERTEX*NUM_GAMEOVERPOLYGON,D3DUSAGE_WRITEONLY,FVF_VERTEX2D,D3DPOOL_MANAGED,&g_pVtxBufferGameoverPolygon,NULL)))
	{
		return E_FAIL;
	}

	//------------------
	//�e�N�X�`���̎擾
	//------------------
	if ( FAILED( D3DXCreateTextureFromFile(pD3DDevice, GAMEOVERPOLYGON01_TEXTURENAME,&g_pTextureGameoverPolygon[0]) ) )
	{
		MessageBox(NULL,"�e�N�X�`���̓ǂݍ��݂����s���܂���","�G���[",MB_OK|MB_ICONHAND);
		return E_FAIL;
	}

	if ( FAILED( D3DXCreateTextureFromFile(pD3DDevice, PAUSEPOLYGON01_TEXTURENAME,&g_pTextureGameoverPolygon[1]) ) )
	{
		MessageBox(NULL,"�e�N�X�`���̓ǂݍ��݂����s���܂���","�G���[",MB_OK|MB_ICONHAND);
		return E_FAIL;
	}

	if ( FAILED( D3DXCreateTextureFromFile(pD3DDevice, PAUSEPOLYGON02_TEXTURENAME,&g_pTextureGameoverPolygon[2]) ) )
	{
		MessageBox(NULL,"�e�N�X�`���̓ǂݍ��݂����s���܂���","�G���[",MB_OK|MB_ICONHAND);
		return E_FAIL;
	}

	if ( FAILED( D3DXCreateTextureFromFile(pD3DDevice, PAUSEPOLYGON03_TEXTURENAME,&g_pTextureGameoverPolygon[3]) ) )
	{
		MessageBox(NULL,"�e�N�X�`���̓ǂݍ��݂����s���܂���","�G���[",MB_OK|MB_ICONHAND);
		return E_FAIL;
	}

	if ( FAILED( D3DXCreateTextureFromFile(pD3DDevice, PAUSEPOLYGON04_TEXTURENAME,&g_pTextureGameoverPolygon[4]) ) )
	{
		MessageBox(NULL,"�e�N�X�`���̓ǂݍ��݂����s���܂���","�G���[",MB_OK|MB_ICONHAND);
		return E_FAIL;
	}

	//���_�o�b�t�@���b�N
	g_pVtxBufferGameoverPolygon -> Lock(0,0,(void**)&pVtx,0);                    //���z�A�h���X���擾���o�b�t�@�����b�N����

	MakeVertexPolygonGameover(pD3DDevice,pVtx);
	pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[1].pos = D3DXVECTOR3(GAMEOVER_WIDTH,0.0f,0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, GAMEOVER_HEIGHT,0.0f);
	pVtx[3].pos = D3DXVECTOR3(GAMEOVER_WIDTH,GAMEOVER_HEIGHT,0.0f);

	g_pVtxBufferGameoverPolygon -> Unlock();                                     //�o�b�t�@�̃A�����b�N

	g_Alpha = 0;                                                                 //���l�̏�����
	g_Color = D3DCOLOR_RGBA(255,255,255,255);                                    //�J���[�̏�����
	g_nCntBar = 0;                                                               //pause�̑I���o�[�̈ʒu

	return S_OK;
}

//�|���S���̍X�V
void UpdateGameover(void)
{
	PLAYER *pPlayer = GetPlayer();
	//�f�[�^�擾
	GAMEMANAGER *pGameManager = GetManager();

	//----------------
	//�Q�[���I�[�o�[��
	//----------------
	if(pGameManager -> bGameOver == true)
	{
		//------------------
		//�f�[�^�����̍X�V
		//------------------
		g_Alpha += GAMEOVER_SHOW_ALPHA_RATE;

		if(g_Alpha > 255)
		{
			g_Alpha = 255;
			SetFade(FADE_OUT,MODE_RESULT);
		}
		g_Color = D3DCOLOR_RGBA(255,255,255,g_Alpha);

	}

	//------------
	//�Q�[�����s��
	//------------
	else
	{
		//Pause�Ď�
		if(GetControllerTrigger(7))
		{
			//�Q�[���͔�|�[�Y���
			if(pGameManager -> bGamePause == false)
			{
				pGameManager -> bGamePause = true;
			}
			//�|�[�Y���
			else
			{
				pGameManager -> bGamePause = false;
			}
		}
	}

	//--------------------
	//�I���o�[�̈ʒu�̕ύX
	//--------------------
	if(pGameManager -> bGamePause == true)
	{
		if(GetControllerTrigger(4))
		{
			g_nCntBar--;
		
		}
		if(g_nCntBar < 0)
		{
			g_nCntBar = 2;
		}

		if(GetControllerTrigger(5))
		{
			g_nCntBar++;
		
		}
		if(g_nCntBar > 2)
		{
			g_nCntBar = 0;
		}

		if(GetControllerTrigger(0))
		{
			switch(g_nCntBar)
			{
			case 0:
				pGameManager -> bGamePause = false;
				break;
			case 1:
				SetFade(FADE_OUT,MODE_TITLE);   //�^�C�g���ɑJ��
				break;
			case 2:
				exit(0);                        //�Q�[���I��
				break;
			}
		}
	}

	else
	{
		g_nCntBar = 0;
	}

	UpdateGameoverPolygon();                         //gameover�|���S���̍X�V
}

void UpdateGameoverPolygon(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice = Get_Device();
	VERTEX_2D *pVtx = NULL;                           //���_�\���̂̃|�C���^
	GAMEMANAGER *pGameManager = GetManager();

	//------------------
	//�|���S�������̍X�V
	//------------------
	g_pVtxBufferGameoverPolygon->Lock(0, 0, (void**)&pVtx, 0);       //���z�A�h���X���擾���o�b�t�@�����b�N����

	MakeVertexPolygonGameover(pD3DDevice, pVtx);

	pVtx[0].pos = D3DXVECTOR3(GAMEOVER_SHOW_POSX,GAMEOVER_SHOW_POSY,0.0f);
	pVtx[1].pos = D3DXVECTOR3(GAMEOVER_SHOW_POSX+GAMEOVER_WIDTH,GAMEOVER_SHOW_POSY,0.0f);
	pVtx[2].pos = D3DXVECTOR3(GAMEOVER_SHOW_POSX, GAMEOVER_HEIGHT+GAMEOVER_SHOW_POSY,0.0f);
	pVtx[3].pos = D3DXVECTOR3(GAMEOVER_SHOW_POSX+GAMEOVER_WIDTH,GAMEOVER_HEIGHT+GAMEOVER_SHOW_POSY,0.0f);

	//�Q�[���I�[�o�[
	if(pGameManager -> nGameResult == 1)
	{
		pVtx[0].tex = D3DXVECTOR2(0.0f,0.5f);
		pVtx[1].tex = D3DXVECTOR2(1.0f,0.5f);
		pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);
	}

	//�Q�[���N���A
	if(pGameManager -> nGameResult == 2)
	{
		pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f,0.5f);
		pVtx[3].tex = D3DXVECTOR2(1.0f,0.5f);
	}

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].color = g_Color;
	pVtx[1].color = g_Color;
	pVtx[2].color = g_Color;
	pVtx[3].color = g_Color;

	pVtx += 4;

	//pause�O���g
	pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH,0.0f,0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f,SCREEN_HEIGHT,0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH,SCREEN_HEIGHT,0.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].color = D3DCOLOR_RGBA(255,255,255,127);
	pVtx[1].color = D3DCOLOR_RGBA(255,255,255,127);
	pVtx[2].color = D3DCOLOR_RGBA(255,255,255,127);
	pVtx[3].color = D3DCOLOR_RGBA(255,255,255,127);

	pVtx += 4;

	//pause�g
	pVtx[0].pos = D3DXVECTOR3(PAUSE_POSX,PAUSE_POSY,0.0f);
	pVtx[1].pos = D3DXVECTOR3(PAUSE_POSX+PAUSE_WIDTH,PAUSE_POSY,0.0f);
	pVtx[2].pos = D3DXVECTOR3(PAUSE_POSX,PAUSE_POSY+PAUSE_HEIGHT,0.0f);
	pVtx[3].pos = D3DXVECTOR3(PAUSE_POSX+PAUSE_WIDTH,PAUSE_POSY+PAUSE_HEIGHT,0.0f);

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

	//pause�t�H���g
	pVtx[0].pos = D3DXVECTOR3(PAUSE_POSX,PAUSE_POSY,0.0f);
	pVtx[1].pos = D3DXVECTOR3(PAUSE_POSX+PAUSE_WIDTH,PAUSE_POSY,0.0f);
	pVtx[2].pos = D3DXVECTOR3(PAUSE_POSX,PAUSE_POSY+PAUSE_HEIGHT,0.0f);
	pVtx[3].pos = D3DXVECTOR3(PAUSE_POSX+PAUSE_WIDTH,PAUSE_POSY+PAUSE_HEIGHT,0.0f);

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

	//�I���o�[
	pVtx[0].pos = D3DXVECTOR3(SELECT_BAR_POSX,SELECT_BAR_POSY+BAR_MOVE*g_nCntBar,0.0f);
	pVtx[1].pos = D3DXVECTOR3(SELECT_BAR_POSX+SELECT_BAR_WIDTH,SELECT_BAR_POSY+BAR_MOVE*g_nCntBar,0.0f);
	pVtx[2].pos = D3DXVECTOR3(SELECT_BAR_POSX,SELECT_BAR_POSY+SELECT_BAR_HEIGHT+BAR_MOVE*g_nCntBar,0.0f);
	pVtx[3].pos = D3DXVECTOR3(SELECT_BAR_POSX+SELECT_BAR_WIDTH,SELECT_BAR_POSY+SELECT_BAR_HEIGHT+BAR_MOVE*g_nCntBar,0.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].color = D3DCOLOR_RGBA(255,255,0,127);
	pVtx[1].color = D3DCOLOR_RGBA(255,255,0,127);
	pVtx[2].color = D3DCOLOR_RGBA(255,255,0,127);
	pVtx[3].color = D3DCOLOR_RGBA(255,255,0,127);

	g_pVtxBufferGameoverPolygon->Unlock();                                     //�o�b�t�@�̃A�����b�N�g
}

////////////////////////////////////////////////
//�|���S���̕`��
////////////////////////////////////////////////
void DrawGameover(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	//�w�i�F���_�t�H�[�}�b�g�ݒ�
	pD3DDevice -> SetStreamSource(0,g_pVtxBufferGameoverPolygon,0,sizeof(VERTEX_2D));                   //VRAM����GPU�Ƀf�[�^���X�g���[��
	pD3DDevice -> SetFVF(FVF_VERTEX2D);

	pD3DDevice -> SetTexture(0,g_pTextureGameoverPolygon[0]);												 //�e�N�X�`���̐ݒ�
	pD3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,0,NUM_POLYGON);                                 //�e�N�X�`���������̊m��

	//�f�[�^�擾
	GAMEMANAGER *pGameManager = GetManager();

	//PAUSE��ʕ\��
	if(pGameManager -> bGamePause == true)
	{
		//PAUSE�O���g
		pD3DDevice -> SetTexture(0,g_pTextureGameoverPolygon[1]);
		pD3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,4,NUM_POLYGON);

		//PAUSE�g
		pD3DDevice -> SetTexture(0,g_pTextureGameoverPolygon[2]);
		pD3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,8,NUM_POLYGON);

		//PAUSE�t�H���g
		pD3DDevice -> SetTexture(0,g_pTextureGameoverPolygon[3]);
		pD3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,12,NUM_POLYGON);

		pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);                                       //���Z�������s��!

		//�I���o�[
		pD3DDevice -> SetTexture(0,g_pTextureGameoverPolygon[4]);
		pD3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,16,NUM_POLYGON);

		pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);                               //�u�����h���[�h���A���t�@�u�����h�ɖ߂�
	}
}

void UninitGameover(void)
{
		// �e�N�X�`���C���^�[�t�F�C�X�̕Еt��
		for (int nCnt = 0; nCnt < NUM_GAMEOVERTEXTURE; nCnt++)
		{
			if(g_pTextureGameoverPolygon[nCnt] != NULL)
			{
				g_pTextureGameoverPolygon[nCnt]->Release();
				g_pTextureGameoverPolygon[nCnt] = NULL;
			}
		}
	
		//���_�o�b�t�@�C���^�[�t�F�C�X�̕Еt��
		if( g_pVtxBufferGameoverPolygon != NULL)
		{
			g_pVtxBufferGameoverPolygon -> Release();
			g_pVtxBufferGameoverPolygon = NULL;
		}
}

//�w�i�|���S���̕\���ʒuX,�|���S���̕\���ʒuY,�|���S���̕�,�|���S���̍���
void MakeVertexPolygonGameover(LPDIRECT3DDEVICE9 pDevice,VERTEX_2D *pVtx)
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