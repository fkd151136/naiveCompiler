#pragma once
#define NKW 13//keyword����
#define ALNG    28//identfier����Ч�ַ�����
#define LLNG    320//�����еĳ���
#define TABLE_SIZE  400//���ű��С�������ɵķ�������
#define ARRAY_TABLE_SIZE    50//������С�������ɶ�������
#define FUNC_TABLE_SIZE     30//�������С�������ɶ��ٺ���
#define MID_CODE_SIZE   2000//�м��������ģ
#define MIPS_CODE_SIZE  4000//mips��������ģ
#define MAX_DAG_POINT   100//DAGͼ�ڵ�����ģ
#define REG_NAME_SIZE   4//�Ĵ���������
#define MAX_VAR_IN_FUNC 100//�����б��������Ŀ
#define GLOABL_REG_NUMBER   15//ȫ�ּĴ�����Ŀ
#define RESERVE_SPACE   (3 + GLOABL_REG_NUMBER) * 4//����Ԥ���ռ�
#define STRING_TABLE_SIZE   300//������С

#define str_char "char"
#define str_int "int"
#define str_string "string"
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
//�����������м�����mips��๲��
#define str_label "label"
#define str_nop "nop"

#define str_add     "add"
#define str_addi    "addi"
#define str_and     "and"
#define str_andi    "andi"
#define str_beq     "beq"
#define str_bne     "bne"
#define str_div     "div"
#define str_j       "j"
#define str_jal     "jal"
#define str_jr      "jr"
#define str_la      "la"
#define str_lui     "lui"
#define str_lw      "lw"
#define str_mflo    "mflo"
#define str_mult    "mult"
#define str_or      "or"
#define str_ori     "ori"
#define str_nor     "nor"
#define str_sll     "sll"
#define str_slt     "slt"
#define str_sltu    "sltu"
#define str_slti    "slti"
#define str_sltiu   "sltiu"
#define str_sub     "sub"
#define str_subu    "subu"
#define str_sw      "sw"
#define str_syscall "syscall"

#define reg_$0 "$0"
#define reg_$1 "$1"
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
#define reg_t8 "$t8"
#define reg_s0 "$s0"
#define reg_s1 "$s1"
#define reg_s2 "$s2"
#define reg_s3 "$s3"
#define reg_s4 "$s4"
#define reg_s5 "$s5"
#define reg_s6 "$s6"
#define reg_s7 "$s7"
#define reg_ra "$ra"
#define reg_sp "$sp"//����ջ
#define reg_fp "$fp"//����ջ
#define reg_gp "$gp"//����ջ
#define reg_t9 "$t9"//ȫ�ֱ������׵�ַ
#define reg_lo "lo"
#define reg_k0 "$k0"
#define reg_k1 "$k1"
#define sa_base 6
#define rd_base 11
#define rt_base 16
#define rs_base 21
#define base_base 21
#define op_base 26

#define e1 "�м�������������"
#define e2 "��ʶ��������"
#define e3 "����ֵ�����ֵ�����޸�"
#define e4 "���Ͳ���"
#define e5 "��⵽��������Ϊ�ַ��ͣ���������ӦΪ���ͣ������Զ�ת��"
#define e6 "��������Խ��"
#define e7 "���ű���������"
#define e8 "�﷨�ɷ�ȱʧ"
#define e9 "��/�������������ں�����ͬ"
#define e10 "���Ҳ�����ʶ��"
#define e11 "�޷���������0��ͷ"
#define e12 "����Խ��"
#define e13 "����ֵ�ظ�"
#define e14 "����������ƥ��"

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


