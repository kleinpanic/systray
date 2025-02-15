#ifndef GPUINFO_H
#define GPUINFO_H

/* Returns a dynamically allocated string with GPU information if available.
 * Caller is responsible for free()ing the returned string.
 */
char* get_gpu_info();

#endif  /* GPUINFO_H */

