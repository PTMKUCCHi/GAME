
/*
*	���H�됰
*
*	���Q�[��������
*	3�X�e�[�W�p��
*
*	�����������
*	�E�L�[
*	[�X�y�[�X�L�[]	�{�[������
*	[���L�[]		�d�͕����ύX
*	�E�}�E�X
*	[���h���b�O]	�I�u�W�F�N�g����(��)
*
*	���ǉ��@�\��
*	�E���L�[�ŏd�͂̌�����ύX������
*
*	���X�e�[�W������
*	
*	1 - �����l������킩��X�e�[�W
*   2 - �~���g�����ȒP�ȃX�e�[�W
*	3 -	�d�͕ω����g�p�����X�e�[�W
*	4 - �A�v���uQ�v�ł��肻���ȃX�e�[�W
*/

#include <Box2D/Box2D.h>
#include "DxLib.h"
#define _USE_MATH_DEFINES
#include <math.h>


//�E�B���h�E�T�C�Y
#define MIN_X 0
#define MIN_Y 0
#define MAX_X 960
#define MAX_Y 720

//��ʂ̒��S���W
#define CENTER_X ((MIN_X + MAX_X)/2)
#define CENTER_Y ((MIN_Y + MAX_Y)/2)

//x,y�����̏d��
#define GRAVITY_X 0.0
#define GRAVITY_Y 9.8
#define GRAVITY 9.8
//�X�P�[���ϊ�
#define SCALE 0.01//1�s�N�Z���������[�g����

//���G�����p
#define MAX_OBJECT 10000
#define MAX_POS 500
#define PEN_W 3

//�Q�[���p
#define BALL_SIZE 25//�{�[���̔��a
#define GOAL_SIZE 25//�S�[���̔��a
#define STAGE_NUM 5//�X�e�[�W��

int bnum=0;




int Title_img;
int Hplay_img;
int Gover_img;

float g_x;					//�d�͉����x
float g_y;					//

//�}�E�X�|�C���^���̍��W
struct Pos{
	int x;
	int y;
};

struct Object{
	Pos pos[MAX_POS];//�}�E�X�|�C���^�̍��W
	int color;
	int p_num;
	b2Body *DrawBody;
};

struct Object object[MAX_OBJECT];

struct Stage{
	b2Body *body;
	Pos start;
	Pos goal;
};

struct Stage stage[STAGE_NUM];

int o_num = 0;//���̂̐�
int x,y;//�}�E�X�|�C���^�̍��W
int lx,ly;//1�t���[���O�̃}�E�X�|�C���^�̍��W
bool isDraw = false;//��`�������ǂ���
bool isBall = false;//�{�[�������邩�ǂ���
int stage_num=0;//�X�e�[�W��

//box2d�֘A

//�d�͉����x�ݒ�
b2Vec2 gravity(GRAVITY_X,GRAVITY_Y);
//b2Vec2 gravity(g_x, g_y);
//Box2DWorld
b2World world(gravity, true);

//groundbody
b2BodyDef groundbodyDef;
b2Body *groundbody;

//�{�[��
b2Body *ball;



//�Q�[���I�[�o�[���ǂ���
bool isClear = false;

//�S�{�f�B�̍폜
void DeleteAllBody(){
	if(world.GetBodyCount() > 0){	//�{�f�B���𒲂ׂ�
		//�{�f�B�폜���[�v
		b2Body* b = world.GetBodyList(); 
		while(b != NULL){
			b2Body* nb = b->GetNext();
			world.DestroyBody(b);
			b = nb;
		}
	}
}

