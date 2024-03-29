/* Camsource module interface */
/* $Id: module.h,v 1.12 2002/09/25 22:37:22 dfx Exp $ */

#ifndef _MODULE_H_
#define _MODULE_H_

#include <pthread.h>
#include <libxml/parser.h>

/*
 * There are three kinds of modules:
 * .) MODULE_THREAD is a worker module which runs in its own thread.
 *    A thread module has a thread() function which will be run
 *    in its own thread. The thread() function will get a pointer
 *    to the module context structure (casted to void*), containing
 *    the xml config structure, the thread id and the custom pointer
 *    (see below in the part about init()). Note that you should not
 *    use any global vars in thread modules, as the same module may
 *    be activated multuple times, and hence thread() may run several
 *    times simultaneously.
 * .) MODULE_FILTER is an image filter module. It provides a filter()
 *    function that takes an image as input and outputs another one.
 *    Filtering happens in-place. A pointer to the xml config
 *    tree is passed to the filter function (so that node->name ==
 *    "filter").
 * .) MODULE_GENERIC is a module which doesn't do anything by itself,
 *    but provides special functionality for other modules. It is
 *    usually listed as dependency in other modules.
 * You must define at least one of the above before including this
 * file (module.h). The brave can even define multiple of them,
 * for example a module that has both its own thread and provides
 * a filter.
 *
 * Things common to all kinds of modules:
 * .) The module "name". It must match the filename, e.g. a module
 *    called "libhello.so" would have a name of "hello".
 * .) A "version" string, containing the version number of the lib.
 *    Each initialized module will be printed in the log together
 *    with its version when it's loaded (if the version info is
 *    present that is; it's optional).
 * .) Dependency list ("deps"). It's a null terminated array of
 *    strings, each giving the name of another module which will
 *    be autoloaded before the current module is activated.
 * .) An optional init() function. If present, it will be called
 *    when the module is loaded. As argument, it gets a pointer
 *    to a module context structure. It contains a pointer to
 *    its xml config (which may be NULL, if the module was loaded
 *    as dependency and there's no mention of it in the config)
 *    and the thread id var (filled in later when the thread
 *    is started). The init() function can put a pointer
 *    to a custom (dynamically allocated) structure into the "user"
 *    struct member. Note that the thread-id and custom pointer
 *    only make sense for a thread module. Returning anything but 0
 *    from init() means module init has failed. You may also omit
 *    init() altogether. Note: do not create threads from init().
 */


struct module_ctx;

extern char *name;
extern char *version;
extern char *deps[];
int init(struct module_ctx *);


#ifdef MODULE_THREAD

void *thread(void *);

#endif	/* MODULE_THREAD */



#ifdef MODULE_FILTER

struct image;
int filter(struct image *, xmlNodePtr);

#endif	/* MODULE_FILTER */





#if !defined(MODULE_THREAD) && !defined(MODULE_FILTER) && !defined(MODULE_GENERIC) && !defined(MODULE_NONE)
# error "Must define the module type prior to including module.h"
#endif	/* !def && !def && !def */

#endif

