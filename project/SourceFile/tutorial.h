#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_
//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
//-----------------------------------------------------------------------------------------
#include <d3dx9.h>

//-----------------------------------------------------------------------------------------
//���_�t�H�[�}�b�g
//-----------------------------------------------------------------------------------------
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define NUM_TUTORIALVERTEX (4)                      //���_��
#define NUM_TUTORIALPOLYGON (2)						//��`��
#define NUM_TUTORIALTEXTURE (3)                     //�e�N�X�`����

//-----------------------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------------------
#define TUTORIALPOLYGON01_TEXTURENAME "data/TEXTURE/sozai/tutorial01.png"
#define TUTORIALPOLYGON02_TEXTURENAME "data/TEXTURE/sozai/tutorial02.png"
#define TUTORIALPOLYGON03_TEXTURENAME "data/TEXTURE/sozai/tutorial_fade.png"

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------------------
HRESULT InitTutorial(void);                                                     //�|���S��(���_)�̏�����
void DrawTutorial(void);                                                        //�|���S���̕`��
void UpdateTutorial(void);                                                      //�|���S���̍X�V
void UpdateTutorialPolygon(void);                                               //�|���S���̍X�V
void UninitTutorial(void);                                               //�|���S���̏I��
void MakeVertexPolygonTutorial(LPDIRECT3DDEVICE9 pDevice,VERTEX_2D *pVtx);      //�w�i�|���S���ݒ�

#endif