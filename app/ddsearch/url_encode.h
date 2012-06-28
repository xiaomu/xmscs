#ifndef _URL_ENCODE_H
#define _URL_ENCODE_H

int URLEncode(const char* str, const int strSize, char* result, const int resultSize);
int URLDecode(const char* str, const int strSize, char* result, const int resultSize);

#endif