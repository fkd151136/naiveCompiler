#include "NaiveCompiler.h"

using namespace std;

int main(int args, char* argv[])//main
{
    NaiveCompiler compiler;
    char sourcefile[ALNG];
    printf("��������Ҫ������ļ�����\n");
    scanf("%s", &sourcefile);
    compiler.fin.open(sourcefile, ios_base::in);
    if (!compiler.fin)
    {
        printf("��Ҫ������ļ�������\n");
        exit(0);
        return 0;
    }
    compiler.fout.open("result.txt", ios_base::out);
    //�������з��ս���������֮ǰ��Ҫ�ȶ���һ��symbol
    compiler.insymbol(compiler.symbols[0]);//Ԥ�ȶ�һ������
    printf("��ʼ�����м���롭��\n");
    compiler.program();//���� ������
    if (!compiler.errors.empty())
    {
        //compiler.errormsg();
        printf("���ִ��󣬾��������Ϣ��鿴result.txt\n");
        compiler.fin.close();
        compiler.fout.close();
        exit(0);
        return 0;
    }

    printf("�м�����������\n");

    int mode = -1;
    printf("���������ѡ�(1��ʾ���Ż���ȫ�ּĴ�������ʹ��ͼ��ɫ����2��ʾ���Ż���ȫ�ּĴ�������ʹ�����ü������������ֱ�ʾ�����Ż�)\n");
    scanf("%d", &mode);
    compiler.opt_sign = (mode == 1 || mode == 2);


    int delay;
    printf("��ѡ���Ƿ�֧���ӳٲۣ�0��ʾ���ԣ��������ֱ�ʾ�����ԣ�\n");
    scanf("%d", &delay);
    compiler.delay_mode = (delay != 0);
    int machineCodeEn;
    printf("��ѡ���Ƿ����ɻ����룿1��ʾmarsģʽ��2��ʾ����cpuģʽ��3��ʾ��оcpuģʽ���������ֱ�ʾ��\n");
    scanf("%d", &machineCodeEn);

    if (mode == 1)
    {
        compiler.intcon2ints();
        printf("��ʼɾ�������ӱ��ʽ����\n");
        compiler.del_common_subexpression();
        compiler.del_common_expression();
        compiler.print_optimized_midcode("opt_midcode.txt");
        printf("�����ӱ��ʽɾ����ϡ���\n");
        printf("��ʼ���л�Ծ����������������ͻͼ��ʹ��ͼ��ɫ�㷨����ȫ�ּĴ�������\n");
        compiler.activeVarAnalysis();
        printf("����ȫ�ּĴ�����ϡ���\n");
        compiler.midcode2mips();
    }
    else if (mode == 2)
    {
        compiler.intcon2ints();
        printf("��ʼɾ�������ӱ��ʽ����\n");
        compiler.del_common_subexpression();
        compiler.del_common_expression();
        compiler.print_optimized_midcode("opt_midcode.txt");
        printf("�����ӱ��ʽɾ����ϡ���\n");
        printf("��ʼʹ�����ü���ȫ�ּĴ������䡭��\n");
        compiler.ref_count();
        printf("ȫ�ּĴ���������ϡ���\n");
        compiler.midcode2mips();
    }
    else
    {
        cout << "���벻�Ż�ģʽ����" << endl;
        compiler.midcode2mips();
    }

    if (machineCodeEn == 1)
    {
        printf("��ʼ���ɻ����롭��\n");
        compiler.mips2machineCode(0x00400000);
        printf("���ɻ�������ϡ���\n");
    }
    else if (machineCodeEn == 2)
    {
        printf("��ʼ���ɻ����롭��\n");
        compiler.mips2machineCode(0x00003000);
        printf("���ɻ�������ϡ���\n");
    }
    else if (machineCodeEn == 2)
    {
        printf("��ʼ���ɻ����롭��\n");
        compiler.mips2machineCode(0xbfc00000);
        printf("���ɻ�������ϡ���\n");
    }

    printf("������ɣ�\n");

    compiler.fin.close();
    compiler.fout.close();
    return 0;
}

