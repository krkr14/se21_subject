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
		int selectMode;

		clear();
		refresh();

		move(0,0);
		printw("0で対戦モード，1でルール表示，2で終了：");
		refresh();
		scanw("%d", &selectMode);

		if (selectMode == 0){
			int selectOpponents;
			clear();
			refresh();
			move(0,0);

			printw("二人で対戦(0)かAI対戦(1)か選択してください：");
			scanw("%d", &selectOpponents);
			if (selectOpponents != 0 && selectOpponents != 1){
				clear();
				refresh();
				move(0,0);
				printw("対戦モードを0か1で入力してください：");
				continue;
			}
			/*乱数用*/
			gettimeofday(&now, NULL); 
			srand(now.tv_usec);
			/*盤面を作る*/
			clear();
			refresh();
			make_board();
			
			/*繰り返し部分*/
			loop_game(selectOpponents);

			score(1);
		} else if (selectMode == 1){
			printRules();
		} else if (selectMode == 2){
			break;
		}
	}

	/*window削除*/
	endwin();

	return 0;
}