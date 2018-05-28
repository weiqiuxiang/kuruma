#ifndef _RESULT_H_
#define _RESULT_H_
//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
//-----------------------------------------------------------------------------------------
#include <d3dx9.h>

//-----------------------------------------------------------------------------------------
//���_�t�H�[�}�b�g
//-----------------------------------------------------------------------------------------
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define NUM_RESULTVERTEX (4)                     //���_��
#define RESULT_POSX (280)
#define RESULT_POSY (120)
#define WIDTH_RESULT (225)                       //�^�C�g���̕�
#define HEIGHT_RESULT (64)                      //�^�C�g���̍���
#define NUM_RESULTTEXTURE (4)                    //�e�N�X�`����
#define NUM_RESULTPOLYGON (10)					 //��`��

//-----------------------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------------------
#define RESULTPOLYGON_TEXTURENAME "data/TEXTURE/sozai/result.png"
#define RESULTPOLYGON02_TEXTURENAME "data/TEXTURE/sozai/score.png"
#define RESULTPOLYGON03_TEXTURENAME "data/TEXTURE/sozai/score_font.png"
#define RESULTPOLYGON04_TEXTURENAME "data/TEXTURE/sozai/normal_bg.png"

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------------------
HRESULT InitResult(void);                                                      //�|���S��(���_)�̏�����
void DrawResult(void);                                                         //�|���S���̕`��
void UpdateResult(void);                                                       //�|���S���̍X�V
void UninitResult(void);                                                        //�|���S���̏I��
void MakeVertexPolygonResult(LPDIRECT3DDEVICE9 pDevice,VERTEX_2D *pVtx);              //�w�i�|���S���ݒ�

#endif