// http.c

#include "http.h"

// 请完成这个函数
// 这个函数的参数是一个字符串，表示原始的请求，你可以用printf打印看看是啥
// 这个函数的返回值是response，表示输出的响应，你需要按照格式来自行构造
// 理论上，你写完这个函数，编译运行，能通过访问 http://127.0.0.1 来交互
// 你可以尝试着让你写的函数去访问htdocs里面的网页，方法是通过读取文件
// 如果你不会写，可以试试最暴力地方法，先自己构造一个静态的response在浏览器里看看效果
// 如果你实在不会，可以在answer文件夹里参考答案，然后自己独立实现出来

#define BUFFER_SIZE (4096)
#define PATH_SIZE (1024)
#define SYNC_SIZE (32)

int prefix_match(char *src, const char *model) {
  return (strncmp(src, model, strlen(model)) == 0);
}

char *http_abstract(char *request) {
  int i, abstract_len = 0;
  char *retval;
  if(prefix_match(request, "GET") || prefix_match(request, "POST") || prefix_match(request, "HEAD") || prefix_match(request, "PUT")) {
    while(1) {
      ++abstract_len;
      if(request[abstract_len] == 0x0d && request[abstract_len + 1] == 0x0a) break;
    }
    retval = (char *) malloc(sizeof(char) * abstract_len);
    for(i=0; i<abstract_len; ++i) {
      retval[i] = request[i];
    }
    retval[i] = '\0';
    return retval;
  }
  return NULL;
}

char *http_process(char *request) {
  int flag = -1;
  // flag = -1: be ready to parse first line.
  // flag =  0: get the line filed.
  // flag =  1: get the value.
  int subflag = 0;

  char *req_ptr;
  char uri[BUFFER_SIZE], field[BUFFER_SIZE], value[BUFFER_SIZE];
  char *uri_ptr = uri, *field_ptr = field, *value_ptr = value;

  char http_version[SYNC_SIZE];

  char path[PATH_SIZE];
  getcwd(path, PATH_SIZE);
  printf("%s", path);

  static char response[BUFFER_SIZE];
  *response = '\0';

  FILE *rdfile;

  // printf("%s", request);
  
  for(req_ptr = request; flag != 2; ++req_ptr) {
    //printf("%s(%d)", req_ptr, flag);
    switch(flag) {
      case -1:
        if(*req_ptr == ' ') {
          *value_ptr = '\0';
          *uri_ptr = '\0';
          // process with value
          if(subflag == 0) {
            
            // printf("<%s>", value); // http method
          }
          value_ptr = value;
          ++subflag;
        } else if(subflag == 0) {
          *value_ptr++ = *req_ptr;
        } else if(subflag == 1) {
          *uri_ptr++ = *req_ptr;
        } else if(subflag == 2) {
          if(*req_ptr == 0x0d && *(req_ptr+1) == 0x0a) {
            *value_ptr = '\0';
            // printf("<%s>", value); // http version
            strcpy(http_version, value);
            value_ptr = value;
            ++req_ptr;
            flag = 0;
          } else {
            *value_ptr++ = *req_ptr;
          }
        }
        break;
      case 0:
        if(*req_ptr == ':') {
          ++req_ptr;
          *field_ptr = '\0';
          field_ptr = field;
          flag = 1;
        } else if(*req_ptr == 0x0d && *(req_ptr+1) == 0x0a) {
          ++req_ptr;
          flag = 2;
        } else {
          *field_ptr++ = *req_ptr;
        }
        break;
      case 1:
        if(*req_ptr == 0x0d && *(req_ptr+1) == 0x0a) {
          ++req_ptr;
          *value_ptr = '\0';
          // printf("%s=>%s\r\n", field, value);
          value_ptr = value;
          flag = 0;
        } else {
          *value_ptr++ = *req_ptr;
        }
        break;
      default:
        printf("[Error] An error occurred by http process state machine meet unexpected flag %d.\r\n", flag);
    }
  }
  
  strcat(path, "/");
  strcat(path, "htdocs");
  
  for(uri_ptr = uri; *uri_ptr != '\0'; ++uri_ptr) {
    if(*uri_ptr == '/') {
      *uri_ptr = '/';
    }
  }
  for(; uri_ptr != uri; --uri_ptr) {
    if(*uri_ptr == '?') {
      *uri_ptr = '\0';
      break;
    }
  }
  if(strcmp(uri, "/") == 0) {
    strcat(uri, "index.html");
  }
  strcat(path, uri);

  // printf("<%s>\r\n", path);
  // printf("(%s)\r\n", uri);

  strcat(response, http_version);
  strcat(response, " ");

  rdfile = fopen(path, "r");

  if(rdfile) {
    strcat(response, "200 OK\r\n");
  } else {
    strcat(response, "404 Not Found\r\n");
  }
  
  strcat(response, "Server: ExLab1\r\n");
  strcat(response, "Connection: close\r\n");
  strcat(response, "Content-type: text/html\r\n");
  
  strcat(response, "\r\n");
  
  if(rdfile) {
    while(fgets(value, BUFFER_SIZE, rdfile) != NULL) {
      strcat(response, value);
    }
    fclose(rdfile);
  } else {
    strcpy(path, "htdocs");
    strcat(path, "/404.html");
    rdfile = fopen(path, "r");
    if(rdfile) {
      while(fgets(value, BUFFER_SIZE, rdfile) != NULL) {
        strcat(response, value);
      }
      fclose(rdfile);
    }
  }

  return response;
}
