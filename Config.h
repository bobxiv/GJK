#pragma once

#ifndef CONFIG_H
#define CONFIG_H

//--------------------------------------------------------------------------
// Inlining macro
//--------------------------------------------------------------------------

// Uncomment or define FORCE_INLINE to use __forceinline
// #define FORCE_INLINE

#if defined(DEBUG)
#define INLINE inline
#else
	#if defined(FORCE_INLINE)
	#define INLINE __forceinline
	#else
	#define INLINE inline
	#endif
#endif

//--------------------------------------------------------------------------
// Primitives types macro
//--------------------------------------------------------------------------

#include "PrimitiveTypes.h"

#include "HelpMath.h"

#endif // end CONFIG_H