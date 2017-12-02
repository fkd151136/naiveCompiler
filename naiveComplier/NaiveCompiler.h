#pragma once
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <set>
#include <tchar.h>
#include <string.h>
#include <stdint.h>
#include "define.h"
#include "array_tab_entry.h"
#include "tab_entry.h"
#include "symbol_record.h"
#include "one_mid_code.h"
#include "mips_code.h"

using namespace std;

class NaiveCompiler
{
public:
    NaiveCompiler();
    ~NaiveCompiler();
    //��ǰ�鿴�ķ��Ż�������Ŀǰ��֪Ϊ�������ǰ�鿴3�����š����insymbol��������չ���������ֺ�����
    //�ڲ���Ҫ��ǰ�鿴��ʱ��ֻʹ��symbols[0],��ǰ�鿴���Ժ�����ֶ�����clear��������������ݡ�
    symbol_record symbols[3];
    int end;//����βָ�룬ά�����Ż���������ʹ��,����������Ϊ��[0, end]Ϊ������symbols�������end��Ϊ0ʱ����ǰsymbol��ָ����ȡ��endΪ0ʱ����ǰsymbol��0��ȡ
    char ch;//��Դ�ļ��ж��������һ���ַ�
    char key[NKW][ALNG];//����������
    int cc; //charactor counter
    int ll; //length of current line
    char line[LLNG];
    fstream fin;
    fstream fout;
    fstream midcodefile;
    fstream programma_info;
    fstream mipsfile;
    symbol ksy[NKW];
    tab_entry tab[TABLE_SIZE];
    int t;//tab������
    array_tab_entry atab[ARRAY_TABLE_SIZE];
    int a;//atab������
    int display[LMAX];
    bool iflag, oflag;//scanf printf �õ��ı��
    int dx;//data alloction index ��Զָ������������һ�����ݵ�ַ,��ȫ�ֱ���ʹ��ʱ����ʾ���Ե�ַ�����ֲ�����ʹ��ʱ����ʾ����ں�����ʼ��ַ��ƫ��
    int ln;//�кţ�������������С�

    char string_global[ALNG];
    char string_main[ALNG];
    int mid_counter;
    one_mid_code midcode[MID_CODE_SIZE];
    int f;

    mips_code mips[MIPS_CODE_SIZE];
    int mips_counter;

    list<pair<int, string>> errors;

    set<symbol> f_assign;
    set<symbol> f_call_return_func;
    set<symbol> f_call_void_func;
    set<symbol> f_case;
    set<symbol> f_caselist;
    set<symbol> f_condition;
    set<symbol> f_const;
    set<symbol> f_const_declare;
    set<symbol> f_const_define;
    set<symbol> f_do_while;
    set<symbol> f_expression;
    set<symbol> f_factor;
    set<symbol> f_if;
    set<symbol> f_integer;
    set<symbol> f_item;
    set<symbol> f_main;
    set<symbol> f_printf;
    set<symbol> f_program;
    set<symbol> f_return;
    set<symbol> f_return_func_declare;
    set<symbol> f_scanf;
    set<symbol> f_statement;
    set<symbol> f_switch;
    set<symbol> f_var_declare;
    set<symbol> f_var_define;
    set<symbol> f_void_func_declare;
    
    void assign_statement(char (&parent)[ALNG]);//������ֵ��䡱
    void call_return_func(char (&parent)[ALNG], char (&rd)[ALNG], types (&typ));//�����з���ֵ����������䡱
    void call_void_func(char (&parent)[ALNG]);//�����޷���ֵ����������䡱
    void caselist(char (&parent)[ALNG], types typ, char (&rs)[ALNG]);//���������
    void compound_statement(char (&parent)[ALNG], bool &has_return);//����������䡱
    void condition(char (&parent)[ALNG], char (&label)[ALNG], bool jump);//����������,jump��ʾ����������ת����������������ת
    int constant(types typ);//������������������case�����
    void const_declare(char (&parent)[ALNG]);//��������������
    void const_define(char (&parent)[ALNG]);//�����������塱
    void do_while_statement(char (&parent)[ALNG]);//����ѭ����䡱
    void expression(char (&parent)[ALNG], char (&rd)[ALNG], types (&typ));//�������ʽ��
    void factor(char (&parent)[ALNG], char (&rd)[ALNG], types (&typ));//�������ӡ�
    void if_statement(char (&parent)[ALNG]);//����������䡱
    void insymbol(symbol_record &aim);//�����������ݱ��浽aim��
    int integer();//����������
    void item(char (&parent)[ALNG], char (&rd)[ALNG], types (&typ));//�����
    void nextch();//��Դ�ļ���ȡ��һ���ַ�
    void one_case(char (&parent)[ALNG], types typ, char (&end)[ALNG], char (&rs)[ALNG], set<int> &exist_const);//�����������䡱����1��case���
    void parameter_list(char (&parent)[ALNG]);//����������
    void printf_statement(char (&parent)[ALNG]);//����д��䡱
    void program();//��������
    void return_func_declare();//�����з���ֵ�������塱,��Ҫ����Ԥ�ȶ���3���ַ���Ԥ�ȶ���1���ַ��������������
    void return_statement(char (&parent)[ALNG]);//����������䡱
    void scanf_statement(char (&parent)[ALNG]);//��������䡱
    void statement(char (&parent)[ALNG], bool &has_return);//������䡱
    void statement_column(char (&parent)[ALNG], bool &has_return);//��������С�
    void switch_statement(char (&parent)[ALNG]);//���������䡱
    void value_parameter_list(char (&parent)[ALNG], char (&called_func)[ALNG]);//����ֵ������
    void var_declare(char (&parent)[ALNG]);//��������������
    void var_define(char (&parent)[ALNG]);//�����������塱
    void void_func_declare();//�����޷���ֵ�������塱
    void void_main();//������������

    void error(string content);//ln�����������к�      content����������
    void errormsg();//���д�ӡ������Ϣ
    void fatal(string content);//���ش��󣬽����ֱ���˳�
    //���ݱ�ʶ�����Ʋ��ң����ر�ʶ������
    int getIndexByNameAndParent(char (&parent)[ALNG], char (&name)[ALNG], operate_table_mode mode);
    void skip(set<symbol> begin_symbol_set);
    void append_midcode(char* op, char* rd = nullptr, char* rs = nullptr, char* rt = nullptr);//����м����
    void allocHelpVar(char* ans);//����һ�����еı�������������Ԫʽ������

    void midcode2mips();

    void add(char* rd, char* rs, char* rt);
    void addi(char* rd, char* rs, int imm);
    void div(char* rs, char* rt);
    void mult(char* rs, char* rt);
    void sub(char* rd, char* rs, char* rt);
    void subi(char* rd, char* rs, int imm);
    void syscall();
    
};
