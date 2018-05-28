//-----------------------------------------------------------------------------------------
//�v���[���[�w�b�h�t�@�C��[enemy_running02.h]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------------------
#ifndef _PARTICALEFFECT_H_
#define _PARTICALEFFECT_H_

#include <d3dx9.h>
//-----------------------------------------------------------------------------------------
//���_�t�H�[�}�b�g
//-----------------------------------------------------------------------------------------
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define MAX_PARTICALEFFECT (4096)
#define NUM_PARTICALEFFECTVERTEX (4)                          //���_��
#define NUM_PARTICALEFFECTTEXTURE (1)                         //�e�N�X�`����

//-----------------------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------------------
#define ParticalEffectPOLYGON01_TEXTURENAME "data/TEXTURE/sozai/particalEffect.jpg"

//-----------------------------------------------------------------------------------------
//�v���[���[�̍\����
//-----------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR2 pos;         //���S���W
	D3DXVECTOR2 posOld;      //
	D3DXVECTOR2 speed;       //���x
	D3DCOLOR color;          //�J���[
	float fRadius;           //���a
	float fColorAlphaValue;  //�F(��)�ω���
	int nLife;               //����
	int nAlpha;              //���l
	bool bCamera;             //
	bool bUse;               //�g�p�t���O
}PARTICALEFFECT;

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------------------
HRESULT InitParticalEffect(void);
void UninitParticalEffect(void);
void UpdateParticalEffect(void);
void DrawParticalEffect(void);
void UpdatePolygonParticalEffect(void);
void MakeVertexPolygonParticalEffect(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx);
void SetParticalEffect(float fPosX,float fPosY,float fSpeedX,float fSpeedY,float fRadius,int nLife,bool bCamera);
void SpriteRorateParticalEffect (PARTICALEFFECT PParticalEffect,VERTEX_2D *pVtx);                                              //��]�p�֐�
PARTICALEFFECT *GetParticalEffect(void);

#endif
