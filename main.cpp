#define _CRT_SECURE_NO_WARNINGS

//ヘッダファイルの読み込み
#include "DxLib.h"		//DxLibを使うときは必要
#include "keyboard.h"	//キーボードの処理
#include "FPS.h"		//FPSの処理

#include <math.h>		//数学

//マクロ定義
#define TAMA_DIV_MAX 4	//弾の画像の最大数
#define TAMA_MAX 20		//弾の総数



//構造体の定義

//画像構造体
struct IMAGE
{
	int handle = -1;	//画像のハンドル(管理番号)
	char path[255];		//画像の場所(パス)

	int x;				//X位置
	int y;				//Y位置
	int width;			//幅
	int height;			//高さ

	BOOL IsDraw = FALSE;	//画像が描画できる

};

//ギャラクタ構造体
struct CHARACTOR
{
	IMAGE img;		//画像構造体
	int speed;		//速度移動
	RECT coll;		//当たり判定の領域(四角)

};

//動画の構造体
struct MOVIE
{
	int handle = -1;	//動画のハンドル
	char path[255];		//動画のパス

	int x;				//x位置
	int y;				//y位置
	int width;			//幅
	int height;			//高さ

	int Volume = 255;	//ボリューム（最小）0〜255(最大)
};

//音楽の構造体

struct AUDIO
{
	int	handle = -1;	//音楽のハンドル
	char path[255];		//音楽のパス

	int Volume = -1;	//ボリューム(MIN 0 〜 255 MAX)
	int playType = -1;	//BGM or SE

};

//弾の構造体

struct TAMA
{
	int handle[TAMA_DIV_MAX];		//画像のハンドル
	char path[255];		//画像のパス

	int DivTate;		//分割数（縦）
	int DivYoko;		//分割数（横）
	int DivMAX;			//分割総数

	int AnimeCnt = 0;		//アニメーションカウンタ
	int AnimeCntMAX = 0;	//アニメーションカウンタMAX

	int NowIndex = 0;	//現在の画像の要素数

	int	StartX;			//最初のX位置
	int	StartY;			//最初のY位置

	float radius;		//半径
	float degree;		//角度

	int x;				//X位置
	int y;				//Y位置
	int width;			//幅
	int height;			//高さ

	int speed;			//速度

	RECT coll;			//当たり判定

	BOOL IsDraw = FALSE;	//画像が描画できる

};

//グローバル変数
//シーンを管理する変数

GAME_SCENE GameScene;		//現在のゲームのシーン
GAME_SCENE OldGameSnece;	//前回のゲームのシーン
GAME_SCENE NextGameSnece;	//次のゲームのシーン

//画面の切り替え
BOOL IsFadeOut = FALSE;		//フェードアウト
BOOL IsFadeIn = FALSE;		//フィードイン

int fadeTimeMill = 2000;	//切り替えミリ秒
int fadeTimeMax = fadeTimeMill / 1000 * 60;	//ミリ秒をフレーム秒に変換

//フェードアウト
int fadeOutCntInit = 0;		//初期値
int fadeOutCnt = fadeOutCntInit;	//フェードアウトのカウンタ
int fadeOutCntMax = fadeTimeMax;	//フェードアウトのカウンタMAX

//フェードイン
int fadeInCntInit = fadeTimeMax;	//初期値
int fadeInCnt = fadeInCntInit;		//フェードアウトのカウンタ
int fadeInCntMax = fadeTimeMax;		//フェードアウトのカウンタMAX

//弾の構造体
struct TAMA tama_moto;		//元
struct TAMA tama[TAMA_MAX];	//実際に使う

//弾の発射カウンタ
int tamashotCnt = 0;
int tamashotCntMAX = 5;

//プレイヤー
CHARACTOR player;

//背景画像
IMAGE back[2];	//背景の画像は2枚



//プロトタイプ宣言
VOID Title(VOID);		//タイトル画面
VOID TitleProc(VOID);	//タイトル画面(処理)
VOID TitleDraw(VOID);	//タイトル画面(描画)

