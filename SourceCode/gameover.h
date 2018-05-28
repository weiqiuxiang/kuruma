#ifndef _GAMEOVER_H_
#define _GAMEOVER_H_
//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
//-----------------------------------------------------------------------------------------
#include <d3dx9.h>

//-----------------------------------------------------------------------------------------
//���_�t�H�[�}�b�g
//-----------------------------------------------------------------------------------------
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define NUM_GAMEOVERVERTEX (4)                      //���_��
#define NUM_GAMEOVERPOLYGON (5)						//��`��
#define NUM_GAMEOVERTEXTURE (5)                     //�e�N�X�`����

#define GAMEOVER_WIDTH (256.0f)
#define GAMEOVER_HEIGHT (64.0f)

//-----------------------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------------------
#define GAMEOVERPOLYGON01_TEXTURENAME "data/TEXTURE/sozai/GameOverFont.png"
#define PAUSEPOLYGON01_TEXTURENAME "data/TEXTURE/sozai/pause.png"
#define PAUSEPOLYGON02_TEXTURENAME "data/TEXTURE/sozai/pause_frame.png"
#define PAUSEPOLYGON03_TEXTURENAME "data/TEXTURE/sozai/pause_font.png"
#define PAUSEPOLYGON04_TEXTURENAME "data/TEXTURE/sozai/pause_font_color.png"

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------------------
HRESULT InitGameover(void);                                                    //�������֐�
void DrawGameover(void);                                                       //�`��֐�
void UpdateGameover(void);                                                     //�X�V�֐�
void UninitGameover(void);                                                     //���������
void MakeVertexPolygonGameover(LPDIRECT3DDEVICE9 pDevice,VERTEX_2D *pVtx);     //�|���S���ݒ�

#endif