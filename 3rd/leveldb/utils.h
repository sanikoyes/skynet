/*
 * utils.h
 *
 *  Created on: 14/set/2013
 *      Author: Marco Pompili
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <string.h>

#define STRTYPE "string"
#define NUMTYPE "number"

// better strcmp support
#define streql(x,y) (strcmp(x,y) == 0)

// type check
#define t_is_string(s) (streql(s, STRTYPE))
#define t_is_number(s) (streql(s, NUMTYPE))

// conversion
#define double2bin(d, buf) memcpy(&d, buf, sizeof d)
#define slice2num(s) lua_str2number(s, NULL)

#endif /* UTILS_H_ */
