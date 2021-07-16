#pragma once
/*--+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8----+----9----+---*/
//Copyright (c) 2020
//Hamamatsu Mirai Sougou SEnmongakkpou GameCreate Y.Suzuki
//All rights reseved.

#pragma once

//�}�`�̃w�b�_�t�@�C��

//�w�b�_�t�@�C���ǂݍ���
//�Q�[���̃��C���w�b�_�t�@�C��
#include"game.h"

//�\����

//int�^��POINT�\����
struct iPOINT {

	int x = -1;		//���W��������
	int y = -1;		//���W��������

};

//�~�̍\����
struct MARU
{
	iPOINT center;	//���S�_
	int radius;		//���a
};

//�v���g�^�C�v�錾
//��extern�́A�O���Ɋ֐�������܂���I�Ƃ����ڈ�ł��Ă��܂��B

extern BOOL CheckCollRectToRect(RECT a, RECT b);			//�l�p�̈�̓����蔻����`�F�b�N����
extern BOOL CheckColliPointToRect(iPOINT pt, RECT r);		//�_�Ǝl�p�̓����蔻��
extern BOOL CheckColliPointToMaru(iPOINT pt, MARU m);		//�_�Ɖ~�̓����蔻��
extern BOOL CheckCollMaruToMaru(MARU maru1, MARU maru2);	//�~�Ɖ~�̓����蔻��

extern RECT GetRect(int left, int Top, int right, int bottom);			//RECT�^���ꎞ�I�ɓn��
extern void DrawRect(RECT r,unsigned int color, bool b);				//RECT�𗘗p���Ďl�p��`��
extern VOID DrawMaru(RECT c,unsigned int color, bool b,int thick=1);	//MARU�𗘗p���ĉ~��`��


