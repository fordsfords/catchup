/* catchup.c - program to demonstrate the "catchup" rate-setting algorithm.  See ??? */

/* This work is dedicated to the public domain under CC0 1.0 Universal:
 * http://creativecommons.org/publicdomain/zero/1.0/
 * 
 * To the extent possible under law, Steven Ford has waived all copyright
 * and related or neighboring rights to this work. In other words, you can 
 * use this code for any purpose without any restrictions.
 * This work is published from: United States.
 * Project home: https://github.com/fordsfords/catchup
 */

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/errno.h>
#include <sys/time.h>

char usage[] = "Usage: catchup send_rate [sleep_usec]";

/* Simple fatal error macro */
#define F(m) do { fprintf(stderr, "Fatal error [%s,%d]: '%s'\n", __FILE__, __LINE__, m);  exit(1);  } while (0)
/* If non-zero status, fatal error with errno string */
#define E0(s) do { char errbuf[99];  if (s != 0) {  strerror_r(errno, errbuf, 99);  F(errbuf);  }  } while (0)


long long gtod64()
{
  int e;  /* error handling */
  struct timeval cur_tv;

  e=gettimeofday(&cur_tv, NULL);  E0(e);

  return (long long)cur_tv.tv_sec * 1000000ll + (long long)cur_tv.tv_usec;
}  /* gtod64 */


int main(int argc, char **argv)
{
  int e;  /* error handling */

  /* Command-line parsing */
  if (argc == 1) { printf("%s\n", usage); exit(0); }
  long send_rate = 0;  if (argc >= 2) send_rate = atoi(argv[1]);
  long sleep_usec = 0;  if (argc >= 3) sleep_usec = atoi(argv[2]);

  long long start_usec = gtod64();
  long tot_num_sent = 0;

  for (;;) {
    long long usec_running = gtod64() - start_usec;
	long should_have_sent = (long)((usec_running * (long long)send_rate) / 1000000ll);

    /* how far behind are we? */
    long burst = should_have_sent - tot_num_sent;  if (burst > send_rate) F("More than 1 second behind");

    /* send a burst of messages to catch up. */
    int i;
    for (i = 0; i < burst; i++) {
      printf(".");  fflush(stdout);  /* send message */
    }  /* for i */
    tot_num_sent += burst;

    if (sleep_usec > 0) {
      e=usleep(sleep_usec);  E0(e);
    }
  }  /* for ;; */
}  /* main */