//�{�f�B����
//x,y�F���S���W
//w,h�F��,����(�~�̏ꍇ w�F���a)
//angle�F�p�x
//isDynamic�F���������ǂ���
//isBall�F�~���ǂ���
//density�F���x
//restitution�F������
//friction�F���C��
b2Body* CreateBody(float x, float y, float h, float w, float angle,
	bool isDynamic, bool isBall, float density, float restitution, float friction)
{
	//�{�f�B��`
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;	//���I�{�f�B
	bodyDef.position.Set(x, y);
	bodyDef.angle = angle;

	if(isDynamic)
		bodyDef.type = b2_dynamicBody;	//���I�{�f�B
	else
		bodyDef.type = b2_staticBody;	//�ÓI�{�f�B

	//�{�f�B�쐬
	b2Body* body = world.CreateBody(&bodyDef);

	//�t�B�N�X�`����`
	b2FixtureDef fixtureDef;

	//�V�F�C�v�쐬�itrue�Ȃ�~�Afalse�Ȃ璷���`�j
	b2CircleShape dynamicBall;
	dynamicBall.m_radius = w;

	b2CircleShape staticBall;
	staticBall.m_radius = w;

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(w, h);

	b2PolygonShape staticBox;
	staticBox.SetAsBox(w, h);

	if(isDynamic){
		if(isBall)
			fixtureDef.shape = &dynamicBall;
		else
			fixtureDef.shape = &dynamicBox;
	}
	else{
		if(isBall)
			fixtureDef.shape = &staticBall;
		else
			fixtureDef.shape = &staticBox;
	}

	fixtureDef.density = density;		 //���x
	fixtureDef.restitution = restitution;//������
	fixtureDef.friction = friction;	//���C��
	body->CreateFixture(&fixtureDef);
	return body;
}

//�摜�E���t�@�C����ǂݍ��ފ֐�
void LoadData()
{
	//�摜�ǂݍ���
	Title_img=LoadGraph("start.PNG");
	Hplay_img=LoadGraph("H_Play.PNG");
	Gover_img=LoadGraph("over.JPG");

}

