//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include <Windows.h>
#include "main.h"
#include "BGpolygon.h"
#include "player.h"
#include "input.h"

//-----------------------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureBGPolygon[NUM_BGTEXTURE] = {};           //テクスチャを操作するためのポインタ(テクスチャバッファインターフェイスにアクセスためのポインタ)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferBGPolygon[NUM_BGPOLYGON] = {};    //頂点バッファを操作するためのポインタ(頂点バッファインターフェイスにアクセスためのポインタ)

////////////////////////////////////////////////
//ポリゴン(頂点)の初期化
////////////////////////////////////////////////
HRESULT InitBGPolygon(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_2D *pVtx = NULL;                           //頂点構造体のポインタ
	pD3DDevice = Get_Device();

	//頂点バッファの生成
	if(FAILED(pD3DDevice -> CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_BGVERTEX*NUM_BGPOLYGON,D3DUSAGE_WRITEONLY,FVF_VERTEX2D,D3DPOOL_MANAGED,&g_pVtxBufferBGPolygon[0],NULL)))
	{
		return E_FAIL;
	}

	//背景テクスチャ01の取得
	if ( FAILED( D3DXCreateTextureFromFile(pD3DDevice, BGPOLYGON01_TEXTURENAME,&g_pTextureBGPolygon[0]) ) )
	{
		MessageBox(NULL,"テクスチャの読み込みが失敗しました","エラー",MB_OK|MB_ICONHAND);
		return E_FAIL;
	}

	//背景テクスチャ02の取得
	if ( FAILED( D3DXCreateTextureFromFile(pD3DDevice, BGPOLYGON02_TEXTURENAME,&g_pTextureBGPolygon[1]) ) )
	{
		MessageBox(NULL,"テクスチャの読み込みが失敗しました","エラー",MB_OK|MB_ICONHAND);
		return E_FAIL;
	}

	//背景の空のテクスチャの取得
	if ( FAILED( D3DXCreateTextureFromFile(pD3DDevice, BGPOLYGON03_TEXTURENAME,&g_pTextureBGPolygon[2]) ) )
	{
		MessageBox(NULL,"テクスチャの読み込みが失敗しました","エラー",MB_OK|MB_ICONHAND);
		return E_FAIL;
	}

	//背景の空ポリゴン
	g_pVtxBufferBGPolygon[0] -> Lock(0,0,(void**)&pVtx,0);                    //仮想アドレスを取得しバッファをロックする

	MakeVertexPolygonBG(pD3DDevice,pVtx);
	pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[1].pos = D3DXVECTOR3(BG_WIDTH,0.0f,0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, BG_HEIGHT,0.0f);
	pVtx[3].pos = D3DXVECTOR3(BG_WIDTH,BG_HEIGHT,0.0f);

	pVtx += 4;

	pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[1].pos = D3DXVECTOR3(BG_WIDTH,0.0f,0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, BG_HEIGHT,0.0f);
	pVtx[3].pos = D3DXVECTOR3(BG_WIDTH,BG_HEIGHT,0.0f);

	pVtx += 4;

	pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[1].pos = D3DXVECTOR3(BG_WIDTH,0.0f,0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, BG_HEIGHT,0.0f);
	pVtx[3].pos = D3DXVECTOR3(BG_WIDTH,BG_HEIGHT,0.0f);

	g_pVtxBufferBGPolygon[0] -> Unlock();                                     //バッファのアンロック

	return S_OK;
}

