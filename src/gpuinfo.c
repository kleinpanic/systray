#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gpuinfo.h"

#define GPU_BUFFER_SIZE 512

/* This function tries to call "nvidia-smi" to query the GPU utilization
 * and temperature. If successful, it returns a formatted string; otherwise,
 * it reports that GPU info is not available.
 */
char* get_gpu_info() {
    FILE *fp = popen("nvidia-smi --query-gpu=utilization.gpu,temperature.gpu --format=csv,noheader,nounits", "r");
    if (fp == NULL) {
        char *result = malloc(64);
        if (result) {
            snprintf(result, 64, "GPU: Not available");
        }
        return result;
    }

    char output[GPU_BUFFER_SIZE];
    if (fgets(output, sizeof(output), fp) != NULL) {
        /* Remove trailing newline if present */
        size_t len = strlen(output);
        if (len > 0 && output[len - 1] == '\n')
            output[len - 1] = '\0';

        char gpu_util[32] = {0}, gpu_temp[32] = {0};
        if (sscanf(output, " %31[^,], %31s", gpu_util, gpu_temp) == 2) {
            char *result = malloc(GPU_BUFFER_SIZE);
            if (result) {
                snprintf(result, GPU_BUFFER_SIZE, "GPU: NVIDIA, Utilization: %s%%, Temperature: %s°C", gpu_util, gpu_temp);
            }
            pclose(fp);
            return result;
        }
    }
    pclose(fp);
    char *result = malloc(64);
    if (result) {
        snprintf(result, 64, "GPU: No data");
    }
    return result;
}

