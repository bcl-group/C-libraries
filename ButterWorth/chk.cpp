// ======================
// チェック関数 - ここから
// ======================

#include <stdio.h>	// 標準入出力
#include <stdlib.h>	// 標準ライブラリ

// 必要な引数の数と実際与えた引数の数が違う場合
// エラーを返して異常終了する関数
// argnum: 必要な引数の数
// argc: 実際の引数の数
// usagetxt: Usageテキスト
void argchk(int argnum, int argc, char *usagetxt)
{
	if(argnum != argc)
	{
		printf("%s\n", usagetxt); 
		exit(EXIT_FAILURE);
	}
}

// 開こうとするファイルの有無を判定
// 存在しない場合、エラーを返して異常終了する関数
// fpr: 判定したいファイル名を与えたファイルポインタ
// filename: 開こうとしているファイル名
void fopenchk(FILE *fpr, char *filename, char *usagetxt)
{
	if(fpr == NULL)
	{ 
		printf("\"%s\" is not found.\n"
			 "%s\n", filename, usagetxt);
		exit(EXIT_FAILURE);
	}
}
