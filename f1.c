#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<unistd.h>
#include<ncurses.h>
#include<locale.h>
#include"alphaBeta.h"


/*// ////////////////////////////////////////////////
// グローバル変数の定義
// ////////////////////////////////////////////////*/

/*boardは盤面の状況,先手(-1),後手(1),空(0),番兵(2)*/
int board[10][10];

/* playerは先手(-1)後手(1)*/
int player = -1;

/* menueバー*/
int menue = 11;


/*//////////////////////////////////////////////////
//盤面生成
//////////////////////////////////////////////////*/

void make_board(){
	int i, j, k;

	/*変数boardに8*8行列,要素全て0を入力*/
	for(i = 1; i < 9; i++){
		for(j = 1; j < 9; j++){
			board[i][j] = 0;
		}
	}

	/*盤外に2を入力*/
	for(k = 0; k < 10; k ++){
		board[0][k] = 2;
		board[k][0] = 2;
		board[9][k] = 2;
		board[k][9] = 2;
	}

	/*変数boardに石の初期値を入力*/
	board[4][4] = 1;
	board[5][5] = 1;
	board[4][5] = -1;
	board[5][4] = -1;
}



/*//////////////////////////////////////////////////
//特定の座標,方向に相手の石を挟めるか判定
//////////////////////////////////////////////////*/

int pos_dir(int row,int column,int dir_r,int dir_c,int color){
	int times;
	/*指定方向に相手の石がない場合は0を返す*/
	if(!(board[row+dir_r][column+dir_c] == (color*-1))){
		return 0;
	}

	/*指定方向に相手の石がある場合は次のマスを探索する*/
	times = 1;
	while(board[row+(dir_r*times)][column+(dir_c*times)] == (color*-1)){
		times++;
	}

	/*指定方向の最後に自分の石がなければ0を返す*/
	if(!(board[row+(dir_r*times)][column+(dir_c*times)] == color)){
		return 0;
	}

	/*指定方向に相手の石が何個あるかを返す*/
	return times-1;
}



/*//////////////////////////////////////////////////
//特定の場所に手番の石を配置可能か判定
//////////////////////////////////////////////////*/

int pos_place(int row,int column,int color){
	int i, j;
	/*場所が空であるかどうか*/
	if(board[row][column] == 0){

		/*全方向を探索*/
		for(i = -1; i < 2; i++){
			for(j = -1; j < 2; j++){
				if(pos_dir(row,column,i,j,color)){

					/*配置可能であれば1を返す*/
					return 1;
				}
			}
		}
	}
	return 0;
}



/*//////////////////////////////////////////////////
//終了判定
//////////////////////////////////////////////////*/

int flag(){
	int i, j;
	/*全ての場所における場所があるかどうか*/
	for(i = 1; i < 9; i++){
		for(j = 1; j < 9; j++){
			if(pos_place(i,j,player)){
				return 0;
			}
		}
	}

	/*プレイヤーを変えて全ての場所における場所があるかどうか*/
	for(i = 1; i < 9; i++){
		for(j = 1; j < 9; j++){
			if(pos_place(i,j,(player*-1))){
				player *= -1;
				printf("置ける場所がないためplayerを変更しました。\n");
				return 0;
			}
		}
	}

	return 1;
}



/*//////////////////////////////////////////////////
//盤面表示
//////////////////////////////////////////////////*/

void show_board(){
	int i, j;

	start_color();
	init_pair(COLOR_BLUE, COLOR_BLACK, COLOR_GREEN);
	init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_GREEN);
	init_pair(COLOR_MAGENTA, COLOR_BLACK, COLOR_RED);
	init_pair(COLOR_YELLOW, COLOR_WHITE, COLOR_BLUE);
	/*ループで各行を1列ずつ表示*/
	for(i = 0; i < 10; i++){
		for(j = 0; j < 10; j++){
			/*カーソルを移動*/
			move(i, 2 * j);
			switch(board[i][j]){
				case -1:
					attrset(COLOR_PAIR(COLOR_BLUE)); 
					printw("●");
					break;
				case 1:
					attrset(COLOR_PAIR(COLOR_WHITE)); 
					printw("●");
					break;
				case 0:
					attrset(COLOR_PAIR(COLOR_BLUE)); 
					printw("ー");
					break;
				case 2:
					attrset(COLOR_PAIR(COLOR_BLUE)); 
					printw("＃");
					break;
				case 3:
					attrset(COLOR_PAIR(COLOR_MAGENTA)); 
					printw("●");
					board[i][j] = -1;
					break;
				case 4:
					attrset(COLOR_PAIR(COLOR_YELLOW)); 
					printw("●");
					board[i][j] = 1;
					break;
				default:
					break;
			}
		}
	}
	init_pair(COLOR_GREEN, COLOR_WHITE, COLOR_BLACK);
	attrset(COLOR_PAIR(COLOR_GREEN)); 
	refresh();
}


/*////////////////////////////////////////////////
//簡易AI(乱数)
////////////////////////////////////////////////*/

void ai_random(int *row,int *column){

	/*乱数から1から8までの組み合わせを作り出す*/
	*row = rand()%8 + 1;
	*column = rand()%8 + 1;
}


