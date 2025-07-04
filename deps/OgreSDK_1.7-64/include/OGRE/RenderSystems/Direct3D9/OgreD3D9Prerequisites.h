/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2011 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#ifndef __D3D9PREREQUISITES_H__
#define __D3D9PREREQUISITES_H__

#include "OgrePrerequisites.h"
#ifdef __MINGW32__
#  include "WIN32/OgreMinGWSupport.h" // extra defines for MinGW to deal with DX SDK
#endif

#if OGRE_THREAD_SUPPORT
#define OGRE_LOCK_RECURSIVE_MUTEX(name)   name.lock();
#define OGRE_UNLOCK_RECURSIVE_MUTEX(name) name.unlock();
#else
#define OGRE_LOCK_RECURSIVE_MUTEX(name) 
#define OGRE_UNLOCK_RECURSIVE_MUTEX(name)
#endif


#if OGRE_THREAD_SUPPORT == 1
#define D3D9_DEVICE_ACCESS_LOCK				OGRE_LOCK_RECURSIVE_MUTEX(msDeviceAccessMutex);
#define D3D9_DEVICE_ACCESS_UNLOCK			OGRE_UNLOCK_RECURSIVE_MUTEX(msDeviceAccessMutex);
#define D3D9_DEVICE_ACCESS_CRITICAL_SECTION	OGRE_LOCK_MUTEX(msDeviceAccessMutex)
#else
#define D3D9_DEVICE_ACCESS_LOCK	
#define D3D9_DEVICE_ACCESS_UNLOCK
#define D3D9_DEVICE_ACCESS_CRITICAL_SECTION
#endif

// Define versions for if DirectX is in use (Win32 only)
#define DIRECT3D_VERSION 0x0900

// some D3D commonly used macros
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

// enable extended d3d debug 
#if OGRE_DEBUG_MODE
#	define D3D_DEBUG_INFO
#endif

#define WIN32_LEAN_AND_MEAN
#if !defined(NOMINMAX) && defined(_MSC_VER)
#	define NOMINMAX // required to stop windows.h messing up std::min
#endif
#include <d3d9.h>
#include <d3dx9.h>
#include <DxErr.h>


namespace Ogre
{
	// Predefine classes
	class D3D9RenderSystem;
	class D3D9RenderWindow;
	class D3D9Texture;
	class D3D9TextureManager;
	class D3D9Driver;
	class D3D9DriverList;
	class D3D9VideoMode;
	class D3D9VideoModeList;
	class D3D9GpuProgram;
	class D3D9GpuProgramManager;
    class D3D9HardwareBufferManager;
    class D3D9HardwareIndexBuffer;
    class D3D9HLSLProgramFactory;
    class D3D9HLSLProgram;
    class D3D9VertexDeclaration;
	class D3D9Resource;

// Should we ask D3D to manage vertex/index buffers automatically?
// Doing so avoids lost devices, but also has a performance impact
// which is unacceptably bad when using very large buffers
#define OGRE_D3D_MANAGE_BUFFERS 1

    //-------------------------------------------
	// Windows setttings
	//-------------------------------------------
#if(OGRE_PLATFORM == OGRE_PLATFORM_WIN32) && !defined(OGRE_STATIC_LIB)
#	ifdef OGRED3DENGINEDLL_EXPORTS
#		define _OgreD3D9Export __declspec(dllexport)
#	else
#       if defined( __MINGW32__ )
#           define _OgreD3D9Export
#       else
#    		define _OgreD3D9Export __declspec(dllimport)
#       endif
#	endif
#else
#	define _OgreD3D9Export
#endif	// OGRE_WIN32
}
#endif
