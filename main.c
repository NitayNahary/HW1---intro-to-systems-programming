#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include "RLEList.h"
#include "AsciiArtTool.h"

#define TARGET_INDEX_MOD 1
#define SOURCE_INDEX_MOD 2
#define ARGC_MIN 3
#define ARGC_MAX 5
#define FLAG_I_INDEX 0
#define FLAG_I "-i"
#define FLAG_E_INDEX 1
#define FLAG_E "-e"
#define FLAG_NUM 2
#define READ "r"
#define WRITE "w"

char invertedPic(char value){
    if(value == ' '){
        return '@';
    }else if(value == '@'){
        return ' ';
    }
    return value;
}

int setAction(bool action[FLAG_NUM], char* flag){
    if(strcmp(FLAG_I, flag) == 0){
        action[FLAG_I_INDEX] = true;
        return 0;
    }else if(strcmp(FLAG_E, flag) == 0){
        action[FLAG_E_INDEX] = true;
        return 0;
    }else{
        fprintf(stderr, "UNEXPECTED_FLAG");
        return -1;
    }
}

void executeAction(bool action[FLAG_NUM],RLEList list,FILE* target, RLEListResult* result){
    if(*result == RLE_LIST_SUCCESS && action[FLAG_I_INDEX]){
        *result = RLEListMap(list,invertedPic);
    }
    if(*result == RLE_LIST_SUCCESS && action[FLAG_E_INDEX]){
        *result = asciiArtPrintEncoded(list,target);
    }else{
        *result = asciiArtPrint(list, target);
    }
}

RLEList initializeList(char* filePath){
    FILE* source = fopen(filePath, READ);
    if(source == NULL) {
        fprintf(stderr, "ERROR_NULL_SOURCE_FILE");
        return NULL;
    }
    RLEList list = asciiArtRead(source);
    fclose(source);
    return list;
}

int main(int argc, char** argv){
    if(argc < ARGC_MIN || argc > ARGC_MAX){
        return 0;
    }

    int fileIndex = argc - SOURCE_INDEX_MOD;
    RLEList list = initializeList(argv[fileIndex]);
    if(list == NULL){
        return 0;
    }
    fileIndex = argc - TARGET_INDEX_MOD;
    FILE* target = fopen(argv[fileIndex], WRITE);

    bool action[FLAG_NUM] = {false};
    int flagsMaxIndex = argc - SOURCE_INDEX_MOD;
    for(int i = 1; i < flagsMaxIndex; i++) {
        if(setAction(action, argv[i]) == -1){
            return 0;
        }
    }
    RLEListResult result = RLE_LIST_SUCCESS;
    executeAction(action,list,target,&result);

    RLEListDestroy(list);
    fclose(target);
}