VOID Play(VOID);		//プレイ画面
VOID PlayProc(VOID);	//プレイ画面(処理)
VOID PlayDraw(VOID);	//プレイ画面(描画)

VOID End(VOID);			//エンド画面
VOID EndProc(VOID);		//エンド画面(処理)
VOID EndDraw(VOID);		//エンド画面(描画)

VOID Change(VOID);		//切り替え画面
VOID ChangeProc(VOID);	//切り替え画面(処理)
VOID ChangeDraw(VOID);	//切り替え画面(描画)

VOID ChangeScene(GAME_SCENE scene);	//シーン切り替え

VOID CollUpdatepPayer(CHARACTOR* chara);	//当たり判定の領域を更新
VOID CollUpdate(CHARACTOR* chara);			//ゴールの当たり判定の領域更新
VOID CollUpdateTama(TAMA* tama);			//弾の当たり判定の領域更新
BOOL OnCollRect(RECT a, RECT b);				//矩形と矩形の当たり判定

BOOL GameLoad(VOID);	//ゲームのデータを読み込み

BOOL LoadImageMem(IMAGE* image, const char* path);	//ゲームの画像を読み込み
BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playType);	//音楽の読み込み
BOOL LoadimageDivMem(int* handle, const char* path, int DivWidth, int DivHeight);	//ゲームの画像の分割の読み込み
VOID DrawTama(TAMA* tama);					//弾の描画
VOID ShotTama(TAMA* tama, float deg);		//弾を飛ばす


VOID GameInit(VOID);	//ゲームのデータの初期化



//プログラムは WinMain から始まります
//Windowsのプログラム方法＝(WinAPI)で動いている！
//DXlibは、DirectXという、ゲームプログラミングをかんたんに使える仕組み

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine
	, int nCmdShodw)
{
	SetOutApplicationLogValidFlag(FALSE);	//Log.txtを出力しない
	ChangeWindowMode(TRUE);				//ウィンドウモードに設定(FALSEの場合は全画面モードになる。)
	SetMainWindowText(GAME_TITLE);			//ウィンドウのタイトルの文字
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//ウィンドウの解像度を設定
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);				//ウィンドウの大きさを設定
	SetBackgroundColor(255, 255, 255);					//デフォルトの背景の色
	SetWindowIconID(GAME_ICON_ID);						//ゲームアイコンを表示
	SetWindowStyleMode(GAME_WINDOW_BAR);				//ウィンドウバーの状態の設定
	SetWaitVSyncFlag(TRUE);								//ウィンドウの垂直同期を有効にする
	SetAlwaysRunFlag(TRUE);								//ウィンドウをずっとアクティブにする（別のウィンドウを開いてもゲームが非アクティブになることはない）

	//上の処理はDXライブラリ初期化前に設定。

	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)
	{
		return -1;			// エラーが起きたら直ちに終了
	}


	//ダブルバッファリング有効化
	SetDrawScreen(DX_SCREEN_BACK);

	//最初のシーンはタイトル画面から
	GameScene = GAME_SCENE_TITLE;

	//ゲーム読み込み
	if (!GameLoad())
	{

		//データの読み込みの失敗したとき
		DxLib_End();		//Dxlib終了	

		return -1;			//異常終了
	}


	//ゲーム全体の初期化

	//ゲームの初期化
	GameInit();

	//無限ループ
	while (1)
	{
		if (ProcessMessage() != 0) { break; }	//無限ループを抜ける。エラーやウィンドウが閉じられた
		if (ClearDrawScreen() != 0) { break; }		//画面を消去する

		//キーボード入力の更新
		AllKeyUpdate();

		//ESCキーで強制終了
		if (KeyClick(KEY_INPUT_ESCAPE) == TRUE) { break; }

		//FPS値の更新
		FPSUpdate();

		//以前のシーンを取得
		if (GameScene != GAME_SCENE_CHANGE)
		{
			OldGameSnece = GameScene;
		}

		//シーンごとに処理を行う
		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			Title();			//タイトル画面
			break;
		case GAME_SCENE_PLAY:
			Play();				//プレイ画面
			break;
		case GAME_SCENE_END:
			End();				//エンド画面
			break;
		case GAME_SCENE_CHANGE:
			Change();			//切り替え画面
			break;
		}

		//シーンを切り替える
		if (OldGameSnece != GameScene)
		{
			//現在のシーンが切り替え画面でないとき
			if (GameScene != GAME_SCENE_CHANGE)
			{
				NextGameSnece = GameScene;		//次のシーンを保存
				GameScene = GAME_SCENE_CHANGE;	//画面切り替えシーンに変える
			}
		}


		//FPS値を描画
		FPSDraw();

		//FPS値を待つ

		FPSWait();

		ScreenFlip();		//ダブルバッファリングした画面を描画
	}

	//読み込んだ画像を解放
	for (int i = 0; i < TAMA_DIV_MAX; i++)
	{
		DeleteGraph(tama_moto.handle[i]);
	}

	// ＤＸライブラリ使用の終了処理
	DxLib_End();

	//プレイヤー解放
	DeleteGraph(player.img.handle);

	//背景画像解放
	DeleteGraph(back[0].handle);
	DeleteGraph(back[1].handle);

	return 0;	// ソフトの終了 
}

