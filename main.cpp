#define _CRT_SECURE_NO_WARNINGS

//�w�b�_�t�@�C���̓ǂݍ���
#include "DxLib.h"		//DxLib���g���Ƃ��͕K�v
#include "keyboard.h"	//�L�[�{�[�h�̏���
#include "FPS.h"		//FPS�̏���

#include "mouse.h"		//�}�E�X�̏���
#include "shape.h"		//�}�`�̏���

#include <math.h>		//���w

//�}�N����`
#define TAMA_DIV_MAX 4	//�e�̉摜�̍ő吔
#define TAMA_MAX 20		//�e�̑���

#define TEKI_MAX 10		//�G�̐�
#define TEKI_KIND 8		//�G�̎��

//�\���̂̒�`

//�摜�\����
struct IMAGE
{
	int handle = -1;	//�摜�̃n���h��(�Ǘ��ԍ�)
	char path[255];		//�摜�̏ꏊ(�p�X)

	int x;				//X�ʒu
	int y;				//Y�ʒu
	int width;			//��
	int height;			//����

	BOOL IsDraw = FALSE;	//�摜���`��ł���

};

//�M�����N�^�\����
struct CHARACTOR
{
	IMAGE img;		//�摜�\����
	int speed;		//���x�ړ�
	RECT coll;		//�����蔻��̗̈�(�l�p)

};

//����̍\����
struct MOVIE
{
	int handle = -1;	//����̃n���h��
	char path[255];		//����̃p�X

	int x;				//x�ʒu
	int y;				//y�ʒu
	int width;			//��
	int height;			//����

	int Volume = 255;	//�{�����[���i�ŏ��j0�`255(�ő�)
};

//���y�̍\����

struct AUDIO
{
	int	handle = -1;	//���y�̃n���h��
	char path[255];		//���y�̃p�X

	int Volume = -1;	//�{�����[��(MIN 0 �` 255 MAX)
	int playType = -1;	//BGM or SE

};

//�e�̍\����

struct TAMA
{
	int handle[TAMA_DIV_MAX];		//�摜�̃n���h��
	char path[255];		//�摜�̃p�X

	int DivTate;		//�������i�c�j
	int DivYoko;		//�������i���j
	int DivMAX;			//��������

	int AnimeCnt = 0;		//�A�j���[�V�����J�E���^
	int AnimeCntMAX = 0;	//�A�j���[�V�����J�E���^MAX

	int NowIndex = 0;	//���݂̉摜�̗v�f��

	int	StartX;			//�ŏ���X�ʒu
	int	StartY;			//�ŏ���Y�ʒu

	float radius;		//���a
	float degree;		//�p�x

	int x;				//X�ʒu
	int y;				//Y�ʒu
	int width;			//��
	int height;			//����

	int speed;			//���x

	RECT coll;			//�����蔻��

	BOOL IsDraw = FALSE;	//�摜���`��ł���

};

//�O���[�o���ϐ�
//�V�[�����Ǘ�����ϐ�

GAME_SCENE GameScene;		//���݂̃Q�[���̃V�[��
GAME_SCENE OldGameSnece;	//�O��̃Q�[���̃V�[��
GAME_SCENE NextGameSnece;	//���̃Q�[���̃V�[��

//��ʂ̐؂�ւ�
BOOL IsFadeOut = FALSE;		//�t�F�[�h�A�E�g
BOOL IsFadeIn = FALSE;		//�t�B�[�h�C��

int fadeTimeMill = 2000;	//�؂�ւ��~���b
int fadeTimeMax = fadeTimeMill / 1000 * 60;	//�~���b���t���[���b�ɕϊ�

//�t�F�[�h�A�E�g
int fadeOutCntInit = 0;		//�����l
int fadeOutCnt = fadeOutCntInit;	//�t�F�[�h�A�E�g�̃J�E���^
int fadeOutCntMax = fadeTimeMax;	//�t�F�[�h�A�E�g�̃J�E���^MAX

//�t�F�[�h�C��
int fadeInCntInit = fadeTimeMax;	//�����l
int fadeInCnt = fadeInCntInit;		//�t�F�[�h�A�E�g�̃J�E���^
int fadeInCntMax = fadeTimeMax;		//�t�F�[�h�A�E�g�̃J�E���^MAX

//�e�̍\����
struct TAMA tama_moto;		//��
struct TAMA tama[TAMA_MAX];	//���ۂɎg��

//�e�̔��˃J�E���^
int tamaShotCnt = 0;
int tamaShotCntMAX = 5;

//�v���C���[
CHARACTOR player;

//�w�i�摜
IMAGE back[2];		//�w�i�̉摜��2��
IMAGE SCENE_TITLE;	//�^�C�g����ʂ̔w�i
IMAGE SCENE_END;	//�G���h��ʂ̔w�i

//�G�f�[�^
CHARACTOR teki[TEKI_MAX];

//�G�f�[�^(��)
CHARACTOR teki_moto[TEKI_KIND];

