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
    int value;//����Ǻ������ͣ��򱣴溯����Ҫ���ڴ�ռ�
    int ref;//����Ǻ������ͣ��򱣴溯����������
    int address;//�������䵥Ԫ���׵�ַ,Ҳ��ʾ��Ҫռ�õĿռ��С
};