/// <summary>
/// ゲームのデータを読み込み
/// </summary>
/// <param name=""></param>
/// <returns>読み込めたらTRUE/読み込めなかったらFALSE</returns>
BOOL GameLoad(VOID)
{
	//弾の分割数を設定
	tama_moto.DivYoko = 4;
	tama_moto.DivTate = 1;

	//弾のパス
	strcpy(tama_moto.path, ".\\Image\\dia_orange.png");

	//画像を分割して読み込み
	if (LoadimageDivMem(&tama_moto.handle[0], tama_moto.path, tama_moto.DivYoko, tama_moto.DivTate) == FALSE) { return FALSE; }

	//幅と高さを取得
	GetGraphSize(tama_moto.handle[0], &tama_moto.width, &tama_moto.height);

	//位置を設定
	tama_moto.x = GAME_WIDTH / 2 - tama_moto.width / 2;	//中央揃え
	tama_moto.y = GAME_HEIGHT - tama_moto.height;		//画面下

	//速度
	tama_moto.speed = 10 ;

	//アニメを変える速度
	tama_moto.AnimeCntMAX = 10;

	//当たり判定の更新
	CollUpdateTama(&tama_moto);

	//画像を表示しない
	tama_moto.IsDraw = FALSE;
	
	//全ての弾に情報をコピー
	for (int i = 0; i < TAMA_MAX; i++)
	{
		tama[i] = tama_moto;
	}

	//プレイヤーの画像を読み込み
	if (LoadImageMem(&player.img, ".\\Image\\player.png") == FALSE) { return FALSE; }
	player.img.x = GAME_WIDTH / 2 - player.img.width;
	player.img.y = GAME_HEIGHT / 2 - player.img.height;
	CollUpdatepPayer(&player);		//当たり判定の更新
	player.img.IsDraw = TRUE;		//描画する

	//背景の画像を読み込み�@
	if (LoadImageMem(&back[0], ".\\Image\\hosi.png") == FALSE) { return FALSE; }
	back[0].x=0;
	back[0].y=-back[0].height;	//画像の高さ、位置にあげる
	back[0].IsDraw = TRUE;		//描画する
	
	//背景の画像を読み込み�A
	if (LoadImageMem(&back[1], ".\\Image\\hosi_rev.png") == FALSE) { return FALSE; }
	back[1].x = 0;
	back[1].y =0;	//画像の高さ、位置にあげる
	back[1].IsDraw = TRUE;		//描画する

	


	return TRUE;	//全て読み込めた！
}

