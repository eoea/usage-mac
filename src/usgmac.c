#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/um.h"

#define NOTIFY_CPU_IDLE_AT_VALUE 20 // If this is 20 it means that CPU Usage is 80 (100 - N).
#define MAX_CPU_STRING_LENGTH 15

char* um_get_cpu_idle_stats_malloc(void) {
  system(
      "top -l 1 -n 0 | grep \"idle\" | awk '{print $7}' > /tmp/cpu_usg_mac.txt");

  FILE* file = fopen("/tmp/cpu_usg_mac.txt", "r");
  if (!file) {
    return NULL;
  }

  char* buffer = (char*)malloc(MAX_CPU_STRING_LENGTH);
  if (!buffer) {
    fclose(file);
    return NULL;
  }

  if (fgets(buffer, MAX_CPU_STRING_LENGTH, file) == NULL) {
    free(buffer);
    fclose(file);
    return NULL;
  }
  fclose(file);

  return buffer;
}

um_result um_cpu_notify(void) {
  um_result result;
  result.notify = 0;
  result.value = 0.0;

  char* cpu_idle_value = um_get_cpu_idle_stats_malloc();
  if (!cpu_idle_value) {
    return result;
  }

  size_t len = strlen(cpu_idle_value);
  if (len > 0 && cpu_idle_value[len - 1] == '%') {
    cpu_idle_value[len - 1] = '\0';
  }

  result.value = atof(cpu_idle_value);
  free(cpu_idle_value);

  if (result.value <= NOTIFY_CPU_IDLE_AT_VALUE) {
    result.value = 100.0 - result.value; // (100 - idle)
    result.notify = 1;
  }

  return result;
}
