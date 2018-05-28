/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
���b�Z�[�WBOX�\������[main.cpp]

Author :
Data   :
----------------------------------------------------------------
��Update : 2016/04/18
 �E_tWinMain��WinMain�ɕύX
 �EWinMain��WINAPI��APIENTRY�ɕύX
 
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

//-----------------------------------------------------------------------------------------
//�C���N���[�h�w�b�h
//-----------------------------------------------------------------------------------------
#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include "main.h"
#include "BGpolygon.h"
#include "tutorial.h"
#include "enemy01.h"
#include "enemy02.h"
#include "enemyrobot.h"
#include "player.h"
#include "stage01.h"
#include "score.h"
#include "title.h"
#include "gameover.h"
#include "result.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "bullet.h"
#include "bulletEffect.h"
#include "bulletEnemy02.h"
#include "debug_hitcheck.h"

#include "particalCenter.h"
#include "partical.h"
#include "particalEffect.h"

//-----------------------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------------------
#define CLASS_NAME      "�T���v��"                  //�E�B���h�E�N���X�̖��O
#define WINDOW_NAME     "�E�C���h�E�\������"        //�E�B���h�E�̖��O

#define ID_BUTTON000 (101)
#define ID_EDIT000 (201)
#define ID_EDIT001 (202)

#define ID_TIMER (301)
#define TIMER_INTERVAL (1000/60)

//-----------------------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------------------
LPDIRECT3D9 g_pD3D = NULL;
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;            //Direct3D�f�o�C�X�C���^�[�t�F�C�X
int g_nCountFPS = 0;
MODE g_mode = MODE_TITLE;                         //��ʑJ�ڕϐ��̍쐬
GAMEMANAGER g_GameManager;                        //�Q�[���Ǘ�

//�f�o�b�N�̂ݕ\�����ꂽ�p�����[�^
#ifdef _DEBUG
LPD3DXFONT g_pFont = NULL;                        //FPS�̃t�H���g(�C���^�[�t�F�C�X)
#endif
LPD3DXFONT g_pGuideFont = NULL;

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
HRESULT Init(HINSTANCE hInstance,HWND hWnd,BOOL bWindow);
void Update(void);                                                             //�v���g�^�C�v�錾�̒�`
void Draw(void);                                                               //�`�揈��
void GameManager(void);

//-----------------------------------------------------------------------------------------
//���C���֐�
// (APIENTRY : Win32API�֐����Ăяo�����̋K��[WINAPI�ł��ǂ�])
// hInstance     : ���̃A�v���P�[�V�����̃C���X�^���X�n���h��(���ʎq)
// hPrevInstance : ���NULL(16bit����̖��c��)
// lpCmdLine     : �R�}���h���C������󂯎���������ւ̃|�C���^
// nCmdShow      : �A�v���P�[�V�����̏����E�C���h�E�\�����@�̎w��
//-----------------------------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	//warning��\�������Ȃ����߁A�Ȃ�����������Ȃ�
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	RECT cr = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
	AdjustWindowRect(&cr,WS_OVERLAPPEDWINDOW ^ (WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX),FALSE);

	RECT dr;
	GetWindowRect(GetDesktopWindow(),&dr);

	int dw = dr.right -dr.left;
	int dh = dr.bottom -dr.top;

	int ww = cr.right - cr.left;
	int wh = cr.bottom - cr.top;

	int wx = ww > dw ? 0 : (dw-ww)/2;
	int wy = wh > dh ? 0 : (dh-wh)/2;

	WNDCLASSEX wcex = 
	{
		sizeof(WNDCLASSEX),                     //WNDCLASSEX�̃������T�C�Y���w��
		CS_CLASSDC,                             //�\������E�B���h�E�̃X�^�C�����w��
		WndProc,								//�E�B���h�E�v���V�[�W���̃A�h���X(�֐���)���w��
		0,										//�ʏ�͎g�p���Ȃ��̂�"0"���w��
		0,										//�ʏ�͎g�p���Ȃ��̂�"0"���w��
		hInstance,								//WinMain�̃p�����[�^�̃C���X�^���X�n���h�����w��
		NULL,									//�g�p����A�C�R�����w��(Windows�������Ă���A�C�R�����g��)
		LoadCursor(NULL,IDC_ARROW),				//�}�E�X�J�[�\�����w��
		(HBRUSH)(COLOR_WINDOW + 1),				//�E�B���h�E�̃N���C�A���g�̈�̔w�i�F��ݒ�
		NULL,									//Window�ɂ��郁�j���[��ݒ�
		CLASS_NAME,								//�E�B���h�E�N���X�̖��O
		NULL									//�������A�C�R�����ݒ肳�ꂽ�ꍇ�̏����L�q
	};

	//�E�B���h�E�N���X�̓o�^(�I���W�i���̃E�B���h�E�̖��O�̓o�^)
	RegisterClassEx(&wcex);

	HWND hWnd;                                  //hWnd:�E�B���h�E�n���h��
	MSG msg;

	//�E�B���h�E���쐬
	hWnd = CreateWindowEx(0,                    //�g���E�B���h�E�X�^�C��
		CLASS_NAME,								//�E�B���h�E�N���X�̖��O
		WINDOW_NAME,							//�E�B���h�E�̖��O
		WS_OVERLAPPEDWINDOW ^ (WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX),					//�E�B���h�E�X�^�C��
		wx,							            //�E�B���h�E�̍���X���W
		wy,							            //�E�B���h�E�̍���Y���W
		ww,							            //�E�B���h�E�̕�
		wh,							            //�E�B���h�E�̍���
		NULL,									//�e�E�B���h�E�̃n���h��
		NULL,									//���j���[�n���h���܂��͎q�E�B���h�EID
		hInstance,								//�C���X�^���X�n���h��
		NULL);									//�E�B���h�E�쐬�f�[�^
	

	//�E�B���h�E�̕\��
	ShowWindow(hWnd,nCmdShow);					//�w�肳�ꂽ�E�C���h�E�̕\��
	UpdateWindow(hWnd);							//�E�B���h�E�̏�Ԓ����ɕԂ�

	//������
	Init(hInstance,hWnd,1);

	//����\�̐ݒ�
	timeBeginPeriod(1);

	//�e�J�E���^�[�̏�����
	DWORD dwFrameCount = 0;
	DWORD dwCurrentTime = 0;
	DWORD dwExecLastTime = timeGetTime();                  //���݂̎������~���b�P��
	DWORD dwFPSLastTime = dwExecLastTime;

	SetFade(FADE_IN,MODE_TITLE);
	//���b�Z�[�W���[�v
	for(;;)
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE) != 0 )
		{
			//Windows�̏���
			if(msg.message == WM_QUIT) break;
			else
			{
				TranslateMessage(&msg);               //�Ăяo�����X���b�h�̃��b�Z�[�W�L���[��
				DispatchMessage(&msg);                //���b�Z�[�W�̖|��Ƒ��o
			}
		}

		//�Q�[���̏���
		else
		{
			dwCurrentTime = timeGetTime();

			if(dwCurrentTime - dwFPSLastTime >= 500)
			{
				g_nCountFPS = ( (dwFrameCount*1000) / (dwCurrentTime - dwFPSLastTime));
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}

			//1�t���[���̎��Ԃ��o��������Q�[�����[�v
			if( (dwCurrentTime - dwExecLastTime) >= (1000/60) )
			{
				dwExecLastTime = dwCurrentTime;

				//DirectX�̏���
				Update();        //�X�V����
				Draw();          //�`�揈��

				dwFrameCount++;
			}
		}
	}

	//����\��߂�
	timeEndPeriod(1);

	//�I������
	Uninit();

	return (int)msg.wParam;
}