//�e�X�e�[�W������
void InitStage(int stage_num)
{
	//�ǁE��
b2Body *floor[10];
	float x, y;//���W
	float w, h;//���E����
	float density = 0.5;//���x
	float restitution = 0.5;//������
	float friction = 0.3f;//���C��

	//MAX_X 960
	//MAX_Y 720

	switch(stage_num){

	case 0://��1�X�e�[�W
		
		stage[stage_num].start.x = 680;
		stage[stage_num].start.y =150;
		stage[stage_num].goal.x = 100;
		stage[stage_num].goal.y = 300;

		restitution = 2.5;//������

		w = 30.0 * SCALE;
		h = 30.0 * SCALE;

		x = 600.0 * SCALE;
		y = 400.0 * SCALE;
		floor[0] = CreateBody(x, y, h, w, 0, false, true, density, restitution, friction);

		x = 500.0 * SCALE;
		y = 550.0 * SCALE;
		floor[0] = CreateBody(x, y, h, w, 0, false, true, density, restitution, friction);

		x = 700.0 * SCALE;
		y = 550.0 * SCALE;
		floor[0] = CreateBody(x, y, h, w, 0, false, true, density, restitution, friction);

		w = 30.0 * SCALE;
		h = 150.0 * SCALE;

		x = 250.0 * SCALE;
		y = 400.0 * SCALE;
		floor[0] = CreateBody(x, y, h, w, 0, false, false, density, restitution, friction);

		w = 30.0 * SCALE;
		h = 30.0 * SCALE;

		x = 100.0 * SCALE;
		y = 600.0 * SCALE;
		floor[0] = CreateBody(x, y, h, w, 0, false, true, density, restitution, friction);

		x = 30.0 * SCALE;
		y = 30.0 * SCALE;
		floor[0] = CreateBody(x, y, h, w, 0, false, true, density, restitution, friction);

		x = 800.0 * SCALE;
		y = 80.0 * SCALE;
		floor[0] = CreateBody(x, y, h, w, 0, false, true, density, restitution, friction);
		
	break;

	case 1://��2�X�e�[�W

		stage[stage_num].start.x = 600;
		stage[stage_num].start.y = 100;
		stage[stage_num].goal.x = CENTER_X;
		stage[stage_num].goal.y = CENTER_Y;

		w = 30.0 * SCALE;
		h = 30.0 * SCALE;

		x = 100.0 * SCALE;
		y = 50.0 * SCALE;
		floor[0] = CreateBody(x, y, h, w, 0, false, true, density, restitution, friction);

		x = 100.0 * SCALE;
		y = CENTER_Y * SCALE;
		floor[0] = CreateBody(x, y, h, w, 0, false, true, density, restitution, friction);

		x = 100.0 * SCALE;
		y = (CENTER_Y+300) * SCALE;
		floor[0] = CreateBody(x, y, h, w, 0, false, true, density, restitution, friction);

		x = CENTER_X * SCALE;
		y = 50.0 * SCALE;
		floor[0] = CreateBody(x, y, h, w, 0, false, true, density, restitution, friction);

		x = (CENTER_X+350) * SCALE;
		y = 50.0 * SCALE;
		floor[0] = CreateBody(x, y, h, w, 0, false, true, density, restitution, friction);

		x = (CENTER_X+350) * SCALE;
		y = CENTER_Y * SCALE;
		floor[0] = CreateBody(x, y, h, w, 0, false, true, density, restitution, friction);

		x = (CENTER_X+350) * SCALE;
		y = (CENTER_Y+300) * SCALE;
		floor[0] = CreateBody(x, y, h, w, 0, false, true, density, restitution, friction);

		x = CENTER_X * SCALE;
		y = (CENTER_Y+300) * SCALE;
		floor[0] = CreateBody(x, y, h, w, 0, false, true, density, restitution, friction);
		
		
	break;

	case 2://��3�X�e�[�W
		stage[stage_num].start.x = 200;
		stage[stage_num].start.y = CENTER_Y-50;
		stage[stage_num].goal.x = 760;
		stage[stage_num].goal.y = 380;

		//��

			//���Ƃ����
		
		w = 180.0 * SCALE;
		h = 30.0 * SCALE;

		x = 200.0 *  SCALE;
		y = 470.0 * SCALE;
			
		if(bnum<10){
			floor[0] = CreateBody(x, y, h, w, 0, false, false, density, restitution, friction);
		}
		floor[0] = CreateBody(x, y, h, w, 0, true, false, density, restitution, friction);

		w = 180.0 * SCALE;
		h = 30.0 * SCALE;

		x = 200.0 *  SCALE;
		y = 200.0 * SCALE;
		floor[0] = CreateBody(x, y, h, w, 0, false, false, density, restitution, friction);




		w = 30.0 * SCALE;
		h = 105.0 * SCALE;

		x = 50.0 *  SCALE;
		y = 335.0 * SCALE;
		floor[0] = CreateBody(x, y, h, w, 0, false, false, density, restitution, friction);

		x = 350.0 *  SCALE;
		y = 335.0 * SCALE;
		floor[0] = CreateBody(x, y, h, w, 0, false, false, density, restitution, friction);

		//�H
		w = 100.0 * SCALE;
		h = 25.0 * SCALE;

		x = 530.0 *  SCALE;
		y = 180.0 * SCALE;
		floor[0] = CreateBody(x, y, h, w, 0, false, false, density, restitution, friction);

		x = 800.0 *  SCALE;
		y = 180.0 * SCALE;
		floor[0] = CreateBody(x, y, h, w, 0, false, false, density, restitution, friction);

		w = 30.0 * SCALE;
		h = 145.0 * SCALE;

		x = 600.0 *  SCALE;
		y = 350 * SCALE;
		floor[0] = CreateBody(x, y, h, w, 0, false, false, density, restitution, friction);

		x = 870.0 *  SCALE;
		y = 350 * SCALE;
		floor[0] = CreateBody(x, y, h, w, 0, false, false, density, restitution, friction);

		w = 40.0 * SCALE;
		h = 20.0 * SCALE;

		x = 530.0 *  SCALE;
		y = 475 * SCALE;
		floor[0] = CreateBody(x, y, h, w, 0, false, false, density, restitution, friction);

		x = 800.0 *  SCALE;
		y = 475 * SCALE;
		floor[0] = CreateBody(x, y, h, w, 0, false, false, density, restitution, friction);

		w = 30.0 * SCALE;
		h = 20.0 * SCALE;

		x = 490.0 *  SCALE;
		y = 280 * SCALE;
		floor[0] = CreateBody(x, y, h, w, 0, false, false, density, restitution, friction);

		x = 760.0 *  SCALE;
		y = 280 * SCALE;
		floor[0] = CreateBody(x, y, h, w, 0, false, false, density, restitution, friction);

		x = 490.0 *  SCALE;
		y = 380 * SCALE;
		floor[0] = CreateBody(x, y, h, w, 0, false, false, density, restitution, friction);

		x = 760.0 *  SCALE;
		y = 380 * SCALE;
		floor[0] = CreateBody(x, y, h, w, 0, false, false, density, restitution, friction);
		break;

		case 3://��3�X�e�[�W
		ChangeGravity(true);
		stage[stage_num].start.x = 150;
		stage[stage_num].start.y = 100;
		stage[stage_num].goal.x = 810;
		stage[stage_num].goal.y = 150;

		w = 150.0 * SCALE;
		h = 30.0 * SCALE;

		x = 810.0 * SCALE;
		y = 200.0 * SCALE;
		floor[0] = CreateBody(x, y, h, w, 0, false, false, density, restitution, friction);

		w = MAX_X * SCALE;
		h = 30.0 * SCALE;

		x = 0.0 * SCALE;
		y = (MAX_Y-10) * SCALE;
		floor[1] = CreateBody(x, y, h, w, 0, false, false, density, restitution, friction);

		
	break;
		}

}


