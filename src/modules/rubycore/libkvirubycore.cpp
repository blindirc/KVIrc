//=============================================================================
//
//   File : libkvirubycore.cpp
//   Creation date : Fri April 24 11:12:24 2016 EST by Matt Ullman
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
#include "KviLocale.h"

#ifdef COMPILE_RUBY_SUPPORT
#include <functional>
#include <memory>
#include <unordered_map>
#include <utility>

#include <ruby.h>

#include "rubycoreinterface.h"

#ifdef RUBY_GLOBAL_SETUP
	// For anyone that ever finds this and wonders what this does,
	// apparently it's legacy for:
	// static void objcdummyfunction(void) {objc_msgSend();}
	// Ruby's documentation really fucking sucks.
	RUBY_GLOBAL_SETUP;
#endif

KviKvsRunTimeContext * g_pCurrentKvsContext = nullptr;
bool g_bExecuteQuiet = false;
QStringList g_lWarningList;
QString g_lError;

struct KviRubyInterpreter {
	KviRubyInterpreter();
	~KviRubyInterpreter();
	bool execute(QString, QStringList&, QString&, QString&, QStringList&);
};

struct KviCaseInsensitiveQStringHash {
	std::size_t operator()(const QString& s) const
	{
		return static_cast<std::size_t>(qHash(s.toLower()));
	}
};

struct KviCaseInsensitiveQStringEqual {
	bool operator()(const QString& s, const QString& t) const
	{
		return (s.toLower() == t.toLower());
	}
};

static std::unordered_map<QString,KviRubyInterpreter,KviCaseInsensitiveQStringHash,KviCaseInsensitiveQStringEqual> g_Interpreters;

static void rubycore_destroy_interpreter(const QString& szContextName)
{
	const auto i = g_Interpreters.find(szContextName);

	if(i != g_Interpreters.end())
		g_Interpreters.erase(i);
}

KviRubyInterpreter::KviRubyInterpreter()
{
	qDebug("In ruby interpreter constructor");
}

KviRubyInterpreter::~KviRubyInterpreter()
{
	qDebug("in ruby interpreter destructor");
	// ruby_cleanup(0); // <-- this segfaults the vm?
}

static VALUE sink(VALUE self, VALUE input)
{
	char * got = StringValueCStr(input);
	qDebug(got);
	return Qnil;
}


bool KviRubyInterpreter::execute(QString szCode, QStringList& lArgs,
	QString& szRetVal, QString& szError, QStringList&)
{
	int retVal, retVal1;

	g_lError.clear();

	QString szVarCode = "aArgs = [";

	bool bFirst = true;
	foreach(QString szArg, lArgs)
	{
		if(!bFirst)
			szVarCode += ",";
		else
			bFirst = false;
		szVarCode += QString::fromLatin1("\"%1\"").arg(szArg);
	}

	szVarCode += "]";

	int state;
	rb_eval_string_protect(
	"class MockStream \n"
	" def write(x) \n"
	" sink(x) \n"
	" end \n"
	"end \n"
	"$stdout = MockStream.new()\n"
	, &state);

	qDebug("state code: %d", state);

	qDebug("Executing %s", szVarCode.toUtf8().data());

	szCode.replace(QRegExp("\r\n?"), "\n");

	rb_eval_string_protect(szVarCode.toUtf8().data(), &retVal1);

	qDebug("Executing %s", szCode.toUtf8().data());

	// the mockstream throws an unconditional fatal error. (0x6)
	// y u do dis
	rb_eval_string_protect(szCode.toUtf8().data(), &retVal);

	// ruby basically doesn't have user-readable error handling through it's
	// C api. :cry:
	VALUE exception = rb_errinfo();

	if(retVal) {
		szError = g_lError;
		qDebug("Ruby script error: %d", exception);
	}

	rb_set_errinfo(Qnil);

	return !retVal;
}

#endif

template<typename T>
T * castFromModParam(void * p)
{
	T * ex = static_cast<T *>(p);

	if(ex->uSize != sizeof(T))
		return nullptr;

	return ex;
}

static bool rubycore_module_ctrl(KviModule *, const char * cmd, void * param)
{
#ifdef COMPILE_RUBY_SUPPORT
	if(!strcmp(cmd,KVI_RUBYCORECTRLCOMMAND_EXECUTE))
	{
		auto * pex = castFromModParam<KviRubyCoreCtrlCommand_execute>(param);

		if(!pex)
			return false;

		auto& ex = *pex;

		g_pCurrentKvsContext = ex.pKvsContext;
		g_bExecuteQuiet = ex.bQuiet;
		if(ex.szContext.isEmpty())
		{
			KviRubyInterpreter m;
			ex.bExitOk = m.execute(ex.szCode, ex.lArgs, ex.szRetVal, ex.szError, ex.lWarnings);
		} else {
			KviRubyInterpreter& m = g_Interpreters[ex.szContext];
			ex.bExitOk = m.execute(ex.szCode, ex.lArgs, ex.szRetVal, ex.szError, ex.lWarnings);
		}
		return true;
	}
	if(!strcmp(cmd,KVI_RUBYCORECTRLCOMMAND_DESTROY))
	{
		auto * pde = castFromModParam<KviRubyCoreCtrlCommand_destroy>(param);

		if(!pde)
			return false;

		rubycore_destroy_interpreter(pde->szContext);
		return true;
	}
#endif
	return false;
}

static bool rubycore_module_init(KviModule *)
{
#ifdef COMPILE_RUBY_SUPPORT
	ruby_init();
	ruby_show_version(); // <-- delete this at some point
	ruby_init_loadpath();

	return true;
#endif // COMPILE_RUBY_SUPPORT
	return false;
}

static bool rubycore_module_can_unload(KviModule *)
{
#ifdef COMPILE_RUBY_SUPPORT
	return g_Interpreters.empty();
#endif
	return true;
}

static bool rubycore_module_cleanup(KviModule *)
{
#ifdef COMPILE_RUBY_SUPPORT
	g_Interpreters.clear();
#endif
	return true;
}

KVIRC_MODULE(
	"RubyCore",
	"4.9.2",
	"Copyright (C) 2016 Matt Ullman (staticfox at staticfox dot net)",
	"Ruby Scripting Engine Core",
	rubycore_module_init,
	rubycore_module_can_unload,
	rubycore_module_ctrl,
	rubycore_module_cleanup,
	"ruby"
)
