
# Implementation Overview

The core of this project is written in C++, with extensive Lua support.

The Lua code is contained solely within the file `lumail2.lua`, which is
expected to be located at `/etc/lumail2/lumail2.lua` on end-user systems.

The C++ code can all be found beneath the top-level `src/` directory.

For IMAP support I've chosen to delegate all operations to external perl
code, located beneath the top-level `perl.d` directory.  On a final
installation this will be located at `/etc/lumail2/perl.d`.


# C++ Overview

The C++ code is located beneath `src/` and consists of a collection of
classes, documented in the header-files, and implemented in the `.cc` files.

Each class has a `C` prefix, for example the class relating to screen-functionality
is named `CScreen`, and the class relating to file-operations is named `CFile`.

The main objects are:

* `CConfig`
    * A class to get/set configuration values.
* `CDirectory`
    * Directory primitives.
* `CLua`
    * For calling Lua code from C++.
* `CFile`
    * File-related primitives
* `CGlobalState`
    * Holding the currently selected message, messages, and maildir.
* `CHistory`
    * For storing input-history.
* `CMaildir`
    * Represents a folder of messages.
* `CMessage`
    * Represents a single message.
* `CMessagePart`
    * Represents and holdes MIME-data from a message.
* `CScreen`
    * Which implements the drawing code, and runs the main event-loop of the application.
* `CViewMode`
    * Is the base-class for each of the modal-modes.
    * Derived classes implement the actual mode.

The different view-modes are each derived from the `CViewMode` base-class,
and are implemented in `*_view.cc`.   The view-modes are instantied in the
`CScreen`-setup phase, and are selected by name.


# Design Patterns

We implement several [design patterns](https://en.wikipedia.org/wiki/Software_design_pattern):

* The [Singleton-pattern](https://en.wikipedia.org/wiki/Singleton_pattern):
     * The `CHistory`, `CInput`, and `CLogfile` classes are singletons.
     * These are implemented via the template class in `singleton.h`.
* The [Observer-pattern](https://en.wikipedia.org/wiki/Observer_pattern):
     * Changes to values stored in the `CConfig` class are broadcast to observers.
     * Observers currently include the `CGlobalState` and `CLua` objects.
     * The observer-pattern is implemented in `observer.h`.


Wrapping C++ to Lua
-------------------

The rule of thumb is that when we want to wrap a C++ object
we create a new file with a `_lua.cc` suffix.

So for example our configuration getter/setter class is `CConfig`,
which is implemented in `config.cc` and `config.h`.  The Lua wrapper to this
class is defined in `config_lua.cc`.

The wrappers are very simple, and follow a template.


Testing
-------

Regretably much of our code is not tested in a systematic fashion.

However there is some test-code which is built into our main binary,
to invoke it please run:

    $ lumail2 --test

Testing is accomplished via the CuTest library, as somethign I picked
on a whim many years ago and stuck with.  The implementation of our
tests is stored in files with a `_test.cc` suffix.