void Init()
{
	//���G�����֘A
	//���̂̓_�̐���������
	for(int i=0;i<MAX_OBJECT;i++)
		object[i].p_num=0;

	object[o_num].color = GetColor(GetRand(255), GetRand(255), GetRand(255));

	GetMousePoint(&x,&y);
	lx = -1000;
	ly = -1000;
}

void DrawObjects()
{

	for(b2Body *body = world.GetBodyList(); body!=NULL;body = body->GetNext() )
	{

		//�`��ʂɏ���������
		for(b2Fixture *fixture = body->GetFixtureList();fixture!=NULL;fixture=fixture->GetNext())
		{
			//�`��
			//��`
			if(fixture->GetType() == b2Shape::e_polygon){
				if(body->GetType()== b2_dynamicBody)
					continue;
				b2PolygonShape *poly = (b2PolygonShape*)fixture->GetShape();
				b2Vec2 vert[b2_maxPolygonVertices];
				int vertNum = poly->GetVertexCount();
				//�ϊ��s����擾����
				b2Transform transformB = body->GetTransform();
				//��`�̐���_���擾����
				for(int i=0; i<vertNum; ++i){
					vert[i] = poly->m_vertices[i];
					//�ϊ��s��Ő���_��ϊ�����
					vert[i] = b2Mul(transformB, vert[i]);
					vert[i].x /= SCALE;
					vert[i].y /= SCALE;
				}
				//�l�p�`
				if(vertNum==4){
					DrawLine((float32)vert[0].x, vert[0].y, vert[1].x, vert[1].y, GetColor(0,255,255));
					DrawLine(vert[1].x, vert[1].y, vert[2].x, vert[2].y, GetColor(0,255,255));
					DrawLine(vert[2].x, vert[2].y, vert[3].x, vert[3].y, GetColor(0,255,255));
					DrawLine(vert[3].x, vert[3].y, vert[0].x, vert[0].y, GetColor(0,255,255));

					DrawLine(vert[2].x, vert[2].y, vert[0].x, vert[0].y, GetColor(0,255,255));
					DrawLine(vert[3].x, vert[3].y, vert[1].x, vert[1].y, GetColor(0,255,255));
				}
			}

			//�~
			if( fixture->GetType() == b2Shape::e_circle ){
				b2CircleShape *poly = (b2CircleShape *)fixture->GetShape();
				//�ϊ��s����擾����
				b2Transform transformB = body->GetTransform();
				b2Vec2 center = b2Mul(transformB,poly->m_p);

				float r = poly->m_radius;//���a;
				float x1,x2,y1,y2;
				float32 angle = body->GetAngle();

				/*x1 = r * cos(angle)+center.x;
				x2 = -r * cos(angle)+center.x;
				y1 = r * sin(angle)+center.y;
				y2 = -r * sin(angle)+center.y;

				DrawLine(x1/SCALE,y1/SCALE,x2/SCALE,y2/SCALE,GetColor(255,0,255));

				angle += M_PI/2;
				x1 = r * cos(angle)+center.x;
				x2 = -r * cos(angle)+center.x;
				y1 = r * sin(angle)+center.y;
				y2 = -r * sin(angle)+center.y;*/

				//DrawLine(x1/SCALE,y1/SCALE,x2/SCALE,y2/SCALE,GetColor(255,0,255));

				DrawCircle(center.x/ SCALE, center.y/SCALE, r/SCALE, GetColor(155,255,155), FALSE);
			}
		}
	}
}

