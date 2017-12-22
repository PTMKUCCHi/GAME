/*******************************************************************

	ブロック崩し プログラム ヒント　　　block0.cpp
	
	松江工業高等専門学校　情報工学科　准教授　橋本　剛

*******************************************************************/
#include "DxLib.h"

// 定数 const を付けると定数 
const int WINDOW_SIZE_X = 640;
const int WINDOW_SIZE_Y = 480;
const int BAR_SIZE_X = 200;
const int BAR_SIZE_Y = 5;
const int BALL_SIZE = 4;
const int BLOCK_SIZE_X = 40;
const int BLOCK_SIZE_Y = 20;
const int BLOCK_NUM_X = WINDOW_SIZE_X / BLOCK_SIZE_X;
const int BLOCK_NUM_Y = 6;
const int BLOCK_TOP_Y = 40;

// グローバル変数
int bar_y = WINDOW_SIZE_Y * 9 / 10;//bar上端のy座標
int bar_x = WINDOW_SIZE_X / 2 - BAR_SIZE_X / 2;//bar左端のx座標
int ball_x = bar_x + BAR_SIZE_X / 2;//ball中心のx座標
int ball_y = bar_y - BALL_SIZE;//ball中心のy座標
int block[BLOCK_NUM_Y][BLOCK_NUM_X] = 
{
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,},
};
int BlockHandle[BLOCK_NUM_Y];//ブロックのビットマップ画像ハンドル

//描画関数
void Draw()
{
	static int GrHandle = LoadGraph( "back.bmp" );//背景画像登録 640x480
	DrawGraph( 0 , 0 , GrHandle , FALSE );//背景を描く
	DrawBox( bar_x, bar_y, bar_x+BAR_SIZE_X, bar_y+BAR_SIZE_Y, GetColor( 255, 255, 255 ), TRUE);//BARを描く
	DrawCircle( ball_x, ball_y, BALL_SIZE, GetColor( 100, 100, 100 ), TRUE);//BALLを描く
	for ( int y = 0; y < BLOCK_NUM_Y; y++ )
	{
		for ( int x = 0; x < BLOCK_NUM_X; x++ )
		{
			if( block[y][x] )
				DrawBox( x * BLOCK_SIZE_X, BLOCK_TOP_Y + y * BLOCK_SIZE_Y, ( x + 1 ) * BLOCK_SIZE_X - 1, BLOCK_TOP_Y + ( y + 1 ) * BLOCK_SIZE_Y - 1, GetColor( 255, 0, 0 ), TRUE);//BLOCKを描く
		}
	}
}

//Barの座標を変える関数
void MoveBar()
{
	if ( CheckHitKey( KEY_INPUT_RIGHT ) ) // → キー を押したか
	{
		bar_x+=2;
	}
	
	if ( CheckHitKey( KEY_INPUT_LEFT ) ) // → キー を押したか
	{
		bar_x-=2;
	}

}

//ボールの座標を変える関数 反射を考える
void MoveBall()
{
	static int vx = 1;//ballのx方向速度
	static int vy = 1;//ballのy方向速度
	int ballx1 = ball_x - BALL_SIZE;//ballの左端
	int ballx2 = ball_x + BALL_SIZE;//ballの右端
	int bally1 = ball_y - BALL_SIZE;//ballの上端
	int bally2 = ball_y + BALL_SIZE;//ballの下端

	

	if(ballx1<=0||ballx2>=WINDOW_SIZE_X){
		vx*=-1;
	}

	if(bally1<=0||bally2>=WINDOW_SIZE_Y){
		vy*=-1;
	}

	if(bally2==bar_y&&ballx2<bar_x+BAR_SIZE_X&&ballx2>bar_x){
		vy*=-1;
	}


	

		 
	ball_x += vx;
	ball_y += vy;
}
// WinMain関数
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
					 LPSTR lpCmdLine, int nCmdShow )
{
	// タイトルを 変更
	SetMainWindowText( "ブロック崩し" ) ;
	// ウインドウモードに変更
	ChangeWindowMode( TRUE ) ;
	// ＤＸライブラリ初期化処理
	if( DxLib_Init() == -1 )		// ＤＸライブラリ初期化処理
		return -1 ;			// エラーが起きたら直ちに終了
	// マウスを表示状態にする
	SetMouseDispFlag( TRUE ) ;
	
	Draw();

	while(1)//ゲームを何度もプレイするためのループ
	{
		// スペースボタンが押されるまで待機
		while( !CheckHitKey( KEY_INPUT_SPACE ) )
		{
			// メッセージループに代わる処理をする
			if( ProcessMessage() == -1 )
			{
				DxLib_End() ;				// ＤＸライブラリ使用の終了処理
				return 0 ;				// ソフトの終了 
			}
		}

		// ゲームメインループ
		while ( 1 )
		{
			// メッセージループに代わる処理をする
			if( ProcessMessage() == -1 )
			{
				DxLib_End() ;				// ＤＸライブラリ使用の終了処理
				return 0 ;				// ソフトの終了 
			}
			MoveBar();
			MoveBall();
			Draw();
			
			WaitTimer(4);
		}
		
	}
	DxLib_End() ;				// ＤＸライブラリ使用の終了処理
	return 0 ;				// ソフトの終了 
}