/// <summary>
/// 画像を分割してメモリに読み込み
/// </summary>
/// <param name="handle">ハンドル配列の先頭アドレス</param>
/// <param name="path">画像のパス</param>
/// <param name="DivWidth">分割するときの横の数</param>
/// <param name="Diveight">分割するときの縦の数</param>
/// <returns></returns>
BOOL LoadimageDivMem(int* handle, const char* path, int DivWidth, int DivHeight)
{


	//弾の読み込み
	int IsTamaLoad = -1;	//画像が読み込めたか？

	//一時的に画像のハンドルを用意する
	int TamaHandle = LoadGraph(path);

	//読み込みエラー
	if (TamaHandle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//ウィンドウハンドル
			path,				//本文
			"画像読み込みエラー",	//タイトル
			MB_OK					//ボタン
		);

		return FALSE;	//読み込み失敗
	}


	//画像と幅と高さを取得
	int TamaWidth = -1;		//幅
	int TamaHeight = -1;	//高さ
	GetGraphSize(TamaHandle, &TamaWidth, &TamaHeight);

	//分割して読み込み
	IsTamaLoad = LoadDivGraph(
		path,											//画像のパス
		TAMA_DIV_MAX,									//分割総数
		DivWidth, DivHeight,								//横の分割,縦の分割
		TamaWidth / DivWidth, TamaHeight / DivHeight,	//画像一つ分の幅，高さ
		handle											//連続で管理する配列の先頭アドレス
	);

	//分割エラー
	if (IsTamaLoad == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//ウィンドウハンドル
			path,					//本文
			"画像分割エラー",	//タイトル
			MB_OK					//ボタン
		);

		return FALSE;	//読み込み失敗
	}

	//一時的に読み込んだハンドルを解放
	DeleteGraph(TamaHandle);

	return TRUE;
}

// <summary>
/// ゲームデータを初期化
/// </summary>
/// <param name=""></param>
VOID GameInit(VOID)
{
	//プレイヤーの初期化
	player.img.x = GAME_WIDTH / 2 - player.img.width;
	player.img.y = GAME_HEIGHT / 2 - player.img.height;
	CollUpdatepPayer(&player);		//当たり判定の更新
	player.img.IsDraw = TRUE;		//描画する

	player.speed = 5;

	//背景の画像を設定�@
	back[0].x = 0;
	back[0].y = -back[0].height;	//画像の高さ、位置にあげる
	back[0].IsDraw = TRUE;		//描画する

	//背景の画像を設定�A
	back[1].x = 0;
	back[1].y = 0;	//画像の高さ、位置にあげる
	back[1].IsDraw = TRUE;		//描画する



}

/// <summary>
/// 画像をメモリに読み込み
/// </summary>
/// <param name="image">画像構造体のアドレス</param>
/// <param name="path">画像のパス</param>
/// <returns></returns>
BOOL LoadImageMem(IMAGE* image, const char* path)
{

	//画像を読み込み
	strcpyDx(image->path, path);					//パスのコピー
	image->handle = LoadGraph(image->path);			//画像の読み込み


	//画像が読み込めなかったときはエラー(-1)が入る
	if (image->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//メインのウィンドウハンドル
			image->path,//メッセージ本文
			"画像読み込みエラー!",	//メッセージタイトル
			MB_OK					//ボタン
		);

		DxLib_End();	//強制終了
		return -1;	//エラー終了

		return FALSE; //読み込み失敗
	}

	//画像の幅と高さを取得
	GetGraphSize(image->handle, &image->width, &image->height);

	//読み込めた
	return TRUE;
}

/// <summary>
/// 音楽をメモリに読み込み
/// </summary>
/// <param name="audio">Audio構造体変数のアドレス</param>
/// <param name="path">Audioの音楽パス</param>
/// <param name="volume">ボリューム</param>
/// <param name="playType">DX_PLAYTYPE_LOOP or DX_PLAYTYPE_BACK or DX_PLAYTYPE_NORMAL</param>
/// <returns></returns>
BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playType)
{

	//音楽の読み込み
	strcatDx(audio->path, path);				//パスのコピー
	audio->handle = LoadSoundMem(audio->path);	//音楽の読み込み

	//音楽が読み込めなったときは、エラー(-1)がはいる

	if (audio->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//メインのウィンドウハンドル
			audio->path,			//メッセージ本文
			"音楽読み込みエラー！",	//メッセージタイトル
			MB_OK					//ボタン
		);

		return FALSE;	//読み込み失敗
	}

	//その他設定
	audio->Volume = volume;
	audio->playType = playType;


	return TRUE;
}