//-----------------------------------------------------------------------------------------
//�E�B���h�E�v���V�[�W��
//(CALLBACK:Win32API�֐����Ăяo�����̋K��)
//hWnd   :�E�B���h�E�̃n���h��
//uMsg   :���b�Z�[�W�̎��ʎq
//wParam :���b�Z�[�W�̍ŏ��̃p�����[�^
//LParam :���b�Z�[�W��2�Ԗڂ̃p�����[�^
//-----------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	static float x = 0;
	static float y = 0;

	static char buf[256];
	const RECT rect = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};

	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);							//"WM_QUIT"���b�Z�[�W��Ԃ�
		break;
		
	case WM_CREATE:
		
		SetTimer(hWnd,ID_TIMER,TIMER_INTERVAL,NULL);

		break;

	case WM_COMMAND:
		
		break;

	case WM_KEYDOWN:
		break;

	case WM_CLOSE:
		//MessageBox(NULL,"CLOSE","CLOSE",MB_YESNO);
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd,uMsg,wParam,lParam);  //����̏������(�E�B���h�E�̊�{�̓�����񋟂���(����͂Ȃ��Ɗ댯�I))
}

//���s�������̓��e���i�[�����Ă���
HRESULT Init(HINSTANCE hInstance,HWND hWnd,BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;         //�f�o�C�X�̓����ݒ肷��\����
	D3DDISPLAYMODE d3ddm;                //�f�o�C�X��IDirect3Device9�C���^�[�t�F�C�X

	//D3D9�C���^�[�t�F�C�X�̎擾
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	
	//�e�N�X�`���̓ǂ݂���
	if(g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//���݂̃f�B�X�v���C���[�h�̎擾
	//FAILED�}�N��:DIRECTX�v���O���������s���H�Ƃ����}�N��
	if ( FAILED( g_pD3D -> GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&d3ddm) ) )
	{
		return E_FAIL;
	}

	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�[�̐ݒ�
	ZeroMemory(&d3dpp,sizeof(d3dpp));
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	d3dpp.EnableAutoDepthStencil = TRUE;                           //DepthStencil���g���܂�
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;                     //�f�u�X�o�b�t�@16�r�b�g
	d3dpp.Windowed = bWindow;                                      //�E�B���h�E���[�h���t���X�N���[�����[�h��I��
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;    //�t���X�N���[�����[�h�ɂ�����
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;      //�t���X�N���[�����[�h�ɂ�����

	//Direct3D�f�o�C�X�̍쐬
	if(FAILED(g_pD3D -> CreateDevice(
		D3DADAPTER_DEFAULT,                                    //�ǂ̕\���n�[�h�E�F�A�ɂ��܂����H(default�Ȃ玩���̃p�\�R���̃X�N���[��)
		D3DDEVTYPE_HAL,                                        //�`�揈���^�C�v(�n�[�h�E�F�A�ł�邩�\�t�g�E�F�A�ł�邩�H_HAL�Ȃ�n�[�h�E�F�A)
		hWnd,                                                  //�E�B���h�E�n���h��
		D3DCREATE_HARDWARE_VERTEXPROCESSING,                   //�f�o�C�X�쐬�I�v�V�����t���O(���̏ꍇ:�n�[�h�E�F�A�Œ��_����������)
		&d3dpp,                                                //���̑��̐ݒ�
		&g_pD3DDevice)))                                       //Direct3D�f�o�C�X�C���^�[�t�F�C�X�̃|�C���^�̃|�C���^(�Ԑڂ���)
	{
		if(FAILED(g_pD3D -> CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&d3dpp,&g_pD3DDevice)))
		{
			if(FAILED(g_pD3D -> CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_REF,hWnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&d3dpp,&g_pD3DDevice)))
			{
				MessageBox(NULL,"�f�o�C�X�̎擾�����s���܂���","�G���[",MB_OK|MB_ICONHAND);
				return E_FAIL;
			}
		}
	}

