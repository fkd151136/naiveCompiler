#pragma once
#include "define.h"

using namespace std;

class func_tab_entry
{
public:
    func_tab_entry(void);
    ~func_tab_entry(void);
    int parameter_number;//�βθ���
    int level;
    symbol return_type;
    //���ܻ�Ҫ��¼�ֲ�������ռ�ռ��С
};

