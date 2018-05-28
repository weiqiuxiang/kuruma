//-----------------------------------------------------------------------------------------
//�v���[���[�w�b�h�t�@�C��[enemy_running02.h]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------------------
#ifndef _ENEMY02_H_
#define _ENEMY02_H_

//-----------------------------------------------------------------------------------------
//���_�t�H�[�}�b�g
//-----------------------------------------------------------------------------------------
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define NUM_ENEMY02VERTEX (4)                            //���_��
#define NUM_ENEMY02POLYGON (10)						  //��`��
#define NUM_ENEMY02TEXTURE (1)                           //�e�N�X�`����

#define ENEMY02_WIDTH (192.0f)
#define ENEMY02_HEIGHT (128.0f)

//����͈͂̏C���p
#define ENEMY02_TOP (20.0f)
#define ENEMY02_HORRIZON (30.0f)
#define ENEMY02_HEIGHT_CHECK (10.0f)

#define ENEMY02_HEIGHT_ADJUST (22.0f)                  //�|���S���̕\������

//�e���ˈʒu�̒���
#define BULLETENEMY02_DISTANCEX (128.0f)
#define BULLETENEMY02_DISTANCEY (20.0f)

//�G�̍ő吔
#define MAX_ENEMY02 (10)

//-----------------------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------------------
#define ENEMY02POLYGON01_TEXTURENAME "data/TEXTURE/sozai/Enemy02.png"

//-----------------------------------------------------------------------------------------
//�v���[���[�̍\����
//-----------------------------------------------------------------------------------------
typedef struct
{
	float nNumEnemy;                               //�G�̔ԍ�
	float fPosX;                                   //Enemy02�̍��W(X���W)
	float fPosY;                                   //Enemy02�̍��W(Y���W)
	float fPosXOld;                                //Enemy02�̌Â����W(X���W)
	float fPosYOld;                                //Enemy02�̌Â����W(X���W)
	float fPosX_Relative;                          //�G��X���̑��΍��W
	float fMoveX;                                  //�G�̈ړ��X�s�[�h
	float fMoveY;                                  //�G�̈ړ��X�s�[�h
	float fDirect;                                 //�����̃t���O(-0.1f�Ȃ獶,0.1f�Ȃ�E)
	bool bUse;                                     //�v���[���[���g�����ǂ���
	bool bMove;                                    //�G�������Ă��邩
	bool bHit;                                     //�U�����󂯂�t���O
	bool bPre;                                     //������Ԃɖ߂�t���O(bMove��true�̎�,bPre��false)
	bool bInter;                                   //�A�ˊԊu��Ԃ�
	bool bRecovery;                                //�d����Ԃ�
	int nLife;                                     //�G�̗̑�
	int nCntPreAttack;                             //�e���˂܂ł̎���
	int nCntBullet;                                //���A�˂ł���e�̐�
	int nCntInterAttack;                           //�A�ˊԊu
	int nCntRecoveryAttack;                        //�U����d������
}ENEMY02;

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------------------
HRESULT InitEnemy02(void);
void UninitEnemy02(void);
void UpdateEnemy02(void);
void DrawEnemy02(void);
void UpdatePolygonEnemy02(void);
void MakeVertexPolygonEnemy02(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx);
void SetEnemy02(float fPosX,float fPosY,float fSpeedX,float fDirect,float nEnemyNum);                          //Enemy02�̃Z�b�g�֐�
ENEMY02 *GetEnemy02(void);
D3DXVECTOR2 BulletLanding(ENEMY02 enemy);                 //�e�̒��e�_�v�Z��

#endif
