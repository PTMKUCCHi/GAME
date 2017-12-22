/*******************************************************************

	オセロ プログラム　GUI関係ファイル　　　GUIothello.cpp
	
	松江工業高等専門学校　情報工学科　准教授　橋本　剛
	ゲームプログラミング　授業用

*******************************************************************/

#include "DxLib.h"
#include "othello.h"

int d = 50;//マスの間隔
int r = d / 2;//石の半径
int sx = 20, sy = 20;//盤左上の座標
int ex = sx + d * SIDE;//盤右下のx座標
int ey = sy + d * SIDE;//盤右下のy座標
int rx = ex + 10;//画面右　結果など表示するための場所ｘ座標
int STRCOLOR = GetColor( 120, 60, 200 );//文字色
int STRFONTSIZE = 32;//文字フォントサイズ

//マウスの座標からオセロの座標に変換する関数
int GetPosfromMouse(int MouseX, int MouseY)
{
	int x,y;
	if(!(MouseX<sx&&MouseX>ex&&MouseY<sy&&MouseY>ey)){
		x=(MouseX-sx+d)/d;
		y=(MouseY-sy+d)/d;
		
		return getposition(x,y);
	}
}
// GUI用のmanPlayer関数
void manPlayerGUI()
{
	//入力をさせ、合法手かチェック
	int x, y, num, move;
	int pushP;

	//パスのチェック　関数generateMovesが完成したらコメントアウトを外す
	Move moves[MOVENUM];
	num = generateMoves( moves );
	// 合法手が無い場合
	if(num == 0)
	{
	DrawFormatString(ex+20,200,GetColor(123,44,64),"PASS♡");// パスであることを表示する
	// 何か入力させる（Enter押すだけなど）
	nextmove = PASSMOVE;
	return;
	}

	do{
		GetMousePoint(&x,&y);
		pushP=GetMouseInput();
		// Check legal move

		if((pushP&MOUSE_INPUT_LEFT)!=0){
		move = GetPosfromMouse( x, y );

		if(isLegalMove(move)==TRUE)
		{
			nextmove=move;
			return;
		}

		printf("Illegal Move!\n");
		}
	}while(1);
}

// 画面描画関数　** 要変更！！**
void ShowBoard()
{
	int color;
	int x, y, stone;
	int i,j;
	int ry = 40, rdy = STRFONTSIZE + 5;
	static int GrHandle = LoadGraph( "back.bmp" );//背景画像登録 640x480
	DrawGraph( 0 , 0 , GrHandle , FALSE );//背景を描く
	SetFontSize( STRFONTSIZE ) ;
	DrawBox( sx, sy, ex, ey, GetColor( 124, 215, 44 ), TRUE );//緑の盤を描画


	for(i=sy;i<ey;i+=d){
		DrawLine(sx,i,ex,i,GetColor(0,0,0),TRUE);
	}
	for(j=sx;j<ex;j+=d){
		DrawLine(j,sy,j,ey,GetColor(0,0,0),TRUE);
	}
	for ( y = 1; y <= SIDE; y++ )
	{
		for ( x = 1; x <= SIDE; x++ )
		{
			//ここから石の描画
			stone = board[getposition(x,y)];//☆石の色を調べる
			if ( stone == 0 )
				continue;
			if ( stone == B )
				color = GetColor( 0, 0, 0 );//黒
			else
				color = GetColor( 255, 255, 255 );//白
			DrawCircle( sx+d*(x-1)+r ,sy+d*(y-1)+r,r-5, color, TRUE);//円で石を描画
		}
	}
	DrawFormatString( rx, ry, GetColor( 10, 10, 10 ),"●: %d", stonenum[BLACK_TURN] );
	ry += rdy;
	DrawFormatString( rx, ry, GetColor(  240, 240, 240 ),"○: %d", stonenum[WHITE_TURN] );
	ry += rdy * 2;
	DrawFormatString( rx, ry, STRCOLOR,"%d 手", ply );
}


// WinMain関数

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow )
{
	int manturn = BLACK_TURN;
	// タイトルを 変更
	SetMainWindowText( "オセロゲーム 口羽" ) ;
	// ウインドウモードに変更
	ChangeWindowMode( TRUE ) ;
	// ＤＸライブラリ初期化処理
	if( DxLib_Init() == -1 )		// ＤＸライブラリ初期化処理
		return -1 ;			// エラーが起きたら直ちに終了
	// マウスを表示状態にする
	SetMouseDispFlag( TRUE ) ;
	
	init();
	ShowBoard();
	SetFontSize( 64 ) ;
	DrawString( 80 , ey / 2 - 20, "Push Space Key!!", STRCOLOR );

	// スペースボタンが押されるまで待機
	while( !CheckHitKey( KEY_INPUT_SPACE ) )
	{
		// メッセージループに代わる処理をする
		if( ProcessMessage() == -1 )
			break;		// エラーが起きたらループを抜ける
	}

	ShowBoard();
	SetFontSize( 32 ) ;
	DrawString( rx , ey / 2 - 20, "GAME START!!", STRCOLOR );
	WaitTimer(2000);

	ShowBoard();

	// 対局メインループ
	while ( !CheckHitKey( KEY_INPUT_ESCAPE ) )//ESCでゲーム終了
	{

		int num, result;
		Move moves[MOVENUM];
		// メッセージループに代わる処理をする
		if( ProcessMessage() == -1 )
			break ;		// エラーが起きたらループを抜ける
		num = generateMoves( moves );
		if ( num == 0 && isTerminalNode(FALSE))//終局かチェック
		{
			SetFontSize( 32 ) ;
			//石の数で勝ち負け判定し表示
			result = stonenum[BLACK_TURN] - stonenum[WHITE_TURN];//こんな感じで
			// result により表示を変える
			if ( result == 0 )
				DrawString( rx , ey / 2 - 20, "DRAW!!", STRCOLOR );
			else 
				DrawFormatString( rx , ey / 2 - 20, STRCOLOR, "%s WIN!!", ( result > 0 ? "BLACK": "WHITE") );
			break;
		}
		if ( turn == manturn )
			manPlayerGUI();
			//randPlayer();
		else
			comPlayer();
		    makemove( nextmove, 0 );
		if( nextmove == PASSMOVE)
			DrawString( rx , ey / 2 - 20, "PASS!!", STRCOLOR );
		else
			ply++;
		ShowBoard();
		WaitTimer(100);
	}
	DrawString( rx , ey / 2 + 50, "Push ANY Key!!", STRCOLOR );
	while( !CheckHitKeyAll())
	{
		// メッセージループに代わる処理をする
		if( ProcessMessage() == -1 )
			break ;		// エラーが起きたらループを抜ける
	}

	DxLib_End() ;				// ＤＸライブラリ使用の終了処理
	return 0 ;				// ソフトの終了 
}