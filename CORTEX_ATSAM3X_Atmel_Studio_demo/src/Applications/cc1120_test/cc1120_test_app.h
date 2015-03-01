/*
 * cc1120_test_app.h
 *
 * Created: 14-02-2015 5:22:45 PM
 *  Author: Blue
 */ 

#include <stdio.h>
#include <stdlib.h>

#ifndef CC1120_TEST_APP_H_
#define CC1120_TEST_APP_H_

void vStartcc1120_testTasks( UBaseType_t uxPriority );

int i2a(char *s, int n);

char* my_itoa(char *output_buff, int num);

#endif /* CC1120_TEST_APP_H_ */