#ifdef _DEBUG
	D3DXCreateFont(g_pD3DDevice,18,0,0,0,FALSE,SHIFTJIS_CHARSET,OUT_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,"Terminal",&g_pFont);
#endif //_DEBUG
	D3DXCreateFont(g_pD3DDevice,18,0,0,0,FALSE,SHIFTJIS_CHARSET,OUT_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,"Terminal",&g_pGuideFont);

	//�T���v���[�X�e�[�g�̐ݒ�
	// �����_�[�X�e�[�g�ݒ�
	
	g_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
	g_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	g_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_CURRENT );

	InitSound(hWnd);
	InitKeyboard(hInstance,hWnd);
	InitController(hInstance,hWnd);
	InitBGPolygon();
	InitPlayer();
	InitStage01();
	InitEnemy01();
	InitEnemy02();
	InitEnemyRobot();
	InitScorePolygon();
	InitTitle();
	InitFade();
	InitBullet();
	InitBulletEffect();
	InitBulletEnemy02();
	InitGameover();
	InitTutorial();

	InitParticalCenter();
	InitPartical();
	InitParticalEffect();

	SetMode(g_mode);
	g_GameManager.bGameOver = false;
	g_GameManager.bGamePause = false;
	g_GameManager.nGameResult = 0;

#ifdef _DEBUG
	InitHitcheck();
