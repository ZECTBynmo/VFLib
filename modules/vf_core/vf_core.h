/*============================================================================*/
/*
  Copyright (C) 2008 by Vinnie Falco, this file is part of VFLib.
  See the file GNU_GPL_v2.txt for full licensing terms.

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by the Free
  Software Foundation; either version 2 of the License, or (at your option)
  any later version.

  This program is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
  details.

  You should have received a copy of the GNU General Public License along with
  this program; if not, write to the Free Software Foundation, Inc., 51
  Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
/*============================================================================*/

/** Include this to get the @ref vf_core module.

    @file vf_core.h
    @ingroup vf_core
*/

#ifndef VF_CORE_VFHEADER
#define VF_CORE_VFHEADER

//==============================================================================
/**
  @mainpage VFLib: A multipurpose library built on JUCE.

  @details

  VFLib is a source code collection of individual \ref modules containing
  functionality for a variety of applications, with an emphasis on building
  concurrent systems. VFLib requires [JUCE][3] (Jules' Utility Class
  Extensions), available from [Raw Material Software][4]. JUCE is available
  under both the [GNU General Public License][5] and a [commercial license][6].
  Other than JUCE, VFLib has no external dependencies.

  VFLib is hosted on Github at [https://github.com/vinniefalco/VFLib][1]

  The online documentation is at [http://vinniefalco.github.com/VFLib][2]

  ## Platforms

  All platforms supported by JUCE are also supported by VFLib. Currently these
  platforms include:

  - **Windows**: Applications and VST/RTAS/NPAPI/ActiveX plugins can be built 
    using MS Visual Studio. The results are all fully compatible with Windows
    XP, Vista or Windows 7.

  - **Mac OS X**: Applications and VST/AudioUnit/RTAS/NPAPI plugins with Xcode.

  - **GNU/Linux**: Applications and plugins can be built for any kernel 2.6 or
    later.

  - **iOS**: Native iPhone and iPad apps.

  - **Android**: Supported.

  ## Prerequisites

  This documentation assumes that the reader has a working knowledge of JUCE.
  Some modules built on external libraries assume that the reader understands
  the operation of those external libraries. Certain modules assume that the
  reader understands additional domain-specific information. Modules with
  additional prerequisites are marked in the documentation.

  ## External Modules

  Some modules bring in functionality provided by external libraries. For
  example, the \ref vf_bzip2 module provides the compression and decompression
  algorithms in [bZip2][7]. Usage of these external library modules is optional.
  They come with complete source code, as well as options for using either
  system or user provided variants of the external libraries: it is not
  necessary to download additional source code packages to use these modules.

  External code incorporated into VFLib is covered by separate licenses. See
  the licensing information and notes in the corresponding source files for
  copyright information and terms of use.

  ## Integration

  VFLib requires recent versions of JUCE. It won't work with versions 1.53 or
  earlier. To use the library it is necessary to first download JUCE to a
  location where your development environment can find it. Or, you can use your
  existing installation of JUCE.

  This library uses the same modularized organizational structure as JUCE. To
  use a module, first add a path to the list of includes searched by your
  development environment or project, which points to the VFLib directory. Then,
  add the single corresponding .c or .cpp file to your existing project which
  already uses JUCE. For example, to use the \ref vf_core module, add the file
  vf_core.cpp to your project. Some modules depend on other modules. All modules
  depend on \ref vf_core.

  To use a module, include the appropriate header from within your source code.
  For example, to access classes in the \ref vf_concurrent module, use this:

      #include "modules/vf_concurrent/vf_concurrent.h"

  ## AppConfig

  Some VFLib features can be controlled at compilation time through
  preprocessor directives. The available choices of compilation options are
  described in AppConfig.h, located in the AppConfigTemplate directory. Copy
  the provided settings into your existing AppConfig.h (a file used by JUCE
  convention).

  ## License

  Copyright (C) 2008 by Vinnie Falco ([email][0])

  Unless otherwise marked, files in VFLib are published under the [GNU General
  Public License][5].  This program is distributed in the hope that it will be
  useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

  Commercial, closed source licenses for VFLib are available, [contact the
  author][0] for details.

  Some files contain portions of these external projects, licensed separately:

  - [bZip2][7] is Copyright (C) 1996-2010 Julian R Seward. All rights
    reserved. See the corresponding file LICENSE for licensing terms.

  - Portions of the software are Copyright (C) 1996-2001, 2006 by [The FreeType
    Project][8]. All rights reserved. [FreeType][8] is distributed
    under both the [GNU General Public License][5], or the
    [FreeType License][9].

  - Portions of this software are Copyright (C) 1994-2012 [Lua.org][10], PUC-Rio.
    Lua is distributed under the terms of the [MIT License][11].

  - [Luabridge][12] is Copyrighted (C) 2007 by Nathan Reed. [Luabridge][12] is
    distributed under the terms of the [MIT License][11].

  - [Soci][13] is Copyright (C) 2004-2008 Maciej Sobczak, Stephen Hutton, and
    various others noted in the corresponding source files. Soci is distributed
    under the [Boost Software License, Version 1.0][14].

  - [SQLite][15], placed in the public domain.

  - [TagLib][16] is distributed under both the [GNU Lesser General Public License,
  Version 2.1][17] and the [Mozilla Public License][18].

  [0]: mailto:vinnie.falco@gmail.com "Vinnie Falco (Email)"
  [1]: https://github.com/vinniefalco/VFLib "VFLib Project"
  [2]: http://vinniefalco.github.com/VFLib/ "VFLib Documentation"
  [3]: http://rawmaterialsoftware.com/juce.php "JUCE"
  [4]: http://rawmaterialsoftware.com/ "Raw Material Software"
  [5]: http://www.gnu.org/licenses/gpl-2.0.html "GNU General Public License, version 2"
  [6]: http://rawmaterialsoftware.com/jucelicense.php "JUCE Licenses"
  [7]: http://www.bzip.org/ "bZip2: Home"
  [8]: http://freetype.org/ "The FreeType Project"
  [9]: http://www.freetype.org/FTL.TXT "The FreeType Project License"
  [10]: http://www.lua.org/ "The Programming Language Lua"
  [11]: http://www.opensource.org/licenses/mit-license.html "The MIT License"
  [12]: https://github.com/vinniefalco/LuaBridge
  [13]: http://soci.sourceforge.net/ "SOCI"
  [14]: http://www.boost.org/LICENSE_1_0.txt "Boost Software License, Version 1.0"
  [15]: http://sqlite.org/ "SQLite Home Page"
  [16]: http://developer.kde.org/~wheeler/taglib.html "TagLib"
  [17]: http://www.gnu.org/licenses/lgpl-2.1.html "Gnu Lesser General Public License, version 2.1"
  [18]: http://www.mozilla.org/MPL/1.1/ "Mozilla Public License"

  @author Vinnie Falco (<a href="mailto:vinnie.falco@gmail.com">email</a>)
  @version 0.9.0 pre-release
  @date 2008
  @copyright Copyright (C) 2008 by Vinnie Falco.
  @copyright Provided under the [GNU General Public License][5]
*/

