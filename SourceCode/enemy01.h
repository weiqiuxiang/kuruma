//-----------------------------------------------------------------------------------------
//�v���[���[�w�b�h�t�@�C��[enemy_running02.h]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------------------
#ifndef _ENEMY01_H_
#define _ENEMY01_H_

//-----------------------------------------------------------------------------------------
//���_�t�H�[�}�b�g
//-----------------------------------------------------------------------------------------
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define NUM_ENEMY01VERTEX (4)                            //���_��
#define NUM_ENEMY01POLYGON (10)						  //��`��
#define NUM_ENEMY01TEXTURE (1)                           //�e�N�X�`����

#define ENEMY01_WIDTH (96.0f*1.5)
#define ENEMY01_HEIGHT (64.0f*1.5)

//����͈͂̏C���p
#define ENEMY01_TOP (17.0f*1.5)
#define ENEMY01_HORRIZON (10.0f*1.5)
#define ENEMY01_HEIGHT_CHECK (3.0f*1.5)

//�e���ˈʒu�̒���
#define BULLETENEMY01_DISTANCEX (50.0f*1.5)
#define BULLETENEMY01_DISTANCEY (46.0f*1.5)

//�G�̍ő吔
#define MAX_ENEMY01 (10)

//-----------------------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------------------
#define Enemy01POLYGON01_TEXTURENAME "data/TEXTURE/sozai/Enemy01.png"

//-----------------------------------------------------------------------------------------
//�v���[���[�̍\����
//-----------------------------------------------------------------------------------------
typedef struct
{
	float nNumEnemy;                               //�G�̔ԍ�
	float fPosX;                                   //Enemy01�̍��W(X���W)
	float fPosY;                                   //Enemy01�̍��W(Y���W)
	float fPosXOld;                                //Enemy01�̌Â����W(X���W)
	float fPosYOld;                                //Enemy01�̌Â����W(X���W)
	float fPosX_Relative;                          //�G��X���̑��΍��W
	float fMoveX;                                  //�GX���̈ړ��X�s�[�h
	float fMoveY;                                  //�GY���̈ړ��X�s�[�h
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

	//Enemy01���U����Ԃɓ��鎞�̈ړ�
	//����:�v���[���[�Ƃ̋����𑪂� -> �ړ� -> �U��(�񐔂���,�ő�2��) -> �e���U
	//���[�v: ��-----------------------------------------------------------��

	//bAttackMeasure�ʏ��flase,�U�����[�h�ɓ�������,�v���[���[�Ƃ̋����𑪂���true�ɂ���
	bool bAttackMeasure;
	//bAttackMove,�ʏ��flase,�U�����[�h�ɓ�������true�ɂ���,true�̎�,�U���ړ����s��,�ړ��I����false
	bool bAttackMove;
	//�U����(�ő�2��),�U���񐔂�0�ɂȂ��,bAttackMeasure��false�ɂ�,���[�v
	int nAttackTime; 
}ENEMY01;

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------------------
HRESULT InitEnemy01(void);
void UninitEnemy01(void);
void UpdateEnemy01(void);
void DrawEnemy01(void);
void UpdatePolygonEnemy01(void);
void MakeVertexPolygonEnemy01(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx);
void SetEnemy01(float fPosX,float fPosY,float fSpeedX,float fSpeedY,float fDirect,float nEnemyNum);                          //Enemy01�̃Z�b�g�֐�
ENEMY01 *GetEnemy01(void);

#endif
