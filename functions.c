#include "main.h"
#include <unistd.h>
#include <dirent.h>
#include <sys/sysinfo.h>
#include <sys/utsname.h>

const char* get_username() {
	return getenv("USER");
}

const char* get_hostname() {
	gethostname(buffer, BUFFER_SIZE);
	return buffer;
}

const char* get_distribution() {
	FILE* file = fopen("/etc/os-release", "r");
	if (file) {
		while (fgets(buffer, BUFFER_SIZE, file)) {
			if (strncmp(buffer, "PRETTY_NAME=", 12) == 0) {
				char* name = strchr(buffer, '=');
				if (name) {
					name += 2;
					name[strlen(name) - 2] = '\0';
					strncpy(buffer, name, BUFFER_SIZE);
					break;
				}
			}
		}
		fclose(file);
	} else {
		snprintf(buffer, BUFFER_SIZE, "Unknown");
	}
	return buffer;
}

const char* get_machine_name() {
	FILE* file = fopen("/sys/devices/virtual/dmi/id/board_name", "r");
	if (file) {
		fgets(buffer, BUFFER_SIZE, file);
		buffer[strcspn(buffer, "\n")] = '\0';
		fclose(file);
	} else {
		snprintf(buffer, BUFFER_SIZE, "Unknown");
	}
	return buffer;
}

const char* get_kernel() {
	static struct utsname kernel;
	uname(&kernel);
	return kernel.release;
}

const char* get_uptime() {
	struct sysinfo sys_info;
	sysinfo(&sys_info);

	long seconds = sys_info.uptime;
	int hours = seconds / 3600;
	int minutes = (seconds % 3600) / 60;

	snprintf(buffer, BUFFER_SIZE, "%dh, %dm", hours, minutes);
	return buffer;
}

int get_installed_packages() {
	int package_count = 0;
	DIR* dir = opendir("/var/lib/pacman/local");
	if (dir) {
		struct dirent* entry;
		while ((entry = readdir(dir)) != NULL) {
			if (entry->d_type == DT_DIR && entry->d_name[0] != '.') {
				package_count++;
			}
		}
		closedir(dir);
	}
	return package_count;
}

const char* get_memory_usage() {
	struct sysinfo info;
	sysinfo(&info);

	double total_mem = (double)(info.totalram * info.mem_unit);
	double used_mem = (double)((info.totalram - info.freeram) * info.mem_unit);

	const char* suffix = "MB";
	if (total_mem > 1 << 30) {
		total_mem /= 1 << 30;
		used_mem /= 1 << 30;
		suffix = "GB";
	} else {
		total_mem /= 1 << 20;
		used_mem /= 1 << 20;
	}

	snprintf(buffer, BUFFER_SIZE, "%.2f %s / %.2f %s", used_mem, suffix, total_mem, suffix);
	return buffer;
}
