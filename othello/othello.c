/*******************************************************************

	�I�Z�� �v���O�����@�q���g�P�@�@�@�@Othello1.c 
	
	���]�H�ƍ������w�Z�@���H�w�ȁ@�y�����@���{�@��
	�Q�[���v���O���~���O�@���Ɨp

	�܂��͋󂢂Ă�}�X�ɂƂ肠�����u���v���O��������A���X��
	���[�����L�q���Ă���
*******************************************************************/
#include <stdio.h>
#include <stdlib.h>

#define SIDE 8 // ��ӂ̒��� 
#define ASIDE (SIDE + 2) // �ǖʗp�z��̈�� �ɏՒn�ѕ��Q�𑫂� Side for array, add 2
#define BOARDSIZE (ASIDE * ASIDE) // �{�[�h�̑傫���@Size of board
#define UP (-ASIDE)
#define DOWN ASIDE
#define RIGHT 1
#define LEFT (-1)
// Piece number
#define B 1 // BLACK 1
#define W 2 // WHITE 2
#define N 3 // �ԕ� Sentinel (out of board) 3
#define BLACK_TURN 0 
#define WHITE_TURN 1


//���W����X���W�܂���Y���W���v�Z����}�N��
#define X(pos) (pos % ASIDE)
#define Y(pos) (pos / ASIDE)
#define TURNCOLOR( turn ) (turn + 1)
#define OPPONENT(turn) !turn

#define TRUE 1
#define FALSE 0
//�\���Ɏg�������萔
const char* piece[3] = { "  ", "��", "��" };
const char* abc[8] = { "a","b","c","d","e","f","g","h"};
int turn; // ���
int ply; //�萔 

typedef unsigned char Move;//�肾�Ƃ킩��₷�����邽�ߌ^��錾�i���Ȃ��Ă��悢�j
Move nextmove;//���̎� 
unsigned char stonenum[2];//�΂̐����L������ϐ�

//2�����̍��W���ꎟ���ɕϊ�
int getposition( int x, int y ){
	return y * ASIDE + x;
};

//��ԑ厖�ȕϐ��A�����ł͈ꎟ���ŕ\��
unsigned char board[BOARDSIZE] = //int�ł��ǂ����A�f�[�^�i�[�̂��Ƃ��l����char�ɂ��Ă���
{//�Ƃ肠�����͂���Ȋ����ŏ�����
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

// �\���֐� display function
void output()
{//�q���g���Q�l�ɁA�Ƃɂ����\������֐������܂��傤
	int x, y=0;
	printf( "   a  b  c  d  e  f  g  h \n" );

	for(y=1;y<=SIDE;y++){
		printf("%d |",y);
		for ( x = 1; x <= SIDE; x++ ){
			printf( "%s|", piece[board[getposition(x,y)]] );//������ƊȌ��ɏ������߂̃q���g
		}

		printf("\n");
	}

}

// ���ۂɎ��i�߂�
void makemove( Move pos )
{
	//�Ƃ肠�����A�w�肳�ꂽ�ꏊ�ɐ΂�u��
	int color = TURNCOLOR( turn );
	board[pos] = color;
	//��Ԍ��
	turn = OPPONENT(turn);//0��1�A1��0�ƂȂ�
}

//�l�Ԃ̓��͂��Ǘ�����֐�
Move manplayer()
{
	//���͂������A���@�肩�`�F�b�N
	//�Ƃ肠�����ŏ��͐΂��Ȃ��Ƃ���ɂǂ��ł��u���邩��n�߂�

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
	//�܂��͕ϐ��錾�A�������Ȃ�
	output();//�ǖʂ̕\��
	while(1)//���̑΋ǂ��I���܂Ń��[�v
	{
		nextmove = manplayer();
		makemove( nextmove );//���i�߂�֐�
		output();//�ǖʕ\��
		//�I�񂾎���\��������
	}
	return 0;
}