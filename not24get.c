/**
 * not24get.c
 *
 * Copyright (C) 2011  Hubert Kario
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 *
 */
//#define DEBUG
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <ldap.h>
#include <string.h>
#include <ctype.h>
#include <passwdqc.h>
#include "ppolicy.h"

#define CONFIG_DIRECTIVE "config=" CONFIG_FILE

#define FAILURE 0x01

// hardcoded ultimate maximum length of password (for overflow checking)
#define MAX_LENGTH 1024

static int
read_config_file (passwdqc_params_t *params)
{
  // reason for parsing failure
  char *parse_reason;
  // saved first error
  char *err_reason = NULL;
  // default config file locations
  const char *const config[] = {CONFIG_DIRECTIVE,
                                "config=/etc/ldap/not24get.conf",
                                "config=/etc/not24get.conf",
                                "config=/etc/ldap/passwdqc.conf",
                                "config=/etc/passwdqc.conf",
                                NULL};

  passwdqc_params_reset(params);

  int i=0;
  while (config[i] != NULL)
    {
      if (passwdqc_params_parse(params, &parse_reason, 1,
            &config[i]))
        {
          if (!err_reason)
            err_reason = strdup((parse_reason)?parse_reason:"Out of memory");
          free(parse_reason);
          i++;
          continue;
        }
      else
        {
          // failback location worked fine, we won't be needing this error
          // message
          if (err_reason)
            free(err_reason);

          return 0;
        }
    }

  fprintf(stderr, "not24get: %s\n", err_reason);
  free(err_reason);
  return 1;
}

int
check_password (char *pPasswd, char **ppErrStr, void *pEntry)
{
  *ppErrStr = NULL;
  passwdqc_params_t params;
  const char *check_reason;

  // check for invalid arguments
  if (!pPasswd || !ppErrStr)
    {
      *ppErrStr = strdup ("Null password");
      return FAILURE;
    }

  if (read_config_file (&params))
    {
      *ppErrStr = strdup ("Internal error, can't read config");
      return FAILURE;
    }

  // check for buffer oveflow
  if (MAX_LENGTH == strnlen (pPasswd, MAX_LENGTH))
    {
      *ppErrStr = strdup ("Password too long");
      return FAILURE;
    }

  // perform actual password checking, third param is old password,
  // fourth param is passwd entry
  // TODO convert pEntry to struct passwd, pass as 4h parameter
  check_reason = passwdqc_check(&params.qc, pPasswd, NULL, NULL);

  if (!check_reason)
    return LDAP_SUCCESS;

  if (asprintf(ppErrStr, "Bad password (%s)", check_reason) == -1)
    {
      *ppErrStr = strdup("Internal error, asprintf failed");
    }
  return FAILURE;
}
