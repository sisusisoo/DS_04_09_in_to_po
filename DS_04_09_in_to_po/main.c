#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS

#define MAX_STACK_SIZE 100

int err = 0;  //�����߻� üũ ����

typedef char element;



typedef struct {
    element data[MAX_STACK_SIZE];
    int top;
} StackType;


void init_stack(StackType* s)
{
    s->top = -1;
}


int is_empty(StackType* s)
{
    return (s->top == -1);
}

int is_full(StackType* s)
{
    return (s->top == (MAX_STACK_SIZE - 1));
}

void push(StackType* s, element item)
{
    if (is_full(s)) {
        fprintf(stderr, "���� ��ȭ ����\n");
        return;
    }
    else s->data[++(s->top)] = item;
}

element pop(StackType* s)
{
    if (is_empty(s)) {
        fprintf(stderr, "���� ���� ����\n");
        exit(1);
    }
    else return s->data[(s->top)--];
}

element peek(StackType* s)
{
    if (is_empty(s)) {
        fprintf(stderr, "���� ���� ����\n");
        exit(1);
    }
    else return s->data[s->top];
}



int eval(char exp[])
{
    int op1, op2, value, i = 0;
    int len = strlen(exp);
    char ch;
    StackType s;

    init_stack(&s);
    for (i = 0; i < len; i++) {
        ch = exp[i];
        if (ch != '+' && ch != '-' && ch != '*' && ch != '/') {
            value = ch - '0';  
            push(&s, value);
        }
        else {
            op2 = pop(&s);
            op1 = pop(&s);
            switch (ch) { 
            case '+': push(&s, op1 + op2); break;
            case '-': push(&s, op1 - op2); break;
            case '*': push(&s, op1 * op2); break;
            case '/': push(&s, op1 / op2); break;
            }
        }
    }
    return pop(&s);
}


int prec(char op)// �������� �켱���� ��ȯ
{
    switch (op) {
    case '(': case ')': return 0;
    case '+': case '-': return 1;
    case '*': case '/': return 2;
    }
    return -1;
}

void check_error(element exp[]) {
    err = -1;
    int len = strlen(exp);


    for (int i = 0; i < len; i++) {
        if (i + 1 < len && exp[i] == '/' && exp[i + 1] == '0') {
            printf("<<error �߻�>>\n");
            printf("in_to_po error0: 0���� ����\n\n");
            err = 0;
            break;
        }
    }

    int count = 0;

    for (int i = 0; i < len; i++) {
        if (exp[i] == '(') {
            count++;
        }
        else if (exp[i] == ')') {
            count--;
        }
    }
    if (count > 0) {
        printf("<<error �߻�>>\n");
        printf("in_to_po error1: ��ȣ �̻� 1\n\n");
        err = 1;
    }
    else if (count < 0) {
        printf("<<error �߻�>>\n");
        printf("in_to_po error1: ��ȣ �̻� 2\n\n");
        err = 1;
    }


    for (int i = 0; i < len; i++) {
        if (exp[i] == '(' || exp[i] == ')') {
            continue;
        }
        else if (exp[i] == '+' || exp[i] == '-' || exp[i] == '*' || exp[i] == '/') {
            continue;
        }
        else if ('0' <= exp[i] && exp[i] <= '9') {
            continue;
        }
        else {
            printf("<<error �߻�>>\n");
            printf("in_to_po error2: ���� ���� ����\n\n");
            err = 2;
        }
    }


    int count_len = 0;
    int max_len = 0;
    for (int i = 0; i < len; i++) {
        if ('0' <= exp[i] && exp[i] <= '9') {
            count_len++;
            if (count_len >= max_len) {
                max_len = count_len;
            }
        }
        else {
            count_len = 0;
        }
    }
    if (max_len >= 2) {
        printf("<<error �߻�>>\n");
        printf("in_to_po error3: �� �ڸ��� �̻� ����\n\n");
        err = 3;
    }
}


element* in_to_po(element exp[])
{
  
    check_error(exp);//����üũ

    if (err != -1) {
        return NULL;
    }

    int i, idx = 0;
    int len = strlen(exp);
    char ch, op;
    StackType s;
    element* postfix_arr = (element*)malloc(MAX_STACK_SIZE);//������ ���ڹ迭 �����Ҵ����� ����
    if (postfix_arr == NULL) {//������ ���ڹ迭�� �޸��Ҵ��� �ȵ����� 
        fprintf(stderr, "���� �Ҵ�  ����\n");
        return;
    }

    init_stack(&s);  //���� �ʱ�ȭ


    for (int i = 0; i < len; i++)
    {

        ch = exp[i];

     
        if ('0' <= ch && ch <= '9') {
            postfix_arr[idx++] = ch;
        }


        else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {//��Ģ�����϶��� 
           
            if (!is_empty(&s) && (prec(ch) <= prec(peek(&s))))//������ �Ⱥ���ְ� �켱������ ���� ch ���� peek������ ���� ������
            {
          
                postfix_arr[idx++] = pop(&s);
                //pop(&s);
            }

            push(&s, ch);
        }


        else if (ch == '(') {
            push(&s, ch);
        }

        else if (ch == ')') {
            op = pop(&s);
            while (op != '(')
            {
                postfix_arr[idx++] = op;
                op = pop(&s);//������ �ִ� "("�� ����// ++������ ��� ��Ģ���� ��Ʈ���� �迭�� ��������
            }
        }
    }

    while (!is_empty(&s)) {//���ÿ� �����ִ°� ��� ���ϱ� (�����ڿ��� push�Ҷ� �����ִ°� )
        op = peek(&s);//
        pop(&s);
        postfix_arr[idx++] = op;
    }


    postfix_arr[idx] = '\0';//���ڹ迭�� �������� null���� �־������
    return postfix_arr;
}

int main(void)
{
    element expression[MAX_STACK_SIZE];
    char word[100];


    while (1) {
    
        printf("enter an infix Exp :");
        scanf_s("%s", expression, (int)sizeof(expression));

      
        printf("<infix to postfix>\n");
        element* result = in_to_po(expression);
        if (err == -1) {

            printf("postFix : %s\n", result);

            printf("result: : %d\n\n", eval(result));
        }


    }

    return 0;
}