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
    mips_counter = 0;

    midcodefile.open("midcode.txt", ios_base::out);
    programma_info.open("programma_analysis.txt", ios_base::out);
    mipsfile.open("mips.asm", ios_base::out);

    f = -1;

    strcpy_s(string_global, "global");
    strcpy_s(string_main, "main");

    strcpy_s(key[0], "case                                              ");
    strcpy_s(key[1], "char                                              ");
    strcpy_s(key[2], "const                                             ");
    strcpy_s(key[3], "do                                                ");
    strcpy_s(key[4], "if                                                ");
    strcpy_s(key[5], "int                                               ");
    strcpy_s(key[6], "main                                              ");
    strcpy_s(key[7], "printf                                            ");
    strcpy_s(key[8], "return                                            ");
    strcpy_s(key[9], "scanf                                             ");
    strcpy_s(key[10],"switch                                            ");
    strcpy_s(key[11],"void                                              ");
    strcpy_s(key[12],"while                                             ");

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

void NaiveCompiler::allocHelpVar(char* ans)
{
    f += 1;
    ans[0]='t';
    _itoa_s(f, &ans[1], ALNG-2, 10);
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
        midcodefile << rs << " " << op << " "  << rt << endl;
    else if (strcmp(op, "ret") == 0)
        midcodefile << op << endl;
    else if (strcmp(op, "GOTO") == 0)
        midcodefile << "GOTO " << rs << endl;
    else if (strcmp(op, "BNZ") == 0)
        midcodefile << "BNZ " << rs << endl;
    else if (strcmp(op, "BZ") == 0)
        midcodefile << "BZ " << rs << endl;
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
                error("��������ӦΪ���ͣ���ִ����ʽ����ת��");
            }
        }
        else
        {
            //����
            error("��[�� ���治�Ǳ��ʽ");
            /*set<symbol> begin_symbol_set;
            begin_symbol_set.insert(semicomma);
            skip(begin_symbol_set);
            return ;*/
        }

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
            error("��ֵ���Ͳ�һ�£���ִ����ʽ����ת��");
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
    strcpy_s(end, "END");
    f += 1;
    _itoa_s(f, &end[3], ALNG-4, 10);

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
                append_midcode("BNZ", nullptr, label);
            else
                append_midcode("BZ", nullptr, label);
        }
        else
            error("��ϵ�����Ҳ�ȱ�ٱ��ʽ");
    }
    //����ֻ��һ�����ʽ��û�й�ϵ�����
    else
    {
        append_midcode("!=", nullptr, rs, "0");
        if (jump)
            append_midcode("BNZ", nullptr, label);
        else
            append_midcode("BZ", nullptr, label);
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
                tab[t].address = dx;
                dx += 4;

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
                tab[t].address = dx;
                dx += 4;

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
        strcpy_s(label, "label");
        f += 1;
        _itoa_s(f, &label[5], ALNG-6, 10);
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
}

void NaiveCompiler::errormsg()
{
    list<pair<int, string>>::iterator iter;
    for (iter = errors.begin(); iter != errors.end(); iter++)
        fout << "��" << (*iter).first << "�У�" << (*iter).second << endl;
    
}

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
                    error("��������ӦΪ���ͣ���ִ����ʽ����ת��");
                }
                    
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
        rd[0] = symbols[0].chval;
        rd[1] = 9;//���ǰ������ַ�����������
        rd[2] = '\0';
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
    errormsg();
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

