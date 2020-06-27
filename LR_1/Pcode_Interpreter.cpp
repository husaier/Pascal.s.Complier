//
// Created by ���� on 2020/6/27.
//

#include <fstream>
#include <sstream>
#include "Pcode_Interpreter.h"

//��ȡ����ַ
int Pcode_Interpreter::getBase(int nowBp, int lev) {
    int oldBp = nowBp;
    while (lev > 0) {
        oldBp = dataStack[oldBp + 1];
        lev--;
    }
    return oldBp;
}

//���ļ������ȡp-code��
vector<Pcode> Pcode_Interpreter::readFile(string filename) {
    vector<Pcode> allPcode;
    Pcode_Interpreter interpreter;
    string line_info, input_result, read;
    Pcode a;

    ifstream file(filename);

    if (!file.is_open())
        cout << "can not open this file" << endl;
    else {
        while (getline(file, line_info)) // line�в�����ÿ�еĻ��з�
        {
            stringstream input(line_info);
            //���������input_result�У�������vectorString��
            input >> read;

            if (read == "LIT")
                a.OP = LIT;
            else if (read == "LOD")
                a.OP = LOD;
            else if (read == "STO")
                a.OP = STO;
            else if (read == "CAL")
                a.OP = CAL;
            else if (read == "INT")
                a.OP = INT;
            else if (read == "JMP")
                a.OP = JMP;
            else if (read == "JPC")
                a.OP = JPC;
            else if (read == "OPR")
                a.OP = OPR;
            else {
                printf("����\n");
                return allPcode;
            }
            input >> read;
            a.L = stoi(read);
            input >> read;
            a.D = stoi(read);
            allPcode.push_back(a);
        }
        return allPcode;
    }
    return allPcode;
}