//�G�f�[�^�̃p�X
char tekiPath[TEKI_KIND][255] =
{

	{ ".\\Image\\teki_blue.png"},
	{ ".\\Image\\teki_gray.png"},
	{ ".\\Image\\teki_green.png"},
	{ ".\\Image\\teki_mizu.png"},
	{ ".\\Image\\teki_purple.png"},
	{ ".\\Image\\teki_red.png"},
	{ ".\\Image\\teki_red_big.png"},
	{ ".\\Image\\teki_yellow.png"},

};

//�G���łĂ���J�E���g
int TekiAddCnt = 0;		
int TekiAddCntMAX = 60;	//60FPS��1��E�E�E1�b��1��

//�Q�[���̃X�R�A
int Score = 0;

//�v���g�^�C�v�錾
VOID Title(VOID);		//�^�C�g�����
VOID TitleProc(VOID);	//�^�C�g�����(����)
VOID TitleDraw(VOID);	//�^�C�g�����(�`��)

VOID Play(VOID);		//�v���C���
VOID PlayProc(VOID);	//�v���C���(����)
VOID PlayDraw(VOID);	//�v���C���(�`��)

VOID End(VOID);			//�G���h���
VOID EndProc(VOID);		//�G���h���(����)
VOID EndDraw(VOID);		//�G���h���(�`��)

VOID Change(VOID);		//�؂�ւ����
VOID ChangeProc(VOID);	//�؂�ւ����(����)
VOID ChangeDraw(VOID);	//�؂�ւ����(�`��)

VOID ChangeScene(GAME_SCENE scene);	//�V�[���؂�ւ�

VOID CollUpdatePlayer(CHARACTOR* chara);	//�����蔻��̗̈���X�V
VOID CollUpdate(CHARACTOR* chara);			//�S�[���̓����蔻��̗̈�X�V
VOID CollUpdateTama(TAMA* tama);			//�e�̓����蔻��̗̈�X�V
BOOL OnCollRect(RECT a, RECT b);			//��`�Ƌ�`�̓����蔻��
VOID CollUpdateTeki(CHARACTOR* chara);		//�G�̓����蔻��

BOOL GameLoad(VOID);	//�Q�[���̃f�[�^��ǂݍ���

BOOL LoadImageMem(IMAGE* image, const char* path);	//�Q�[���̉摜��ǂݍ���
BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playType);	//���y�̓ǂݍ���
BOOL LoadimageDivMem(int* handle, const char* path, int DivWidth, int DivHeight);	//�Q�[���̉摜�̕����̓ǂݍ���
VOID DrawTama(TAMA* tama);					//�e�̕`��
VOID ShotTama(TAMA* tama, float deg);		//�e���΂�


VOID GameInit(VOID);	//�Q�[���̃f�[�^�̏�����



