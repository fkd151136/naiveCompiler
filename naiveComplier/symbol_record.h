#pragma once
#include "define.h"
#include <string.h>

using namespace std;

class symbol_record
{
public:
    symbol_record(void);
    ~symbol_record(void);
    long long inum;//insymbol�����õ�������ֵ
    char chval;//insymbol�����������ַ��ͳ�����ֵ
    int sleng;//�ַ�������
    char id[ALNG];
    symbol sy;//insymbol�����õ������һ������
    char strval[600];

    void clear();
    void copy(symbol_record (&dest));//����symbol_record�����ݸ��Ƶ�dest��
};

