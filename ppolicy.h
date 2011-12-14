/**
 * this file defines the API which the pwdCheckModule, as used by the OpenLDAP
 * ppolicy module, needs to implement
 */
#ifndef _PPOLICY_H
#define _PPOLICY_H

/**
 * Check quality of provided password
 *
 * @param[in]  pPasswd  The password to be checked for quality, NULL terminated
 * @param[out] ppErrStr Double pointer to human-readable error description, it
 *                      must be NULL terminated and dynamically allocated as
 *                      OpenLDAP will call free() on it
 * @param[in]  pEntry   OpenLDAP Entry object of which the password is being
 *                      changed
 * @return LDAP_SUCCESS if the password meets the additional requirements,
 *         any other value means failure
 */
int check_password (char *pPasswd, char **ppErrStr, void *pEntry);

#endif