void NaiveCompiler::if_statement(char (&parent)[ALNG])
{
    
    char label[ALNG] = {};
    strcpy_s(label, "label");
    f += 1;
    _itoa_s(f, &label[5], ALNG-6, 10);

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
        memset(aim.id, ' ', sizeof(aim.id));
        aim.id[0] = tolower(ch);
        aim.id[ALNG-1] = '\0';
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

void NaiveCompiler::midcode2mips()
{
    int i = 0;
    while (midcode[i].op != nullptr)
    {
        if (strcmp(midcode[i].op, "+"))
        {

        }
        else if (strcmp(midcode[i].op, "-"))
        {

        }
        else if (strcmp(midcode[i].op, "*"))
        {

        }
        else if (strcmp(midcode[i].op, "/"))
        {

        }
        else if (strcmp(midcode[i].op, "push"))
        {

        }
        else if (strcmp(midcode[i].op, "call"))
        {

        }
        else if (strcmp(midcode[i].op, "="))
        {

        }
        else if (strcmp(midcode[i].op, "=="))
        {

        }
        else if (strcmp(midcode[i].op, "!="))
        {

        }
        else if (strcmp(midcode[i].op, ">"))
        {

        }
        else if (strcmp(midcode[i].op, ">="))
        {

        }
        else if (strcmp(midcode[i].op, "<"))
        {

        }
        else if (strcmp(midcode[i].op, "<="))
        {

        }
        else if (strcmp(midcode[i].op, "GOTO"))
        {

        }
        else if (strcmp(midcode[i].op, "BNZ"))
        {

        }
        else if (strcmp(midcode[i].op, "BZ"))
        {

        }
        else if (strcmp(midcode[i].op, "ret"))
        {

        }
        else if (strcmp(midcode[i].op, "[]="))
        {

        }
        else if (strcmp(midcode[i].op, "=[]"))
        {

        }
        else if (strcmp(midcode[i].op, "prt"))
        {

        }
        else if (strcmp(midcode[i].op, "scf"))
        {

        }
        else
        {
            //�����ǩ
        }


        i += 1;
    }
}

void NaiveCompiler::nextch()
{
    if (cc == ll)
    {
        if (fin.eof())
        {
            //fout << "incomplete program" << endl;
            errormsg();
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
    strcpy_s(nextCase, "nextcase");
    f += 1;
    _itoa_s(f, &nextCase[8], ALNG-9, 10);

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
    append_midcode("BZ", nullptr, nextCase);

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
            rs[0] = symbols[0].strval[i];
            rs[1] = 9;
            rs[2] = 0;
            append_midcode("prt", nullptr, rs);
        }
        
        //ȡ��stab�����ַ������ݱ�����symbol_record�����У���Ҫʱֱ�������
        insymbol(symbols[0]);
        if (symbols[0].sy == comma)
        {
            insymbol(symbols[0]);
            if (f_expression.count(symbols[0].sy))
            {
                expression(parent, rs, typ);
                append_midcode("prt", nullptr, rs);
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
        append_midcode("prt", nullptr, rs);

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
            append_midcode("=", "ret", rs);
        }
        else
            error("��⵽�Ƿ��ı��ʽ");

        if (symbols[0].sy == rparent)
            insymbol(symbols[0]);
        else
            error("ȱ�� )");
    }
    else if (symbols[0].sy == semicomma)
        append_midcode("ret");
    //����ֻ��һ��return���ٶ�һ���ַ��Ѿ����������ķ�Χ�ˣ����Բ��ñ���
    programma_info << "����һ����������䡱" << endl;
}

void NaiveCompiler::scanf_statement(char (&parent)[ALNG])
{
    
    char rd[ALNG];
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

        strcpy_s(rd, tab[index].name);
        append_midcode("scf", rd);

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

                append_midcode("scf", rd);

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
    char rs[ALNG];

    int func_index = getIndexByNameAndParent(string_global, called_func, search_mode);
    if (func_index == -1)
        error("�Ҳ���ֵ���������ĺ���");
    

    if (f_expression.count(symbols[0].sy))
    {
        //������ʽ
        expression(parent, rs, typ);
        count += 1;
        if (count <= tab[func_index].ref)
        {
            if (tab[func_index+count].typ == chars && typ == ints)
                error("���ܽ����͵�ֵ�����ַ��͵Ĳ���");
        }
        else
            error("ʵ�θ�������");
        
        append_midcode("push", nullptr, rs);
        
        while (symbols[0].sy == comma)
        {
            insymbol(symbols[0]);
            if (f_expression.count(symbols[0].sy))
            {
                expression(parent, rs, typ);
                count += 1;
                if (count <= tab[func_index].ref)
                {
                    if (tab[func_index+count].typ == chars && typ == ints)
                        error("���ܽ����͵�ֵ�����ַ��͵Ĳ���");
                }
                else
                    error("ʵ�θ�������");
                append_midcode("push", nullptr, rs);
            }
            else
                error("��⵽�Ƿ����ʽ");
        }
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

    if (symbols[0].sy == rbrack)
        insymbol(symbols[0]);
    else
        error("ȱ�� }");

    programma_info << "����һ�����޷���ֵ����������" << endl;
}

void NaiveCompiler::void_main()
{
    
    if (end == 1)
    {
        symbols[0].clear();
        symbols[1].clear();
        end = 0;
        append_midcode("func_def", nullptr, "void", "main");
        insymbol(symbols[0]);
    }
    else
    {
        insymbol(symbols[0]);
        if (symbols[0].sy == mainsy)
        {
            append_midcode("func_def", nullptr, "void", "main");
            insymbol(symbols[0]);
        }
        else
            error("ȱ�� main");
    }
    

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
        strcpy(mips[mips_counter].op, "add");
        strcpy(mips[mips_counter].rs, rs);
        strcpy(mips[mips_counter].rt, rt);
        strcpy(mips[mips_counter].rd, rd);
        mipsfile << mips[mips_counter].op << " " << rd << " " << rs << " " << rt << endl;
    }
    else
        error("mips����������������");
}

void NaiveCompiler::addi(char* rd, char* rs, int imm)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy(mips[mips_counter].op, "addi");
        strcpy(mips[mips_counter].rs, rs);
        itoa(imm, mips[mips_counter].imm, 10);
        strcpy(mips[mips_counter].rd, rd);
        mipsfile << mips[mips_counter].op << " " << rd << " " << rs << " " << imm << endl;
    }
    else
        error("mips����������������");
}

