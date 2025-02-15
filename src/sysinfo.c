#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/statvfs.h>
#include "sysinfo.h"

#define BUFFER_SIZE 1024

/* Helper: Get uptime from /proc/uptime */
static void get_uptime(char *buffer, size_t size) {
    FILE *fp = fopen("/proc/uptime", "r");
    if (fp) {
        double uptime_seconds;
        if (fscanf(fp, "%lf", &uptime_seconds) == 1) {
            int days = uptime_seconds / (24 * 3600);
            uptime_seconds -= days * (24 * 3600);
            int hours = uptime_seconds / 3600;
            uptime_seconds -= hours * 3600;
            int minutes = uptime_seconds / 60;
            int seconds = (int)uptime_seconds % 60;
            snprintf(buffer, size, "Uptime: %d days, %d hours, %d minutes, %d seconds", days, hours, minutes, seconds);
        } else {
            snprintf(buffer, size, "Uptime: N/A");
        }
        fclose(fp);
    } else {
        snprintf(buffer, size, "Uptime: N/A");
    }
}

/* Helper: Get load average from /proc/loadavg */
static void get_load_average(char *buffer, size_t size) {
    FILE *fp = fopen("/proc/loadavg", "r");
    if (fp) {
        double load1, load5, load15;
        if (fscanf(fp, "%lf %lf %lf", &load1, &load5, &load15) == 3) {
            snprintf(buffer, size, "Load Average: %.2f, %.2f, %.2f", load1, load5, load15);
        } else {
            snprintf(buffer, size, "Load Average: N/A");
        }
        fclose(fp);
    } else {
        snprintf(buffer, size, "Load Average: N/A");
    }
}

/* Helper: Get memory usage from /proc/meminfo */
static void get_memory_usage(char *buffer, size_t size) {
    FILE *fp = fopen("/proc/meminfo", "r");
    if (fp) {
        char line[256];
        long memTotal = 0, memFree = 0, buffers = 0, cached = 0;
        while (fgets(line, sizeof(line), fp)) {
            if (sscanf(line, "MemTotal: %ld kB", &memTotal) == 1)
                ;
            else if (sscanf(line, "MemFree: %ld kB", &memFree) == 1)
                ;
            else if (sscanf(line, "Buffers: %ld kB", &buffers) == 1)
                ;
            else if (sscanf(line, "Cached: %ld kB", &cached) == 1)
                ;
        }
        fclose(fp);
        long used = memTotal - memFree - buffers - cached;
        double used_percent = (double)used / memTotal * 100;
        snprintf(buffer, size, "Memory: %ld kB used / %ld kB total (%.1f%%)", used, memTotal, used_percent);
    } else {
        snprintf(buffer, size, "Memory: N/A");
    }
}

/* Helper: Get CPU information from /proc/stat (basic numbers) */
static void get_cpu_info(char *buffer, size_t size) {
    FILE *fp = fopen("/proc/stat", "r");
    if (fp) {
        char line[256];
        if (fgets(line, sizeof(line), fp)) {
            /* The first line begins with "cpu" */
            unsigned long user, nice, system, idle;
            int count = sscanf(line, "cpu  %lu %lu %lu %lu", &user, &nice, &system, &idle);
            if (count == 4) {
                snprintf(buffer, size, "CPU: user=%lu, nice=%lu, system=%lu, idle=%lu", user, nice, system, idle);
            } else {
                snprintf(buffer, size, "CPU: N/A");
            }
        } else {
            snprintf(buffer, size, "CPU: N/A");
        }
        fclose(fp);
    } else {
        snprintf(buffer, size, "CPU: N/A");
    }
}

/* Helper: Get disk usage for the root partition (/) */
static void get_disk_usage(char *buffer, size_t size) {
    struct statvfs stat;
    if (statvfs("/", &stat) == 0) {
        unsigned long total = stat.f_blocks * stat.f_frsize;
        unsigned long available = stat.f_bavail * stat.f_frsize;
        unsigned long used = total - available;
        double used_percent = (double)used / total * 100;
        snprintf(buffer, size, "Disk (/): %lu MB used / %lu MB total (%.1f%%)", used / (1024 * 1024), total / (1024 * 1024), used_percent);
    } else {
        snprintf(buffer, size, "Disk: N/A");
    }
}

/* Helper: Get cumulative network usage from /proc/net/dev */
static void get_network_usage(char *buffer, size_t size) {
    FILE *fp = fopen("/proc/net/dev", "r");
    if (fp) {
        char line[256];
        unsigned long rx_total = 0, tx_total = 0;
        /* Skip the first two header lines */
        fgets(line, sizeof(line), fp);
        fgets(line, sizeof(line), fp);
        while (fgets(line, sizeof(line), fp)) {
            char iface[64];
            unsigned long rx_bytes, tx_bytes;
            if (sscanf(line, " %[^:]: %lu %*s %*s %*s %*s %*s %*s %*s %lu", iface, &rx_bytes, &tx_bytes) == 3) {
                rx_total += rx_bytes;
                tx_total += tx_bytes;
            }
        }
        fclose(fp);
        snprintf(buffer, size, "Network: %lu bytes received, %lu bytes transmitted", rx_total, tx_total);
    } else {
        snprintf(buffer, size, "Network: N/A");
    }
}

/* Combines the above metrics into one output string.
 * Returns a dynamically allocated string which must be freed by the caller.
 */
char* get_system_info() {
    char uptime[128];
    char load[128];
    char memory[128];
    char cpu[128];
    char disk[128];
    char network[128];

    get_uptime(uptime, sizeof(uptime));
    get_load_average(load, sizeof(load));
    get_memory_usage(memory, sizeof(memory));
    get_cpu_info(cpu, sizeof(cpu));
    get_disk_usage(disk, sizeof(disk));
    get_network_usage(network, sizeof(network));

    /* Allocate a result buffer (adjust size as needed) */
    char *result = malloc(BUFFER_SIZE * 2);
    if (result) {
        snprintf(result, BUFFER_SIZE * 2, "%s\n%s\n%s\n%s\n%s\n%s",
                 uptime, load, memory, cpu, disk, network);
    }
    return result;
}

