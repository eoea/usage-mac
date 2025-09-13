#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/um.h"

#define NOTIFY_CPU_AT_VALUE 1  // TODO(eoea): value should be at 80 after test.
#define MAX_CPU_STRING_LENGTH 256

char* um_get_cpu(void) {
  system(
      "top -l 1 -n 0 | grep \"idle\" | awk '{print $7}' >/tmp/cpu_usg_mac.txt");

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
  um_result result = {0, 0.0};

  char* cpu_value = um_get_cpu();
  if (!cpu_value) {
    return result;
  }

  size_t len = strlen(cpu_value);
  if (len > 0 && cpu_value[len - 1] == '%') {
    cpu_value[len - 1] = '\0';
  }

  double value = atof(cpu_value);
  free(cpu_value);

  result.value = 100 - ceil(value);
  if (value >= NOTIFY_CPU_AT_VALUE) {
    result.notify = 1;
  }
  return result;
}