//�v���O������ WinMain ����n�܂�܂�
//Windows�̃v���O�������@��(WinAPI)�œ����Ă���I
//DXlib�́ADirectX�Ƃ����A�Q�[���v���O���~���O�����񂽂�Ɏg����d�g��

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine
	, int nCmdShodw)
{
	SetOutApplicationLogValidFlag(FALSE);	//Log.txt���o�͂��Ȃ�
	ChangeWindowMode(TRUE);				//�E�B���h�E���[�h�ɐݒ�(FALSE�̏ꍇ�͑S��ʃ��[�h�ɂȂ�B)
	SetMainWindowText(GAME_TITLE);			//�E�B���h�E�̃^�C�g���̕���
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//�E�B���h�E�̉𑜓x��ݒ�
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);				//�E�B���h�E�̑傫����ݒ�
	SetBackgroundColor(255, 255, 255);					//�f�t�H���g�̔w�i�̐F
	SetWindowIconID(GAME_ICON_ID);						//�Q�[���A�C�R����\��
	SetWindowStyleMode(GAME_WINDOW_BAR);				//�E�B���h�E�o�[�̏�Ԃ̐ݒ�
	SetWaitVSyncFlag(TRUE);								//�E�B���h�E�̐���������L���ɂ���
	SetAlwaysRunFlag(TRUE);								//�E�B���h�E�������ƃA�N�e�B�u�ɂ���i�ʂ̃E�B���h�E���J���Ă��Q�[������A�N�e�B�u�ɂȂ邱�Ƃ͂Ȃ��j

	//��̏�����DX���C�u�����������O�ɐݒ�B

	// �c�w���C�u��������������
	if (DxLib_Init() == -1)
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}


	//�_�u���o�b�t�@�����O�L����
	SetDrawScreen(DX_SCREEN_BACK);

	//�ŏ��̃V�[���̓^�C�g����ʂ���
	GameScene = GAME_SCENE_TITLE;

	//�Q�[���ǂݍ���
	if (!GameLoad())
	{

		//�f�[�^�̓ǂݍ��݂̎��s�����Ƃ�
		DxLib_End();		//Dxlib�I��	

		return -1;			//�ُ�I��
	}


	//�Q�[���S�̂̏�����

	//�Q�[���̏�����
	GameInit();

	//�������[�v
	while (1)
	{
		if (ProcessMessage() != 0) { break; }	//�������[�v�𔲂���B�G���[��E�B���h�E������ꂽ
		if (ClearDrawScreen() != 0) { break; }		//��ʂ���������

		//�L�[�{�[�h���͂̍X�V
		AllKeyUpdate();

		//�}�E�X���͂̍X�V
		MouseUpdete();

		//ESC�L�[�ŋ����I��
		if (KeyClick(KEY_INPUT_ESCAPE) == TRUE) { break; }

		//FPS�l�̍X�V
		FPSUpdate();

		//�ȑO�̃V�[�����擾
		if (GameScene != GAME_SCENE_CHANGE)
		{
			OldGameSnece = GameScene;
		}

		//�V�[�����Ƃɏ������s��
		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			Title();			//�^�C�g�����
			break;
		case GAME_SCENE_PLAY:
			Play();				//�v���C���
			break;
		case GAME_SCENE_END:
			End();				//�G���h���
			break;
		case GAME_SCENE_CHANGE:
			Change();			//�؂�ւ����
			break;
		}

		//�V�[����؂�ւ���
		if (OldGameSnece != GameScene)
		{
			//���݂̃V�[�����؂�ւ���ʂłȂ��Ƃ�
			if (GameScene != GAME_SCENE_CHANGE)
			{
				NextGameSnece = GameScene;		//���̃V�[����ۑ�
				GameScene = GAME_SCENE_CHANGE;	//��ʐ؂�ւ��V�[���ɕς���
			}
		}


		//FPS�l��`��
		FPSDraw();

		//FPS�l��҂�

		FPSWait();

		ScreenFlip();		//�_�u���o�b�t�@�����O������ʂ�`��
	}

	//�ǂݍ��񂾉摜�����
	for (int i = 0; i < TAMA_DIV_MAX; i++)
	{
		DeleteGraph(tama_moto.handle[i]);
	}

	// �c�w���C�u�����g�p�̏I������
	DxLib_End();

	//�v���C���[���
	DeleteGraph(player.img.handle);

	//�w�i�摜���
	DeleteGraph(back[0].handle);
	DeleteGraph(back[1].handle);
	DeleteGraph(SCENE_TITLE.handle);
	DeleteGraph(SCENE_END.handle);


	//�G�̉摜�����
	for (int i=0;i<TEKI_KIND;i++)
	{

		DeleteGraph(teki_moto[i].img.handle);

	}

	return 0;	// �\�t�g�̏I�� 
}

/// <summary>
/// �Q�[���̃f�[�^��ǂݍ���
/// </summary>
/// <param name=""></param>
/// <returns>�ǂݍ��߂���TRUE/�ǂݍ��߂Ȃ�������FALSE</returns>
BOOL GameLoad(VOID)
{
	//�e�̕�������ݒ�
	tama_moto.DivYoko = 4;
	tama_moto.DivTate = 1;

	//�e�̃p�X
	strcpy(tama_moto.path, ".\\Image\\dia_orange.png");

	//�摜�𕪊����ēǂݍ���
	if (LoadimageDivMem(&tama_moto.handle[0], tama_moto.path, tama_moto.DivYoko, tama_moto.DivTate) == FALSE) { return FALSE; }

	//���ƍ������擾
	GetGraphSize(tama_moto.handle[0], &tama_moto.width, &tama_moto.height);

	//�ʒu��ݒ�
	tama_moto.x = GAME_WIDTH / 2 - tama_moto.width / 2;	//��������
	tama_moto.y = GAME_HEIGHT - tama_moto.height;		//��ʉ�

	//���x
	tama_moto.speed = 10 ;

	//�A�j����ς��鑬�x
	tama_moto.AnimeCntMAX = 10;

	//�����蔻��̍X�V
	CollUpdateTama(&tama_moto);

	//�摜��\�����Ȃ�
	tama_moto.IsDraw = FALSE;
	
	//�S�Ă̒e�ɏ����R�s�[
	for (int i = 0; i < TAMA_MAX; i++)
	{
		tama[i] = tama_moto;
	}

	//�v���C���[�̉摜��ǂݍ���
	if (LoadImageMem(&player.img, ".\\Image\\player.png") == FALSE) { return FALSE; }

	//�v���C��ʔw�i�̉摜��ǂݍ��݇@
	if (LoadImageMem(&back[0], ".\\Image\\hosi.png") == FALSE) { return FALSE; }
	
	
	//�v���C��ʔw�i�̉摜��ǂݍ��݇A
	if (LoadImageMem(&back[1], ".\\Image\\hosi_rev.png") == FALSE) { return FALSE; }


	//�^�C�g����ʔw�i�̉摜��ǂݍ���
	if (LoadImageMem(&SCENE_TITLE, ".\\Image\\SCENE_TITLE.png") == FALSE) { return FALSE; }

	//�G���h��ʔw�i�̉摜��ǂݍ���
	if (LoadImageMem(&SCENE_END, ".\\Image\\SCENE_END.png") == FALSE) { return FALSE; }

	//�G�̉摜��ǂݍ���
	for (int i = 0; i < TEKI_KIND; i++)
	{
		if (LoadImageMem(&teki_moto[0].img, tekiPath[i]) == FALSE) { return FALSE; }

	}

	return TRUE;	//�S�ēǂݍ��߂��I
}

