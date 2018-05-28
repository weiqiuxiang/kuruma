//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
//-----------------------------------------------------------------------------------------
#include <Windows.h>
#include "main.h"
#include "tutorial.h"
#include "player.h"
#include "fade.h"
#include "input.h"

//-----------------------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureTutorialPolygon[NUM_TUTORIALTEXTURE] = {};           //�e�N�X�`���𑀍삷�邽�߂̃|�C���^(�e�N�X�`���o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferTutorialPolygon = NULL;                       //���_�o�b�t�@�𑀍삷�邽�߂̃|�C���^(���_�o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)
int g_nCntChange;
FADE g_TutorialFade;
int g_TutorialAlpha;

bool UpdateTutorialFade(void);                                                    //�`���[�g���A��fade�̍X�V

////////////////////////////////////////////////
//�|���S��(���_)�̏�����
////////////////////////////////////////////////
HRESULT InitTutorial(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_2D *pVtx = NULL;                           //���_�\���̂̃|�C���^
	pD3DDevice = Get_Device();

	//���_�o�b�t�@�̐���
	if(FAILED(pD3DDevice -> CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_TUTORIALVERTEX*NUM_TUTORIALPOLYGON,D3DUSAGE_WRITEONLY,FVF_VERTEX2D,D3DPOOL_MANAGED,&g_pVtxBufferTutorialPolygon,NULL)))
	{
		return E_FAIL;
	}

	//�`���[�g���A��
	if ( FAILED( D3DXCreateTextureFromFile(pD3DDevice, TUTORIALPOLYGON01_TEXTURENAME,&g_pTextureTutorialPolygon[0]) ) )
	{
		MessageBox(NULL,"�e�N�X�`���̓ǂݍ��݂����s���܂���","�G���[",MB_OK|MB_ICONHAND);
		return E_FAIL;
	}

	if ( FAILED( D3DXCreateTextureFromFile(pD3DDevice, TUTORIALPOLYGON02_TEXTURENAME,&g_pTextureTutorialPolygon[1]) ) )
	{
		MessageBox(NULL,"�e�N�X�`���̓ǂݍ��݂����s���܂���","�G���[",MB_OK|MB_ICONHAND);
		return E_FAIL;
	}

	if ( FAILED( D3DXCreateTextureFromFile(pD3DDevice, TUTORIALPOLYGON03_TEXTURENAME,&g_pTextureTutorialPolygon[2]) ) )
	{
		MessageBox(NULL,"�e�N�X�`���̓ǂݍ��݂����s���܂���","�G���[",MB_OK|MB_ICONHAND);
		return E_FAIL;
	}

	//���b�N
	g_pVtxBufferTutorialPolygon -> Lock(0,0,(void**)&pVtx,0);                    //���z�A�h���X���擾���o�b�t�@�����b�N����

	MakeVertexPolygonTutorial(pD3DDevice,pVtx);
	pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH,0.0f,0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT,0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH,SCREEN_HEIGHT,0.0f);

	pVtx += 4;

	pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH,0.0f,0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT,0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH,SCREEN_HEIGHT,0.0f);

	g_pVtxBufferTutorialPolygon -> Unlock();                                     //�o�b�t�@�̃A�����b�N

	g_nCntChange = 0;
	g_TutorialFade = FADE_NONE;

	return S_OK;
}

void UpdateTutorial(void)
{
	FADE *pFade = GetFade();                              //�t�F�[�h�̏�Ԃ��Q�b�g
	static int nCntChange = 0;
	
	if(nCntChange >= 120)
	{
		//X�{�^���ŉ�ʑJ��
		if(GetControllerTrigger(0) || GetKeyboardTrigger(DIK_J))
		{
			g_TutorialFade = FADE_OUT;
			nCntChange = 0;
		}
	}

	if(*pFade == FADE_NONE) nCntChange++;               //��ʑJ�ڊ����ォ��J�E���g�A�b�v

	if(UpdateTutorialFade() == true) g_nCntChange++;    //�`���[�g���A�������̉�ʑJ��

	//�Q�[���ɑJ��
	if(g_nCntChange >= 2)
	{
		SetFade(FADE_OUT,MODE_GAME);
	}

	UpdateTutorialPolygon();                             //�|���S���e�N�X�`���̍X�V
}

