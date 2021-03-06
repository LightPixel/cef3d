//----------------------------------------------------------------------------
//Yume Engine
//Copyright (C) 2015  arkenthera
//This program is free software; you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation; either version 2 of the License, or
//(at your option) any later version.
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//You should have received a copy of the GNU General Public License along
//with this program; if not, write to the Free Software Foundation, Inc.,
//51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.*/
//----------------------------------------------------------------------------
//
// File : <Filename> YumeGraphics.h
// Date : 2.19.2016
// Comments :
//
//----------------------------------------------------------------------------
#include "include/cef_render_handler.h"
#include "include/cef_app.h"
#include "include/cef_client.h"
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/wrapper/cef_helpers.h"

#include "cef3d/Cef3DRendererApp.h"

#include <Core/YumeFile.h>

int main(int argc,char* argv[]) {
#ifdef _WIN32
	CefMainArgs main_args(GetModuleHandle(NULL));
#else
	CefMainArgs main_args(argc,argv);
#endif
	using namespace YumeEngine;
	YumeFile file(YumeString("D:/Arken/C++/Yume/v2/YumeEngine/Engine/Assets/UI/Cef3D.js"));

	YumeString extension = file.ReadString();
	CefRefPtr<Cef3DRendererApp> app(new Cef3DRendererApp(extension.c_str()));
	return CefExecuteProcess(main_args,app.get(),nullptr);
}