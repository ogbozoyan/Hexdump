#include <iostream>
#include <cstdio>
#include <ncurses.h>

using namespace std;

FILE* f;
fpos_t offst;
char bytes[16];
char clearbytes[16];
int OFFSET = 0;
char ch,key = 0;
bool flag = true;
bool eof = true;
int line = 1;

void clear_bytes(){
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

void print_file() {
    fread(&bytes, 1, 16, f);
    clear_bytes();
    if (line % 49 == 0) {
        clear();
    }
    if (!feof(f)){
        mvprintw(line % 49, 0, "%#010x || %x%x%x%x %x%x%x%x %x%x%x%x %x%x%x%x || %c%c%c%c   %c%c%c%c  %c%c%c%c  %c%c%c%c \n",
                 OFFSET, bytes[0], bytes[1], bytes[2], bytes[3], bytes[4], bytes[5], bytes[6], bytes[7], bytes[8],
                 bytes[9], bytes[10], bytes[11], bytes[12], bytes[13], bytes[14], bytes[15],
                 clearbytes[0], clearbytes[1], clearbytes[2], clearbytes[3], clearbytes[4], clearbytes[5],
                 clearbytes[6], clearbytes[7], clearbytes[8], clearbytes[9], clearbytes[10],
                 clearbytes[11], clearbytes[12], clearbytes[13], clearbytes[14], clearbytes[15]);
        refresh();
        line++;
        OFFSET+=16;
    }
    else{
        fread(&bytes, 1, 16, f);
        clear_bytes();
        mvprintw(line % 49, 0, "%#010x || %x%x%x%x %x%x%x%x %x%x%x%x %x%x%x%x || %c%c%c%c   %c%c%c%c  %c%c%c%c  %c%c%c%c \n",
                 OFFSET, bytes[0], bytes[1], bytes[2], bytes[3], bytes[4], bytes[5], bytes[6], bytes[7], bytes[8],
                 bytes[9], bytes[10], bytes[11], bytes[12], bytes[13], bytes[14], bytes[15],
                 clearbytes[0], clearbytes[1], clearbytes[2], clearbytes[3], clearbytes[4], clearbytes[5],
                 clearbytes[6], clearbytes[7], clearbytes[8], clearbytes[9], clearbytes[10],
                 clearbytes[11], clearbytes[12], clearbytes[13], clearbytes[14], clearbytes[15]);
        refresh();
        line++;
        OFFSET+=16;
    }
}

int main(int argc,char **argv) {

    char *namefile = argv[1];

    if (argc < 2) {
        printf("Usage: %s name_of_file", argv[0]);
        return -1;
    }

    f = fopen(argv[1], "rb");
    if (f == NULL) {
        fprintf(stderr, "Couldn't open %s: %s\n", argv[1], strerror(errno));
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

    fseek(f, 0, SEEK_SET);
    init_ncurses();

    do {
        switch (key = getch()) {
            case 's':
                if (eof)
                    line = 0;
                    for (int i = 0; i < 48; i++) {
                        print_file();
                    }
                break;
            case 'w':
                if (eof){
                    line = 0;

                    fgetpos(f,&offst);
                    fsetpos(f,&offst-48);
                for (int i = 0; i < 48; i++) {
                    print_file();
                    OFFSET -= 16;
                }
                    }
                break;
                case 'q':
                    flag = false;
                break;
                default:
                    flag = false;
                break;
            }
    }while(flag);

    endwin();
    fclose(f);
    return 0;
}


