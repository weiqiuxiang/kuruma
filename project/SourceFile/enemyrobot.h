//-----------------------------------------------------------------------------------------
//�v���[���[�w�b�h�t�@�C��[enemy_running02.h]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------------------
#ifndef _ENEMYROBOT_H_
#define _ENEMYROBOT_H_

//-----------------------------------------------------------------------------------------
//���_�t�H�[�}�b�g
//-----------------------------------------------------------------------------------------
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define NUM_ENEMYROBOTVERTEX (4)                            //���_��
#define NUM_ENEMYROBOTPOLYGON (50)						    //��`��
#define NUM_ENEMYROBOTTEXTURE (1)                           //�e�N�X�`����

#define ENEMYROBOT_WIDTH (96.0f)
#define ENEMYROBOT_HEIGHT (96.0f)

//����͈͂̏C���p
#define ENEMYROBOT_TOP (7.5f)
#define ENEMYROBOT_HORRIZON (15.0f)
#define ENEMYROBOT_HEIGHT_CHECK (2.0f)

//�e���ˈʒu�̒���
#define BULLETENEMYROBOT_DISTANCEX (12.0f)
#define BULLETENEMYROBOT_DISTANCEY (48.0f)

//�G�̃i�C�t�̔���͈̓t���O
#define KNIFEENEMYROBOT_DISTANCEX_L (12.0f)
#define KNIFEENEMYROBOT_DISTANCEX_R (31.5f)
#define KNIFEENEMYROBOT_DISTANCEY_T (70.5f)
#define KNIFEENEMYROBOT_DISTANCEY_B (90.0f)

//�n�ʂƂ̓��蔻��
#define ENEMYROBOT_HEIGHT_ADJUST (12.0f)

//�G�̍ő吔
#define MAX_ENEMYROBOT (50)

//-----------------------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------------------
#define ENEMYROBOTPOLYGON01_TEXTURENAME "data/TEXTURE/sozai/EnemyRobot.png"

//-----------------------------------------------------------------------------------------
//�v���[���[�̍\����
//-----------------------------------------------------------------------------------------
typedef struct
{
	int nType;                                     //EnemyRobot�̃^�C�v : 1.�e�^�C�v 2.���^�C�v
	float nNumEnemy;                               //�G�̔ԍ�
	float fPosX;                                   //EnemyRobot�̍��W(X���W)
	float fPosY;                                   //EnemyRobot�̍��W(Y���W)
	float fPosXOld;                                //EnemyRobot�̌Â����W(X���W)
	float fPosYOld;                                //EnemyRobot�̌Â����W(X���W)
	float fMoveX;                                  //�GX���̈ړ��X�s�[�h
	float fMoveY;                                  //�GY���̈ړ��X�s�[�h
	float fDirect;                                 //�����̃t���O(-0.1f�Ȃ獶,0.1f�Ȃ�E)
	bool bUse;                                     //�v���[���[���g�����ǂ���
	bool bMove;                                    //�G�������Ă��邩
	bool bHit;                                     //�U�����󂯂�t���O
	int nLife;                                     //�G�̗̑�
	int nCntPreAttack;                             //�e���˂܂ł̎���
	int nCntBullet;                                //���A�˂ł���e�̐�
	float fBullet_MoveX;                           //�e�̈ړ����x

	//���^�C�v�̃p�����[�^
	bool bRecoveryFlag;                            //�d����ԃt���O
	int nCntRecovery;                              //�U����d������
	bool bAttack;                                  //�U���t���O
	bool bJump;                                    //�W�����v�t���O
	bool bAttackDropFlag;                          //�W�����v�̗����������̍U������L���t���O
}ENEMYROBOT;

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------------------
HRESULT InitEnemyRobot(void);
void UninitEnemyRobot(void);
void UpdateEnemyRobot(void);
void DrawEnemyRobot(void);
void UpdatePolygonEnemyRobot(void);
void MakeVertexPolygonEnemyRobot(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx);
void SetEnemyRobot(int nType,float fPosX,float fPosY,float fMoveX,float fDirect,float nEnemyNum);    //EnemyRobot�̃Z�b�g�֐�
ENEMYROBOT *GetEnemyRobot(void);
void HitCheckForBullet(int nCnt);                              //�v���[���[�̒e�ɑ΂��铖���蔻��m�F

#endif