bool UpdateTutorialFade(void)
{
	static int Alpha = 255;
	bool flag = false;

	if(g_TutorialFade == FADE_NONE)
	{
		return flag;
	}

	if(g_TutorialFade == FADE_IN)
	{
		Alpha -= FADE_RATE;   //���l�����Z���Č��̂���ʂ𕂂��яオ�点��
		//�t�F�[�h�C���̏I������
		if(Alpha < 0 )
		{
			Alpha = 0.0f;
			g_TutorialFade = FADE_NONE;
		}
	}

	else if(g_TutorialFade == FADE_OUT)
	{
		Alpha += FADE_RATE;   //���l�����Z���Č��̂���ʂ������Ă���
		//�t�F�[�h�C���̏I������
		if(Alpha > 255)
		{
			Alpha = 255;
			g_TutorialFade = FADE_IN;     //�t�F�[�h�C���ɐ؂�ւ�
			flag = true;
		}
	}

	g_TutorialAlpha = Alpha;         //���̃��l��n��

	return flag;
}

//�|���S���̍X�V
void UpdateTutorialPolygon(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice = Get_Device();
	PLAYER *pPlayer = GetPlayer();
	VERTEX_2D *pVtx = NULL;                           //���_�\���̂̃|�C���^
	static float fMoveCloud = 0;                                  //�w�i�̋�̈ړ�����

	//�w�i��̌v�Z
	g_pVtxBufferTutorialPolygon->Lock(0, 0, (void**)&pVtx, 0);       //���z�A�h���X���擾���o�b�t�@�����b�N����

	MakeVertexPolygonTutorial(pD3DDevice, pVtx);

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

	pVtx[0].color = D3DCOLOR_RGBA(255,255,255,g_TutorialAlpha);
	pVtx[1].color = D3DCOLOR_RGBA(255,255,255,g_TutorialAlpha);
	pVtx[2].color = D3DCOLOR_RGBA(255,255,255,g_TutorialAlpha);
	pVtx[3].color = D3DCOLOR_RGBA(255,255,255,g_TutorialAlpha);

	g_pVtxBufferTutorialPolygon->Unlock();                                     //�o�b�t�@�̃A�����b�N�g
}

////////////////////////////////////////////////
//�|���S���̕`��
////////////////////////////////////////////////
void DrawTutorial(void)
{
	static int nCnt_TutorialChange = 0;
	static int nCnt_FrameChange = 1;

	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	//�w�i�F���_�t�H�[�}�b�g�ݒ�
	pD3DDevice -> SetStreamSource(0,g_pVtxBufferTutorialPolygon,0,sizeof(VERTEX_2D));                   //VRAM����GPU�Ƀf�[�^���X�g���[��
	pD3DDevice -> SetFVF(FVF_VERTEX2D);

	if(g_nCntChange == 0)
	{
		pD3DDevice -> SetTexture(0,g_pTextureTutorialPolygon[0]);												 //�e�N�X�`���̐ݒ�
		pD3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,0,NUM_POLYGON);                              //�e�N�X�`���������̊m��
	}

	if(g_nCntChange == 1)
	{
		pD3DDevice -> SetTexture(0,g_pTextureTutorialPolygon[1]);												 //�e�N�X�`���̐ݒ�
		pD3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,0,NUM_POLYGON);                              //�e�N�X�`���������̊m��
	}

	//�t�F�[�h
	pD3DDevice -> SetTexture(0,g_pTextureTutorialPolygon[2]);												 //�e�N�X�`���̐ݒ�
	pD3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,4,NUM_POLYGON);                                 //�e�N�X�`���������̊m��
}

void UninitTutorial(void)
{
		// �e�N�X�`���C���^�[�t�F�C�X�̕Еt��
		for (int nCnt = 0; nCnt < NUM_TUTORIALTEXTURE; nCnt++)
		{
			SAFE_RELEASE(g_pTextureTutorialPolygon[nCnt]);
		}
	
		//���_�o�b�t�@�C���^�[�t�F�C�X�̕Еt��
		SAFE_RELEASE(g_pVtxBufferTutorialPolygon);
}

//�w�i�|���S���̕\���ʒuX,�|���S���̕\���ʒuY,�|���S���̕�,�|���S���̍���
void MakeVertexPolygonTutorial(LPDIRECT3DDEVICE9 pDevice,VERTEX_2D *pVtx)
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