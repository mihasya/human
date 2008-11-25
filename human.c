/*
  +----------------------------------------------------------------------+
  | PHP Version 5														|
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2007 The PHP Group								|
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,	  |
  | that is bundled with this package in the file LICENSE, and is		|
  | available through the world-wide-web at the following url:		   |
  | http://www.php.net/license/3_01.txt								  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to		  |
  | license@php.net so we can mail you a copy immediately.			   |
  +----------------------------------------------------------------------+
  | Author:															  |
  +----------------------------------------------------------------------+
*/

/* $Id: header,v 1.16.2.1.2.1 2007/01/01 19:32:09 iliaa Exp $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <math.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_human.h"

/* If you declare any globals in php_human.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(human)
*/

/* True global resources - no need for thread safety here */
static int le_human;

/* {{{ human_functions[]
 *
 * Every user visible function must have an entry in human_functions[].
 */
zend_function_entry human_functions[] = {
	PHP_FE(human_interval_precise, NULL)
	PHP_FE(human_interval, NULL)
	{NULL, NULL, NULL}	/* Must be the last line in human_functions[] */
};
/* }}} */

/* {{{ human_module_entry
 */
zend_module_entry human_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"human",
	human_functions,
	PHP_MINIT(human),
	PHP_MSHUTDOWN(human),
	PHP_RINIT(human),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(human),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(human),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_HUMAN
ZEND_GET_MODULE(human)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
	STD_PHP_INI_ENTRY("human.global_value",	  "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_human_globals, human_globals)
	STD_PHP_INI_ENTRY("human.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_human_globals, human_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_human_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_human_init_globals(zend_human_globals *human_globals)
{
	human_globals->global_value = 0;
	human_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(human)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(human)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(human)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(human)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(human)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "human support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/*{{{ function to convert a number of seconds into an interval
*/
PHP_FUNCTION(human_interval_precise)
{
	//declare variables
	unsigned long weeks;
	unsigned long days;
	unsigned long hours;
	unsigned long minutes;
	unsigned long seconds;
	
	//deckare constants used in calculation
	const unsigned long sec_in_week = 604800;
	const unsigned long sec_in_day = 86400;
	const unsigned long sec_in_hour = 3600;
	const unsigned long sec_in_min = 60;
	
	//declare and zero out the return string
	char retstr[60];
	*retstr = 0;
	
	//grab parameter
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &seconds) == FAILURE) {
		RETURN_NULL();
	}
	
	//logic
	weeks = seconds/sec_in_week;
	if (weeks > 0) {
		seconds = seconds%sec_in_week;
		sprintf(retstr+strlen(retstr), "%dw ", weeks);
	}
	days = seconds/sec_in_day;
	if (days > 0) {
		seconds = seconds%sec_in_day;
		sprintf(retstr+strlen(retstr), "%dd ", days);
	}
	hours = seconds/sec_in_hour;
	if (hours > 0) {
		seconds = seconds%sec_in_hour;
		sprintf(retstr+strlen(retstr), "%dh ", hours);
	}
	minutes = seconds/sec_in_min;
	if (minutes > 0) {
		seconds = seconds%sec_in_min;
		sprintf(retstr+strlen(retstr), "%dm ", minutes);
	}
	if (seconds > 0) {
		sprintf(retstr+strlen(retstr), "%ds", seconds);
	}
	//get rid of trailing space
	int lastchar = strlen(retstr)-1;
	if (retstr[lastchar]==' ') {
		retstr[lastchar] = 0;
	}
	//DRUMROLL PLZ!!
	RETURN_STRING(retstr, 1);
} /*}}}*/

/*{{{ return the approximation of the interval in the largest sensible unit
*/
PHP_FUNCTION(human_interval) { 
	
	long seconds, num;
	
	//deckare constants used in calculation
	const float sec_in_week = 604800;
	const float sec_in_day = 86400;
	const float sec_in_hour = 3600;
	const float sec_in_min = 60;
	
	float frac;
	char retstr[60];
	*retstr = 0;
	
	//grab parameter
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &seconds) == FAILURE) {
		RETURN_NULL();
	}
	
	if ((num = round(seconds/sec_in_week))>0) {
		sprintf(retstr, "%dw", num);
	} else if ((num = round(seconds/sec_in_day))>0) {
		sprintf(retstr, "%dd", num);
	} else if ((num = round(seconds/sec_in_hour))>0) {
		sprintf(retstr, "%dh", num);
	} else if ((num = round(seconds/sec_in_min))>0) {
		sprintf(retstr, "%dm", num);
	}
	
	RETURN_STRING(retstr, 1);
} /*}}}*/
/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
