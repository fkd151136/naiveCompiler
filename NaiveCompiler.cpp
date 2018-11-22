#include "NaiveCompiler.h"

NaiveCompiler::NaiveCompiler()
{
    ch = ' ';
    cc = 0;
    ll = 0;
    memset(line, 0, sizeof(line));
    end = 0;
    t = -1;
    a = -1;
    f = -1;
    dx = 0;
    ln = 0;
    mid_counter = 0;
    mips_counter = -1;
    d = -1;
    s = -1;

    tab = (tab_entry* )malloc(TABLE_SIZE*sizeof(tab_entry));
    midcode = (one_mid_code* )malloc(MID_CODE_SIZE*sizeof(one_mid_code));
    mips = (mips_code* )malloc(MIPS_CODE_SIZE*sizeof(mips_code));
    stab = (str_tab_entry* )malloc(STRING_TABLE_SIZE*sizeof(str_tab_entry));
    memset(tab, 0, TABLE_SIZE*sizeof(tab_entry));
    memset(midcode, 0, MID_CODE_SIZE*sizeof(one_mid_code));
    memset(mips, 0, MIPS_CODE_SIZE*sizeof(mips_code));
    memset(stab, 0, STRING_TABLE_SIZE*sizeof(str_tab_entry));

    midcodefile.open("midcode.txt", ios_base::out);
    programma_info.open("programma_analysis.txt", ios_base::out);
    mipsfile.open("mips.asm", ios_base::out);
    mipsfile << ".data" << endl;

    strcpy_s(string_main, "main");
    strcpy_s(string_global, "global");

    fcount = -1;
    strcpy_s(key[0], "case");
    strcpy_s(key[1], "char");
    strcpy_s(key[2], "const");
    strcpy_s(key[3], "do");
    strcpy_s(key[4], "if");
    strcpy_s(key[5], "int");
    strcpy_s(key[6], "main");
    strcpy_s(key[7], "printf");
    strcpy_s(key[8], "return");
    strcpy_s(key[9], "scanf");
    strcpy_s(key[10],"switch");
    strcpy_s(key[11],"void");
    strcpy_s(key[12],"while");

    ksy[0]=casesy;
    ksy[1]=charsy;
    ksy[2]=constsy;
    ksy[3]=dosy;
    ksy[4]=ifsy;
    ksy[5]=intsy;
    ksy[6]=mainsy;
    ksy[7]=printfsy;
    ksy[8]=returnsy;
    ksy[9]=scanfsy;
    ksy[10]=switchsy;
    ksy[11]=voidsy;
    ksy[12]=whilesy;

    f_assign.insert(ident);

    f_call_return_func.insert(ident);

    f_call_void_func.insert(ident);

    f_case.insert(casesy);

    f_caselist.insert(casesy);

    f_condition.insert(plussy);
    f_condition.insert(minussy);
    f_condition.insert(ident);
    f_condition.insert(intcon);
    f_condition.insert(charcon);
    f_condition.insert(lparent);

    f_const.insert(plussy);
    f_const.insert(minussy);
    f_const.insert(intcon);
    f_const.insert(charcon);

    f_const_declare.insert(constsy);

    f_const_define.insert(intsy);
    f_const_define.insert(charsy);

    f_do_while.insert(dosy);

    f_expression.insert(plussy);
    f_expression.insert(minussy);
    f_expression.insert(ident);
    f_expression.insert(intcon);
    f_expression.insert(charcon);
    f_expression.insert(lparent);

    f_factor.insert(ident);
    f_factor.insert(plussy);
    f_factor.insert(minussy);
    f_factor.insert(intcon);
    f_factor.insert(charcon);
    f_factor.insert(lparent);

    f_if.insert(ifsy);

    f_integer.insert(plussy);
    f_integer.insert(minussy);
    f_integer.insert(intcon);

    f_item.insert(ident);
    f_item.insert(plussy);
    f_item.insert(minussy);
    f_item.insert(intcon);
    f_item.insert(charcon);
    f_item.insert(lparent);

    f_main.insert(voidsy);

    f_printf.insert(printfsy);

    f_program.insert(constsy);
    f_program.insert(intsy);
    f_program.insert(charsy);
    f_program.insert(voidsy);

    f_return.insert(returnsy);

    f_return_func_declare.insert(intsy);
    f_return_func_declare.insert(charsy);

    f_scanf.insert(scanfsy);

    f_statement.insert(ifsy);
    f_statement.insert(dosy);
    f_statement.insert(switchsy);
    f_statement.insert(lbrack);
    f_statement.insert(ident);
    f_statement.insert(scanfsy);
    f_statement.insert(printfsy);
    f_statement.insert(semicomma);
    f_statement.insert(returnsy);

    f_switch.insert(switchsy);

    f_var_declare.insert(intsy);
    f_var_declare.insert(charsy);

    f_var_define.insert(intsy);
    f_var_define.insert(charsy);

    f_void_func_declare.insert(voidsy);
}
NaiveCompiler::~NaiveCompiler()
{
}
void NaiveCompiler::allocHelpVar(char (&ans)[ALNG])
{
    fcount += 1;
    strcpy_s(ans, "fangkedongt");
    _itoa_s(fcount, &ans[11], ALNG-12, 10);
}
void NaiveCompiler::append_midcode(char* op, char* rd, char* rs, char* rt)
{
    if (mid_counter < MID_CODE_SIZE)
    {
        strcpy_s(midcode[mid_counter].op, op);
        if (rd != nullptr)
            strcpy_s(midcode[mid_counter].rd, rd);
        if (rs != nullptr)
            strcpy_s(midcode[mid_counter].rs, rs);
        if (rt != nullptr)
            strcpy_s(midcode[mid_counter].rt, rt);
        mid_counter += 1;
    }
    else
        fatal("�м�������������");

    if (strcmp(op, str_func_def) == 0)
        midcodefile << rs << " " << rt << "()" << endl;
    else if (strcmp(op, str_para) == 0)
        midcodefile << op << " " << rs << " " << rt << endl;
    else if (strcmp(op, str_push) == 0)
        midcodefile << op << " " << rs << endl;
    else if (strcmp(op, str_call) == 0)
        midcodefile << op << " " << rs << endl;
    else if (strcmp(op, "=") == 0)
        midcodefile << rd << " " << op << " " << rs << endl;
    else if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0
         ||  strcmp(op, "*") == 0 || strcmp(op, "/") == 0)
        midcodefile << rd << "=" << rs << " " << op << " " << rt << endl;
    else if (strcmp(op, "==") == 0 || strcmp(op, "!=") == 0
         ||  strcmp(op, ">") == 0 || strcmp(op, ">=") == 0
         ||  strcmp(op, "<") == 0 || strcmp(op, "<=") == 0)
        midcodefile << rs << " " << op << " " << rt << endl;
    else if (strcmp(op, str_BNZ) == 0)
        midcodefile << "BNZ " << rd << endl;
    else if (strcmp(op, str_BZ) == 0)
        midcodefile << "BZ " << rd << endl;
    else if (strcmp(op, str_ret) == 0)
    {
        if (rs == nullptr)
            midcodefile << op << endl;
        else
            midcodefile << op << " " << rs << endl;
    }
        
    else if (strcmp(op, str_GOTO) == 0)
        midcodefile << "GOTO " << rs << endl;
    else if (strcmp(op, "=[]") == 0)
        midcodefile << rd << "=" << rs << "[" << rt << "]" << endl;
    else if (strcmp(op, "[]=") == 0)
        midcodefile << rs << "[" << rt << "]=" << rd << endl;
    else if (strcmp(op, str_prt) == 0)
        midcodefile << op << " " << rs << " " << rt << endl;
    else if (strcmp(op, str_scf) == 0)
        midcodefile << op << " " << rd << " " << rs << endl;
    else if (strcmp(op, str_label) == 0)
        midcodefile << rd << ":" << endl;
    else
        error("����ʶ���op");
}
void NaiveCompiler::assign_statement(char (&parent)[ALNG])
{
    
    types typ;
    char rs[ALNG];
    char rt[ALNG];
    char rd[ALNG];
    int index;
    //���ұ�־��
    index = getIndexByNameAndParent(parent, symbols[0].id, search_mode);
    if (index == -1)
    {
        //��ʶ��������
        string msg = symbols[0].id;
        msg.append("������");
        error(msg);
    }

    if (tab[index].obj != var && tab[index].obj != arr && tab[index].obj != para)
    {
        string msg = symbols[0].id;
        msg.append("��ֵ�����޸�");
        error(msg);
    }

    strcpy_s(rs, symbols[0].id);

    if (end == 1)
    {
        symbols[1].copy(symbols[0]);
        symbols[1].clear();
        end = 0;
    }
    else
        insymbol(symbols[0]);

    if (symbols[0].sy == lsquare)
    {
        //˵��������Ԫ�ظ�ֵ
        //����ʶ��������Ϊ����
        if (tab[index].obj != arr)
        {
            //�������Ͳ���
            string msg = tab[index].name;
            msg.append("�������飬���Ͳ���");
            error(msg);
        }

        typ = atab[tab[index].ref].elementType;
        insymbol(symbols[0]);
        if (f_expression.count(symbols[0].sy))
        {
            types indexType;
            expression(parent, rt, indexType);
            if (indexType == chars)
            {
                indexType = ints;
                error("��⵽��������Ϊ�ַ��ͣ���������ӦΪ���ͣ������Զ�ת��", false);
            }
            
            if (isdigit(rt[0]) || (rt[0] == '-' && isdigit(rt[1])))
            {
                if (atoi(rt) >= atab[tab[index].ref].size)
                    error("�������������Ͻ�");
                else if (atoi(rt) < 0)
                    error("�������������½�");
            }
            
        }
        else
            error("��[�� ���治�Ǳ��ʽ");

        if (symbols[0].sy == rsquare)
            insymbol(symbols[0]);
        else
            error("ȱ�� ]");
    }
    else
    {
        if (tab[index].obj == arr)
        {
            //�������Ͳ���
            string msg = tab[index].name;
            msg.append("�����飬���ܶ���������ֵ,�˴���Ϊ���ش����޷���������");
            error(msg);
            exit(0);
        }
        else
        {
            typ = tab[index].typ;
        }
    }
        

    if (symbols[0].sy == becomesy)
        insymbol(symbols[0]);
    else
        error("ȱ�� =");

    if (f_expression.count(symbols[0].sy))
    {
        types valueType;
        expression(parent, rd, valueType);
        if (typ == ints && valueType == chars)
        {
            error("��ֵ����н��ַ��͸������ͣ���ִ������ת��", false);
            valueType = ints;
        }
        else if (typ == chars && valueType == ints)
        {
            error("��ֵ����н����͸����ַ��ͣ���ִ������ת��", false);
            valueType = chars;
        }

        if (tab[index].obj == arr)
        {
            append_midcode("[]=", rd, rs, rt);//����ֵ������ͨ��ʶ����ֵ��������Ԫ��
        }
        else
        {
            //��֮ǰ�е㲻һ����rs�ǵ�һ����־����������ֵ��һ����rd�����һ�����ʽ���Ǹ�ֵ��һ��
            append_midcode("=", rs, rd);
        }
    }
    else
        error("= ����ȱ�ٱ��ʽ");
    programma_info << "����һ������ֵ��䡱" << endl;

}
//�п��������ӣ�Ҳ�п�������䣬���Ѱ��������ıȽϺã���ʱ�Ȳ���
void NaiveCompiler::call_return_func(char (&parent)[ALNG], char (&rd)[ALNG], types (&typ))//ע�������ֵ��λ��
{
    
    int index;//���ڱ����Ƿ��ڷ��ű��е�����
    //����symbols[0]�ı�Ƿ�����
    index = getIndexByNameAndParent(string_global, symbols[0].id, search_mode);
    if (index == -1)
    {
        //��ʶ��������
        string msg = symbols[0].id;
        msg.append("������");
        error(msg);
    }

    //��֤��ʶ���������ǲ��Ǻ���
    if (tab[index].obj != func)
    {
        //�������Ͳ���
        string msg = tab[index].name;
        msg.append("���Ǻ��������Ͳ���");
        error(msg);
    }

    if (tab[index].typ == notyp)
    {
        //������õ����з���ֵ�������޷���ֵҪ����
        string msg = tab[index].name;
        msg.append("�޷���ֵ");
        error(msg);
    }
    typ = tab[index].typ;

    if (end == 1)
    {
        symbols[0].clear();
        symbols[1].clear();
        end = 0;
        insymbol(symbols[0]);
    }
    else
    {
        insymbol(symbols[0]);
        if (symbols[0].sy == lparent)
            insymbol(symbols[0]);
        else
            error("ȱ�� (");
    }

    value_parameter_list(parent, tab[index].name);//����Ϊ�գ��ʲ�����֤��ʼ���ż�

    if (symbols[0].sy == rparent)
        insymbol(symbols[0]);
    else
        error("ȱ�� )");

    append_midcode(str_call, nullptr, tab[index].name);
    strcpy_s(rd, str_ret);
    programma_info << "����һ�����з���ֵ����������䡱" << endl;
}
void NaiveCompiler::call_void_func(char (&parent)[ALNG])
{
    
    int index;//���ڱ����Ƿ��ڷ��ű��е�����
    //����symbols[0]�ı�Ƿ�����
    index = getIndexByNameAndParent(string_global, symbols[0].id, search_mode);
    //statement�����Ѿ���֤�������ˣ�����������func

    //�޷���ֵ����ֻ����statement�������ã��볬ǰ�鿴һ���ַ��ж����ų���ֵ��䣬��end��Ϊ1
    symbols[0].clear();
    symbols[1].clear();
    end = 0;
    insymbol(symbols[0]);

    value_parameter_list(parent, tab[index].name);//����Ϊ�գ��ʲ�����֤��ʼ���ż�

    if (symbols[0].sy == rparent)
        insymbol(symbols[0]);
    else
        error("ȱ�� )");

    append_midcode(str_call, nullptr, tab[index].name);
    programma_info << "����һ�����޷���ֵ����������䡱" << endl;
}
void NaiveCompiler::caselist(char (&parent)[ALNG], types typ, char (&rs)[ALNG], bool &has_return)
{
    set <int> exist_const;
    char end[ALNG];
    strcpy_s(end, "fkd_switchend");
    fcount += 1;
    _itoa_s(fcount, &end[13], ALNG-14, 10);

    one_case(parent, typ, end, rs, exist_const, has_return);

    while (f_case.count(symbols[0].sy))
        one_case(parent, typ, end, rs, exist_const, has_return);

    append_midcode(str_label, end);
    programma_info << "����һ���������" << endl;
}
void NaiveCompiler::compound_statement(char (&parent)[ALNG], bool &has_return)
{
    
    //��������������
    if (f_const_declare.count(symbols[0].sy))//count����ֵ����0˵��sy���ڼ���
        const_declare(parent);

    //��������������
    if (f_var_declare.count(symbols[0].sy))
        var_declare(parent);

    int index = getIndexByNameAndParent(string_global, parent, search_mode);
    if (index != -1)
    {
        ftab[tab[index].ref].space = dx;
    }

    statement_column(parent, has_return);//����п���Ϊ�գ�����Ҫ�жϿ�ʼ���ż�
    programma_info << "����һ����������䡱" << endl;
}
void NaiveCompiler::condition(char (&parent)[ALNG], char (&label)[ALNG], bool jump)
{
    char rs[ALNG];
    char rt[ALNG];
    char op[ALNG];
    //�����ͱ��ʽ�Ŀ�ʼ���ż�һ�������������ж�
    types typ;
    expression(parent, rs, typ);

    //������ת���

    if (symbols[0].sy == beq || symbols[0].sy == bne || symbols[0].sy == bgt
        || symbols[0].sy == bge || symbols[0].sy == blt || symbols[0].sy == ble)
    {
        switch(symbols[0].sy)
        {
        case beq:   strcpy_s(op, "==");    break;
        case bne:   strcpy_s(op, "!=");    break;
        case bgt:   strcpy_s(op, ">");    break;
        case bge:   strcpy_s(op, ">=");    break;
        case ble:   strcpy_s(op, "<=");    break;
        case blt:   strcpy_s(op, "<");    break;
        }
        
        insymbol(symbols[0]);

        if (f_expression.count(symbols[0].sy))
        {
            expression(parent, rt, typ);
            append_midcode(op, nullptr, rs, rt);
            if (jump)
                append_midcode(str_BNZ, label);
            else
                append_midcode(str_BZ, label);
        }
        else
            error("��ϵ�����Ҳ�ȱ�ٱ��ʽ");
    }
    //����ֻ��һ�����ʽ��û�й�ϵ�����
    else
    {
        append_midcode("!=", nullptr, rs, "0");
        if (jump)
            append_midcode(str_BNZ, label);
        else
            append_midcode(str_BZ, label);
    }
    programma_info << "����һ����������" << endl;
}
int NaiveCompiler::constant(types typ)
{
    
    int value = 0;
    if (symbols[0].sy == charcon)
    {
        //���ַ�
        if (typ == ints)
            error("�������������������Ͳ�һ��");

        value = (int)symbols[0].chval;
        insymbol(symbols[0]);
        programma_info << "����һ����������" << endl;
        return value;
    }
    else
    {
        //����������
        if (typ == chars)
            error("�������������������Ͳ�һ��");
        value = integer();
        programma_info << "����һ����������" << endl;
        return value;
    }
    
}
void NaiveCompiler::const_declare(char (&parent)[ALNG])
{
    
    while (symbols[0].sy == constsy)
    {
        insymbol(symbols[0]);

        if (f_var_define.count(symbols[0].sy))
        {
            const_define(parent);
        }
        else
        {
            //����ȱ�ٳ�������
            error("ȱ�ٳ�������");
        }

        if (symbols[0].sy == semicomma)
            insymbol(symbols[0]);
        else
            error("ȱ�� ;");
    }
    programma_info << "����һ��������˵����" << endl;
}
void NaiveCompiler::const_define(char (&parent)[ALNG])
{
    
    char const_name[ALNG] = "";
    int value = 0;
    if (symbols[0].sy == intsy)
    {
        insymbol(symbols[0]);
intsy_label_1:
        if (symbols[0].sy == ident)
        {
            if (strcmp(parent, symbols[0].id) == 0 && strcmp(parent, string_global) != 0)
                error("�������������ں�������");
            else
            {
                //�Ǽǳ�������
                strcpy_s(const_name, symbols[0].id);
                insymbol(symbols[0]);
            }
        }
        else
            error("�������Ʋ��Ϸ�");

        if (symbols[0].sy == becomesy)
            insymbol(symbols[0]);
        else
            error("ȱ�� =");

        if (f_integer.count(symbols[0].sy))
        {
            value = integer();
        }
        else
            error("����������δ��⵽���ͳ���ֵ");

        //����������Ч�����Ͳ�Ϊ��,��Ǽǵ�����
        if (strcmp(const_name, ""))
        {
            if (getIndexByNameAndParent(parent, const_name, declare_mode) == -1)
            {
                if (t == TABLE_SIZE - 1)
                {
                    //�������ű�����������fatal���˳�����
                    fatal("���ű���������");
                }

                t += 1;
                strcpy_s(tab[t].name, const_name);
                tab[t].obj = con;
                tab[t].typ = ints;
                tab[t].value = value;
                strcpy_s(tab[t].parent, parent);

                midcodefile << "const int " << const_name << " = " << value << endl;
            }
            else
            {
                //�ظ�����������
                string msg = const_name;
                msg.append("�ظ�����");
                error(msg);
            }
        }
        //�Ƿ��������򲻵�����ű�

        if (symbols[0].sy == comma)
        {
            insymbol(symbols[0]);
            goto intsy_label_1;
        }

    }
    else if (symbols[0].sy == charsy)
    {
        insymbol(symbols[0]);
charsy_label_1:
        if (symbols[0].sy == ident)
        {
            if (strcmp(parent, symbols[0].id) == 0 && strcmp(parent, string_global) != 0)
                error("�������������ں�������");
            else
            {
                //�Ǽǳ�������
                strcpy_s(const_name, symbols[0].id);
                insymbol(symbols[0]);
            }
        }
        else
            error("�������Ʋ��Ϸ�");

        if (symbols[0].sy == becomesy)
            insymbol(symbols[0]);
        else
            error("ȱ�� =");

        if (symbols[0].sy == charcon)
        {
            //�Ǽǳ�����ֵ
            value = symbols[0].chval;
            insymbol(symbols[0]);
        }
        else
            error("����������δ��⵽�ַ��ͳ���ֵ");

        //����������Ч�����Ͳ�Ϊ��,��Ǽǵ�����
        if (strcmp(const_name, ""))
        {
            if (getIndexByNameAndParent(parent, const_name, declare_mode) == -1)
            {
                if (t == TABLE_SIZE - 1)
                {
                    //�������ű�����������fatal���˳�����
                    fatal("���ű���������");
                }

                t += 1;
                strcpy_s(tab[t].name, const_name);
                tab[t].obj = con;
                tab[t].typ = chars;
                tab[t].value = value;
                strcpy_s(tab[t].parent, parent);

                midcodefile << "const char " << const_name << " = " << value << endl;
            }
            else
            {
                //�ظ�����������
                string msg = const_name;
                msg.append("�ظ�����");
                error(msg);
            }
        }
        //�Ƿ��������򲻵�����ű�

        if (symbols[0].sy == comma)
        {
            insymbol(symbols[0]);
            goto charsy_label_1;
        }
    }
    
    programma_info << "����һ�����������塱" << endl;
}
void NaiveCompiler::do_while_statement(char (&parent)[ALNG], bool &has_return)
{
    
    char label[ALNG];
    insymbol(symbols[0]);
    if (f_statement.count(symbols[0].sy))
    {
        strcpy_s(label, "fkd_dobegin");
        fcount += 1;
        _itoa_s(fcount, &label[11], ALNG-12, 10);
        append_midcode(str_label, label);
        statement(parent, has_return);
    }
    else
        error("ȱ�ٺϷ������");

    if (symbols[0].sy == whilesy)
        insymbol(symbols[0]);
    else
        error("ȱ�� while");

    if (symbols[0].sy == lparent)
        insymbol(symbols[0]);
    else
        error("ȱ�� )");

    if (f_condition.count(symbols[0].sy))
    {
        condition(parent, label, true);
    }
    else
        error("ѭ�����ȱ������");

    if (symbols[0].sy == rparent)
        insymbol(symbols[0]);
    else
        error("ȱ�� )");

    programma_info << "����һ����ѭ����䡱" << endl;
}
void NaiveCompiler::error(string content, bool add2set)
{
    pair<int, string> a(ln, content);
    if (add2set)
        errors.push_back(a);
    fout << "��" << ln << "�У�" << content << endl;
    cout << "��" << ln << "�У�" << content << endl;
}
/*void NaiveCompiler::errormsg()
{
    list<pair<int, string>>::iterator iter;
    for (iter = errors.begin(); iter != errors.end(); iter++)
        
    
}*/
void NaiveCompiler::expression(char (&parent)[ALNG], char (&rd)[ALNG], types (&typ))
{
    int numSym = 1;
    char rs[ALNG];
    char rt[ALNG];
    char op[ALNG];

    if (symbols[0].sy == plussy)
    {
        insymbol(symbols[0]);
    }
    else if (symbols[0].sy == minussy)
    {
        //������������Ϊ��
        numSym = -1;
        insymbol(symbols[0]);
    }

    if (f_item.count(symbols[0].sy))
    {
        item(parent, rs, typ);
        if (numSym == -1)
        {
            allocHelpVar(rd);
            if (getIndexByNameAndParent(parent, rd, declare_mode) != -1)
                error("Դ�����д���fangkedong��ͷ�ı���");
            else
            {
                if (t == TABLE_SIZE - 1)
                {
                    //�������ű�����������fatal���˳�����
                    fatal("���ű���������");
                }

                t += 1;
                strcpy_s(tab[t].name, rd);
                tab[t].obj = var;
                tab[t].typ = typ;
                strcpy_s(tab[t].parent, parent);
                int func_index=getIndexByNameAndParent(string_global, parent, search_mode);
                if (func_index == -1)
                    error("�Ҳ�������");
                else
                {
                    tab[t].address = ftab[tab[func_index].ref].space;
                    ftab[tab[func_index].ref].space += 4;
                }
            }
            append_midcode("-", rd, "0", rs);//ȡ�෴����rs����rtλ
            strcpy_s(rs, rd);
        }
    }
    else
        error("���ʽ�д��ڷǷ�����");

    bool flag = false;
    while (symbols[0].sy == plussy || symbols[0].sy == minussy)
    {
        flag = true;
        //����ӷ������
        if (symbols[0].sy == plussy)
            strcpy_s(op, "+");
        else
            strcpy_s(op, "-");

        insymbol(symbols[0]);
        if (f_item.count(symbols[0].sy))
        {
            item(parent, rt, typ);
        }
        else
            error("���ʽ�д��ڷǷ�����");

        allocHelpVar(rd);
        if (getIndexByNameAndParent(parent, rd, declare_mode) != -1)
            error("Դ�����д���fangkedong��ͷ�ı���");
        else
        {
            if (t == TABLE_SIZE - 1)
            {
                //�������ű�����������fatal���˳�����
                fatal("���ű���������");
            }

            t += 1;
            strcpy_s(tab[t].name, rd);
            tab[t].obj = var;
            tab[t].typ = typ;
            strcpy_s(tab[t].parent, parent);
            int func_index=getIndexByNameAndParent(string_global, parent, search_mode);
            if (func_index == -1)
                error("�Ҳ�������");
            else
            {
                tab[t].address = ftab[tab[func_index].ref].space;
                ftab[tab[func_index].ref].space += 4;
            }
        }

        append_midcode(op, rd, rs, rt);
        strcpy_s(rs, rd);
        typ = ints;
    }

    if (flag == false)
        strcpy_s(rd, rs);

    programma_info << "����һ�������ʽ��" << endl;
}
void NaiveCompiler::factor(char (&parent)[ALNG], char (&rd)[ALNG], types (&typ))
{
    int index;
    char rt[ALNG];
    typ = ints;
    if (symbols[0].sy == ident)
    {
        index = getIndexByNameAndParent(parent, symbols[0].id, search_mode);
        if (index == -1)
        {
            //�Ҳ���������
            string msg = symbols[0].id;
            msg.append("������");
            error(msg);
        }

        insymbol(symbols[1]);
        end = 1;
        if (symbols[1].sy == lsquare)
        {
            //��������Ԫ��
            //��֤֮ǰ�ı�ʶ��������������
            if (tab[index].obj != arr)
            {
                //�������Ͳ���
                string msg = tab[index].name;
                msg.append("�������飬���Ͳ���");
                error(msg);
            }

            typ = atab[tab[index].ref].elementType;

            symbols[0].clear();
            symbols[1].clear();
            end = 0;
            insymbol(symbols[0]);

            if (f_expression.count(symbols[0].sy))
            {
                types indexType;
                expression(parent, rt, indexType);
                if (indexType == chars)
                {
                    indexType = ints;
                    error("��⵽��������Ϊ�ַ��ͣ���������ӦΪ���ͣ������Զ�ת��", false);
                }
                
                if (isdigit(rt[0]) || (rt[0] == '-' && isdigit(rt[1])))
                {
                    if (atoi(rt) >= atab[tab[index].ref].size)
                        error("�������������Ͻ�");
                    else if (atoi(rt) < 0)
                        error("�������������½�");
                }
                    
                allocHelpVar(rd);
                if (getIndexByNameAndParent(parent, rd, declare_mode) != -1)
                    error("Դ�����д���fangkedong��ͷ�ı���");
                else
                {
                    if (t == TABLE_SIZE - 1)
                    {
                        //�������ű�����������fatal���˳�����
                        fatal("���ű���������");
                    }

                    t += 1;
                    strcpy_s(tab[t].name, rd);
                    tab[t].obj = var;
                    tab[t].typ = indexType;
                    strcpy_s(tab[t].parent, parent);
                    int func_index=getIndexByNameAndParent(string_global, parent, search_mode);
                    if (func_index == -1)
                        error("�Ҳ�������");
                    else
                    {
                        tab[t].address = ftab[tab[func_index].ref].space;
                        ftab[tab[func_index].ref].space += 4;
                    }
                }

                append_midcode("=[]", rd, tab[index].name, rt);
            }
            else
                error("[ ���治�Ǳ��ʽ");

            
            if (symbols[0].sy == rsquare)
                insymbol(symbols[0]);
            else
                error("ȱ�� ]");
        }
        else if (symbols[1].sy == lparent)
        {
            call_return_func(parent, rd, typ);
        }
        else
        {
            //�����ʶ��
            strcpy_s(rd, symbols[0].id);
            typ = tab[index].typ;
            symbols[1].copy(symbols[0]);
            symbols[1].clear();
            end = 0;
        }
    }
    else if (f_integer.count(symbols[0].sy))
    {
        _itoa_s(integer(), rd, 10);
    }
    else if (symbols[0].sy == charcon)
    {
        //�����ַ���ֵ
        typ = chars;
        _itoa_s(symbols[0].chval, rd, 10);
        insymbol(symbols[0]);
    }
    else if (symbols[0].sy == lparent)
    {
        insymbol(symbols[0]);
        if (f_expression.count(symbols[0].sy))
        {
            expression(parent, rd, typ);
        }
        else
            error("( ���治�Ǳ��ʽ");

        if (symbols[0].sy == rparent)
            insymbol(symbols[0]);
        else
            error("ȱ�� )");
    }
    programma_info << "����һ�������ӡ�" << endl;
}
void NaiveCompiler::fatal(string content)
{
    fout << content << endl;
    cout << content << endl;
    exit(0);
}
//�鵽�򷵻�>=0,�鲻������-1
int NaiveCompiler::getIndexByNameAndParent(char (&parent)[ALNG], char (&name)[ALNG], operate_table_mode mode)
{
    if (mode == declare_mode)
    {
        for (int i = 0; i <= t; i++)
        {
            if (strcmp(tab[i].parent, parent) == 0)
            {
                if (strcmp(tab[i].name, name) == 0)
                    return i;
            }
        }
        return -1;
    }
    else if (mode == search_mode)
    {
        for (int i = 0; i <= t; i++)
        {
            if (strcmp(tab[i].parent, parent) == 0)
            {
                if (strcmp(tab[i].name, name) == 0)
                    return i;
            }
        }

        for (int i = 0; i <= t; i++)
        {
            if (strcmp(tab[i].parent, string_global) == 0)
            {
                if (strcmp(tab[i].name, name) == 0)
                    return i;
            }
        }
        return -1;
    }
    return -1;
}
/*
int NaiveCompiler::getAddrByName(char* name)
{
    int addr = -1;
    list<pair<char* , int>>::iterator ptr;
    ptr = data_stack.begin();
    for (; ptr != data_stack.end(); ptr++)
    {
        if (strcmp(ptr->first, name) == 0)
        {
            addr = ptr->second;
            break;
        }
    }
    return addr;
}*/
void NaiveCompiler::if_statement(char (&parent)[ALNG], bool &has_return)
{
    
    char label[ALNG] = {};
    strcpy_s(label, "fangkd_ifend");
    fcount += 1;
    _itoa_s(fcount, &label[12], ALNG-13, 10);

    insymbol(symbols[0]);
    if (symbols[0].sy == lparent)
        insymbol(symbols[0]);
    else
        error("ȱ�� (");

    if (f_condition.count(symbols[0].sy))
    {
        condition(parent, label, false);
    }
    else
        error("if���ȱ������");

    if (symbols[0].sy == rparent)
        insymbol(symbols[0]);
    else
        error("ȱ�� )");

    if (f_statement.count(symbols[0].sy))
    {
        statement(parent, has_return);
    }
    else
        error("���ڷǷ����");

    
    append_midcode(str_label, label);//����ǩ�ӵ����
    programma_info << "����һ����������䡱" << endl;
}
void NaiveCompiler::insymbol(symbol_record (&aim))
{
    int i, j, k;
label_1:  
    aim.clear();
    while(ch == ' ' || ch == 9)
        nextch();

    if ((ch == '_') || islower(ch) || isupper(ch))
    {
        // key words and ident
        k = 1;
        memset(aim.id, 0, sizeof(aim.id));
        aim.id[0] = tolower(ch);
        nextch();
        while(isdigit(ch) || islower(ch) || isupper(ch) || ch == '_')
        {
            if (k < ALNG)
            {
                aim.id[k] = tolower(ch);
                k += 1;
            }
            nextch();
        }

        i = 0;
        j = NKW - 1;
        while(i <= j){
            k = (i + j) / 2;
            if (strcmp(aim.id, key[k]) == 0)
                break;
            if (strcmp(aim.id, key[k]) < 0)
                j = k - 1;
            if (strcmp(aim.id, key[k]) > 0)
                i = k + 1;
        }

        if (i <= j)//ǰ��break��
            aim.sy = ksy[k];
        else
            aim.sy = ident;

    }
    else if (isdigit(ch))
    {
        // int
        i = 0;
        aim.inum = 0;
        aim.sy = intcon;

        if (ch == '0') // 0 һλ
        {
            nextch();
            if (isdigit(ch))
            {
                while (isdigit(ch))
                    nextch();

                // �޷���������Ӧ��0��ͷ������
                error("�޷���������Ӧ��0��ͷ");
            }

        }
        else // �޷����������ɶ�λ
        {
            aim.inum = ch - '0';
            k = 1;
            nextch();
            while (isdigit(ch))
            {
                aim.inum = aim.inum * 10 + ch -'0';
                k += 1;
                nextch();
            }
            if (aim.inum > UINT32_MAX)
            {
                //��ֵ�����޷��������Ͻ磬����
                error("��ֵ�����޷��������Ͻ�");

                aim.inum = 0;
                k = 0;
            }

        }
    }
    else if (ch == '\'')
    {
        // charcon
        aim.sy = charcon;
        nextch();
        if (islower(ch) || isupper(ch) || isdigit(ch) || (ch == '_') 
            || (ch == '+') || (ch == '-') || (ch == '*') || (ch == '/'))
        {
            aim.chval = ch;
            nextch();
            if (ch != '\'')
            {
                //�ַ�����ǵ����ţ�����
                error("�ַ�����ǵ�����");
                aim.sy = nosy;
                goto label_1;
            }
            else
                nextch();
        }
        else
        {
            //�����ź���ӵĲ�����ĸ�����ֻ�Ӽ��˳����ţ�����
            error("�����ź���ӵĲ�����ĸ�����ֻ�Ӽ��˳�����");
            aim.sy = nosy;
            goto label_1;
        }

    }
    else if (ch == '\"')
    {
        // stringcon
        memset(aim.strval, 0, sizeof(aim.strval));
        k = 0;
label_2:
        nextch();
        if (ch == '\"')
            goto label_3;

        //�ַ����Ϸ����ݵ�ascii��Ϊ32��33,35-126
        if (ch >= 32 && ch <= 126 && ch != 34)
        {
            aim.strval[k] = ch;
            k += 1;
            goto label_2;
        }
        else
        {
            error("�ַ������ݲ��Ϸ�");
            //error
            goto label_1;
        }

label_3:
        aim.sy = stringcon;
        aim.sleng = k;
        nextch();
    }
    else if (ch == '=')
    {
        nextch();
        if (ch == '=')
        {
            // ==
            aim.sy = beq;
            nextch();
        }
        else
        {
            // = 
            aim.sy = becomesy;
        }
    }
    else if (ch == '>')
    {
        nextch();
        if (ch == '=')
        {
            // >=
            aim.sy = bge;
            nextch();
        }
        else
        {
            // >
            aim.sy = bgt;
        }
    }
    else if (ch == '<')
    {
        nextch();
        if (ch == '=')
        {
            // <=
            aim.sy = ble;
            nextch();
        }
        else
        {
            // <
            aim.sy = blt;
        }
    }
    else if (ch == '!')
    {
        nextch();
        if (ch == '=')
        {
            // != 
            aim.sy = bne;
            nextch();
        }
        else 
        {
            //������
            error("! �ǲ��Ϸ���");
            aim.sy = nosy;
            goto label_1;
        }
    }
    else if (ch == '+')
    {
        // +
        aim.sy = plussy;
        nextch();
    }
    else if (ch == '-')
    {
        // -
        aim.sy = minussy;
        nextch();
    }
    else if (ch == '*')
    {
        // *
        aim.sy = multsy;
        nextch();
    }
    else if (ch == '/')
    {
        // /
        aim.sy = divsy;
        nextch();
    }
    else if (ch == ',')
    {
        // ,
        aim.sy = comma;
        nextch();
    }
    else if (ch == ';')
    {
        // ;
        aim.sy = semicomma;
        nextch();
    }
    else if (ch == ':')
    {
        // :
        aim.sy = colon;
        nextch();
    }
    else if (ch == '(')
    {
        // ( lparent
        aim.sy = lparent;
        nextch();
    }
    else if (ch == ')')
    {
        // ) rparent
        aim.sy = rparent;
        nextch();
    }
    else if (ch == '[')
    {
        // [ lsquare
        aim.sy = lsquare;
        nextch();
    }
    else if (ch == ']')
    {
        // ] rsquare
        aim.sy = rsquare;
        nextch();
    }
    else if (ch == '{')
    {
        // { lbrack
        aim.sy = lbrack;
        nextch();
    }
    else if (ch == '}')
    {
        // } rbrack
        aim.sy = rbrack;
        nextch();
    }
    else 
    {
        //����Ƿ�����
        error("����Ƿ�����");
        nextch();
        goto label_1;
    }
}
int NaiveCompiler::integer()
{
    
    int sym = 1;//1��ʾ���ţ�-1��ʾ����
    int value = 0;

    if (symbols[0].sy == intcon && symbols[0].inum == 0)
    {
        insymbol(symbols[0]);
        return 0;
    }
    else
    {
        if (symbols[0].sy == plussy)
        {
            insymbol(symbols[0]);
        }
        else if (symbols[0].sy == minussy)
        {
            //������������Ϊ��
            sym = -1;
            insymbol(symbols[0]);
        }

        if (symbols[0].sy == intcon)
        {
            //�Ǽ�������ֵ
            value = (int)symbols[0].inum;
            insymbol(symbols[0]);
            value = sym * value;
            return value;
        }
        else
            error("�������Ƿ�������ֵ");
    }
    return 0;
    programma_info << "����һ����������" << endl;
}
void NaiveCompiler::item(char (&parent)[ALNG], char (&rd)[ALNG], types (&typ))
{
    char rs[ALNG];
    char rt[ALNG];
    char op[ALNG];
    factor(parent, rs, typ);
    
    bool flag = false;
    while (symbols[0].sy == multsy || symbols[0].sy == divsy)
    {
        flag = true;
        //�Ǽǳ˷������
        if (symbols[0].sy == multsy)
            strcpy_s(op, "*");
        else
            strcpy_s(op, "/");

        insymbol(symbols[0]);
        if (f_factor.count(symbols[0].sy))
            factor(parent, rt, typ);
        else
            error("���������ȱ�ٲ��������ֵ");

        allocHelpVar(rd);
        if (getIndexByNameAndParent(parent, rd, declare_mode) != -1)
            error("Դ�����д���fangkedong��ͷ�ı���");
        else
        {
            if (t == TABLE_SIZE - 1)
            {
                //�������ű�����������fatal���˳�����
                fatal("���ű���������");
            }

            t += 1;
            strcpy_s(tab[t].name, rd);
            tab[t].obj = var;
            tab[t].typ = typ;
            strcpy_s(tab[t].parent, parent);
            int func_index=getIndexByNameAndParent(string_global, parent, search_mode);
            if (func_index == -1)
                error("�Ҳ�������");
            else
            {
                tab[t].address = ftab[tab[func_index].ref].space;
                ftab[tab[func_index].ref].space += 4;
            }
        }

        append_midcode(op, rd, rs, rt);
        strcpy_s(rs, rd);
        typ = ints;
    }

    if (flag == false)
        strcpy_s(rd, rs);

    programma_info << "����һ�����" << endl;
}
void NaiveCompiler::loadValue2reg(char (&parent)[ALNG], char (&name)[ALNG], char* tar_reg)
{
    if (isdigit(name[0]) || (name[0] == '-' && isdigit(name[1])))//��ʶ�������������ֿ�ͷ���������ֿ�ͷ��һ���ǳ���
    {
        addi(tar_reg, reg_$0, atoi(name));
        return ;
    }

    int index;
    index = getIndexByNameAndParent(parent, name, search_mode);
    if (index == -1)
    {
        //˵�����Ǿֲ�������ȫ�ֱ��������Ǹ�����������ջ����
        lw(tar_reg, reg_sp, 4);
        addi(reg_sp, reg_sp, 4);
    }
    else
    {
        if (strcmp(tab[index].parent, string_global) == 0)
        {
            if (tab[index].obj == con)
                addi(tar_reg, reg_$0, tab[index].value);
            else
                lw(tar_reg, reg_t9, tab[index].address);
        } 
        else
        {
            if (tab[index].obj == con)
                addi(tar_reg, reg_$0, tab[index].value);
            else
                lw(tar_reg, reg_fp, tab[index].address);
        } 

    }

}
void NaiveCompiler::midcode2mips()
{
    printf("��ʼ����mips�����롭��\n");
    mipsfile << ".text" << endl;
    set<string> initedArrayName;
    char parent[ALNG];
    strcpy_s(parent, string_global);
    ori(reg_t3, reg_$0, 1);
    if (opt_sign)
        lui(reg_k0, 0x1001);//��ʼ��.data�׵�ַ������ַ���
    lui(reg_gp, 0x1000);//��ʼ������ջ�׵�ַ����������
    lui(reg_fp, 0x1004);//��ʼ������ջ�׵�ַ����������
    lui(reg_sp, 0x7fff);//��ʼ������ջ�׵�ַ����������
    addi(reg_sp, reg_sp, 0xeffc);
    lui(reg_t9, 0x1000);//��ʼ��ȫ�ֱ����׵�ַ
    addi(reg_t9, reg_t9, 0x1000);
    _j(string_main);
    if (delay_mode)
        nop();
    //addi("$t9", reg_$0, 0x27fc);//$t9�������ջָ��
    for (int i = 0; i<mid_counter; i++)
    {
        if (strcmp(midcode[i].op, "+") == 0)
        {
            char rs[REG_NAME_SIZE];
            char rt[REG_NAME_SIZE];
            bool rs_is_imm = false, rt_is_imm = false;
            int index = getIndexByNameAndParent(parent, midcode[i].rt, search_mode);
            if (index != -1 && tab[index].reg[0] != '\0')
            {
                strcpy_s(rt, tab[index].reg);
            }
            else
            {
                if (opt_sign && (isdigit(midcode[i].rt[0]) || (midcode[i].rt[0] == '-' && isdigit(midcode[i].rt[1]))))//��ʶ�������������ֿ�ͷ���������ֿ�ͷ��һ���ǳ���
                    rt_is_imm = true;
                else
                {
                    loadValue2reg(parent, midcode[i].rt, reg_t1);
                    strcpy_s(rt, reg_t1);
                }
            }

            index = getIndexByNameAndParent(parent, midcode[i].rs, search_mode);
            if (index != -1 && tab[index].reg[0] != '\0')
            {
                strcpy_s(rs, tab[index].reg);
            }
            else
            {
                if (opt_sign && !rt_is_imm && (isdigit(midcode[i].rs[0]) || (midcode[i].rs[0] == '-' && isdigit(midcode[i].rs[1]))))//��ʶ�������������ֿ�ͷ���������ֿ�ͷ��һ���ǳ���
                    rs_is_imm = true;
                else
                {
                    loadValue2reg(parent, midcode[i].rs, reg_t0);
                    strcpy_s(rs, reg_t0);
                }
            }

            index = getIndexByNameAndParent(parent, midcode[i].rd, search_mode);
            if (index != -1 && tab[index].reg[0] != '\0')
            {
                if (!rs_is_imm && !rt_is_imm)
                    add(tab[index].reg, rs, rt);
                else if (rs_is_imm && !rt_is_imm)
                    addi(tab[index].reg, rt, atoi(midcode[i].rs));
                else if (!rs_is_imm && rt_is_imm)
                    addi(tab[index].reg, rs, atoi(midcode[i].rt));
            }
            else
            {
                if (!rs_is_imm && !rt_is_imm)
                    add(reg_t2, rs, rt);
                else if (rs_is_imm && !rt_is_imm)
                    addi(reg_t2, rt, atoi(midcode[i].rs));
                else if (!rs_is_imm && rt_is_imm)
                    addi(reg_t2, rs, atoi(midcode[i].rt));
                saveValue2mem(parent, midcode[i].rd, reg_t2);
            }
        }
        else if (strcmp(midcode[i].op, "-") == 0)
        {
            char rs[REG_NAME_SIZE];
            char rt[REG_NAME_SIZE];
            bool rt_is_imm= false;
            int index = getIndexByNameAndParent(parent, midcode[i].rt, search_mode);
            if (index != -1 && tab[index].reg[0] != '\0')
            {
                strcpy_s(rt, tab[index].reg);
            }
            else
            {
                if (opt_sign && (isdigit(midcode[i].rt[0]) || (midcode[i].rt[0] == '-' && isdigit(midcode[i].rt[1]))))//��ʶ�������������ֿ�ͷ���������ֿ�ͷ��һ���ǳ���
                    rt_is_imm = true;
                else
                {
                    loadValue2reg(parent, midcode[i].rt, reg_t1);
                    strcpy_s(rt, reg_t1);
                }
            }

            index = getIndexByNameAndParent(parent, midcode[i].rs, search_mode);
            if (index != -1 && tab[index].reg[0] != '\0')
            {
                strcpy_s(rs, tab[index].reg);
            }
            else
            {
                loadValue2reg(parent, midcode[i].rs, reg_t0);
                strcpy_s(rs, reg_t0);
            }

            index = getIndexByNameAndParent(parent, midcode[i].rd, search_mode);
            if (index != -1 && tab[index].reg[0] != '\0')
            {
                if (rt_is_imm)
                    addi(tab[index].reg, rs, -atoi(midcode[i].rt));
                else
                    sub(tab[index].reg, rs, rt);
            }
            else
            {
                if (rt_is_imm)
                    addi(reg_t2, rs, -atoi(midcode[i].rt));
                else
                    sub(reg_t2, rs, rt);
                saveValue2mem(parent, midcode[i].rd, reg_t2);
            }
        }
        else if (strcmp(midcode[i].op, "*") == 0)
        {
            char rs[REG_NAME_SIZE];
            char rt[REG_NAME_SIZE];
            int index = getIndexByNameAndParent(parent, midcode[i].rt, search_mode);
            if (index != -1 && tab[index].reg[0] != '\0')
            {
                strcpy_s(rt, tab[index].reg);
            }
            else
            {
                loadValue2reg(parent, midcode[i].rt, reg_t1);
                strcpy_s(rt, reg_t1);
            }

            index = getIndexByNameAndParent(parent, midcode[i].rs, search_mode);
            if (index != -1 && tab[index].reg[0] != '\0')
            {
                strcpy_s(rs, tab[index].reg);
            }
            else
            {
                loadValue2reg(parent, midcode[i].rs, reg_t0);
                strcpy_s(rs, reg_t0);
            }

            mult(rs, rt);

            index = getIndexByNameAndParent(parent, midcode[i].rd, search_mode);
            if (index != -1 && tab[index].reg[0] != '\0')
            {
                mflo(tab[index].reg);
            }
            else
            {
                saveValue2mem(parent, midcode[i].rd, reg_lo);
            }
        }
        else if (strcmp(midcode[i].op, "/") == 0)
        {
            char rs[REG_NAME_SIZE];
            char rt[REG_NAME_SIZE];
            int index = getIndexByNameAndParent(parent, midcode[i].rt, search_mode);
            if (index != -1 && tab[index].reg[0] != '\0')
            {
                strcpy_s(rt, tab[index].reg);
            }
            else
            {
                loadValue2reg(parent, midcode[i].rt, reg_t1);
                strcpy_s(rt, reg_t1);
            }

            index = getIndexByNameAndParent(parent, midcode[i].rs, search_mode);
            if (index != -1 && tab[index].reg[0] != '\0')
            {
                strcpy_s(rs, tab[index].reg);
            }
            else
            {
                loadValue2reg(parent, midcode[i].rs, reg_t0);
                strcpy_s(rs, reg_t0);
            }

            div(rs, rt);

            index = getIndexByNameAndParent(parent, midcode[i].rd, search_mode);
            if (index != -1 && tab[index].reg[0] != '\0')
            {
                mflo(tab[index].reg);
            }
            else
            {
                saveValue2mem(parent, midcode[i].rd, reg_lo);
            }
        }
        else if (strcmp(midcode[i].op, str_push) == 0)
        {
            char rs[REG_NAME_SIZE];
            int index = getIndexByNameAndParent(parent, midcode[i].rs, search_mode);
            if (index != -1 && tab[index].reg[0] != '\0')
                strcpy_s(rs, tab[index].reg);
            else
            {
                loadValue2reg(parent, midcode[i].rs, reg_t0);
                strcpy_s(rs, reg_t0);
            }
            sw(rs, reg_gp, 0);
            addi(reg_gp, reg_gp, 4);
        }
        else if (strcmp(midcode[i].op, str_call) == 0)
        {
            //jal
            int par_index = getIndexByNameAndParent(string_global, parent, search_mode);
            int index = getIndexByNameAndParent(string_global, midcode[i].rs, search_mode);
            addi(reg_t0, reg_fp, 0);
            addi(reg_fp, reg_fp, ftab[tab[par_index].ref].space);
            sw(reg_t0, reg_fp, 0);//������һ��fp
            if (ftab[tab[index].ref].alloc_reg[0])
                sw(reg_s0, reg_fp, 12);//����$s0
            if (ftab[tab[index].ref].alloc_reg[1])
                sw(reg_s1, reg_fp, 16);//����$s1
            if (ftab[tab[index].ref].alloc_reg[2])
                sw(reg_s2, reg_fp, 20);//����$s2
            if (ftab[tab[index].ref].alloc_reg[3])
                sw(reg_s3, reg_fp, 24);//����$s3
            if (ftab[tab[index].ref].alloc_reg[4])
                sw(reg_s4, reg_fp, 28);//����$s4
            if (ftab[tab[index].ref].alloc_reg[5])
                sw(reg_s5, reg_fp, 32);//����$s5
            if (ftab[tab[index].ref].alloc_reg[6])
                sw(reg_s6, reg_fp, 36);//����$s6
            if (ftab[tab[index].ref].alloc_reg[7])
                sw(reg_s7, reg_fp, 40);//����$s7
            if (ftab[tab[index].ref].alloc_reg[8])
                sw(reg_a1, reg_fp, 44);//����$a1
            if (ftab[tab[index].ref].alloc_reg[9])
                sw(reg_a2, reg_fp, 48);//����$a2
            if (ftab[tab[index].ref].alloc_reg[10])
                sw(reg_a3, reg_fp, 52);//����$a3
            if (ftab[tab[index].ref].alloc_reg[11])
                sw(reg_t5, reg_fp, 56);//����$t5
            if (ftab[tab[index].ref].alloc_reg[12])
                sw(reg_t6, reg_fp, 60);//����$t6
            if (ftab[tab[index].ref].alloc_reg[13])
                sw(reg_t7, reg_fp, 64);//����$t7
            if (ftab[tab[index].ref].alloc_reg[14])
                sw(reg_t8, reg_fp, 68);//����$t8
            jal(midcode[i].rs);
            if (delay_mode)
                nop();
        }
        else if (strcmp(midcode[i].op, "=") == 0)
        {
            char rs[REG_NAME_SIZE];
            int index = getIndexByNameAndParent(parent, midcode[i].rs, search_mode);
            bool rs_is_imm = false;
            if (index != -1 && tab[index].reg[0] != '\0')
                strcpy_s(rs, tab[index].reg);
            else
            {
                if (opt_sign && (isdigit(midcode[i].rs[0]) || (midcode[i].rs[0] == '-' && isdigit(midcode[i].rs[1]))))//��ʶ�������������ֿ�ͷ���������ֿ�ͷ��һ���ǳ���
                    rs_is_imm = true;
                else
                {
                    loadValue2reg(parent, midcode[i].rs, reg_t0);
                    strcpy_s(rs, reg_t0);
                }
                
            }

            index = getIndexByNameAndParent(parent, midcode[i].rd, search_mode);
            if (index != -1 && tab[index].reg[0] != '\0')
            {
                if (rs_is_imm)
                    addi(tab[index].reg, reg_$0, atoi(midcode[i].rs));
                else
                    addi(tab[index].reg, rs, 0);
            }
            else
            {
                if (rs_is_imm)
                {
                    addi(reg_t0, reg_$0, atoi(midcode[i].rs));
                    strcpy_s(rs, reg_t0);
                }
                saveValue2mem(parent, midcode[i].rd, rs);
            }
        }
        else if (strcmp(midcode[i].op, "==") == 0)
        {
            char rs[REG_NAME_SIZE];
            char rt[REG_NAME_SIZE];
            bool rt_is_imm = false;
            int index = getIndexByNameAndParent(parent, midcode[i].rt, search_mode);
            if (index != -1 && tab[index].reg[0] != '\0')
            {
                strcpy_s(rt, tab[index].reg);
            }
            else
            {
                if (opt_sign && (isdigit(midcode[i].rt[0]) || (midcode[i].rt[0] == '-' && isdigit(midcode[i].rt[1]))))//��ʶ�������������ֿ�ͷ���������ֿ�ͷ��һ���ǳ���
                    rt_is_imm = true;
                else
                {
                    loadValue2reg(parent, midcode[i].rt, reg_t1);
                    strcpy_s(rt, reg_t1);
                }
            }

            index = getIndexByNameAndParent(parent, midcode[i].rs, search_mode);
            if (index != -1 && tab[index].reg[0] != '\0')
            {
                strcpy_s(rs, tab[index].reg);
            }
            else
            {
                loadValue2reg(parent, midcode[i].rs, reg_t0);
                strcpy_s(rs, reg_t0);
            }

            /*slt(reg_t2, rs, rt);
            slt(reg_t3, rt, rs);
            _nor(reg_t0, reg_t2, reg_t3);
            andi(reg_t0, reg_t0, 0x1);*/
            if (rt_is_imm)
                addi(reg_t0, rs, -atoi(midcode[i].rt));
            else
                subu(reg_t0, rs, rt);
            sltiu(reg_t0, reg_t0, 1);
        }
        else if (strcmp(midcode[i].op, "!=") == 0)
        {
            char rs[REG_NAME_SIZE];
            char rt[REG_NAME_SIZE];
            bool rt_is_imm=false;
            int index = getIndexByNameAndParent(parent, midcode[i].rt, search_mode);
            if (index != -1 && tab[index].reg[0] != '\0')
            {
                strcpy_s(rt, tab[index].reg);
            }
            else
            {
                if (opt_sign && (isdigit(midcode[i].rt[0]) || (midcode[i].rt[0] == '-' && isdigit(midcode[i].rt[1]))))//��ʶ�������������ֿ�ͷ���������ֿ�ͷ��һ���ǳ���
                    rt_is_imm = true;
                else
                {
                    loadValue2reg(parent, midcode[i].rt, reg_t1);
                    strcpy_s(rt, reg_t1);
                }
            }

            index = getIndexByNameAndParent(parent, midcode[i].rs, search_mode);
            if (index != -1 && tab[index].reg[0] != '\0')
            {
                strcpy_s(rs, tab[index].reg);
            }
            else
            {
                loadValue2reg(parent, midcode[i].rs, reg_t0);
                strcpy_s(rs, reg_t0);
            }

            /*slt(reg_t2, rs, rt);
            slt(reg_t3, rt, rs);
            _or(reg_t0, reg_t2, reg_t3);*/
            if (rt_is_imm)
                addi(reg_t0, rs, -atoi(midcode[i].rt));
            else
                subu(reg_t0, rs, rt);
            sltu(reg_t0, reg_$0, reg_t0);
        }
        else if (strcmp(midcode[i].op, ">") == 0)
        {
            char rs[REG_NAME_SIZE];
            char rt[REG_NAME_SIZE];
            bool rs_is_imm = false;
            int index = getIndexByNameAndParent(parent, midcode[i].rt, search_mode);
            if (index != -1 && tab[index].reg[0] != '\0')
            {
                strcpy_s(rt, tab[index].reg);
            }
            else
            {
                loadValue2reg(parent, midcode[i].rt, reg_t1);
                strcpy_s(rt, reg_t1);
            }

            index = getIndexByNameAndParent(parent, midcode[i].rs, search_mode);
            if (index != -1 && tab[index].reg[0] != '\0')
            {
                strcpy_s(rs, tab[index].reg);
            }
            else
            {
                if (opt_sign && (isdigit(midcode[i].rs[0]) || (midcode[i].rs[0] == '-' && isdigit(midcode[i].rs[1]))))//��ʶ�������������ֿ�ͷ���������ֿ�ͷ��һ���ǳ���
                    rs_is_imm = true;
                else
                {
                    loadValue2reg(parent, midcode[i].rs, reg_t0);
                    strcpy_s(rs, reg_t0);
                }
                
            }

            if (rs_is_imm)
                slti(reg_t0, rt, atoi(midcode[i].rs));
            else
                slt(reg_t0, rt, rs);
        }
        else if (strcmp(midcode[i].op, ">=") == 0)
        {
            char rs[REG_NAME_SIZE];
            char rt[REG_NAME_SIZE];
            bool rt_is_imm = false;
            int index = getIndexByNameAndParent(parent, midcode[i].rt, search_mode);
            if (index != -1 && tab[index].reg[0] != '\0')
            {
                strcpy_s(rt, tab[index].reg);
            }
            else
            {
                if (opt_sign && (isdigit(midcode[i].rt[0]) || (midcode[i].rt[0] == '-' && isdigit(midcode[i].rt[1]))))//��ʶ�������������ֿ�ͷ���������ֿ�ͷ��һ���ǳ���
                    rt_is_imm = true;
                else
                {
                    loadValue2reg(parent, midcode[i].rt, reg_t1);
                    strcpy_s(rt, reg_t1);
                }
            }

            index = getIndexByNameAndParent(parent, midcode[i].rs, search_mode);
            if (index != -1 && tab[index].reg[0] != '\0')
            {
                strcpy_s(rs, tab[index].reg);
            }
            else
            {
                loadValue2reg(parent, midcode[i].rs, reg_t0);
                strcpy_s(rs, reg_t0);
            }

            /*slt(reg_t2, rs, rt);
            slt(reg_t3, rt, rs);
            _nor(reg_t4, reg_t2, reg_t3);
            andi(reg_t4, reg_t4, 0x1);
            _or(reg_t0, reg_t4, reg_t3);*/
            if (rt_is_imm)
                slti(reg_t0, rs, atoi(midcode[i].rt));
            else
                slt(reg_t0, rs, rt);
            subu(reg_t0, reg_t3, reg_t0);
        }
        else if (strcmp(midcode[i].op, "<") == 0)
        {
            char rs[REG_NAME_SIZE];
            char rt[REG_NAME_SIZE];
            bool rt_is_imm = false;
            int index = getIndexByNameAndParent(parent, midcode[i].rt, search_mode);
            if (index != -1 && tab[index].reg[0] != '\0')
            {
                strcpy_s(rt, tab[index].reg);
            }
            else
            {
                if (opt_sign && (isdigit(midcode[i].rt[0]) || (midcode[i].rt[0] == '-' && isdigit(midcode[i].rt[1]))))//��ʶ�������������ֿ�ͷ���������ֿ�ͷ��һ���ǳ���
                    rt_is_imm = true;
                else
                {
                    loadValue2reg(parent, midcode[i].rt, reg_t1);
                    strcpy_s(rt, reg_t1);
                }
            }

            index = getIndexByNameAndParent(parent, midcode[i].rs, search_mode);
            if (index != -1 && tab[index].reg[0] != '\0')
            {
                strcpy_s(rs, tab[index].reg);
            }
            else
            {
                loadValue2reg(parent, midcode[i].rs, reg_t0);
                strcpy_s(rs, reg_t0);
            }

            if (rt_is_imm)
                slti(reg_t0, rs, atoi(midcode[i].rt));
            else
                slt(reg_t0, rs, rt);
        }
        else if (strcmp(midcode[i].op, "<=") == 0)
        {
            char rs[REG_NAME_SIZE];
            char rt[REG_NAME_SIZE];
            bool rs_is_imm = false;
            int index = getIndexByNameAndParent(parent, midcode[i].rt, search_mode);
            if (index != -1 && tab[index].reg[0] != '\0')
            {
                strcpy_s(rt, tab[index].reg);
            }
            else
            {
                loadValue2reg(parent, midcode[i].rt, reg_t1);
                strcpy_s(rt, reg_t1);
            }

            index = getIndexByNameAndParent(parent, midcode[i].rs, search_mode);
            if (index != -1 && tab[index].reg[0] != '\0')
            {
                strcpy_s(rs, tab[index].reg);
            }
            else
            {
                if (opt_sign && (isdigit(midcode[i].rs[0]) || (midcode[i].rs[0] == '-' && isdigit(midcode[i].rs[1]))))//��ʶ�������������ֿ�ͷ���������ֿ�ͷ��һ���ǳ���
                    rs_is_imm = true;
                else
                {
                    loadValue2reg(parent, midcode[i].rs, reg_t0);
                    strcpy_s(rs, reg_t0);
                }
                
            }
            /*slt(reg_t2, rs, rt);
            slt(reg_t3, rt, rs);
            _nor(reg_t4, reg_t2, reg_t3);
            andi(reg_t4, reg_t4, 0x1);
            _or(reg_t0, reg_t2, reg_t4);*/
            if (rs_is_imm)
                slti(reg_t0, rt, atoi(midcode[i].rs));
            else
                slt(reg_t0, rt, rs);
            subu(reg_t0, reg_t3, reg_t0);
        }
        else if (strcmp(midcode[i].op, str_BNZ) == 0)
        {
            _bne(reg_t0, reg_$0, midcode[i].rd);
            if (delay_mode)
                nop();
        }
        else if (strcmp(midcode[i].op, str_BZ) == 0)
        {
            _beq(reg_t0, reg_$0, midcode[i].rd);
            if (delay_mode)
                nop();
        }
        else if (strcmp(midcode[i].op, str_GOTO) == 0)
        {
            //j
            _j(midcode[i].rs);
            if (delay_mode)
                nop();
        }
        else if (strcmp(midcode[i].op, str_ret) == 0)
        {
            //jr $ra
            if (midcode[i].rs[0] != '\0')
            {
                char rs[REG_NAME_SIZE];
                int index = getIndexByNameAndParent(parent, midcode[i].rs, search_mode);
                if (index != -1 && tab[index].reg[0] != '\0')
                {
                    strcpy_s(rs, tab[index].reg);
                }
                else
                {
                    loadValue2reg(parent, midcode[i].rs, reg_v0);
                    strcpy_s(rs, reg_v0);
                }
                sw(rs, reg_sp, 0);
                addi(reg_sp, reg_sp, -4);
            }
            
            int index = getIndexByNameAndParent(string_global, parent, search_mode);
            lw(reg_ra, reg_fp, 8);
            if (ftab[tab[index].ref].alloc_reg[0])
                lw(reg_s0, reg_fp, 12);//�ָ�$s0
            if (ftab[tab[index].ref].alloc_reg[1])
                lw(reg_s1, reg_fp, 16);//�ָ�$s1
            if (ftab[tab[index].ref].alloc_reg[2])
                lw(reg_s2, reg_fp, 20);//�ָ�$s2
            if (ftab[tab[index].ref].alloc_reg[3])
                lw(reg_s3, reg_fp, 24);//�ָ�$s3
            if (ftab[tab[index].ref].alloc_reg[4])
                lw(reg_s4, reg_fp, 28);//�ָ�$s4
            if (ftab[tab[index].ref].alloc_reg[5])
                lw(reg_s5, reg_fp, 32);//�ָ�$s5
            if (ftab[tab[index].ref].alloc_reg[6])
                lw(reg_s6, reg_fp, 36);//�ָ�$s6
            if (ftab[tab[index].ref].alloc_reg[7])
                lw(reg_s7, reg_fp, 40);//�ָ�$s7
            if (ftab[tab[index].ref].alloc_reg[8])
                lw(reg_a1, reg_fp, 44);//�ָ�$a1
            if (ftab[tab[index].ref].alloc_reg[9])
                lw(reg_a2, reg_fp, 48);//�ָ�$a2
            if (ftab[tab[index].ref].alloc_reg[10])
                lw(reg_a3, reg_fp, 52);//�ָ�$a3
            if (ftab[tab[index].ref].alloc_reg[11])
                lw(reg_t5, reg_fp, 56);//�ָ�$t5
            if (ftab[tab[index].ref].alloc_reg[12])
                lw(reg_t6, reg_fp, 60);//�ָ�$t6
            if (ftab[tab[index].ref].alloc_reg[13])
                lw(reg_t7, reg_fp, 64);//�ָ�$t7
            if (ftab[tab[index].ref].alloc_reg[14])
                lw(reg_t8, reg_fp, 68);//�ָ�$t8
            lw(reg_fp, reg_fp, 0);
            jr(reg_ra);
            if (delay_mode)
                nop();
        }
        else if (strcmp(midcode[i].op, "[]=") == 0)
        {
            //���������׵�ַ��������������ƫ�ƣ�lw sw
            //��ֵ���򣺼�ֵ->����Ԫ��
            char rd[REG_NAME_SIZE];
            int array_index;
            bool rt_is_imm = false;
            int index = getIndexByNameAndParent(parent, midcode[i].rd, search_mode);
            if (index != -1 && tab[index].reg[0] != '\0')
            {
                strcpy_s(rd, tab[index].reg);
            }
            else
            {
                loadValue2reg(parent, midcode[i].rd, reg_t0);
                strcpy_s(rd, reg_t0);
            }

            char rs[REG_NAME_SIZE];
            index = getIndexByNameAndParent(parent, midcode[i].rs, search_mode);
            array_index = index;
            if (index != -1 && tab[index].reg[0] != '\0')
            {
                if (initedArrayName.count(midcode[i].rs) == 0)
                {
                    addi(tab[index].reg, reg_$0, tab[index].address);
                    initedArrayName.insert(midcode[i].rs);
                }
                strcpy_s(rs, tab[index].reg);
            }
            else
            {
                addi(reg_t1, reg_$0, tab[index].address);
                strcpy_s(rs, reg_t1);
            }

            char rt[REG_NAME_SIZE];
            index = getIndexByNameAndParent(parent, midcode[i].rt, search_mode);
            if (index != -1 && tab[index].reg[0] != '\0')
            {
                strcpy_s(rt, tab[index].reg);
            }
            else
            {
                if (opt_sign && (isdigit(midcode[i].rt[0]) || (midcode[i].rt[0] == '-' && isdigit(midcode[i].rt[1]))))//��ʶ�������������ֿ�ͷ���������ֿ�ͷ��һ���ǳ���
                    rt_is_imm = true;
                else
                {
                    loadValue2reg(parent, midcode[i].rt, reg_t2);
                    strcpy_s(rt, reg_t2);
                }
                
            }
            
            if (rt_is_imm)
            {
                int addr = atoi(midcode[i].rt);
                addr = addr * 4;
                addi(reg_t2, reg_$0, addr);
            }
            else
                sll(reg_t2, rt, 2);
            
            strcpy_s(rt, reg_t2);
            add(reg_t1, rs, rt);
            strcpy_s(rs, reg_t1);
            if (strcmp(tab[array_index].parent, string_global) == 0)
                add(rs, rs, reg_t9);
            else
                add(rs, rs, reg_fp);
            sw(rd, rs, 0);
        }
        else if (strcmp(midcode[i].op, "=[]") == 0)
        {
            //���������׵�ַ��������������ƫ�ƣ�lw sw
            //��ֵ��������Ԫ��->��ֵ
            char rs[REG_NAME_SIZE];
            char rt[REG_NAME_SIZE];
            int array_index;
            bool rt_is_imm = false;
            int index = getIndexByNameAndParent(parent, midcode[i].rs, search_mode);
            array_index = index;
            if (index != -1 && tab[index].reg[0] != '\0')
            {
                if (initedArrayName.count(midcode[i].rs) == 0)
                {
                    addi(tab[index].reg, reg_$0, tab[index].address);
                    initedArrayName.insert(midcode[i].rs);
                }
                strcpy_s(rs, tab[index].reg);
            }
            else
            {
                addi(reg_t1, reg_$0, tab[index].address);
                strcpy_s(rs, reg_t1);
            }

            index = getIndexByNameAndParent(parent, midcode[i].rt, search_mode);
            if (index != -1 && tab[index].reg[0] != '\0')
            {
                strcpy_s(rt, tab[index].reg);
            }
            else
            {
                if (opt_sign && (isdigit(midcode[i].rt[0]) || (midcode[i].rt[0] == '-' && isdigit(midcode[i].rt[1]))))//��ʶ�������������ֿ�ͷ���������ֿ�ͷ��һ���ǳ���
                    rt_is_imm = true;
                else
                {
                    loadValue2reg(parent, midcode[i].rt, reg_t2);
                    strcpy_s(rt, reg_t2);
                }
            }

            if (rt_is_imm)
            {
                int addr = atoi(midcode[i].rt);
                addr = addr * 4;
                addi(reg_t2, reg_$0, addr);
            }
            else
                sll(reg_t2, rt, 2);

            strcpy_s(rt, reg_t2);
            add(reg_t1, rs, rt);
            strcpy_s(rs, reg_t1);
            if (strcmp(tab[array_index].parent, string_global) == 0)
                add(rs, rs, reg_t9);
            else
                add(rs, rs, reg_fp);

            index = getIndexByNameAndParent(parent, midcode[i].rd, search_mode);
            if (index != -1 && tab[index].reg[0] != '\0')
            {
                lw(tab[index].reg, rs, 0);
            }
            else
            {
                lw(reg_t0, rs, 0);
                saveValue2mem(parent, midcode[i].rd, reg_t0);
            }
        }
        else if (strcmp(midcode[i].op, str_prt) == 0)
        {
            //addi $v0 $0 [����]�����ģʽ���ַ�or���֣�
            //lw
            //syscall
            if (strcmp(midcode[i].rt, str_char) == 0)
                addi(reg_v0, reg_$0, 11);
            else if (strcmp(midcode[i].rt, str_int) == 0)
                addi(reg_v0, reg_$0, 1);
            else
                addi(reg_v0, reg_$0, 4);

            if ((strcmp(midcode[i].rt, str_char) == 0) || (strcmp(midcode[i].rt, str_int) == 0))
            {
                int index = getIndexByNameAndParent(parent, midcode[i].rs, search_mode);
                if (index != -1 && tab[index].reg[0] != '\0')
                    add(reg_a0, tab[index].reg, reg_$0);
                else 
                    loadValue2reg(parent, midcode[i].rs, reg_a0);
            }
            else
            {
                int str_index;
                for (str_index = 0; str_index <= s; str_index++)
                {
                    if (strcmp(stab[str_index].name, midcode[i].rs) == 0)
                        break;
                }
                if (str_index > s)
                {
                    cout << "�м�����" << i << "���ַ������ִ���" << midcode[i].rs << endl;
                }

                if (!opt_sign)
                    lui(reg_k0, 0x1001);
                addi(reg_a0, reg_k0, stab[str_index].address);
            }

            syscall();
        }
        else if (strcmp(midcode[i].op, str_scf) == 0)//rd�ֶα�ʾ����ֵ������rs�ֶα�ʾ��������
        {
            //addi $v0 $0 [����]������ģʽ���ַ�or���֣�
            //syscall
            if (strcmp(midcode[i].rs, str_char) == 0)
                addi(reg_v0, reg_$0, 12);
            else
                addi(reg_v0, reg_$0, 5);

            syscall();
            
            int index = getIndexByNameAndParent(parent, midcode[i].rd, search_mode);
            if (index != -1 && tab[index].reg[0] != '\0')
                add(tab[index].reg, reg_v0, reg_$0);
            else
                saveValue2mem(parent, midcode[i].rd, reg_v0);
        }
        else if (strcmp(midcode[i].op, str_func_def) == 0)
        {
            //label:
            memset(parent, 0, sizeof(parent));
            initedArrayName.clear();
            strcpy_s(parent, midcode[i].rt);
            generate_label(midcode[i].rt);
            if (opt_sign)
            {
                if (strcmp(parent, string_main) != 0)
                    sw(reg_ra, reg_fp, 8);//���淵�ص�ַ��main����û��ϵ��������ı�������⸲�ǵ�������main����Ҫ���淵��ֵ
            }
            else
                sw(reg_ra, reg_fp, 8);
        }
        else if (strcmp(midcode[i].op, str_para) == 0)
        {
            int index = getIndexByNameAndParent(parent, midcode[i].rt, search_mode);

            if (index != -1 && tab[index].reg[0] != '\0')
            {
                lw(tab[index].reg, reg_gp, -4);
                addi(reg_gp, reg_gp, -4);
            }
            else
            {
                lw(reg_t0, reg_gp, -4);
                addi(reg_gp, reg_gp, -4);
                sw(reg_t0, reg_fp, tab[index].address);
            }
        }
        else if (strcmp(midcode[i].op, str_label) == 0)
        {
            //�����ǩ
            generate_label(midcode[i].rd);
        }
        else if (strcmp(midcode[i].op, str_nop) == 0)
        {
            //do nothing
        }
        else
        {
            cout << "midcode2mips�м�⵽�Ƿ���op:" << midcode[i].op << endl;
        }
    }
    printf("mips������������ϣ����ɵ�mips���뱣����mips.asm��\n");
}
void NaiveCompiler::nextch()
{
    if (cc == ll)
    {
        if (fin.eof())
        {
            cout << "��������" << endl;
            exit(0);
        }

        ll = -1;
        cc = -1;
        memset(line, ' ', sizeof(line));
        fin.getline(line, sizeof(line));
        ln += 1;
        if (fin.fail())
        {
            printf("�����ļ���ȡʧ�� \n");
        }
        while (ll < LLNG)
        {
            ll += 1;
            if ((line[ll] == '\0') || (line[ll] == '\n'))
            {
                line[ll] = ' ';
                break;
            }
                
        }
    }

    cc += 1;
    ch = line[cc];
}
void NaiveCompiler::one_case(char (&parent)[ALNG], types typ, char (&end)[ALNG], char (&rs)[ALNG], set<int> (&exist_const), bool &has_return)
{
    char nextCase[ALNG];
    char rt[ALNG];
    strcpy_s(nextCase, "fkd_nextcase");
    fcount += 1;
    _itoa_s(fcount, &nextCase[12], ALNG-13, 10);

    int const_value = 0;
    insymbol(symbols[0]);

    if (f_const.count(symbols[0].sy))
    {
        const_value = constant(typ);
        _itoa_s(const_value, rt, 10);
        if (exist_const.count(const_value))
            error("��⵽case����г����ظ��ĳ���ֵ");
        else
            exist_const.insert(const_value);
    }
    else
        error("case����ȱ�ٳ���");

    append_midcode("==", nullptr, rs, rt);
    append_midcode(str_BZ, nextCase);

    if (symbols[0].sy == colon)
        insymbol(symbols[0]);
    else
        error("ȱ�� :");

    if (f_statement.count(symbols[0].sy))
    {
        statement(parent, has_return);
        append_midcode(str_GOTO, nullptr, end);
    }
    else
        error("���ڷǷ����");

    append_midcode(str_label, nextCase);
    programma_info << "����һ�����������䡱" << endl;
}
void NaiveCompiler::parameter_list(char (&parent)[ALNG])
{
    
    types typ = notyp;
    char para_name[ALNG] = "";

    if (symbols[0].sy == intsy || symbols[0].sy == charsy)
    {
        //�Ǽǲ�������
        typ = symbols[0].sy == intsy  ? ints  :
              symbols[0].sy == charsy ? chars : notyp ;
        insymbol(symbols[0]);
        if (symbols[0].sy == ident)
        {
            //�Ǽǲ�����
            strcpy_s(para_name, symbols[0].id);
            insymbol(symbols[0]);
        }
        else
            error("��⵽�Ƿ�������");

        //������ű�
        if (strcmp(para_name, "") && typ != notyp)
        {
            if (t == TABLE_SIZE - 1)
            {
                //fatal
                fatal("���ű���������");
            }
            t += 1;

            strcpy_s(tab[t].name, para_name);
            tab[t].obj = para;
            tab[t].typ = typ;
            strcpy_s(tab[t].parent, parent);
            tab[t].address = dx;
            dx += 4;

            //��Ӧ�����Ĳ�������+1
            int func_index = getIndexByNameAndParent(string_global, parent, search_mode);
            if (func_index == -1)
                error("�Ҳ�������");
            else
                ftab[tab[func_index].ref].paraNum += 1;

            if (typ == ints)
                append_midcode(str_para, nullptr, str_int, para_name);
            else if (typ == chars)
                append_midcode(str_para, nullptr, str_char, para_name);
        }

        while (symbols[0].sy == comma)
        {
            strcpy_s(para_name, "");
            typ = notyp;

            insymbol(symbols[0]);
            if (symbols[0].sy == intsy || symbols[0].sy == charsy)
            {
                //�Ǽǲ�������
                typ = symbols[0].sy == intsy  ? ints  :
                    symbols[0].sy == charsy ? chars : notyp ;
                insymbol(symbols[0]);
                if (symbols[0].sy == ident)
                {
                    //�Ǽǲ�����
                    strcpy_s(para_name, symbols[0].id);
                    insymbol(symbols[0]);
                    
                }
                else
                    error("��⵽�Ƿ�������");

                if (getIndexByNameAndParent(parent, symbols[0].id, declare_mode) == -1)
                {
                    //������ű�
                    if (strcmp(para_name, "") && typ != notyp)
                    {
                        if (t == TABLE_SIZE - 1)
                        {
                            //fatal
                            fatal("���ű���������");
                        }
                        t += 1;

                        strcpy_s(tab[t].name, para_name);
                        tab[t].obj = para;
                        tab[t].typ = typ;
                        strcpy_s(tab[t].parent, parent);
                        tab[t].address = dx;
                        dx += 4;

                        //��Ӧ�����Ĳ�������+1
                        int func_index = getIndexByNameAndParent(string_global, parent, search_mode);
                        if (func_index == -1)
                            error("�Ҳ�������");
                        else
                            ftab[tab[func_index].ref].paraNum += 1;

                        if (typ == ints)
                            append_midcode(str_para, nullptr, str_int, para_name);
                        else if (typ == chars)
                            append_midcode(str_para, nullptr, str_char, para_name);

                    }
                }
                else
                    error("�������ظ�");
            }
            else
                error("���ź�����ַǷ����͵Ĳ���");
        }
    }
    //����Ҫelse����Ϊ���������Ϊ��

    programma_info << "����һ����������" << endl;
}
void NaiveCompiler::printf_statement(char (&parent)[ALNG])
{
    
    types typ;
    char rs[ALNG];
    char rt[ALNG];
    insymbol(symbols[0]);
    if (symbols[0].sy == lparent)
        insymbol(symbols[0]);
    else
        error("ȱ�� (");

    if (symbols[0].sy == stringcon)
    {
        char str_name[ALNG];
        strcpy_s(str_name, "fangkedong_str");
        fcount += 1;
        _itoa_s(fcount, &str_name[14], ALNG-15, 10);
        mipsfile << str_name << ":  .asciiz\"";
        for (int i=0; i<symbols[0].sleng; i++)
        {
            if (symbols[0].strval[i] == '\\')
            {
                mipsfile << "\\";
            }
            mipsfile << symbols[0].strval[i];
        }
        mipsfile << "\"" << endl;

        s += 1;
        strcpy_s(stab[s].name, str_name);
        stab[s].length = 1 + symbols[0].sleng;
        if (s == 0)
            stab[s].address = 0;
        else 
            stab[s].address = stab[s - 1].address + stab[s - 1].length;
        
        //ȡ��stab�����ַ������ݱ�����symbol_record�����У���Ҫʱֱ�������
        insymbol(symbols[0]);
        if (symbols[0].sy == comma)
        {
            insymbol(symbols[0]);
            if (f_expression.count(symbols[0].sy))
            {
                char exp_rs[ALNG];
                char exp_rt[ALNG];
                expression(parent, exp_rs, typ);
                if (typ == chars)
                    strcpy_s(exp_rt, str_char);
                else
                    strcpy_s(exp_rt, str_int);

                append_midcode(str_prt, nullptr, str_name, str_string);

                append_midcode(str_prt, nullptr, exp_rs, exp_rt);
            }
            else
                error("���ַǷ��ı��ʽ");
        }
        else//����û�ж��źͱ��ʽ��ֻ���ַ���
        {
            append_midcode(str_prt, nullptr, str_name, str_string);
        }
        

        if (symbols[0].sy == rparent)
            insymbol(symbols[0]);
        else
            error("ȱ�� )");
    }
    else if (f_expression.count(symbols[0].sy))
    {
        expression(parent, rs, typ);
        if (typ == chars)
            strcpy_s(rt, str_char);
        else
            strcpy_s(rt, str_int);
        append_midcode(str_prt, nullptr, rs, rt);

        if (symbols[0].sy == rparent)
            insymbol(symbols[0]);
        else
            error("ȱ�� )");
    }
    else
        error("��⵽�Ƿ����������");

    programma_info << "����һ���������䡱" << endl;
}
void NaiveCompiler::program()
{
    dx = 0;
    global_dx = 0;
    //��������������
    if (f_const_declare.count(symbols[0].sy))//count����ֵ����0˵��sy���ڼ���
        const_declare(string_global);

    //��������������������ǰ���������ţ��鿴�Ǳ������������з���ֵ�ĺ�������
    if (f_var_declare.count(symbols[0].sy))
    {
        insymbol(symbols[1]);
        end++;
        if (symbols[1].sy != ident)
            error("��⵽�Ƿ��ĺ�����/������");
        insymbol(symbols[2]);
        end++;
        

        if (symbols[2].sy == semicomma || symbols[2].sy == comma || symbols[2].sy == lsquare)
        {
            //������������������ʱ�Ѷ��������ַ�
            var_declare(string_global);
            global_dx = dx;
        }

        if (end == 2)
        {
            //��˱�������Ϊ�պͱ�������������ž����з���ֵ������������������������������ǰ����3��symbol
            if (symbols[2].sy == lparent)
            {
                return_func_declare();
            }
            else
            {
                //�������ֵȷ��������һ������ʱֻ��symbols[0]��ֵ����Ч��
                symbols[0].clear();
                symbols[1].clear();
                symbols[2].clear();
                end = 0;
                insymbol(symbols[0]);
            }
        }
        else if (end == 0)
        {

        }
        else
            error("program�������������δ���ǵ����");
        
    }

    while (f_return_func_declare.count(symbols[0].sy)
        || f_void_func_declare.count(symbols[0].sy))
    {
        if (symbols[0].sy == intsy || symbols[0].sy == charsy)
            return_func_declare();
        else
        {
            insymbol(symbols[1]);
            end = 1;
            if (symbols[1].sy == mainsy)
                break;
            else if (symbols[1].sy == ident)
                void_func_declare();
            else
                error("��⵽�Ƿ��ĺ�����");
        }
    }

    if (symbols[0].sy == voidsy && symbols[1].sy == mainsy)
    {
        void_main();
    }
    else
        error("ȱ��������");
    programma_info << "����һ��������" << endl;
}
void NaiveCompiler::return_func_declare()
{
    
    dx = RESERVE_SPACE;
    char func_name[ALNG] = "";
    types return_type = notyp;
    bool has_return = false;

    if (end == 2)
    {
        //�ǼǺ������ͷ���ֵ����
        strcpy_s(func_name, symbols[1].id);
        return_type = symbols[0].sy == intsy  ? ints  :
                      symbols[0].sy == charsy ? chars : notyp ;
        symbols[0].clear();
        symbols[1].clear();
        symbols[2].clear();
        end = 0;
        insymbol(symbols[0]);
    }
    else
    {
        //�ǼǷ���ֵ����
        return_type = symbols[0].sy == intsy  ? ints  :
                      symbols[0].sy == charsy ? chars : notyp ;
        insymbol(symbols[0]);
        if (symbols[0].sy == ident)
        {
            //�ǼǺ�������
            strcpy_s(func_name, symbols[0].id);
            insymbol(symbols[0]);
        }
        else
            error("��⵽�Ƿ��ĺ�����");

        if (symbols[0].sy == lparent)
            insymbol(symbols[0]);
        else
            error("ȱ�� (");
    }

    if (strcmp(func_name, "") && return_type != notyp 
     && getIndexByNameAndParent(string_global, func_name, declare_mode) == -1)
    {
        //�ǼǺ������ƺͷ���ֵ
        if (t == TABLE_SIZE - 1)
        {
            //fatal,���ű������������˳�
            fatal("���ű���������");
        }
        if (f == FUNC_TABLE_SIZE- 1)
        {
            //fatal,���ű������������˳�
            fatal("��������������");
        }
        t += 1;
        f += 1;

        strcpy_s(tab[t].name, func_name);
        tab[t].obj = func;
        tab[t].typ = return_type;
        strcpy_s(tab[t].parent, string_global);
        tab[t].ref = f;
        ftab[f].paraNum = 0;
        ftab[f].space = RESERVE_SPACE;

        if (return_type == ints)
            append_midcode(str_func_def, nullptr, str_int, func_name);
        else if (return_type == chars)
            append_midcode(str_func_def, nullptr, str_char, func_name);
        
    }

    parameter_list(func_name);//����Ϊ�գ��ʲ����жϿ�ʼ���ż�

    if (symbols[0].sy == rparent)
        insymbol(symbols[0]);
    else
        error("ȱ�� )");

    if (symbols[0].sy == lbrack)
        insymbol(symbols[0]);
    else
        error("ȱ�� {");

    compound_statement(func_name, has_return);
    if (has_return == false)
        error("����ȱ�ٷ���ֵ");



    if (symbols[0].sy == rbrack)
        insymbol(symbols[0]);
    else
        error("ȱ�� }");

    programma_info << "����һ�����з���ֵ����������" << endl;
}
void NaiveCompiler::return_statement(char (&parent)[ALNG])
{
    types typ;
    char rs[ALNG];
    insymbol(symbols[0]);
    if (symbols[0].sy == lparent)
    {
        int index = getIndexByNameAndParent(string_global, parent, search_mode);
        if (tab[index].typ == notyp)
            error("�޷���ֵ�����ķ�����䲻�ܰ��� ( ");

        insymbol(symbols[0]);
        if (f_expression.count(symbols[0].sy))
        {
            expression(parent, rs, typ);
            if (tab[index].typ == chars && typ == ints)
                error("��������ʱ����ֵ����Ϊchar������������б��ʽ����Ϊint");
            append_midcode(str_ret, nullptr, rs);
        }
        else
            error("��⵽�Ƿ��ı��ʽ");

        if (symbols[0].sy == rparent)
            insymbol(symbols[0]);
        else
            error("ȱ�� )");
    }
    else if (symbols[0].sy == semicomma)
    {
        int index = getIndexByNameAndParent(string_global, parent, search_mode);
        if (tab[index].typ != notyp)
            error("�з���ֵ�����ķ������ȱ�� ( ");

        if (strcmp(parent, string_main) == 0)//���������������û����һ�������ˣ�ֱ����������ĩ��
            append_midcode(str_GOTO, nullptr, "fkd_program_end");
        else
            append_midcode(str_ret);
    }
        
    //����ֻ��һ��return���ٶ�һ���ַ��Ѿ����������ķ�Χ�ˣ����Բ��ñ���
    programma_info << "����һ����������䡱" << endl;
}
void NaiveCompiler::saveValue2mem(char (&parent)[ALNG], char (&name)[ALNG], char* src_reg)
{
    char src[ALNG];
    if (strcmp(src_reg, reg_lo) == 0)
    {
        mflo(reg_t0);
        strcpy_s(src, reg_t0);
    }
    else
        strcpy_s(src, src_reg);

    int index;
    index = getIndexByNameAndParent(parent, name, search_mode);
    if (index == -1)
    {
        //˵�����Ǿֲ�������ȫ�ֱ��������Ǹ�����������ջ����
        sw(src, reg_sp, 0);
        addi(reg_sp, reg_sp, -4);
    }
    else
    {
        if (strcmp(tab[index].parent, string_global) == 0)
            sw(src, reg_t9, tab[index].address);
        else
            sw(src, reg_fp, tab[index].address);
    }
}
void NaiveCompiler::scanf_statement(char (&parent)[ALNG])
{
    
    char rd[ALNG];
    char rs[ALNG];
    int index = 0;
    insymbol(symbols[0]);
    if (symbols[0].sy == lparent)
        insymbol(symbols[0]);
    else
        error("ȱ�� (");

    if (symbols[0].sy == ident)
    {
        //�����ʶ��
        index = getIndexByNameAndParent(parent, symbols[0].id, search_mode);
        if (index == -1)
        {
            string msg = symbols[0].id;
            msg.append("������");
            error(msg);
        }

        if (tab[index].obj != var)
        {
            //�������Ǳ���
            string msg = tab[index].name;
            msg.append("���Ǳ���");
            error(msg);
        }

        if (tab[index].typ == chars)
            strcpy_s(rs, str_char);
        else if (tab[index].typ == ints)
            strcpy_s(rs, str_int);
        else
        {
            string msg = tab[index].name;
            msg.append("�Ȳ����ַ���Ҳ�������ͣ���������޷�Ϊ֮��ֵ");
            error(msg);
        }

        strcpy_s(rd, tab[index].name);
        append_midcode(str_scf, rd, rs);

        insymbol(symbols[0]);
        while (symbols[0].sy == comma)
        {
            insymbol(symbols[0]);
            if (symbols[0].sy == ident)
            {
                //�����ʶ��
                index = getIndexByNameAndParent(parent, symbols[0].id, search_mode);
                if (index == -1)
                {
                    string msg = symbols[0].id;
                    msg.append("������");
                    error(msg);
                }

                if (tab[index].obj != var)
                {
                    string msg = tab[index].name;
                    msg.append("���Ǳ���");
                    error(msg);
                }

                if (tab[index].typ == chars)
                    strcpy_s(rs, str_char);
                else if (tab[index].typ == ints)
                    strcpy_s(rs, str_int);
                else
                {
                    string msg = tab[index].name;
                    msg.append("�Ȳ����ַ���Ҳ�������ͣ���������޷�Ϊ֮��ֵ");
                    error(msg);
                }

                strcpy_s(rd, tab[index].name);
                append_midcode(str_scf, rd, rs);

                insymbol(symbols[0]);
            }
            else
            {
                //����
                error("���ź���ȱ�ٲ���");
            }
        }
    }
    else
        error("scanfӦ������һ������");

    if (symbols[0].sy == rparent)
        insymbol(symbols[0]);
    else
        error("ȱ�� )");

    programma_info << "����һ����������䡱" << endl;
}
void NaiveCompiler::skip(set<symbol> begin_symbol_set)
{
    while (begin_symbol_set.count(symbols[0].sy) == 0)
        insymbol(symbols[0]);
}
void NaiveCompiler::statement(char (&parent)[ALNG], bool &has_return)
{
    
    if (f_if.count(symbols[0].sy))
    {
        //�����������
        if_statement(parent, has_return);
    }
    else if (f_do_while.count(symbols[0].sy))
    {
        //����ѭ�����
        do_while_statement(parent, has_return);
    }
    else if (f_switch.count(symbols[0].sy))
    {
        //����������
        switch_statement(parent, has_return);
    }
    else if (symbols[0].sy == lbrack)
    {
        insymbol(symbols[0]);
        statement_column(parent, has_return);
        if (symbols[0].sy == rbrack)
        {
            insymbol(symbols[0]);
        }
        else
            error("ȱ�� }");
    }
    else if (symbols[0].sy == ident)
    {
        insymbol(symbols[1]);
        end = 1;
        if (symbols[1].sy == becomesy || symbols[1].sy == lsquare)
        {
            //����ֵ���
            assign_statement(parent);
        }
        else if (symbols[1].sy == lparent)
        {
            int index = 0;
            //�����з���ֵ���������޷���ֵ�������
            //����õ�����ֵ����
            index = getIndexByNameAndParent(string_global, symbols[0].id, search_mode);
            if (index == -1)
            {
                //����������
                string msg = symbols[0].id;
                msg.append("������");
                error(msg);
            }
            else if (tab[index].obj != func)
            {
                //�������Ǻ���
                string msg = tab[index].name;
                msg.append("���Ǻ��������Ͳ���");
                error(msg);
            }
            else
            {
                //�����void
                //call_void_func();
                //�������void
                //call_return_func();
                if (tab[index].typ == notyp)
                {
                    call_void_func(parent);
                }
                else
                {
                    char rd[ALNG];
                    types typ;
                    call_return_func(parent, rd, typ);
                }
            }
        }
        else
            error("��⵽�Ƿ����");

        if (symbols[0].sy == semicomma)
            insymbol(symbols[0]);
        else
            error("ȱ�� ;");
    }
    else if (f_scanf.count(symbols[0].sy))
    {
        //��������
        scanf_statement(parent);

        if (symbols[0].sy == semicomma)
            insymbol(symbols[0]);
        else
            error("ȱ�� ;");
    }
    else if (f_printf.count(symbols[0].sy))
    {
        //����д���
        printf_statement(parent);

        if (symbols[0].sy == semicomma)
            insymbol(symbols[0]);
        else
            error("ȱ�� ;");
    }
    else if (f_return.count(symbols[0].sy))
    {
        //���������
        return_statement(parent);
        has_return = true;
        if (symbols[0].sy == semicomma)
            insymbol(symbols[0]);
        else
            error("ȱ�� ;");
    }
    else if (symbols[0].sy == semicomma)
    {
        //�����
        programma_info << "����һ��������䡱" << endl;
        insymbol(symbols[0]);
    }
    else
        error("��⵽�Ƿ����");

    programma_info << "����һ������䡱" << endl;
}
void NaiveCompiler::statement_column(char (&parent)[ALNG], bool &has_return)
{
    
    while (f_statement.count(symbols[0].sy))
    {
        statement(parent, has_return);
    }
    //�п���Ϊ�գ�����Ҫelse����

    programma_info << "����һ��������С�" << endl;
}
void NaiveCompiler::switch_statement(char (&parent)[ALNG], bool &has_return)
{
    
    types typ;
    char rs[ALNG];
    insymbol(symbols[0]);
    if (symbols[0].sy == lparent)
        insymbol(symbols[0]);
    else
        error("ȱ�� (");

    if (f_expression.count(symbols[0].sy))
    {
        expression(parent, rs, typ);
    }
    else
        error("��⵽�Ƿ����ʽ");

    if (symbols[0].sy == rparent)
        insymbol(symbols[0]);
    else
        error("ȱ�� )");

    if (symbols[0].sy == lbrack)
        insymbol(symbols[0]);
    else
        error("ȱ�� {");

    if (f_caselist.count(symbols[0].sy))
    {
        caselist(parent, typ, rs, has_return);
    }
    else
        error("��⵽�Ƿ��������");

    if (symbols[0].sy == rbrack)
        insymbol(symbols[0]);
    else
        error("ȱ�� }");

    programma_info << "����һ���������䡱" << endl;
}
void NaiveCompiler::value_parameter_list(char (&parent)[ALNG], char (&called_func)[ALNG])
{
    int count = 0;
    types typ;
    char rs[20][ALNG];

    int func_index = getIndexByNameAndParent(string_global, called_func, search_mode);
    if (func_index == -1)
        error("�Ҳ���ֵ���������ĺ���");
    

    if (f_expression.count(symbols[0].sy))
    {
        //������ʽ
        expression(parent, rs[count], typ);
        count += 1;
        if (count <= ftab[tab[func_index].ref].paraNum)
        {
            if (tab[func_index+count].typ != typ)
                error("ʵ�����β����Ͳ���");
        }
        else
            error("ʵ�θ�������");
        
        //append_midcode(str_push, nullptr, rs);
        
        while (symbols[0].sy == comma)
        {
            insymbol(symbols[0]);
            if (f_expression.count(symbols[0].sy))
            {
                expression(parent, rs[count], typ);
                count += 1;
                if (count <= ftab[tab[func_index].ref].paraNum)
                {
                    if (tab[func_index+count].typ == chars && typ == ints)
                        error("���ܽ����͵�ֵ�����ַ��͵Ĳ���");
                }
                else
                    error("ʵ�θ�������");
                //append_midcode(str_push, nullptr, rs);
            }
            else
                error("��⵽�Ƿ����ʽ");
        }

        for (int i=count-1; i>=0; i--)
            append_midcode(str_push, nullptr, rs[i]);
    }
    //����Ϊ��,�ʲ��ر���

    //����βθ�����ʵ�θ����Ƿ����
    if (func_index != -1)
    {
        if (count < ftab[tab[func_index].ref].paraNum)
            error("����ʱ���ݵĲ�������");
    }
    programma_info << "����һ����ֵ������" << endl;
}
//ÿ�δ�����һ���������壬��Ҫ��ǰ��3��symbol������������з���ֵ��������������������������޷���ֵ������ֱ�ӷ��أ���������ǰ��3��symbol
void NaiveCompiler::var_declare(char (&parent)[ALNG])
{
    
var_declare_label:
    var_define(parent);//var_declare��var_define�Ŀ�ʼ���ż�һ�£��������ж�
    
    if (end == 0)
    {
        //����û��Ԥ�����������
        if (symbols[0].sy == semicomma)
            insymbol(symbols[0]);
        else
            error("ȱ�� ;");
    }
    else
        error("����δ���ǵ����");

    if (f_var_define.count(symbols[0].sy))
    {
        insymbol(symbols[1]);
        end++;
        if (symbols[1].sy != ident)
            error("��⵽�Ƿ��ĺ�����/������");
        insymbol(symbols[2]);
        end++;

        if (symbols[2].sy == semicomma || symbols[2].sy == comma || symbols[2].sy == lsquare)
        {
            goto var_declare_label;
        }
    }
    programma_info << "����һ��������˵����" << endl;
}
void NaiveCompiler::var_define(char (&parent)[ALNG])
{
    
    types typ = notyp;
    char var_name[ALNG] = "";
    objecttyp obj = var;//�п�����var���򵥱�������array�����飩
    int array_size = 0;

    if (end == 2)
    {
        //�ǼǷ��ű�,���ֺ�����
        typ = symbols[0].sy == charsy   ? chars :
              symbols[0].sy == intsy    ? ints  : notyp ;
        symbols[0].clear();

        if (strcmp(parent, symbols[1].id) == 0 && strcmp(parent, string_global) != 0)
            error("�������������ں�������");
        else
        {
            //�ǼǱ�������
            strcpy_s(var_name, symbols[1].id);
        }

        symbols[1].clear();

        if (symbols[2].sy == lsquare)
        {
            //��symbols[2].sy == lsquare, �Ǽ�����Ϊ����
            obj = arr;
            symbols[2].clear();
            end = 0;
            insymbol(symbols[0]);
            if (symbols[0].sy == intcon)
            {
                if (symbols[0].inum != 0)
                {
                    //�Ǽ�����size
                    array_size = (int)symbols[0].inum;
                    insymbol(symbols[0]);
                }
                else
                    error("�������Ԫ����ĿΪ0");
            }
            else
                error("��⵽�Ƿ�������Ԫ�ظ���");

            if (symbols[0].sy == rsquare)
                insymbol(symbols[0]);
            else
                error("ȱ�� ]");
        }

        //������ű�
        if (strcmp(var_name, "") && typ != notyp)
        {
            if (getIndexByNameAndParent(parent, var_name, declare_mode) == -1)
            {
                if (t == TABLE_SIZE - 1)
                {
                    //fatal,���ű���������
                    fatal("���ű���������");
                }

                t += 1;
                if (obj == arr)
                {
                    if (a == ARRAY_TABLE_SIZE - 1)
                    {
                        //fatal���������������
                        fatal("�������������");
                    }
                    //¼����ű�
                    strcpy_s(tab[t].name, var_name);
                    tab[t].obj = obj;
                    strcpy_s(tab[t].parent, parent);
                    
                    //¼�������
                    a += 1;
                    tab[t].ref = a;//������������е�����
                    atab[a].size = array_size;
                    atab[a].elementType = typ;
                    atab[a].elementSize = 4;
                    tab[t].address = dx;
                    dx = dx + atab[a].elementSize * array_size;

                    if (typ == chars)
                        midcodefile << "var char " << var_name << "[" << array_size << "]" << endl;
                    else if (typ == ints)
                        midcodefile << "var int " << var_name << "[" << array_size << "]" << endl;
                }
                else//obj == var
                {
                    strcpy_s(tab[t].name, var_name);
                    tab[t].obj = obj;
                    tab[t].typ = typ;
                    strcpy_s(tab[t].parent, parent);
                    tab[t].address = dx;
                    dx = dx + 4;

                    if (typ == chars)
                        midcodefile << "var char " << var_name << endl;
                    else if (typ == ints)
                        midcodefile << "var int " << var_name << endl;
                }
            }
            else
            {
                string msg = var_name;
                msg.append("�ظ�����");
                error(msg);
            }
        }

        if (obj == arr)
        {
            if (symbols[0].sy == comma)
            {
                insymbol(symbols[0]);
                goto var_define_label;
            }
            //else�����Ϊ�ֺţ�ֱ���˳�����
        }
        else if (symbols[2].sy == comma)
        {
            symbols[2].clear();
            end = 0;
            insymbol(symbols[0]);
            goto var_define_label;
        }
        //else�����Ϊ�ֺţ�ֱ���˳�����
        else
        {
            symbols[2].copy(symbols[0]);
            end = 0;
            symbols[2].clear();
        }
    }
    else//end == 0
    {
        //�ǼǷ�������
        typ = symbols[0].sy == charsy   ? chars :
              symbols[0].sy == intsy    ? ints  : notyp ;
        insymbol(symbols[0]);

var_define_label:
        obj = var;
        memset(var_name, 0, sizeof(var_name));
        if (symbols[0].sy == ident)
        {
            if (strcmp(parent, symbols[0].id) == 0 && strcmp(parent, string_global) != 0)
                error("�������������ں�������");
            else
            {
                //�ǼǷ�������
                strcpy_s(var_name, symbols[0].id);
                insymbol(symbols[0]);
            }
        }
        else
            error("��⵽�Ƿ��ı�����");

        if (symbols[0].sy == lsquare)
        {
            //�Ǽ�����
            obj = arr;
            insymbol(symbols[0]);
            if (symbols[0].sy == intcon)
            {
                if (symbols[0].inum != 0)
                {
                    //�Ǽ�����size
                    array_size = (int)symbols[0].inum;
                    insymbol(symbols[0]);
                }
                else
                    error("�������Ԫ����ĿΪ0");
            }
            else
                error("��⵽�Ƿ�������Ԫ�ظ���");

            if (symbols[0].sy == rsquare)
                insymbol(symbols[0]);
            else
                error("ȱ�� ]");
        }

        //������ű�
        if (strcmp(var_name, "") && typ != notyp)
        {
            if (getIndexByNameAndParent(parent, var_name, declare_mode) == -1)
            {
                if (t == TABLE_SIZE - 1)
                {
                    //fatal,���ű���������
                    fatal("���ű���������");
                }

                t += 1;
                if (obj == arr)
                {
                    if (a == ARRAY_TABLE_SIZE - 1)
                    {
                        //fatal���������������
                        fatal("�������������");
                    }
                    //¼����ű�
                    strcpy_s(tab[t].name, var_name);
                    tab[t].obj = obj;
                    strcpy_s(tab[t].parent, parent);

                    //¼�������
                    a += 1;
                    tab[t].ref = a;//������������е�����
                    atab[a].size = array_size;
                    atab[a].elementType = typ;
                    atab[a].elementSize = 4;
                    tab[t].address = dx;
                    dx = dx + atab[a].elementSize * array_size;

                    if (typ == chars)
                        midcodefile << "var char " << var_name << "[" << array_size << "]" << endl;
                    else if (typ == ints)
                        midcodefile << "var int " << var_name << "[" << array_size << "]" << endl;
                }
                else//obj == var
                {
                    strcpy_s(tab[t].name, var_name);
                    tab[t].obj = obj;
                    tab[t].typ = typ;
                    strcpy_s(tab[t].parent, parent);
                    tab[t].address = dx;
                    dx = dx + 4;

                    if (typ == chars)
                        midcodefile << "var char " << var_name << endl;
                    else if (typ == ints)
                        midcodefile << "var int " << var_name << endl;
                }
            }
            else
            {
                string msg = var_name;
                msg.append("�ظ�����");
                error(msg);
            }
        }

        if (symbols[0].sy == comma)
        {
            insymbol(symbols[0]);
            goto var_define_label;
        }
    }
    programma_info << "����һ�����������塱" << endl;
}
void NaiveCompiler::void_func_declare()
{
    
    dx = RESERVE_SPACE;
    char func_name[ALNG] = "";

    if (end == 1)
    {
        //�ǼǺ�������
        strcpy_s(func_name, symbols[1].id);
        symbols[0].clear();
        symbols[1].clear();
        end = 0;
        insymbol(symbols[0]);
    }
    else
    {
        insymbol(symbols[0]);
        if (symbols[0].sy == ident)
        {
            //�ǼǺ�������
            strcpy_s(func_name, symbols[0].id);
            insymbol(symbols[0]);
        }
        else
            error("��⵽�Ƿ��ĺ�����");
    }
    
    if (symbols[0].sy == lparent)
        insymbol(symbols[0]);
    else
        error("ȱ�� (");

    if (strcmp(func_name, "") 
        && getIndexByNameAndParent(string_global, func_name, declare_mode) == -1)
    {
        //�ǼǺ������ƺͷ���ֵ
        if (t == TABLE_SIZE - 1)
        {
            //fatal,���ű������������˳�
            fatal("���ű���������");
        }
        if (f == FUNC_TABLE_SIZE - 1)
        {
            //fatal,�����������������˳�
            fatal("��������������");
        }
        t += 1;
        f += 1;

        strcpy_s(tab[t].name, func_name);
        tab[t].obj = func;
        tab[t].typ = notyp;
        strcpy_s(tab[t].parent, string_global);
        tab[t].ref = f;
        ftab[tab[t].ref].paraNum = 0;

        append_midcode(str_func_def, nullptr, "void", func_name);
    }

    parameter_list(func_name);//����Ϊ�գ��ʲ����жϿ�ʼ���ż�

    if (symbols[0].sy == rparent)
        insymbol(symbols[0]);
    else
        error("ȱ�� )");

    if (symbols[0].sy == lbrack)
        insymbol(symbols[0]);
    else
        error("ȱ�� {");

    bool has_return=false;
    compound_statement(func_name, has_return);
    append_midcode(str_ret);    

    if (symbols[0].sy == rbrack)
        insymbol(symbols[0]);
    else
        error("ȱ�� }");

    programma_info << "����һ�����޷���ֵ����������" << endl;
}
void NaiveCompiler::void_main()
{
    dx = 0;
    if (end == 1)
    {
        symbols[0].clear();
        symbols[1].clear();
        end = 0;
        append_midcode(str_func_def, nullptr, "void", string_main);
        insymbol(symbols[0]);
    }
    else
    {
        insymbol(symbols[0]);
        if (symbols[0].sy == mainsy)
        {
            append_midcode(str_func_def, nullptr, "void", string_main);
            insymbol(symbols[0]);
        }
        else
            error("ȱ�� main");
    }

    //�ǼǺ������ƺͷ���ֵ
    if (t == TABLE_SIZE - 1)
    {
        //fatal,���ű������������˳�
        fatal("���ű���������");
    }
    if (f == FUNC_TABLE_SIZE - 1)
    {
        //fatal,�����������������˳�
        fatal("��������������");
    }
    t += 1;
    f += 1;

    strcpy_s(tab[t].name, string_main);
    tab[t].obj = func;
    tab[t].typ = notyp;
    strcpy_s(tab[t].parent, string_global);
    tab[t].ref = f;
    ftab[tab[t].ref].paraNum = 0;

    if (symbols[0].sy == lparent)
        insymbol(symbols[0]);
    else
        error("ȱ�� (");

    if (symbols[0].sy == rparent)
        insymbol(symbols[0]);
    else
        error("ȱ�� )");

    if (symbols[0].sy == lbrack)
        insymbol(symbols[0]);
    else
        error("ȱ�� {");

    bool has_return=false;
    compound_statement(string_main, has_return);
    append_midcode(str_label, "fkd_program_end");

    if (symbols[0].sy == rbrack)
    {
        //���򵽴�Ϊֹ�ˣ����������
    }
    else
        error("ȱ�� }");

    programma_info << "����һ������������" << endl;
}
//����Ϊmips��������ɺ�����ÿ��ָ��һ�����ɺ���
void NaiveCompiler::add(char* rd, char* rs, char* rt)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy_s(mips[mips_counter].op, str_add);
        strcpy_s(mips[mips_counter].rs, rs);
        strcpy_s(mips[mips_counter].rt, rt);
        strcpy_s(mips[mips_counter].rd, rd);
        mipsfile << mips[mips_counter].op << " " << rd << " " << rs << " " << rt << endl;
    }
    else
        error("mips����������������");
}
void NaiveCompiler::addi(char* rt, char* rs, int imm)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy_s(mips[mips_counter].op, str_addi);
        strcpy_s(mips[mips_counter].rs, rs);
        _itoa_s(imm, mips[mips_counter].imm, 10);
        strcpy_s(mips[mips_counter].rt, rt);
        mipsfile << mips[mips_counter].op << " " << rt << " " << rs << " " << imm << endl;
    }
    else
        error("mips����������������");
}
void NaiveCompiler::_and(char* rd, char* rs, char* rt)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy_s(mips[mips_counter].op, str_and);
        strcpy_s(mips[mips_counter].rs, rs);
        strcpy_s(mips[mips_counter].rt, rt);
        strcpy_s(mips[mips_counter].rd, rd);
        mipsfile << mips[mips_counter].op << " " << rd << " " << rs << " " << rt << endl;
    }
    else
        error("mips����������������");
}
void NaiveCompiler::andi(char* rt, char* rs, int imm)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy_s(mips[mips_counter].op, str_andi);
        strcpy_s(mips[mips_counter].rs, rs);
        _itoa_s(imm, mips[mips_counter].imm, 10);
        strcpy_s(mips[mips_counter].rt, rt);
        mipsfile << mips[mips_counter].op << " " << rt << " " << rs << " " << imm << endl;
    }
    else
        error("mips����������������");
}
void NaiveCompiler::_beq(char* rs, char* rt, char* label)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy_s(mips[mips_counter].op, str_beq);
        strcpy_s(mips[mips_counter].rs, rs);
        strcpy_s(mips[mips_counter].rt, rt);
        strcpy_s(mips[mips_counter].target, label);
        mipsfile << mips[mips_counter].op << " " << rs << " " << rt << " " << label << endl;
    }
    else
        error("mips����������������");
}
/*
void NaiveCompiler::_bge(char* rs, char* rt, char* label)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy_s(mips[mips_counter].op, "bge");
        strcpy_s(mips[mips_counter].rs, rs);
        strcpy_s(mips[mips_counter].rt, rt);
        strcpy_s(mips[mips_counter].target, label);
        mipsfile << mips[mips_counter].op << " " << rs << " " << rt << " " << label << endl;
    }
    else
        error("mips����������������");
}
void NaiveCompiler::_bgt(char* rs, char* rt, char* label)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy_s(mips[mips_counter].op, "bgt");
        strcpy_s(mips[mips_counter].rs, rs);
        strcpy_s(mips[mips_counter].rt, rt);
        strcpy_s(mips[mips_counter].target, label);
        mipsfile << mips[mips_counter].op << " " << rs << " " << rt << " " << label << endl;
    }
    else
        error("mips����������������");
}
void NaiveCompiler::_ble(char* rs, char* rt, char* label)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy_s(mips[mips_counter].op, "ble");
        strcpy_s(mips[mips_counter].rs, rs);
        strcpy_s(mips[mips_counter].rt, rt);
        strcpy_s(mips[mips_counter].target, label);
        mipsfile << mips[mips_counter].op << " " << rs << " " << rt << " " << label << endl;
    }
    else
        error("mips����������������");
}
void NaiveCompiler::_blt(char* rs, char* rt, char* label)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy_s(mips[mips_counter].op, "blt");
        strcpy_s(mips[mips_counter].rs, rs);
        strcpy_s(mips[mips_counter].rt, rt);
        strcpy_s(mips[mips_counter].target, label);
        mipsfile << mips[mips_counter].op << " " << rs << " " << rt << " " << label << endl;
    }
    else
        error("mips����������������");
}*/
void NaiveCompiler::_bne(char* rs, char* rt, char* label)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy_s(mips[mips_counter].op, str_bne);
        strcpy_s(mips[mips_counter].rs, rs);
        strcpy_s(mips[mips_counter].rt, rt);
        strcpy_s(mips[mips_counter].target, label);
        mipsfile << mips[mips_counter].op << " " << rs << " " << rt << " " << label << endl;
    }
    else
        error("mips����������������");
}
void NaiveCompiler::div(char* rs, char* rt)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy_s(mips[mips_counter].op, str_div);
        strcpy_s(mips[mips_counter].rs, rs);
        strcpy_s(mips[mips_counter].rt, rt);
        mipsfile << mips[mips_counter].op << " " << rs << " " << rt << endl;
    }
    else
        error("mips����������������");
}
void NaiveCompiler::generate_label(char* label)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy_s(mips[mips_counter].op, str_label);
        strcpy_s(mips[mips_counter].rs, label);
        mipsfile << mips[mips_counter].rs << ":" << endl;
    }
    else
        error("mips����������������");
}
void NaiveCompiler::_j(char* target)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy_s(mips[mips_counter].op, str_j);
        strcpy_s(mips[mips_counter].target, target);
        mipsfile << mips[mips_counter].op << " " << target << endl;
    }
    else
        error("mips����������������");
}
void NaiveCompiler::jal(char* target)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy_s(mips[mips_counter].op, str_jal);
        strcpy_s(mips[mips_counter].target, target);
        mipsfile << mips[mips_counter].op << " " << target << endl;
    }
    else
        error("mips����������������");
}
void NaiveCompiler::jr(char* target_register)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy_s(mips[mips_counter].op, str_jr);
        strcpy_s(mips[mips_counter].rs, target_register);
        mipsfile << mips[mips_counter].op << " " << mips[mips_counter].rs << endl;
    }
    else
        error("mips����������������");
}
void NaiveCompiler::la(char* rt, char* label)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy_s(mips[mips_counter].op, str_la);
        strcpy_s(mips[mips_counter].rt, rt);
        strcpy_s(mips[mips_counter].target, label);//label��������ַ����ĵ�ַ
        mipsfile << mips[mips_counter].op << " " << rt << " " << label << endl;
    }
    else
        error("mips����������������");
}
void NaiveCompiler::lui(char* rt, int imm)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy_s(mips[mips_counter].op, str_lui);
        _itoa_s(imm, mips[mips_counter].imm, 10);
        strcpy_s(mips[mips_counter].rt, rt);
        mipsfile << mips[mips_counter].op << " " << rt << " " << imm << endl;
    }
    else
        error("mips����������������");
}
void NaiveCompiler::lw(char* rt, char* rs, int imm)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy_s(mips[mips_counter].op, str_lw);
        strcpy_s(mips[mips_counter].rs, rs);
        _itoa_s(imm, mips[mips_counter].imm, 10);
        strcpy_s(mips[mips_counter].rt, rt);
        mipsfile << mips[mips_counter].op << " " << rt << " " << imm << "(" << rs << ")" << endl;
    }
    else
        error("mips����������������");
}
void NaiveCompiler::mflo(char* rd)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy_s(mips[mips_counter].op, str_mflo);
        strcpy_s(mips[mips_counter].rd, rd);
        mipsfile << mips[mips_counter].op << " " << rd << endl;
    }
    else
        error("mips����������������");
}
void NaiveCompiler::mult(char* rs, char* rt)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy_s(mips[mips_counter].op, str_mult);
        strcpy_s(mips[mips_counter].rs, rs);
        strcpy_s(mips[mips_counter].rt, rt);
        mipsfile << mips[mips_counter].op << " " << rs << " " << rt << endl;
    }
    else
        error("mips����������������");
}
void NaiveCompiler::_nor(char* rd, char* rs, char* rt)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy_s(mips[mips_counter].op, str_nor);
        strcpy_s(mips[mips_counter].rs, rs);
        strcpy_s(mips[mips_counter].rt, rt);
        strcpy_s(mips[mips_counter].rd, rd);
        mipsfile << mips[mips_counter].op << " " << rd << " " << rs << " " << rt << endl;
    }
    else
        error("mips����������������");
}
void NaiveCompiler::_or(char* rd, char* rs, char* rt)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy_s(mips[mips_counter].op, str_or);
        strcpy_s(mips[mips_counter].rs, rs);
        strcpy_s(mips[mips_counter].rt, rt);
        strcpy_s(mips[mips_counter].rd, rd);
        mipsfile << mips[mips_counter].op << " " << rd << " " << rs << " " << rt << endl;
    }
    else
        error("mips����������������");
}
void NaiveCompiler::ori(char* rt, char* rs, int imm)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy_s(mips[mips_counter].op, str_ori);
        strcpy_s(mips[mips_counter].rs, rs);
        _itoa_s(imm, mips[mips_counter].imm, 10);
        strcpy_s(mips[mips_counter].rt, rt);
        mipsfile << mips[mips_counter].op << " " << rt << " " << rs << " " << imm << endl;
    }
    else
        error("mips����������������");
}
void NaiveCompiler::sll(char* rd, char* rt, int imm)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy_s(mips[mips_counter].op, str_sll);
        strcpy_s(mips[mips_counter].rd, rd);
        _itoa_s(imm, mips[mips_counter].imm, 10);
        strcpy_s(mips[mips_counter].rt, rt);
        mipsfile << mips[mips_counter].op << " " << rd << " " << rt << " " << imm << endl;
    }
    else
        error("mips����������������");
}
void NaiveCompiler::slt(char* rd, char* rs, char* rt)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy_s(mips[mips_counter].op, str_slt);
        strcpy_s(mips[mips_counter].rs, rs);
        strcpy_s(mips[mips_counter].rt, rt);
        strcpy_s(mips[mips_counter].rd, rd);
        mipsfile << mips[mips_counter].op << " " << rd << " " << rs << " " << rt << endl;
    }
    else
        error("mips����������������");
}
void NaiveCompiler::sltu(char* rd, char* rs, char* rt)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy_s(mips[mips_counter].op, str_sltu);
        strcpy_s(mips[mips_counter].rs, rs);
        strcpy_s(mips[mips_counter].rt, rt);
        strcpy_s(mips[mips_counter].rd, rd);
        mipsfile << mips[mips_counter].op << " " << rd << " " << rs << " " << rt << endl;
    }
    else
        error("mips����������������");
}
void NaiveCompiler::slti(char* rt, char* rs, int imm)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy_s(mips[mips_counter].op, str_slti);
        strcpy_s(mips[mips_counter].rs, rs);
        _itoa_s(imm, mips[mips_counter].imm, 10);
        strcpy_s(mips[mips_counter].rt, rt);
        mipsfile << mips[mips_counter].op << " " << rt << " " << rs << " " << imm << endl;
    }
    else
        error("mips����������������");
}
void NaiveCompiler::sltiu(char* rt, char* rs, int imm)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy_s(mips[mips_counter].op, str_sltiu);
        strcpy_s(mips[mips_counter].rs, rs);
        _itoa_s(imm, mips[mips_counter].imm, 10);
        strcpy_s(mips[mips_counter].rt, rt);
        mipsfile << mips[mips_counter].op << " " << rt << " " << rs << " " << imm << endl;
    }
    else
        error("mips����������������");
}
void NaiveCompiler::sub(char* rd, char* rs, char* rt)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy_s(mips[mips_counter].op, str_sub);
        strcpy_s(mips[mips_counter].rs, rs);
        strcpy_s(mips[mips_counter].rt, rt);
        strcpy_s(mips[mips_counter].rd, rd);
        mipsfile << mips[mips_counter].op << " " << rd << " " << rs << " " << rt << endl;
    }
    else
        error("mips����������������");
}
void NaiveCompiler::subu(char* rd, char* rs, char* rt)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy_s(mips[mips_counter].op, str_subu);
        strcpy_s(mips[mips_counter].rs, rs);
        strcpy_s(mips[mips_counter].rt, rt);
        strcpy_s(mips[mips_counter].rd, rd);
        mipsfile << mips[mips_counter].op << " " << rd << " " << rs << " " << rt << endl;
    }
    else
        error("mips����������������");
}
void NaiveCompiler::sw(char* rt, char* rs, int imm)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy_s(mips[mips_counter].op, str_sw);
        strcpy_s(mips[mips_counter].rs, rs);
        _itoa_s(imm, mips[mips_counter].imm, 10);
        strcpy_s(mips[mips_counter].rt, rt);
        mipsfile << mips[mips_counter].op << " " << rt << " " << imm << "(" << rs << ")" << endl;
    }
    else
        error("mips����������������");
}
void NaiveCompiler::syscall()
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy_s(mips[mips_counter].op, str_syscall);
        mipsfile << mips[mips_counter].op << endl;
    }
    else
        error("mips����������������");
}
void NaiveCompiler::nop()
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy_s(mips[mips_counter].op, str_nop);
        mipsfile << mips[mips_counter].op << endl;
    }
    else
        error("mips����������������");
}