//���G�����֘A
void MouseDraw()
{
	int MouseState;
	MouseState = GetMouseInput();

	//���_�����ő�ɂȂ����ꍇ
	if( object[o_num].p_num == MAX_POS )
		MouseState=0;//�Ȃɂ�������ĂȂ���Ԃɂ���

	switch( MouseState )
	{
	case MOUSE_INPUT_LEFT://�}�E�X�̍��{�^���܂��̓y���^�u�̒ʏ�`��
		GetMousePoint(&x,&y);//�}�E�X�|�C���^�̍��W�擾
		// �~��`��
		if( hypot(lx - x, ly - y) > 5 || !isDraw )//�`�抴�o�̒���
		{
			int p_num = object[o_num].p_num;
			//���W�̕ۑ�
			object[o_num].pos[p_num].x = x;
			object[o_num].pos[p_num].y = y;

			object[o_num].p_num++;

			//1�t���[���O�̍��W�Ƃ��ĕۑ�
			lx = x;
			ly = y;

		}
		isDraw = true;//�`�撆
		break;

	default://���͂��Ȃ��ꍇ

		if(object[o_num].p_num==1)//���_��ł͕��̂����Ȃ�
		{
			object[o_num].p_num=0;
			isDraw=false;

			break;
		}

		if(isDraw)//���̂̕`����I�������ꍇ
		{
			//�{�f�B������
			b2BodyDef bodyDef;
			bodyDef.type = b2_dynamicBody;
			object[o_num].DrawBody = world.CreateBody(&bodyDef);

			float bx, by;
			bx = (x + lx) / 2;
			by = (y + ly) / 2;
			float w;
			w = hypot( lx - x, ly - y );
			float angle;
			angle = atan2( float(ly - y), float(lx - x) );

			b2FixtureDef fixtureDef;

			float density = 1.0;//���x
			float restitution = 0.4;//������
			float friction = 0.7f;//���C��
			fixtureDef.density = density;
			fixtureDef.restitution = restitution;
			fixtureDef.friction = friction;

			//���body�ɑ΂��ĕ�����shape����������
			for( int i=1; i < object[o_num].p_num; i++ )
			{

				b2PolygonShape boxShape;

				//shape�̒��S�����݂̃}�E�X�|�C���^���W��1�t���[���O�̃}�E�X�|�C���^�̍��W�̒��Ԃɂ���
				bx = ( object[o_num].pos[i].x + object[o_num].pos[i-1].x )/2;
				by = ( object[o_num].pos[i].y + object[o_num].pos[i-1].y )/2;

				//����2�_�̋����ɂ���
				w = hypot( object[o_num].pos[i-1].x - object[o_num].pos[i].x ,  object[o_num].pos[i-1].y - object[o_num].pos[i].y ) * SCALE ;

				angle = atan2( (float)(object[o_num].pos[i-1].y - object[o_num].pos[i].y) ,(float)(object[o_num].pos[i-1].x - object[o_num].pos[i].x) );

				boxShape.SetAsBox(w, PEN_W * SCALE, b2Vec2(bx * SCALE,by * SCALE), angle);

				fixtureDef.shape = &boxShape;

				b2Fixture *fixture;

				fixture = object[o_num].DrawBody->CreateFixture(&fixtureDef);

			}

			o_num++;
			object[o_num].color = GetColor(GetRand(255),GetRand(255),GetRand(255));
		}
		isDraw = false;
		break;
	}

	//���G�������̃}�E�X�|�C���^��`��
	if(isDraw)
	{
		for(int j =0; j < object[o_num].p_num; j++)
		{
			int dx, dy;
			int color;
			dx = object[o_num].pos[j].x;
			dy = object[o_num].pos[j].y;
			color = object[o_num].color;
			DrawCircle( dx , dy, PEN_W , color , true ) ;
		}
	}

	//���̂̕`��
	for(int i = 0; i < o_num; i++)
	{
		int color = object[i].color;
		for(b2Fixture *fixture = object[i].DrawBody->GetFixtureList(); fixture!=NULL; fixture=fixture->GetNext())
		{
			//�`��
			//��`
			if(fixture->GetType() == b2Shape::e_polygon){
				b2PolygonShape *poly = (b2PolygonShape*)fixture->GetShape();
				b2Vec2 vert[b2_maxPolygonVertices];
				int vertNum = poly->GetVertexCount();
				//�ϊ��s����擾����
				b2Transform transformB = object[i].DrawBody->GetTransform();
				//��`�̐���_���擾����

				for(int i=0; i<vertNum; ++i){
					vert[i] = poly->m_vertices[i];
					//�ϊ��s��Ő���_��ϊ�����
					vert[i] = b2Mul(transformB, vert[i]);
					//�X�P�[���ϊ�
					vert[i].x /= SCALE;
					vert[i].y /= SCALE;
				}

				//�l�p�`
				if(vertNum==4){
					DrawLine((float32)vert[0].x, vert[0].y, vert[1].x, vert[1].y, color);
					DrawLine(vert[1].x, vert[1].y, vert[2].x, vert[2].y, color);
					DrawLine(vert[2].x, vert[2].y, vert[3].x, vert[3].y, color);
					DrawLine(vert[3].x, vert[3].y, vert[0].x, vert[0].y, color);

					DrawLine(vert[2].x, vert[2].y, vert[0].x, vert[0].y, color);
					DrawLine(vert[3].x, vert[3].y, vert[1].x, vert[1].y, color);
				}
			}
		}
	}
}

