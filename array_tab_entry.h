#pragma once
#include "define.h"
#include "tab_entry.h"

using namespace std;

class array_tab_entry
{
public:
    array_tab_entry(void);
    ~array_tab_entry(void);
    types elementType;
    int size;
    int elementSize;//ÿ��Ԫ��ռ�ռ�Ĵ�С

};

