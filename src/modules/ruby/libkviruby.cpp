//=============================================================================
//
//   File : libkviruby.cpp.cpp
//   Creation date : Fri April 26 07:51:07 2016 EST by Matt Ullman
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
#include "KviModule.h"
#include "KviModuleManager.h"
#include "KviLocale.h"

#ifdef COMPILE_RUBY_SUPPORT
#include "../rubycore/rubycoreinterface.h"

static KviModule * g_pRubyCoreModule = nullptr;

#define KVS_CHECK_RUBYCORE(m,c) \
	g_pRubyCoreModule = g_pModuleManager->getModule("rubycore"); \
	if(!g_pRubyCoreModule) \
	{ \
		if(!c->switches()->find('q',"quiet")) \
		{ \
			c->warning(__tr2qs_ctx("The rubycore module cannot be loaded: Ruby support is not available","ruby")); \
			c->warning(__tr2qs_ctx("To see more details about the loading failure, try /rubycore.load","ruby")); \
			return true; \
		} \
	}

#define KVS_CHECK_MODULE_STATE(m,c) KVS_CHECK_RUBYCORE(m,c)

#else

#define KVS_CHECK_RUBYCORE(m,c)

#define KVS_CHECK_MODULE_STATE(m,c) \
	if(!c->switches()->find('q',"quiet")) \
		c->warning(__tr2qs_ctx("This KVIrc executable has been compiled without Ruby scripting support","ruby")); \
	return true;

static KviModule * g_pRubyCoreModule = nullptr;
#endif

static bool ruby_kvs_cmd_begin(KviKvsModuleCommandCall * c)
{
	QString szCode, szContext;
	KviKvsVariantList vList;
	KVSM_PARAMETERS_BEGIN(c)
		KVSM_PARAMETER("code",KVS_PT_STRING,0,szCode)
		KVSM_PARAMETER("context",KVS_PT_STRING,KVS_PF_OPTIONAL,szContext)
		KVSM_PARAMETER("args",KVS_PT_VARIANTLIST,KVS_PF_OPTIONAL,vList)
	KVSM_PARAMETERS_END(c)

	KVS_CHECK_MODULE_STATE(m,c)

#ifdef COMPILE_RUBY_SUPPORT

	KviRubyCoreCtrlCommand_execute ex;
	ex.uSize = sizeof(KviRubyCoreCtrlCommand_execute);
	ex.pKvsContext = c->context();
	ex.szCode = szCode;
	for(KviKvsVariant * v = vList.first(); v; v = vList.next())
	{
		QString tmp;
		v->asString(tmp);
		ex.lArgs.append(tmp);
	}
	ex.bQuiet = c->switches()->find('q',"quiet");

	if(!g_pRubyCoreModule->ctrl(KVI_RUBYCORECTRLCOMMAND_EXECUTE, &ex))
	{
		if(!c->switches()->find('q',"quiet"))
			c->warning(__tr2qs_ctx("The rubycore module failed to execute the code: something is wrong with the Ruby support","ruby"));
		return true;
	}

	if(!ex.lWarnings.isEmpty())
	{
		for(const auto& it : ex.lWarnings)
			c->warning(it);
	}

	if(!ex.bExitOk && !c->switches()->find('q',"quiet"))
	{
		c->warning(__tr2qs_ctx("Ruby execution error:","ruby"));
		c->warning(ex.szError);

		if(c->switches()->find('f',"fail-on-error"))
			return false;
	}

	if(!c->switches()->find('n',"no-return"))
		c->context()->returnValue()->setString(ex.szRetVal);

#endif

	return true;
}

static bool ruby_kvs_cmd_destroy(KviKvsModuleCommandCall * c)
{
	QString szContext;
	KVSM_PARAMETERS_BEGIN(c)
		KVSM_PARAMETER("context",KVS_PT_NONEMPTYSTRING,0,szContext)
	KVSM_PARAMETERS_END(c)

	KVS_CHECK_MODULE_STATE(m,c)

#ifdef COMPILE_RUBY_SUPPORT
	KviRubyCoreCtrlCommand_destroy ex;
	ex.uSize = sizeof(KviRubyCoreCtrlCommand_destroy);
	ex.szContext = szContext;

	if(!g_pRubyCoreModule->ctrl(KVI_RUBYCORECTRLCOMMAND_DESTROY,&ex))
	{
		if(!c->switches()->find('q',"quiet"))
			c->warning(__tr2qs_ctx("The rubycore module failed to execute the code: something is wrong with the Ruby support","ruby"));
	}
#endif

	return true;
}

static bool ruby_kvs_fnc_isAvailable(KviKvsModuleFunctionCall * c)
{
#ifdef COMPILE_RUBY_SUPPORT
	g_pRubyCoreModule = g_pModuleManager->getModule("rubycore");
	c->returnValue()->setBoolean(g_pRubyCoreModule ? true : false);
#else
	c->returnValue()->setBoolean(false);
#endif
	return true;
}

static bool ruby_module_init(KviModule * m)
{
	// qDebug("in static bool ruby_module_init(KviModule * m)");
	KVSM_REGISTER_SIMPLE_COMMAND(m,"begin",ruby_kvs_cmd_begin);
	KVSM_REGISTER_SIMPLE_COMMAND(m,"destroy",ruby_kvs_cmd_destroy);

	KVSM_REGISTER_FUNCTION(m,"isAvailable",ruby_kvs_fnc_isAvailable);
#ifdef COMPILE_RUBY_SUPPORT
	g_pRubyCoreModule = g_pModuleManager->getModule("rubycore");
#endif
	return true;
}

static bool ruby_module_cleanup(KviModule *)
{
	return true;
}

KVIRC_MODULE(
	"RubyCore",
	"4.9.2",
	"Copyright (C) 2016 Matt Ullman (staticfox at staticfox dot net)",
	"Ruby Scripting Engine",
	ruby_module_init,
	0,
	0,
	ruby_module_cleanup,
	"ruby"
)

