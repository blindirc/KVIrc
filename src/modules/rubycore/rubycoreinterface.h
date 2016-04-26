#ifndef _RUBYCOREINTERFACE_
#define _RUBYCOREINTERFACE_

//=============================================================================
//
//   File : rubycoreinterface.h
//   Creation date : Fri April 24 19:45:03 2016 EST by Matt Ullman
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
#include "KviQString.h"
#include "KviKvsRunTimeContext.h"

#include <QStringList>

#define KVI_RUBYCORECTRLCOMMAND_EXECUTE "execute"

typedef struct _KviRubyCoreCtrlCommand_execute
{
	unsigned int           uSize;
	KviKvsRunTimeContext * pKvsContext;
	QString                szContext;
	QString                szCode;
	bool                   bExitOk;
	QString                szRetVal;
	QString                szError;
	QStringList            lWarnings;
	QStringList            lArgs;
	bool                   bQuiet;
} KviRubyCoreCtrlCommand_execute;

#define KVI_RUBYCORECTRLCOMMAND_DESTROY "destroy"

typedef struct _KviRubyCoreCtrlCommand_destroy
{
	unsigned int uSize;
	QString szContext;
} KviRubyCoreCtrlCommand_destroy;

#endif
