#ifndef Ruby_KVIRCMODULE_H
#define Ruby_KVIRCMODULE_H
//=============================================================================
//
//   File : libkvirubycore.cpp
//   Creation date : Fri April 25 15:57:43 2016 EST by Matt Ullman
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

/**
* \file kvircmodule.h
* \author Matt Ullman
* \brief KVIrc module for Ruby
*
* This ruby module defines some functions to interact with KVIRc.
*/

#include "kvi_settings.h"

#ifdef COMPILE_RUBY_SUPPORT

void Init_kvirc();

#endif

#endif
