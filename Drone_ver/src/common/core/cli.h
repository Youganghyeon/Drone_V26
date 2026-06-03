/*
 * cli.h
 *
 *  Created on: 2020. 12. 19.
 *      Author: baram
 */

/*
 * cli library created by baram
 *
 */


#ifndef SRC_COMMON_CORE_CLI_H_
#define SRC_COMMON_CORE_CLI_H_

#include "def.h"

#ifdef _USE_DEF_CLI

#define CLI_CMD_LIST_MAX      DEF_CLI_CMD_LIST_MAX
#define CLI_CMD_NAME_MAX      DEF_CLI_CMD_NAME_MAX
#define CLI_LINE_HIS_MAX      DEF_CLI_LINE_HIS_MAX
#define CLI_LINE_BUF_MAX      DEF_CLI_LINE_BUF_MAX




typedef struct
{
  uint16_t   argc;
  char     **argv;

  int32_t  (*getData)(uint8_t index);
  float    (*getFloat)(uint8_t index);
  char    *(*getStr)(uint8_t index);
  bool     (*isStr)(uint8_t index, char *p_str);
} cli_args_t;


bool cliInit(void);
bool cliOpen(uint8_t ch, uint32_t baud);
bool cliOpenLog(uint8_t ch, uint32_t baud);
bool cliMain(void);
void cliPrintf(const char *fmt, ...);
bool cliAdd(const char *cmd_str, void (*p_func)(cli_args_t *));
bool cliKeepLoop(void);


#endif



#endif /* SRC_COMMON_CORE_CLI_H_ */