void NaiveCompiler::markBasicBlock()
{
    for (int i = 0; i<mid_counter; i++)
    {
        if (strcmp(midcode[i].op, str_label) == 0
            || strcmp(midcode[i].op, str_func_def) == 0)
        {
            basicBlockStartLines.push_back(i);
        }
        else if (strcmp(midcode[i].op, str_GOTO) == 0
            || strcmp(midcode[i].op, str_ret) == 0
            || strcmp(midcode[i].op, str_BNZ) == 0
            || strcmp(midcode[i].op, str_BZ) == 0)
        {
            if (!(strcmp(midcode[i+1].op, str_label) == 0
               || strcmp(midcode[i+1].op, str_func_def) == 0))
            {
                basicBlockStartLines.push_back(i+1);
            }
        } 
    }
    list<int>::iterator it;
    fstream fbb;
    fbb.open("basic_block_begin_points.txt", ios::out);
    it = basicBlockStartLines.begin();
    for (; it != basicBlockStartLines.end(); it++)
        fbb << (*it) << endl;
}
bool NaiveCompiler::isTempVar(char (&name)[ALNG])
{
    char head[ALNG];
    memcpy(head, name, 11);
    head[11]='\0';
    if (strcmp(head, "fangkedongt") == 0)
        return true;
    
    return false;
}
bool NaiveCompiler::isCal(int index)
{
    if (strcmp(midcode[index].op, "+") == 0
        || strcmp(midcode[index].op, "-") == 0
        || strcmp(midcode[index].op, "*") == 0
        || strcmp(midcode[index].op, "/") == 0)
        return true;
    
    return false;
}
void NaiveCompiler::del_common_expression()
{
    markBasicBlock();
    list<int>::iterator it;
    it = basicBlockStartLines.begin();
    int block_start;
    int block_end = (*it) - 1;
    it++;
    for (; it != basicBlockStartLines.end(); it++)
    {
        block_start = block_end + 1;//�������һ�����
        block_end = (*it) - 1;//���������һ�����

        //�˴����볣���ϲ��봫��
        constantMP(block_start, block_end);

        for (int i = block_start; i<=block_end; i++)//��һ��������Ϊ��λ
        {
            if (!((isCal(i) || strcmp(midcode[i].op, "=") == 0) && isTempVar(midcode[i].rd)))
                continue;

            for (int j = i+1; j<=block_end; j++)//Ѱ��op, rs, rt��һ���ı��ʽ
            {
                if ((isCal(j) || strcmp(midcode[i].op, "=") == 0) && (strcmp(midcode[i].rs, midcode[j].rd) == 0 || strcmp(midcode[i].rt, midcode[j].rd) == 0))
                    break;

                if (strcmp(midcode[i].op, midcode[j].op) == 0
                 && strcmp(midcode[i].rs, midcode[j].rs) == 0
                 && strcmp(midcode[i].rt, midcode[j].rt) == 0
                 && isTempVar(midcode[j].rd))//����ҵ���
                {
                    
                    for (int k = j+1; k<=block_end; k++)//������������õ�midcode[j].rd�ĵط�������midcode[i].rd
                    {
                        if (strcmp(midcode[k].op, "[]=") == 0)
                        {
                            if (strcmp(midcode[j].rd, midcode[k].rd) == 0)
                                strcpy_s(midcode[k].rd, midcode[i].rd);
                            if (strcmp(midcode[j].rd, midcode[k].rt) == 0)
                                strcpy_s(midcode[k].rt, midcode[i].rd);
                        }
                        else
                        {
                            if (strcmp(midcode[j].rd, midcode[k].rs) == 0)
                                strcpy_s(midcode[k].rs, midcode[i].rd);
                            if (strcmp(midcode[j].rd, midcode[k].rt) == 0)
                                strcpy_s(midcode[k].rt, midcode[i].rd);
                        }
                    }
                    strcpy_s(midcode[j].op, str_nop);  
                }
            }
        }

    }

}
void NaiveCompiler::print_optimized_midcode(string filepath)
{
    fstream midcodefile;
    midcodefile.open(filepath, ios::out);
    for (int i=0; i<mid_counter; i++)
    {
        if (strcmp(midcode[i].op, str_func_def) == 0)
            midcodefile << midcode[i].rs << " " << midcode[i].rt << "()" << endl;
        else if (strcmp(midcode[i].op, str_para) == 0)
            midcodefile << midcode[i].op << " " << midcode[i].rs << " " << midcode[i].rt << endl;
        else if (strcmp(midcode[i].op, str_push) == 0)
            midcodefile << midcode[i].op << " " << midcode[i].rs << endl;
        else if (strcmp(midcode[i].op, str_call) == 0)
            midcodefile << midcode[i].op << " " << midcode[i].rs << endl;
        else if (strcmp(midcode[i].op, "=") == 0)
            midcodefile << midcode[i].rd << " " << midcode[i].op << " " << midcode[i].rs << endl;
        else if (strcmp(midcode[i].op, "+") == 0 || strcmp(midcode[i].op, "-") == 0
            ||  strcmp(midcode[i].op, "*") == 0 || strcmp(midcode[i].op, "/") == 0)
            midcodefile << midcode[i].rd << "=" << midcode[i].rs << " " << midcode[i].op << " " << midcode[i].rt << endl;
        else if (strcmp(midcode[i].op, "==") == 0 || strcmp(midcode[i].op, "!=") == 0
            ||  strcmp(midcode[i].op, ">") == 0 || strcmp(midcode[i].op, ">=") == 0
            ||  strcmp(midcode[i].op, "<") == 0 || strcmp(midcode[i].op, "<=") == 0)
            midcodefile << midcode[i].rs << " " << midcode[i].op << " " << midcode[i].rt << endl;
        else if (strcmp(midcode[i].op, str_BNZ) == 0)
            midcodefile << "BNZ " << midcode[i].rd << endl;
        else if (strcmp(midcode[i].op, str_BZ) == 0)
            midcodefile << "BZ " << midcode[i].rd << endl;
        else if (strcmp(midcode[i].op, str_ret) == 0)
        {
            if (midcode[i].rs == nullptr)
                midcodefile << midcode[i].op << endl;
            else
                midcodefile << midcode[i].op << " " << midcode[i].rs << endl;
        }
        else if (strcmp(midcode[i].op, str_GOTO) == 0)
            midcodefile << "GOTO " << midcode[i].rs << endl;
        else if (strcmp(midcode[i].op, "=[]") == 0)
            midcodefile << midcode[i].rd << "=" << midcode[i].rs << "[" << midcode[i].rt << "]" << endl;
        else if (strcmp(midcode[i].op, "[]=") == 0)
            midcodefile << midcode[i].rs << "[" << midcode[i].rt << "]=" << midcode[i].rd << endl;
        else if (strcmp(midcode[i].op, str_prt) == 0)
            midcodefile << midcode[i].op << " " << midcode[i].rs << " " << midcode[i].rt << endl;
        else if (strcmp(midcode[i].op, str_scf) == 0)
            midcodefile << midcode[i].op << " " << midcode[i].rd << " " << midcode[i].rs << endl;
        else if (strcmp(midcode[i].op, str_label) == 0)
            midcodefile << midcode[i].rd << ":" << endl;
        else if (strcmp(midcode[i].op, str_nop) == 0)
            midcodefile << str_nop << endl;
        else
            error("����ʶ���op");
    }
}