/// <summary>
/// �摜�𕪊����ă������ɓǂݍ���
/// </summary>
/// <param name="handle">�n���h���z��̐擪�A�h���X</param>
/// <param name="path">�摜�̃p�X</param>
/// <param name="DivWidth">��������Ƃ��̉��̐�</param>
/// <param name="Diveight">��������Ƃ��̏c�̐�</param>
/// <returns></returns>
BOOL LoadimageDivMem(int* handle, const char* path, int DivWidth, int DivHeight)
{


	//�e�̓ǂݍ���
	int IsTamaLoad = -1;	//�摜���ǂݍ��߂����H

	//�ꎞ�I�ɉ摜�̃n���h����p�ӂ���
	int TamaHandle = LoadGraph(path);

	//�ǂݍ��݃G���[
	if (TamaHandle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//�E�B���h�E�n���h��
			path,				//�{��
			"�摜�ǂݍ��݃G���[",	//�^�C�g��
			MB_OK					//�{�^��
		);

		return FALSE;	//�ǂݍ��ݎ��s
	}


	//�摜�ƕ��ƍ������擾
	int TamaWidth = -1;		//��
	int TamaHeight = -1;	//����
	GetGraphSize(TamaHandle, &TamaWidth, &TamaHeight);

	//�������ēǂݍ���
	IsTamaLoad = LoadDivGraph(
		path,											//�摜�̃p�X
		TAMA_DIV_MAX,									//��������
		DivWidth, DivHeight,								//���̕���,�c�̕���
		TamaWidth / DivWidth, TamaHeight / DivHeight,	//�摜����̕��C����
		handle											//�A���ŊǗ�����z��̐擪�A�h���X
	);

	//�����G���[
	if (IsTamaLoad == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//�E�B���h�E�n���h��
			path,					//�{��
			"�摜�����G���[",	//�^�C�g��
			MB_OK					//�{�^��
		);

		return FALSE;	//�ǂݍ��ݎ��s
	}

	//�ꎞ�I�ɓǂݍ��񂾃n���h�������
	DeleteGraph(TamaHandle);

	return TRUE;
}

// <summary>
/// �Q�[���f�[�^��������
/// </summary>
/// <param name=""></param>
VOID GameInit(VOID)
{
	//�Q�[���X�R�A�̏�����
	Score = 0;

	//�v���C���[�̏�����
	player.img.x = GAME_WIDTH / 2 - player.img.width;
	player.img.y = GAME_HEIGHT / 2 - player.img.height;
	CollUpdatePlayer(&player);		//�����蔻��̍X�V
	player.img.IsDraw = TRUE;		//�`�悷��

	player.speed = 5;

	//�w�i�̉摜��ݒ�@
	back[0].x = 0;
	back[0].y = -back[0].height;	//�摜�̍����A�ʒu�ɂ�����
	back[0].IsDraw = TRUE;		//�`�悷��

	//�w�i�̉摜��ݒ�A
	back[1].x = 0;
	back[1].y = 0;	//�摜�̍����A�ʒu�ɂ�����
	back[1].IsDraw = TRUE;		//�`�悷��

	//�G�̉摜��ǂݍ���
	for (int i = 0; i < TEKI_KIND; i++)
	{
		teki_moto[i].img.x = GAME_WIDTH / 2 - teki_moto[i].img.width;
		teki_moto[i].img.y = -teki_moto[i].img.height;
		CollUpdatePlayer(&teki_moto[i]);		//�����蔻��̍X�V
		teki_moto[i].img.IsDraw = FALSE;		//�`�悷��
	}

	//�^�C�g����ʂ̔w�i��ݒ�
	SCENE_TITLE.x = 0;
	SCENE_TITLE.y = 0;
	SCENE_TITLE.IsDraw = TRUE;			//�`�悷��

	//�G���h��ʂ̔w�i��ݒ�
	SCENE_END.x = 0;
	SCENE_END.y = 0;
	SCENE_END.IsDraw = TRUE;			//�`�悷��

}

/// <summary>
/// �摜���������ɓǂݍ���
/// </summary>
/// <param name="image">�摜�\���̂̃A�h���X</param>
/// <param name="path">�摜�̃p�X</param>
/// <returns></returns>
BOOL LoadImageMem(IMAGE* image, const char* path)
{

	//�摜��ǂݍ���
	strcpyDx(image->path, path);					//�p�X�̃R�s�[
	image->handle = LoadGraph(image->path);			//�摜�̓ǂݍ���


	//�摜���ǂݍ��߂Ȃ������Ƃ��̓G���[(-1)������
	if (image->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
			image->path,//���b�Z�[�W�{��
			"�摜�ǂݍ��݃G���[!",	//���b�Z�[�W�^�C�g��
			MB_OK					//�{�^��
		);

		DxLib_End();	//�����I��
		return -1;	//�G���[�I��

		return FALSE; //�ǂݍ��ݎ��s
	}

	//�摜�̕��ƍ������擾
	GetGraphSize(image->handle, &image->width, &image->height);

	//�ǂݍ��߂�
	return TRUE;
}

