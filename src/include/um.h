#ifndef UM_H
#define UM_H

typedef struct {
  int notify;
  double value;
} um_result;

char* um_get_cpu_stats_malloc(void);
um_result um_cpu_notify(void);

#endif  // UM_H
