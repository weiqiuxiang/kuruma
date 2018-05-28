//-----------------------------------------------------------------------------------------
//�v���[���[�w�b�h�t�@�C��[enemy_running02.h]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------------------
#ifndef _PARTICAL_H_
#define _PARTICAL_H_

#include <d3dx9.h>
//-----------------------------------------------------------------------------------------
//���_�t�H�[�}�b�g
//-----------------------------------------------------------------------------------------
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define MAX_PARTICAL (4096)
#define NUM_PARTICALVERTEX (4)                          //���_��
#define NUM_PARTICALTEXTURE (1)                         //�e�N�X�`����

//-----------------------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------------------
#define PARTICALPOLYGON01_TEXTURENAME "data/TEXTURE/sozai/partical.png"

//-----------------------------------------------------------------------------------------
//�v���[���[�̍\����
//-----------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR2 pos;         //���W
	D3DXVECTOR2 posOld;
	D3DXVECTOR2 speed;       //�ړ���(���x)
	D3DXCOLOR color;         //�J���[
	float fRadius;           //���a
	float fColorAlphaValue;  //�F(��)�ω���
	int nLife;               //����
	int nAlpha;              //���l
	bool bCamera;            //�J�����Ɗ֌W���邩
	bool bUse;               //�g�p�t���O
}PARTICAL;

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------------------
HRESULT InitPartical(void);
void UninitPartical(void);
void UpdatePartical(void);
void DrawPartical(void);
void UpdatePolygonPartical(void);
void MakeVertexPolygonPartical(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx);
void SetPartical(float fPosX,float fPosY,float fSpeedX,float fSpeedY,float fRadius,float nLife,bool bCamera);
PARTICAL *GetPartical(void);

#endif
