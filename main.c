#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<unistd.h>
#include<ncurses.h>
#include<locale.h>
#include <time.h>
#include <sys/time.h>
#include"f1.h"


/*//////////////////////////////////////////////////
//メイン関数
//////////////////////////////////////////////////*/

int main(void){
	struct timeval now;

	/*window作成*/
	setlocale(LC_ALL, "");
	initscr();

	/* カーソルキーを利用 */  
	keypad(stdscr, TRUE); 


	/*乱数用*/
	gettimeofday(&now, NULL); 
	srand(now.tv_usec);

	/*盤面を作る*/
	make_board();

	/*繰り返し部分*/
	loop_game();

	score(1);

	/*window削除*/
	endwin();

	return 0;
}

/*参照 【C言語】標準入出力でオセロ作ってみた【雑記】
https://tora-k.com/2019/06/17/othello/*/ 