#include <stdio.h>
#include <stdlib.h>
#include "process.h"

int print_error, context_change, readline, printline, lost_deadline;

void print_trace(Process);

void print_cpu_usage(Process, int);

void print_cpu_liberation(Process, int);

void print_output(Process);

void print_context_change();
