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

int
read_config ()
{
  return LDAP_SUCCESS;
}

int
count_classes (char *password)
{
  int lcase = 0;
  int ucase = 0;
  int digits = 0;
  int punct = 0;
  int other = 0;

  char *p = password;

  while (*p != '\0')
    {
      if (islower (*p))
	lcase = 1;
      // ignore capital letter at the as the first character
      else if (isupper (*p) && p != password)
	ucase = 1;
      // ignore digit as last character
      else if (isdigit (*p) && *(p+1) != '\0')
	digits = 1;
      else if (ispunct (*p))
	punct = 1;
      else if (!iscntrl (*p) && isascii(*p))
	other = 1;
      p++;
    }

  return lcase + ucase + digits + punct + other;
}

// count number of "words" in password, where word is an alpha sequence of
// at least 3 characters
int
count_words (char *password)
{
  int words = 0;
  int word_len = 0;
  int is_word = 0;
  char *p;

  for (p = password; *p != '\0'; p++)
    {
      // word just started
      if (isalpha (*p) && !is_word)
	{
	  is_word = 1;
	  word_len = 1;
	  continue;
	}

      // word just ended
      if (!isalpha (*p) && is_word)
	{
	  is_word = 0;
	  if (word_len >= 3)
	    words++;
	  word_len = 0;
	  continue;
	}

      // word is in progress, count letters
      if (isalpha (*p) && is_word)
	{
	  word_len++;
	  continue;
	}

      // no word in progress, just continue
    }

  if (is_word && word_len >= 3)
    words++;

  return words;
}

int
check_password (char *pPasswd, char **ppErrStr, void *pEntry)
{
  *ppErrStr = NULL;

  if (!pPasswd)
    {
      *ppErrStr = strdup ("Null password");
      return FAILURE;
    }

  if (read_config ())
    {
      *ppErrStr = strdup ("Internal error");
      return FAILURE;
    }

  int pass_len = strnlen (pPasswd, MAX_LENGTH);

  if (pass_len == MAX_LENGTH || pass_len >= max_len)
    {
      *ppErrStr = strdup ("Password too long");
      return FAILURE;
    }

  if (pass_len <= 6)
    {
      *ppErrStr = strdup ("Password REALLY short");
      return FAILURE;
    }

  int pass_class = count_classes (pPasswd);

  if (pass_class == 0)
    {
      *ppErrStr = strdup ("Password has no printable characters");
      return FAILURE;
    }

  if (pass_class >= 1)
    if (min_1class != -1 && pass_len >= min_1class)
      return LDAP_SUCCESS;
  if (pass_class >= 2)
    if (min_2class != -1 && pass_len >= min_2class)
      return LDAP_SUCCESS;
  if (pass_class >= 3)
    if (min_3class != -1 && pass_len >= min_3class)
      return LDAP_SUCCESS;
  if (pass_class >= 4)
    if (min_4class != -1 && pass_len >= min_4class)
      return LDAP_SUCCESS;

  if (pass_class < 2)
    {
      *ppErrStr = strdup ("Password too simple");
      return FAILURE;
    }

  int pass_words = count_words (pPasswd);

  if (min_words != -1 && pass_words >= min_words)
    return LDAP_SUCCESS;

  *ppErrStr = strdup ("Password too simple");
  return FAILURE;
}
