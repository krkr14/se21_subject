#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<unistd.h>
#include<ncurses.h>
#include<locale.h>
#include"f1.h"


/*//////////////////////////////////////////////////
//メイン関数
//////////////////////////////////////////////////*/

int main(void){
	int res[3];
	int i;

	/*window作成*/
	setlocale(LC_ALL, "");
	initscr();

	/* カーソルキーを利用 */  
	keypad(stdscr, TRUE); 

	for (i=0; i<1000; i++){

		/*乱数用*/
		srand((int)time(NULL));

		/*盤面を作る*/
		make_board();

		/*繰り返し部分*/
		loop_game();

		res[score(0)]++;
	}

	/*window削除*/
	endwin();

	printf("先手：%d 後手：%d 引き分け：%d", res[0], res[1], res[2]);

	return 0;
}

/*参照 【C言語】標準入出力でオセロ作ってみた【雑記】
https://tora-k.com/2019/06/17/othello/*/ 