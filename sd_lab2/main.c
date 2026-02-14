#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
        enqueue(q, new_tok);
    }

}

void sort_station() {

}

void main() {
    char user_input[512];
    printf("Input the math expression: ");
    scanf("%s", user_input);

    token cur_tok;
    queue q_in, q_out;
    init_queue(&q_in); init_queue(&q_out);

    stack_dbl numbs; stack_str ops;
    init_stack_dbl(&numbs); init_stack_dbl(&ops);

    tokenize(&q_in, user_input);
    while (!queue_is_empty) {
        token* cur_tok = dequeue(&q_in);
        if (cur_tok->type == NUMB || cur_tok->type == PEREM) enqueue(&q_out, cur_tok);
        /*else if (cur_tok->type == OPERATOR) push_str(&ops, cur_tok->data);*/
    }
}


