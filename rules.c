#include <stdio.h>
#include<unistd.h>
#include<ncurses.h>
#include<locale.h>

#define MAX_N 1000

void printRules(){
    FILE* fp;
    /*日本語はマルチバイトなので普段より多めにサイズをとる必要がある*/
    char txt[MAX_N];
    char name[] = "rules.txt";
    int x = 0, y = 0;

    clear();
    refresh();

    if ((fp = fopen(name, "r")) == NULL){
        fprintf(stderr, "rules.txtがありません．\n");
    } else {
        while (fscanf(fp, "%s", txt) != EOF){
            move(x++, y);
            printw("%s", txt);
        }
        refresh();
        while(1){
            char c;
            scanw("%c:", &c);
            if (c == 'q'){
                break;
            }
        }
    }
    return;
}


/*int main(void){
    printRules();
    return 0;
}*/

