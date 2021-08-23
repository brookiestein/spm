AC_INIT([spm],[1.4.1],[https://github.com/brookiestein/spm])
AM_INIT_AUTOMAKE([foreign])
AC_PROG_CC
AC_CONFIG_FILES([Makefile])
AC_PROG_INSTALL

AC_ARG_ENABLE([gui],
        [  --enable-gui       Enable graphic user interface],
        [case "${enableval}" in
         yes) gui=true ;;
         no) gui=false ;;
         *) AC_MSG_ERROR([bad value ${enableval} for --enable-gui]) ;;
        esac],[gui=false])
AM_CONDITIONAL([GUI], [test x$gui = xtrue])

if test "x$gui" = "xtrue"
then
        CFLAGS="$CFLAGS $(pkg-config --cflags --libs gtk+-3.0)"
        CPPFLAGS="$CPPFLAGS -D_ENABLE_GUI"
fi

AC_ARG_ENABLE([locker],
        [  --enable-locker    Run a user-defined screen locker before suspending or hibernating when using gui],
        [case "${enableval}" in
         yes) locker=true ;;
         no) locker=false ;;
         *) AC_MSG_ERROR([bad value ${enableval} for --enable-locker]) ;;
        esac],[locker=false])
AM_CONDITIONAL([LOCKER], [test x$locker = xtrue])

AC_ARG_ENABLE([default_locker],
        [  --enable-default-locker[=locker] Default screen locker to run before suspending or hibernating (overrides --enable-locker)],
        [default_locker="${enableval}"])

if test -n "$default_locker"
then
        CPPFLAGS="$CPPFLAGS -D_DEFAULT_LOCKER=\\\"$default_locker\\\""
fi

if test "x$locker" = "xtrue"
then
        CPPFLAGS="$CPPFLAGS -D_ASK_FOR_LOCKER"
fi

CFLAGS="$CFLAGS -pthread $(pkg-config --cflags --libs dbus-1)"

AC_OUTPUT