void NaiveCompiler::del_common_subexpression()
{
    int _start = 0;
    int _end;
    int i = 0;
    for (;i < mid_counter; i++)
    {
        //�ҵ�һ�����ʽ���׺�β
        if (isCal(i) && isTempVar(midcode[i].rd)){
            _start = i;
            while((isCal(i) && isTempVar(midcode[i].rd)) || strcmp(midcode[i].op, "=") == 0)
            {
                if (strcmp(midcode[i].op, "=") == 0)
                {
                    i++;
                    break;
                }
                else
                    i++;
            }
                
            i--;
            _end = i;

            midcode2DAG(_start, _end);
            DAG2midcode(_start, _end);

            for (int j = 0; j <= d; j++)
            {
                memset(dag[j].op, 0, sizeof(dag[j].op));
                memset(dag[j].name, 0, sizeof(dag[j].name));
                dag[j].isLeaf = false;
                dag[j].left_child_num = -1;
                dag[j].right_child_num = -1;
            }
            d = -1;
            dag_table.clear();
            midqueue.clear();
        }
    }
}
int NaiveCompiler::getNodeNum(string name)
{
    list<pair<string, int>>::iterator it;
    it = dag_table.begin();
    for (; it != dag_table.end(); it++)
    {
        if (name.compare((*it).first) == 0)
            return (*it).second;
    }
    return -1;
}
bool NaiveCompiler::checkParents(int i)
{
    list<int>::iterator it;
    set<int> midset;

    it = midqueue.begin();
    for (; it != midqueue.end(); it++)
        midset.insert(*it);

    it = dag[i].parents.begin();
    for (; it != dag[i].parents.end(); it++)
    {
        if (midset.count(*it) == 0)
            return false;
    }
    return true;
}
void NaiveCompiler::insert_left_child(int i, set<int> &midset)
{
    if (midset.count(dag[i].left_child_num) > 0)
        return ;

    if (checkParents(dag[i].left_child_num))
    {
        int _left = dag[i].left_child_num;
        if (dag[_left].left_child_num == -1 && dag[_left].right_child_num == -1)//�����Ҷ�ӽ��
            return ;
        midqueue.push_front(dag[i].left_child_num);
        midset.insert(dag[i].left_child_num);
        insert_left_child(dag[i].left_child_num, midset);
    }

}
bool NaiveCompiler::checkMidPoint(set<int> &midset)
{
    for (int i=0; i <= d; i++)
    {
        if (midset.count(i) > 0 || (dag[i].left_child_num == -1 && dag[i].right_child_num == -1))
            continue;
        else
            return true;
    }
    return false;
}
void NaiveCompiler::midcode2DAG(int s, int e)
{
    int left, right, par, old_par;
    dag_table.clear();
    for (int i = s; i <= e; i++)
    {
        if (isCal(i))//�����������ָ��
        {
            if (midcode[i].rs[0] != 0)
            {
                left = getNodeNum(midcode[i].rs);
                if (left == -1)
                {
                    if (d == MAX_DAG_POINT - 1)
                        fatal("DAGͼ�ڵ����ﵽ����");
                    d += 1;
                    strcpy_s(dag[d].name, midcode[i].rs);
                    dag[d].left_child_num = -1;
                    dag[d].right_child_num = -1;
                    dag[d].isLeaf = true;
                    dag[d].parents.clear();
                    left = d;
                    dag_table.push_back(make_pair(dag[d].name, d));
                }
            }

            if (midcode[i].rt[0] != 0)
            {
                right = getNodeNum(midcode[i].rt);
                if (right == -1)
                {
                    if (d == MAX_DAG_POINT - 1)
                        fatal("DAGͼ�ڵ����ﵽ����");
                    d += 1;
                    strcpy_s(dag[d].name, midcode[i].rt);
                    dag[d].left_child_num = -1;
                    dag[d].right_child_num = -1;
                    dag[d].isLeaf = true;
                    dag[d].parents.clear();
                    right = d;
                    dag_table.push_back(make_pair(dag[d].name, d));
                }
            }

            par = -1;
            for (int j = 0; j <= d; j++)
            {
                if (strcmp(dag[j].op, midcode[i].op) == 0
                    && dag[j].left_child_num == left
                    && dag[j].right_child_num == right)
                {
                    par = j;
                    break;
                }
            }

            if (par == -1)
            {
                if (d == MAX_DAG_POINT - 1)
                    fatal("DAGͼ�ڵ����ﵽ����");
                d += 1;
                strcpy_s(dag[d].op, midcode[i].op);
                strcpy_s(dag[d].name, midcode[i].rd);
                dag[d].left_child_num = left;
                dag[d].right_child_num = right;
                dag[d].isLeaf = false;
                dag[d].parents.clear();
                par = d;
                dag[left].parents.push_back(par);
                dag[right].parents.push_back(par);
            }
            else//����鵽�ˣ��͸��³����µı�������ȷ��a=fangkedongt1֮����������a������fangkedongt1
                strcpy_s(dag[par].name, midcode[i].rd);

            old_par = getNodeNum(midcode[i].rd);
            if (old_par != -1)
                dag_table.remove(make_pair(midcode[i].rd, old_par));
            dag_table.push_back(make_pair(midcode[i].rd, par));
        }
        else
        {
            for (int j = 0; j <= d; j++)
            {
                if (strcmp(dag[j].name, midcode[i].rs) == 0)
                {
                    strcpy_s(dag[j].name, midcode[i].rd);
                    break;
                }
            }
            
        }
    }
}
void NaiveCompiler::DAG2midcode(int s, int e)
{
    set<int> midset;
    list<int>::iterator it;
    
    while (checkMidPoint(midset))
    {
        for (int i=0; i <= d; i++)
        {
            if (midset.count(i) > 0 || (dag[i].left_child_num == -1 && dag[i].right_child_num == -1))
                continue;

            if (checkParents(i))
            {
                midqueue.push_front(i);
                midset.insert(i);
                insert_left_child(i, midset);
            }
        }
    }

    it = midqueue.begin();
    for (; it != midqueue.end(); it++)
    {
        strcpy_s(midcode[s].op, dag[*it].op);
        if (dag[*it].left_child_num != -1)
            strcpy_s(midcode[s].rs, dag[dag[*it].left_child_num].name);
        if (dag[*it].right_child_num != -1)
            strcpy_s(midcode[s].rt, dag[dag[*it].right_child_num].name);
        strcpy_s(midcode[s].rd, dag[*it].name);
        s++;
    }

    for (int i = s; i <= e; i++)
    {
        strcpy_s(midcode[i].op, str_nop);
    }
}

