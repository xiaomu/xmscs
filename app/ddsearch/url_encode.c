#include <stdio.h>

#define NON_NUM '0' 

char Char2Num(char ch){ 
	if(ch>='0' && ch<='9')return (char)(ch-'0'); 
	if(ch>='a' && ch<='f')return (char)(ch-'a'+10); 
	if(ch>='A' && ch<='F')return (char)(ch-'A'+10); 
	return NON_NUM; 
} 

/************************************************ 
* ���ַ�������URL���롣 
* ���룺 
* str: Ҫ������ַ��� 
* strSize: �ַ����ĳ��ȡ�����str�п����Ƕ��������� 
* result: ����������ĵ�ַ 
* resultSize:�����ַ�Ļ�������С(���str�����ַ������룬��ֵΪstrSize*3) 
* ����ֵ�� 
* >0: result��ʵ����Ч���ַ����ȣ� 
* 0: ����ʧ�ܣ�ԭ���ǽ��������result�ĳ���̫С 
************************************************/ 
int URLEncode(const char* str, const int strSize, char* result, const int resultSize) { 
	int i; 
	int j = 0; /* for result index */ 
	char ch; 

	if ((str == NULL) || (result == NULL) || (strSize <= 0) || (resultSize <= 0)) { 
		return 0; 
	} 

	for (i=0; (i<strSize) && (j<resultSize); i++) { 
		ch = str[i]; 
		if ((ch >= 'A') && (ch <= 'Z')) { 
			result[j++] = ch; 
		} else if ((ch >= 'a') && (ch <= 'z')) { 
			result[j++] = ch; 
		} else if ((ch >= '0') && (ch <= '9')) { 
			result[j++] = ch; 
		} else if(ch == ' '){ 
			result[j++] = '+'; 
		} else { 
			if (j + 3 < resultSize) { 
				sprintf(result+j, "%%%02X", (unsigned char)ch); 
				j += 3; 
			} else { 
				return 0; 
			} 
		} 
	} 

	result[j] = '\0'; 
	return j; 
} 


/************************************************ 
* ���ַ�������URL���롣 
* ���룺 
* str: Ҫ������ַ��� 
* strSize: �ַ����ĳ��ȡ� 
* result: ����������ĵ�ַ 
* resultSize:�����ַ�Ļ�������С������<=strSize 
* ����ֵ�� 
* >0: result��ʵ����Ч���ַ����ȣ� 
* 0: ����ʧ�ܣ�ԭ���ǽ��������result�ĳ���̫С 
************************************************/ 
int URLDecode(const char* str, const int strSize, char* result, const int resultSize) { 
	char ch, ch1, ch2; 
	int i; 
	int j = 0; /* for result index */ 

	if ((str == NULL) || (result == NULL) || (strSize <= 0) || (resultSize <= 0)) { 
		return 0; 
	} 

	for (i=0; (i<strSize) && (j<resultSize); i++) { 
		ch = str[i]; 
		switch (ch) { 
		case '+': 
			result[j++] = ' '; 
			break; 

		case '%': 
			if (i+2 < strSize) { 
				ch1 = Char2Num(str[i+1]); 
				ch2 = Char2Num(str[i+2]); 
				if ((ch1 != NON_NUM) && (ch2 != NON_NUM)) { 
					result[j++] = (char)((ch1<<4) | ch2); 

					i += 2; 
					break; 
				} 
			} 

			/* goto default */ 
		default: 
			result[j++] = ch; 
			break; 
		} 
	} 

	result[j] = '\0'; 
	return j; 
} 