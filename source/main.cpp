/**
 * @file main.cpp
 * @author Al-Wadi (omar.alwadi@outlook.com)
 * @brief This is main file
 * @version 0.1
 * @date 2020-01-02
 * 
 * @copyright Copyright (c) 2020
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <Windows.h>
#include "cmp/cmp.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
using namespace std;

static bool read_bytes(void *data, size_t sz, FILE *fh) {
    return fread(data, sizeof(uint8_t), sz, fh) == (sz * sizeof(uint8_t));
}

static bool file_reader(cmp_ctx_t *ctx, void *data, size_t limit) {
    return read_bytes(data, limit, (FILE *)ctx->buf);
}

static bool file_skipper(cmp_ctx_t *ctx, size_t count) {
    return fseek((FILE *)ctx->buf, count, SEEK_CUR);
}

static size_t file_writer(cmp_ctx_t *ctx, const void *data, size_t count) {
    return fwrite(data, sizeof(uint8_t), count, (FILE *)ctx->buf);
}

void error_and_exit(const char *msg) {
    fprintf(stderr, "%s\n\n", msg);
    exit(EXIT_FAILURE);
}

int main(void) {
    spdlog::info("Welcome to spdlog!");
    spdlog::error("Some error message with arg: {}", 1);
    cout << "this is OPC UA Server" << endl;
    FILE *fh = NULL;
    cmp_ctx_t cmp;
    uint32_t array_size = 0;
    uint32_t str_size = 0;
    char hello[6] = {0, 0, 0, 0, 0, 0};
    char message_pack[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    fh = fopen("cmp_data.dat", "w+b");

    if (fh == NULL)
        error_and_exit("Error opening data.dat");

    cmp_init(&cmp, fh, file_reader, file_skipper, file_writer);

    if (!cmp_write_array(&cmp, 2))
        error_and_exit(cmp_strerror(&cmp));

    if (!cmp_write_str(&cmp, "Hello", 5))
        error_and_exit(cmp_strerror(&cmp));

    if (!cmp_write_str(&cmp, "MessagePack", 11))
        error_and_exit(cmp_strerror(&cmp));

    rewind(fh);

    if (!cmp_read_array(&cmp, &array_size))
        error_and_exit(cmp_strerror(&cmp));

    if (!cmp_read_str_size(&cmp, &str_size))
        error_and_exit(cmp_strerror(&cmp));

    if (str_size > (sizeof(hello) - 1))
        error_and_exit("Packed 'hello' length too long\n");

    if (!read_bytes(hello, str_size, fh))
        error_and_exit(cmp_strerror(&cmp));

    

    str_size = sizeof(message_pack);
    if (!cmp_read_str(&cmp, message_pack, &str_size))
        error_and_exit(cmp_strerror(&cmp));
    
    printf("Array Length: %u.\n", array_size);
    printf("[\"%s\", \"%s\"]\n", hello, message_pack);

    fclose(fh);
    return EXIT_SUCCESS;
}