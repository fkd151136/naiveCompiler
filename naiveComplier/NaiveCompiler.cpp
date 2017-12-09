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
    display[0] = 1;
    dx = 0;
    ln = 0;
    mid_counter = 0;
    mips_counter = -1;

    midcodefile.open("midcode.txt", ios_base::out);
    programma_info.open("programma_analysis.txt", ios_base::out);
    mipsfile.open("mips.asm", ios_base::out);

    f = -1;

    strcpy_s(string_global, "global");
    strcpy_s(string_main, "main");
    strcpy_s(reg_sp, "$sp");
    strcpy_s(reg_fp, "$fp");
    strcpy_s(reg_gp, "$gp");
    strcpy_s(reg_t9, "$t9");

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
    f += 1;
    strcpy_s(ans, "fangkedongt");
    _itoa_s(f, &ans[11], ALNG-12, 10);
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

    if (strcmp(op, "func_def") == 0)
        midcodefile << rs << " " << rt << "()" << endl;
    else if (strcmp(op, "para") == 0)
        midcodefile << op << " " << rs << " " << rt << endl;
    else if (strcmp(op, "push") == 0)
        midcodefile << op << " " << rs << endl;
    else if (strcmp(op, "call") == 0)
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
    else if (strcmp(op, "BNZ") == 0)
        midcodefile << "BNZ " << rd << endl;
    else if (strcmp(op, "BZ") == 0)
        midcodefile << "BZ " << rd << endl;
    else if (strcmp(op, "ret") == 0)
    {
        if (rs == nullptr)
            midcodefile << op << endl;
        else
            midcodefile << op << " " << rs << endl;
    }
        
    else if (strcmp(op, "GOTO") == 0)
        midcodefile << "GOTO " << rs << endl;
    else if (strcmp(op, "=[]") == 0)
        midcodefile << rd << "=" << rs << "[" << rt << "]" << endl;
    else if (strcmp(op, "[]=") == 0)
        midcodefile << rs << "[" << rt << "]=" << rd << endl;
    else if (strcmp(op, "prt") == 0)
        midcodefile << op << " " << rs << endl;
    else if (strcmp(op, "scf") == 0)
        midcodefile << op << " " << rd << endl;
    else
        midcodefile << op << ":" << endl;
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
        /*set<symbol> begin_symbol_set;
        begin_symbol_set.insert(semicomma);
        skip(begin_symbol_set);
        return ;*/
    }

    if (tab[index].obj != var && tab[index].obj != arr && tab[index].obj != para)
    {
        //��ʶ�����Ǳ���
        string msg = symbols[0].id;
        msg.append("��ֵ�����޸�");
        error(msg);
        /*set<symbol> begin_symbol_set;
        begin_symbol_set.insert(semicomma);
        skip(begin_symbol_set);
        return ;*/
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
            /*set<symbol> begin_symbol_set;
            begin_symbol_set.insert(semicomma);
            skip(begin_symbol_set);
            return ;*/
        }

        typ = atab[tab[index].ref].elementType;
        insymbol(symbols[0]);
        if (f_expression.count(symbols[0].sy))
        {
            types indexType;
            expression(parent, rt, indexType);
            if (indexType == chars)
            {
                int value = (int)rt[0];
                memset(rt, 0, sizeof(rt));
                _itoa_s(value, rt, 10);
                error("��������ӦΪ����");
            }
            else if (atoi(rt) >= atab[tab[index].ref].size)
                error("�������������Ͻ�");
            else if (atoi(rt) < 0)
                error("�������������½�");
        }
        else
            error("��[�� ���治�Ǳ��ʽ");

        if (symbols[0].sy == rsquare)
            insymbol(symbols[0]);
        else
            error("ȱ�� ]");
    }
    else
        typ = tab[index].typ;

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
            int value = (int)rt[0];
            memset(rd, 0, sizeof(rd));
            _itoa_s(value, rd, 10);
            error("��ֵ������Ŀ�����Ͳ�һ��");
        }
        else if (typ == chars && valueType == ints)
        {
            error("�����͸����ַ��ͣ���Խ��ķ���");
            /*int value = atoi(rd);
            if (value == '+' || value == '-' || value == '*' || value == '/'
             || isdigit(value || islower(value) || isupper(value) || value == '_')
            {
                memset(rd, 0, sizeof(rd));
                rd[0] = value;
                rd[1] = 9;
                rd[2] = '\0';
            }
            else
                error("ֵԽ�磬�޷���");*/
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
    {
        //����
        error("�Ⱥ��Ҳ಻�Ǳ��ʽ");
        /*set<symbol> begin_symbol_set;
        begin_symbol_set.insert(semicomma);
        skip(begin_symbol_set);
        return ;*/
    }
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

    append_midcode("aar");

    value_parameter_list(parent, tab[index].name);//����Ϊ�գ��ʲ�����֤��ʼ���ż�

    if (symbols[0].sy == rparent)
        insymbol(symbols[0]);
    else
        error("ȱ�� )");

    append_midcode("call", nullptr, tab[index].name);
    strcpy_s(rd, "ret");
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

    append_midcode("aar");

    value_parameter_list(parent, tab[index].name);//����Ϊ�գ��ʲ�����֤��ʼ���ż�

    if (symbols[0].sy == rparent)
        insymbol(symbols[0]);
    else
        error("ȱ�� )");

    append_midcode("call", nullptr, tab[index].name);
    programma_info << "����һ�����޷���ֵ����������䡱" << endl;
}
void NaiveCompiler::caselist(char (&parent)[ALNG], types typ, char (&rs)[ALNG])
{
    set <int> exist_const;
    char end[ALNG];
    strcpy_s(end, "fkd_switchend");
    f += 1;
    _itoa_s(f, &end[13], ALNG-14, 10);

    one_case(parent, typ, end, rs, exist_const);

    while (f_case.count(symbols[0].sy))
        one_case(parent, typ, end, rs, exist_const);

    append_midcode(end);
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
        tab[index].value = dx;
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
                append_midcode("BNZ", label);
            else
                append_midcode("BZ", label);
        }
        else
            error("��ϵ�����Ҳ�ȱ�ٱ��ʽ");
    }
    //����ֻ��һ�����ʽ��û�й�ϵ�����
    else
    {
        append_midcode("!=", nullptr, rs, "0");
        if (jump)
            append_midcode("BNZ", label);
        else
            append_midcode("BZ", label);
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
            error("�������ʹ���");
            /*set<symbol> begin_symbol_set;
            begin_symbol_set.insert(semicomma);
            skip(begin_symbol_set);*/
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
            //�Ǽǳ�������
            strcpy_s(const_name, symbols[0].id);
            insymbol(symbols[0]);
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
            error("������int�͵ģ�ֵҲӦ����int�͵�");

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
                /*set<symbol> begin_symbol_set;
                begin_symbol_set.insert(semicomma);
                begin_symbol_set.insert(comma);
                skip(begin_symbol_set);*/
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
            //�Ǽǳ�������
            strcpy_s(const_name, symbols[0].id);
            insymbol(symbols[0]);
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
            error("������char�͵ģ�ֵҲӦ����char�͵�");

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
                /*set<symbol> begin_symbol_set;
                begin_symbol_set.insert(semicomma);
                begin_symbol_set.insert(comma);
                skip(begin_symbol_set);*/
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
void NaiveCompiler::do_while_statement(char (&parent)[ALNG])
{
    
    char label[ALNG];
    insymbol(symbols[0]);
    if (f_statement.count(symbols[0].sy))
    {
        strcpy_s(label, "fkd_dobegin");
        f += 1;
        _itoa_s(f, &label[11], ALNG-12, 10);
        append_midcode(label);
        bool has_return=false;
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
void NaiveCompiler::error(string content)
{
    pair<int, string> a(ln, content);
    errors.push_back(a);
    fout << "��" << ln << "�У�" << content << endl;
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
    if (symbols[0].sy ==ident)
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
                    int value = (int)rt[0];
                    memset(rt, 0, sizeof(rt));
                    _itoa_s(value, rt, 10);
                    error("��������ӦΪ����");
                }
                else if (atoi(rt) >= atab[tab[index].ref].size)
                    error("�������������Ͻ�");
                else if (atoi(rt) < 0)
                    error("�������������½�");
                    
                allocHelpVar(rd);
                append_midcode("=[]", rd, tab[index].name, rt);
            }
            else
                error("[ ���治�Ǳ��ʽ");

            
            if (symbols[0].sy == rsquare)
            {
                insymbol(symbols[0]);
            }
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
    //errormsg();
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
}
void NaiveCompiler::if_statement(char (&parent)[ALNG])
{
    
    char label[ALNG] = {};
    strcpy_s(label, "fangkd_ifend");
    f += 1;
    _itoa_s(f, &label[12], ALNG-13, 10);

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
        bool has_return=false;
        statement(parent, has_return);
    }
    else
        error("���ڷǷ����");

    
    append_midcode(label);//����ǩ�ӵ����
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
                printf("�޷���������Ӧ��0��ͷ������ \n");
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
                printf("��ֵ�����޷��������Ͻ磬���� \n");

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
                printf("�ַ�����ǵ����ţ�����\n");
                aim.sy = nosy;
                goto label_1;
            }
            else
                nextch();
        }
        else
        {
            //�����ź���ӵĲ�����ĸ�����ֻ�Ӽ��˳����ţ�����
            printf("�����ź���ӵĲ�����ĸ�����ֻ�Ӽ��˳����ţ�����\n");
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
            printf("�ַ������ݲ��Ϸ�\n");
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
            printf(" ! �ǲ��Ϸ��� \n");
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
        printf(" ����Ƿ����� \n");
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
        {
            factor(parent, rt, typ);
        }
        else
            error("��������湩�����ֵ");

        allocHelpVar(rd);
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
    if (isdigit(name[0]))//��ʶ�������������ֿ�ͷ���������ֿ�ͷ��һ���ǳ���
    {
        //���������
        addi(tar_reg, "$0", atoi(name));
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
                addi(tar_reg, "$0", tab[index].value);
            else
                lw(tar_reg, reg_t9, tab[index].address);
        } 
        else
        {
            if (tab[index].obj == con)
                addi(tar_reg, "$0", tab[index].value);
            else
                lw(tar_reg, reg_fp, tab[index].address);
        } 

    }

}
void NaiveCompiler::midcode2mips()
{
    char parent[ALNG];
    strcpy_s(parent, string_global);
    lui(reg_gp, 0x1000);//��ʼ������ջ�׵�ַ����������
    lui(reg_fp, 0x1004);//��ʼ������ջ�׵�ַ����������
    lui(reg_sp, 0x7fff);//��ʼ������ջ�׵�ַ����������
    addi(reg_sp, "$sp", 0xeffc);
    lui(reg_t9, 0x1001);//��ʼ��ȫ�ֱ����׵�ַ
    _j("main");
    //addi("$t9", "$0", 0x27fc);//$t9�������ջָ��
    for (int i = 0; i<mid_counter; i++)
    {
        if (strcmp(midcode[i].op, "+") == 0)
        {
            //lw lw add sw
            loadValue2reg(parent, midcode[i].rt, "$t1");
            loadValue2reg(parent, midcode[i].rs, "$t0");
            add("$t2", "$t0", "$t1");
            saveValue2mem(parent, midcode[i].rd, "$t2");
        }
        else if (strcmp(midcode[i].op, "-") == 0)
        {
            //lw lw sub sw
            loadValue2reg(parent, midcode[i].rt, "$t1");
            loadValue2reg(parent, midcode[i].rs, "$t0");
            sub("$t2", "$t0", "$t1");
            saveValue2mem(parent, midcode[i].rd, "$t2");
        }
        else if (strcmp(midcode[i].op, "*") == 0)
        {
            // lw lw mult mflo
            loadValue2reg(parent, midcode[i].rt, "$t1");
            loadValue2reg(parent, midcode[i].rs, "$t0");
            mult("$t0", "$t1");
            saveValue2mem(parent, midcode[i].rd, "lo");
        }
        else if (strcmp(midcode[i].op, "/") == 0)
        {
            // lw lw div mflo
            loadValue2reg(parent, midcode[i].rt, "$t1");
            loadValue2reg(parent, midcode[i].rs, "$t0");
            div("$t0", "$t1");
            saveValue2mem(parent, midcode[i].rd, "lo");
        }
        else if (strcmp(midcode[i].op, "aar") == 0)
        {
            //�ݲ�����
        }
        else if (strcmp(midcode[i].op, "push") == 0)
        {
            //lw sw
            //addi $sp $sp 4
            loadValue2reg(parent, midcode[i].rs, "$t0");
            sw("$t0", reg_gp, 0);
            addi(reg_gp, reg_gp, 4);
        }
        else if (strcmp(midcode[i].op, "call") == 0)
        {
            //jal
            int index = getIndexByNameAndParent(string_global, parent, search_mode);
            addi("$t0", reg_fp, 0);
            addi(reg_fp, reg_fp, tab[index].value);
            sw("$t0", reg_fp, 0);//������һ��fp
            /*sw("$s0", reg_fp, 12);//����$s0
            sw("$s1", reg_fp, 16);//����$s1
            sw("$s2", reg_fp, 20);//����$s2
            sw("$s3", reg_fp, 24);//����$s3
            sw("$s4", reg_fp, 28);//����$s4
            sw("$s5", reg_fp, 32);//����$s5
            sw("$s6", reg_fp, 36);//����$s6
            sw("$s7", reg_fp, 40);//����$s7*/
            jal(midcode[i].rs);
        }
        else if (strcmp(midcode[i].op, "=") == 0)
        {
            //lw, sw
            loadValue2reg(parent, midcode[i].rs, "$t0");
            saveValue2mem(parent, midcode[i].rd, "$t0");
        }
        else if (strcmp(midcode[i].op, "==") == 0)
        {
            //lw lw beq
            loadValue2reg(parent, midcode[i].rt, "$t1");
            loadValue2reg(parent, midcode[i].rs, "$t0");
            slt("$t2", "$t0", "$t1");
            slt("$t3", "$t1", "$t0");
            _nor("$t0", "$t2", "$t3");
            andi("$t0", "$t0", 0x1);
        }
        else if (strcmp(midcode[i].op, "!=") == 0)
        {
            //lw lw bne
            loadValue2reg(parent, midcode[i].rt, "$t1");
            loadValue2reg(parent, midcode[i].rs, "$t0");
            slt("$t2", "$t0", "$t1");
            slt("$t3", "$t1", "$t0");
            _or("$t0", "$t2", "$t3");
        }
        else if (strcmp(midcode[i].op, ">") == 0)
        {
            //lw lw bgt
            loadValue2reg(parent, midcode[i].rt, "$t1");
            loadValue2reg(parent, midcode[i].rs, "$t0");
            slt("$t2", "$t0", "$t1");
            slt("$t3", "$t1", "$t0");
            _nor("$t4", "$t2", "$t3");
            andi("$t4", "$t4", 0x1);
            _nor("$t0", "$t2", "$t4");
            andi("$t0", "$t0", 0x1);
        }
        else if (strcmp(midcode[i].op, ">=") == 0)
        {
            //lw lw bge
            loadValue2reg(parent, midcode[i].rt, "$t1");
            loadValue2reg(parent, midcode[i].rs, "$t0");
            slt("$t0", "$t1", "$t0");
        }
        else if (strcmp(midcode[i].op, "<") == 0)
        {
            //lw lw blt
            loadValue2reg(parent, midcode[i].rt, "$t1");
            loadValue2reg(parent, midcode[i].rs, "$t0");
            slt("$t0", "$t0", "$t1");
        }
        else if (strcmp(midcode[i].op, "<=") == 0)
        {
            //lw lw ble
            loadValue2reg(parent, midcode[i].rt, "$t1");
            loadValue2reg(parent, midcode[i].rs, "$t0");
            slt("$t2", "$t0", "$t1");
            slt("$t3", "$t1", "$t0");
            _nor("$t4", "$t2", "$t3");
            andi("$t4", "$t4", 0x1);
            _or("$t0", "$t2", "$t4");
        }
        else if (strcmp(midcode[i].op, "BNZ") == 0)
        {
            _bne("$t0", "$0", midcode[i].rd);
        }
        else if (strcmp(midcode[i].op, "BZ") == 0)
        {
            _beq("$t0", "$0", midcode[i].rd);
        }
        else if (strcmp(midcode[i].op, "GOTO") == 0)
        {
            //j
            _j(midcode[i].rs);
        }
        else if (strcmp(midcode[i].op, "ret") == 0)
        {
            //jr $ra
            if (midcode[i].rs[0] != '\0')
            {
                loadValue2reg(parent, midcode[i].rs, "$v0");
                sw("$v0", reg_sp, 0);
                addi("$sp", reg_sp, -4);
            }
            
            lw("$31", reg_fp, 8);
            lw(reg_fp, reg_fp, 0);
            jr(31);
        }
        else if (strcmp(midcode[i].op, "[]=") == 0)
        {
            //���������׵�ַ��������������ƫ�ƣ�lw sw
            //��ֵ���򣺼�ֵ->����Ԫ��
            int index = getIndexByNameAndParent(parent, midcode[i].rs, search_mode);
            int baseAddr = tab[index].address;
            addi("$t1", "$0", baseAddr);
            loadValue2reg(parent, midcode[i].rt, "$t2");
            //int element_offset = baseAddr + 4 * offset;
            sll("$t2", "$t2", 2);
            add("$t1", "$t1", "$t2");
            loadValue2reg(parent, midcode[i].rd, "$t0");
            if (strcmp(tab[index].parent, string_global) == 0)
                add("$t1", "$t1", reg_t9);
            else
                add("$t1", "$t1", reg_fp);
            sw("$t0", "$t1", 0);
        }
        else if (strcmp(midcode[i].op, "=[]") == 0)
        {
            //���������׵�ַ��������������ƫ�ƣ�lw sw
            //��ֵ��������Ԫ��->��ֵ
            int index = getIndexByNameAndParent(parent, midcode[i].rs, search_mode);
            int baseAddr = tab[index].address;
            addi("$t1", "$0", baseAddr);
            loadValue2reg(parent, midcode[i].rt, "$t2");
            //int element_offset = baseAddr + 4 * offset;
            sll("$t2", "$t2", 2);
            add("$t1", "$t1", "$t2");
            if (strcmp(tab[index].parent, string_global) == 0)
                add("$t1", "$t1", reg_t9);
            else
                add("$t1", "$t1", reg_fp);
            lw("$t0", "$t1", 0);
            saveValue2mem(parent, midcode[i].rd, "$t0");
        }
        else if (strcmp(midcode[i].op, "prt") == 0)
        {
            //addi $v0 $0 [����]�����ģʽ���ַ�or���֣�
            //lw
            //syscall
            if (strcmp(midcode[i].rt, "char") == 0)
                addi("$v0", "$0", 11);
            else
                addi("$v0", "$0", 1);

            loadValue2reg(parent, midcode[i].rs, "$a0");
            syscall();
        }
        else if (strcmp(midcode[i].op, "scf") == 0)//rd�ֶα�ʾ����ֵ������rs�ֶα�ʾ��������
        {
            //addi $v0 $0 [����]������ģʽ���ַ�or���֣�
            //syscall
            if (strcmp(midcode[i].rs, "char") == 0)
                addi("$v0", "$0", 12);
            else
                addi("$v0", "$0", 5);

            syscall();
            saveValue2mem(parent, midcode[i].rd, "$v0");
        }
        else if (strcmp(midcode[i].op, "func_def") == 0)
        {
            //label:
            memset(parent, 0, sizeof(parent));
            strcpy_s(parent, midcode[i].rt);
            generate_label(midcode[i].rt);
            sw("$31", reg_fp, 8);//���淵�ص�ַ��main����û��ϵ��������ı�������⸲�ǵ�������main����Ҫ���淵��ֵ
        }
        else if (strcmp(midcode[i].op, "para") == 0)
        {
            //lw sw 
            //addi $sp $sp -4
            int index = getIndexByNameAndParent(parent, midcode[i].rt, search_mode);
            lw("$t0", reg_gp, -4);
            addi(reg_gp, reg_gp, -4);
            sw("$t0", reg_fp, tab[index].address);
        }
        else
        {
            //�����ǩ
            generate_label(midcode[i].op);
        }
    }
}
void NaiveCompiler::nextch()
{
    if (cc == ll)
    {
        if (fin.eof())
        {
            //fout << "incomplete program" << endl;
            //errormsg();
            exit(0);
        }

        ll = -1;
        cc = -1;
        memset(line, ' ', sizeof(line));
        fin.getline(line, sizeof(line));
        ln += 1;
        if (fin.fail())
        {
            //printf("��ȡʧ�� \n");
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
void NaiveCompiler::one_case(char (&parent)[ALNG], types typ, char (&end)[ALNG], char (&rs)[ALNG], set<int> (&exist_const))
{
    
    char nextCase[ALNG];
    char rt[ALNG];
    strcpy_s(nextCase, "fkd_nextcase");
    f += 1;
    _itoa_s(f, &nextCase[12], ALNG-13, 10);

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
        error("case����Ӧ�ýӳ���");

    append_midcode("==", nullptr, rs, rt);
    append_midcode("BZ", nextCase);

    if (symbols[0].sy == colon)
        insymbol(symbols[0]);
    else
        error("ȱ�� :");

    if (f_statement.count(symbols[0].sy))
    {
        bool has_return=false;
        statement(parent, has_return);
        append_midcode("GOTO", nullptr, end);
    }
    else
        error("���ڷǷ����");

    append_midcode(nextCase);
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
            error("�������Ʋ��Ϸ�");

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
                tab[func_index].ref += 1;

            if (typ == ints)
                append_midcode("para", nullptr, "int", para_name);
            else if (typ == chars)
                append_midcode("para", nullptr, "char", para_name);
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
                    error("�������Ʋ��Ϸ�");

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
                            tab[func_index].ref += 1;

                        if (typ == ints)
                            append_midcode("para", nullptr, "int", para_name);
                        else if (typ == chars)
                            append_midcode("para", nullptr, "char", para_name);

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
        //����ַ�������
        for (int i = 0; i < symbols[0].sleng; i++)
        {
            _itoa_s(symbols[0].strval[i], rs, 10);
            strcpy_s(rt, "char");
            append_midcode("prt", nullptr, rs, rt);
        }
        
        //ȡ��stab�����ַ������ݱ�����symbol_record�����У���Ҫʱֱ�������
        insymbol(symbols[0]);
        if (symbols[0].sy == comma)
        {
            insymbol(symbols[0]);
            if (f_expression.count(symbols[0].sy))
            {
                expression(parent, rs, typ);
                if (typ == chars)
                    strcpy_s(rt, "char");
                else
                    strcpy_s(rt, "int");
                append_midcode("prt", nullptr, rs, "int");
            }
            else
                error("���ַǷ��ı��ʽ");
        }
        //����û�ж��źͱ��ʽ��ֻ���ַ���

        if (symbols[0].sy == rparent)
            insymbol(symbols[0]);
        else
            error("ȱ�� )");
    }
    else if (f_expression.count(symbols[0].sy))
    {
        expression(parent, rs, typ);
        if (typ == chars)
            strcpy_s(rt, "char");
        else
            strcpy_s(rt, "int");
        append_midcode("prt", nullptr, rs, rt);

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
    
    dx = 44;//�з���ֵ��������ʽ�����������ϼ�fp������ֵ�����ص�ַ��ÿ��ռ4�ֽڣ���ռ12�ֽڡ�����s0��s7���˸��Ĵ���Ҫ����
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
        t += 1;

        strcpy_s(tab[t].name, func_name);
        tab[t].obj = func;
        tab[t].typ = return_type;
        strcpy_s(tab[t].parent, string_global);
        tab[t].ref = 0;//��ʼ��������������������ʱ����

        if (return_type == ints)
            append_midcode("func_def", nullptr, "int", func_name);
        else if (return_type == chars)
            append_midcode("func_def", nullptr, "char", func_name);
        
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
        insymbol(symbols[0]);
        if (f_expression.count(symbols[0].sy))
        {
            expression(parent, rs, typ);
            append_midcode("ret", nullptr, rs);
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
        if (strcmp(parent, string_main) == 0)//���������������û����һ�������ˣ�ֱ����������ĩ��
            append_midcode("GOTO", nullptr, "fkd_program_end");
        else
            append_midcode("ret");
    }
        
    //����ֻ��һ��return���ٶ�һ���ַ��Ѿ����������ķ�Χ�ˣ����Բ��ñ���
    programma_info << "����һ����������䡱" << endl;
}
void NaiveCompiler::saveValue2mem(char (&parent)[ALNG], char (&name)[ALNG], char* src_reg)
{
    char src[ALNG];
    if (strcmp(src_reg, "lo") == 0)
    {
        mflo("$t0");
        strcpy_s(src, "$t0");
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
            strcpy_s(rs, "char");
        else if (tab[index].typ == ints)
            strcpy_s(rs, "int");
        else
        {
            string msg = tab[index].name;
            msg.append("�Ȳ����ַ���Ҳ�������ͣ���������޷�Ϊ֮��ֵ");
            error(msg);
        }

        strcpy_s(rd, tab[index].name);
        append_midcode("scf", rd, rs);

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
                    strcpy_s(rs, "char");
                else if (tab[index].typ == ints)
                    strcpy_s(rs, "int");
                else
                {
                    string msg = tab[index].name;
                    msg.append("�Ȳ����ַ���Ҳ�������ͣ���������޷�Ϊ֮��ֵ");
                    error(msg);
                }

                strcpy_s(rd, tab[index].name);
                append_midcode("scf", rd, rs);

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
        if_statement(parent);
    }
    else if (f_do_while.count(symbols[0].sy))
    {
        //����ѭ�����
        do_while_statement(parent);
    }
    else if (f_switch.count(symbols[0].sy))
    {
        //����������
        switch_statement(parent);
    }
    else if (symbols[0].sy == lbrack)
    {
        insymbol(symbols[0]);
        bool has_return=false;
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
                string msg = tab[index].name;
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
void NaiveCompiler::switch_statement(char (&parent)[ALNG])
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
        caselist(parent, typ, rs);
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
        if (count <= tab[func_index].ref)
        {
            if (tab[func_index+count].typ != typ)
                error("ʵ�����β����Ͳ���");
        }
        else
            error("ʵ�θ�������");
        
        //append_midcode("push", nullptr, rs);
        
        while (symbols[0].sy == comma)
        {
            insymbol(symbols[0]);
            if (f_expression.count(symbols[0].sy))
            {
                expression(parent, rs[count], typ);
                count += 1;
                if (count <= tab[func_index].ref)
                {
                    if (tab[func_index+count].typ == chars && typ == ints)
                        error("���ܽ����͵�ֵ�����ַ��͵Ĳ���");
                }
                else
                    error("ʵ�θ�������");
                //append_midcode("push", nullptr, rs);
            }
            else
                error("��⵽�Ƿ����ʽ");
        }

        for (int i=count-1; i>=0; i--)
            append_midcode("push", nullptr, rs[i]);
    }
    //����Ϊ��,�ʲ��ر���

    //����βθ�����ʵ�θ����Ƿ����
    if (func_index != -1)
    {
        if (count < tab[func_index].ref)
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

        strcpy_s(var_name, symbols[1].id);
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
                    atab[a].elementSize = typ == ints ? 4 : 1 ;//int : 4 bytes ; char : 1 byte
                    /*if (typ == ints)//����Ҫ4���ֽڣ�����Ҫ���룻�ַ���һ���ֽڣ�һֱ�Ƕ���ġ�
                        {
                            dx = (dx + 3) / 4;
                        }*/
                    tab[t].address = dx;
                    dx = dx + 4 * array_size;

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
                    /*if (typ == ints)//����Ҫ4���ֽڣ�����Ҫ���룻�ַ���һ���ֽڣ�һֱ�Ƕ���ġ�
                        {
                            dx = (dx + 3) / 4;
                        }*/
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
            //�ǼǷ�������
            strcpy_s(var_name, symbols[0].id);
            insymbol(symbols[0]);
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
                    atab[a].elementSize = typ == ints ? 4 : 1 ;//int : 4 bytes ; char : 1 byte
                    /*if (typ == ints)//����Ҫ4���ֽڣ�����Ҫ���룻�ַ���һ���ֽڣ�һֱ�Ƕ���ġ�
                        {
                            dx = (dx + 3) / 4;
                        }*/
                    tab[t].address = dx;
                    dx = dx + 4 * array_size;

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
                    /*if (typ == ints)//����Ҫ4���ֽڣ�����Ҫ���룻�ַ���һ���ֽڣ�һֱ�Ƕ���ġ�
                        {
                            dx = (dx + 3) / 4;
                        }*/
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
    
    dx = 44;//�з���ֵ��������ʽ�����������ϼ�fp������ֵ�����ص�ַ��ÿ��ռ4�ֽڣ���ռ12�ֽڡ�����s0��s7���˸��Ĵ���Ҫ����
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
        t += 1;

        strcpy_s(tab[t].name, func_name);
        tab[t].obj = func;
        tab[t].typ = notyp;
        strcpy_s(tab[t].parent, string_global);
        tab[t].ref = 0;//��ʼ��������������������ʱ����

        append_midcode("func_def", nullptr, "void", func_name);
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
    if (has_return == false)
        append_midcode("ret");

    

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
        append_midcode("func_def", nullptr, "void", string_main);
        insymbol(symbols[0]);
    }
    else
    {
        insymbol(symbols[0]);
        if (symbols[0].sy == mainsy)
        {
            append_midcode("func_def", nullptr, "void", string_main);
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
    t += 1;

    strcpy_s(tab[t].name, string_main);
    tab[t].obj = func;
    tab[t].typ = notyp;
    strcpy_s(tab[t].parent, string_global);
    tab[t].ref = 0;

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
    append_midcode("fkd_program_end");

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
        strcpy_s(mips[mips_counter].op, "add");
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
        strcpy_s(mips[mips_counter].op, "addi");
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
        strcpy_s(mips[mips_counter].op, "and");
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
        strcpy_s(mips[mips_counter].op, "andi");
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
        strcpy_s(mips[mips_counter].op, "beq");
        strcpy_s(mips[mips_counter].rs, rs);
        strcpy_s(mips[mips_counter].rt, rt);
        strcpy_s(mips[mips_counter].target, label);
        mipsfile << mips[mips_counter].op << " " << rs << " " << rt << " " << label << endl;
    }
    else
        error("mips����������������");
}
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
}
void NaiveCompiler::_bne(char* rs, char* rt, char* label)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy_s(mips[mips_counter].op, "bne");
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
        strcpy_s(mips[mips_counter].op, "div");
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
        strcpy_s(mips[mips_counter].op, label);
        mipsfile << mips[mips_counter].op << ":" << endl;
    }
    else
        error("mips����������������");
}
void NaiveCompiler::_j(char* target)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy_s(mips[mips_counter].op, "j");
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
        strcpy_s(mips[mips_counter].op, "jal");
        strcpy_s(mips[mips_counter].target, target);
        mipsfile << mips[mips_counter].op << " " << target << endl;
    }
    else
        error("mips����������������");
}
void NaiveCompiler::jr(int target_register)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy_s(mips[mips_counter].op, "jr");
        mips[mips_counter].target[0] = '$';
        _itoa_s(target_register, &mips[mips_counter].target[1], ALNG-2, 10);
        mipsfile << mips[mips_counter].op << " " << mips[mips_counter].target << endl;
    }
    else
        error("mips����������������");
}
void NaiveCompiler::lui(char* rt, int imm)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy_s(mips[mips_counter].op, "lui");
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
        strcpy_s(mips[mips_counter].op, "lw");
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
        strcpy_s(mips[mips_counter].op, "mflo");
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
        strcpy_s(mips[mips_counter].op, "mult");
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
        strcpy_s(mips[mips_counter].op, "nor");
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
        strcpy_s(mips[mips_counter].op, "or");
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
        strcpy_s(mips[mips_counter].op, "ori");
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
        strcpy_s(mips[mips_counter].op, "sll");
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
        strcpy_s(mips[mips_counter].op, "slt");
        strcpy_s(mips[mips_counter].rs, rs);
        strcpy_s(mips[mips_counter].rt, rt);
        strcpy_s(mips[mips_counter].rd, rd);
        mipsfile << mips[mips_counter].op << " " << rd << " " << rs << " " << rt << endl;
    }
    else
        error("mips����������������");
}
void NaiveCompiler::sub(char* rd, char* rs, char* rt)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy_s(mips[mips_counter].op, "sub");
        strcpy_s(mips[mips_counter].rs, rs);
        strcpy_s(mips[mips_counter].rt, rt);
        strcpy_s(mips[mips_counter].rd, rd);
        mipsfile << mips[mips_counter].op << " " << rd << " " << rs << " " << rt << endl;
    }
    else
        error("mips����������������");
}
void NaiveCompiler::subi(char* rt, char* rs, int imm)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy_s(mips[mips_counter].op, "subi");
        strcpy_s(mips[mips_counter].rs, rs);
        _itoa_s(imm, mips[mips_counter].imm, 10);
        strcpy_s(mips[mips_counter].rt, rt);
        mipsfile << mips[mips_counter].op << " " << rt << " " << rs << " " << imm << endl;
    }
    else
        error("mips����������������");
}
void NaiveCompiler::sw(char* rt, char* rs, int imm)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy_s(mips[mips_counter].op, "sw");
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
        strcpy_s(mips[mips_counter].op, "syscall");
        mipsfile << mips[mips_counter].op << endl;
    }
    else
        error("mips����������������");
}

