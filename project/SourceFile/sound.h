//=============================================================================
//
// サウンド処理 [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// BGM0
	//SOUND_LABEL_BGM001,			// BGM1
	//SOUND_LABEL_BGM002,			// BGM2
	SOUND_LABEL_SE_HITENEMY,			// ヒット音
	SOUND_LABEL_SE_EXPLOSION,	// 爆発音
	SOUND_LABEL_BGM_STAGE,		// stagebgm
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);                                 //サウンドの初期化処理
void UninitSound(void);                                       //終了処理（デバイスを解放）
HRESULT PlaySound(SOUND_LABEL label);                         //指定ラベルのサウンドを再生
void StopSound(SOUND_LABEL label);                            //特定のサウンドを止める
void StopSound(void);                                         //全部のサウンドを止める

#endif
