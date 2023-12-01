// http.h

#ifndef _HTTP_H_
#define _HTTP_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <direct.h>

#ifdef __cpluscplus
extern "C" {
#endif

int prefix_match(char *src, const char *model);
char *http_abstract(char *request);
char *http_process(char *request);

#ifdef __cpluscplus
}
#endif

#endif // _HTTP_H_
