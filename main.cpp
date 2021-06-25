//�w�b�_�t�@�C���̓ǂݍ���
#include "DxLib.h"		//DxLib���g���Ƃ��͕K�v
#include "keyboard.h"	//�L�[�{�[�h�̏���
#include "FPS.h"		//FPS�̏���

//�}�N����`
#define TAMA_DIV_MAX 4	//�e�̉摜�̍ő吔

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

//�e�̉摜�̃n���h��
int Tama[TAMA_DIV_MAX];
int TamaIndex = 0;				//�摜�̓Y��
int TamachengeCnt = 0;			//�摜��ς���^�C�~���O
int TamachengeCntMax = 30;		//�摜��ς���^�C�~���OMAX

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

VOID CollUpdatepPayer(CHARACTOR* chara);	//�����蔻��̗̈���X�V
VOID CollUpdate(CHARACTOR* chara);			//�S�[���̓����蔻��̗̈�X�V

BOOL OnCollRect(RECT a, RECT b);				//��`�Ƌ�`�̓����蔻��

BOOL GameLoad(VOID);	//�Q�[���̃f�[�^��ǂݍ���

BOOL LoadImageMem(IMAGE* image, const char* path);	//�Q�[���̉摜��ǂݍ���
BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playType);	//���y�̓ǂݍ���
BOOL LoadimageDivMem(int* handle, const char* path, int DivWidth, int DivHeight);	//�Q�[���̉摜�̕����̓ǂݍ���

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
		DeleteGraph(Tama[i]);
	}

	// �c�w���C�u�����g�p�̏I������
	DxLib_End();

	return 0;	// �\�t�g�̏I�� 
}

/// <summary>
/// �Q�[���̃f�[�^��ǂݍ���
/// </summary>
/// <param name=""></param>
/// <returns>�ǂݍ��߂���TRUE/�ǂݍ��߂Ȃ�������FALSE</returns>
BOOL GameLoad(VOID)
{
	//�摜�𕪊����ēǂݍ���
	if (LoadimageDivMem(&Tama[0], ".\\Image\\STG_M1.png", 4, 1) == FALSE) { return FALSE; }

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
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//�v���C��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_PLAY);
	}


	return;
}

//�^�C�g����ʂ̕`��
VOID TitleDraw(VOID)
{
	//�e�̕`��
	DrawGraph(0, 0, Tama[TamaIndex], TRUE);

	if (TamachengeCnt < TamachengeCntMax)
	{

		TamachengeCnt++;

	}
	else
	{
		//�e�̓Y�����e�̕������̍ő�������̂Ƃ�
		if (TamaIndex < TAMA_DIV_MAX - 1)
		{
			TamaIndex++;	//���̉摜��
		}
		else
		{
			TamaIndex = 0;	//�ŏ��ɖ߂�
		}
		TamachengeCnt = 0;
	}

	DrawString(0, 0, "�^�C�g�����", GetColor(0, 0, 0));

	return;
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

	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//�G���h��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_END);
	}

	return;
}

//�v���C��ʂ̕`��
VOID PlayDraw(VOID)
{
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

	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
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
VOID CollUpdatepPayer(CHARACTOR* chara)
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