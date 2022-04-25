#ifndef HW1_ASCIIARTTOOL_H
#define HW1_ASCIIARTTOOL_H

#include "RLEList.h"
#include <stdio.h>
#include <stdlib.h>

#define CHUNK_SIZE 256

/**
*   asciiArtRead: Reads the given file and compresses it in RLE method.
*
* @param in_stream - * FILE object that contains the image that needs to compress.
* @return
* 	An RLEList list that contains all the characters in the image.
* 	NULL if the in_stream is NULL or if an allocation failed
*/
RLEList asciiArtRead(FILE* in_stream);


/**
*   asciiArtPrint: writes an image represented by an RAEList list to a file.
*
* @param list - An RLEList list that contains all the characters in the image.
* @param out_stream - An * FILE object to which the image is written.
* @return
* 	RLE_LIST_NULL_ARGUMENT if a NULL was sent as one of the parameters.
* 	RLE_LIST_SUCCESS if the image written successfully.
*
*/
RLEListResult asciiArtPrint(RLEList list,FILE* out_stream);


/**
*   asciiArtPrintEncoded: writes the image to the file in a compressed form (in a format returned from RLEListExportToString).
*
* @param list - An RLEList list that contains all the characters in the image.
* @param out_stream - An * FILE object to which the image is written.
* @return
* 	RLE_LIST_NULL_ARGUMENT if a NULL was sent as one of the parameters.
* 	RLE_LIST_SUCCESS if the image written successfully.
*
*/
RLEListResult asciiArtPrintEncoded(RLEList list,FILE* out_stream);


#endif //HW1_ASCIIARTTOOL_H