void NaiveCompiler::div(char* rs, char* rt)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy(mips[mips_counter].op, "div");
        strcpy(mips[mips_counter].rs, rs);
        strcpy(mips[mips_counter].rt, rt);
        mipsfile << mips[mips_counter].op << " " << rs << " " << rt << endl;
    }
    else
        error("mips����������������");
}

void NaiveCompiler::mult(char* rs, char* rt)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy(mips[mips_counter].op, "mult");
        strcpy(mips[mips_counter].rs, rs);
        strcpy(mips[mips_counter].rt, rt);
        mipsfile << mips[mips_counter].op << " " << rs << " " << rt << endl;
    }
    else
        error("mips����������������");
}

void NaiveCompiler::sub(char* rd, char* rs, char* rt)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy(mips[mips_counter].op, "sub");
        strcpy(mips[mips_counter].rs, rs);
        strcpy(mips[mips_counter].rt, rt);
        strcpy(mips[mips_counter].rd, rd);
        mipsfile << mips[mips_counter].op << " " << rd << " " << rs << " " << rt << endl;
    }
    else
        error("mips����������������");
}

void NaiveCompiler::subi(char* rd, char* rs, int imm)
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy(mips[mips_counter].op, "subi");
        strcpy(mips[mips_counter].rs, rs);
        itoa(imm, mips[mips_counter].imm, 10);
        strcpy(mips[mips_counter].rd, rd);
        mipsfile << mips[mips_counter].op << " " << rd << " " << rs << " " << imm << endl;
    }
    else
        error("mips����������������");
}

void NaiveCompiler::syscall()
{
    if (mips_counter < MIPS_CODE_SIZE - 1)
    {
        mips_counter += 1;
        strcpy(mips[mips_counter].op, "syscall");
        mipsfile << mips[mips_counter].op << endl;
    }
    else
        error("mips����������������");
}


