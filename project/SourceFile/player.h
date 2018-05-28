//-----------------------------------------------------------------------------------------
//�v���[���[�w�b�h�t�@�C��[player.h]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------------------
#ifndef _PLAYER_H_
#define _PLAYER_H_

//-----------------------------------------------------------------------------------------
//���_�t�H�[�}�b�g
//-----------------------------------------------------------------------------------------
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define NUM_PLAYERVERTEX (4)                            //���_��
#define NUM_PLAYERPOLYGON (8)						    //��`��
#define NUM_PLAYERTEXTURE (3)                           //�e�N�X�`����
#define NUM_SCOREDIGIT (6)					            //����

//�v���[���[�̕�����
#define PLAYER_WIDTH (128.0f)
#define PLAYER_HEIGHT (64.0f)

//�v���[���[�̃}�V���K���̕�����
#define GUN_WIDTH (24.0f)
#define GUN_HEIGHT (24.0f)
#define GUN_DISTANCEX (50.0f)
#define GUN_DISTANCEY (25.0f)

//�v���[���[���蔻��͈͂̒���
#define PLAYER_TOP (10.0f)
#define PLAYER_HORRIZON (16.0f)
#define PLAYER_HEIGHT_CHECK (16.0f)

//-----------------------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------------------
#define PLAYERPOLYGON01_TEXTURENAME "data/TEXTURE/sozai/Player.png"
#define PLAYERPOLYGON02_TEXTURENAME "data/TEXTURE/sozai/MachineGun.png"
#define PLAYERPOLYGON03_TEXTURENAME "data/TEXTURE/sozai/HP.png"

//-----------------------------------------------------------------------------------------
//�v���[���[�̍\����
//-----------------------------------------------------------------------------------------
typedef struct
{
	float fPosX;                                   //�v���[���[���݂̈ʒu(X���W)
	float fPosY;                                   //�v���[���[���݂̈ʒu(Y���W)
	float fPosXOld;                                //�O��̈ʒu(X���W)
	float fPosYOld;                                //�O��̈ʒu(Y���W)
	float fDirect;                                 //�����̃t���O(-0.1f�Ȃ獶,0.1f�Ȃ�E)
	float fMoveX;                                  //�v���[���[��X���̃X�s�[�h
	float fPosY_Move;                              //Y�������̃X�s�[�h
	float fScore;                                  //�v���[���[�̃X�R�A
	float fGunAngel;                               //�}�V���K���̉�]�p�x
	char aScore[20];                               //�v���[���[�̃X�R�A�̕�����
	bool bJump;                                    //�W�����v�̃t���O
	bool bJump_Once;                               //�W�����v�{�^���̉������ςȂ��h�~
	bool bRun;                                     //�_�b�V���̃t���O
	bool bUse;                                     //�v���[���[���g�����ǂ���
	bool bHit;                                     //�v���[���[���_���[�W���󂯂�t���O
	int nLife;                                     //�v���[���[�̃��C�t
	int nInvinFrame;                               //�v���[���[�̃_���[�W�󂯂���і��G�t���[��

	//�J�����ړ��̃p�����[�^
	float fPosX_SUM;                               //X���ړ�����(�J����X���W)
	float fRecoveryDistance;                       //�J�����񕜋���
	bool bCameraRecovery;                          //�J�����񕜃t���O

	//��e�̃p�����[�^
	bool bDamage;                                  //�_���[�W�󂯃t���O
	int nCntInvincible;                            //���G����
}PLAYER;

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------------------
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void UpdatePolygonPlayer(void);
void MakeVertexPolygonPlayer(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx);
void SpriteRorate (VERTEX_2D *pVtx);                                              //��]�p�֐�
PLAYER *GetPlayer(void);

#endif
