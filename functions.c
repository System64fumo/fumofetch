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
				char* name = buffer + 12;
				name[strcspn(name, "\n")] = 0;
				memmove(name, name + 1, strlen(name));
				name[strlen(name) - 1] = '\0';
				fclose(file);
				return strdup(name);
			}
		}
	}

	fclose(file);
	return "Unknown";
}

const char* get_machine_name() {
	FILE* file_arm = fopen("/sys/firmware/devicetree/base/model", "r");
	FILE* file_x86 = fopen("/sys/devices/virtual/dmi/id/board_name", "r");

	if (file_arm) {
		fgets(buffer, BUFFER_SIZE, file_arm);
		fclose(file_arm);
	}
	else if (file_x86) {
		fgets(buffer, BUFFER_SIZE, file_x86);
		buffer[strcspn(buffer, "\n")] = '\0';
		fclose(file_x86);
	}
	else {
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
	static char buffer[BUFFER_SIZE];
	FILE* meminfo = fopen("/proc/meminfo", "r");

	unsigned long total_mem = 0, free_mem = 0, buffers = 0, cached = 0, sreclaimable = 0, shmem = 0;
	char label[32]; unsigned long value;

	while (fscanf(meminfo, "%31s %lu kB", label, &value) == 2) {
		if (!strcmp(label, "MemTotal:")) total_mem = value;
		else if (!strcmp(label, "MemFree:")) free_mem = value;
		else if (!strcmp(label, "Buffers:")) buffers = value;
		else if (!strcmp(label, "Cached:")) cached = value;
		else if (!strcmp(label, "SReclaimable:")) sreclaimable = value;
		else if (!strcmp(label, "Shmem:")) shmem = value;
	}
	fclose(meminfo);

	unsigned long used_mem = total_mem - free_mem - buffers - cached + shmem - sreclaimable;

	const char* suffix = "MB";
	double display_total = total_mem / 1024.0, display_used = used_mem / 1024.0;
	if (display_total > 1024) { display_total /= 1024.0; display_used /= 1024.0; suffix = "GB"; }

	snprintf(buffer, BUFFER_SIZE, "%.2f %s / %.2f %s", display_used, suffix, display_total, suffix);
	return buffer;
}