/// <summary>
/// ���y���������ɓǂݍ���
/// </summary>
/// <param name="audio">Audio�\���̕ϐ��̃A�h���X</param>
/// <param name="path">Audio�̉��y�p�X</param>
/// <param name="volume">�{�����[��</param>
/// <param name="playType">DX_PLAYTYPE_LOOP or DX_PLAYTYPE_BACK or DX_PLAYTYPE_NORMAL</param>
/// <returns></returns>
BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playType)
{

	//���y�̓ǂݍ���
	strcatDx(audio->path, path);				//�p�X�̃R�s�[
	audio->handle = LoadSoundMem(audio->path);	//���y�̓ǂݍ���

	//���y���ǂݍ��߂Ȃ����Ƃ��́A�G���[(-1)���͂���

	if (audio->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
			audio->path,			//���b�Z�[�W�{��
			"���y�ǂݍ��݃G���[�I",	//���b�Z�[�W�^�C�g��
			MB_OK					//�{�^��
		);

		return FALSE;	//�ǂݍ��ݎ��s
	}

	//���̑��ݒ�
	audio->Volume = volume;
	audio->playType = playType;


	return TRUE;
}


//�V�[����؂�ւ���֐�
VOID ChangeScene(GAME_SCENE scene)
{
	GameScene = scene;	//�v���C��ʂɐ؂�ւ�
	IsFadeIn = FALSE;				//�t�F�[�h�C�����Ȃ�
	IsFadeOut = TRUE;					//�t�B�[�h�A�E�g����
}

//�^�C�g�����
VOID Title(VOID)
{

	TitleProc();	//����
	TitleDraw();	//�`��

	return;
}

//�^�C�g����ʂ̏���
VOID TitleProc(VOID)
{
	if (MouseDown(MOUSE_INPUT_RIGHT) == TRUE)
	{
		//�Q�[���̏�����
		GameInit();

		//�v���C��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_PLAY);

		//�}�E�X��`�悵�Ȃ�
		SetMouseDispFlag(FALSE);
	}


	return;
}

//�^�C�g����ʂ̕`��
VOID TitleDraw(VOID)
{
	//�^�C�g����ʔw�i�̉摜��\��
	DrawGraph(SCENE_TITLE.x, SCENE_TITLE.y, SCENE_TITLE.handle, true);

	DrawString(0, 0, "�^�C�g�����", GetColor(0, 0, 0));	

	return;
}

/// <summary>
/// �e�̕`��
/// </summary>
/// <param name="tama">�e�̍\����</param>
VOID DrawTama(TAMA* tama)
{
	//�e�̕`�悪�ł���Ƃ�
	if (tama->IsDraw == TRUE)
	{
		//�e�̕`��
		DrawGraph(tama->x, tama->y, tama->handle[tama->NowIndex], TRUE);

		if (tama->AnimeCnt < tama->AnimeCntMAX)
		{

			tama->AnimeCnt++;

		}
		else
		{
			//�e�̓Y�����e�̕������̍ő�������̂Ƃ�
			if (tama->NowIndex < TAMA_DIV_MAX - 1)
			{
				tama->NowIndex++;	//���̉摜��
			}
			else
			{
				tama->NowIndex = 0;	//�ŏ��ɖ߂�
			}
			tama->AnimeCnt = 0;
		}
	}
}

//�v���C���
VOID Play(VOID)
{

	PlayProc();	//����
	PlayDraw();	//�`��

	return;
}

