//FPSのソースファイル

//ヘッダファイル読み込み
#include"DxLib.h"
#include"FPS.h"

//グローバル変数
FPS fps;	//FPSの取得

//関数
 

/// <summary>
/// FPS値を計算し、値を更新する
/// </summary>
/// <param name=""></param>
VOID FPSUpdate(VOID)
{
	

	/// 1番最初の処理

	if (FALSE == fps.IsInitFlg )
	{

		//測定開始時刻をマイクロ秒単位で取得
		fps.StartTime = GetNowHiPerformanceCount();		//Windowsが起動してから経過した時間(マイクロ秒)

		fps.IsInitFlg = TRUE;	//フラグを立てる
	}

	//現在の時刻をマイクロ秒単位で取得
	fps.NowTime = GetNowHiPerformanceCount();

	//前回取得した時間から経過時間を秒（小数）に変換してからセット
	fps.DeltaTime = (fps.NowTime - fps.OldTime) / 1000000.0f;

	//今回取得した時間を保存

	fps.OldTime = fps.NowTime;

	//1フレーム目～FPS設定値までは、カウントアップ
	if (fps.Count < fps.SampleRate)
	{
		//カウンタを増やす
		fps.Count++;

	}
	else
	{
		//FPS設定値のフレームで、平均FPSをけいさん
		//現在の時刻から、０フレーム目の時間を引き、FPSの数値で割る
		//現在の平均FPS値が出る

		fps.DrawValue = 1000000.f / ((fps.NowTime - fps.StartTime) / (float)fps.SampleRate);


		//測定開始時刻をマイクロ秒単位で取得
		fps.StartTime = GetNowHiPerformanceCount();		//Windowsが起動してから経過した時間(マイクロ秒)

		//カウンタを初期化
		fps.Count = 1;

	}	
	
	return;
}


/// <summary>
/// 平均FPS値を描画する
/// </summary>
/// <param name=""></param>
VOID FPSDraw(VOID)
{
	if (GAME_DEBUG == TRUE)
	{
		//文字列を描画
		DrawFormatString(0, GAME_HEIGHT-20, GetColor(0, 0, 0), "FPS:%.1f", fps.DrawValue);

	}
	

	return;
}


/// <summary>
/// FPSで処理を待つ
/// </summary>
/// <param name=""></param>
VOID FPSWait(VOID) 
{
	//現在の時刻　-　最初の時刻で、現在かかっている時刻を取得する
	LONGLONG resultTime = fps.NowTime - fps.StartTime;

	//待つべきミリ秒数(1秒/FPS値 * 現在のフレーム数)から現在かかっている時刻を引く
	LONGLONG waitTime = 1000000.0f / fps.Value * fps.Count - resultTime;

	//マイクロ秒からミリ秒に変換
	waitTime /= 1000.0f;

	//処理が早かったら、処理を待つ
	if (waitTime > 0)
	{

		WaitTimer(waitTime);	//引数ミリ秒待つ

		//Sleep(1000);

	}

	//垂直同期をOFFにしているか？
	if (GetWaitVSyncFlag() == FALSE)
	{
		//FPS最大値でないとき
		if (fps.Value < GAME_FAS_MAX)
		{
			//1秒毎のFPS値よりも、待つ時間が小さい時は、もっとFPS値を上げても良い
			//待つ時間 20ミリ <= 1秒60FPS =16.666ミリ	もう少し早くできる
			if (waitTime>0 
					&& waitTime<= 1000.0f/fps.Value)
			{
				fps.Value++;
			}
			else
			{
				//FPS値が追いついておらず、遅い時はFPS値を下げる
				if (fps.Value>GAME_FPS_MIN)
				{
					fps.Value--;
				}
			}
		}

	}
	return;
}