/*//////////////////////////////////////////////////
//カーソル入力
//////////////////////////////////////////////////*/

void get_data(int *row,int *column){
	int ch =0;
	
	mvprintw(menue++, 0, "入力してください(改行で終了)");
	/*キー入力を表示しない*/
	noecho();
	while(ch != '\n') {
		move(*row, 2 * (*column)); 
		ch = getch();  
		if (ch == KEY_UP && *row > 1) *row = *row - 1;
		else if (ch == KEY_UP && *row == 1) *row = 8;

		if (ch == KEY_RIGHT && *column < 8) *column = *column + 1; 
		else if (ch == KEY_RIGHT && *column == 8) *column = 1;

		if (ch == KEY_DOWN && *row < 8) *row = *row + 1;  
		else if (ch == KEY_DOWN && *row == 8) *row = 1;

		if (ch == KEY_LEFT && *column > 1) *column = *column - 1;
		else if (ch == KEY_LEFT && *column == 1) *column = 8;
	}
	/*キー入力を表示する*/
	echo();
}



/*//////////////////////////////////////////////////
//石を配置
//////////////////////////////////////////////////*/

void put_board(int row,int column){
	int i, j, k;
	/*石を変える方向を探索*/
	for(i = -1; i < 2; i++){
		for(j = -1; j < 2; j++){

			/*石を変える必要がある時,変える個数がnumに入る*/
			int num = pos_dir(row,column,i,j,player);
			if(num){

				/*変えられる時,numに入った個数だけ順番に手番の石にする*/
				for(k = 1; k < num + 1; k++){
					board[row+(k*i)][column+(k*j)] = player;
				}
			}
		}
	}

	/*石を追加する*/
	/*先手が言って先に置いたマスに３，後行が４*/
	if (player == -1){
		board[row][column] = 3;
	} else {
		board[row][column] = 4;
	}
}

/*置ける石の場所の背景を白く塗る*/
void can_put(){
	int i, j;
	start_color();
	init_pair(COLOR_RED, COLOR_BLACK, COLOR_WHITE);
	attrset(COLOR_PAIR(COLOR_WHITE));
	for (i=1; i<=8; i++){
		for (j=1; j<=8; j++){
			if (pos_place(i, j, player)){
				move(i, 2 * j); 
				printw("ー");
			}
		}
	}
	init_pair(COLOR_RED, COLOR_WHITE, COLOR_BLACK);
	attrset(COLOR_PAIR(COLOR_RED)); 
	refresh();
}



/*//////////////////////////////////////////////////
//一手分実行
//////////////////////////////////////////////////*/

int do_round(int selectOpponents){

	int row = 1;
	int column = 1;

	/*カーソル座標を取って来る*/
	switch(player){
		case -1:
			mvprintw(menue++, 0, "黒の手番です。\n");
			can_put();
			get_data(&row, &column);
			break;
		case 1:
			mvprintw(menue++, 0, "白の手番です。\n");
			can_put();
			if (selectOpponents){
				computer_get_data(board, &row,&column);
			} else {
				get_data(&row, &column);
			}
			break;
		default :
		break;
	}

	/*配置可能であれば配置*/
	if(pos_place(row,column,player)){
		mvprintw(menue++, 0, "(%d, %d)に配置します。\n",row,column);
		put_board(row,column);
		return 0;
	}

	/*配置不可能であればもう一度入力させる*/
	mvprintw(menue++, 0, "(%d, %d)には配置できません。\n",row,column);
	return do_round(selectOpponents);
}



/*//////////////////////////////////////////////////
//ゲームの繰り返し実行
//////////////////////////////////////////////////*/
void loop_game(int selectOpponents){

	while(1){

		/*終了判定*/
		if(flag()){
			break;
		}

		/*盤面の表示*/
		show_board();

		/*配置*/
		do_round(selectOpponents);

		/*手番交代*/
		player *= -1;

		/*logの位置を初期化*/
		menue = 11;
		move(menue, 0);
		clrtobot();
		refresh(); 
	}
}



/*//////////////////////////////////////////////////
//結果表示
//////////////////////////////////////////////////*/

int score(int flag){
	int i, j;
	int result;
	/*countは石の数を数える*/
	int count_b = 0;
	int count_w = 0;

	/*全探索*/
	for(i = 1; i < 9; i++){
		for(j = 1; j < 9; j++){
			if(board[i][j] == -1){
				count_b++;
			}
			if(board[i][j] == 1){
				count_w++;
			}
		}
	}

	/*盤面の表示*/
	show_board();

	/*結果の表示*/
	mvprintw(menue++, 0, "先手%d石,後手%d石",count_b,count_w);
	if(count_b > count_w){
		mvprintw(menue++, 0, "先手の勝利");
		result = 0;
	}else if(count_b < count_w){
		mvprintw(menue++, 0, "後手の勝利");
		result = 1;
	}else{
		mvprintw(menue++, 0, "引き分け");
		result = 2;
	}
	refresh();

	if (flag){
		mvprintw(menue++, 0, "終了(q)：");
		while(1){
			char c;
			scanw("%c", &c);
			if (c == 'q') break;
		}
		return 0;
	} else {
		return result;
	}
}