//�Q�[���N���A����
void checkClear()
{
	int bodycnt = 0;
	for(b2Body *body = ball; body!=NULL ;body = body->GetNext() )
	{
		//printfDx( "%d %p\n", bodycnt, body );
		//�`��ʂɏ���������
		for(b2Fixture *fixture = body->GetFixtureList();fixture!=NULL;fixture=fixture->GetNext())
		{
			//�~�`�Ȃ珈�����s��
			if( fixture->GetType() != b2Shape::e_circle )
				continue;
			b2CircleShape *poly = (b2CircleShape *)fixture->GetShape();

			//�ϊ��s����擾����
			b2Transform transformB = body->GetTransform();
			b2Vec2 center = b2Mul(transformB,poly->m_p);

			float jx,jy;
			jx = stage[stage_num].goal.x - center.x / SCALE;
			jy = stage[stage_num].goal.y - center.y / SCALE;

			if( hypot(jx, jy) < GOAL_SIZE + BALL_SIZE )//�S�[��
			{
				DeleteAllBody();//�SBody�̏���
				isBall = false;
				stage_num++;//�X�e�[�W����i�߂�
				if(stage_num == STAGE_NUM)
				{
					DrawString(CENTER_X-100, CENTER_Y, "Game Clear", GetColor(255, 255, 255));
				}
				InitStage(stage_num);

				o_num = 0;

				for(int i=0;i<MAX_OBJECT;i++)
					object[i].p_num=0;
				GetMousePoint(&x,&y);
				lx = -1000;
				ly = -1000;

				isDraw = false;
				return;
			}
		}
	}
}

