//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
//-----------------------------------------------------------------------------------------
#include <Windows.h>
#include "main.h"
#include "BGpolygon.h"
#include "player.h"
#include "input.h"

//-----------------------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureBGPolygon[NUM_BGTEXTURE] = {};           //�e�N�X�`���𑀍삷�邽�߂̃|�C���^(�e�N�X�`���o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferBGPolygon[NUM_BGPOLYGON] = {};    //���_�o�b�t�@�𑀍삷�邽�߂̃|�C���^(���_�o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)

////////////////////////////////////////////////
//�|���S��(���_)�̏�����
////////////////////////////////////////////////
HRESULT InitBGPolygon(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_2D *pVtx = NULL;                           //���_�\���̂̃|�C���^
	pD3DDevice = Get_Device();

	//���_�o�b�t�@�̐���
	if(FAILED(pD3DDevice -> CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_BGVERTEX*NUM_BGPOLYGON,D3DUSAGE_WRITEONLY,FVF_VERTEX2D,D3DPOOL_MANAGED,&g_pVtxBufferBGPolygon[0],NULL)))
	{
		return E_FAIL;
	}

	//�w�i�e�N�X�`��01�̎擾
	if ( FAILED( D3DXCreateTextureFromFile(pD3DDevice, BGPOLYGON01_TEXTURENAME,&g_pTextureBGPolygon[0]) ) )
	{
		MessageBox(NULL,"�e�N�X�`���̓ǂݍ��݂����s���܂���","�G���[",MB_OK|MB_ICONHAND);
		return E_FAIL;
	}

	//�w�i�e�N�X�`��02�̎擾
	if ( FAILED( D3DXCreateTextureFromFile(pD3DDevice, BGPOLYGON02_TEXTURENAME,&g_pTextureBGPolygon[1]) ) )
	{
		MessageBox(NULL,"�e�N�X�`���̓ǂݍ��݂����s���܂���","�G���[",MB_OK|MB_ICONHAND);
		return E_FAIL;
	}

	//�w�i�̋�̃e�N�X�`���̎擾
	if ( FAILED( D3DXCreateTextureFromFile(pD3DDevice, BGPOLYGON03_TEXTURENAME,&g_pTextureBGPolygon[2]) ) )
	{
		MessageBox(NULL,"�e�N�X�`���̓ǂݍ��݂����s���܂���","�G���[",MB_OK|MB_ICONHAND);
		return E_FAIL;
	}

	//�w�i�̋�|���S��
	g_pVtxBufferBGPolygon[0] -> Lock(0,0,(void**)&pVtx,0);                    //���z�A�h���X���擾���o�b�t�@�����b�N����

	MakeVertexPolygonBG(pD3DDevice,pVtx);
	pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[1].pos = D3DXVECTOR3(BG_WIDTH,0.0f,0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, BG_HEIGHT,0.0f);
	pVtx[3].pos = D3DXVECTOR3(BG_WIDTH,BG_HEIGHT,0.0f);

	pVtx += 4;

	pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[1].pos = D3DXVECTOR3(BG_WIDTH,0.0f,0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, BG_HEIGHT,0.0f);
	pVtx[3].pos = D3DXVECTOR3(BG_WIDTH,BG_HEIGHT,0.0f);

	pVtx += 4;

	pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[1].pos = D3DXVECTOR3(BG_WIDTH,0.0f,0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, BG_HEIGHT,0.0f);
	pVtx[3].pos = D3DXVECTOR3(BG_WIDTH,BG_HEIGHT,0.0f);

	g_pVtxBufferBGPolygon[0] -> Unlock();                                     //�o�b�t�@�̃A�����b�N

	return S_OK;
}