//�v���C��ʂ̏���
VOID PlayProc(VOID)
{

	if(MouseDown(MOUSE_INPUT_RIGHT) == TRUE)
	{
		//�G���h��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_END);

		//�}�E�X��`�悷��
		SetMouseDispFlag(TRUE);

		return;
	}

	/*
	//�v���C���[�𑀍삷��
	if (KeyDown(KEY_INPUT_LEFT) == TRUE)
	{
		if (player.img.x - player.speed >= 0)
		{
			player.img.x -= player.speed;
		}
	}

	if (KeyDown(KEY_INPUT_RIGHT) == TRUE)
	{
		if (player.img.x + player.img.width + player.speed <= GAME_WIDTH)
		{
			player.img.x += player.speed;
		}
	}

	if (KeyDown(KEY_INPUT_UP) == TRUE)
	{
		if (player.img.y - player.speed >= 0)
		{
			player.img.y -= player.speed;
		}
	}

	if (KeyDown(KEY_INPUT_DOWN) == TRUE)
	{
		if (player.img.y + player.img.height + player.speed <= GAME_HEIGHT)
		{
			player.img.y += player.speed;
		}
	}
	*/

	//�}�E�X�̈ʒu�Ƀv���C���[��u��
	player.img.x = mouse.Point.x - player.img.width / 2;	//�}�E�X�̈ʒu���摜�̒��S�ɂ���
	player.img.y = mouse.Point.y - player.img.height / 2;	//�}�E�X�̈ʒu���摜�̒��S�ɂ���

	//�v���C���[�̓����蔻����X�V
	CollUpdatePlayer(&player);

	//�X�y�[�X�L�[�������Ă���Ƃ�
	/*
	if (KeyDown(KEY_INPUT_SPACE) == TRUE)
	*/

	//�}�E�X�����{�^���������Ă���Ƃ�
	if (MouseDown(MOUSE_INPUT_LEFT) == TRUE)
	{
		if (tamaShotCnt == 0)
		{
			//�e�𔭎˂���(�e��`�悷��) 
			for (int i = 0; i < TAMA_MAX; i++)
			{
				if (tama[i].IsDraw == FALSE)
				{
					ShotTama(&tama[i], 240);

					//�e���P���o������A���[�v�𔲂���
					break;
				}
			}

			//�e�𔭎˂���(�e��`�悷��) 
			for (int i = 0; i < TAMA_MAX; i++)
			{
				if (tama[i].IsDraw == FALSE)
				{
					ShotTama(&tama[i], 270);

					//�e���P���o������A���[�v�𔲂���
					break;
				}
			}

			//�e�𔭎˂���(�e��`�悷��) 
			for (int i = 0; i < TAMA_MAX; i++)
			{
				if (tama[i].IsDraw == FALSE)
				{
					ShotTama(&tama[i], 300);

					//�e���P���o������A���[�v�𔲂���
					break;
				}
			}

		}

		//�e�̔��ˑ҂�
		if (tamaShotCnt < tamaShotCntMAX)
		{
			tamaShotCnt++;
		}
		else
		{
			tamaShotCnt = 0;
		}
	}

	//�e���΂�
	for (int i = 0; i < TAMA_MAX; i++)
	{
		//�`�悳��Ă���Ƃ�
		if (tama[i].IsDraw == TRUE)
		{
			//�e�̈ʒu���C��
						//�@���S�ʒu�@�{�@��΂��p�x����΂��������v�Z�@���@����
			tama[i].x = tama[i].StartX + cos(tama[i].degree * DX_PI / 180.0f) * tama[i].radius;
			tama[i].y = tama[i].StartY + sin(tama[i].degree * DX_PI / 180.0f) * tama[i].radius;

			//���a�𑫂�
			tama[i].radius += tama[i].speed;

			//�e�𓖂��蔻����X�V
			CollUpdateTama(&tama[i]);

			//��ʊO�ɏo����A�`�悵�Ȃ�
			if (tama[i].y + tama[i].height < 0 ||	//��ʊO�i��j
				tama[i].y > GAME_HEIGHT ||			//��ʊO�i���j
				tama[i].x + tama[i].width < 0 ||	//��ʊO�i���j
				tama[i].x > GAME_WIDTH)				//��ʊO�i�E�j
			{
				tama[i].IsDraw = FALSE;
			}
		}
	}

	if (TekiAddCnt < TekiAddCntMAX)
	{
		TekiAddCnt++;
	}
	else
	{
		TekiAddCnt = 0;

		//�G�𐶐�
		for (int i = 0; i < TEKI_MAX; i++)
		{
			//�`�悳��Ă��Ȃ��G��T��
			if (teki[i].img.IsDraw == FALSE)
			{
				int Bunkatu = 10;	//��ʂ̉�������

				if (Score < 1000)
				{
					teki[i] = teki_moto[0];
				}
				else if (Score < 2000)
				{
					teki[i] = teki_moto[1];
				}
				else
				{
					teki[i] = teki_moto[GetRand(TEKI_KIND - 1)];
				}

				teki[i].img.x = GetRand(Bunkatu - 1) * GAME_WIDTH / Bunkatu;
				teki[i].img.y = -teki[i].img.height;

				teki[i].img.IsDraw = TRUE;	//�`�悷��
				break;
			}
		}
	}

	//�G�̏���
	for (int i = 0; i < TEKI_MAX; i++)
	{
		//�`�悵�Ă���G
		if (teki[i].img.IsDraw == TRUE)
		{
			teki[i].img.y += 1;	//�Ƃ肠�������ֈړ�

			//�G�̓����蔻����X�V
			CollUpdateTeki(&teki[i]);

			//�G�����܂ōs������\�����Ȃ�
			if (teki[i].img.y > GAME_HEIGHT)
			{
				teki[i].img.IsDraw = FALSE;
			}

			//�G�Ǝ����̒e�����������Ƃ�
			for (int cnt = 0; cnt < TAMA_MAX; cnt++)
			{
				//�`�悳��Ă���Ƃ�
				if (tama[cnt].IsDraw == TRUE)
				{
					//�����蔻��
					if (OnCollRect(teki[i].coll, tama[cnt].coll) == TRUE)
					{
						tama[cnt].IsDraw = FALSE;		//�e�̕`��͂��Ȃ�
						teki[i].img.IsDraw = FALSE;	//�G�̕`������Ȃ�

						Score += 100;	//�X�R�A���Z
					}
				}
			}
		}
	}

	return;
}

