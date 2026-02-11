#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct stack_numb {
	double val;
	stack_numb* next;
} stack_numb;

typedef struct stack_str {
	char* val;
	stack_str* next;
}stack_str;

stack_numb* push_n(stack_numb *top, double val) {
	stack_numb* next_stack = (stack_numb*)malloc(sizeof(stack_numb));
	next_stack->val = val;
	next_stack->next = top;
	return next_stack;
}

stack_str* push_s(stack_str* top, char* val) {
	stack_str* next_stack = (stack_str*)malloc(sizeof(stack_str));
	next_stack->val = (char*)malloc(strlen(val) + 1);
	strcpy(next_stack->val, val);
	next_stack->next = top;
	return next_stack;
}

double pop_n(stack_numb** top) {
	if (*top == NULL) {
		exit(0);//какая-то ошибка
	}
	stack_numb *tmp = *top;
	double val = (*top)->val;
	*top = (*top)->next;
	free(tmp);
	return val;
}

char* pop_s(stack_str** top) {
	if (*top == NULL) exit(0);
	char* val = (*top)->val;
	stack_str* tmp = *top;
	*top = (*top)->next;
	free(tmp);
	return val;
}

int stack_n_empty(stack_numb* top) {
	return top == NULL;
}

int stack_s_empty(stack_str* top) {
	return top == NULL;
}

void main() {
	stack_numb *num_stack = NULL;
	num_stack = push_n(num_stack, 10);
	num_stack = push_n(num_stack, 15);
	double res = pop_n(&num_stack);

}