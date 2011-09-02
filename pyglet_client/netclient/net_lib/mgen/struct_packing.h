
static inline int pack_struct_test1(void* buffer, struct test1* s, int* n) { 
	int n=0;
	PACK_uint8_t(s->testbyte, buffer, &n);
	PACK_float(s->testfloat, buffer, &n);
	*n+=sizeof(%s);
}

static inline void unpack_struct_test1(void* buffer, struct test1* s, int* n) { 
	int n=0;
	UNPACK_float(s->testfloat, buffer, &n);
	UNPACK_uint8_t(s->testbyte, buffer, &n);
	*n+=sizeof(uint8_t)
}

static inline int pack_struct_test2(void* buffer, struct test2* s, int* n) { 
	int n=0;
	PACK_int16_t(s->testshort, buffer, &n);
	PACK_int32_t(s->testint, buffer, &n);
	*n+=sizeof(%s);
}

static inline void unpack_struct_test2(void* buffer, struct test2* s, int* n) { 
	int n=0;
	UNPACK_int32_t(s->testint, buffer, &n);
	UNPACK_int16_t(s->testshort, buffer, &n);
	*n+=sizeof(int16_t)
}

static inline int pack_struct_test3(void* buffer, struct test3* s, int* n) { 
	int n=0;
	PACK_uint16_t(s->testushort, buffer, &n);
	PACK_uint32_t(s->testuint, buffer, &n);
	*n+=sizeof(%s);
}

static inline void unpack_struct_test3(void* buffer, struct test3* s, int* n) { 
	int n=0;
	UNPACK_uint32_t(s->testuint, buffer, &n);
	UNPACK_uint16_t(s->testushort, buffer, &n);
	*n+=sizeof(uint16_t)
}

static inline int pack_struct_uni(void* buffer, struct uni* s, int* n) { 
	int n=0;
	PACK_uint8_t(s->t1, buffer, &n);
	PACK_int16_t(s->t4, buffer, &n);
	PACK_uint16_t(s->t5, buffer, &n);
	PACK_int32_t(s->t2, buffer, &n);
	PACK_uint32_t(s->t3, buffer, &n);
	PACK_float(s->t6, buffer, &n);
	*n+=sizeof(%s);
}

static inline void unpack_struct_uni(void* buffer, struct uni* s, int* n) { 
	int n=0;
	UNPACK_float(s->t6, buffer, &n);
	UNPACK_uint32_t(s->t3, buffer, &n);
	UNPACK_int32_t(s->t2, buffer, &n);
	UNPACK_uint16_t(s->t5, buffer, &n);
	UNPACK_int16_t(s->t4, buffer, &n);
	UNPACK_uint8_t(s->t1, buffer, &n);
	*n+=sizeof(uint8_t)
}

