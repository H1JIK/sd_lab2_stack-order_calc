#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#define MAX_BUF_SIZE 512 
#define MAX_V_TABLE 128


typedef struct {
    char* name;
    double val;
}pre_table;

pre_table p_t[MAX_V_TABLE];
int table_cnt = 0;

enum token_type {
    NUMB, OPERATOR, FUNC, PEREM, LSKOB, RSKOB
};

//DOUBLE
typedef struct node_dbl {
    double data;
    struct node_dbl* next;
} node_dbl;

typedef struct stack_dbl {
    struct node_dbl* top;
}stack_dbl;

//STR
typedef struct node_str {
    char* data;
    struct node_str* next;
} node_str;

typedef struct stack_str {
    struct node_str* top;
}stack_str;

//oper i td

typedef struct token {
    char* data;
    int type;
    struct token* next;
}token;

typedef struct {
    token* head;
    token* tail;
} queue;


void init_stack_dbl(stack_dbl* st) {
    st->top = NULL;
}

void init_stack_str(stack_str* st) {
    st->top = NULL;
}

void init_queue(queue* q) {
    q->head = NULL;
    q->tail = NULL;
}

void push_dbl(stack_dbl* st, double data) {
    node_dbl* new_node = (node_dbl*)malloc(sizeof(node_dbl));
    new_node->data = data;
    new_node->next = st->top;
    st->top = new_node;
}

void push_str(stack_str* st, char* data) {
    node_str* new_node = (node_str*)malloc(sizeof(node_str));
    new_node->data = (char*)malloc(sizeof(char) * strlen(data) + 1);   //+1 для нуля
    strcpy(new_node->data, data);
    new_node->next = st->top;
    st->top = new_node;
}

void enqueue(queue* q, token* tok) {
    if (q->head == NULL) {
        q->head = tok;
        q->tail = tok;
    }
    else {
        q->tail->next = tok;
        q->tail = tok;
    }
}

token* dequeue(queue* q) {
    token* del_tok = q->head;
    q->head = del_tok->next;
    if (q->head == NULL) q->tail = NULL;
    return del_tok;
}

double pop_dbl(stack_dbl* st) {
    node_dbl* cur_top = st->top;
    double returnable;
    //if (cur_top == NULL) return -1;
    st->top = cur_top->next;
    returnable = cur_top->data;
    free(cur_top);
    return returnable;
}

char* pop_str(stack_str* st) {
    node_str* cur_top = st->top;
    char* returnable;
    st->top = cur_top->next;
    returnable = cur_top->data;
    free(cur_top);
    return returnable;
}

int stack_dbl_empty(stack_dbl* st) {
    return (st->top == NULL) ? 1 : 0;
}

int stack_str_empty(stack_str* st) {
    return (st->top == NULL) ? 1 : 0;
}

int queue_is_empty(queue* q) {
    return (q->head == NULL) ? 1 : 0;
}

void tokenize(queue* q, char* str) {
    int i = 0;
    int start, str_ln;
    while (str[i] != '\0') {
        token* new_tok = (token*)malloc(sizeof(token));
        new_tok->type = -1;
        if (isdigit(str[i])) {
            start = i;
            while (isdigit(str[i])) i++;
            new_tok->type = NUMB;
        }
        else if (isalpha(str[i])) {
            start = i;
            while (isalpha(str[i])) i++;
        }
        else {
            start = i;
            if (str[i] == '(') new_tok->type = LSKOB;
            else if (str[i] == ')') new_tok->type = RSKOB;
            else new_tok->type = OPERATOR;
            i++;
        }
        str_ln = i - start;
        char* new_str = (char*)malloc(sizeof(char) * (str_ln)+1);
        for (int k = 0; k < str_ln; k++) new_str[k] = str[k + start];
        new_str[str_ln] = '\0';
        new_tok->data = new_str;
        if (new_tok->type == -1) {
            if (strcmp(new_str, "sin") == 0 || strcmp(new_str, "cos") == 0 || strcmp(new_str, "tg") == 0 || strcmp(new_str, "ctg") == 0 || strcmp(new_str, "arcsin") == 0 || strcmp(new_str, "arccos") == 0 || strcmp(new_str, "arctg") == 0 || strcmp(new_str, "arcctg") == 0 || strcmp(new_str, "sqrt") == 0)
                new_tok->type = FUNC;
            else new_tok->type = PEREM;
        }
        new_tok->next = NULL;
        enqueue(q, new_tok);
    }

}

int priority(token* tok) {
    int pri;
    if (tok->type == FUNC)pri = 4;
    else if (tok->type == LSKOB) pri = 5;
    else {
        switch (tok->data[0]) {
        case '+':
        case '-':
            pri = 0;
            break;
        case '*':
        case '/':
            pri = 1;
            break;
        case '^':
            pri = 2;
            break;
        case '!':
            pri = 3;
            break;
        default:
            pri = -1;
            break;
        }
    }
    return pri;
}

int is_stepen(char* s) {
    return (s[0] == '^');
}

int str_type(char* s) {
    if (strcmp(s, "sin") == 0 || strcmp(s, "cos") == 0 || strcmp(s, "tg") == 0 || strcmp(s, "ctg") == 0 || strcmp(s, "arcsin") == 0 || strcmp(s, "arccos") == 0 || strcmp(s, "arctg") == 0 || strcmp(s, "arcctg") == 0 || strcmp(s, "sqrt") == 0) {
        return FUNC;
    }
    return OPERATOR;
}

