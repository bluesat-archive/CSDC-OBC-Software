/*
 * cc1120_test_app.h
 *
 * Created: 14-02-2015 5:22:45 PM
 *  Author: Blue
 */ 

#include <stdio.h>
#include <stdlib.h>

#ifndef M25P32_TEST_APP_H_
#define M25P32_TEST_APP_H_

void vStartm25p32_testTasks( UBaseType_t uxPriority );

// test functions
void m25p32_read_write_test();

#endif /* M25P32_TEST_APP_H_ */