void NaiveCompiler::ref_count(bool distributeFlag)
{
    int i=0;
    int func_start, func_end;
    char parent[ALNG];
    while(i<mid_counter)
    {
        if (strcmp(midcode[i].op, str_func_def) == 0)
        {
            //��ÿһ�������еľֲ������͸�����������ȫ�ּĴ�������
            strcpy_s(parent, midcode[i].rt);
            i++;
            if (i >= mid_counter)
                return ;
            func_start = i;
            while (strcmp(midcode[i].op, str_func_def) != 0 && i < mid_counter)
                i++;
            func_end = i - 1;

            int w_times = 1;
            int index;
            for (int j = func_start; j<=func_end; j++)
            {
                if (strcmp(midcode[j].op, str_label) == 0)
                {
                    char temp[ALNG];
                    memset(temp, 0, sizeof(temp));
                    memcpy(temp, midcode[j].rs, 11);
                    if (strcmp(temp, "fkd_dobegin") == 0)
                        w_times *= 10;
                }
                else if (strcmp(midcode[j].op, str_BNZ) == 0)
                {
                    if (w_times >= 10)
                        w_times /= 10;
                }

                index = getIndexByNameAndParent(parent, midcode[j].rs, declare_mode);
                if (index != -1 && tab[index].obj != con)
                    tab[index].value += w_times;


                index = getIndexByNameAndParent(parent, midcode[j].rt, declare_mode);
                if (index != -1 && tab[index].obj != con)
                    tab[index].value += w_times;
                

                index = getIndexByNameAndParent(parent, midcode[j].rd, declare_mode);
                if (index != -1 && tab[index].obj != con)
                    tab[index].value += w_times;
                
            }

            if (distributeFlag)
            {
                int func_index = getIndexByNameAndParent(string_global, parent, search_mode);
                if (distribute_reg(parent, reg_s0) != -1)
                    ftab[tab[func_index].ref].alloc_reg.set(0);
                if (distribute_reg(parent, reg_s1) != -1)
                    ftab[tab[func_index].ref].alloc_reg.set(1);
                if (distribute_reg(parent, reg_s2) != -1)
                    ftab[tab[func_index].ref].alloc_reg.set(2);
                if (distribute_reg(parent, reg_s3) != -1)
                    ftab[tab[func_index].ref].alloc_reg.set(3);
                if (distribute_reg(parent, reg_s4) != -1)
                    ftab[tab[func_index].ref].alloc_reg.set(4);
                if (distribute_reg(parent, reg_s5) != -1)
                    ftab[tab[func_index].ref].alloc_reg.set(5);
                if (distribute_reg(parent, reg_s6) != -1)
                    ftab[tab[func_index].ref].alloc_reg.set(6);
                if (distribute_reg(parent, reg_s7) != -1)
                    ftab[tab[func_index].ref].alloc_reg.set(7);
                if (distribute_reg(parent, reg_a1) != -1)
                    ftab[tab[func_index].ref].alloc_reg.set(8);
                if (distribute_reg(parent, reg_a2) != -1)
                    ftab[tab[func_index].ref].alloc_reg.set(9);
                if (distribute_reg(parent, reg_a3) != -1)
                    ftab[tab[func_index].ref].alloc_reg.set(10);
                if (distribute_reg(parent, reg_t5) != -1)
                    ftab[tab[func_index].ref].alloc_reg.set(11);
                if (distribute_reg(parent, reg_t6) != -1)
                    ftab[tab[func_index].ref].alloc_reg.set(12);
                if (distribute_reg(parent, reg_t7) != -1)
                    ftab[tab[func_index].ref].alloc_reg.set(13);
                if (distribute_reg(parent, reg_t8) != -1)
                    ftab[tab[func_index].ref].alloc_reg.set(14);
            }
        }
        else
            i++;
    }
}
int NaiveCompiler::distribute_reg(char (&parent)[ALNG], char* reg)
{
    int maximum = 0;
    int index = -1;
    for (int i = 0; i <= t; i++)
    {
        if (strcmp(tab[i].parent, parent) == 0 && (tab[i].obj == var || tab[i].obj == arr || tab[i].obj == para))
        {
            if (tab[i].value > maximum)
            {
                maximum = tab[i].value;
                index = i;
            }
        }
    }

    if (index != -1)
    {
        strcpy_s(tab[index].reg, reg);
        tab[index].value = 0;
    }
    return index;
}

