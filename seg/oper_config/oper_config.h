/*
 * author: dengzhaoqun
 * date	 : 2011/07/23
 */

#ifndef OPER_CONFIG_H_
#define OPER_CONFIG_H_

#define MAX_LINE_LEN 210

char *read_config(FILE *fp, char *key);
int add_config(FILE *fp, char *key, char *value);
int del_config(FILE *fp, char *key);
int alter_config(FILE *fp, char *key, char *new_value);

#endif
