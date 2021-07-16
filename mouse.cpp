//マウス処理のソースファイル

//ヘッダファイル読み込み
#include"game.h"
#include"mouse.h"
#include"FPS.h"

//マウス入力を取得
MOUSE mouse;

//関数

/// <summary>
/// マウスの入力情報を更新する
/// </summary>
/// <param name=""></param>
VOID MouseUpdete(VOID)
{
	//マウスの以前の情報を取得
	mouse.OldPoint = mouse.Point;

	//マウスの以前のボタン入力を取得
	mouse.OldButton[MOUSE_INPUT_LEFT] = mouse.Button[MOUSE_INPUT_LEFT];
	mouse.OldButton[MOUSE_INPUT_MIDDLE] = mouse.Button[MOUSE_INPUT_MIDDLE];
	mouse.OldButton[MOUSE_INPUT_RIGHT] = mouse.Button[MOUSE_INPUT_RIGHT];

	//マウスの座標を取得
	GetMousePoint(&mouse.Point.x, &mouse.Point.y);

	//マウスの座標が画面外の場合は画面内に収める
	if (mouse.Point.x < 0) { mouse.Point.x = 0; }
	if (mouse.Point.x > GAME_WIDTH) { mouse.Point.x = GAME_WIDTH; }
	if (mouse.Point.y < 0) { mouse.Point.y = 0; }
	if (mouse.Point.y > GAME_HEIGHT) { mouse.Point.x = GAME_HEIGHT; }
	
	//マウスの押しているボタンを取得
	mouse.InputValue = GetMouseInput();

	//左ボタンを押しているかチェック(TRUEは0以外なので、2に4もTRUE)
	if ((mouse.InputValue & MOUSE_INPUT_LEFT) == MOUSE_INPUT_LEFT) { mouse.Button[MOUSE_INPUT_LEFT]++; }	//押している
	if ((mouse.InputValue & MOUSE_INPUT_LEFT) != MOUSE_INPUT_LEFT) { mouse.Button[MOUSE_INPUT_LEFT]=0; }	//押していない

	//中ボタンを押しているかチェック(TRUEは0以外なので、2に4もTRUE)
	if ((mouse.InputValue & MOUSE_INPUT_MIDDLE) == MOUSE_INPUT_MIDDLE) { mouse.Button[MOUSE_INPUT_MIDDLE]++; }		//押している
	if ((mouse.InputValue & MOUSE_INPUT_MIDDLE) != MOUSE_INPUT_MIDDLE) { mouse.Button[MOUSE_INPUT_MIDDLE] = 0; }	//押していない

	//右ボタンを押しているかチェック(TRUEは0以外なので、2に4もTRUE)
	if ((mouse.InputValue & MOUSE_INPUT_RIGHT) == MOUSE_INPUT_RIGHT) { mouse.Button[MOUSE_INPUT_RIGHT]++; }		//押している
	if ((mouse.InputValue & MOUSE_INPUT_RIGHT) != MOUSE_INPUT_RIGHT) { mouse.Button[MOUSE_INPUT_RIGHT] = 0; }	//押していない
	
	//ホイールの回転量を取得
	mouse.WheelValu = GetMouseWheelRotVol();

	return;
}

/// <summary>
/// ボタンを押しているか、マウスコードで判断する
/// </summary>
/// <param name="MOUSE_IMPUT_">MOUSE_IMPUT_</param>
/// <returns>ボタンを押しているならTRUE</returns>
BOOL MouseDown(int MOUSE_INPUT_)
{
	//マウスコードのボタンを押している時
	if (mouse.Button[MOUSE_INPUT_] != 0)
	{
		return TRUE;	//ボタンを押している
	}
	else
	{
		return FALSE;	//ボタンを押していない
	}
}


/// <summary>
/// ボタンを押し上げたか、マウスコードで判断する
/// </summary>
/// <param name="MOUSE_INPUT_">MOUSE_INPUT_</param>
/// <returns>ボタンを押しているならTRUE</returns>
BOOL MouseUp(int MOUSE_INPUT_)
{
	if (mouse.OldButton[MOUSE_INPUT_] >=1			//直前は押していて
			&& mouse.Button[MOUSE_INPUT_] == 0)		//今は押していないとき
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

/// <summary>
/// ボタンを押し上げたか、マウスコードで判断する
/// </summary>
/// <param name="MOUSE_INPUT_">MOUSE_INPUT_</param>
/// <param name="millitime">ボタンを押し続けている時間(ミリ秒)</param>
/// <returns>押し続けていたらTRUE</returns>
BOOL MouseDownKeep(int MOUSE_INPUT_, int millitime)
{
	float MilliSec = 1000.0f;	//1秒は1000ミリ秒

	//押し続ける時間 ＝ 秒数 × FPS値
	//例）60FPSのゲームで、1秒間押し続けるなら、1秒×60FPS

	int UpdateTime = (millitime / MilliSec) * fps.Value;

	if (mouse.Button[MOUSE_INPUT_] > UpdateTime)
	{
		return TRUE;	//押し続けている
	}
	else
	{
		return FALSE;	//押し続けていない
	}

}


/// <summary>
/// マウスをクリックしたか、マウスコードで判断する
/// </summary>
/// <param name="MOUSE_INPUT">MOUSE_INPUT</param>
/// <returns>クリックしたらTRUE</returns>
BOOL MouseClick(int MOUSE_INPUT_)
{
	if (mouse.OldButton[MOUSE_INPUT_] >= 1			//直前は押していて
			&& mouse.Button[MOUSE_INPUT_] == 0)		//今は押していないとき
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/// <summary>
/// マウスの情報を描画する
/// </summary>
/// <param name=""></param)
VOID MouseDraw(VOID)
{
	if (GAME_DEBUG == TRUE)	//デバッグモードなら
	{

		//マウスの座標を描画
		DrawFormatString(0, GAME_HEIGHT - 40, GetColor(255, 255, 255), "MOUSE[X:%4d/Y:%4d]", mouse.Point.x, mouse.Point.y);

	}

	return;
}

/// <summary>
/// マウスが矩形領域をクリックしたか？
/// </summary>
/// <param name="rect">矩形領域</param>
/// <param name="MOUSE_INPUT_">マウスコード</param>
/// <returns></returns>
BOOL MouseRectClick(RECT rect, int MOUSE_INPUT_)
{
	//点と四角の当たり判定
	if(CheckColliPointToRect(mouse.Point, rect)==TRUE)
	{
		//マウスをクリックしているか？
		return MouseClick(MOUSE_INPUT_);
	}
}

/// <summary>
/// マウスが円をクリックしたか？
/// </summary>
/// <param name="circle">円領域</param>
/// <param name="MOUSE_INPUT_">マウスコード</param>
/// <returns></returns>
BOOL MouseMaruClick(MARU circle, int MOUSE_INPUT_)
{
	//点と円の当たり判定
	if (CheckColliPointToMaru(mouse.Point,circle)==TRUE)
	{
		//マウスをクリックしているか
		return MouseClick(MOUSE_INPUT_);
	}
}