#endif

	srand((unsigned)time(NULL));                                //�����̎���Z�b�g

	return S_OK;
}
//�I���֐�(��ЂÂ�)
void Uninit(void)
{
	UninitKeyboard();
	UninitController();
	UninitSound();
	UninitBGPolygon();
	UninitPlayer();
	UninitStage01();
	UninitEnemy01();
	UninitEnemy02();
	UninitEnemyRobot();
	UninitScorePolygon();
	UninitTitle();
	UninitResult();
	UninitFade();
	UninitBullet();
	UninitBulletEffect();
	UninitBulletEnemy02();
	UninitGameover();
	UninitTutorial();

	UninitParticalCenter();
	UninitPartical();
	UninitParticalEffect();

#ifdef _DEBUG
	UninitHitcheck();
#endif
	
	SAFE_RELEASE (g_pD3DDevice);
	SAFE_RELEASE (g_pD3D);

#ifdef _DEBUG 
	SAFE_RELEASE(g_pFont);
#endif
	SAFE_RELEASE(g_pGuideFont);
}

////////////////////////////////////////////////
//�v���g�^�C�v�錾�̒�`
////////////////////////////////////////////////
void Update(void)
{
	//�L�[�{�[�h�ɂ��X�V����
	UpdateKeyboard();
	UpdateController();
	UpdateFade();
	//�X�V����
	switch(g_mode)
	{
	case MODE_TITLE:
		UpdateTitle();                                //�^�C�g��
		break;
	case MODE_TUTORIAL:
		UpdateTutorial();
		break;
	case MODE_GAME:
		if( (g_GameManager.bGameOver == false) && (g_GameManager.bGamePause == false) )
		{
			UpdatePlayer();
			UpdateStage01();
			UpdateBullet();
			UpdateBulletEffect();
			UpdateEnemy01();
			UpdateEnemy02();
			UpdateEnemyRobot();
			UpdateBulletEnemy02();
			UpdateBGPolygon();
			UpdateScorePolygon();

			UpdateParticalCenter();
			UpdatePartical();
			UpdateParticalEffect();
		}
		UpdateGameover();
#ifdef _DEBUG
		UpdateHitcheck();
#endif
		break;
	case MODE_RESULT:
		UpdateResult();                              //�Q�[�����ʂ̕\��
		break;
	}
}

////////////////////////////////////////////////
//�`�揈��
////////////////////////////////////////////////
void Draw(void)
{
	//�o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A
	g_pD3DDevice -> Clear(
		0,
		NULL,
		D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,          //�F|�[�x���
		D3DCOLOR_RGBA(0,0,0,0),                    //�N���A�F
		1.0f,                                      //�N���A�[�x�l(0:��O 1:��Ԍ��)
		0                                          //�X�e���V���l(0~255)
		);

	//�`��̊J�n
	if(SUCCEEDED(g_pD3DDevice -> BeginScene()))
	{
		switch(g_mode)
		{
		case MODE_TITLE:
			DrawTitle();                                //�^�C�g��
			break;
		case MODE_TUTORIAL:
			DrawTutorial();
			break;
		case MODE_GAME:
			{

				DrawBGPolygon();
				DrawPlayer();
				DrawBullet();                              //�e�̕`��
				DrawBulletEffect();                        //�e�̃G�t�F�N�g�̕`��
				//�G�̕`��
				DrawEnemy01();
				DrawEnemy02();
				DrawEnemyRobot();
				DrawBulletEnemy02();                       //�G�̒e�̕`��
				DrawScorePolygon();

				DrawPartical();
				DrawParticalEffect();

//////////////////////�f�o�b�N���b�Z�[�W�̕\��////////////////////////////
#ifdef _DEBUG
				static bool DrawHitCheckFlag = true;
				if(GetKeyboardTrigger(DIK_F1))
				{

					if(DrawHitCheckFlag == false) DrawHitCheckFlag = true;
					else DrawHitCheckFlag = false;
				}
				if(DrawHitCheckFlag == true)
				{
					DrawHitcheck();
				}
#endif
//////////////////////�f�o�b�N���b�Z�[�W�̕\��////////////////////////////

				//�Q�[���I�[�o�[,�N���A�������̓|�[�Y�̎��̕`��
				if( (g_GameManager.bGameOver == true) || (g_GameManager.bGamePause == true) )
				{
					DrawGameover();
				}
				break;
			}

		case MODE_RESULT:
			DrawResult();                              //�Q�[�����ʂ̕\��
			break;
		}

		DrawFade();                                   //fade�f�[�X
		
#ifdef _DEBUG
		DrawFPS();
#endif //_DEBUG
		//�`��I��
		g_pD3DDevice -> EndScene();
	}

	g_pD3DDevice -> Present(NULL,NULL,NULL,NULL);
}

