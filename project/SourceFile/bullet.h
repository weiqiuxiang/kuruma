//-----------------------------------------------------------------------------------------
//�v���[���[�w�b�h�t�@�C��[enemy_running02.h]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------------------
#ifndef _BULLET_H_
#define _BULLET_H_

#include <d3dx9.h>
//-----------------------------------------------------------------------------------------
//���_�t�H�[�}�b�g
//-----------------------------------------------------------------------------------------
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define MAX_BULLET (256)
#define NUM_BULLETVERTEX (4)                          //���_��
#define NUM_BULLETTEXTURE (1)                         //�e�N�X�`����

#define BULLET_WIDTH (24.0f)
#define BULLET_HEIGHT (24.0f)
#define CHECK_RADIUS (6.0f)                           //���蔻��̔��a
#define BULLET_DISTANCEX (50.0f)
#define BULLET_DISTANCEY (25.0f)

//-----------------------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------------------
#define BULLETPOLYGON01_TEXTURENAME "data/TEXTURE/sozai/bullet.png"

//-----------------------------------------------------------------------------------------
//�v���[���[�̍\����
//-----------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR2 pos;         //���W
	D3DXVECTOR2 speed;       //�ړ���(���x)
	D3DXCOLOR color;         //�J���[
	int nLife;               //����
	bool bUse;               //�g�p�t���O
	float fAngle;            //�e�̊p�x
}BULLET;

typedef struct
{
	//�e�̓��蔻��̏㉺���E���W
	float Up;
	float Bottom;
	float Left;
	float Right;
}BULLET_HITCHECK;

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------------------
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void UpdatePolygonBullet(void);
void MakeVertexPolygonBullet(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx);
void SetBullet(float fPosX,float fPosY,float fSpeedX,float fSpeedY,int nLife,float fAngle);
void SpriteRorateBullet (BULLET Bbullet,VERTEX_2D *pVtx);                                              //��]�p�֐�
//�e�̃f�[�^���Q�b�g
BULLET *GetBullet(void);
BULLET_HITCHECK *GetBulletHitcheck(void);

#endif
