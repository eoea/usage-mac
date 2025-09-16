#ifndef SRC_INCLUDE_UM_H_
#define SRC_INCLUDE_UM_H_

typedef struct {
  int notify;
  double value;
} um_result;

char* um_get_cpu_stats_malloc(void);
um_result um_cpu_notify(void);

#endif  // SRC_INCLUDE_UM_H_
