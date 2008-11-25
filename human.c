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
  | Author:	Mikhail Panchenko <m@mihasya.com>							|
  +----------------------------------------------------------------------+
@todo: add larger units to time_interval (months, years, decades etc)
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

/* CONSTANTS */
#define SEC_IN_YEAR 	31449600
#define SEC_IN_MONTH 	2592000
#define SEC_IN_WEEK 	604800
#define SEC_IN_DAY 		86400
#define SEC_IN_HOUR 	3600
#define SEC_IN_MIN 		60

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
	long seconds;
	char retstr[60];
	
	*retstr = 0;
	
	//grab parameter
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &seconds) == FAILURE) {
		RETURN_NULL();
	}
	
	//logic
	if (seconds >= SEC_IN_WEEK) {
		sprintf(retstr+strlen(retstr), "%dw ", seconds/SEC_IN_WEEK);
		seconds = seconds%SEC_IN_WEEK;
	}
	if (seconds >= SEC_IN_DAY) {		
		sprintf(retstr+strlen(retstr), "%dd ", seconds/SEC_IN_DAY);
		seconds = seconds%SEC_IN_DAY;
	}
	if (seconds >= SEC_IN_HOUR) {
		sprintf(retstr+strlen(retstr), "%dh ", seconds/SEC_IN_HOUR);
		seconds = seconds%SEC_IN_HOUR;
	}
	if (seconds >= SEC_IN_MIN) {
		sprintf(retstr+strlen(retstr), "%dm ", seconds/SEC_IN_MIN);
		seconds = seconds%SEC_IN_MIN;
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
	
	long seconds;
	char retstr[60];
	
	*retstr = 0;
	
	//grab parameter
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &seconds) == FAILURE) {
		RETURN_NULL();
	}
	
	if (seconds >= SEC_IN_WEEK) {
		sprintf(retstr, "%dw", seconds/SEC_IN_WEEK);
	} else if (seconds >= SEC_IN_DAY) {
		sprintf(retstr, "%dd", seconds/SEC_IN_DAY);
	} else if (seconds >= SEC_IN_HOUR) {
		sprintf(retstr, "%dh", seconds/SEC_IN_HOUR);
	} else {
		sprintf(retstr, "%dm", seconds/SEC_IN_MIN);
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
