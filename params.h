/*
 * params.h
 *
 *  Created on: 16 июл. 2023 г.
 *      Author: User
 */

#ifndef PARAMS_H_
#define PARAMS_H_

#include "dns_call1.h"

const char *str_ip4 = 0;
const char *str_webser = 0;

static void
parse (int argc, char *argv[])
{

//    for (int i = 1; i < argc; ++i) {
//        argv[i];
//    }

  char *defip = "8.8.8.8";

  char *par = 0;
  if (argc > 1)
    if (argv[1] == 0)
      argc = 0;

  if (argc > 1)
    par = strdup (argv[1]);
  else
    par = defip;

  if (par[0] == 0)
    par = defip;

  char *ip4chars = "0123456789.";
  if (strspn (par, ip4chars) == strlen (par))
    {
      str_ip4 = par;
      str_webser = par;
    }
  else
    {
      str_ip4 = dns_call (par);
      str_webser = par;
    }

}

#endif /* PARAMS_H_ */
