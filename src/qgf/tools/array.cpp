#include "qgf/tools/array.h"
#include "qgf/tools/func.h"
#include <stdlib.h>

void
qgf_array_new(qgf_arr* arr) {
	arr = ALLOC(qgf_arr);
	arr->capa = 1;
	arr->size = 0;
	arr->sorted = TRUE;
	arr->values = (ruby_value*)malloc(sizeof(ruby_value));
}

void
qgf_array_new(qgf_arr* arr, long capa) {
	if (capa < 1) capa = 1;
	arr = ALLOC(qgf_arr);
	arr->capa = capa;
	arr->size = 0;
	arr->sorted = TRUE;
	arr->values = (ruby_value*)malloc(sizeof(ruby_value) * capa);
}

int
qgf_array_compare(const ruby_value* a, const ruby_value* b) {
	TZObject* az;
	TZObject* bz;
	Data_Get_Struct((*a), TZObject, az);
	Data_Get_Struct((*b), TZObject, bz);
	return az->z > bz->z ? 1 : (bz->z > az->z ? -1 : 0);
}

typedef int (*qsort_func_ptr_type)(const void* a, const void* b);

void 
qgf_array_sort(qgf_arr* arr) {
	if (arr->sorted) return;
	if (arr->size == 0) return;
	qsort(arr->values, arr->size - 1, sizeof(ruby_value), (qsort_func_ptr_type)qgf_array_compare);
	arr->sorted = TRUE;
}

void
qgf_array_add(qgf_arr* arr, ruby_value obj) {
	if (!arr) return;
	if (arr->capa == 0) {
		arr->size = 0;
		arr->capa = 1;
		arr->sorted = TRUE;
		arr->values = ALLOC(ruby_value);
	} else if (arr->capa <= arr->size) {
		arr->capa = arr->size * 2;
		ruby_value* temp = (ruby_value*)realloc(&(arr->values[0]), sizeof(ruby_value) * arr->capa);
		if (temp) {
			arr->values = temp;
		}
	}
	arr->values[arr->size] = obj;
	arr->size = arr->size + 1;
	arr->sorted = FALSE;
}

void
qgf_array_remove(qgf_arr* arr, ruby_value obj) {
	for (int i = 0; i < arr->size; i++) {
		if (obj == arr->values[i]) {
			for (int j = i; j < arr->size - 1; j++) {
				arr->values[j] = arr->values[j + 1];
				arr->values[j + 1] = Qnil;
			}
			arr->size = arr->size - 1;
		}
	}
}

void
qgf_array_gc_mark(qgf_arr* arr) {
	for (int i = 0; i < arr->size; i++) {
		rb_gc_mark(arr->values[i]);
	}
}

void
qgf_array_free(qgf_arr* arr) {
	free(arr->values);
}