//シーンを切り替える関数
VOID ChangeScene(GAME_SCENE scene)
{
	GameScene = scene;	//プレイ画面に切り替え
	IsFadeIn = FALSE;				//フェードインしない
	IsFadeOut = TRUE;					//フィードアウトする
}

//タイトル画面
VOID Title(VOID)
{

	TitleProc();	//処理
	TitleDraw();	//描画

	return;
}

//タイトル画面の処理
VOID TitleProc(VOID)
{
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//プレイ画面に切り替え
		ChangeScene(GAME_SCENE_PLAY);
	}


	return;
}

//タイトル画面の描画
VOID TitleDraw(VOID)
{

	DrawString(0, 0, "タイトル画面", GetColor(0, 0, 0));

	return;
}

/// <summary>
/// 弾の描画
/// </summary>
/// <param name="tama">弾の構造体</param>
VOID DrawTama(TAMA* tama)
{
	//弾の描画ができるとき
	if (tama->IsDraw == TRUE)
	{
		//弾の描画
		DrawGraph(tama->x, tama->y, tama->handle[tama->NowIndex], TRUE);

		if (tama->AnimeCnt < tama->AnimeCntMAX)
		{

			tama->AnimeCnt++;

		}
		else
		{
			//弾の添字が弾の分割数の最大よりも下のとき
			if (tama->NowIndex < TAMA_DIV_MAX - 1)
			{
				tama->NowIndex++;	//次の画像へ
			}
			else
			{
				tama->NowIndex = 0;	//最初に戻す
			}
			tama->AnimeCnt = 0;
		}
	}
}

//プレイ画面
VOID Play(VOID)
{

	PlayProc();	//処理
	PlayDraw();	//描画

	return;
}

//プレイ画面の処理
VOID PlayProc(VOID)
{
	//背景の描画
	for (int i = 0; i < 2; i++) 
	{
		//描画
		DrawGraph(back[i].x, back[i].y, back[i].handle, TRUE);

		//画像を下まで行ったとき
		if (back[i].y > GAME_HEIGHT)
		{
			back[i].y = -back[i].height+1;	//高さ分上に戻す
		}

		//画像を下に動かす
		back[i].y++;
	}

	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//エンド画面に切り替え
		ChangeScene(GAME_SCENE_END);
	}

	if (KeyDown(KEY_INPUT_LEFT) == TRUE)
	{
		if (player.img.x - player.speed>=0)
		{
			player.img.x -= player.speed;
		}

	}

	if (KeyDown(KEY_INPUT_RIGHT) == TRUE)
	{
		if (player.img.x + player.speed <= GAME_WIDTH)
		{
			player.img.x += player.speed;
		}

	}

	if (KeyDown(KEY_INPUT_UP) == TRUE)
	{
		if (player.img.y - player.speed >= 0)
		{
			player.img.y -= player.speed;
		}

	}
	
	if (KeyDown(KEY_INPUT_DOWN) == TRUE)
	{
		if (player.img.y + player.speed <= GAME_HEIGHT)
		{
			player.img.y += player.speed;
		}

	}

	//プレイヤーの当たり判定の更新
	CollUpdatepPayer(&player);


	//スペースキーを押しているとき
	if (KeyDown(KEY_INPUT_SPACE) == TRUE)
	{	
		if (tamashotCnt == 0)
		{
			//弾の発射する(弾の描画する)
			for (int i = 0; i < TAMA_MAX; i++)
			{
				if (tama[i].IsDraw == FALSE)
				{
					ShotTama(&tama[i], 300.0f);

					//弾を一発出したら、ループを抜ける
					break;
				}
			}

			//弾の発射する(弾の描画する)
			for (int i = 0; i < TAMA_MAX; i++)
			{
				if (tama[i].IsDraw == FALSE)
				{
					ShotTama(&tama[i], 240.0f);

					//弾を一発出したら、ループを抜ける
					break;
				}
			}

			//弾の発射する(弾の描画する)
			for (int i = 0; i < TAMA_MAX; i++)
			{
				if (tama[i].IsDraw == FALSE)
				{
					ShotTama(&tama[i], 270.0f);

					//弾を一発出したら、ループを抜ける
					break;
				}
			}
		}

		//弾の発射待ち
		if (tamashotCnt<tamashotCntMAX)
		{
			tamashotCnt++;
		}
		else
		{
			tamashotCnt = 0;
		}

	}

		//弾を飛ばす
		for (int i = 0; i < TAMA_MAX; i++)
		{
			
		
			//描画されているとき
			if (tama[i].IsDraw == TRUE)
			{

				//弾の位置を修正
				//tama[i].x;
				tama[i].x =  tama[i].StartX+cos(tama[i].degree*DX_PI/180.0f)*tama[i].radius;
				tama[i].y =  tama[i].StartY+sin(tama[i].degree*DX_PI/180.0f)*tama[i].radius;

				//半径を足す
				tama[i].radius += tama[i].speed;

				CollUpdateTama(&tama[i]);

				//画面外に出たら、描画しない
				if (tama[i].y + tama[i].height<0 || //画面外(上)
					tama[i].y>GAME_HEIGHT		 || //画面外(下)
					tama[i].x+tama[i].width<0	 || //画面外(左)
					tama[i].x>GAME_WIDTH			 //画面外(右)
					)
				{
					tama[i].IsDraw = FALSE;

				}
			}
		}
	


	return;
}