//�Q�[���N���A����
void CheckClearStage()
{
	for(b2Body *body = ball; body!=NULL ;body = body->GetNext() )
	{
		//�`��ʂɏ���������
		for(b2Fixture *fixture = body->GetFixtureList();fixture!=NULL;fixture=fixture->GetNext())
		{
			//�~�`�Ȃ珈�����s��
			if( fixture->GetType() != b2Shape::e_circle )
				continue;
			b2CircleShape *poly = (b2CircleShape *)fixture->GetShape();

			//�ϊ��s����擾����
			b2Transform transformB = body->GetTransform();
			b2Vec2 center = b2Mul(transformB,poly->m_p);

			float jx,jy;
			jx = stage[stage_num].goal.x - center.x / SCALE;
			jy = stage[stage_num].goal.y - center.y / SCALE;

			if( hypot(jx, jy) < GOAL_SIZE + BALL_SIZE )//�S�[��
			{
				//isClear = true;

				DeleteAllBody();//�SBody�̏���
				isBall = false;
				stage_num++;//�X�e�[�W����i�߂�
				InitStage(stage_num);

				o_num = 0;

				for(int i=0;i<MAX_OBJECT;i++)
					object[i].p_num=0;
				GetMousePoint(&x,&y);
				lx = -1000;
				ly = -1000;

				isDraw = false;
				if(stage_num == STAGE_NUM)
					DrawString(CENTER_X-100, CENTER_Y, "Game Clear", GetColor(255, 255, 255));
				return;
			}
		}
	}
}

//�d�͕����ύX 
void ChangeGravity(bool isReset)
{
	static bool HitFlag_UP = false;		//�L�[����p
	static bool HitFlag_DOWN = false;	//�L�[����p
	static bool HitFlag_LEFT = false;	//�L�[����p
	static bool HitFlag_RIGHT = false;	//�L�[����p

	//�L�[���͎�
	if( CheckHitKey( KEY_INPUT_UP ) && !HitFlag_UP )
	{
		HitFlag_UP = true;
		g_x = 0.0;
		g_y = -GRAVITY;
		gravity.Set( g_x, g_y );
		world.SetGravity( gravity );
	}
	if( CheckHitKey( KEY_INPUT_DOWN ) && !HitFlag_DOWN )
	{
		HitFlag_DOWN = true;
		g_x = 0.0;
		g_y = GRAVITY;
		gravity.Set( g_x, g_y );
		world.SetGravity( gravity );
	}
	if( CheckHitKey( KEY_INPUT_LEFT ) && !HitFlag_LEFT )
	{
		HitFlag_LEFT = true;
		g_x = -GRAVITY;
		g_y = 0.0;
		gravity.Set( g_x, g_y );
		world.SetGravity( gravity );
	}
	if( CheckHitKey( KEY_INPUT_RIGHT ) && !HitFlag_RIGHT )
	{
		HitFlag_RIGHT = true;
		g_x = GRAVITY;
		g_y = 0.0;
		gravity.Set( g_x, g_y );
		world.SetGravity( gravity );
	}

	//�t���O����
	if( !CheckHitKey( KEY_INPUT_UP ) )
	{
		HitFlag_UP = false;
	}
	if( !CheckHitKey( KEY_INPUT_DOWN ) )
	{
		HitFlag_DOWN = false;
	}
	if( !CheckHitKey( KEY_INPUT_LEFT ) )
	{
		HitFlag_LEFT = false;
	}
	if( !CheckHitKey( KEY_INPUT_RIGHT ) )
	{
		HitFlag_RIGHT = false;
	}
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )

