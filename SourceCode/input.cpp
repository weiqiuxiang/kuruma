//=============================================================================
//
// ���͏��� [input.cpp]
// Author : AKIRA TANAKA
//
//=============================================================================
#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	NUM_KEY_MAX			(256)	// �L�[�̍ő吔
#define	LIMIT_COUNT_REPEAT	(8)	// ���s�[�g�J�E���g���~�b�^�[

#define NUM_BUTTON_MAX      (32)    //�{�^���̍ő吔

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd);
void UninitInput(void);
void UpdateInput(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECTINPUT8			g_pInput = NULL;					// DirectInput�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECTINPUTDEVICE8	g_pDevKeyboard = NULL;				// ���̓f�o�C�X(�L�[�{�[�h)�ւ̃|�C���^
LPDIRECTINPUTDEVICE8	g_pDevController = NULL;			// ���̓f�o�C�X(�L�[�{�[�h)�ւ̃|�C���^
DIDEVCAPS               g_diDevCaps;                        // ?
BYTE					g_aKeyState[NUM_KEY_MAX];			// �L�[�{�[�h�̓��͏�񃏁[�N
BYTE					g_aKeyStateTrigger[NUM_KEY_MAX];	// �L�[�{�[�h�̃g���K�[��񃏁[�N
BYTE					g_aKeyStateRelease[NUM_KEY_MAX];	// �L�[�{�[�h�̃����[�X��񃏁[�N
BYTE					g_aKeyStateRepeat[NUM_KEY_MAX];		// �L�[�{�[�h�̃��s�[�g��񃏁[�N
int						g_aKeyStateRepeatCnt[NUM_KEY_MAX];	// �L�[�{�[�h�̃��s�[�g�J�E���^

BYTE					g_aButtonState[NUM_BUTTON_MAX];			    // �R���g���[���̓��͏�񃏁[�N
BYTE					g_aButtonStateTrigger[NUM_BUTTON_MAX];	    // �R���g���[���̃g���K�[��񃏁[�N
BYTE					g_aButtonStateRelease[NUM_BUTTON_MAX];	    // �R���g���[���̃����[�X��񃏁[�N
BYTE					g_aButtonStateRepeat[NUM_BUTTON_MAX];		// �R���g���[���̃��s�[�g��񃏁[�N
int						g_aButtonStateRepeatCnt[NUM_BUTTON_MAX];	// �R���g���[���̃��s�[�g�J�E���^

DIJOYSTATE js;

//=============================================================================
// ���͏����̏�����
//=============================================================================
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd)
{
	if(g_pInput == NULL)
	{
		// DirectInput�I�u�W�F�N�g�̍쐬
		if(FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
										IID_IDirectInput8, (void**)&g_pInput, NULL)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// ���͏����̏I������
//=============================================================================
void UninitInput(void)
{
	if(g_pInput != NULL)
	{// DirectInput�I�u�W�F�N�g�̊J��
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//=============================================================================
// ���͏����̍X�V����
//=============================================================================
void UpdateInput(void)
{
}

/*********************************************************�L�[�{�[�h************************************************************/

//=============================================================================
// �L�[�{�[�h�̏�����
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	// ���͏����̏�����
	if(FAILED(InitInput(hInstance, hWnd)))
	{
		MessageBox(hWnd, "DirectInput�I�u�W�F�N�g�����˂��I", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	// �f�o�C�X�̍쐬
	if(FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		MessageBox(hWnd, "�L�[�{�[�h���˂��I", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if(FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	if(FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//=============================================================================
// �L�[�{�[�h�̏I������
//=============================================================================
void UninitKeyboard(void)
{
	if(g_pDevKeyboard != NULL)
	{// ���̓f�o�C�X(�L�[�{�[�h)�̊J��
		// �L�[�{�[�h�ւ̃A�N�Z�X�����J��(���͐���I��)
		g_pDevKeyboard->Unacquire();

		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	// ���͏����̏I������
	UninitInput();
}

//=============================================================================
// �L�[�{�[�h�̍X�V����
//=============================================================================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];

	// �f�o�C�X����f�[�^���擾
	if(SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), aKeyState)))
	{
		for(int nCnKey = 0; nCnKey < NUM_KEY_MAX; nCnKey++)
		{
			// �L�[�v���X����ۑ�
			//�L�[�g���K�[�����[�X���̍쐬
			g_aKeyStateTrigger[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey]) & aKeyState[nCnKey];                          //g_aKeyState�͑O,aKeyState�͍�
			g_aKeyStateRelease[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey]) & g_aKeyState[nCnKey];                        //g_aKeyState�͑O,aKeyState�͍�
			g_aKeyState[nCnKey] = aKeyState[nCnKey];

			//�L�[���r�[�Ə��̐���
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

	//���s�����ꍇ�͂�����x�L�[�{�[�h�ւ̃A�X�Z�X��v������
	else
	{
		// �L�[�{�[�h�ւ̃A�N�Z�X�����擾
		g_pDevKeyboard->Acquire();
	}
}

//=============================================================================
// �L�[�{�[�h�̃v���X��Ԃ��擾
//=============================================================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true: false;
}

//=============================================================================
// �L�[�{�[�h�̃g���K�[��Ԃ��擾
//=============================================================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true: false;
}

//=============================================================================
// �L�[�{�[�h�̃��s�[�g��Ԃ��擾
//=============================================================================
bool GetKeyboardRepeat(int nKey)
{
	return (g_aKeyStateRepeat[nKey] & 0x80) ? true: false;
}

//=============================================================================
// �L�[�{�[�h�̃����|�X��Ԃ��擾
//=============================================================================
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true: false;
}

/*********************************************************�R���g���[��************************************************************/

//=============================================================================
// �R���g���[���[�̏�����
//=============================================================================
HRESULT InitController(HINSTANCE hInstance, HWND hWnd)
{
	// ���͏����̏�����
	if(FAILED(InitInput(hInstance, hWnd)))
	{
		MessageBox(hWnd, "DirectInput�I�u�W�F�N�g�����Ȃ��I", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	// �f�o�C�X�̍쐬
	if(FAILED(g_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL,DIEDFL_ATTACHEDONLY)))
	{
		MessageBox(hWnd, "�R���g���[�^�ڑ����Ă��Ȃ��I", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	if (g_pDevController != NULL)
	{
		// �f�[�^�t�H�[�}�b�g��ݒ�
		if (FAILED(g_pDevController->SetDataFormat(&c_dfDIJoystick)))
		{
			MessageBox(hWnd, "�R���g���[���̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
			return E_FAIL;
		}

		// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
		if (FAILED(g_pDevController->SetCooperativeLevel(hWnd, (DISCL_EXCLUSIVE | DISCL_FOREGROUND))))
		{
			MessageBox(hWnd, "�R���g���[���̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
			return E_FAIL;
		}

		//�f�o�C�X�̔\�͎擾
		g_diDevCaps.dwSize = sizeof(DIDEVCAPS);
		if (FAILED(g_pDevController->GetCapabilities(&g_diDevCaps)))
		{
			MessageBox(hWnd, "�R���g���[���̔\�͂�ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
			return E_FAIL;
		}

		//�f�o�C�X�ɑ΂��ď\���L�[�͈͓̔����w�肷��
		if (g_pDevController->EnumObjects(EnumAxesCallback, (VOID*)hWnd, DIDFT_AXIS))
		{
			MessageBox(hWnd, "�R���g���[���̏\���L�[�͈͓̔���ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
			return E_FAIL;
		}

		// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
		g_pDevController->Acquire();
	}

	return S_OK;
}

//=============================================================================
// �R���g���[���̏I������
//=============================================================================
void UninitController(void)
{
	if(g_pDevController != NULL)
	{// ���̓f�o�C�X(�L�[�{�[�h)�̊J��
		// �L�[�{�[�h�ւ̃A�N�Z�X�����J��(���͐���I��)
		g_pDevController->Unacquire();

		g_pDevController->Release();
		g_pDevController = NULL;
	}

	// ���͏����̏I������
	UninitInput();
}

//=============================================================================
// �R���g���[���̍X�V����
//=============================================================================
void UpdateController(void)
{
	if (g_pDevController != NULL)
	{
		g_pDevController->Poll();

		// �f�o�C�X����f�[�^���擾
		if (SUCCEEDED(g_pDevController->GetDeviceState(sizeof(DIJOYSTATE), &js)))
		{
			for (int nCnButton = 0; nCnButton < NUM_BUTTON_MAX; nCnButton++)
			{
				// �L�[�v���X����ۑ�
				//�L�[�g���K�[�����[�X���̍쐬
				g_aButtonStateTrigger[nCnButton] = (g_aButtonState[nCnButton] ^ js.rgbButtons[nCnButton]) & js.rgbButtons[nCnButton];                          //g_aKeyState�͑O,aKeyState�͍�
				g_aButtonStateRelease[nCnButton] = (g_aButtonState[nCnButton] ^ js.rgbButtons[nCnButton]) & g_aButtonState[nCnButton];                        //g_aKeyState�͑O,aKeyState�͍�
				g_aButtonState[nCnButton] = js.rgbButtons[nCnButton];

				//�L�[���r�[�Ə��̐���
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

		//���s�����ꍇ�͂�����x�R���g���[���ւ̃A�X�Z�X��v������
		else
		{
			// �R���g���[���ւ̃A�N�Z�X�����擾
			g_pDevController->Acquire();
		}
	}
}

//=============================================================================
//�R�[���o�b�N�֐�
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
// �R���g���[���̃v���X��Ԃ��擾
//=============================================================================
bool GetControllerPress(int nButton)
{
	return (g_aButtonState[nButton]) ? true: false;
}

//=============================================================================
// �R���g���[���̃g���K�[��Ԃ��擾
//=============================================================================
bool GetControllerTrigger(int nButton)
{
	return (g_aButtonStateTrigger[nButton]) ? true: false;
}

//=============================================================================
// �R���g���[���̃��s�[�g��Ԃ��擾
//=============================================================================
bool GetControllerRepeat(int nButton)
{
	return (g_aButtonStateRepeat[nButton]) ? true: false;
}

//=============================================================================
// �R���g���[���̃����|�X��Ԃ��擾
//=============================================================================
bool GetControllerRelease(int nButton)
{
	return (g_aButtonStateRelease[nButton]) ? true: false;
}

DIJOYSTATE *GetJS(void)
{
	return &js;
}

