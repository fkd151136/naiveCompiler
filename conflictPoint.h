#pragma once
#include "define.h"
#include <set>

using namespace std;

class conflictPoint
{
public:
    conflictPoint(void);
    ~conflictPoint(void);
    void initializer();
    
    char name[ALNG];
    int number;//�����ں����е����
    int reg_number;//�Ĵ������
    //�Ĵ�����ţ�0-14��Ӧ$s0-$s7,$a1-$a3,$t5-t8
    bool enable;
    int weight;//Ȩ�أ����������ɾ����˳��
    set<int> neighbours;
};

