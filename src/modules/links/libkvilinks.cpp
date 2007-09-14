//
//   File : libkvilinks.cpp
//   Creation date : Thu Dec 21 2000 12:39:12 CEST by Szymon Stefanek
//
//   Copyright (C) 1999-2001 Szymon Stefanek (pragma at kvirc dot net)
//
//   This program is FREE software. You can redistribute it and/or
//   modify it under the terms of the GNU General Public License
//   as published by the Free Software Foundation; either version 2
//   of the License, or (at your opinion) any later version.
//
//   This program is distributed in the HOPE that it will be USEFUL,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//   See the GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program. If not, write to the Free Software Foundation,
//   Inc. ,59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//


#include "kvi_module.h"


#include "linkswindow.h"

#include "kvi_frame.h"
#include "kvi_window.h"
#include "kvi_locale.h"
#include <QList>
#include <qsplitter.h>

QList<KviLinksWindow*> * g_pLinksWindowList = 0;


/*
	@doc: links.open
	@type:
		command
	@title:
		links.open
	@short:
		Opens a Links window
	@syntax:
		links.open
	@description:
		Opens a "links" window attacched to the current irc context.[br]
		The links window handles the RPL_LINKS server replies and shows
		them in a Tree-View form: this is useful in vizualizing the
		current network connections.[br]
		Please note that the total number of links received (and the
		total count of servers displayed when all the links have been received)
		may actually be less than the real number of irc servers in the network.
		Servers that contain a wildcard in their name often act as gateways (hubs)
		for a "local irc network"; if you're requesting links from a server that is
		external to the gateway, the servers internal to the network "behind the gateway"
		will not be shown; in the extreme case you will see the gateway as leaf node (and it isn't).
		To see the internal network server tree you might request LINKS from the gateway server.[br]
*/


static bool links_kvs_cmd_open(KviKvsModuleCommandCall * c)
{ 
	
	// FIND ME
	if(!c->window()->console())return c->context()->errorNoIrcContext();

	if(!(c->window()->console()->ircContext()->linksWindow()))
	{
		KviLinksWindow *w = new KviLinksWindow(c->window()->frame(),c->window()->console());
		c->window()->frame()->addWindow(w);
	} else {
		c->warning(__tr2qs("Links window alread open for this IRC context"));
	}

	return true;
}




static bool links_module_init(KviModule * m)
{
	g_pLinksWindowList = new QList<KviLinksWindow*>;


	KVSM_REGISTER_SIMPLE_COMMAND(m,"open",links_kvs_cmd_open);

	return true;
}

static bool links_module_cleanup(KviModule *m)
{
	while(!g_pLinksWindowList->isEmpty())g_pLinksWindowList->first()->die();
	delete g_pLinksWindowList;
    g_pLinksWindowList = 0;
	return true;
}

static bool links_module_can_unload(KviModule *m)
{
	return (g_pLinksWindowList->isEmpty());
}

KVIRC_MODULE(
	"Links",                                             // module name
	"1.0.0",                                                // module version
	"Copyright (C) 2000-2001 Szymon Stefanek (pragma at kvirc dot net)", // author & (C)
	"Links window extension",
	links_module_init,
	links_module_can_unload,
	0,
	links_module_cleanup
)