////////////////////////////////////////////////
//DIRECT3D�f�o�C�X���Q�b�g
////////////////////////////////////////////////
LPDIRECT3DDEVICE9 Get_Device(void)
{
	return g_pD3DDevice;
}

#ifdef _DEBUG
//�t�H���g�\��
void DrawFPS(void)
{
	char aStr[256];
	RECT rect = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
	sprintf(&aStr[0],"FPS:%d\n",g_nCountFPS);
	g_pFont -> DrawText(NULL,&aStr[0],-1,&rect,DT_LEFT,D3DCOLOR_RGBA(0,255,0,255));                  //DT_LEFT�͍��l��(���ނ̂�ƈꏏ�Ɏg����,���� | ���g��,)
}
#endif  //_DEBUG

////////////////////////////////////////////////
//���݂̉�ʃ��[�h���Q�b�g
////////////////////////////////////////////////
MODE *GetMode(void)
{
	return &g_mode;
}

////////////////////////////////////////////////
//��ʃ��[�h���Z�b�g
////////////////////////////////////////////////
void SetMode(MODE mode)
{
	//�I������
	switch(mode)
	{
	case MODE_TITLE:
		UninitTitle();                                //�^�C�g��
		break;

	case MODE_TUTORIAL:
		UninitTutorial();
		break;

	case MODE_GAME:
		UninitPlayer();
		UninitStage01();
		UninitEnemy01();
		UninitEnemy02();
		UninitEnemyRobot();
		UninitBullet();
		UninitBulletEffect();
		UninitBulletEnemy02();
		UninitBGPolygon();
		UninitScorePolygon();
		UninitParticalCenter();
		UninitPartical();
		UninitParticalEffect();
#ifdef _DEBUG
		UninitHitcheck();
#endif
		break;
	case MODE_RESULT:
		UninitResult();                                //�^�C�g��
		break;
	}

	//����������
	switch(mode)
	{
	case MODE_TITLE:
		InitTitle();                                //�^�C�g��
		break;
	case MODE_TUTORIAL:
		InitTutorial();
		break;
	case MODE_GAME:
		InitPlayer();
		InitStage01();
		InitEnemy01();
		InitEnemy02();
		InitEnemyRobot();
		InitBullet();
		InitBulletEffect();
		InitBulletEnemy02();
		InitBGPolygon();
		InitScorePolygon();
		InitGameover();

		InitParticalCenter();
		InitPartical();
		InitParticalEffect();

		g_GameManager.bGameOver = false;
		g_GameManager.bGamePause = false;
		g_GameManager.nGameResult = 0;
#ifdef _DEBUG
		InitHitcheck();
#endif
		break;
	case MODE_RESULT:
		InitResult();                                //�^�C�g��
		break;
	}

	//���[�h�ϊ��ƃt���O�𗧂�����
	g_mode = mode;
}

////////////////////////////////////////////////
//�Q�[���Ǘ�
////////////////////////////////////////////////
void GameManager(void)
{
	if(g_mode == MODE_GAME)
	{
		if(g_GameManager.bGameOver == false)
		{
			//�Q�[�����ꎞ��~
			if( GetKeyboardTrigger(DIK_ESCAPE) )
			{
				if(g_GameManager.bGamePause == false)
				{
					g_GameManager.bGamePause = true;
				}

				else
				{
					g_GameManager.bGamePause = false;
				}
			}
		}

		//�Q�[���I���̒m�点
		else
		{
			if(g_GameManager.bGameOver == true)
			{
				PLAYER *pPlayer = GetPlayer();

				//�v���[���[������
				if(pPlayer -> nLife <=0)
				{
					
				}
				//�G�̑S�Ŋm�F
				else
				{
				
				}
			}
		}
	}
}

GAMEMANAGER *GetManager()
{
	return &g_GameManager;
}
