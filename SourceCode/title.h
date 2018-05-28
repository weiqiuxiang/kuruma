#ifndef _TITLE_H_
#define _TITLE_H_
//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
//-----------------------------------------------------------------------------------------
#include <d3dx9.h>

//-----------------------------------------------------------------------------------------
//���_�t�H�[�}�b�g
//-----------------------------------------------------------------------------------------
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define NUM_TITLEVERTEX (4)                     //���_��
#define NUM_TITLETEXTURE (3)                    //�e�N�X�`����
#define NUM_TITLEPOLYGON (3)					//��`��

//�^�C�g���̕��A����
#define WIDTH_TITLE (640) 
#define HEIGHT_TITLE (640)  

//�I���J�[�\���̕��A����
#define WIDTH_CURSOR (128)
#define HEIGHT_CURSOR (64)
#define ICON_POSX (239)
#define ICON_POSY (398)
#define DISTANCE_FROM_START (130)                   //����START�Ƃ̋���
#define DISTANCE_FROM_EXIT (130)                    //����EXIT�Ƃ̋���

//start��exit�̈ʒu
#define START_POSX (239)
#define START_POSY (398)
#define EXIT_POSX (267)
#define EXIT_POSY (502)

//-----------------------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------------------
#define TITLEPOLYGON00_TEXTURENAME "data/TEXTURE/sozai/title.png"
#define TITLEPOLYGON01_TEXTURENAME "data/TEXTURE/sozai/SelectIcon.png"
#define TITLEPOLYGON02_TEXTURENAME "data/TEXTURE/sozai/normal_bg.png"

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------------------
HRESULT InitTitle(void);                                                      //�|���S��(���_)�̏�����
void DrawTitle(void);                                                         //�|���S���̕`��
void UpdateTitle(void);                                                       //�|���S���̍X�V
void UninitTitle(void);                                                       //�|���S���̏I��
void MakeVertexPolygonTitle(LPDIRECT3DDEVICE9 pDevice,VERTEX_2D *pVtx);              //�w�i�|���S���ݒ�

#endif