#ifndef _SCORE_H_
#define _SCORE_H_
//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
//-----------------------------------------------------------------------------------------
#include <d3dx9.h>

//-----------------------------------------------------------------------------------------
//���_�t�H�[�}�b�g
//-----------------------------------------------------------------------------------------
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define NUM_SCOREVERTEX (4)                     //���_��
#define POS_SCORE_X (100)                       //�X�R�A�̏���X���W
#define POS_SCORE_Y (20)                       //�X�R�A�̏���Y���W
#define WIDTH_SCORE (12)                       //�X�R�A��1���̕�
#define HEIGHT_SCORE (24)                      //�X�R�A��1���̍���
#define SCORE_INTERVAL (12)                    //���ƌ��̊Ԋu
#define WIDTH_FONT (80)
#define HEIGHT_FONT (24)
#define DISTANCE_FROM_SCORE (95)

//-----------------------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------------------
#define SCOREPOLYGON_TEXTURENAME "data/TEXTURE/sozai/score.png"
#define SCOREPOLYGON02_TEXTURENAME "data/TEXTURE/sozai/score_font.png"

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------------------
HRESULT InitScorePolygon(void);                                                      //�|���S��(���_)�̏�����
void DrawScorePolygon(void);                                                         //�|���S���̕`��
void UpdateScorePolygon(void);                                                       //�|���S���̍X�V
void UninitScorePolygon(void);                                                       //�|���S���̏I��
void MakeVertexPolygonScore(LPDIRECT3DDEVICE9 pDevice,VERTEX_2D *pVtx);              //�w�i�|���S���ݒ�

#endif