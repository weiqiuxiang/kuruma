//=============================================================================
//
// 入力処理 [input.cpp]
// Author : AKIRA TANAKA
//
//=============================================================================
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	NUM_KEY_MAX			(256)	// キーの最大数
#define	LIMIT_COUNT_REPEAT	(8)	// リピートカウントリミッター

#define NUM_BUTTON_MAX      (32)    //ボタンの最大数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd);
void UninitInput(void);
void UpdateInput(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECTINPUT8			g_pInput = NULL;					// DirectInputオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8	g_pDevKeyboard = NULL;				// 入力デバイス(キーボード)へのポインタ
LPDIRECTINPUTDEVICE8	g_pDevController = NULL;			// 入力デバイス(キーボード)へのポインタ
DIDEVCAPS               g_diDevCaps;                        // ?
BYTE					g_aKeyState[NUM_KEY_MAX];			// キーボードの入力情報ワーク
BYTE					g_aKeyStateTrigger[NUM_KEY_MAX];	// キーボードのトリガー情報ワーク
BYTE					g_aKeyStateRelease[NUM_KEY_MAX];	// キーボードのリリース情報ワーク
BYTE					g_aKeyStateRepeat[NUM_KEY_MAX];		// キーボードのリピート情報ワーク
int						g_aKeyStateRepeatCnt[NUM_KEY_MAX];	// キーボードのリピートカウンタ

BYTE					g_aButtonState[NUM_BUTTON_MAX];			    // コントローラの入力情報ワーク
BYTE					g_aButtonStateTrigger[NUM_BUTTON_MAX];	    // コントローラのトリガー情報ワーク
BYTE					g_aButtonStateRelease[NUM_BUTTON_MAX];	    // コントローラのリリース情報ワーク
BYTE					g_aButtonStateRepeat[NUM_BUTTON_MAX];		// コントローラのリピート情報ワーク
int						g_aButtonStateRepeatCnt[NUM_BUTTON_MAX];	// コントローラのリピートカウンタ

DIJOYSTATE js;

//=============================================================================
// 入力処理の初期化
//=============================================================================
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd)
{
	if(g_pInput == NULL)
	{
		// DirectInputオブジェクトの作成
		if(FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
										IID_IDirectInput8, (void**)&g_pInput, NULL)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 入力処理の終了処理
//=============================================================================
void UninitInput(void)
{
	if(g_pInput != NULL)
	{// DirectInputオブジェクトの開放
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//=============================================================================
// 入力処理の更新処理
//=============================================================================
void UpdateInput(void)
{
}

/*********************************************************キーボード************************************************************/

//=============================================================================
// キーボードの初期化
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	// 入力処理の初期化
	if(FAILED(InitInput(hInstance, hWnd)))
	{
		MessageBox(hWnd, "DirectInputオブジェクトが作れねぇ！", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// デバイスの作成
	if(FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		MessageBox(hWnd, "キーボードがねぇ！", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// データフォーマットを設定
	if(FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		MessageBox(hWnd, "キーボードのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	if(FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		MessageBox(hWnd, "キーボードの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//=============================================================================
// キーボードの終了処理
//=============================================================================
void UninitKeyboard(void)
{
	if(g_pDevKeyboard != NULL)
	{// 入力デバイス(キーボード)の開放
		// キーボードへのアクセス権を開放(入力制御終了)
		g_pDevKeyboard->Unacquire();

		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	// 入力処理の終了処理
	UninitInput();
}

//=============================================================================
// キーボードの更新処理
//=============================================================================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];

	// デバイスからデータを取得
	if(SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), aKeyState)))
	{
		for(int nCnKey = 0; nCnKey < NUM_KEY_MAX; nCnKey++)
		{
			// キープレス情報を保存
			//キートリガーリリース情報の作成
			g_aKeyStateTrigger[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey]) & aKeyState[nCnKey];                          //g_aKeyStateは前,aKeyStateは今
			g_aKeyStateRelease[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey]) & g_aKeyState[nCnKey];                        //g_aKeyStateは前,aKeyStateは今
			g_aKeyState[nCnKey] = aKeyState[nCnKey];

			//キーリビーと情報の生成
			if(aKeyState[nCnKey])
			{
				if(g_aKeyStateRepeatCnt[nCnKey] < LIMIT_COUNT_REPEAT)
				{
					g_aKeyStateRepeatCnt[nCnKey]++;
					if(g_aKeyStateRepeatCnt[nCnKey] == 1 || g_aKeyStateRepeatCnt[nCnKey] >= LIMIT_COUNT_REPEAT) 
					{
						g_aKeyStateRepeat[nCnKey] = aKeyState[nCnKey];
						g_aKeyStateRepeatCnt[nCnKey] = 2;
					}

					else
					{
						g_aKeyStateRepeat[nCnKey] = 0;
					}
				}
			}

			else
			{
				g_aKeyStateRepeat[nCnKey] = 0;
				g_aKeyStateRepeatCnt[nCnKey] = 0; 
			}
		}
	}

	//失敗した場合はもう一度キーボードへのアスセスを要求する
	else
	{
		// キーボードへのアクセス権を取得
		g_pDevKeyboard->Acquire();
	}
}

//=============================================================================
// キーボードのプレス状態を取得
//=============================================================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true: false;
}

//=============================================================================
// キーボードのトリガー状態を取得
//=============================================================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true: false;
}

//=============================================================================
// キーボードのリピート状態を取得
//=============================================================================
bool GetKeyboardRepeat(int nKey)
{
	return (g_aKeyStateRepeat[nKey] & 0x80) ? true: false;
}

//=============================================================================
// キーボードのリリ－ス状態を取得
//=============================================================================
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true: false;
}

/*********************************************************コントローラ************************************************************/

//=============================================================================
// コントローラーの初期化
//=============================================================================
HRESULT InitController(HINSTANCE hInstance, HWND hWnd)
{
	// 入力処理の初期化
	if(FAILED(InitInput(hInstance, hWnd)))
	{
		MessageBox(hWnd, "DirectInputオブジェクトが作れない！", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// デバイスの作成
	if(FAILED(g_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL,DIEDFL_ATTACHEDONLY)))
	{
		MessageBox(hWnd, "コントロータ接続していない！", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	if (g_pDevController != NULL)
	{
		// データフォーマットを設定
		if (FAILED(g_pDevController->SetDataFormat(&c_dfDIJoystick)))
		{
			MessageBox(hWnd, "コントローラのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
			return E_FAIL;
		}

		// 協調モードを設定（フォアグラウンド＆非排他モード）
		if (FAILED(g_pDevController->SetCooperativeLevel(hWnd, (DISCL_EXCLUSIVE | DISCL_FOREGROUND))))
		{
			MessageBox(hWnd, "コントローラの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
			return E_FAIL;
		}

		//デバイスの能力取得
		g_diDevCaps.dwSize = sizeof(DIDEVCAPS);
		if (FAILED(g_pDevController->GetCapabilities(&g_diDevCaps)))
		{
			MessageBox(hWnd, "コントローラの能力を設定できませんでした。", "警告！", MB_ICONWARNING);
			return E_FAIL;
		}

		//デバイスに対して十字キーの範囲等を指定する
		if (g_pDevController->EnumObjects(EnumAxesCallback, (VOID*)hWnd, DIDFT_AXIS))
		{
			MessageBox(hWnd, "コントローラの十字キーの範囲等を設定できませんでした。", "警告！", MB_ICONWARNING);
			return E_FAIL;
		}

		// キーボードへのアクセス権を獲得(入力制御開始)
		g_pDevController->Acquire();
	}

	return S_OK;
}

//=============================================================================
// コントローラの終了処理
//=============================================================================
void UninitController(void)
{
	if(g_pDevController != NULL)
	{// 入力デバイス(キーボード)の開放
		// キーボードへのアクセス権を開放(入力制御終了)
		g_pDevController->Unacquire();

		g_pDevController->Release();
		g_pDevController = NULL;
	}

	// 入力処理の終了処理
	UninitInput();
}

//=============================================================================
// コントローラの更新処理
//=============================================================================
void UpdateController(void)
{
	if (g_pDevController != NULL)
	{
		g_pDevController->Poll();

		// デバイスからデータを取得
		if (SUCCEEDED(g_pDevController->GetDeviceState(sizeof(DIJOYSTATE), &js)))
		{
			for (int nCnButton = 0; nCnButton < NUM_BUTTON_MAX; nCnButton++)
			{
				// キープレス情報を保存
				//キートリガーリリース情報の作成
				g_aButtonStateTrigger[nCnButton] = (g_aButtonState[nCnButton] ^ js.rgbButtons[nCnButton]) & js.rgbButtons[nCnButton];                          //g_aKeyStateは前,aKeyStateは今
				g_aButtonStateRelease[nCnButton] = (g_aButtonState[nCnButton] ^ js.rgbButtons[nCnButton]) & g_aButtonState[nCnButton];                        //g_aKeyStateは前,aKeyStateは今
				g_aButtonState[nCnButton] = js.rgbButtons[nCnButton];

				//キーリビーと情報の生成
				if (js.rgbButtons[nCnButton])
				{
					if (g_aButtonStateRepeatCnt[nCnButton] < LIMIT_COUNT_REPEAT)
					{
						g_aButtonStateRepeatCnt[nCnButton]++;
						if (g_aButtonStateRepeatCnt[nCnButton] == 1 || g_aButtonStateRepeatCnt[nCnButton] >= LIMIT_COUNT_REPEAT)
						{
							g_aButtonStateRepeat[nCnButton] = js.rgbButtons[nCnButton];
							g_aButtonStateRepeatCnt[nCnButton] = 2;
						}

						else
						{
							g_aButtonStateRepeat[nCnButton] = 0;
						}
					}
				}

				else
				{
					g_aButtonStateRepeat[nCnButton] = 0;
					g_aButtonStateRepeatCnt[nCnButton] = 0;
				}
			}
		}

		//失敗した場合はもう一度コントローラへのアスセスを要求する
		else
		{
			// コントローラへのアクセス権を取得
			g_pDevController->Acquire();
		}
	}
}

//=============================================================================
//コールバック関数
//=============================================================================
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance,VOID *pContext)
{
	HRESULT hr;
	hr = g_pInput->CreateDevice(pdidInstance->guidInstance,&g_pDevController,NULL);

	if ( FAILED( hr ) ) return DIENUM_CONTINUE;
	
	return DIENUM_STOP;
}

BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *pdidoi,VOID *pContext)
{
	HRESULT     hr;
	DIPROPRANGE diprg;
	
	diprg.diph.dwSize       = sizeof( DIPROPRANGE );
	diprg.diph.dwHeaderSize = sizeof( DIPROPHEADER );
	diprg.diph.dwHow        = DIPH_BYID;
	diprg.diph.dwObj        = pdidoi->dwType;
	diprg.lMin              = 0 - 1000;
	diprg.lMax              = 0 + 1000;
	hr = g_pDevController->SetProperty( DIPROP_RANGE , &diprg.diph );
	
	if ( FAILED( hr ) ) return DIENUM_STOP;
	
	return DIENUM_CONTINUE;
}

//=============================================================================
// コントローラのプレス状態を取得
//=============================================================================
bool GetControllerPress(int nButton)
{
	return (g_aButtonState[nButton]) ? true: false;
}

//=============================================================================
// コントローラのトリガー状態を取得
//=============================================================================
bool GetControllerTrigger(int nButton)
{
	return (g_aButtonStateTrigger[nButton]) ? true: false;
}

//=============================================================================
// コントローラのリピート状態を取得
//=============================================================================
bool GetControllerRepeat(int nButton)
{
	return (g_aButtonStateRepeat[nButton]) ? true: false;
}

//=============================================================================
// コントローラのリリ－ス状態を取得
//=============================================================================
bool GetControllerRelease(int nButton)
{
	return (g_aButtonStateRelease[nButton]) ? true: false;
}

DIJOYSTATE *GetJS(void)
{
	return &js;
}