void NaiveCompiler::activeVarAnalysis()
{
    ref_count(false);
    int func_start, func_end;
    char parent[ALNG];
    for (int i = 0; i < mid_counter; i++)
    {
        //���ҵ���������ֹ��
        if (strcmp(midcode[i].op, str_func_def) != 0)
            continue;
        strcpy_s(parent, midcode[i].rt);
        func_start = i;
        i++;
        while (i < mid_counter && strcmp(midcode[i].op, str_func_def) != 0)
            i++;
        i--;
        func_end = i;

        //��ѯ��������õ��ú����Ļ�����
        list<int> matchTable;
        list<basicBlockPoint> blocks;
        int count = -1;
        list<int>::iterator it;
        it = basicBlockStartLines.begin();
        for (; it != basicBlockStartLines.end() && (*it) <= func_end; it++)
        {
            if ((*it) >= func_start)
                matchTable.push_back(*it);
        }
        matchTable.push_back(func_end + 1);

        //��ÿ�����������
        list<int>::iterator it1;
        int block_start, block_end;
        it1 = matchTable.begin();
        block_end = (*it1) - 1;
        it1++;
        for (; it1 != matchTable.end(); it1++)
        {
            basicBlockPoint block;
            //�ҵ����������ֹ��
            block_start = block_end + 1;
            block_end = (*it1) - 1;
            block.start_line = block_start;
            block.end_line = block_end;
            
            //��def��use��������
            for (int j = block.start_line; j <= block.end_line; j++)
            {
                int func_index, index, numInFunc;
                func_index = getIndexByNameAndParent(string_global, parent, search_mode);
                index = getIndexByNameAndParent(parent, midcode[j].rs, declare_mode);
                if (index != -1)
                {
                    numInFunc = index - func_index;
                    if (!block.def[numInFunc])
                        block.use.set(numInFunc);
                }
                index = getIndexByNameAndParent(parent, midcode[j].rt, declare_mode);
                if (index != -1)
                {
                    numInFunc = index - func_index;
                    if (!block.def[numInFunc])
                        block.use.set(numInFunc);
                }

                index = getIndexByNameAndParent(parent, midcode[j].rd, declare_mode);
                if (index != -1)
                {
                    numInFunc = index - func_index;
                    if (strcmp(midcode[j].op, "[]="))// []= ������rd��useһ����
                    {
                        if (!block.def[numInFunc])
                            block.use.set(numInFunc);
                    }
                    else
                    {
                        if (!block.use[numInFunc])
                            block.def.set(numInFunc);
                    }
                }

            }

            //Ѱ�Һ�̻�����
            if (strcmp(midcode[block.end_line].op, str_BNZ) == 0
             || strcmp(midcode[block.end_line].op, str_BZ) == 0)
            {
                if (block.end_line + 1 >= mid_counter){}//do nothing
                else if (strcmp(midcode[block.end_line].op, str_BNZ) == 0){}//do nothing
                else
                {
                    //Ѱ��ֱ�Ӻ�̻����������
                    block._next.push_back(block.end_line + 1);
                }
                //Ѱ�ұ�ǩ��Ӧ�Ļ����������
                addNextByLabel(block, func_start, func_end);
            }
            else if (strcmp(midcode[block.end_line].op, str_GOTO) == 0)
            {
                addNextByLabel(block, func_start, func_end);
            }
            else if (strcmp(midcode[block.end_line].op, str_ret) == 0)
            {

            }
            else
            {
                if (block.end_line < func_end)
                    block._next.push_back(block.end_line + 1);
            }

            blocks.push_back(block);
        }//ͳ����Ϻ�����ÿ���������def��use����̼���
        //�������һ����
        /*basicBlockPoint block;
        block.start_line = block_end + 1;
        block.end_line = block.start_line;
        block.def.reset();
        block.use.reset();
        block._in.reset();
        block.out.reset();
        block._next.clear();
        blocks.push_back(block);*/

        //���濪ʼ���з���
        bool changed = true;//������ֵ������Ƿ���in/out�����ı䣬���������ε������޸ı䣬�����������
        bitset<MAX_VAR_IN_FUNC> pre;
        list<basicBlockPoint>::reverse_iterator blocks_it;
        while(changed)
        {
            changed = false;
            blocks_it = blocks.rbegin();
            for (; blocks_it != blocks.rend(); blocks_it++)
            {
                if ((*blocks_it)._next.size() > 0)
                {
                    list<int>::iterator it_next;
                    it_next = (*blocks_it)._next.begin();
                    pre = (*blocks_it).out;
                    (*blocks_it).out.reset();
                    for (; it_next != (*blocks_it)._next.end(); it_next++)
                    {
                        bitset<MAX_VAR_IN_FUNC> in;
                        int start_line = (*it_next);
                        in = getInByStartLine(start_line, blocks);
                        (*blocks_it).out = (*blocks_it).out | in;
                    }
                    if (pre != (*blocks_it).out)
                        changed = true;
                }
                
                pre = (*blocks_it)._in;
                (*blocks_it)._in = (*blocks_it).use | ((*blocks_it).out & (~(*blocks_it).def));
                if (pre != (*blocks_it)._in)
                    changed = true;
            }
        }
        //��Ծ�����������,���濪ʼ������ͻͼ
        buildConflictGraph(blocks, parent);
    }
}
void NaiveCompiler::addNextByLabel(basicBlockPoint &block, int func_start, int func_end)
{
    list<int>::iterator it2;
    //Ѱ�ұ�ǩ��Ӧ�Ļ����������
    int j;
    for (j = func_start; j <= func_end; j++)
    {
        if (strcmp(midcode[block.end_line].op, str_GOTO) == 0)
        {
            if (strcmp(midcode[j].op, str_label) == 0
                && strcmp(midcode[j].rd, midcode[block.end_line].rs) == 0)
            {
                block._next.push_back(j);
                break;
            }
        }
        else
        {
            if (strcmp(midcode[j].op, str_label) == 0
                && strcmp(midcode[j].rd, midcode[block.end_line].rd) == 0)
            {
                block._next.push_back(j);
                break;
            }
        }
    }
    if (j > func_end)
    {
        cout << "������Χ��" << func_start << "-" << func_end << " ��" << block.end_line << "�У���Ծ�������������е�ǰ������(BNZ/BZ)�Ҳ�����һ��������" << endl;
    }
        
}
bitset<MAX_VAR_IN_FUNC> NaiveCompiler::getInByStartLine(int start_line, list<basicBlockPoint> &blocks)
{
    list<basicBlockPoint>::iterator it2;
    it2 = blocks.begin();
    for (; it2 != blocks.end(); it2++)
    {
        if ((*it2).start_line == start_line)
            return (*it2)._in;
    }
    cout << "start line = " << start_line << " getInByStartLine�������ִ���" << endl;
}
void NaiveCompiler::buildConflictGraph(list<basicBlockPoint> &blocks, char (&func_name)[ALNG])
{
    //��ʼ������ͻͼ
    int func_index = getIndexByNameAndParent(string_global, func_name, search_mode);
    int count = 1;
    while(strcmp(tab[func_index + count].parent, func_name) == 0)
        count++;

    conflictPoint* points;
    //points = (conflictPoint*)malloc(count*sizeof(conflictPoint));
    points = new conflictPoint[count];
    //memset(points, 0, count*sizeof(conflictPoint));
    list<basicBlockPoint>::iterator block_it;
    bitset<MAX_VAR_IN_FUNC> active_var;//��ǻ�Ծ�ı�����ֻ����������Ծ�ı�������ȫ�ּĴ���
    block_it = blocks.begin();
    for (; block_it != blocks.end(); block_it++)
    {
        //����ÿ���������in,out���ϣ����ɳ�ͻͼ
        active_var = active_var | (*block_it)._in | (*block_it).out;
        int i, j;
        for (i = 1; i < count; i++)
        {
            for (j = i + 1; j < count; j++)
            {
                if (((*block_it)._in[i]  && (*block_it)._in[j])
                 || ((*block_it).out[i]  && (*block_it).out[j]))
                {
                    points[i].neighbours.insert(j);
                    points[j].neighbours.insert(i);
                }
            }
        }
    }

    int counter = 0;
    
    //ÿ��point������ű��е�һ��������Ϊÿ��point����һ��weightֵ��-1��ʾδ���䣬-2��ʾ������Ĵ�����������Ȼ���Ӵ�С��ʾ����Ĵ�����˳�򣬴�weight���Ŀ�ʼ��
restart_delete:
    bool flag = true;
    while(flag)
    {
        flag = false;
        for (int i = 1; i < count; i++)
        {
            //����ط���Ҫͳ�Ƶ������ڵ���δ��ɾ���ĵ㣬Ҳ����weight == -1�ĵ�
            if ((   tab[func_index + i].obj == var 
                 || tab[func_index + i].obj == arr 
                 || tab[func_index + i].obj == para) 
             && active_var[i]
             && countNeighbours(points, i) < GLOABL_REG_NUMBER 
             && points[i].weight == -1)
            {
                points[i].weight = count;
                counter++;
                flag = true;
                break;
            }
        }
    }

    for (int i = 1; i < count; i++)
    {
        if ((   tab[func_index + i].obj == var 
            || tab[func_index + i].obj == arr 
            || tab[func_index + i].obj == para) 
         && active_var[i]
         && countNeighbours(points, i) >= GLOABL_REG_NUMBER 
         && points[i].weight == -1)
        {
            points[i].weight = -2;
            goto restart_delete;
        }
    }
    //�����������Ȩ�أ����濪ʼ����Ĵ���
    distributeRegByConflictGraph(points, count, func_name);
}
int NaiveCompiler::countNeighbours(conflictPoint *points, int i)
{
    int result = 0;
    set<int>::iterator it;
    for (it = points[i].neighbours.begin(); it != points[i].neighbours.end(); it++)
    {
        if (points[*it].weight == -1)
            result++;
    }
    return result;
}
void NaiveCompiler::distributeRegByConflictGraph(conflictPoint *points, int length, char (&func_name)[ALNG])
{
    int max_reg_number = -1;
    //����ÿ��point��weightֵ����Ĵ�����С��0�򲻷���
    int func_index = getIndexByNameAndParent(string_global, func_name, search_mode);
    int index;
    while ((index = getIndexOfMaxWeight(points, length)) > 0)
    {
        points[index].weight = -2;
        points[index].reg_number = getUnconfilctRegNumber(points, length, index);
        switch(points[index].reg_number)
        {
        case 0: strcpy_s(tab[func_index + index].reg, reg_s0);  
                ftab[tab[func_index].ref].alloc_reg.set(0);  
                break;
        case 1: strcpy_s(tab[func_index + index].reg, reg_s1);  
                ftab[tab[func_index].ref].alloc_reg.set(1);
                break;
        case 2: strcpy_s(tab[func_index + index].reg, reg_s2);  
                ftab[tab[func_index].ref].alloc_reg.set(2);
                break;
        case 3: strcpy_s(tab[func_index + index].reg, reg_s3);  
                ftab[tab[func_index].ref].alloc_reg.set(3);
                break;
        case 4: strcpy_s(tab[func_index + index].reg, reg_s4);  
                ftab[tab[func_index].ref].alloc_reg.set(4);
                break;
        case 5: strcpy_s(tab[func_index + index].reg, reg_s5);  
                ftab[tab[func_index].ref].alloc_reg.set(5);
                break;
        case 6: strcpy_s(tab[func_index + index].reg, reg_s6);  
                ftab[tab[func_index].ref].alloc_reg.set(6);
                break;
        case 7: strcpy_s(tab[func_index + index].reg, reg_s7);  
                ftab[tab[func_index].ref].alloc_reg.set(7);
                break;
        case 8: strcpy_s(tab[func_index + index].reg, reg_a1);  
                ftab[tab[func_index].ref].alloc_reg.set(8);
                break;
        case 9: strcpy_s(tab[func_index + index].reg, reg_a2);  
                ftab[tab[func_index].ref].alloc_reg.set(9);
                break;
        case 10: strcpy_s(tab[func_index + index].reg, reg_a3);  
                ftab[tab[func_index].ref].alloc_reg.set(10);
                break;
        case 11: strcpy_s(tab[func_index + index].reg, reg_t5);  
                ftab[tab[func_index].ref].alloc_reg.set(11);
                break;
        case 12: strcpy_s(tab[func_index + index].reg, reg_t6);  
                ftab[tab[func_index].ref].alloc_reg.set(12);
                break;
        case 13: strcpy_s(tab[func_index + index].reg, reg_t7);  
                ftab[tab[func_index].ref].alloc_reg.set(13);
                break;
        case 14: strcpy_s(tab[func_index + index].reg, reg_t8);  
                ftab[tab[func_index].ref].alloc_reg.set(14);
                break;
        }
    }
}
int NaiveCompiler::getIndexOfMaxWeight(conflictPoint *points, int length)
{
    int max_weight = -1;
    int result = 0;
    for (int i = 1; i < length; i++)
    {
        if (points[i].weight > max_weight)
        {
            max_weight = points[i].weight;
            result = i;
        }
    }
    return result;
}
int NaiveCompiler::getUnconfilctRegNumber(conflictPoint *points, int length, int index)
{
    //�������ڽ��ڵ㲻��ͻ�ļĴ������
    for (int i = 0; i < 15; i++)
    {
        bool isFree = true;
        for (int j = 1; j < length; j++)
        {
            if (points[index].neighbours.count(j) > 0 && points[j].reg_number == i)
            {
                isFree = false;
                break;
            }
        }

        if (isFree)
            return i;
    }

    cout << "����ͻͼ����Ĵ���������ͻ��" << endl;
    return -1;
}

