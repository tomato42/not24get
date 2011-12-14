#include <ldap.h>
#include <string.h>
#include <ctype.h>
#include "ppolicy.h"

#define FAILURE 0x01

// hardcoded ultimate maximum length of password
#define MAX_LENGTH 1024

// minumum length of passwords having characters from 4 classes of characters
int min_4class = 10;
// minimum length of passwords having characters from 3 classes of characters
int min_3class = 12;
// minimum length of passwords having characters from 2 classes of characters 
int min_2class = 14;
// minimum length of passwords having characters from single class of
// characters
int min_1class = -1;
// minimume length of passwords meeting criteria for a passphrase
int min_phrase = 16;
// minimum number of words in a password meeting criteria for a passphrase
int min_words = 3;
// maximum length of password
int max_len = 40;

int read_config ()
{
  return LDAP_SUCCESS;
}

int count_classes(char *password)
{
  int lcase = 0;
  int ucase = 0;
  int digits = 0;
  int punct = 0;
  int other = 0;

  char *p = password;

  while (*p != '\0')
    {
      if (islower(*p))
        lcase=1;
      else if (isupper(*p))
        ucase=1;
      else if (isdigit(*p))
        digits=1;
      else if (ispunct(*p))
        punct=1;
      else if (!isspace(*p) && !iscntrl(*p))
        other=1;
      p++;
    }

  return lcase + ucase + digits + punct + other;
}

int check_password (char *pPasswd, char **ppErrStr, void *pEntry)
{
  if  (!pPasswd)
    {
      *ppErrStr = strdup("Null password");
      return FAILURE;
    }

  if(read_config())
    {
      *ppErrStr = strdup("Internal error");
      return FAILURE;
    }

  int pass_len = strnlen(pPasswd, MAX_LENGTH);

  if (pass_len == MAX_LENGTH || pass_len >= max_len)
    {
      *ppErrStr = strdup("Password too long");
      return FAILURE;
    }

  if (pass_len <= 6 )
    {
      *ppErrStr = strdup("Password REALLY short");
      return FAILURE;
    }

  int pass_class = count_classes(pPasswd);

  switch (pass_class)
    {
    case 0:
      *ppErrStr = strdup("Password has no printable characters");
      return FAILURE;
    case 1:
      if (min_1class == -1 || pass_len < min_1class)
        {
          *ppErrStr = strdup("Password too simple");
          return FAILURE;
        }
      break;
    case 2:
      if (min_2class == -1 || pass_len < min_2class)
        {
          *ppErrStr = strdup("Password too simple");
          return FAILURE;
        }
      break;
    case 3:
      if (min_3class == -1 || pass_len < min_3class)
        {
          *ppErrStr = strdup("Password too simple");
          return FAILURE;
        }
      break;
    case 4:
      if (min_4class == -1 || pass_len < min_4class)
        {
          *ppErrStr = strdup("Password too simple");
          return FAILURE;
        }
      break;
    default:
      *ppErrStr = strdup("Internal error");
      return FAILURE;
    }

  return 0;
}