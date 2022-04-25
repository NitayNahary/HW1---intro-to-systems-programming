#include <stdio.h>
#include <string.h>
#include "RLEList.h"
#include "AsciiArtTool.h"

RLEList asciiArtRead(FILE* in_stream){
    if(in_stream == NULL){
        return NULL;
    }
    char buffer[CHUNK_SIZE] = {'\0'};
    RLEList list = RLEListCreate();
    if(list == NULL){
        return NULL;
    }
    while(fgets(buffer,CHUNK_SIZE,in_stream) != NULL) {
        for (int i = 0; buffer[i]; i++) {
            if(RLEListAppend(list, buffer[i]) != RLE_LIST_SUCCESS){
                RLEListDestroy(list);
                return NULL;
            }
            buffer[i] = '\0';
        }
    }
    return list;
}

RLEListResult asciiArtPrint(RLEList list,FILE* out_stream){
    if(list == NULL || out_stream == NULL){
        return RLE_LIST_NULL_ARGUMENT;
    }
    RLEListResult result = RLE_LIST_SUCCESS;
    char buffer[CHUNK_SIZE] = {'\0'};
    int textLength = CHUNK_SIZE - 1;
    int  bufferIndex = 0;
    for(int listIndex = 0; listIndex <  RLEListSize(list); listIndex++ , bufferIndex++) {
        if(bufferIndex == textLength ){
            fputs(buffer, out_stream);
            bufferIndex = 0;
        }
        buffer[bufferIndex] = RLEListGet(list, listIndex, &result);
        if(result != RLE_LIST_SUCCESS){
            return result;
        }
    }
    buffer[bufferIndex] = '\0';
    fputs(buffer, out_stream);
    return RLE_LIST_SUCCESS;
}

RLEListResult asciiArtPrintEncoded(RLEList list,FILE* out_stream){
    if(list == NULL || out_stream == NULL){
        return RLE_LIST_NULL_ARGUMENT;
    }
    char buffer[CHUNK_SIZE] = {'\0'};
    RLEListResult result = RLE_LIST_SUCCESS;
    char* encodedArt = RLEListExportToString(list, &result);
    if(result != RLE_LIST_SUCCESS) {
        return result;
    }
    int encodedArtLen = strlen(encodedArt);
    int textLength = CHUNK_SIZE - 1;
    while(CHUNK_SIZE < encodedArtLen) {
        memcpy(buffer, encodedArt, textLength);
        encodedArt += textLength;
        encodedArtLen -= textLength;
        fputs(buffer, out_stream);
    }
    buffer[encodedArtLen] = '\0';
    memcpy(buffer, encodedArt, encodedArtLen);
    fputs(buffer, out_stream);
    return RLE_LIST_SUCCESS;
}