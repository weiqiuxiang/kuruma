#ifndef _POLYGON_H_
#define _POLYGON_H_
//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
//-----------------------------------------------------------------------------------------
#include <d3dx9.h>

//-----------------------------------------------------------------------------------------
//���_�t�H�[�}�b�g
//-----------------------------------------------------------------------------------------
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define NUM_BGVERTEX (4)                        //���_��
#define NUM_BGPOLYGON (3)						//��`��
#define NUM_BGTEXTURE (3)                       //�e�N�X�`����

#define BG_WIDTH (3840.0f)
#define BG_HEIGHT (640.0f)

//-----------------------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------------------
#define BGPOLYGON01_TEXTURENAME "data/TEXTURE/sozai/Background.png"
#define BGPOLYGON02_TEXTURENAME "data/TEXTURE/sozai/Background_sabun01.png"
#define BGPOLYGON03_TEXTURENAME "data/TEXTURE/sozai/Background_Sky.png"


//-----------------------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------------------
HRESULT InitBGPolygon(void);                                                     //�|���S��(���_)�̏�����
void DrawBGPolygon(void);                                                        //�|���S���̕`��
void UpdateBGPolygon(void);                                                      //�|���S���̍X�V
void UninitBGPolygon(void);                                                      //�|���S���̏I��
void MakeVertexPolygonBG(LPDIRECT3DDEVICE9 pDevice,VERTEX_2D *pVtx);           //�w�i�|���S���ݒ�

#endif