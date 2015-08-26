#ifndef QGF_TOOLS_ARRAY_H
#define QGF_TOOLS_ARRAY_H 1

#include "qgf/types.h"

void
qgf_array_new(qgf_arr* arr);

void
qgf_array_new(qgf_arr* arr, long capa);

void 
qgf_array_sort(qgf_arr* arr);

void
qgf_array_add(qgf_arr* arr, ruby_value obj);

void
qgf_array_remove(qgf_arr* arr, ruby_value obj);

void
qgf_array_gc_mark(qgf_arr* arr);

void
qgf_array_free(qgf_arr* arr);

#endif