//������
void Pcode_Interpreter::interpreter(vector<Pcode> pcode) {
    int pc = 0; //�����������ָ����һ��ָ��
    int base = 0; //��ǰ����ַ
    int top = 0; //��������ջջ��
    int size = 0; //pcode�뼯�ϵĴ�С
    allPcode.assign(pcode.begin(), pcode.end());//ΪallPcode��ֵ
    size = allPcode.size();
    do {
        Pcode currentPcode = allPcode[pc];
        pc++;
        switch (currentPcode.OP) {
            case OPR:
                //OPR����ϵ���������㣬A��ָ����������
                switch (currentPcode.D) {
                    case 0:
                        //OPR 0 0   ���̵��ý�����,���ص��õ㲢��ջ
                        top = base;
                        pc = dataStack[base + 2];
                        base = dataStack[base];
                        break;
                    case 1:
                        //OPR 0 1     ջ��Ԫ��ȡ��
                        dataStack[top - 1] = -dataStack[top - 1];
                        break;
                    case 2:
                        //OPR 0 2   ��ջ����ջ����ӣ�������ջԪ�أ����ֵ��ջ
                        dataStack[top - 2] = dataStack[top - 1] + dataStack[top - 2];
                        top--;
                        break;
                    case 3:
                        //OPR 0 3   ��ջ����ȥջ����������ջԪ�أ����ֵ��ջ
                        dataStack[top - 2] = dataStack[top - 2] - dataStack[top - 1];
                        top--;
                        break;
                    case 4:
                        //OPR 0 4     ��ջ������ջ����������ջԪ�أ����ֵ��ջ
                        dataStack[top - 2] = dataStack[top - 1] * dataStack[top - 2];
                        top--;
                        break;
                    case 5:
                        //OPR 0 5     ��ջ������ջ����������ջԪ�أ����ֵ��ջ
                        dataStack[top - 2] = dataStack[top - 2] / dataStack[top - 1];
                        top--;
                        break;
                    case 6:
                        //OPR 0 6   ջ��Ԫ�ص���ż�жϣ����ֵ��ջ��
                        dataStack[top - 1] = dataStack[top - 1] % 2;
                        break;
                    case 8:
                        //OPR 0 8   ��ջ����ջ���Ƿ���ȣ�������ջԪ�أ����ֵ��ջ
                        if (dataStack[top - 2] == dataStack[top - 1]) {
                            dataStack[top - 2] = 1;
                        } else {
                            dataStack[top - 2] = 0;
                        }
                        top--;
                        break;
                    case 9:
                        //OPR 0 9   ��ջ����ջ���Ƿ񲻵ȣ�������ջԪ�أ����ֵ��ջ
                        if (dataStack[top - 2] != dataStack[top - 1]) {
                            dataStack[top - 2] = 1;
                        } else {
                            dataStack[top - 2] = 0;
                        }
                        top--;
                        break;
                    case 10:
                        //OPR 0 10  ��ջ���Ƿ�С��ջ����������ջԪ�أ����ֵ��ջ
                        if (dataStack[top - 2] < dataStack[top - 1]) {
                            dataStack[top - 2] = 1;
                        } else {
                            dataStack[top - 2] = 0;
                        }
                        top--;
                        break;
                    case 11:
                        //OPR 0 11    ��ջ���Ƿ���ڵ���ջ����������ջԪ�أ����ֵ��ջ
                        if (dataStack[top - 2] >= dataStack[top - 1]) {
                            dataStack[top - 2] = 1;
                        } else {
                            dataStack[top - 2] = 0;
                        }
                        top--;
                        break;
                    case 12:
                        //OPR 0 12  ��ջ���Ƿ����ջ����������ջԪ�أ����ֵ��ջ
                        if (dataStack[top - 2] > dataStack[top - 1]) {
                            dataStack[top - 2] = 1;
                        } else {
                            dataStack[top - 2] = 0;
                        }
                        top--;
                        break;
                    case 13:
                        //OPR 0 13  ��ջ���Ƿ�С�ڵ���ջ����������ջԪ�أ����ֵ��ջ
                        if (dataStack[top - 2] <= dataStack[top - 1]) {
                            dataStack[top - 2] = 1;
                        } else {
                            dataStack[top - 2] = 0;
                        }
                        top--;
                        break;
                    case 14:
                        //OPR 0 14  ջ��ֵ�������Ļ
                        cout << dataStack[top - 1] << " ";
                        break;
                    case 15:
                        //OPR 0 15  ��Ļ�������
                        cout << endl;
                        break;
                    case 16:
                        //OPR 0 16  �������ж���һ����������ջ��
                        cout << "please input a number" << endl;
                        cin >> dataStack[top];
                        top++;
                        break;
                    default:
                        cout << "OPRָ����ִ���" << currentPcode.D << " " << pc;
                        pc = 0;
                        break;
                }
                break;
            case LIT:
                //LIT���������͵�����ջS��ջ������ʱA��Ϊ����ֵ
                dataStack[top] = currentPcode.D;
                top++;
                break;
            case LOD:
                // LOD���������͵�����ջS��ջ������ʱA��Ϊ��������˵�����е����λ�á�
                dataStack[top] = dataStack[currentPcode.D + getBase(base, currentPcode.L)];
                top++;
                break;
            case STO:
                //STO��������ջS��ջ����������ĳ��������Ԫ�У�A��Ϊ��������˵�����е����λ�á�
                dataStack[currentPcode.D + getBase(base, currentPcode.L)] = dataStack[top - 1];
                top--;
                break;
            case CAL:
                //CAL�����ù��̣���ʱA��Ϊ�����ù��̵Ĺ����壨������֮ǰһ��ָ���Ŀ�����������ڵ�ַ��
                //��תʱ�����ò����ַ����ת�����ַ��pcָ�뱣����ջ��
                //����ַbase��Ϊ��ʱջ��top��pcָ��Ҫ��ת�ĵط�
                //���޸�top����Ϊǰ������Ѿ���address+3������Pcode������INT��䣬�޸�topֵ
                dataStack[top] = base;
                dataStack[top + 1] = getBase(base, currentPcode.L);
                dataStack[top + 2] = pc;
                base = top;
                pc = currentPcode.D;
                break;
            case INT:
                //INT��Ϊ�����õĹ��̣����������̣�������ջS�п�������������ʱA��Ϊ�������ݵ�Ԫ���������������������ݣ���L�κ�Ϊ0��
                top = top + currentPcode.D;
                break;
            case JMP:
                //JMP��������ת�ƣ���ʱA��Ϊת���ַ��Ŀ����򣩡�
                pc = currentPcode.D;
                break;
            case JPC:
                //JPC������ת�ƣ�������ջS��ջ���Ĳ���ֵΪ�٣�0��ʱ����ת��A����ָĿ������ַ������˳��ִ�С�
                if (dataStack[top - 1] == 0) {
                    pc = currentPcode.D;
                }
                break;
        }
        if(pc == 0 || pc >= size)
            cout<< 1;
    } while (pc != 0 && pc < size);
}