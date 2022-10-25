#include <iostream>
#include <cstdio>
#include <vector>
#include <ncurses.h>

using namespace std;

char bytes[16];
FILE* f;


void printar(){
    printf("%X.%X.%X.%X %X.%X.%X.%X %X.%X.%X.%X %X.%X.%X.%X || %c%c%c%c   %c%c%c%c  %c%c%c%c  %c%c%c%c \n",
           bytes[0],bytes[1],bytes[2],bytes[3],bytes[4],bytes[5],bytes[6],bytes[7],bytes[8],bytes[9],bytes[10],bytes[11],bytes[12],bytes[13],bytes[14],bytes[15],
           bytes[0],bytes[1],bytes[2],bytes[3],bytes[4],bytes[5],bytes[6],bytes[7],bytes[8],bytes[9],bytes[10],bytes[11],bytes[12],bytes[13],bytes[14],bytes[15]);
}
void cclear(){
    for(int i = 0;i<16;i++){
        if(bytes[i] == '\n'){
            bytes[i] = NULL;
        }
    }
}
int main(int argc,char **argv) {
    char *namefile = argv[1];
    char ch = 0;
    int size = 0;
    if (argc < 2) {
        printf("Usage: %s name_of_file", argv[0]);
        return -1;
    }
    f = fopen(argv[1], "rb");
    if (f == NULL) {
        fprintf(stderr, "Couldn't open %s: %s\n", argv[1], strerror(errno));
        exit(1);
    }
    fseek(f, 0, SEEK_SET);
    while(1)
    {
        ch = fgetc(f);
        if(feof(f)){
            break;
        }
        size += 1;
    }
    fseek(f, 0, SEEK_SET);
    for(int j = 0;j<size/16+(size%16);j++){
        fread(&bytes, 1, 16, f);
        cclear();
        printar();
    }

    fclose(f);
    return 0;
}


