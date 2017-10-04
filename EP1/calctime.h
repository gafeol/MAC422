#ifndef CALCTIME_H
#define CALCTIME_H

#include <stdio.h>
#include <sys/time.h>

typedef struct timeval timev;
timev start_time;
static FILE *out;

double sec(timev);

double running_time();

#endif
