//=============================================================================
//
//   File : kvircmodule.cpp
//   Creation date : Fri April 25 15:33:37 2016 EST by Matt Ullman
//
//   This file is part of the KVIrc IRC client distribution
//   Copyright (C) 2016 Matt Ullman (staticfox at staticfox dot net)
//
//   This program is FREE software. You can redistribute it and/or
//   modify it under the terms of the GNU General Public License
//   as published by the Free Software Foundation; either version 2
//   of the License, or (at your option) any later version.
//
//   This program is distributed in the HOPE that it will be USEFUL,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//   See the GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program. If not, write to the Free Software Foundation,
//   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
//=============================================================================

#include "kvi_settings.h"

#ifdef COMPILE_RUBY_SUPPORT

#include "kvircmodule.h"
#include "KviApplication.h"
#include "KviWindow.h"
#include "KviUserInput.h"
#include "KviKvsRunTimeContext.h"
#include "KviKvsScript.h"

#include <ruby.h>

#define KVIRC_MODULE

extern KviKvsRunTimeContext * g_pCurrentKvsContext;
extern bool g_bExecuteQuiet;
extern KviCString g_szLastReturnValue;
extern QStringList g_lWarningList;
extern QString g_lError;

VALUE cKVIrc = Qnil;

static VALUE echo(VALUE self) {
	int x = 10;
	return INT2NUM(x);
}

void Init_kvirc() {
	cKVIrc = rb_define_class("KVIrc", rb_cObject);
	rb_define_method(cKVIrc, "echo", (VALUE(*)(ANYARGS))echo, 0);
}

#endif
