#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <locale.h>
#include <time.h>
#include <sys/time.h>
#include "f1.h"
#include "rules.h"


/*//////////////////////////////////////////////////
//メイン関数
//////////////////////////////////////////////////*/

int main(void){

	/*window作成*/
	setlocale(LC_ALL, "");
	initscr();

	/* カーソルキーを利用 */  
	keypad(stdscr, TRUE); 

	while (1){
		struct timeval now;
		int input;

		clear();
		refresh();

		move(0,0);
		printw("0でAI対戦，1でルール表示，2で終了\n");
		refresh();
		scanw("%d", &input);

		if (input == 0){
			/*乱数用*/
			gettimeofday(&now, NULL); 
			srand(now.tv_usec);
			/*盤面を作る*/
			clear();
			refresh();
			make_board();
			
			/*繰り返し部分*/
			loop_game();

			score(1);
		} else if (input == 1){
			printRules();
		} else if (input == 2){
			break;
		}
	}

	/*window削除*/
	endwin();

	return 0;
}

/*参照 【C言語】標準入出力でオセロ作ってみた【雑記】
https://tora-k.com/2019/06/17/othello/*/ 