//ポリゴンの更新
void UpdateBGPolygon(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice = Get_Device();
	PLAYER *pPlayer = GetPlayer();
	VERTEX_2D *pVtx = NULL;                           //頂点構造体のポインタ
	static float fMoveCloud = 0;                                  //背景の空の移動増量

	//背景空の計算
	g_pVtxBufferBGPolygon[0]->Lock(0, 0, (void**)&pVtx, 0);       //仮想アドレスを取得しバッファをロックする

	MakeVertexPolygonBG(pD3DDevice, pVtx);

	pVtx[0].pos = D3DXVECTOR3(-pPlayer->fPosX_SUM, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(-pPlayer->fPosX_SUM+BG_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-pPlayer->fPosX_SUM, BG_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(-pPlayer->fPosX_SUM+BG_WIDTH, BG_HEIGHT, 0.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(2.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(2.0f,1.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[1].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[2].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[3].color = D3DCOLOR_RGBA(255,255,255,255);

	pVtx += 4;

	pVtx[0].pos = D3DXVECTOR3(-pPlayer->fPosX_SUM, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(-pPlayer->fPosX_SUM+BG_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-pPlayer->fPosX_SUM, BG_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(-pPlayer->fPosX_SUM+BG_WIDTH, BG_HEIGHT, 0.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(2.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(2.0f,1.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[1].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[2].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[3].color = D3DCOLOR_RGBA(255,255,255,255);

	pVtx += 4;
	
	pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[1].pos = D3DXVECTOR3(BG_WIDTH,0.0f,0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, BG_HEIGHT,0.0f);
	pVtx[3].pos = D3DXVECTOR3(BG_WIDTH,BG_HEIGHT,0.0f);

	pVtx[0].tex = D3DXVECTOR2(fMoveCloud,0.0f);
	pVtx[1].tex = D3DXVECTOR2(fMoveCloud+2.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(fMoveCloud,1.0f);
	pVtx[3].tex = D3DXVECTOR2(fMoveCloud+2.0f,1.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[1].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[2].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[3].color = D3DCOLOR_RGBA(255,255,255,255);

	g_pVtxBufferBGPolygon[0]->Unlock();                                     //バッファのアンロックト

	fMoveCloud += 0.0005f;
	if(fMoveCloud >= 10000) fMoveCloud = 0;
}

////////////////////////////////////////////////
//ポリゴンの描画
////////////////////////////////////////////////
void DrawBGPolygon(void)
{
	static int nCnt_BGChange = 0;
	static int nCnt_FrameChange = 1;

	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	//背景：頂点フォーマット設定
	pD3DDevice -> SetStreamSource(0,g_pVtxBufferBGPolygon[0],0,sizeof(VERTEX_2D));                   //VRAMからGPUにデータをストリーム
	pD3DDevice -> SetFVF(FVF_VERTEX2D);
	pD3DDevice -> SetTexture(0,g_pTextureBGPolygon[2]);												 //テクスチャの設定
	pD3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,2*4,NUM_POLYGON);                                 //テクスチャメモリの確保

	pD3DDevice -> SetTexture(0,g_pTextureBGPolygon[nCnt_BGChange % 2]);												 //テクスチャの設定
	pD3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,nCnt_BGChange % 2 * 4,NUM_POLYGON);                              //テクスチャメモリの確保

	nCnt_FrameChange++;
	if(nCnt_FrameChange % 60 == 0) nCnt_BGChange++;                                                     //8フレームごとに背景画像を切り替え

}

void UninitBGPolygon(void)
{
		// テクスチャインターフェイスの片付け
		for (int nCnt = 0; nCnt < NUM_BGTEXTURE; nCnt++)
		{
			SAFE_RELEASE(g_pTextureBGPolygon[nCnt]);
		}
	
		//頂点バッファインターフェイスの片付け
		for (int nCnt = 0; nCnt < NUM_BGPOLYGON; nCnt++)
		{
			SAFE_RELEASE(g_pVtxBufferBGPolygon[nCnt]);
		}
}

//背景ポリゴンの表示位置X,ポリゴンの表示位置Y,ポリゴンの幅,ポリゴンの高さ
void MakeVertexPolygonBG(LPDIRECT3DDEVICE9 pDevice,VERTEX_2D *pVtx)
{
	pDevice -> SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);   //引数1:設定名カリング 引数2:設定値(D3DCULL_CCW:逆時計回りを裏面をする)
	pDevice -> SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	pDevice -> SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	pDevice -> SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[1].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[2].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[3].color = D3DCOLOR_RGBA(255,255,255,255);

	//ポリゴン描画する前にテクスチャをセット
	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);
}