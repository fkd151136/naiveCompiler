#pragma once
#define NKW 13//keyword����
#define ALNG    51//identfier����Ч�ַ�����
#define LLNG    501//�����еĳ���
#define LMAX    7//level max ������
#define TABLE_SIZE  300//���ű��С�������ɵķ�������
#define ARRAY_TABLE_SIZE    100//������С�������ɶ�������
#define MID_CODE_SIZE   3000//�м��������ģ
#define MIPS_CODE_SIZE  3000//mips��������ģ

enum symbol
{
constsy,intsy,charsy,voidsy,mainsy,ifsy,dosy,whilesy,switchsy,casesy,printfsy,scanfsy,returnsy, 
//+			-		*		/	==	  !=   >	>=   <   <=	   ,        ;        =     �Ƿ����� 
plussy, minussy, multsy, divsy, beq, bne, bgt, bge, blt, ble, comma, semicomma, becomesy, nosy,
//int     char     string     (         )        [       ]		  {        }    ��ʶ��  ð��
intcon, charcon, stringcon, lparent, rparent, lsquare, rsquare, lbrack, rbrack, ident, colon
};

enum objecttyp{con, var, para, func, arr};
enum types{notyp, ints, bools, chars, arrays};
enum operate_table_mode{declare_mode, search_mode};
// declare_mode ���ģʽ��������Ҫ��ͬһ�㲻��ͻ����
// SEARCH_MODE  ���ģʽ�����������ڱ����ң��Ҳ�����ȥ��һ����

