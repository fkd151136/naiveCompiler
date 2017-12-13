#pragma once
#define NKW 13//keyword����
#define ALNG    26//identfier����Ч�ַ�����
#define LLNG    320//�����еĳ���
#define LMAX    7//level max ������
#define TABLE_SIZE  300//���ű��С�������ɵķ�������
#define ARRAY_TABLE_SIZE    100//������С�������ɶ�������
#define MID_CODE_SIZE   2000//�м��������ģ
#define MIPS_CODE_SIZE  4000//mips��������ģ


#define str_char "char"
#define str_int "int"
#define str_prt "prt"
#define str_scf "scf"
#define str_func_def "func_def"
#define str_para "para"
#define str_push "push"
#define str_call "call"
#define str_BNZ "BNZ"
#define str_BZ "BZ"
#define str_ret "ret"
#define str_GOTO "GOTO"

#define reg_$0 "$0"
#define reg_v0 "$v0"
#define reg_v1 "$v1"
#define reg_a0 "$a0"
#define reg_a1 "$a1"
#define reg_a2 "$a2"
#define reg_a3 "$a3"
#define reg_t0 "$t0"
#define reg_t1 "$t1"
#define reg_t2 "$t2"
#define reg_t3 "$t3"
#define reg_t4 "$t4"
#define reg_t5 "$t5"
#define reg_t6 "$t6"
#define reg_t7 "$t7"
#define reg_ra "$ra"
#define reg_sp "$sp"//����ջ
#define reg_fp "$fp"//����ջ
#define reg_gp "$gp"//����ջ
#define reg_t9 "$t9"//ȫ�ֱ������׵�ַ
#define reg_lo "lo"

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

//dagͼ�Ľڵ���ֻ�������������ӽڵ㣬ÿ�������Ӷ��˽ڵ��б������кü������˽ڵ㣩��ʼ����
struct DAG_point{
    char value[ALNG];
    struct DAG_point* left_num;
    struct DAG_point* right_num;
    struct DAG_point* next;//���ڶ��˽ڵ��б�Ѱ����һ���ڵ�
};
