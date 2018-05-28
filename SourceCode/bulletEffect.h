//-----------------------------------------------------------------------------------------
//�v���[���[�w�b�h�t�@�C��[enemy_running02.h]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------------------
#ifndef _BULLETEFFECT_H_
#define _BULLETEFFECT_H_

#include <d3dx9.h>
//-----------------------------------------------------------------------------------------
//���_�t�H�[�}�b�g
//-----------------------------------------------------------------------------------------
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define MAX_BULLETEFFECT (4096)
#define NUM_BULLETEFFECTVERTEX (4)                          //���_��
#define NUM_BULLETEFFECTTEXTURE (1)                         //�e�N�X�`����

#define BULLETEFFECT_WIDTH (24.0f)
#define BULLETEFFECT_HEIGHT (24.0f)
#define BULLETEFFECT_DISTANCEX (50.0f)
#define BULLETEFFECT_DISTANCEY (25.0f)

#define RADIUS (12.0f)
#define BULLETLIFE (10.0f)

//-----------------------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------------------
#define BULLETEFFECTPOLYGON01_TEXTURENAME "data/TEXTURE/sozai/bulletEffect.png"

//-----------------------------------------------------------------------------------------
//�v���[���[�̍\����
//-----------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR2 pos;         //�|���S���̒��S���W
	D3DCOLOR color;         //�J���[
	float fRadius;           //���a
	float fRaidusValue;      //���a�̕ω���
	float fColorAlphaValue;  //�F(��)�ω���
	int nLife;               //����
	int nAlpha;              //���l
	bool bUse;               //�g�p�t���O
	float fAngle;            //�e�̊p�x
}BULLETEFFECT;

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------------------
HRESULT InitBulletEffect(void);
void UninitBulletEffect(void);
void UpdateBulletEffect(void);
void DrawBulletEffect(void);
void UpdatePolygonBulletEffect(void);
void MakeVertexPolygonBulletEffect(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx);
void SetBulletEffect(float fPosX,float fPosY,int nLife,float fAngle);
void SpriteRorateBulletEffect (BULLETEFFECT BBulletEffect,VERTEX_2D *pVtx);                                              //��]�p�֐�
BULLETEFFECT *GetBulletEffect(void);

#endif
