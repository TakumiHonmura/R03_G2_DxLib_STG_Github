//�}�E�X�����̃w�b�_�t�@�C��

//�w�b�_�t�@�C���ǂݍ���
#include"game.h"
#include"shape.h"

//�}�N����`

//�}�E�X�̃{�^��
#define MOUSE_BUTTON_CODE 129	//0x0000�`0x0080�܂�

//�}�E�X�\����
struct MOUSE
{
	int InputValue = 0;					//GetMouseInput�̒l������
	int WheelValu = 0;					//�}�E�X�z�C�[���̉�]��(���̓v���X�l / ��O�̓}�C�i�X�l)
	iPOINT Point;						//�}�E�X�̍��W������
	iPOINT OldPoint;					//�}�E�X�̍��W�i���O�j������
	int OldButton[MOUSE_BUTTON_CODE];	//�}�E�X�̃{�^������(���O)������
	int Button[MOUSE_BUTTON_CODE];		//�}�E�X�̃{�^�����͂�����
};


//�O���̃O���[�o���ϐ�

//�}�E�X���͂��擾
extern MOUSE mouse;

//�v���g�^�C�v�錾
//��extern�́A�O���Ɋ֐�������܂���I�Ƃ����ڈ�ł��Ă��܂��B

extern VOID MouseUpdete(VOID);									//�}�E�X�̓��͏����X�V����
extern BOOL MouseDown(int MOUSE_INPUT);							//�{�^���������Ă��邩�A�}�E�X�R�[�h�Ŕ��f����
extern BOOL MouseUp(int MOUSE_INPUT);							//�{�^���������グ�����A�}�E�X�R�[�h�Ŕ��f����
extern BOOL MouseDownKeep(int MOUSE_INPUT_, int milliTime);		//�{�^�������������Ă��邩�A�}�E�X�R�[�h�Ŕ��f����
extern BOOL MouseClick(int MOUSE_INPUT_);						//�}�E�X���N���b�N�������A�}�E�X�R�[�h�Ŕ��f����
extern VOID MouseDraw(VOID);									//�}�E�X�̏���`�悷��

extern BOOL MouseRectClick(MARU rect, int MOUSE_INPUT_);		//�}�E�X����`�̈���N���b�N������
extern BOOL MouseMaruClick(MARU rect, int MOUSE_INPUT_);		//�}�E�X���~�̈���N���b�N������