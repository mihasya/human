dnl $Id$
dnl config.m4 for extension human

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(human, for human support,
dnl Make sure that the comment is aligned:
dnl [  --with-human             Include human support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(human, whether to enable human support,
dnl Make sure that the comment is aligned:
dnl [  --enable-human           Enable human support])

if test "$PHP_HUMAN" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-human -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/human.h"  # you most likely want to change this
  dnl if test -r $PHP_HUMAN/$SEARCH_FOR; then # path given as parameter
  dnl   HUMAN_DIR=$PHP_HUMAN
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for human files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       HUMAN_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$HUMAN_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the human distribution])
  dnl fi

  dnl # --with-human -> add include path
  dnl PHP_ADD_INCLUDE($HUMAN_DIR/include)

  dnl # --with-human -> check for lib and symbol presence
  dnl LIBNAME=human # you may want to change this
  dnl LIBSYMBOL=human # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $HUMAN_DIR/lib, HUMAN_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_HUMANLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong human lib version or lib not found])
  dnl ],[
  dnl   -L$HUMAN_DIR/lib -lm -ldl
  dnl ])
  dnl
  dnl PHP_SUBST(HUMAN_SHARED_LIBADD)

  PHP_NEW_EXTENSION(human, human.c, $ext_shared)
fi
