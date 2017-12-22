/*//////////////////////////////////////////////////////////

x��y�ɃT�C�N���C�h�̌����������ē��삳���܂����D
r�ɑ������l��(1+cos��)�������Ď���t�ɂ���ĕω�������悤�ɂ����̂�
�v���Ă����ȏ�ɕs�v�c�ȓ��������Ă���Ă������납�����ł��D
�܂�,direct�ϐ���p�ӂ���
���E�Ώۂɓ��삳����悤�ɂ��܂���.


//////////////////////////////////////////////////////////*/


#define _USE_MATH_DEFINES
#include "DxLib.h"
#include <math.h>



//�E�B���h�E�T�C�Y
#define MIN_X 0
#define MIN_Y 0
#define MAX_X 640
#define MAX_Y 480

//��ʂ̒��S���W
#define CENTER_X ((MIN_X + MAX_X)/2)
#define CENTER_Y ((MIN_Y + MAX_Y)/2)

//����t�̍ő�l
#define MAX_T 3600

//�p�x�̌v�Z
#define OMEGA( t ) (t * M_PI / 180)

//�F
#define WHITE GetColor( 255, 255, 255 )//��ʕ\�����镶���̐F

int t;//����
double x,y;//�퓬�@�̍��W
double r=100;//���a
int fighter_img;//�摜�p�ϐ�

int count=-1;
int direct=1;

//�O���`��p�̕ϐ�
double track_x[MAX_T],track_y[MAX_T];

void Init()
{
	x = CENTER_X;
	y = CENTER_Y;
	t = 0;
	r = 100;
}

//�摜�E���t�@�C����ǂݍ��ފ֐�
void LoadData()
{
	fighter_img = LoadGraph("fighter.png");
}

//�퓬�@�̈ړ�
void MoveFighter()
{
	static int his=1000;

	//r=100;
	r=20*(1+cos(OMEGA((t%his))));
	x = CENTER_X+(r*(OMEGA((t%his))-sin(OMEGA(5*(t%his)))))*direct;
	y = CENTER_Y+r*(1-cos(OMEGA(5*(t%his))));

	track_x[t] = x;
	track_y[t] = y;

	if(x==CENTER_X&&y==CENTER_Y){
		count++;
	}

	//if(count==2){
		if(his==1000){
			his=t;
		}
		direct*=-1;
		count=0;
	}

	//r+=0.5;
}

//�퓬�@�̕`��
void DrawFighter()
{
	
	//���W�\��
	DrawFormatString( 0, 0, WHITE, "���W�@x�F%.2f, y�F%.2f", x, y );
	
	//�퓬�@�\��
	DrawGraphF( x, y, fighter_img, TRUE );
    //�p�x�ɍ��킹�ĉ摜����]
	//DrawRotaGraphF( x, y, 1.0f, OMEGA(t), fighter_img, TRUE ) ;
	//�O���\��
	for(int i=1; i < t; i++){
		DrawLine((int)track_x[i-1], (int)track_y[i-1], (int)track_x[i], (int)track_y[i], WHITE, false);
	}
	
}

//�����������邩�ǂ����`�F�b�N
//����������Ȃ�true, ���Ȃ��Ȃ�false��Ԃ�
bool isInit()
{
	if( t >= MAX_T )
		return true;
	return false;
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
					 LPSTR lpCmdLine, int nCmdShow )
{
	// �^�C�g���� �ύX
	SetMainWindowText( "�T�C�N���C�h�̓���" ) ;
	// �E�C���h�E���[�h�ɕύX
	ChangeWindowMode( TRUE ) ;
	// �c�w���C�u��������������
	if( DxLib_Init() == -1 )		// �c�w���C�u��������������
		return -1 ;			// �G���[���N�����璼���ɏI��
	
	// �`����ʂ𗠂ɂ���
	SetDrawScreen(DX_SCREEN_BACK);
	
	LoadData();
	Init();
	
	//���C�����[�v Esc�L�[�ŏI��
	while(!ProcessMessage() && !CheckHitKey(KEY_INPUT_ESCAPE))
	{
		
		ClearDrawScreen();
        
        //�퓬�@�̈ړ�
		MoveFighter();
		
        //�퓬�@�̕\��
		DrawFighter();
		
		t++;//���Ԃ�i�߂�
		
        //����������
		if(isInit())
			Init();
			
		// ����ʂ̓��e��\��ʂɔ��f
		ScreenFlip();
	}
	return 0;
}
