#ifndef _MAIN_H_
#define _MAIN_H_

#pragma warning( disable: 4996 )
#pragma comment (lib,"d3d9.lib")
#pragma comment (lib,"d3dx9.lib")
#pragma comment (lib,"dxguid.lib")
#pragma comment (lib,"winmm.lib")             //�V�X�e�������擾�ɕK�v
#pragma comment (lib,"dinput8.lib")

//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
//-----------------------------------------------------------------------------------------
#define _USE_VOICECALLBACK_
#define DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>
#include <d3dx9.h>
#include <XAudio2.h>

//-----------------------------------------------------------------------------------------
//�}�N��
//-----------------------------------------------------------------------------------------
#define SCREEN_WIDTH (800)                         //�E�B���h�E�̕�
#define SCREEN_HEIGHT (640)                         //�E�B���h�E�̍���
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(memory) { if(memory != NULL) { memory -> Release(); memory = NULL; } }  //�������̃N���A
#endif
#define NUM_POLYGON (2)                             //�|���S����
#define PI (3.1415926535)
#define GRAVITY (0.15f)                             //�d��

//-----------------------------------------------------------------------------------------
//�\���̐錾
//-----------------------------------------------------------------------------------------
//���_�\����(���̃t�@�[�}�b�g�̌^�ɍ��킹�邱��)
typedef struct
{
	D3DXVECTOR3 pos;                         //
	float rhw;								 //���W�ϊ��ςݒ��_�t���O(�K��1.0f)
	D3DCOLOR color;						 //���_�J���[
	D3DXVECTOR2 tex;                         //�e�N�X�`�����W(UV or ST)
}VERTEX_2D;

typedef enum
{
	MODE_TITLE = 0,
	MODE_TUTORIAL,
	MODE_GAME,
	MODE_RESULT,
	MODE_MAX,
}MODE;

typedef struct
{
	bool bGamePause;
	bool bGameOver;
	int nGameResult;                   //���U���g:1�̓v���[���[�Q�[���I�[�o�[ 2�̓Q�[���N���A 3�̓Q�[���|�[�Y
}GAMEMANAGER;

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------------------
LPDIRECT3DDEVICE9 Get_Device(void);
void Uninit(void);                                                             //�I��(��Еt���֐�)
//�f�o�b�N�̎��̂ݕ\�����ꂽ�p�����[�^
#ifdef _DEBUG
void DrawFPS(void);                                 //FPS�̏󋵂�\������֐�
#endif
void SetMode(MODE mode);
MODE *GetMode(void);
GAMEMANAGER *GetManager();                          //�Q�[���Ǘ��t���O��Ԃ�

#endif