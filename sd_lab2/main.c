#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct stack_numb {
	double val;
	struct stack_numb* top;
} stack_numb;


stack_numb* push_n(stack_numb *top, double val) {
	stack_numb* next_stack = (stack_numb*)malloc(sizeof(stack_numb));
	next_stack->val = val;
	next_stack->top = top;
	return next_stack;
}

double pop_n(stack_numb** top) {
	if (*top == NULL) {
		exit(0);//какая-то ошибка
	}
	stack_numb *tmp = *top;
	double val = (*top)->val;
	*top = (*top)->top;
	free(tmp);
	return val;
}

int stack_empty(stack_numb stack) {
	if (stack.top == NULL) return 0;
}

void main() {
	stack_numb *num_stack = NULL;
	num_stack = push_n(num_stack, 10);
	num_stack = push_n(num_stack, 15);
	pop_n(&num_stack);

}