void NaiveCompiler::constantMP(int block_start, int block_end)
{
    int i = block_start;
    while (i < mid_counter && i <= block_end)
    {
        if (isCal(i) 
        && (isdigit(midcode[i].rs[0]) || (midcode[i].rs[0] == '-' && isdigit(midcode[i].rs[1]))) 
        && (isdigit(midcode[i].rt[0]) || (midcode[i].rt[0] == '-' && isdigit(midcode[i].rt[1]))))
        {
            //�����ϲ�
            int left = atoi(midcode[i].rs);
            int right = atoi(midcode[i].rt);
            int result;
            if (strcmp(midcode[i].op, "+") == 0)
                result = left + right;
            else if (strcmp(midcode[i].op, "-") == 0)
                result = left - right;
            else if (strcmp(midcode[i].op, "*") == 0)
                result = left * right;
            else 
                result = left / right;

            memset(midcode[i].op, 0, sizeof(midcode[i].op));
            midcode[i].op[0] = '=';
            _itoa_s(result, midcode[i].rs, 10);

propagation:
            //��������
            for (int j = i + 1; j <= block_end; j++)
            {
                if (strcmp(midcode[j].rs, midcode[i].rd) == 0)
                    strcpy_s(midcode[j].rs, midcode[i].rs);
                if (strcmp(midcode[j].rt, midcode[i].rd) == 0)
                    strcpy_s(midcode[j].rt, midcode[i].rs);
                if (strcmp(midcode[j].op, "[]=") == 0)
                {
                    if (strcmp(midcode[j].rd, midcode[i].rd) == 0)
                        strcpy_s(midcode[j].rd, midcode[i].rs);
                }else if (strcmp(midcode[j].rd, midcode[i].rd) == 0)
                    break;

            }

        }
        else if (strcmp(midcode[i].op, "=") == 0 
            && (isdigit(midcode[i].rs[0]) || (midcode[i].rs[0] == '-' && isdigit(midcode[i].rs[1]))))
        {
            goto propagation;
        }

        i++;
    }
}
void NaiveCompiler::mips2machineCode(int base)
{
    fstream machine_code_file;
    machine_code_file.open("machine_code.coe", ios_base::out);
    machine_code_file << "memory_initialization_radix = 16;" << endl;
    machine_code_file << "memory_initialization_vector =" << endl;
    int Instruction_addr = base;//cpuָ������ַ
    for (int i = 0; i <= mips_counter; i++)
    {
        if (strcmp(mips[i].op, str_label) != 0)
        {
            mips[i].instruction_addr = Instruction_addr;
            Instruction_addr += 4;
        }
    }
    mips[mips_counter].instruction_addr = Instruction_addr;

    for (int i = mips_counter - 1; i >= 0; i--)
    {
        if (strcmp(mips[i].op, str_label) == 0)
        {
            mips[i].instruction_addr = mips[i+1].instruction_addr;
        }
    }

    unsigned int ans;
    unsigned int index;
    for (int i = 0; i <= mips_counter; i++)
    {
        if (strcmp(mips[i].op, str_label) == 0)
        {
            continue;
        }
        else if (strcmp(mips[i].op, str_add) == 0)
        {
            ans = 0 | 0x20;
            index = regName2num(mips[i].rs);
            ans |= (index << rs_base);
            index = regName2num(mips[i].rt);
            ans |= (index << rt_base);
            index = regName2num(mips[i].rd);
            ans |= (index << rd_base);
        }
        else if (strcmp(mips[i].op, str_addi) == 0)
        {
            ans = 0 | (0x8 << op_base);
            index = regName2num(mips[i].rs);
            ans |= (index << rs_base);
            index = regName2num(mips[i].rt);
            ans |= (index << rt_base);
            int imm = atoi(mips[i].imm);
            imm &= 0xffff;
            ans |= imm;
        }
        else if (strcmp(mips[i].op, str_and) == 0)
        {
            ans = 0 | 0x24;
            index = regName2num(mips[i].rs);
            ans |= (index << rs_base);
            index = regName2num(mips[i].rt);
            ans |= (index << rt_base);
            index = regName2num(mips[i].rd);
            ans |= (index << rd_base);
        }
        else if (strcmp(mips[i].op, str_andi) == 0)
        {
            ans = 0 | (0xc << op_base);
            index = regName2num(mips[i].rs);
            ans |= (index << rs_base);
            index = regName2num(mips[i].rt);
            ans |= (index << rt_base);
            int imm = atoi(mips[i].imm);
            imm &= 0xffff;
            ans |= imm;
        }
        else if (strcmp(mips[i].op, str_beq) == 0)
        {
            ans = 0 | (0x4 << op_base);
            index = regName2num(mips[i].rs);
            ans |= (index << rs_base);
            index = regName2num(mips[i].rt);
            ans |= (index << rt_base);
            int aim_addr = getAddrOfLabel(mips[i].target);
            int imm = aim_addr - mips[i].instruction_addr;
            imm = imm >> 2;
            imm &= 0xffff;
            ans |= imm;
        }
        else if (strcmp(mips[i].op, str_bne) == 0)
        {
            ans = 0 | (0x5 << op_base);
            index = regName2num(mips[i].rs);
            ans |= (index << rs_base);
            index = regName2num(mips[i].rt);
            ans |= (index << rt_base);
            int aim_addr = getAddrOfLabel(mips[i].target);
            int imm = aim_addr - mips[i].instruction_addr;
            imm = imm >> 2;
            imm &= 0xffff;
            ans |= imm;
        }
        else if (strcmp(mips[i].op, str_div) == 0)
        {
            ans = 0 | 0x1a;
            index = regName2num(mips[i].rs);
            ans |= (index << rs_base);
            index = regName2num(mips[i].rt);
            ans |= (index << rt_base);
        }
        else if (strcmp(mips[i].op, str_j) == 0)
        {
            ans = 0 | (0x2 << op_base);
            int aim_addr = getAddrOfLabel(mips[i].target);
            aim_addr &= 0x0fffffff;
            aim_addr = aim_addr >> 2;
            ans |= aim_addr;
        }
        else if (strcmp(mips[i].op, str_jal) == 0)
        {
            ans = 0 | (0x3 << op_base);
            int aim_addr = getAddrOfLabel(mips[i].target);
            aim_addr &= 0x0fffffff;
            aim_addr = aim_addr >> 2;
            ans |= aim_addr;
        }
        else if (strcmp(mips[i].op, str_jr) == 0)
        {
            ans = 0 | 0x8;
            index = regName2num(mips[i].rs);
            ans |= (index << rs_base);
        }
        else if (strcmp(mips[i].op, str_lui) == 0)
        {
            ans = 0 | (0xf << op_base);
            index = regName2num(mips[i].rt);
            ans |= (index << rt_base);
            int imm = atoi(mips[i].imm);
            imm &= 0xffff;
            ans |= imm;
        }
        else if (strcmp(mips[i].op, str_lw) == 0)
        {
            ans = 0 | (0x23 << op_base);
            index = regName2num(mips[i].rs);
            ans |= (index << rs_base);
            index = regName2num(mips[i].rt);
            ans |= (index << rt_base);
            int imm = atoi(mips[i].imm);
            imm &= 0xffff;
            ans |= imm;
        }
        else if (strcmp(mips[i].op, str_mflo) == 0)
        {
            ans = 0 | 0x12;
            index = regName2num(mips[i].rd);
            ans |= (index << rd_base);
        }
        else if (strcmp(mips[i].op, str_mult) == 0)
        {
            ans = 0 | 0x18;
            index = regName2num(mips[i].rs);
            ans |= (index << rs_base);
            index = regName2num(mips[i].rt);
            ans |= (index << rt_base);
        }
        else if (strcmp(mips[i].op, str_or) == 0)
        {
            ans = 0 | 0x25;
            index = regName2num(mips[i].rs);
            ans |= (index << rs_base);
            index = regName2num(mips[i].rt);
            ans |= (index << rt_base);
            index = regName2num(mips[i].rd);
            ans |= (index << rd_base);
        }
        else if (strcmp(mips[i].op, str_ori) == 0)
        {
            ans = 0 | (0xd << op_base);
            index = regName2num(mips[i].rs);
            ans |= (index << rs_base);
            index = regName2num(mips[i].rt);
            ans |= (index << rt_base);
            int imm = atoi(mips[i].imm);
            imm &= 0xffff;
            ans |= imm;
        }
        else if (strcmp(mips[i].op, str_nor) == 0)
        {
            ans = 0 | 0x27;
            index = regName2num(mips[i].rs);
            ans |= (index << rs_base);
            index = regName2num(mips[i].rt);
            ans |= (index << rt_base);
            index = regName2num(mips[i].rd);
            ans |= (index << rd_base);
        }
        else if (strcmp(mips[i].op, str_sll) == 0)
        {
            ans = 0;
            index = regName2num(mips[i].rt);
            ans |= (index << rt_base);
            index = regName2num(mips[i].rd);
            ans |= (index << rd_base);
            int imm = atoi(mips[i].imm);
            ans |= (imm << sa_base);
        }
        else if (strcmp(mips[i].op, str_slt) == 0)
        {
            ans = 0 | 0x2a;
            index = regName2num(mips[i].rs);
            ans |= (index << rs_base);
            index = regName2num(mips[i].rt);
            ans |= (index << rt_base);
            index = regName2num(mips[i].rd);
            ans |= (index << rd_base);
        }
        else if (strcmp(mips[i].op, str_sltu) == 0)
        {
            ans = 0 | 0x2b;
            index = regName2num(mips[i].rs);
            ans |= (index << rs_base);
            index = regName2num(mips[i].rt);
            ans |= (index << rt_base);
            index = regName2num(mips[i].rd);
            ans |= (index << rd_base);
        }
        else if (strcmp(mips[i].op, str_slti) == 0)
        {
            ans = 0 | (0xa << op_base);
            index = regName2num(mips[i].rs);
            ans |= (index << rs_base);
            index = regName2num(mips[i].rt);
            ans |= (index << rt_base);
            int imm = atoi(mips[i].imm);
            imm &= 0xffff;
            ans |= imm;
        }
        else if (strcmp(mips[i].op, str_sltiu) == 0)
        {
            ans = 0 | (0xb << op_base);
            index = regName2num(mips[i].rs);
            ans |= (index << rs_base);
            index = regName2num(mips[i].rt);
            ans |= (index << rt_base);
            int imm = atoi(mips[i].imm);
            imm &= 0xffff;
            ans |= imm;
        }
        else if (strcmp(mips[i].op, str_sub) == 0)
        {
            ans = 0 | 0x22;
            index = regName2num(mips[i].rs);
            ans |= (index << rs_base);
            index = regName2num(mips[i].rt);
            ans |= (index << rt_base);
            index = regName2num(mips[i].rd);
            ans |= (index << rd_base);
        }
        else if (strcmp(mips[i].op, str_subu) == 0)
        {
            ans = 0 | 0x23;
            index = regName2num(mips[i].rs);
            ans |= (index << rs_base);
            index = regName2num(mips[i].rt);
            ans |= (index << rt_base);
            index = regName2num(mips[i].rd);
            ans |= (index << rd_base);
        }
        else if (strcmp(mips[i].op, str_sw) == 0)
        {
            ans = 0 | (0x2b << op_base);
            index = regName2num(mips[i].rs);
            ans |= (index << rs_base);
            index = regName2num(mips[i].rt);
            ans |= (index << rt_base);
            int imm = atoi(mips[i].imm);
            imm &= 0xffff;
            ans |= imm;
        }
        else if (strcmp(mips[i].op, str_syscall) == 0)
        {
            ans = 0 | 0xc;
        }
        else if (strcmp(mips[i].op, str_nop) == 0)
        {
            ans = 0;
        }
        else
        {
            cout << "��⵽�Ƿ���mipsָ��" << endl;
            ans = 0;
        }

        machine_code_file << setfill('0') << hex << setw(8) << ans << endl;
    }
    ans = 0;
    machine_code_file << setfill('0') << hex << setw(8) << ans << endl;
}
int NaiveCompiler::getAddrOfLabel(char* label)
{
    for (int i = 0; i <= mips_counter; i++)
    {
        if (strcmp(mips[i].op, str_label) == 0
         && strcmp(mips[i].rs, label) == 0)
        {
            return mips[i].instruction_addr;
        }
    }
    return -1;
}
int NaiveCompiler::regName2num(char* reg_name)
{
    if (strcmp(reg_name, reg_$0) == 0)
        return 0;
    else if (strcmp(reg_name, reg_v0) == 0)
        return 2;
    else if (strcmp(reg_name, reg_v1) == 0)
        return 3;
    else if (strcmp(reg_name, reg_a0) == 0)
        return 4;
    else if (strcmp(reg_name, reg_a1) == 0)
        return 5;
    else if (strcmp(reg_name, reg_a2) == 0)
        return 6;
    else if (strcmp(reg_name, reg_a3) == 0)
        return 7;
    else if (strcmp(reg_name, reg_t0) == 0)
        return 8;
    else if (strcmp(reg_name, reg_t1) == 0)
        return 9;
    else if (strcmp(reg_name, reg_t2) == 0)
        return 10;
    else if (strcmp(reg_name, reg_t3) == 0)
        return 11;
    else if (strcmp(reg_name, reg_t4) == 0)
        return 12;
    else if (strcmp(reg_name, reg_t5) == 0)
        return 13;
    else if (strcmp(reg_name, reg_t6) == 0)
        return 14;
    else if (strcmp(reg_name, reg_t7) == 0)
        return 15;
    else if (strcmp(reg_name, reg_s0) == 0)
        return 16;
    else if (strcmp(reg_name, reg_s1) == 0)
        return 17;
    else if (strcmp(reg_name, reg_s2) == 0)
        return 18;
    else if (strcmp(reg_name, reg_s3) == 0)
        return 19;
    else if (strcmp(reg_name, reg_s4) == 0)
        return 20;
    else if (strcmp(reg_name, reg_s5) == 0)
        return 21;
    else if (strcmp(reg_name, reg_s6) == 0)
        return 22;
    else if (strcmp(reg_name, reg_s7) == 0)
        return 23;
    else if (strcmp(reg_name, reg_t8) == 0)
        return 24;
    else if (strcmp(reg_name, reg_t9) == 0)
        return 25;
    else if (strcmp(reg_name, reg_k0) == 0)
        return 26;
    else if (strcmp(reg_name, reg_k1) == 0)
        return 27;
    else if (strcmp(reg_name, reg_gp) == 0)
        return 28;
    else if (strcmp(reg_name, reg_sp) == 0)
        return 29;
    else if (strcmp(reg_name, reg_fp) == 0)
        return 30;
    else if (strcmp(reg_name, reg_ra) == 0)
        return 31;
    else 
    {
        cout << "���ɻ���������м�⵽�Ƿ��ļĴ�������:" << reg_name << endl;
        return -1;
    }
}
void NaiveCompiler::intcon2ints()
{
    char parent[ALNG];
    int func_start, func_end;
    for (int i = 0; i < mid_counter; i++)
    {
        //���ҵ���������ֹ��
        if (strcmp(midcode[i].op, str_func_def) != 0)
            continue;
        strcpy_s(parent, midcode[i].rt);
        func_start = i;
        i++;
        while (i < mid_counter && strcmp(midcode[i].op, str_func_def) != 0)
            i++;
        i--;
        func_end = i;

        for (int j = func_start; j <= func_end; j++)
        {
            int index = getIndexByNameAndParent(parent, midcode[j].rs, search_mode);
            if (index != 1 && tab[index].obj == con && tab[index].typ == ints)
                _itoa_s(tab[index].value, midcode[j].rs, 10);
            index = getIndexByNameAndParent(parent, midcode[j].rt, search_mode);
            if (index != 1 && tab[index].obj == con && tab[index].typ == ints)
                _itoa_s(tab[index].value, midcode[j].rt, 10);
            if (strcmp(midcode[j].op, "[]=") == 0)
            {
                index = getIndexByNameAndParent(parent, midcode[j].rd, search_mode);
                if (index != 1 && tab[index].obj == con && tab[index].typ == ints)
                    _itoa_s(tab[index].value, midcode[j].rd, 10);
            }
        }
    }
}
