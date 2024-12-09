#include "main.h"

void setup_details() {
	snprintf(details[DETAIL_OFFSET + 0], BUFFER_SIZE, "%s@%s", get_username(), get_hostname());
	snprintf(details[DETAIL_OFFSET + 1], BUFFER_SIZE, "Distro - %s", get_distribution());
	snprintf(details[DETAIL_OFFSET + 2], BUFFER_SIZE, "System - %s", get_machine_name());
	snprintf(details[DETAIL_OFFSET + 3], BUFFER_SIZE, "Kernel - %s", get_kernel());
	snprintf(details[DETAIL_OFFSET + 4], BUFFER_SIZE, "Uptime - %s", get_uptime());
	snprintf(details[DETAIL_OFFSET + 5], BUFFER_SIZE, "Pacman - %d", get_installed_packages());
	snprintf(details[DETAIL_OFFSET + 6], BUFFER_SIZE, "Memory - %s", get_memory_usage());
}

int main(int argc, char *argv[]) {
	setup_details();

	for (int i = 0; logo_array[i] != NULL; i++)
		printf("%s %s\n", logo_array[i], details[i]);

	printf("\e[?12;25h\e[0m");
	return 0;
}
