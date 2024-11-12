#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 128
#define DETAIL_OFFSET 1

static char buffer[BUFFER_SIZE];
static char** details;

const char* get_username();
const char* get_hostname();
const char* get_distribution();
const char* get_machine_name();
const char* get_kernel();
const char* get_uptime();
int get_installed_packages();
const char* get_memory_usage();
