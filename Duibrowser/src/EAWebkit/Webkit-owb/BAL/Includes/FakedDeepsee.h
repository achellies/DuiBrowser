//Faked deepsee.h:
//	generated on: vendredi 22 février 2008, 16:45:07 (UTC+0100)
//	from sources (.*\.cpp$\|.*\.h$\|.*\.cxx$\|.*\.hxx$) found under: /home/fred/src/deepsee/src
//	by script: ./deepsee_generateFakedHeader.sh
//To regenerate this header, just re-run ./deepsee_generateFakedHeader.sh, or delete the file and re-configure cmake if it uses the DeepSee block.
//
//You can get a fresh DeepSee from svn at http://www.sand-labs.org/deepsee/svn/trunk/
//And visit DeepSee project homepage at http://www.sand-labs.org/deepsee/
//

/*
    This header is generated directly from DeepSee sources, so all current (2008-02-22) macros from the API are stubbed here.
    We needed this because we want to offer our users basic logs even if they are not (yet) using DeepSee.
    For exemple, even if DeepSee is disabled, DS_ASS
*/

/*
* This file was modified by Electronic Arts Inc Copyright © 2009-2010
*/
#ifndef FAKEDDEEPSEE_H
#define FAKEDDEEPSEE_H


//if we don't use DeepSee, but want to redefine few macros instead of stubbing them
#ifndef OWB_FAILSAFE_WITHOUT_DEEPSEE
    #define OWB_FAILSAFE_WITHOUT_DEEPSEE 1
#endif

#if OWB_FAILSAFE_WITHOUT_DEEPSEE

    //the file that defines ASSERT, in JavaScriptCore/wtf/Assertions.h
    #include "wtf/Assertions.h"

    #define DS_ASS(var1,cond,var2) ASSERT(var1 cond var2)
    #define DS_ASS_LOW(var1,cond,var2) ASSERT(var1 cond var2)
    #define DS_ASS_PTR(ptr) ASSERT(ptr)

#else

    #define DS_ASS(args, ...)
    #define DS_ASS_LOW(args, ...)
    #define DS_ASS_PTR(args, ...)

#endif


//all other macros are stubbed

#define DS_BACK_TRACE(args, ...)
#define DS_BACK_TRACE_SKIP(args, ...)
#define DS_CLEAN_DEEPSEE_FRAMEWORK(...)
#define DS_CONSTRUCT(...)
#define DS_DEFINE_CONSOLE_PTR(args, ...)
#define DS_DELETE_CONSOLE(args, ...)
#define DS_DESTRUCT(...)
#define DS_ERR(args, ...)
#define DS_GET_USEDMEMORYSIZE(args, ...)
#define DS_INIT_DEEPSEE_FRAMEWORK(...)
#define DS_INSTANCE_COUNT_ALLOCATE(args, ...)
#define DS_INST_CHECK_LEAK(args, ...)
#define DS_INST_CHECK_LEAK_FOR_CLASS(args, ...)
#define DS_INST_DIFF(args, ...)
#define DS_INST_DIFF_TO_SNAPSHOT(args, ...)
#define DS_INST_DUMP_CURRENT(args, ...)
#define DS_INST_DUMP_SNAPSHOT(args, ...)
#define DS_INST_GET_CURRENT_NB(args, ...)
#define DS_INST_GET_SNAPSHOT_NB(args, ...)
#define DS_INST_REMOVE_SNAPSHOT(args, ...)
#define DS_INST_TAKE_SNAPSHOT(args, ...)
#define DS_LOGGER_ADD_CMD_TO_CONSOLE(args, ...)
#define DS_LOGGER_CLEAR_FILTERS(args, ...)
#define DS_LOGGER_DUMP(args, ...)
#define DS_LOGGER_SET_FILENAME_HEAD_TO_REMOVE(args, ...)
#define DS_MEMORY_ADD_CMD_TO_CONSOLE(args, ...)
#define DS_MEMORY_CHECK_LEAK(args, ...)
#define DS_MEMORY_LEAK(args, ...)
#define DS_MEMORY_TAKE_SNAPSHOT_TO_STACK(args, ...)
#define DS_MSG(args, ...)
#define DS_NEW_CONSOLE(args, ...)
#define DS_SIGNAL_CATCH_ALL(args, ...)
#define DS_SIGNAL_CATCH(args, ...)
#define DS_SRR(args, ...)
#define DS_STACK_GET_USEDMEMORYSIZE(args, ...)
#define DS_TIMER_(args, ...)
#define DS_TIMER_DISPLAY(args, ...)
#define DS_TIMER_DISPLAY_DIFF(args, ...)
#define DS_TIMER_STACK_CREATE(args, ...)
#define DS_TIMER_STACK_GET(args, ...)
#define DS_TIMER_STACK_RESET(args, ...)
#define DS_TIMER_STACK_SET(args, ...)
#define DS_TIMER_STATIC_DISPLAY(args, ...)
#define DS_TIMER_STATIC_GET(args, ...)
#define DS_TIMER_STATIC_MAX(args, ...)
#define DS_TIMER_STATIC_RESET(args, ...)
#define DS_TIMER_STATIC_SET(args, ...)
#define DS_TIMER_STATIC_SUM_DISPLAY(args, ...)
#define DS_TIMER_STATIC_SUM_END(args, ...)
#define DS_TIMER_STATIC_SUM_GET(args, ...)
#define DS_TIMER_STATIC_SUM_RESET(args, ...)
#define DS_TIMER_STATIC_SUM_START(args, ...)
#define DS_TRC(args, ...)
#define DS_USE_CONSOLE(args, ...)
#define DS_WAR(args, ...)
#define DS_WAR_IF(args, ...)
#define DS_WAR_IF_novalue(args, ...)
#define DS_WAR_RAW(args, ...)


#endif

