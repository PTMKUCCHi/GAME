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

int clearflag=0;
int overflag=0;

int mouseX,mouseY;

int startflag=0;


// �O���[�o���ϐ�
int bar_y = WINDOW_SIZE_Y * 9 / 10;//bar��[��y���W
int bar_x = WINDOW_SIZE_X / 2 - BAR_SIZE_X / 2;//bar���[��x���W
int ball_x = bar_x + BAR_SIZE_X / 2;//ball���S��x���W
int ball_y = bar_y - BALL_SIZE;//ball���S��y���W
int BGMHandle1,BGMHandle2,BGMHandle3,BGMHandle4;
int baikinHandle;
int count=0;
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
int BlockHandle2[BLOCK_NUM_Y];//�u���b�N�̃r�b�g�}�b�v�摜�n���h��

bool isDeleteBlock(int bx,int by){
	if(bx!=-1&&by!=-1){
		if(block[by][bx]==1){
			block[by][bx]=0;
			count+=10;
			return true;
		}
	}
	return false;
}

int getBlockNumX(int bx){

	if(bx<WINDOW_SIZE_X&&bx>0){
		return bx/BLOCK_SIZE_X;
	}
	else{
		return -1;
	}
}


int getBlockNumY(int by){
	if(by<BLOCK_NUM_Y*BLOCK_SIZE_Y+BLOCK_TOP_Y&&by>BLOCK_TOP_Y){
		return (by-BLOCK_TOP_Y)/BLOCK_SIZE_Y;
	}
	else{
		return -1;
	}
}
void gameover(){


	DrawString( WINDOW_SIZE_X/2 , WINDOW_SIZE_Y/2 , "GAME OVER " , GetColor(255,10,20) );
	count=0;

}

void gameclear(){

	DrawString( WINDOW_SIZE_X/2 , WINDOW_SIZE_Y/2 , "GAME CLEAR " , GetColor(22,255,220) );

}

//�`��֐�
void Draw()
{
	static int GrHandle = LoadGraph( "back.bmp" );//�w�i�摜�o�^ 640x480
	//int Cr;
	//bar_x=mouseX;
	DrawGraph( 0 , 0 , GrHandle , FALSE );//�w�i��`��
	DrawBox( bar_x, bar_y, bar_x+BAR_SIZE_X, bar_y+BAR_SIZE_Y, GetColor( 255, 255, 255 ), TRUE);//BAR��`��
	DrawCircle( ball_x, ball_y, BALL_SIZE, GetColor( 100, 100, 100 ), TRUE);//BALL��`��
	for ( int y = 0; y < BLOCK_NUM_Y; y++ )
	{
		for ( int x = 0; x < BLOCK_NUM_X; x++ )
		{
			if( block[y][x] ){
				DrawGraph( BLOCK_SIZE_X*x,  BLOCK_SIZE_Y*y+BLOCK_TOP_Y, BlockHandle[ y ] , FALSE ) ;//BLOCK��`��
				//DrawFormatString( BLOCK_SIZE_X*x,  BLOCK_SIZE_Y*y+BLOCK_TOP_Y,Cr=GetColor( 25, 255, 255),"%d",block[y][x]);
			}
		}
	}

	//DrawFormatString( 0, 400,Cr=GetColor( 25, 255, 255),"%d %d",getBlockNumY(ball_y),getBlockNumX(ball_x));
	 // �ǂ݂��񂾃O���t�B�b�N����ʍ���ɕ`��
    DrawGraph( mouseX , mouseY , baikinHandle, TRUE ) ;
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

	/*if(clearflag==1||overflag==1){
		vy=1;
		vx=1;
	}*/
	
	if(ballx1<=0||ballx2>=WINDOW_SIZE_X){
	
		PlaySoundMem(BGMHandle2,DX_PLAYTYPE_BACK);
		
		vx*=-1;
	}

	if(bally1<=0||bally2>=WINDOW_SIZE_Y){
		PlaySoundMem(BGMHandle2,DX_PLAYTYPE_BACK);
		vy*=-1;
	}

	if(bally2==bar_y&&ballx2<bar_x+BAR_SIZE_X&&ballx2>bar_x){
		PlaySoundMem(BGMHandle2,DX_PLAYTYPE_BACK);
		vy*=-1;
	}

	ball_x += vx;
	ball_y += vy;

	if(isDeleteBlock(getBlockNumX(ball_x),getBlockNumY(ball_y))){
		vy*=-1;
		PlaySoundMem(BGMHandle3,DX_PLAYTYPE_BACK);
	}

	if(WINDOW_SIZE_Y==bally2){
		overflag=1;
	}
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
	unsigned int Cr ;
	int i,j;

	// �}�E�X��\����Ԃɂ���
    SetMouseDispFlag( TRUE ) ;
	

	LoadDivGraph( "block.bmp" , 6, 6 , 1 , BLOCK_SIZE_X , BLOCK_SIZE_Y ,BlockHandle) ;
	LoadDivGraph( "block_hibi.bmp" , 6, 6 , 1 , BLOCK_SIZE_X , BLOCK_SIZE_Y ,BlockHandle2) ;
	baikinHandle=LoadGraph("baikinnmann.bmp");
	BGMHandle1=LoadSoundMem("wa_001.wav");
	BGMHandle2=LoadSoundMem("putCOM.wav");
	BGMHandle3=LoadSoundMem("putMAN.wav");
	BGMHandle4=LoadSoundMem("gameover.mp3");

	Draw();

	while(1)//�Q�[�������x���v���C���邽�߂̃��[�v
	{
		ball_x = bar_x + BAR_SIZE_X / 2;//ball���S��x���W
		ball_y = bar_y - BALL_SIZE-1;//ball���S��y���W

		for(i=0;i<BLOCK_NUM_Y;i++){//�u���b�N������
			for(j=0;j<BLOCK_NUM_X;j++){
				block[i][j]=1;
			}
		}

		overflag=0;
		clearflag=0;


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

        // �}�E�X�̈ʒu���擾
        GetMousePoint( &mouseX , &mouseY ) ;
		PlaySoundMem(BGMHandle1,DX_PLAYTYPE_LOOP);
		// �Q�[�����C�����[�v
		while ( 1 )
		{
			//
			GetMousePoint( &mouseX ,&mouseY);
			Cr = GetColor( GetRand(255) , GetRand(255) , GetRand(255) ) ;
			 
			// ���b�Z�[�W���[�v�ɑ��鏈��������
			if( ProcessMessage() == -1 )
			{
				DxLib_End() ;				// �c�w���C�u�����g�p�̏I������
				return 0 ;				// �\�t�g�̏I�� 
			}
			
			MoveBar();
			MoveBall();
			Draw();

			if(count==960){
				clearflag=1;
				break;
			}

			if(overflag==1){
				break;
			}


			DrawString( 10 , 10 , "STAGE 1" , Cr );
			DrawString( 100 , 10 , "SCORE " , Cr );
			DrawFormatString(160,10,Cr,"%d",count);
			WaitTimer(4);

		}

		if(clearflag==1){
		 gameclear();
		}

		if(overflag==1){
			StopSoundMem( BGMHandle1 ) ;
			PlaySoundMem(BGMHandle4,DX_PLAYTYPE_BACK);
			gameover();
		}

	}
	DxLib_End() ;				// �c�w���C�u�����g�p�̏I������
	return 0 ;				// �\�t�g�̏I�� 
}