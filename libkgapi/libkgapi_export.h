/*
    Copyright 2012  Dan Vratil <dan@progdan.cz>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LIBKGAPI_EXPORT_H
#define LIBKGAPI_EXPORT_H

#include <QtGlobal>

#ifndef KGAPI_DEPRECATED
#  ifndef KGAPI_DEPRECATED_NOWARN
#    define KGAPI_DEPRECATED Q_DECL_DEPRECATED
#  else
#    define KGAPI_DEPRECATED
#  endif
#endif

#if defined(LIBKGAPI_LIBRARY)
#  define LIBKGAPI_EXPORT_DEPRECATED Q_DECL_EXPORT KGAPI_DEPRECATED
#else
#  define LIBKGAPI_EXPORT_DEPRECATED Q_DECL_IMPORT KGAPI_DEPRECATED
#endif

#endif // LIBKGAPI_EXPORT_H
