#include "main.h"

void setup_details() {
	int row = 0;
	int cnt;

#define PRINT(...) if (DETAIL_OFFSET + row < LOGO_HEIGHT) snprintf(details[DETAIL_OFFSET + row++], BUFFER_SIZE, __VA_ARGS__)

	PRINT("%s@%s", get_username(), get_hostname());
	PRINT("Distro - %s", get_distribution());
	PRINT("System - %s", get_machine_name());
	PRINT("Kernel - %s", get_kernel());
	PRINT("Uptime - %s", get_uptime());

	if ((cnt = get_installed_packages_pacman()))
		PRINT("Pacman - %d", cnt);
	if ((cnt = get_installed_packages_emerge()))
		PRINT("Emerge - %d", cnt);
	if ((cnt = get_installed_packages_dpkg()))
		PRINT("Dpkg   - %d", cnt);

	PRINT("Memory - %s", get_memory_usage());

#undef PRINT
}

int main(int argc, char *argv[]) {
	setup_details();

	for (int i = 0; logo_array[i] != NULL; i++)
		printf("%s %s\n", logo_array[i], details[i]);

	printf("\e[?12;25h\e[0m");
	return 0;
}
