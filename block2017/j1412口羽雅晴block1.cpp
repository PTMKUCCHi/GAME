/*******************************************************************

	�u���b�N���� �v���O���� �q���g�@�@�@block0.cpp
	
	���]�H�ƍ������w�Z�@���H�w�ȁ@�y�����@���{�@��

*******************************************************************/
#include "DxLib.h"

// �萔 const ��t����ƒ萔 
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

// �O���[�o���ϐ�
int bar_y = WINDOW_SIZE_Y * 9 / 10;//bar��[��y���W
int bar_x = WINDOW_SIZE_X / 2 - BAR_SIZE_X / 2;//bar���[��x���W
int ball_x = bar_x + BAR_SIZE_X / 2;//ball���S��x���W
int ball_y = bar_y - BALL_SIZE;//ball���S��y���W
int block[BLOCK_NUM_Y][BLOCK_NUM_X] = 
{
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,},
};
int BlockHandle[BLOCK_NUM_Y];//�u���b�N�̃r�b�g�}�b�v�摜�n���h��

//�`��֐�
void Draw()
{
	static int GrHandle = LoadGraph( "back.bmp" );//�w�i�摜�o�^ 640x480
	DrawGraph( 0 , 0 , GrHandle , FALSE );//�w�i��`��
	DrawBox( bar_x, bar_y, bar_x+BAR_SIZE_X, bar_y+BAR_SIZE_Y, GetColor( 255, 255, 255 ), TRUE);//BAR��`��
	DrawCircle( ball_x, ball_y, BALL_SIZE, GetColor( 100, 100, 100 ), TRUE);//BALL��`��
	for ( int y = 0; y < BLOCK_NUM_Y; y++ )
	{
		for ( int x = 0; x < BLOCK_NUM_X; x++ )
		{
			if( block[y][x] )
				DrawBox( x * BLOCK_SIZE_X, BLOCK_TOP_Y + y * BLOCK_SIZE_Y, ( x + 1 ) * BLOCK_SIZE_X - 1, BLOCK_TOP_Y + ( y + 1 ) * BLOCK_SIZE_Y - 1, GetColor( 255, 0, 0 ), TRUE);//BLOCK��`��
		}
	}
}

//Bar�̍��W��ς���֐�
void MoveBar()
{
	if ( CheckHitKey( KEY_INPUT_RIGHT ) ) // �� �L�[ ����������
	{
		bar_x+=2;
	}
	
	if ( CheckHitKey( KEY_INPUT_LEFT ) ) // �� �L�[ ����������
	{
		bar_x-=2;
	}

}

//�{�[���̍��W��ς���֐� ���˂��l����
void MoveBall()
{
	static int vx = 1;//ball��x�������x
	static int vy = 1;//ball��y�������x
	int ballx1 = ball_x - BALL_SIZE;//ball�̍��[
	int ballx2 = ball_x + BALL_SIZE;//ball�̉E�[
	int bally1 = ball_y - BALL_SIZE;//ball�̏�[
	int bally2 = ball_y + BALL_SIZE;//ball�̉��[

	

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
// WinMain�֐�
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
					 LPSTR lpCmdLine, int nCmdShow )
{
	// �^�C�g���� �ύX
	SetMainWindowText( "�u���b�N����" ) ;
	// �E�C���h�E���[�h�ɕύX
	ChangeWindowMode( TRUE ) ;
	// �c�w���C�u��������������
	if( DxLib_Init() == -1 )		// �c�w���C�u��������������
		return -1 ;			// �G���[���N�����璼���ɏI��
	// �}�E�X��\����Ԃɂ���
	SetMouseDispFlag( TRUE ) ;
	
	Draw();

	while(1)//�Q�[�������x���v���C���邽�߂̃��[�v
	{
		// �X�y�[�X�{�^�����������܂őҋ@
		while( !CheckHitKey( KEY_INPUT_SPACE ) )
		{
			// ���b�Z�[�W���[�v�ɑ��鏈��������
			if( ProcessMessage() == -1 )
			{
				DxLib_End() ;				// �c�w���C�u�����g�p�̏I������
				return 0 ;				// �\�t�g�̏I�� 
			}
		}

		// �Q�[�����C�����[�v
		while ( 1 )
		{
			// ���b�Z�[�W���[�v�ɑ��鏈��������
			if( ProcessMessage() == -1 )
			{
				DxLib_End() ;				// �c�w���C�u�����g�p�̏I������
				return 0 ;				// �\�t�g�̏I�� 
			}
			MoveBar();
			MoveBall();
			Draw();
			
			WaitTimer(4);
		}
		
	}
	DxLib_End() ;				// �c�w���C�u�����g�p�̏I������
	return 0 ;				// �\�t�g�̏I�� 
}