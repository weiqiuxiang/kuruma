#ifndef _FADE_H_
#define _FADE_H_
//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
//-----------------------------------------------------------------------------------------
#include <d3dx9.h>

//-----------------------------------------------------------------------------------------
//���_�t�H�[�}�b�g
//-----------------------------------------------------------------------------------------
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define NUM_FADEVERTEX (4)                        //���_��
#define NUM_FADEPOLYGON (1)						//��`��
#define NUM_FADETEXTURE (1)                       //�e�N�X�`����

#define FADE_WIDTH (1000.0f)
#define FADE_HEIGHT (800.0f)
#define FADE_RATE (5)                 //faderate���`

//-----------------------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------------------
#define FADEPOLYGON01_TEXTURENAME "data/TEXTURE/sozai/fade.png"

//-----------------------------------------------------------------------------------------
//�t�F�[�h���[�h�̒�`
//-----------------------------------------------------------------------------------------
typedef enum
{
	FADE_NONE = 0,
	FADE_IN,
	FADE_OUT,
	FADE_MAX,
}FADE;

typedef struct
{
	D3DXVECTOR3 pos;                         //
	float rhw;								 //���W�ϊ��ςݒ��_�t���O(�K��1.0f)
	D3DCOLOR color;						 //���_�J���[
	D3DXVECTOR2 tex;                         //�e�N�X�`�����W(UV or ST)
}VERTEX_2DF;

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------------------
HRESULT InitFade(void);                                                     //�|���S��(���_)�̏�����
void DrawFade(void);                                                        //�|���S���̕`��
void UpdateFade(void);                                                      //�|���S���̍X�V
void UninitFade(void);                                                      //�|���S���̏I��
void MakeVertexPolygonFade(LPDIRECT3DDEVICE9 pDevice,VERTEX_2DF *pVtx);           //�w�i�|���S���ݒ�
void SetFade(FADE fade,MODE modeNext);                                      //�t�F�[�h�ݒ�
FADE *GetFade(void);

#endif