#include <minix/drivers.h>

#include "test3.h"
#include "macros.h"

static int proc_args(int argc, char *argv[]);
static unsigned long parse_ulong(char *str, int base);
static long parse_long(char *str, int base);
static void print_usage(char *argv[]);

int main(int argc, char **argv) {

  /* Initialize service */
  
  sef_startup();


  if ( argc == 1 ) {
      print_usage(argv);
      return 0;
  } else {   
      proc_args(argc, argv);
  }
  return 0;

}

static void print_usage(char *argv[]) {
  printf("Usage: one of the following:\n"
	 "\t service run %s -args \"test_scan <flag>\" \n",
	 argv[0]);
}

static int proc_args(int argc, char *argv[]) {

  unsigned long C;
  char *str;
  long num;

  /* check the function to test: if the first characters match, accept it */
  if (strncmp(argv[1], "test_scan", strlen("test_scan")) == 0) {
	  if( argc != 3 ) {
		  printf("kbd: wrong no of arguments for test of timer_test_config \n");
		  return 1;
	  }
	  if( (C = parse_ulong(argv[2], 10)) == ULONG_MAX )
		  return 1;
	  printf("kbd:: kbd_test_scan(%lu)\n",
			  (unsigned)C);
	  kbd_test_scan(C);
	  return 0;
  }
  else {
	  printf("kbd: non valid function \"%s\" to test\n", argv[1]);
	  return 1;
  }
}

static unsigned long parse_ulong(char *str, int base) {
  char *endptr;
  unsigned long val;

  val = strtoul(str, &endptr, base);

  if ((errno == ERANGE && val == ULONG_MAX )
	  || (errno != 0 && val == 0)) {
	  perror("strtol");
	  return ULONG_MAX;
  }

  if (endptr == str) {
	  printf("timer: parse_ulong: no digits were found in %s \n", str);
	  return ULONG_MAX;
  }

  /* Successful conversion */
  return val;
}

static long parse_long(char *str, int base) {
  char *endptr;
  unsigned long val;

  val = strtol(str, &endptr, base);

  if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
	  || (errno != 0 && val == 0)) {
	  perror("strtol");
	  return LONG_MAX;
  }

  if (endptr == str) {
	  printf("timer: parse_long: no digits were found in %s \n", str);
	  return LONG_MAX;
  }

  /* Successful conversion */
  return val;
}