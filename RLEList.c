#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "RLEList.h"

#define BUFFER_SIZE 20

#define SET_RESULT(value, toIgnore) \
if(!toIgnore){                      \
*result = value;                    \
}


 struct RLEList_t{
    char rleChar;
    int repetition;
    struct RLEList_t* next;
};

// ----------------------------------------- Static Functions Decleration -------------------------------------

static void writeBufferStr(RLEList list, char buffer[BUFFER_SIZE]);

static char* createEncodedString(RLEList list, char* buffer, char* dest, int size, RLEListResult* result);

static void writeRLEString(RLEList list,char* buffer, char* dest);

static RLEList RLEListInitializer(char value, RLEListResult* result);

static void RLEListMerge(RLEList list);

//------------------------------------------ Functions --------------------------------------------

RLEList RLEListCreate(){
    RLEList new_list = malloc(sizeof(*new_list));
    if(!new_list){
        return NULL;
    }
    new_list->rleChar = 0;
    new_list->repetition = 0;
    new_list->next = NULL;
    return new_list;
}

void RLEListDestroy(RLEList list){
    if(list){
        RLEListDestroy(list->next);
        free(list);
    }
}

RLEListResult RLEListAppend(RLEList list, char value){
    RLEListResult result = RLE_LIST_SUCCESS;
    if(list == NULL || value == 0){
        return RLE_LIST_NULL_ARGUMENT;
    }
    if(list->next == NULL) {
        if (list->rleChar == value) {
            list->repetition++;
            return RLE_LIST_SUCCESS;
        }
        list->next = RLEListInitializer(value, &result);
        return result;
    }
    return RLEListAppend(list->next,value);
}

int RLEListSize(RLEList list){
    if(list == NULL) {
        return -1;
    }
    if(list->next == NULL){
        return list->repetition;
    }
    return (list->repetition+RLEListSize(list->next));
}

RLEListResult RLEListRemove(RLEList list, int index){
    if(list == NULL) {
        return RLE_LIST_NULL_ARGUMENT;
    }
    if(index < 0 || index >= RLEListSize(list)) {
        return RLE_LIST_INDEX_OUT_OF_BOUNDS;
    }
    int collectRepetitions = 0;
    RLEList backList;
    while(collectRepetitions <= index) {
        backList = list;
        list = list->next;
        collectRepetitions += list->repetition;
    }
    if(list->repetition == 1) {
        backList->next = list->next;
        free(list);
        RLEListMerge(backList);
    }else{
        list->repetition--;
    }
    return RLE_LIST_SUCCESS;
}

char RLEListGet(RLEList list, int index, RLEListResult *result){
    bool defaultResult = (result == NULL);
    if(list == NULL){
        SET_RESULT(RLE_LIST_NULL_ARGUMENT,defaultResult)
        return 0;
    }
    if(index < 0 || index >= RLEListSize(list)) {
        SET_RESULT(RLE_LIST_INDEX_OUT_OF_BOUNDS,defaultResult)
        return 0;
    }
    int collectRepetitions = 0;
    while(collectRepetitions <= index) {
        list = list->next;
        collectRepetitions += list->repetition;
    }
    SET_RESULT(RLE_LIST_SUCCESS,defaultResult)
    return list->rleChar;
}

char* RLEListExportToString(RLEList list, RLEListResult* result){
    bool defaultResult = (result == NULL);
    if(list == NULL){
        SET_RESULT(RLE_LIST_NULL_ARGUMENT,defaultResult)
        return NULL;
    }
    list = list->next;
    char buffer[BUFFER_SIZE] = {0};
    RLEListResult tempResult = RLE_LIST_SUCCESS;
    char* dest = createEncodedString(list, buffer, NULL, 0, &tempResult);
    if(dest){
        writeRLEString(list,buffer,dest);
    }
    SET_RESULT(tempResult,defaultResult)
    return dest;
}

RLEListResult RLEListMap(RLEList list, MapFunction map_function){
    if(!list || !map_function){
        return RLE_LIST_NULL_ARGUMENT;
    }
    list = list->next;
    RLEList mapList = list;
    while(mapList){
        mapList->rleChar = map_function(mapList->rleChar);
        mapList = mapList->next;
    }
    while(list){
        RLEListMerge(list);
        list = list->next;
    }
    return RLE_LIST_SUCCESS;
}

// ----------------------------------------- Static Functions -------------------------------------

static void writeBufferStr(RLEList list, char buffer[BUFFER_SIZE]){
    assert(list);
    assert(buffer);
    buffer[0] = list->rleChar;
    for(int i = 1; i < strlen(buffer); i++){
        buffer[i] = 0;
    }
    sprintf(buffer + 1, "%d", list->repetition);
    strcat(buffer, "\n");
}

static char* createEncodedString(RLEList list, char* buffer, char* dest, int size, RLEListResult* result){
    if(!list) {
        return "";
    }
    writeBufferStr(list, buffer);
    size += strlen(buffer);
    dest = createEncodedString(list->next, buffer, dest, size, result);
    if (list->next == NULL){
        dest = (char *) malloc(sizeof(char)*(size + 1));
        if (dest == NULL) {
            *result = RLE_LIST_OUT_OF_MEMORY;
        }
    }
    return dest;
}

static void writeRLEString(RLEList list,char* buffer, char* dest){
    if(!list) {
        return;
    }
    writeBufferStr(list, buffer);
    strcpy(dest,buffer);
    writeRLEString(list->next, buffer, dest+strlen(buffer));
}

static RLEList RLEListInitializer(char value, RLEListResult* result){
    assert(value);
    assert(result);
    RLEList list = RLEListCreate();
    if(list == NULL){
        *result = RLE_LIST_OUT_OF_MEMORY;
        return NULL;
    }
    list->rleChar = value;
    list->repetition = 1;
    *result = RLE_LIST_SUCCESS;
    return list;
}

static void RLEListMerge(RLEList list){
    if(list == NULL || list->next == NULL){
        return;
    }
    RLEList frontList = list->next;
    if(frontList->rleChar == list->rleChar){
        list->repetition += frontList->repetition;
        list->next = frontList->next;
        free(frontList);
    }
}
