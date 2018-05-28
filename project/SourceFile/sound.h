//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// BGM0
	//SOUND_LABEL_BGM001,			// BGM1
	//SOUND_LABEL_BGM002,			// BGM2
	SOUND_LABEL_SE_HITENEMY,			// �q�b�g��
	SOUND_LABEL_SE_EXPLOSION,	// ������
	SOUND_LABEL_BGM_STAGE,		// stagebgm
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);                                 //�T�E���h�̏���������
void UninitSound(void);                                       //�I�������i�f�o�C�X������j
HRESULT PlaySound(SOUND_LABEL label);                         //�w�胉�x���̃T�E���h���Đ�
void StopSound(SOUND_LABEL label);                            //����̃T�E���h���~�߂�
void StopSound(void);                                         //�S���̃T�E���h���~�߂�

#endif