/// <summary>
/// 弾を飛ばす
/// </summary>
/// <param name="tama"></param>
VOID ShotTama(TAMA* tama, float deg)
{
	
		//弾を発射する
		tama->IsDraw = TRUE;

		//弾の位置を決める
		tama->StartX = player.img.x + player.img.width / 2 - tama->width / 2;
		tama->StartY = player.img.y;

		//弾の速度を変える
		tama->speed = 6;

		//弾の角度
		tama->degree = deg;

		//弾の半径
		tama->radius = 0.0f;

		//弾の当たり判定の更新
		CollUpdateTama(tama);
	
}

//プレイ画面の描画
VOID PlayDraw(VOID)
{
	//プレイヤーの描画
	if (player.img.IsDraw == TRUE)
	{

		//プレイヤーの描画
		DrawGraph(player.img.x, player.img.y, player.img.handle, TRUE);

		//当たり判定の描画
		if (GAME_DEBUG == TRUE)
		{
			DrawBox(
				player.coll.left, player.coll.top, player.coll.right, player.coll.bottom,
				GetColor(255, 0, 0), FALSE
				);

		}

	}

	//弾の描画
	for (int i = 0; i < TAMA_MAX; i++)
	{
		//描画されているとき
		if (tama[i].IsDraw==TRUE)
		{
			DrawTama(&tama[i]);

			//当たり判定の描画
				if (GAME_DEBUG == TRUE)
				{
					DrawBox(
						tama[i].coll.left,tama[i].coll.top,tama[i].coll.right,tama[i].coll.bottom,
						GetColor(255, 0, 0), FALSE
					);

				}
		}
	}

	DrawString(0, 0, "プレイ画面", GetColor(0, 0, 0));

	return;
}

//エンド画面
VOID End(VOID)
{

	EndProc();	//処理
	EndDraw();	//描画

	return;
}

//エンド画面の処理
VOID EndProc(VOID)
{

	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//タイトル画面に切り替え
		ChangeScene(GAME_SCENE_TITLE);
	}


	return;
}

//エンド画面の描画
VOID EndDraw(VOID)
{

	DrawString(0, 0, "エンド画面", GetColor(0, 0, 0));

	return;
}

//切り替え画面
VOID Change(VOID)
{

	ChangeProc();	//処理
	ChangeDraw();	//描画

	return;
}

