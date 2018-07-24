/* Picol use example: custom command. */

#define PICOL_CONFIGURATION
#define PICOL_MAX_STR   4096
#define PICOL_MAX_LEVEL 10
/* A minimal Picol configuration: no arrays, no I/O, no [interp] or [glob]
command. */
#define PICOL_FEATURE_ARRAYS  0
#define PICOL_FEATURE_GLOB    0
#define PICOL_FEATURE_INTERP  0
#define PICOL_FEATURE_IO      0
#define PICOL_FEATURE_PUTS    1

#define PICOL_IMPLEMENTATION
#include "picol.h"

COMMAND(square) {
    int n;
    ARITY2(argc == 2, "square number");
    SCAN_INT(n, argv[1]);
    picolSetIntResult(interp, n * n);
    return PICOL_OK;
}

void report_error(picolInterp* interp, int rc) {
    if (rc != PICOL_OK) {
        printf("[%d] %s\n", rc, interp->result);
    }
}

#include <sys/time.h>
#include <time.h>

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

// thanks @jbenet -- https://gist.github.com/jbenet/1087739
void current_utc_time(struct timespec* ts)
{
#ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time
  clock_serv_t cclock;
  mach_timespec_t mts;
  host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
  clock_get_time(cclock, &mts);
  mach_port_deallocate(mach_task_self(), cclock);
  ts->tv_sec = mts.tv_sec;
  ts->tv_nsec = mts.tv_nsec;
#else
  clock_gettime(CLOCK_REALTIME, ts);
#endif
}

int main(int argc, char** argv) {
    struct timespec ts1, ts2, ts3, ts4;

    int rc = 0;
    /* Create an interpreter with no core commands. Do not call srand(). */
    picolInterp* interp = picolCreateInterp2(0, 0);
    /* Manually register only one built-in. */
    picolRegisterCmd(interp, "puts", picol_puts, NULL);
    /* Register our custom command. */
    picolRegisterCmd(interp, "square", picol_square, NULL);

    rc = picolEval(interp, "puts [square]"); /* Wrong usage. */
    report_error(interp, rc);
    rc = picolEval(interp, "puts [square foo]"); /* Wrong usage. */
    report_error(interp, rc);
    rc = picolEval(interp, "puts [square 5]"); /* Correct usage. */
    report_error(interp, rc);

    float avg = 1.0;

    for (int i = 0; i < 10000; i++) {
      
      current_utc_time(&ts1);
        rc = picolEval(interp, "[square 5]"); /* Correct usage. */
      current_utc_time(&ts2);
        /* report_error(interp, rc); */

      avg *= (ts2.tv_nsec - ts1.tv_nsec) / 1000.0 / 2.0;
    }

    printf("\t\t total: %5.2f us \n",
           (ts2.tv_nsec - ts1.tv_nsec) / 1000.0);


    return rc & 0xFF;
}
