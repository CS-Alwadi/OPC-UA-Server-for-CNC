#include "opcua_msgpack.h"

static bool opcua_cmp_reader(struct cmp_ctx_s *ctx, void *data, size_t limit)
{
	if(((struct msgpack_container_ctx *)ctx)->p_cmp_write + limit > (uint8_t*)ctx->buf + ((struct msgpack_container_ctx *)ctx)->buffer_size)
		return false;

	memcpy(data, ((struct msgpack_container_ctx *)ctx)->p_cmp_write, limit);
	((struct msgpack_container_ctx *)ctx)->p_cmp_write += limit;
	return true;
}

static bool opcua_cmp_skipper(struct cmp_ctx_s *ctx, size_t count)
{
	if (((struct msgpack_container_ctx *)ctx)->p_cmp_write + count >
	    (uint8_t *)ctx->buf + ((struct msgpack_container_ctx *)ctx)->buffer_size)
		return false;

	((struct msgpack_container_ctx *)ctx)->p_cmp_write += count;
	return true;
}

static size_t opcua_cmp_writer(struct cmp_ctx_s *ctx, const void *data, size_t count)
{
	if (((struct msgpack_container_ctx *)ctx)->p_cmp_write + count >
	    (uint8_t *)ctx->buf + ((struct msgpack_container_ctx *)ctx)->buffer_size)
		return false;

	memcpy(((struct msgpack_container_ctx *)ctx)->p_cmp_write, data, count);
	((struct msgpack_container_ctx *)ctx)->p_cmp_write += count;
	return true;
}

uint16_t opcua_cmp_msg_len(struct msgpack_container_ctx  *msgpack_ctx)
{
	return (msgpack_ctx)->p_cmp_write - (uint8_t*)(msgpack_ctx)->cmp.buf;
}

void opcua_cmp_rewind(struct msgpack_container_ctx  *msgpack_ctx)
{
	msgpack_ctx->p_cmp_write = (uint8_t*)msgpack_ctx->cmp.buf;
}

struct cmp_ctx_s * opcua_cmp_init(struct msgpack_container_ctx *msgpack_ctx, uint8_t *buffer, uint16_t buffer_size)
{
	cmp_init(&msgpack_ctx->cmp, buffer, opcua_cmp_reader, opcua_cmp_skipper, opcua_cmp_writer);
	msgpack_ctx->buffer_size = buffer_size;
	msgpack_ctx->p_cmp_write = buffer;
	return &msgpack_ctx->cmp;
}

int opcua_cmp_usage()
{
	uint8_t msg_buffer [50] = {0};
	char * test_str = (const char*)"testing msgpach str\0"; 
	double test_double = 36.5;
	int test_int = 100;
	uint16_t test_int_16 = 500;

	struct msgpack_container_ctx msg_container;
	struct cmp_ctx_s * msgpack_ctx = opcua_cmp_init(&msg_container,msg_buffer,sizeof(msg_buffer));
	CMP_ASSERT(cmp_write_str(msgpack_ctx,test_str,sizeof(test_str) - 1),"cmp_write_str(test_str)");
	cmp_write_double(msgpack_ctx,test_double);
	cmp_write_integer(msgpack_ctx,test_int);
	cmp_write_u8(msgpack_ctx,test_int_16);

	//@TODO сделать пример с чтением буфера
}