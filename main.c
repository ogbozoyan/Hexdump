#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>

FILE* f;
FILE* dbg;

int OFFSET = 0;
char ch = 0;
bool flag = true;
int line = 1;
int pos = 0;

void clear_bytes(char* bytes, char* clearbytes);

int init_ncurses();

void print_file(char *bytes,char *clearbytes) {
    if (line % 49 == 0) {
        clear();
    }
    mvprintw(line % 49, 0, "%#010x || %x%x%x%x %x%x%x%x %x%x%x%x %x%x%x%x || %c%c%c%c   %c%c%c%c  %c%c%c%c  %c%c%c%c \n",
             OFFSET, bytes[OFFSET], bytes[OFFSET+1], bytes[OFFSET+2], bytes[OFFSET+3], bytes[OFFSET+4], bytes[OFFSET+5], bytes[OFFSET+6], bytes[OFFSET+7], bytes[OFFSET+8],
             bytes[OFFSET+9], bytes[OFFSET+10], bytes[OFFSET+11], bytes[OFFSET+12], bytes[OFFSET+13], bytes[OFFSET+14], bytes[OFFSET+15],
             clearbytes[OFFSET], clearbytes[OFFSET+1], clearbytes[OFFSET+2], clearbytes[OFFSET+3], clearbytes[OFFSET+4], clearbytes[OFFSET+5],
             clearbytes[OFFSET+6], clearbytes[OFFSET+7], clearbytes[OFFSET+8], clearbytes[OFFSET+9], clearbytes[OFFSET+10],
             clearbytes[OFFSET+11], clearbytes[OFFSET+12], clearbytes[OFFSET+13], clearbytes[OFFSET+14], clearbytes[OFFSET+15]);
    refresh();
    line++;
}


int main(int argc,char **argv) {
    /*===============================|+FILE GET INFO+|======================================*/
    char *namefile = argv[1];

    if (argc < 2) {
        printf("Usage: %s name_of_file", argv[0]);
        return -1;
    }

    f = fopen(argv[1], "rb");
    if (f == NULL) {
        fprintf(stderr, "Couldn't open %s:\n", argv[1]);
        exit(1);
    }

    int size = 0;

    while (true) {
        ch = fgetc(f);
        if (feof(f)) {
            break;
        }
        size += 1;
    }

    char bytes[size];
    char clearbytes[size];
    fseek(f, 0, SEEK_SET);
    fread(&bytes,size,1,f);
    fclose(f);
    /*===============================|+END+|======================================*/

    init_ncurses();
    clear_bytes(bytes,clearbytes);
    dbg = fopen("../debug.txt","w");
    fwrite(&bytes,size+337,1,dbg);
    do {

        switch (ch = getch()) {
            case 's':
                line = 0;

                for (int i = 0; i < 48; i++) {
                    print_file(bytes,clearbytes);
                    fwrite(&bytes[OFFSET+i],1,1,dbg);
                    OFFSET+=16;
                }
                break;
            case 'w':
                line = 0;

                for (int i = 0; i < 48; i++) {
                    print_file(bytes,clearbytes);
                    OFFSET -= 16;
                }
                break;
            case 'q':
                flag = false;
                break;
            default:
                flag = false;
                break;
        }

    } while(flag);

    endwin();
    return 0;
}

void clear_bytes(char* bytes, char* clearbytes){
    for(int i =0;i<sizeof(bytes);i++){
        if(bytes[i] == '\n') clearbytes[i] = ' ';
        else clearbytes[i] = bytes[i];
    }
}
int init_ncurses(){
    if( NULL == initscr()) {
        return __LINE__;
    }
    noecho();
    curs_set(0);
    keypad(stdscr,TRUE);
    return 1;
}