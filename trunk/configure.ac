AC_PREREQ([2.68])
AC_INIT([tecnoballz], [0.93], [info@tlkgames.com])
dnl AC_CONFIG_SRCDIR([src/list_sprites.cc])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign])

AC_ARG_ENABLE(devel,
[  --enable-devel          Development version (default disabled)],
enable_devel=yes, enable_devel=no)


# Checks for programs.
AC_PROG_CXX

# Checks for libraries.
AM_PATH_SDL
LDFLAGS="${LDFLAGS} ${SDL_LIBS}"
AC_CHECK_LIB(SDL_mixer, Mix_GetChunk, [
  SDL_LIBS="-lSDL_mixer ${SDL_LIBS}"
],[
  AC_MSG_ERROR(Could not find -lSDL_mixer)
])
AC_CHECK_LIB(SDL_image, main, [
  SDL_LIBS="-lSDL_image ${SDL_LIBS}"
],[
  AC_MSG_ERROR(Could not find -lSDL_image)
])
AC_CHECK_LIB(SDL_mixer, Mix_GetChunk, [
  SDL_LIBS="-lSDL_mixer ${SDL_LIBS}"
],[
  AC_MSG_ERROR(Could not find -lSDL_mixer)
])
AC_CHECK_LIB(mikmod, Player_Stop, [
  MIKMOD_LIBS="-lmikmod"
  MIKMOD_FLAGS="-pthread -D_REENTRANT"
],[
  AC_MSG_ERROR(Could not find the Mikmod library : -lmikmod)
])
AC_SUBST(MIKMOD_FLAGS)
AC_SUBST(MIKMOD_LIBS)

AC_CHECK_LIB([tinyxml], [main], [
  TINYXML_LIBS="-ltinyxml"
],[
  AC_MSG_ERROR([tinyxml library is not present in your system])
])
AC_SUBST(TINYXML_LIBS)

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stddef.h stdlib.h string.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([atexit memmove strchr strdup strerror])

if test "x${enable_devel}" = "xyes"; then
  AC_DEFINE(UNDER_DEVELOPMENT,  1, Define to enable development version)
  CXXFLAGS="-O2 -Werror -Wall -pedantic -Wextra -std=gnu++98 -g"
else
  CXXFLAGS="-O2 -Werror -Wall -pedantic -Wextra -std=gnu++98"
fi

AC_OUTPUT(
  Makefile
  src/Makefile
  src/TecnoballZ/Makefile
  src/TecnoballZ/hires/Makefile
  src/TecnoballZ/lores/Makefile
  src/TecnoballZ/textures/Makefile
  src/TecnoballZ/musics/Makefile
  src/TecnoballZ/sounds/Makefile
  src/TecnoballZ/texts/Makefile)

echo "

TecnoballZ ready for building!

  enable_devel      : $enable_devel

Type 'make' to build.

"