/// <summary>
/// �e���΂�
/// </summary>
/// <param name="tama"></param>
VOID ShotTama(TAMA* tama, float deg)
{
	
		//�e�𔭎˂���
		tama->IsDraw = TRUE;

		//�e�̈ʒu�����߂�
		tama->StartX = player.img.x + player.img.width / 2 - tama->width / 2;
		tama->StartY = player.img.y;

		//�e�̑��x��ς���
		tama->speed = 6;

		//�e�̊p�x
		tama->degree = deg;

		//�e�̔��a
		tama->radius = 0.0f;

		//�e�̓����蔻��̍X�V
		CollUpdateTama(tama);
	
}

//�v���C��ʂ̕`��
VOID PlayDraw(VOID)
{	

	//�w�i�̕`��
	for (int i = 0; i < 2; i++)
	{
		//�`��
		DrawGraph(back[i].x, back[i].y, back[i].handle, TRUE);

		//�摜�����܂ōs�����Ƃ�
		if (back[i].y > GAME_HEIGHT)
		{
			back[i].y = -back[i].height + 1;	//��������ɖ߂�
		}

		//�摜�����ɓ�����
		back[i].y++;
	}

	//�G�̕`��
	for (int i = 0; i < TEKI_MAX; i++)
	{
		/*
		if (teki[i].img.IsDraw == TRUE)
		{
			DrawGraph(teki[i].img.x, teki[i].img.y, teki[i].img.handle, TRUE);
		}

		//�����蔻��̕`��
		if (GAME_DEBUG == TRUE)
		{
			DrawBox(
				teki[i].coll.left, teki[i].coll.top, teki[i].coll.right, teki[i].coll.bottom,
				GetColor(0, 0, 255), FALSE);
		}
		*/

		//�G���`��ł���Ƃ���
		if (teki[i].img.IsDraw == TRUE)
		{
			DrawGraph(teki[i].img.x, teki[i].img.y, teki[i].img.handle, TRUE);

			//�����蔻��̕`��
			if (GAME_DEBUG == TRUE)
			{
				DrawBox(
					teki[i].coll.left, teki[i].coll.top, teki[i].coll.right, teki[i].coll.bottom,
					GetColor(0, 0, 255), FALSE);
			}
		}

	}


	


	//�v���C���[�̕`��
	if (player.img.IsDraw == TRUE)
	{

		//�v���C���[�̕`��
		DrawGraph(player.img.x, player.img.y, player.img.handle, TRUE);

		//�����蔻��̕`��
		if (GAME_DEBUG == TRUE)
		{
			DrawBox(
				player.coll.left, player.coll.top, player.coll.right, player.coll.bottom,
				GetColor(255, 0, 0), FALSE
				);

		}

	}

	//�e�̕`��
	for (int i = 0; i < TAMA_MAX; i++)
	{
		//�`�悳��Ă���Ƃ�
		if (tama[i].IsDraw==TRUE)
		{
			DrawTama(&tama[i]);

			//�����蔻��̕`��
				if (GAME_DEBUG == TRUE)
				{
					DrawBox(
						tama[i].coll.left,tama[i].coll.top,tama[i].coll.right,tama[i].coll.bottom,
						GetColor(255, 0, 0), FALSE
					);

				}
		}
	}

	//�X�R�A�̕`��
	int old = GetFontSize();	//�ȑO�̃t�H���g�̃T�C�Y���擾
	SetFontSize(40);			//�t�H���g��傫������
	DrawFormatString(0, 100, GetColor(255, 255, 255), "SOCORE:%05d", Score);
	SetFontSize(old);			//�t�H���g�����Ƃɖ߂�

	//�}�E�X�̈ʒu��`��
	MouseDraw();

	DrawString(0, 0, "�v���C���", GetColor(0, 0, 0));

	return;
}

//�G���h���
VOID End(VOID)
{

	EndProc();	//����
	EndDraw();	//�`��

	return;
}

//�G���h��ʂ̏���
VOID EndProc(VOID)
{

	if (MouseDown(MOUSE_INPUT_RIGHT) == TRUE)
	{
		//�^�C�g����ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_TITLE);
	}


	return;
}

//�G���h��ʂ̕`��
VOID EndDraw(VOID)
{

	DrawString(0, 0, "�G���h���", GetColor(0, 0, 0));

	//�G���h��ʔw�i�̉摜��\��
	DrawGraph(SCENE_END.x, SCENE_END.y, SCENE_END.handle, true);
	return;
}

//�؂�ւ����
VOID Change(VOID)
{

	ChangeProc();	//����
	ChangeDraw();	//�`��

	return;
}