/*============================================================================*/
/**
  @internal

  Implementation classes.

  Thase classes are used internally.

  @defgroup internal internal
*/

/*============================================================================*/
/**
  External modules.

  These modules bring in functionality from third party or system libraries.

  @defgroup external external
*/

/*============================================================================*/
/**
  Core classes.

  This module provides core required functionality, and classes useful for
  general development. All other modules require this module.

  @todo Discuss the treatment of exceptions versus Error objects in the library.

  @todo Discuss the additions to AppConfig.h

  @defgroup vf_core vf_core
*/

/*  See the Juce notes regarding AppConfig.h

    This file must always be included before any Juce headers.

    There are some VFLib specific build options that may be placed
    into this file. See the AppConfig.h provided with VFLib.
*/

/* AppConfig.h must be included before this file */

/* Use sensible default configurations if they forgot
   to append the necessary macros into their AppConfig.h.
*/
#ifndef VF_USE_BOOST
#define VF_USE_BOOST      0
#endif

#ifndef VF_USE_BZIP2
#define VF_USE_BZIP2      0
#endif

#ifndef VF_USE_FREETYPE
#define VF_USE_FREETYPE   0
#endif

#ifndef VF_USE_NATIVE_FREETYPE
#define VF_USE_NATIVE_FREETYPE 1
#endif

#ifndef VF_USE_NATIVE_SQLITE
#define VF_USE_NATIVE_SQLITE 1
#endif

#ifndef VF_USE_LEAKCHECKED
#define VF_USE_LEAKCHECKED JUCE_CHECK_MEMORY_LEAKS
#endif

/* Get this early so we can use it. */
#include "modules/juce_core/system/juce_TargetPlatform.h"

// Handy macro that lets pragma warnings be clicked in the output window
// Usage: #pragma message(VF_LOC_"Advertise here!")
#define VF_STR2_(x) #x
#define VF_STR1_(x) VF_STR2_(x)
#define VF_LOC_ __FILE__ "("VF_STR1_(__LINE__)") : WARNING: "

// Turns vflib specific debugging support on or off
//
#ifndef VF_DEBUG
#define VF_DEBUG JUCE_DEBUG // get the flag from Juce
#endif

