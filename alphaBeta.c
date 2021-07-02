#include<stdio.h>
#include<stdlib.h>
#define MIN_VALUE -1001001
#define MAX_VALUE 1001001
#define SEARCH_LEVEL 9

struct UNDO {
    int x;
    int y;
    int c;
    int pos_x[64];
    int pos_y[64];
};


const int valueOfPlace[8][8] = {
            {120, -20, 20,  5,  5, 20, -20, 120},
            {-20, -40, -5, -5, -5, -5, -40, -20},
            { 20,  -5, 15,  3,  3, 15,  -5,  20},
            {  5,  -5,  3,  3,  3,  3,  -5,   5},
            {  5,  -5,  3,  3,  3,  3,  -5,   5},
            { 20,  -5, 15,  3,  3, 15,  -5,  20},
            {-20, -40, -5, -5, -5, -5, -40, -20},
            {120, -20, 20,  5,  5, 20, -20, 120}
};

/*
void show_board(int board[10][10]){
	int i, j;
	for(i = 0; i < 10; i++){
		for(j = 0; j < 10; j++){
			printf("%d", board[i][j]);
		}
        printf("\n");
	}
    printf("\n");
}*/


/*//////////////////////////////////////////////////
//特定の座標,方向に相手の石を挟めるか判定
//////////////////////////////////////////////////*/

int pos_dir_undo(int board[10][10], int row,int column,int dir_r,int dir_c,int color){
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
//石を配置
//////////////////////////////////////////////////*/

void put_board_undo(int board[10][10], int row,int column, struct UNDO *undo, int ai){
	int i, j, k;
	/*石を変える方向を探索*/
	for(i = -1; i < 2; i++){
		for(j = -1; j < 2; j++){

			/*石を変える必要がある時,変える個数がnumに入る*/
			int num = pos_dir_undo(board, row, column, i, j, ai);
			if(num){

				/*変えられる時,numに入った個数だけ順番に手番の石にする*/
				for(k = 1; k < num + 1; k++){
					board[row+(k*i)][column+(k*j)] = ai;
                    undo->pos_x[undo->c] = row + (k*i);
                    undo->pos_y[undo->c++] = column + (k*j);
				}
			}
		}
	}

	/*石を追加する*/
	board[row][column] = ai;
}


void undoBoard(int board[10][10], struct UNDO undo){
    int i;
    for (i=0; i<undo.c; i++){
        /* ひっくり返した位置を取得*/
        int x = undo.pos_x[i];
        int y = undo.pos_y[i];
        /* 元に戻すには-1をかければよい*/
        /* 黒(1)は白(-1)に白は黒になる*/
        board[x][y] *= -1;
    }
    /* 石を打つ前に戻す*/
    board[undo.x][undo.y] = 0;
}



/*//////////////////////////////////////////////////
//特定の場所に手番の石を配置可能か判定
//////////////////////////////////////////////////*/

int pos_place_undo(int board[10][10], int row,int column,int color){
	int i, j;
	/*場所が空であるかどうか*/
	if(board[row][column] == 0){

	/*全方向を探索*/
	for(i = -1; i < 2; i++){
		for(j = -1; j < 2; j++){
			if(pos_dir_undo(board, row,column,i,j,color)){

				/*配置可能であれば1を返す*/
				return 1;
			}
		}
	}
	}
	return 0;
}


/*評価関数*/
int value_board(int board[10][10]){
    int x, y;
    int value = 0;
    for (x=1; x<=8; x++){
        for (y=1; y<=8; y++){
            value += board[x][y] * valueOfPlace[x-1][y-1];
        }
    }
    return value;
}


/*置くことができるなら0，できないなら1*/
int flag_undo(int board[10][10]){
    int x, y;
    for (x=1; x<=8; x++){
        for (y=1; y<=8; y++){
            if (board[x][y] == 0) return 0;
        }
    }
    return 1;
}



int  alphaBeta(int board[10][10], int ai, int level, int alpha, int beta){

    int x, y;
    /* ノードの評価値*/
    int value;
    /* 子ノードから伝播してきた評価値*/
    int childValue;
    /* ミニマックス法で求めた最大の評価値を持つ場所*/
    int bestX = 0;
    int bestY = 0;

    /* ゲーム木の末端では盤面評価
     その他のノードはMIN or MAXで伝播する*/
    if (level == 0 || flag_undo(board)) {
        return value_board(board);
    }


    if (ai == 1) {
        /* AIの手番では最大の評価値を見つけたいので最初に最小値をセットしておく*/
        value = MIN_VALUE;
    } else {
        /* プレイヤーの手番では最小の評価値を見つけたいので最初に最大値をセットしておく*/
        value = MAX_VALUE;
    }

    /* 打てるところはすべて試す（試すだけで実際には打たない）*/
    for (x = 1; x <= 8; x++) {
        for (y = 1; y <= 8; y++) {
            if (pos_place_undo(board, x, y, ai)) {
                struct UNDO undo;
                undo.x = x;
                undo.y = y;
                undo.c = 0;
                /* 試しに打ってみる*/
                put_board_undo(board, x, y, &undo, ai);
                
                /* 子ノードの評価値を計算（再帰）
                 今度は相手の番なのでflagが逆転する*/
                childValue = alphaBeta(board, -ai, level - 1, alpha, beta);
    
                /* 子ノードとこのノードの評価値を比較する*/
                if (ai == 1) {
                    /* AIのノードなら子ノードの中で最大の評価値を選ぶ*/
                    if (childValue >= value) {
                        value = childValue;
                        alpha = value;
                        bestX = x;
                        bestY = y;
                    }

                    /*βカット*/
                    if (value > beta){
                        undoBoard(board, undo);
                        return value;
                    }
                } else {
                    /* プレイヤーのノードなら子ノードの中で最小の評価値を選ぶ*/
                    if (childValue <= value) {
                        value = childValue;
                        beta = value;
                        bestX = x;
                        bestY = y;
                    }

                    /*αカット*/
                    if (value < alpha){
                        undoBoard(board, undo);
                        return value;
                    }
                }
                /* 打つ前に戻す*/
                undoBoard(board, undo);
            }
        }
    }

    if (level == SEARCH_LEVEL) {
        /* ルートノードなら最大評価値を持つ場所を返す*/
        return bestX + bestY * 64;
    } else {
        /* 子ノードならノードの評価値を返す*/
        return value;
    }
}

void computer_get_data(int board[10][10], int *row, int *column){
    int tmp = alphaBeta(board, 1, SEARCH_LEVEL, MIN_VALUE, MAX_VALUE);
    *row = tmp % 64;
    *column = tmp / 64;
}

/*ドライバー
int main(){
    int board[10][10] = {{2,2,2,2,2,2,2,2,2,2},
                     {2,0,0,0,0,0,0,0,0,2},
                     {2,0,0,0,0,0,0,0,0,2},
                     {2,0,0,0,0,0,0,0,0,2},
                     {2,0,0,0,1,-1,0,0,0,2},
                     {2,0,0,0,-1,1,0,0,0,2},
                     {2,0,0,0,0,0,0,0,0,2},
                     {2,0,0,0,0,0,0,0,0,2},
                     {2,0,0,0,0,0,0,0,0,2},
                     {2,2,2,2,2,2,2,2,2,2}};
    int row, column;

    computer_get_data(board, &row, &column);
    return 0;
}*/