//�؂�ւ���ʂ̏���
VOID ChangeProc(VOID)
{
	//�t�F�[�h�C��
	if (IsFadeIn == TRUE)
	{
		if (fadeInCnt > fadeInCntMax)
		{
			fadeInCnt--;	//�J�E���^�����炷
		}
		else
		{
			//�t�F�[�h�C���������I�����

			fadeInCnt = fadeInCntInit;	//�J�E���^������
			IsFadeIn = FALSE;			//�t�F�[�h�C�������I��
		}
	}

	//�t�F�[�h�A�E�g
	if (IsFadeOut == TRUE)
	{
		if (fadeOutCnt < fadeOutCntMax)
		{
			fadeOutCnt++;	//�J�E���^�𑝂₷
		}
		else
		{
			//�t�F�[�h�A�E�g�������I�����

			fadeOutCnt = fadeOutCntInit;	//�J�E���^������
			IsFadeOut = FALSE;			//�t�F�[�h�A�E�g�����I��
		}
	}

	//�؂�ւ������I��
	if (IsFadeIn == FALSE && IsFadeOut == FALSE)
	{
		//�t�F�[�h�C�����Ă��Ȃ��A�t�F�[�h�A�E�g�����Ă��Ȃ��Ƃ�
		GameScene = NextGameSnece;	//���̃V�[���ɐ؂�ւ�
		OldGameSnece = GameScene;	//�ȑO�̃Q�[���V�[���ɍX�V
	}


	return;
}


//�؂�ւ���ʂ̕`��
VOID ChangeDraw(VOID)
{
	//�ȑO�̃V�[����`��
	switch (OldGameSnece)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();	//�^�C�g����ʂ̕`��
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();		//�v���C��ʂ̕`��
		break;
	case GAME_SCENE_END:
		EndDraw();		//�G���h��ʂ̕`��
		break;

	default:
		break;
	}

	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 127);		//0�`255
	//DrawBox(0, 0, GAME_WIDHT, GAME_WIDTH, GetColor(0, 0, 0), TRUE);
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//�t�F�[�h�C��
	if (IsFadeIn == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeInCnt / fadeInCntMax) * 255);
	}

	//�t�F�[�h�A�E�g
	if (IsFadeOut == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeOutCnt / fadeOutCntMax) * 255);
	}


	//�l�p��`��
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0), TRUE);

	//�������I��
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(0, 0, "�؂�ւ����", GetColor(0, 0, 0));

	return;
}

/// <summary>
/// �����蔻��̗̈�X�V (�v���C���[)
/// </summary>
/// <param name="Coll">�����蔻��̗̈�</param>
VOID CollUpdatePlayer(CHARACTOR* chara)
{
	chara->coll.left = chara->img.x + 10;
	chara->coll.top = chara->img.y + 10;
	chara->coll.right = chara->img.x + chara->img.width - 10;
	chara->coll.bottom = chara->img.y + chara->img.height - 10;

	return;
}


/// <summarimg.y>
/// �����蔻��̗̈�X�V(�G)
/// </summarimg.y>
/// <param name="Coll">�����蔻��̗̈�</param>
VOID CollUpdateTeki(CHARACTOR* chara)
{
	chara->coll.left = chara->img.x;
	chara->coll.top = chara->img.y;

	chara->coll.right = chara->img.x + chara->img.width;
	chara->coll.bottom = chara->img.y + chara->img.height;

	return;
}

/// <summarimg.y>
/// �����蔻��̗̈�X�V
/// </summarimg.y>
/// <param name="Coll">�����蔻��̗̈�</param>
VOID CollUpdate(CHARACTOR* chara)
{
	chara->coll.left = chara->img.x;
	chara->coll.top = chara->img.y;

	chara->coll.right = chara->img.x + chara->img.width;
	chara->coll.bottom = chara->img.y + chara->img.height;

	return;
}

/// <summarimg.y>
/// �����蔻��̗̈�X�V�i�e�j
/// </summarimg.y>
/// <param name="Coll">�����蔻��̗̈�</param>
VOID CollUpdateTama(TAMA* tama)
{
	tama->coll.left = tama->x;
	tama->coll.top = tama->y;

	tama->coll.right = tama->x + tama->width;
	tama->coll.bottom = tama->y + tama->height;

	return;
}

/// <summary>
///�@��`�Ƌ�`�̓����蔻��
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <returns></returns>
BOOL OnCollRect(RECT a, RECT b)
{

	if (
		a.left < b.right &&		// ��`A�̍���X���W < ��`B�̉E��X���W�@����
		a.right > b.left &&		// ��`A�̉E��X���W > ��`B�̍���X���W�@����
		a.top < b.bottom &&		// ��`A�̏��Y���W < ��`B�̉���Y���W	����
		a.bottom > b.top		// ��`A�̉���Y���W > ��`B�̏��Y���W
		)
	{
		//�������Ă�Ƃ�
		return true;
	}
	else
	{
		//�������Ă��Ȃ��Ƃ�
		return false;
	}



}