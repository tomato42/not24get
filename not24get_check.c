#include <stdio.h>
#include <stdlib.h>
#include "ppolicy.h"

int
main (int argc, char **argv)
{
  char p[1024];
  char *password;
  char *err;
  int ret;

  password = fgets (p, sizeof p, stdin);
  if (!password)
    {
      fprintf(stderr, "Cannot read password\n");
      return 1;
    }

  ret = check_password(password, &err, NULL);

  if (ret == 0)
    return 0;

  fprintf(stderr, "ERR - %s", err);
  free(err);

  return ret;
}
