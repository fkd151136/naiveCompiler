#pragma once
#include "define.h"

using namespace std;

class tab_entry
{
public:
    tab_entry(void);
    ~tab_entry(void);
    char name[ALNG];
    char parent[ALNG];//��־�������ĺ�����ȫ�ֵľ�дglobal
    objecttyp obj;//����
    types typ;//����
    int value;
    int ref;
    int address;//�������䵥Ԫ���׵�ַ,Ҳ��ʾ��Ҫռ�õĿռ��С
};

