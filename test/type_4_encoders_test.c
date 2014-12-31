#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include "cbor.h"
#include <inttypes.h>


unsigned char buffer[512];

static void test_embedded_array_start(void **state) {
	assert_int_equal(1, cbor_encode_array_start(1, buffer, 512));
	assert_memory_equal(buffer, ((unsigned char[]){ 0x81 }), 1);
}

static void test_array_start(void **state) {
	assert_int_equal(5, cbor_encode_array_start(1000000, buffer, 512));
	assert_memory_equal(buffer, ((unsigned char[]){ 0x9A, 0x00, 0x0F, 0x42, 0x40 }), 5);
}

static void test_indef_array_start(void **state) {
	assert_int_equal(1, cbor_encode_indef_array_start(buffer, 512));
	assert_int_equal(0, cbor_encode_indef_array_start(buffer, 0));
	assert_memory_equal(buffer, ((unsigned char[]){ 0x9F }), 1);
}

static void test_indef_array_encoding(void **state) {
	cbor_item_t * array = cbor_new_indefinite_array();
	cbor_item_t * one = cbor_build_uint8(1);
	cbor_item_t * two = cbor_build_uint8(2);
	cbor_array_push(array, one);
	cbor_array_push(array, two);
	assert_int_equal(3, cbor_serialize_array(array, buffer, 512));
	assert_memory_equal(buffer, ((unsigned char[]){ 0x9F, 0x01, 0x02 }), 3);
}

int main(void) {
	const UnitTest tests[] = {
		unit_test(test_embedded_array_start),
		unit_test(test_array_start),
		unit_test(test_indef_array_start),
		unit_test(test_indef_array_encoding)
	};
	return run_tests(tests);
}
