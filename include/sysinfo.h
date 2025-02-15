#ifndef SYSINFO_H
#define SYSINFO_H

/* Returns a dynamically allocated string containing various system metrics.
 * Caller is responsible for free()ing the returned string.
 */
char* get_system_info();

#endif  /* SYSINFO_H */