{
	// �^�C�g���� �ύX
	SetMainWindowText( "Physical G Produced by KUCCHi" ) ;
	ChangeWindowMode(true);
	SetGraphMode(960, 720, 32); 
	
	if( DxLib_Init() == -1 )//�c�w���C�u��������������
		return -1;//�G���[���N�����璼���ɏI��
	//������
	Init();

	//�`���𗠉�ʂɐݒ�
	SetDrawScreen( DX_SCREEN_BACK );

	//�����v�Z�p�ϐ�
	//�X�V���x
	float32 timeStep = 1.0f / 60.0f;
	//������
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	bool HitFlag_SPACE = false;//�L�[����p
	bool HitFlag_Z = false;//�L�[����p
	bool isClear = false;

	int count=0;

	InitStage(stage_num);//�X�e�[�W�̏�����

	LoadData();

	DrawGraph(0,0,Title_img,false);
		//DrawString(0,330,"Press Space Bar",WHITE);//�X�y�[�X�L�[�������ĊJ�n
		ScreenFlip();
		while(!CheckHitKey(KEY_INPUT_SPACE))
		{
			if ( ProcessMessage() == -1 )
			{
				DxLib_End();// �c�w���C�u�����g�p�̏I������
				return 0;
			}		//���C�����[�v Esc�L�[�ŏI��
		}

		ClearDrawScreen();

		DrawGraph(0,0,Hplay_img,false);

		ScreenFlip();

		while(!CheckHitKey(KEY_INPUT_RETURN))
		{
			if ( ProcessMessage() == -1 )
			{
				DxLib_End();// �c�w���C�u�����g�p�̏I������
				return 0;
			}		//���C�����[�v Esc�L�[�ŏI��
		}

		


	//Esc�L�[�ŏI��
	while(!ProcessMessage() && !CheckHitKey(KEY_INPUT_ESCAPE) && stage_num<STAGE_NUM )
	{
		//��ʂ�������
		ClearDrawScreen();

		//�S�Ă̕��̂�����
		if( CheckHitKey(KEY_INPUT_Z) && !HitFlag_Z ){
			DeleteAllBody();//�SBody�̏���
			InitStage(stage_num);

			//���G�����p�ϐ��̏�����
			o_num = 0;

			for(int i=0;i<MAX_OBJECT;i++)
				object[i].p_num=0;

			GetMousePoint(&x,&y);

			lx = -1000;
			ly = -1000;

			//�e�t���O��ݒ�
			isDraw = false;
			HitFlag_Z = true;
			isBall = false;
			continue;

		}
		if( !CheckHitKey(KEY_INPUT_Z) )
			HitFlag_Z = false;

		if( CheckHitKey(KEY_INPUT_SPACE) && !HitFlag_SPACE ){

			HitFlag_SPACE = true;
			isBall = true;//�{�[�������݂��邩�ǂ���

			bnum++;

			float density = 0.5;//���x
			float restitution = 0.5;//������
			float friction = 0.3f;//���C��

			float sx,sy;
			sx = stage[stage_num].start.x * SCALE;
			sy = stage[stage_num].start.y * SCALE;

			float w = BALL_SIZE * SCALE;

			ball = CreateBody( sx, sy, 0, w, 0, true, true, density, restitution, friction);
		}
		if( !CheckHitKey(KEY_INPUT_SPACE) )
			HitFlag_SPACE = false;

		//���̐���
		MouseDraw();

		//�����v�Z
		world.Step(timeStep, velocityIterations, positionIterations);
		world.ClearForces();

		
		if(bnum>10&&stage_num==2){
			if(world.GetBodyCount()>0){
				b2Body *b=world.GetBodyList();
				b=b->GetNext();
				while(b->GetNext()!=NULL){
					b=b->GetNext();
					//count++;
				}

				b->SetType(b2_dynamicBody);
			}
		}

		//DrawFormatString(CENTER_X, 200, GetColor(255, 255, 155),"STAGE %d",count );

		if(stage_num==2){
			ChangeGravity(false);
		}
		
		//�S�[���̕`��
		DrawCircle(stage[stage_num].goal.x, stage[stage_num].goal.y, GOAL_SIZE, GetColor(255,255,255), true);

		//���̂̕`��
		DrawObjects();

		SetFontSize(50);
		DrawFormatString(CENTER_X+200, 0, GetColor(255, 255, 155),"STAGE %d",stage_num+1 );
		

		//�Q�[���N���A����
		if(isBall )//�{�[�������݂���Ȃ�
			checkClear();

		//����ʂ̓��e��\��ʂɔ��f
		ScreenFlip();
	}

	//����ʂ̓��e��\��ʂɔ��f
	ScreenFlip();
	
	WaitKey();
	DxLib_End();//�c�w���C�u�����g�p�̏I������

	return 0;//�\�t�g�̏I�� 
}