// ======================
// �����å��ؿ� - ��������
// ======================

#include <stdio.h>	// ɸ��������
#include <stdlib.h>	// ɸ��饤�֥��

// ɬ�פʰ����ο��ȼº�Ϳ���������ο����㤦���
// ���顼���֤��ư۾ｪλ����ؿ�
// argnum: ɬ�פʰ����ο�
// argc: �ºݤΰ����ο�
// usagetxt: Usage�ƥ�����
void argchk(int argnum, int argc, char *usagetxt)
{
	if(argnum != argc)
	{
		printf("%s\n", usagetxt); 
		exit(EXIT_FAILURE);
	}
}

// �������Ȥ���ե������̵ͭ��Ƚ��
// ¸�ߤ��ʤ���硢���顼���֤��ư۾ｪλ����ؿ�
// fpr: Ƚ�ꤷ�����ե�����̾��Ϳ�����ե�����ݥ���
// filename: �������Ȥ��Ƥ���ե�����̾
void fopenchk(FILE *fpr, char *filename, char *usagetxt)
{
	if(fpr == NULL)
	{ 
		printf("\"%s\" is not found.\n"
			 "%s\n", filename, usagetxt);
		exit(EXIT_FAILURE);
	}
}
