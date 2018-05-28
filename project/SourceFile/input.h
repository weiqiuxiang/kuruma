//=============================================================================
//
// 入力処理 [input.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//=============================================================================
// Joypad情報用マクロ
//=============================================================================
#define BUTTON_UP     0x00000001l	//方向キー上(dijs.IY<0)
#define BUTTON_DOWN   0x00000002l	//方向キー下(dijs.IY>0)
#define BUTTON_LEFT   0x00000004l	//方向キー左(dijs.IX<0)
#define BUTTON_RIGHT  0x00000008l	//方向キー右(dijs.IX>0)
#define BUTTON_A      0x00000010l	//Ａボタン(dijs.rgbButtons[0]&0x80)
#define BUTTON_B      0x00000020l	//Ｂボタン(dijs.rgbButtons[1]&0x80)
#define BUTTON_C      0x00000040l	//Ｃボタン(dijs.rgbButtons[2]&0x80)
#define BUTTON_X      0x00000080l	//Ｘボタン(dijs.rgbButtons[3]&0x80)
#define BUTTON_Y      0x00000100l	//Ｙボタン(dijs.rgbButtons[4]&0x80)
#define BUTTON_Z      0x00000200l	//Ｚボタン(dijs.rgbButtons[5]&0x80)
#define BUTTON_L      0x00000400l	//Ｌボタン(dijs.rgbButtons[6]&0x80)
#define BUTTON_R      0x00000800l	//Ｒボタン(dijs.rgbButtons[7]&0x80)
#define BUTTON_START  0x00001000l	//ＳＴＡＲＴボタン(dijs.rgbButtons[8]&0x80)
#define BUTTON_M      0x00002000l	//Ｍボタン(dijs.rgbButtons[9]&0x80)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//----------
//キーボード
//----------
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);

bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRepeat(int nKey);
bool GetKeyboardRelease(int nKey);

//----------
//マウス
//----------

//----------
//コントローラ
//----------
HRESULT InitController(HINSTANCE hInstance, HWND hWnd);
void UninitController(void);
void UpdateController(void);

bool GetControllerPress(int nButton);
bool GetControllerTrigger(int nButton);
bool GetControllerRepeat(int nButton);
bool GetControllerRelease(int nButton);

//コールバック関数
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance,VOID *pContext);
BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *pdidoi,VOID *pContext);

//test
DIJOYSTATE *GetJS(void);

#endif