double get_perem(token* tok) {
    double user_inpt;
    for (int i = 0; i < table_cnt; i++) {
        if (strcmp(tok->data, p_t[i].name) == 0) {
            return p_t[i].val;
        }
    }
    printf("Enter the %s variable:", tok->data);
    scanf("%lf", &user_inpt);
    p_t[table_cnt].name = malloc(sizeof(char) * strlen(tok->data) + 1);
    strcpy(p_t[table_cnt].name, tok->data);
    p_t[table_cnt].val = user_inpt;
    table_cnt++;
    return user_inpt;
}

double calс_func(token* func, double dbl_arg) {
    //double dbl_arg = atof(arg->data);
    double res;
    if (!strcmp("sin", func->data)) res = sin(dbl_arg);
    else if (!strcmp("cos", func->data)) res = cos(dbl_arg);
    else if (!strcmp("tg", func->data)) res = tan(dbl_arg);
    else if (!strcmp("ctg", func->data)) { if (dbl_arg == 0) { printf("ERROR (divisor = 0)"); exit(0); }res = 1 / tan(dbl_arg); }
    else if (!strcmp("arcsin", func->data)) res = asin(dbl_arg);
    else if (!strcmp("arccos", func->data)) res = acos(dbl_arg);
    else if (!strcmp("arctg", func->data)) res = atan(dbl_arg);
    else if (!strcmp("arcctg", func->data)) { if (dbl_arg == 0) { printf("ERROR (divisor = 0)"); exit(0); }res = 1 / atan(dbl_arg); }
    else if (!strcmp("sqrt", func->data)) {
    if (dbl_arg < 0) {
            printf("SQRT ERROR < 0"); exit(0);
        }
        res = sqrt(dbl_arg);
    }
    return res;
}

double calc_ops(token* func, double arg1, double arg2) {
    double res;
    switch (func->data[0]) {
    case '+':
        res = arg2 + arg1;
        break;
    case '-':
        res = arg2 - arg1;
        break;
    case '*':
        res = arg2 * arg1;
        break;
    case '/':
        res = arg2 / arg1;
        break;
    case '^':
        res = pow(arg2,arg1);
        break;
    }
    return res;
}

void prepapre_and_calc(queue* q, stack_dbl* s) {
    token* cur_tok;
    double cur_numb;
    while (!queue_is_empty(q)) {
        cur_tok = dequeue(q);
        if (cur_tok->type == NUMB) {
            cur_numb = atof(cur_tok->data);
        }
        else if (cur_tok->type == PEREM) {
            cur_numb = get_perem(cur_tok);
        }
        else if (cur_tok->type == FUNC) {
            cur_numb = calс_func(cur_tok, pop_dbl(s));
        }
        else if (cur_tok->type == OPERATOR) {
            if (cur_tok->data[0] == '!') {
                cur_numb = tgamma(pop_dbl(s) + 1);  //fact
            }
            else {
                double arg1 = pop_dbl(s);
                double arg2 = pop_dbl(s);
                cur_numb = calc_ops(cur_tok, arg1, arg2);
            }
        }
        push_dbl(s, cur_numb);
        free(cur_tok->data);
        free(cur_tok);
    }
}

void print_result(stack_dbl* s) {
    printf("Result: %lf", s->top->data);
}

void main() {
    char user_input[MAX_BUF_SIZE];
    printf("Input the math expression: ");
    scanf("%s", user_input);

    queue q_in, q_out;
    init_queue(&q_in); init_queue(&q_out);

    stack_dbl numbs; stack_str ops;
    init_stack_dbl(&numbs); init_stack_str(&ops);

    tokenize(&q_in, user_input);
    while (!queue_is_empty(&q_in)) {
        token* cur_tok = dequeue(&q_in);
        if (cur_tok->type == NUMB || cur_tok->type == PEREM) enqueue(&q_out, cur_tok);  //число и перем добавляем в вых очередь
        else if (cur_tok->type == FUNC) {
            push_str(&ops, cur_tok->data); free(cur_tok);       //функции пушим в стек
        }
        else if (cur_tok->type == OPERATOR) {
            if (ops.top == NULL) {
                push_str(&ops, cur_tok->data); free(cur_tok);
            }
            else {
                while (!stack_str_empty(&ops)) {
                    token* new_token = (token*)malloc(sizeof(token));
                    new_token->type = str_type(ops.top->data);
                    new_token->data = ops.top->data;
                    new_token->next = NULL;
                    
                    if ((is_stepen(cur_tok->data) && priority(cur_tok) < priority(new_token)) || (!is_stepen(cur_tok->data) && priority(cur_tok) <= priority(new_token))) {
                        new_token->data = pop_str(&ops);
                        enqueue(&q_out, new_token);
                    }
                    else{
                        free(new_token);
                        break;
                    }
                }
                push_str(&ops, cur_tok->data); free(cur_tok);
            }
        }
        else if (cur_tok->type == LSKOB) {
            push_str(&ops, cur_tok->data); free(cur_tok);
        }
        else if (cur_tok->type == RSKOB) {
            while (ops.top->data[0] != '(') {
                token* new_token = (token*)malloc(sizeof(token));
                new_token->data = pop_str(&ops);
                new_token->type = str_type(new_token->data);
                new_token->next = NULL;
                enqueue(&q_out, new_token);
            }
            pop_str(&ops);
            free(cur_tok);
        }
    }
    while (!stack_str_empty(&ops)) {
        token* new_token = (token*)malloc(sizeof(token));
        new_token->data = pop_str(&ops);
        new_token->type = str_type(new_token->data);
        new_token->next = NULL;
        enqueue(&q_out, new_token);
        //enqueue(&q_out, pop_str(&ops));
    }
    prepapre_and_calc(&q_out, &numbs);
    print_result(&numbs);

    //очистка таблицы переменных
    for (int i = 0; i < table_cnt; i++) {
        free(p_t[i].name);
    }
}