//切り替え画面の処理
VOID ChangeProc(VOID)
{
	//フェードイン
	if (IsFadeIn == TRUE)
	{
		if (fadeInCnt > fadeInCntMax)
		{
			fadeInCnt--;	//カウンタを減らす
		}
		else
		{
			//フェードイン処理が終わった

			fadeInCnt = fadeInCntInit;	//カウンタ初期化
			IsFadeIn = FALSE;			//フェードイン処理終了
		}
	}

	//フェードアウト
	if (IsFadeOut == TRUE)
	{
		if (fadeOutCnt < fadeOutCntMax)
		{
			fadeOutCnt++;	//カウンタを増やす
		}
		else
		{
			//フェードアウト処理が終わった

			fadeOutCnt = fadeOutCntInit;	//カウンタ初期化
			IsFadeOut = FALSE;			//フェードアウト処理終了
		}
	}

	//切り替え処理終了
	if (IsFadeIn == FALSE && IsFadeOut == FALSE)
	{
		//フェードインしていない、フェードアウトもしていないとき
		GameScene = NextGameSnece;	//次のシーンに切り替え
		OldGameSnece = GameScene;	//以前のゲームシーンに更新
	}


	return;
}


//切り替え画面の描画
VOID ChangeDraw(VOID)
{
	//以前のシーンを描画
	switch (OldGameSnece)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();	//タイトル画面の描画
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();		//プレイ画面の描画
		break;
	case GAME_SCENE_END:
		EndDraw();		//エンド画面の描画
		break;

	default:
		break;
	}

	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 127);		//0〜255
	//DrawBox(0, 0, GAME_WIDHT, GAME_WIDTH, GetColor(0, 0, 0), TRUE);
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//フェードイン
	if (IsFadeIn == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeInCnt / fadeInCntMax) * 255);
	}

	//フェードアウト
	if (IsFadeOut == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeOutCnt / fadeOutCntMax) * 255);
	}


	//四角を描画
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0), TRUE);

	//半透明終了
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(0, 0, "切り替え画面", GetColor(0, 0, 0));

	return;
}

/// <summary>
/// 当たり判定の領域更新 (プレイヤー)
/// </summary>
/// <param name="Coll">当たり判定の領域</param>
VOID CollUpdatepPayer(CHARACTOR* chara)
{
	chara->coll.left = chara->img.x + 10;
	chara->coll.top = chara->img.y + 10;
	chara->coll.right = chara->img.x + chara->img.width - 10;
	chara->coll.bottom = chara->img.y + chara->img.height - 10;

	return;
}


/// <summarimg.y>
/// 当たり判定の領域更新
/// </summarimg.y>
/// <param name="Coll">当たり判定の領域</param>
VOID CollUpdate(CHARACTOR* chara)
{
	chara->coll.left = chara->img.x;
	chara->coll.top = chara->img.y;

	chara->coll.right = chara->img.x + chara->img.width;
	chara->coll.bottom = chara->img.y + chara->img.height;

	return;
}

/// <summarimg.y>
/// 当たり判定の領域更新
/// </summarimg.y>
/// <param name="Coll">当たり判定の領域</param>
VOID CollUpdateTama(TAMA* tama)
{
	tama->coll.left = tama->x;
	tama->coll.top = tama->y;

	tama->coll.right = tama->x + tama->width;
	tama->coll.bottom = tama->y + tama->height;

	return;
}

/// <summary>
///　矩形と矩形の当たり判定
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <returns></returns>
BOOL OnCollRect(RECT a, RECT b)
{

	if (
		a.left < b.right &&		// 矩形Aの左辺X座標 < 矩形Bの右辺X座標　かつ
		a.right > b.left &&		// 矩形Aの右辺X座標 > 矩形Bの左辺X座標　かつ
		a.top < b.bottom &&		// 矩形Aの上辺Y座標 < 矩形Bの下辺Y座標	かつ
		a.bottom > b.top		// 矩形Aの下辺Y座標 > 矩形Bの上辺Y座標
		)
	{
		//当たってるとき
		return true;
	}
	else
	{
		//当たっていないとき
		return false;
	}



}