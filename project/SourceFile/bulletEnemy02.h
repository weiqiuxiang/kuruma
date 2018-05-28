//-----------------------------------------------------------------------------------------
//�v���[���[�w�b�h�t�@�C��[enemy_running02.h]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------------------
#ifndef _BULLETENEMY02_H_
#define _BULLETENEMY02_H_

#include <d3dx9.h>
//-----------------------------------------------------------------------------------------
//���_�t�H�[�}�b�g
//-----------------------------------------------------------------------------------------
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define MAX_BULLETENEMY02 (256)
#define NUM_BULLETENEMY02VERTEX (4)                          //���_��
#define NUM_BULLETENEMY02TEXTURE (1)                         //�e�N�X�`����

#define BULLETENEMY02_WIDTH (24.0f)
#define BULLETENEMY02_HEIGHT (24.0f)

//-----------------------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------------------
#define BULLETENEMY02POLYGON01_TEXTURENAME "data/TEXTURE/sozai/BulletEnemy02.png"

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
	float fRadius;           //�e�̔��a
	int nUnit;               //�ǂ̓G���e�𔭎ˌ���(�Ⴂ�G�̒e�̓��삪�Ⴂ)
}BULLETENEMY02;

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------------------
HRESULT InitBulletEnemy02(void);
void UninitBulletEnemy02(void);
void UpdateBulletEnemy02(void);
void DrawBulletEnemy02(void);
void UpdatePolygonBulletEnemy02(void);
void MakeVertexPolygonBulletEnemy02(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx);
void SetBulletEnemy02(int nUnit,float fPosX,float fPosY,float fSpeedX,float fSpeedY,int nLife,float fAngle,float fRadius);
BULLETENEMY02 *GetBulletEnemy02(void);

#endif