#if VF_USE_BOOST
#include <boost/thread/tss.hpp>
#endif

#if JUCE_MSVC
# include <crtdbg.h>
# include <functional>
#elif JUCE_IOS || JUCE_MAC
# if VF_USE_BOOST
#  include <boost/bind.hpp>
#  include <boost/function.hpp>
# else
#  include <tr1/functional>
# endif
#else
# include <bind>
# include <functional>
#endif

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <cstdarg>
#include <cstddef>
#include <exception>
#include <istream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <new>
#include <numeric>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <vector>

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <locale.h>
#include <math.h>
#include <memory.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Includes Juce

#ifdef _CRTDBG_MAP_ALLOC
#error "MSVC C Runtime Debug Macros not supported"
#endif

// If the MSVC debug heap headers were included, disable
// the macros during the juce include since they conflict.
#ifdef _CRTDBG_MAP_ALLOC
  #include <crtdbg.h>
  #include <stdlib.h>
  #include <malloc.h>

  #pragma push_macro("calloc")
  #pragma push_macro("free")
  #pragma push_macro("malloc")
  #pragma push_macro("realloc")
  #pragma push_macro("_recalloc")
  #pragma push_macro("_aligned_free")
  #pragma push_macro("_aligned_malloc")
  #pragma push_macro("_aligned_offset_malloc")
  #pragma push_macro("_aligned_realloc")
  #pragma push_macro("_aligned_recalloc")
  #pragma push_macro("_aligned_offset_realloc")
  #pragma push_macro("_aligned_offset_recalloc")
  #pragma push_macro("_aligned_msize")

  #undef calloc
  #undef free
  #undef malloc
  #undef realloc
  #undef _recalloc
  #undef _aligned_free
  #undef _aligned_malloc
  #undef _aligned_offset_malloc
  #undef _aligned_realloc
  #undef _aligned_recalloc
  #undef _aligned_offset_realloc
  #undef _aligned_offset_recalloc
  #undef _aligned_msize
#endif

#include "modules/juce_core/juce_core.h"

#ifdef _CRTDBG_MAP_ALLOC
  #pragma pop_macro("_aligned_msize")
  #pragma pop_macro("_aligned_offset_recalloc")
  #pragma pop_macro("_aligned_offset_realloc")
  #pragma pop_macro("_aligned_recalloc")
  #pragma pop_macro("_aligned_realloc")
  #pragma pop_macro("_aligned_offset_malloc")
  #pragma pop_macro("_aligned_malloc")
  #pragma pop_macro("_aligned_free")
  #pragma pop_macro("_recalloc")
  #pragma pop_macro("realloc")
  #pragma pop_macro("malloc")
  #pragma pop_macro("free")
  #pragma pop_macro("calloc")
#endif

using juce::int64;
using juce::uint64;
using juce::uint32;
using juce::uint8;

namespace vf
{

using namespace juce;

#define vfassert jassert

}

namespace vf
{

#include "memory/vf_Uncopyable.h"

#include "containers/vf_List.h"
#include "containers/vf_LockFreeStack.h"
#include "containers/vf_LockFreeQueue.h"
#include "containers/vf_SortedLookupTable.h"

#include "diagnostic/vf_CatchAny.h"
#include "diagnostic/vf_Debug.h"
#include "diagnostic/vf_Error.h"
#include "diagnostic/vf_FPUFlags.h"
#include "diagnostic/vf_LeakChecked.h"
#include "diagnostic/vf_SafeBool.h"
#include "diagnostic/vf_Throw.h"

#include "events/vf_OncePerSecond.h"
#include "events/vf_PerformedAtExit.h"

#include "functor/vf_Bind.h"
#include "functor/vf_Function.h"

#include "math/vf_Interval.h"
#include "math/vf_Math.h"
#include "math/vf_MurmurHash.h"

#include "memory/vf_AtomicCounter.h"
#include "memory/vf_AtomicFlag.h"
#include "memory/vf_AtomicPointer.h"
#include "memory/vf_AtomicState.h"

#if JUCE_MSVC
#pragma warning (push)
#pragma warning (disable: 4100) // unreferenced formal parmaeter
#pragma warning (disable: 4355) // 'this' used in base member
#endif
#include "memory/vf_CacheLine.h"
#if JUCE_MSVC
#pragma warning (pop)
#endif

#include "memory/vf_MemoryAlignment.h"
#include "memory/vf_ReferenceCountedSingleton.h"
#include "memory/vf_StaticObject.h"

#include "threads/vf_SpinDelay.h"
#include "threads/vf_InterruptibleThread.h"

}

#endif

