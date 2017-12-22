/*******************************************************************

	オセロ プログラム　ヒント１　　　　Othello1.c 
	
	松江工業高等専門学校　情報工学科　准教授　橋本　剛
	ゲームプログラミング　授業用

	まずは空いてるマスにとりあえず置くプログラムから、徐々に
	ルールを記述していく
*******************************************************************/
#include <stdio.h>
#include <stdlib.h>

#define SIDE 8 // 一辺の長さ 
#define ASIDE (SIDE + 2) // 局面用配列の一辺 緩衝地帯分２を足す Side for array, add 2
#define BOARDSIZE (ASIDE * ASIDE) // ボードの大きさ　Size of board
#define UP (-ASIDE)
#define DOWN ASIDE
#define RIGHT 1
#define LEFT (-1)
// Piece number
#define B 1 // BLACK 1
#define W 2 // WHITE 2
#define N 3 // 番兵 Sentinel (out of board) 3
#define BLACK_TURN 0 
#define WHITE_TURN 1


//座標からX座標またはY座標を計算するマクロ
#define X(pos) (pos % ASIDE)
#define Y(pos) (pos / ASIDE)
#define TURNCOLOR( turn ) (turn + 1)
#define OPPONENT(turn) !turn

#define TRUE 1
#define FALSE 0
//表示に使う文字定数
const char* piece[3] = { "  ", "●", "○" };
const char* abc[8] = { "a","b","c","d","e","f","g","h"};
int turn; // 手番
int ply; //手数 

typedef unsigned char Move;//手だとわかりやすくするため型を宣言（しなくてもよい）
Move nextmove;//次の手 
unsigned char stonenum[2];//石の数を記憶する変数

//2次元の座標を一次元に変換
int getposition( int x, int y ){
	return y * ASIDE + x;
};

//一番大事な変数、ここでは一次元で表現
unsigned char board[BOARDSIZE] = //intでも良いが、データ格納のことを考えてcharにしている
{//とりあえずはこんな感じで初期化
	N, N, N, N, N, N, N, N, N, N, 
	N, 0, 0, 0, 0, 0, 0, 0, 0, N,
	N, 0, 0, 0, 0, 0, 0, 0, 0, N,
	N, 0, 0, 0, 0, 0, 0, 0, 0, N,
	N, 0, 0, 0, B, W, 0, 0, 0, N,
	N, 0, 0, 0, W, B, 0, 0, 0, N,
	N, 0, 0, 0, 0, 0, 0, 0, 0, N,
	N, 0, 0, 0, 0, 0, 0, 0, 0, N,
	N, 0, 0, 0, 0, 0, 0, 0, 0, N,
	N, N, N, N, N, N, N, N, N, N 
};

// 表示関数 display function
void output()
{//ヒントを参考に、とにかく表示する関数を作りましょう
	int x, y=0;
	printf( "   a  b  c  d  e  f  g  h \n" );

	for(y=1;y<=SIDE;y++){
		printf("%d |",y);
		for ( x = 1; x <= SIDE; x++ ){
			printf( "%s|", piece[board[getposition(x,y)]] );//ちょっと簡潔に書くためのヒント
		}

		printf("\n");
	}

}

// 実際に手を進める
void makemove( Move pos )
{
	//とりあえず、指定された場所に石を置く
	int color = TURNCOLOR( turn );
	board[pos] = color;
	//手番交代
	turn = OPPONENT(turn);//0→1、1→0となる
}

//人間の入力を管理する関数
Move manplayer()
{
	//入力をさせ、合法手かチェック
	//とりあえず最初は石がないところにどこでも置けるから始める

	char line[256];
	int x, y, num, move;

	// input X coordinate
	do
	{
		printf("x(a-h):");
		fgets(line,256,stdin);
	}while(line[0] < 'a' || line[0] > 'h');
	x = line[0] - 'a' + 1;

	// input Y coordinate
	do
	{
		printf("y(1-8):");
		fgets(line,256,stdin);
		y=atoi(&line[0]);
	}while(y < 1 || y > 8);

	

	// Check legal move
	move = getposition( x, y );
	return (Move)move;
}

int isLeagalMove(Move pos){
	
	do{




	}while()


}

int main()
{
	//まずは変数宣言、初期化など
	output();//局面の表示
	while(1)//一回の対局が終わるまでループ
	{
		nextmove = manplayer();
		makemove( nextmove );//一手進める関数
		output();//局面表示
		//選んだ手も表示したい
	}
	return 0;
}