//�|���S���̍X�V
void UpdateBGPolygon(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice = Get_Device();
	PLAYER *pPlayer = GetPlayer();
	VERTEX_2D *pVtx = NULL;                           //���_�\���̂̃|�C���^
	static float fMoveCloud = 0;                                  //�w�i�̋�̈ړ�����

	//�w�i��̌v�Z
	g_pVtxBufferBGPolygon[0]->Lock(0, 0, (void**)&pVtx, 0);       //���z�A�h���X���擾���o�b�t�@�����b�N����

	MakeVertexPolygonBG(pD3DDevice, pVtx);

	pVtx[0].pos = D3DXVECTOR3(-pPlayer->fPosX_SUM, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(-pPlayer->fPosX_SUM+BG_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-pPlayer->fPosX_SUM, BG_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(-pPlayer->fPosX_SUM+BG_WIDTH, BG_HEIGHT, 0.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(2.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(2.0f,1.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[1].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[2].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[3].color = D3DCOLOR_RGBA(255,255,255,255);

	pVtx += 4;

	pVtx[0].pos = D3DXVECTOR3(-pPlayer->fPosX_SUM, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(-pPlayer->fPosX_SUM+BG_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-pPlayer->fPosX_SUM, BG_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(-pPlayer->fPosX_SUM+BG_WIDTH, BG_HEIGHT, 0.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(2.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(2.0f,1.0f);

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
	pVtx[1].pos = D3DXVECTOR3(BG_WIDTH,0.0f,0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, BG_HEIGHT,0.0f);
	pVtx[3].pos = D3DXVECTOR3(BG_WIDTH,BG_HEIGHT,0.0f);

	pVtx[0].tex = D3DXVECTOR2(fMoveCloud,0.0f);
	pVtx[1].tex = D3DXVECTOR2(fMoveCloud+2.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(fMoveCloud,1.0f);
	pVtx[3].tex = D3DXVECTOR2(fMoveCloud+2.0f,1.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[1].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[2].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[3].color = D3DCOLOR_RGBA(255,255,255,255);

	g_pVtxBufferBGPolygon[0]->Unlock();                                     //�o�b�t�@�̃A�����b�N�g

	fMoveCloud += 0.0005f;
	if(fMoveCloud >= 10000) fMoveCloud = 0;
}

////////////////////////////////////////////////
//�|���S���̕`��
////////////////////////////////////////////////
void DrawBGPolygon(void)
{
	static int nCnt_BGChange = 0;
	static int nCnt_FrameChange = 1;

	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	//�w�i�F���_�t�H�[�}�b�g�ݒ�
	pD3DDevice -> SetStreamSource(0,g_pVtxBufferBGPolygon[0],0,sizeof(VERTEX_2D));                   //VRAM����GPU�Ƀf�[�^���X�g���[��
	pD3DDevice -> SetFVF(FVF_VERTEX2D);
	pD3DDevice -> SetTexture(0,g_pTextureBGPolygon[2]);												 //�e�N�X�`���̐ݒ�
	pD3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,2*4,NUM_POLYGON);                                 //�e�N�X�`���������̊m��

	pD3DDevice -> SetTexture(0,g_pTextureBGPolygon[nCnt_BGChange % 2]);												 //�e�N�X�`���̐ݒ�
	pD3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,nCnt_BGChange % 2 * 4,NUM_POLYGON);                              //�e�N�X�`���������̊m��

	nCnt_FrameChange++;
	if(nCnt_FrameChange % 60 == 0) nCnt_BGChange++;                                                     //8�t���[�����Ƃɔw�i�摜��؂�ւ�

}

void UninitBGPolygon(void)
{
		// �e�N�X�`���C���^�[�t�F�C�X�̕Еt��
		for (int nCnt = 0; nCnt < NUM_BGTEXTURE; nCnt++)
		{
			SAFE_RELEASE(g_pTextureBGPolygon[nCnt]);
		}
	
		//���_�o�b�t�@�C���^�[�t�F�C�X�̕Еt��
		for (int nCnt = 0; nCnt < NUM_BGPOLYGON; nCnt++)
		{
			SAFE_RELEASE(g_pVtxBufferBGPolygon[nCnt]);
		}
}

//�w�i�|���S���̕\���ʒuX,�|���S���̕\���ʒuY,�|���S���̕�,�|���S���̍���
void MakeVertexPolygonBG(LPDIRECT3DDEVICE9 pDevice,VERTEX_2D *pVtx)
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