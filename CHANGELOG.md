# NEXT RELEASE

### Bugfixes

* Fix a race involving destruction order of InterprocessMutex static variables.
* Fix a crash when a Query is reimported into the SharedGroup it was exported
  for handover from.
* Fix a crash when calling mkfifo on Android 4.x external storage. On 4.x devices,
  errno is EPERM instead of EACCES.

### Breaking changes

* Lorem ipsum. 

### Enhancements

* Added support for LIKE queries (wildcard with `?` and `*`)
* Offer facilities to prevent multiple sync agents per Realm file access session
  (`Replication::is_sync_agent()` to be overridden by sync-specific
  implementation). The utilized lock-file flag
  (`SharedInfo::sync_agent_present`) was added a long time ago, but the
  completion of detection mechanism got postponed until now.

-----------

### Internals

* Lorem ipsum.

----------------------------------------------

# 2.1.4 Release notes

### Bugfixes

* Fix storage of very large refs (MSB set) on 32-bit platforms.
* Fixed a race between destruction of a global mutex as part of main thread exit
  and attempt to lock it on a background thread, or conversely attempt to lock a
  mutex after it has been destroyed. (PR #2238, fixes issues #2238, #2137, #2009)

----------------------------------------------

# 2.1.3 Release notes

### Bugfixes

* Deleting rows through a `TableView` generated wrong instructions by way of
  `Table::batch_erase_rows()`, which would only be noticed after reapplying the
  transaction log to a separate Realm file or via synchronization.

-----------

### Internals

* `array_direct.hpp` added to installed headers.

----------------------------------------------

# 2.1.2 Release notes

### Bugfixes

* When adding a nullable column of type Float while other columns existed
  already, the values of the new column would be non-null. This is now fixed.

----------------------------------------------

# 2.1.1 Release notes

### Internals

* Upgraded to OpenSSL 1.0.2j.

----------------------------------------------

# 2.1.0 Release notes

### Bugfixes

* Fix an assertion failure when upgrading indexed nullable int columns to the
  new index format.
* Extra SetUnique instructions are no longer generated in the transaction log
  when a conflict was resolved locally.

### Breaking changes

* The ChangeLinkTargets instruction was a misnomer and has been renamed to
  MergeRows.

-----------

### Internals

* Android builds: upgraded to OpenSSL 1.0.1u.
* The behavior of MergeRows (formerly ChangeLinkTargets) has been simplified to
  be semantically equivalent to a row swap.

----------------------------------------------

# 2.0.0 Release notes

### Bugfixes

* TimestampColumn::is_nullable() could return a wrong value. Also, when adding a new
  Float/Double column to a Table with rowcount > 0, the new entries would be non-null
  even though the column was created as nullable.
* Update accessors after a change_link_target or set_unique operation, so that users
  will have the latest data immediately. Previously this would require manually
  refetching the data or looking for the unique key again.

----------------------------------------------

# 2.0.0-rc9 Release notes

### Internals

* Use Xcode 7.3.1 to build core for Cocoa

----------------------------------------------

# 2.0.0-rc8 Release notes

### Bugfixes

* Fixed a crash related to queries that was introduced in rc7. (#2186)
* Fixed a bug triggered through set unique of primary keys through
  the ROS. (#2180)
  
-----------

### Internals

* Optimized query code on a string column with a search index to address a
  performance regression observed in the recent format changes to the
  string index (see #2173)

----------------------------------------------

# 2.0.0-rc7 Release notes

### Bugfixes

* Fixed a race in the handover machinery which could cause crashes following handover
  of a Query or a TableView. (#2117)
* Reversed the decision process of resolving primary key conflicts. Instead of
  letting the newest row win, the oldest row will now always win in order to not
  lose subsequent changes.

-----------

### Breaking changes

* Changed the format of the StringIndex structure to not recursivly store
  strings past a certain depth. This fixes crashes when storing strings
  with a long common prefix in an index. This is a file format breaking change.
  The file format has been incremented and old Realm files must upgrade.
  The upgrade will rebuild any StringIndexes to the new format automatically
  so other than the upgrade, this change should be effectivly invisible to
  the bindings. (see #2153)

-----------

### Internals

* Removed ("deleted") the default copy constructor for RowBase. This constructor
  was used by accident by derived classes, which led to a data race. Said race was
  benign, but would be reported by the thread sanitizer.

----------------------------------------------

# 2.0.0-rc6 Release notes

### Enhancements

* Added debian packages for Ubuntu 16.04.

----------------------------------------------

# 2.0.0-rc4 Release notes

### Bugfixes

* Fixed a bug where find() on a Query constructed from a restricting view
  did not correctly return an row index into the underlying table.
  (issue #2127)
* Fixed a bug where linked tables were not updated after a table move operation, when
  run through the replicator.
* Fixed a bug where moving a column to itself caused a crash.

### Breaking changes

* New instruction for `Table::add_int()`, which impacts the transaction log
  format.

### Enhancements

* Added `Table::add_int()` for implementing CRDT counters.

----------------------------------------------

# 2.0.0-rc3 Release notes

### Bugfixes

* Fixed a bug with link columns incorrectly updating on a `move_last_over`
  operation when the link points to the same table.
* Fix subspecs not updating properly after a move operation.
* Fixed various crashes when using subtables. The crash will occur when the first column
  of the subtable if of type `col_type_Timestamp` or if it is nullable and of type Bool, Int
  or OldDateTime. Caused by bad static `get_size_from_ref()` methods of columns. (#2101)
* Fixed a bug with link columns incorrectly updating on a `move_last_over`
  operation when the link points to the same table.

### Breaking changes

* Refactored the `SharedGroup` constructors and open methods to use a new
  `SharedGroupOptions` parameter which stores all options together.
* BREAKING! Until now, a Query would return indexes into a restricting view if such was 
  present (a view given in the `.where(&view) method`, or it would return indexes into the
  Table if no restricting view was present. This would make query results useless if you did 
  not know whether or not a restricting view was present. This fix make it *always* return 
  indexes into the Table in all cases. Also, any `begin` and `end` arguments could point into 
  eitherthe View or the Table. These now always point into the Table. Also see 
  https://github.com/realm/realm-core/issues/1565

### Enhancements

* Accessors pointing to subsumed rows are updated to the new row rather than detached.

-----------

### Internals

* When creating a `SharedGroup`, optionally allow setting the temporary 
  directory to when making named pipes fails. This is to fix a bug
  involving mkfifo on recent android devices (#1959).
* Bug fixed in test harness: In some cases some tests and checks would be
  counted twice due to counters not being reset at all the right times.

----------------------------------------------

# 2.0.0-rc2 Release notes

### Enhancements

* Add back log level prefixes for `StderrLogger` and `StreamLogger`

----------------------------------------------

# 2.0.0-rc1 Release notes

### Breaking changes

* API Breaking change: Added log level argument to util::Logger::do_log().
  Existing implementations can ignore the argument, or use it to add log level
  info to the log output.
* API Breaking change: The WriteLogCollector is no longer available.
  To create a history object for SharedGroup, make_in_realm_history()
  must now be used instead of make_client_history().
* The commit logs have been moved into the Realm file. This means we no longer
  need the .log_a, .log_b and .log files, significantly reducing the number of
  both files and open file handles. This is a breaking change, since versions
  without .log files cannot interoperate with earlier versions which still
  uses separate .log files. (issues #2065, #1354).
* The version for .lock-file data has been bumped to reflect that this is
  an API breaking change.

### Enhancements

* Elimination of the .log files also eliminates all locking related to
  accessing  the .log files, making read-transactions lock-free.
* The critical phase of commits have been reduced significantly in length.
  If a process is killed while in the critical phase, any other process
  working jointly on the same Realm file is barred from updating the Realm
  file until the next session. Reducing the length of the critical phase
  reduces the risk of any user experiencing this limitation.
  (issues #2065, #1354)

-----------

### Internals

* Added support for very large commit history entries. (issues #2038, #2050)
  This also implies an API change (but to the internal API) to the
  History::get_changesets() method, which must be taken into account by
  any derived classes.
* Support for setting and getting thread names (`util::Thread::set_name()` and
  `util::Thread::get_name()`) when the platform supports
  it. `util::Thread::set_name()` is now used by the test harness as a help while
  debugging. Also, the terminate handler (in `util/terminate.cpp`) writes out
  the name of the terminating thread if the name is available.
* Fixed doxygen warnings.

----------------------------------------------

# 2.0.0-rc0 Release notes

### Internals

* Changed instruction log format of Set instructions to be more amenable to the
addition of future variants.
* Changed instruction log format of LinkList instructions to include information
about the size of the list in question prior to carrying out the instruction.

----------------------------------------------

# 1.5.1 Release notes

### Bugfixes

* Fixed java bug #3144 / Core #2014. Management of Descriptor class was
  not thread safe with respect to destruction/creation/management of
  accessor tree. Bug could be triggered by destruction of TableView on
  one thread, while new TableViews where created on a different thread.
* Fixed incorrect results when updating a backlink TableView after inserting
  new columns into the source table.

----------------------------------------------

# 1.5.0 Release notes

### Bugfixes

* Fix a race condition that could result in a crash if a `LinkView` was
  destroyed while another thread was adjusting accessors on a `LinkListColumn`.
* Fix crashes and general brokenness when constructing a Query, inserting a
  column into the queried table, and then running the query.
* Fix crashes and general brokenness when syncing a sorted or distincted
  TableView after inserting new columns into the source Table.

### Breaking changes

* Added support for sorting and distincting table views through a chain of
  links. (#1030)

### Enhancements

* Improve performance of sorting on non-nullable columns.
* Improve overall sort performance.

-----------

### Internals

* Updated the header license to prepare for open sourcing the code.

----------------------------------------------

# 1.4.2 Release notes

### Bugfixes

* Fix a bug with the local mutex for the robust mutex emulation.
* Reduce the number of file descriptors used in robust mutex emulation,
  multi instances of InterprocessMutex share the same descriptor. (#1986)

----------------------------------------------

# 1.4.1 Release notes

### Bugfixes

* Fixing util::MemoryInputStream to support tellg() and seekg().
* Fix truncation of the supplied value when querying for a float or double that
  is less than a column's value.
* Workaround for the Blackberry mkfifo bug.

-----------

### Internals

* Removed `realm::util::network` library.
* Removed event loop library.
* Reduced the number of open files on Android.

----------------------------------------------

# 1.4.0 Release notes

### Breaking changes

* Throw a logic error (of type `table_has_no_columns`) if an attempt is made to
  add rows to a table with no columns. (#1897)
* S: A clear operation is emitted on removal of the last public column of a table.

----------------------------------------------

# 1.3.1 Release notes

### Bugfixes

* Add missing locks when access `Table::m_views` which may lead to some java
  crashes since java will not guarantee destruction and construction always
  happen in the same thread. (#1958)
* Fixed a bug where tableviews created via backlinks were not automatically
  updated when the source table changed. (#1950)

### Breaking changes

* Throw a logic error (of type `table_has_no_columns`) if an attempt is made to
  add rows to a table with no columns. (#1897)
* S: A clear operation is emitted on removal of the last public column of a table.

### Enhancements

* Increased the verbosity of some exception error messages to help debugging.

----------------------------------------------

# 1.3.0 Release notes

### Bugfixes

* Fix a crash when `Group::move_table()` is called before table accessors are
  initialized. (#1939)

### Breaking changes

* Sorting with `STRING_COMPARE_CORE` now sorts with pre 1.1.2 ordering. Sorting
  with 1.1.2 order is available by using `STRING_COMPARE_CORE_SIMILAR`. (#1947)

-----------

### Internals

* Performance improvements for `LinkListColumn::get_ptr()`. (#1933)

----------------------------------------------

# 1.2.0 Release notes

### Bugfixes

* Update table views so that rows are not attached after calling Table::clear() (#1837)
* The SlabAlloctor was not correctly releasing all its stale memory mappings
  when it was detached. If a SharedGroup was reused to access a database
  following both a call of compact() and a commit() (the latter potentially
  by a different SharedGroup), the stale memory mappings would shadow part
  of the database. This would look like some form of corruption. Specifically
  issues #1092 and #1601 are known to be symptoms of this bug, but issues
  #1506 and #1769 are also likely to be caused by it. Note that even though
  this bug looks like corruption, the database isn't corrupted at all.
  Reopening it by a different SharedGroup will work fine; Only the SharedGroup
  that executed the compact() will have a stale view of the file.
* Check and retry if flock() returns EINTR (issue #1916)
* The slabs (regions of memory used for temporary storage during a write transaction),
  did not correctly track changes in file size, if the allocator was detached, the
  file shrunk and the allocator was re-attached. This scenario can be triggered by
  compact, or by copying/creating a new realm file which is then smaller than the
  old one when you re-attach. The bug led to possible allocation of overlapping
  memory chunks, one of which would then later corrupt the other. To a user this
  would look like file corruption. It is theoretically possibly, but not likely,
  that the corrupted datastructure could be succesfully committed leading to a real
  corruption of the database. The fix is to release all slabs when the allocator
  is detached. Fixes #1898, #1915, #1918, very likely #1337 and possibly #1822.

### Breaking changes

* Removed the 'stealing' variant of export for handover. It was not a great
  idea. It was not being used and required locking which we'd like to avoid.
* S: A concept of log levels was added to `util::Logger`. `util::Logger::log()`
  now takes a log level argument, and new shorthand methods were added
  (`debug()`, `info()`, `warn()`, ...). All loggers now have a `level_threshold`
  property through which they efficiently query for the current log level
  threshold.

### Enhancements

* Allow SharedGroups to pin specific versions for handover
* Reduced the object-size overhead of assertions.
* Fixed a spelling mistake in the message of the `LogicError::wrong_group_state`.

-----------

### Internals

* Non concurrent tests are run on the main process thread. (#1862)
* S: `REALM_QUOTE()` macro moved from `<realm/version.hpp>` to
  `<realm/util/features.h>`. This also fixes a dangling reference to
  `REALM_QUOTE_2()` in `<realm/util/features.h>`.
* Minimize the amount of additional virtual address space used during Commit().
  (#1478)
* New feature in the unit test framework: Ability to specify log level
  threshold for custom intra test logging (`UNITTEST_LOG_LEVEL`).
* Switch from `-O3` to `-Os` to compile OpenSSL: https://github.com/android-ndk/ndk/issues/110

----------------------------------------------


# 1.1.2 Release notes

### Bugfixes

* S: In the network API (namespace `util::network`), do not report an error to
  the application if system calls `read()`, `write()`, or `accept()` fail with
  `EAGAIN` on a socket in nonblocking mode after `poll()` has signalled
  readiness. Instead, go back and wait for `poll()` to signal readiness again.

### Breaking changes

* Sorting order of strings is now according to more common scheme for special
  characters (space, dash, etc), and for letters it's now such that visually
  similiar letters (that is, those that differ only by diacritics, etc) are
  grouped together. (#1639)

-----------

### Internals

* S: New unit tests `Network_ReadWriteLargeAmount` and
  `Network_AsyncReadWriteLargeAmount`.

----------------------------------------------


# 1.1.1 Release notes

### Bugfixes

* Fixed a recently introduced crash bug on indexed columns (#1869)
* Implement `TableViewBase`'s copy-assignment operator to prevent link errors when it is used.
* No longer assert on a "!cfg.session_initiator" in SlabAlloc::attach_file(). This makes issue
  #1784 go away, but also removes an option to detect and flag if the ".lock" file is deleted
  while a SharedGroup is attached to the file. Please note: Removal of the ".lock" file while
  the database is attached may lead to corruption of the database.

### Enhancements

* Improve performance of opening Realm files and making commits when using
  external writelogs by eliminating some unneeded `fsync()`s.

----------------------------------------------

# 1.1.0 Release notes

### Bugfixes

* Fix for #1846: If an exception is thrown from SlabAlloc::attach_file(), it
  forgot to unlock a mutex protecting the shared memory mapping. In cases
  where the last reference to the memory mapping goes out of scope, it would
  cause the assert "Destruction of mutex in use". Fix is to use unique_lock
  to ensure the mutex is unlocked before destruction.
* Fix a crash when `Table::set_string_unique()` is called but the underlying
  column is actually a StringEnumColumn.
* Fix an assertion failure when combining a `Query` with no conditions with another `Query`.

### Breaking changes

* S: Type of completion handler arguments changed from `const H&` to `H` for all
  asynchronous operations offered by the networking API (namespace
  `util::network`).
* S: `util::network::deadline_timer::async_wait()` no longer declared `noexcept`
  (it never should have been).

### Enhancements

* Strictly enforce not allowing search indexes to be created on unsupported column types.
* S: Event loop API reworked to more closely align with the `util::network` API,
  and to better provide for multiple alternative implementations (not considered
  breaking because the event loop API was not yet in use).
* S: Bugs fixed in the POSIX based implementation (not listed under bug fixes
  because the event loop API was not yet in use).
* S: A new Apple CoreFoundation implementation of event loop API was added.
* S: Movable completion handler objects are no longer copied by the networking
  API (namespace `util::network`).

-----------

### Internals

* Upgrade build scripts to build as C++14 by default.
* Corrected two usages of undefined REALM_PLATFORM_ANDROID to REALM_ANDROID.
  This correctly enables Android log output on termination and allows using
  robust mutexes on Android platforms. (#1834)


----------------------------------------------

# 1.0.2 Release notes

### Internals

* This is functionally the same as 1.0.1. For Xamarin we now do a specialized
  cocoa build with only iOS support and without bitcode.

----------------------------------------------

# 1.0.1 Release notes

### Bugfixes

* Fix a situation where a failure during SharedGroup::open() could cause stale
  memory mappings to become accessible for later:
  In case one of the following exceptions are thrown from SharedGroup::open():
  - "Bad or incompatible history type",
  - LogicError::mixed_durability,
  - LogicError::mixed_history_type,
  - "File format version deosn't match: "
  - "Encrypted interprocess sharing is currently unsupported"
  Then:
  a) In a single process setting a later attempt to open the file would
     hit the assert "!cfg.session_initiator" reported in issue #1782.
  b) In a multiprocess setting, another process would be allowed to run
     compact(), but the current process would retain its mapping of the
     old file and attempt to reuse those mappings when a new SharedGroup
     is opened, which would likely lead to a crash later. In that case, the
     !cfg.session_initiator would not be triggered.
  May fix issue #1782.

**Note: This is a hotfix release built on top of 1.0.0

----------------------------------------------

# 1.0.0 Release notes

### Bugfixes

* Fixed move_last_over() replacing null values for binary columns in the moved
  row with zero-length values.

### Enhancements

* File operations would previously throw `std::runtime_error` for error cases without a
  specialized exception. They now throw `AccessError` instead and include path information.

-----------

### Internals

* Fixed an error in Query_Sort_And_Requery_Untyped_Monkey2 test which would cause
  this test to fail sometimes.

----------------------------------------------

# 0.100.4 Release notes

### Bugfixes

* Fix queries over multiple levels of backlinks to work when the tables involved have
  their backlink columns at different indices.

### Breaking changes

* Reverting the breaking changes wrongly introduced by 0.100.3, so that
  this release does NOT have breaking changes with respect to 0.100.2


----------------------------------------------

# 0.100.3 Release notes (This is a faulty release and should not be used)

### Bugfixes

* Fix initialization of read-only Groups which are sharing file mappings with
  other read-only Groups for the same path.
* Fix TableView::clear() to work in imperative mode (issue #1803, #827)
* Fixed issue with Timestamps before the UNIX epoch not being read correctly in
  the `TransactLogParser`. Rollbacks and advances with such Timestamps would
  throw a `BadTransactLog` exception. (#1802)

### Breaking changes

* Search indexes no longer support strings with lengths greater than
  `Table::max_indexed_string_length`. If you try to add a string with a longer length
  (through the Table interface), then a `realm::LogicError` will be thrown with type
  `string_too_long_for_index`. Calling `Table::add_search_index()` will now return a
  boolean value indicating whether or not the index could be created on the column. If
  the column contains strings that exceed the maximum allowed length, then
  `Table::add_search_index()` will return false and the index will not be created, but the data
  in the underlying column will remain unaffected. This is so that bindings can attempt to
  create a search index on a column without knowing the lengths of the strings in the column.
  Realm will continue to operate as before on any search index that already stores strings longer
  than the maximum allowed length meaning that this change is not file breaking (no upgrade is
  required). However, as stated above, any new strings that exceed the maximum length will
  not be allowed into a search index, to insert long strings just turn off the search index
  (although this could be left up to the user).

### Enhancements

* Distinct is now supported for columns without a search index. Bindings no longer
  need to ensure that a column has a search index before calling distinct. (#1739)

-----------

### Internals

* Upgrading to OpenSSL 1.0.1t.

----------------------------------------------

# 0.100.2 Release notes

### Bugfixes

* Fix handing over an out of sync TableView that depends on a deleted link list or
  row so that it doesn't remain perpetually out of sync (#1770).
* Fix a use-after-free when using a column which was added to an existing table
  with rows in the same transaction as it was added, which resulted in the
  automatic migration from DateTime to Timestamp crashing with a stack overflow
  in some circumstances.

----------------------------------------------

# 0.100.1 Release notes

### Bugfixes:

* Fix for: The commit logs were not properly unmapped and closed when a SharedGroup
  was closed. If one thread closed and reopened a SharedGroup which was the sole
  session participant at the time it was closed, while a different SharedGroup opened
  and closed the database in between, the first SharedGroup could end up reusing it's
  memory mappings for the commit logs, while the later accesses through a different
  SharedGroup would operate on a different set of files. This could cause inconsistency
  between the commit log and the database. In turn, this could lead to crashes during
  advance_read(), promote_to_write() and possibly commit_and_continue_as_read().
  Worse, It could also silently lead to accessors pointing to wrong objects which might
  later open for changes to the database that would be percieved as corrupting. (#1762)
* Fix for: When commitlogs change in size, all readers (and writers) must update their
  memory mmapings accordingly. The old mechanism was based on comparing the size of
  the log file with the previous size and remapping if they differ. Unfortunately, this
  is not good enough, as the commitlog may first be shrunk, then expanded back to the
  original size and in this case, the existing mechanism will not trigger remapping.
  Without remapping in such situations, POSIX considers accesses to the part of the
  mapping corresponding to deleted/added sections of the file to be undefined. Consequences
  of this bug could be crashes in advance_read(), promote_to_write() or
  commit_and_continue_as_read(). Conceivably it could also cause wrong accessor updates
  leading to accessors pointing to wrong database objects. This, in turn, could lead
  to what would be percieved as database corruption. (#1764)
* S: Assertion was sometimes dereferencing a dangling pointer in
  `util::network::buffered_input_stream::read_oper<H>::recycle_and_execute()`.

### Enhancements:

* S: `util::bind_ptr<>` extended with capability to adopt and release naked
  pointers.
* The `SharedGroup` constructor now takes an optional callback function so bindings can
  be notified when a Realm is upgraded. (#1740)

----------------------------------------------

# 0.100.0 Release notes

### Bugfixes:

* Fix of #1605 (LinkView destruction/creation should be thread-safe) and most
  likely also #1566 (crash below LinkListColumn::discard_child_accessors...) and
  possibly also #1164 (crash in SharedGroup destructor on OS X).
* Copying a `Query` restricted by a `TableView` will now avoid creating a dangling
  reference to the restricting view if the query owns the view. Dangling references
  may still occur if the `Query` does not own the restricting `TableView`.
* Fixed #1747 (valgrind report of unitialized variable).
* Fixed issue with creation of `ArrayIntNull` with certain default values that would
  result in an all-null array. (Pull request #1721)

### API breaking changes:

* The return value for LangBindHelper::get_linklist_ptr() and the argument
  to LangBindHelper::unbind_linklist_ptr has changed from being a 'LinkView*'
  into a 'const LinkViewRef&'.
* Fixed a bug, where handing over a TableView based on a Query restricted
  by another TableView would fail to propagate synchronization status correctly
  (issue #1698)
* Fixed TableViews that represent backlinks to track the same row, even if that row
  moves within its table. (Issue #1710)
* Fixed incorrect semantics when comparing a LinkList column with a Row using a
  query expression. (Issue #1713)
* Fixed TableViews that represent backlinks to not assert beneath `sync_if_needed` when
  the target row has been deleted.
* `TableView::depends_on_deleted_linklist` is now `TableView::depends_on_deleted_object`,
  and will also return true if the target row of a `TableView` that represents backlinks
  is deleted. (Issue #1710)
* New nanosecond precision `Timestamp` data and column type replace our current `DateTime`
  data and column type. (Issue #1476)
* Notice: Due to the new `Timestamp` data and column type a file upgrade will take place.
  Read-only Realm files in apps will have to be updated manually.

### Enhancements:

* TableView can now report whether its rows are guaranteed to be in table order. (Issue #1712)
* `Query::sync_view_if_needed()` allows for bringing a query's restricting view into sync with
  its underlying data source.

-----------

### Internals:

* Opening a Realm file which already has a management directory no longer throws
  and catches an exception.
* The r-value constructor for StringData has been removed because StringIndex does not
  store any data. This prevents incorrect usage which can lead to strange results.

----------------------------------------------

# 0.99.0 Release notes

### Breaking changes:

* Lock file (`foo.realm.lock`) format bumped.
* Moved all supporting files (all files except the .realm file) into a
  separate ".management" subdirectory.

### Bugfixes:

* S: Misbehavior of empty asynchronous write in POSIX networking API.
* S: Access dangling pointer while handling canceled asynchronous accept
  in POSIX networking API.
* Changed group operator== to take table names into account.  

### Enhancements:

* Multiple shared groups now share the read-only memory-mapping of
  the database. This significantly lowers pressure on virtual memory
  in multithreaded scenarios. Fixes issue #1477.
* Added emulation of robust mutexes on platforms which do not
  provide the full posix API for it. This prevents a situation
  where a crash in one process holding the lock, would leave
  the database locked. Fixes #1429
* Added support for queries that traverse backlinks. Fixes #776.
* Improve the performance of advance_read() over transations that inserted rows
  when there are live TableViews.
* The query expression API now supports equality comparisons between
  `Columns<Link>` and row accessors. This allows for link equality
  comparisons involving backlinks, and those that traverse multiple
  levels of links.

* S: Adding `util::network::buffered_input_stream::reset()`.

-----------

### Internals:

* Disabled unittest Shared_RobustAgainstDeathDuringWrite on Linux, as
  it could run forever.
* Fixed a few compiler warnings
* Disabled unittest Shared_WaitForChange again, as it can still run forever
* New features in the unit test framework: Ability to log to a file (one for
  each test thread) (`UNITTEST_LOG_TO_FILES`), and an option to abort on first
  failed check (`UNITTEST_ABORT_ON_FAILURE`). Additionally, logging
  (`util::Logger`) is now directly available to each unit test.
* New failure simulation features: Ability to prime for random triggering.

* S: New unit tests: `Network_CancelEmptyWrite`, `Network_ThrowFromHandlers`.

----------------------------------------------

# 0.98.4 Release notes

### Bugfixes:

* Copying a `Query` restricted by a `TableView` will now avoid creating a dangling
  reference to the restricting view if the query owns the view. Dangling references
  may still occur if the `Query` does not own the restricting `TableView`. (#1741)

### Enhancements:

* `Query::sync_view_if_needed()` allows for bringing a query's restricting view into sync with
  its underlying data source. (#1742)

**Note: This is a hotfix release built on top of 0.98.3. The above fixes are
        not present in version 0.99**

----------------------------------------------

# 0.98.3 Release notes

### Bugfixes:

* Fixed TableViews that represent backlinks to not assert beneath `sync_if_needed` when
  the target row has been deleted. (Issue #1723)

**Note: This is a hotfix release built on top of 0.98.2. The above fixes are
        not present in version 0.99**

----------------------------------------------

# 0.98.2 Release notes

### Bugfixes:

* Fixed TableViews that represent backlinks to track the same row, even if that row
  moves within its table. (Issue #1710)
* Fixed incorrect semantics when comparing a LinkList column with a Row using a
  query expression. (Issue #1713)

### API breaking changes:

* `TableView::depends_on_deleted_linklist` is now `TableView::depends_on_deleted_object`,
  and will also return true if the target row of a `TableView` that represents backlinks
  is deleted. (Issue #1710)

### Enhancements:

* TableView can now report whether its rows are guaranteed to be in table order. (Issue #1712)

**Note: This is a hotfix release built on top of 0.98.1. The above fixes are
        not present in version 0.99

----------------------------------------------

# 0.98.1 Release notes

### Bugfixes:

* Fixed a bug, where handing over a TableView based on a Query restricted
  by another TableView would fail to propagate synchronization status correctly
  (issue #1698)

**Note: This is a hotfix release. The above bugfix is not present
        in version 0.99

----------------------------------------------

# 0.98.0 Release notes

### Enhancements:

* Added support for queries that traverse backlinks. Fixes #776. See #1598.
* The query expression API now supports equality comparisons between
  `Columns<Link>` and row accessors. This allows for link equality
  comparisons involving backlinks, and those that traverse multiple
  levels of links. See #1609.

### Bugfixes:

* Fix a crash that occurred after moving a `Query` that owned a `TableView`.
  See #1672.

**NOTE: This is a hotfix release which is built on top of [0.97.4].**

-----------------------------------------------

# 0.97.4 Release notes

### Bugfixes:

* #1498: A crash during opening of a Realm could lead to Realm files
  which could not later be read. The symptom would be a realm file with zeroes
  in the end but on streaming form (which requires a footer at the end of the
  file instead). See issue #1638.
* Linked tables were not updated properly when calling erase with num_rows = 0
  which could be triggered by rolling back a call to insert with num_rows = 0.
  See issue #1652.
* `TableView`s created by `Table::get_backlink_view` are now correctly handled by
  `TableView`'s move assignment operator. Previously they would crash when used.
  See issue #1641.

**NOTE: This is a hotfix release which is built on top of [0.97.3].**

----------------------------------------------

# 0.97.3 Release notes

### Bugfixes:

* Update table accessors after table move rollback, issue #1551. This
  issue could have caused corruption or crashes when tables are moved
  and then the transaction is rolled back.
* Detach subspec and enumkey accessors when they are removed
  via a transaction (ex rollback). This could cause crashes
  when removing the last column in a table of type link,
  linklist, backlink, subtable, or enumkey. See #1585.
* Handing over a detached row accessor no longer crashes.

**NOTE: This is a hotfix release. The above changes are not present in
versions [0.97.2].**

----------------------------------------------

# 0.97.2 Release notes

### Enhancements:

* Add more information to IncompatibleLockFile.

**NOTE: This is a hotfix release. The above changes are not present in
versions [0.97.1].**

----------------------------------------------

# 0.97.1 Release notes

### Bugfixes:

* Fix an alignment problem which could cause crash when opening a Realm file
  on 32-bit IOS devices. (issue 1558)

**NOTE: This is a hotfix release. The above bugfixes are not present in
versions [0.97.0].**

----------------------------------------------

# 0.97.0 Release notes

### Bugfixes:

* Backlink columns were not being refreshed when the connected link column
  updated it's index in the table (insert/remove/move column). This is now
  fixed. See issue #1499.
* Backlink columns were always inserted at the end of a table, however on a
  transaction rollback in certain cases, backlink columns were removed from
  internal (not the end) indices and the roll back should put them back there.
  This could cause a crash on rollback and was reported in ticket #1502.
* Bumps table version when `Table::set_null()` called.
  `TableView::sync_if_needed()` wouldn't be able to see the version changes
  after `Table::set_null()` was called.
  (https://github.com/realm/realm-java/issues/2366)
* Fix an assertion failure in `Query::apply_patch` when handing over
  certain queries.
* Fix incorrect results from certain handed-over queries.

### API breaking changes:

* Language bindings can now test if a TableView depends on a deleted LinkList
  (detached LinkView) using `bool TableViewBase::depends_deleted_linklist()`.
  See https://github.com/realm/realm-core/issues/1509 and also
  TEST(Query_ReferDeletedLinkView) in test_query.cpp for details.
* `LangBindHelper::advance_read()` and friends no longer take a history
  argument. Access to the history is now gained automatically via
  `Replication::get_history()`. Applications and bindings should simply delete
  the history argument at each call site.
* `SharedGroup::get_current_version()`, `LangBindHelper::get_current_version()`,
  and `Replication::get_current_version()` were all removed. They are not used
  by the Cocoa or Android binding, and `SharedGroup::get_current_version()` was
  never supposed to be public.

### Enhancements:

* Adds support for in-Realm history of changes (`<realm/history.hpp>`), but
  keeps the current history implementation as the default for now
  (`<realm/commit_log.hpp>`).
* New methods `ReadTransaction::get_version()` and
  `WriteTransaction::get_version()` for getting the version of the bound
  snapshot during a transaction.

-----------

### Internals:

* Bumps file format version from 3 to 4 due to support for in-Realm history of
  changes (extra entries in `Group::m_top`). The bump is necessary due to lack
  of forwards compatibility. The changes are backwards compatible, and automatic
  upgrade is implemented.
* Adds checks for consistent use of history types.
* Removes the "server sync mode" flag from the Realm file header. This feature
  is now superseded by the more powerful history type consistency checks. This
  is not considered a file format change, as no released core version will ever
  set the "server sync mode" flag.
* The SharedInfo file format version was bumped due to addition of history type
  information (all concurrent session participants must agree on SharedInfo file
  format version).
* Make it possible to open both file format version 3 and 4 files without
  upgrading. If in-Realm history is required and the current file format version
  is less than 4, upgrade to version 4. Otherwise, if the current file format
  version is less than 3, upgrade to version 3.
* The current file format version is available via
  `Allocator::get_file_format_version()`.
* Set Realm file format to zero (not yet decided) when creating a new empty
  Realm where top-ref is zero. This was done to minimize the number of distinct
  places in the code dealing with file format upgrade logic.
* Check that all session participants agree on target Realm file format for that
  session. File format upgrade required when larger than the actual file format.
* Eliminate a temporary memory mapping of the SharedInfo file during the Realm
  opening process.
* Improved documentation of some of the complicated parts of the Realm opening
  process.
* Introducing `RefOrTagged` value type whan can be used to make it safer to work
  with "tagged integers" in arrays having the "has refs" flag.
* New features in the unit test framework: Ability to specify number of internal
  repetitions of the set of selected tests. Also, progress reporting now
  includes information about which test thread runs which unit test. Also, new
  test introduction macro `NO_CONCUR_TEST()` for those tests that cannot run
  concurrently with other tests, or with other executions of themselves. From
  now on, all unit tests must be able to run multiple times, and must either be
  fully thread safe, or must be introduced with `NO_CONCUR_TEST()`.

----------------------------------------------

# 0.96.2 Release notes

### Bugfixes:

* `Group::TransactAdvancer::move_group_level_table()` was forgetting some of its
  duties (move the table accessor). That has been fixed.
* While generating transaction logs, we didn't always deselect nested
  accessors. For example, when performing a table-level operation, we didn't
  deselect a selected link list. In some cases, it didn't matter, but in others
  it did. The general rule is that an operation on a particular level must
  deselect every accessor at deeper (more nested) levels. This is important for
  the merge logic of the sync mechanism, and for transaction log reversal. This
  has been fixed.
* While reversing transaction logs, group level operations did not terminate the
  preceding section of table level operations. Was fixed.
* Table::clear() issues link nullification instructions for each link that did
  point to a removed row. It did however issue those instructions after the
  clear instruction, which is incorrect, as the links do not exist after the
  clear operation. Was fixed.
* `SharedGroup::compact()` does a sync before renaming to avoid corrupted db
  file after compacting.

### Enhancements:

* Add SharedGroup::get_transact_stage().

### Internals:

* Improve documentation of `Group::move_table()` and `LinkView::move()`.
* Early out from `Group::move_table()` if `from_index == to_index`. This
  behaviour agrees with `LinkView::move()` and is assumed by other parts of
  core, and by the merge logic of the sync mechanism.
* Convert some assertions on arguments of public `Group`, `Table`, and
  `LinkView` methods to throwing checks.
* Align argument naming of `Group::move_table()` and `LinkView::move()`.

----------------------------------------------

# 0.96.1 Release notes

### API breaking changes:

* Important for language bindings: Any method on Query and TableView that
  depends on a deleted LinkView will now return sane return values;
  Query::find() returns npos, Query::find_all() returns empty TableView,
  Query::count() returns 0, TableView::sum() returns 0 (TableView created
  from LinkView::get_sorted_view). So they will no longer throw
  DeletedLinkView or crash. See TEST(Query_ReferDeletedLinkView) in
  test_query.cpp for more info.

### Enhancements:

* Memory errors caused by calls to mmap/mremap will now throw a specific
  AddressSpaceExhausted exception which is a subclass of the previously
  thrown std::runtime_error. This is so that iOS and Android language
  bindings can specifically catch this case and handle it differently
  than the rest of the general std::runtime_errors.
* Doubled the speed of TableView::clear() when parent table has an
  indexed column.

----------------------------------------------

# 0.96.0 Release notes

### Bugfixes:

* Handing over a query that includes an expression node will now avoid
  sharing the expression nodes between `Query` instances. This prevents
  data races that could give incorrect results or crashes.

### Enhancements:

* Subqueries are now supported via `Table::column(size_t, Query)`.
  This allows for queries based on the number of rows in the linked table
  that match the given subquery.

----------------------------------------------

# 0.95.9 Release notes

### Bugfixes:

* Fixed terminate() being called rather than InvalidDatabase being thrown when
  a non-enrypted file that begins with four zero bytes was opened as an
  encrypted file.

----------------------------------------------

# 0.95.8 Release notes

### Bugfixes:

* Fixed error when opening encrypted streaming-form files which would be
  resized on open due to the size not aligning with a chunked mapping section
  boundary.

### API breaking changes:

* Any attempt to execute a query that depends on a LinkList that has been
  deleted from its table will now throw `DeletedLinkView` instead of
  segfaulting. No other changes has been made; you must still verify
  LinkViewRef::is_attached() before calling any methods on a LinkViewRef, as
  usual.

### Enhancements:

* Optimized speed of TableView::clear() on an indexed unordered Table. A clear()
  that before took several minutes with 300000 rows now takes a few seconds.

----------------------------------------------

# 0.95.7 Release notes

### Bugfixes:

* Corrected a bug which caused handover of a query with a restricting
  view to loose the restricting view.

----------------------------------------------

# 0.95.6 Release notes

### Bugfixes:

* Fixed incorrect initialization of TableViews from queries on LinkViews
  resulting in `TableView::is_in_sync()` being incorrect until the first time
  it is brought back into sync.
* Fixed `TableView` aggregate methods to give the correct result when called on
  a table view that at one point had detached refs but has since been synced.
* Fixed another bug in `ColumnBase::build()` which would cause it to produce an
  invalid B+-tree (incorrect number of elements per child in the compact
  form). This is a bug that could have been triggered through proper use of our
  bindings in their current form. In particular, it would have been triggered
  when adding a new attribute to a class that already has a sufficiently large
  number of objects in it (> REALM_MAX_BPNODE_SIZE^2 = 1,000,000).
* Fixed a bug in handover of Queries which use links. The bug was incomplete
  cloning of the underlying data structure. This bug goes unnoticed as long
  as the original datastructure is intact and is only seen if the original
  datastructure is deleted or changed before the handed over query is re-executed

### Enhancements:

* Added support for handing over TableRefs from one thread to another.

-----------

### Internals:

* Add `test_util::to_string()` for convenience. std::to_string() is not
  available via all Android NDK toolchains.
* New operation: ChangeLinkTargets. It replaces all links to one row with
  links to a different row.
* Regular assertions (REALM_ASSERT()) are no longer enabled by default in
  release mode. Note that this is a reversion back to the "natural" state of
  affairs, after a period of having them enabled by default in release mode. The
  Cocoa binding was the primary target when the assertions were enabled a while
  back, and steps were taken to explicitely disable those assertions in the
  Android binding to avoid a performance-wise impact there. It is believed that
  the assertions are no longer needed in the Cocoa binding, but in case they
  are, the right approach, going forward, is to enable them specifically for the
  Cocoa binding. Note that with these changes, the Android binding no longer
  needs to explicitely disable regular assertions in release mode.
* Upgraded Android toolchain to R10E and gcc to 4.9 for all architectures.

----------------------------------------------


# 0.95.5 Release notes

### Bugfixes:

* Fixed Row accessor updating after an unordered `TableView::clear()`.
* Fixed bug in `ColumnBase::build()` which would cause it to produce an invalid
  (too shallow) B+-tree. This is a bug that could have been triggered through
  proper use of our bindings in their current form. In particular, it would have
  been triggered when adding a new attribute to a class that already has a
  sufficiently large number of objects in it (> REALM_MAX_BPNODE_SIZE^2 =
  1,000,000).

### Enhancements:

* New default constructor added to `BasicRowExpr<>`. A default constructed
  instance is in the detached state.

----------------------------------------------

# 0.95.4 Release notes

### Bugfixes:

* Fixed incorrect handling of a race between a commit() and a new thread
  or process opening the database. In debug mode, the race would trigger an
  assert "cfg.session_initiator || !cfg.is_shared", in release mode it could
  conceivably result in undefined behaviour.
* Fixed a segmentation fault in SharedGroup::do_open_2
* Fixed a bug en ringbuffer handling that could cause readers to get a wrong
  top pointer - causing later asserts regarding the size of the top array, or
  asserts reporting mismatch between versions.

### API breaking changes:

* Primary key support has been removed. Instead, new instructions have been
  introduced: SetIntUnique, SetStringUnique. To implement primary keys, callers
  should manually check the PK constraint and then emit these instructions in
  place of the regular SetInt and SetString instructions.

### Enhancements:

* Added TableView::distinct() method. It obeys TableView::sync_if_needed().
  A call to distinct() will first fully populate the TableView and then perform
  a distinct algorithm on that (i.e. it will *not* add a secondary distinct filter
  to any earlier filter applied). See more in TEST(TableView_Distinct) in
  test_table_view.cpp.

-----------

### Internals:

* Changed `Group::remove_table`, `Group::TransactAdvancer::insert_group_level_table`
  and `Group::TransactAdvancer::erase_group_level_table` from _move-last-over_ to
  preserve table ordering within the group.

----------------------------------------------

# 0.95.3 Release notes

### Bugfixes:

* Reverted what was presumably a fix for a race between commit and opening the database (0.95.2).

----------------------------------------------

# 0.95.2 Release notes

### Bugfixes:

* Fixed bug where Query::average() would include the number of nulls in the
  result.
* Presumably fixed a race between commit and opening the database.

### Enhancements:

* Recycle memory allocated for asynchronous operations in the networking
  subsystem (`util::network`).

----------------------------------------------

# 0.95.1 Release notes

### Bugfixes:
* Fixed bug that would give false search results for queries on integer columns
  due to bug in bithacks deep inside Array::find()

### Enhancements:

* Added Table::get_version_counter() exposing the versioning counter for the Table
* Add `TableView::get_query()`.


----------------------------------------------

# 0.95.0 Release notes

### Bugfixes:

* When inserting a new non-nullable Binary column to a table that had
  *existing* rows, then the automatically added values would become null
* Fixed updating TableViews when applying a transaction log with a table clear.
* Fewer things are copied in TableView's move constructor.
* Prevent spurious blocking in networking subsystem (put sockets in nonblocking
  mode even when used with poll/select).
* Fixed the shared group being left in an inconsistent state if the transaction
  log observer threw an exception.
* Fixed issue with table accessors not being updated properly, when link columns
  were changed (e.g. in Group::remove_table, when the table had link columns).

### API breaking changes:

* Use `util::Logger` instead of `std::ostream` for logging during changeset
  replay (`Replication::apply_changeset()`).

### Enhancements:

* Eliminated use of signals in encryption. This also fixes failures related
  to signals on some devices.

-----------

### Internals:

* More checking and throwing of logical errors in `Table::set_binary()` and
  `Table::set_link()`.

----------------------------------------------

# 0.94.4 Release notes

### Bugfixes:

* Fixed crash in find_all()

### Enhancements:

* Queries are no longer limited to 16 levels of grouping.
* New substring operations (ranged insert, erase on values in string columns).
* Adds schema change notification handler API to Group.

-----------

### Internals:

* New operations: Swap rows, move rows, move column, move group level table.
* Changes order of nullify instructions that appeared as a result of erase
  to occur in the transaction log before the erase instruction that caused
  them.
* New utility class: DirScanner.
* New test utility function: quote.
* New assertion macro: REALM_ASSERT_EX, replacing REALM_ASSERT_n macros.


----------------------------------------------

# 0.94.3 Release notes

### Bugfixes:

* Fixed mremap() fallback on Blackberry.

----------------------------------------------

# 0.94.2 Release notes

### Bugfixes:

* Fixed a bug that lead to SharedGroup::compact failing to attach to the newly
  written file.

----------------------------------------------

# 0.94.1 Release notes

### Bugfixes:

* Fixed a bug in SharedGroup::Compact() which could leave the database in an
  inconsistent state.

### Enhancements:

* Queries are no longer limited to 16 levels of grouping.

-----------

### Internals:

* Obsolete YAML-based documentation removed.
* Removed `std::` in front integral types (e.g. `size_t`, `int64_t` etc.)

----------------------------------------------

# 0.94.0 Release notes

### Bugfixes:

* Fixed a crash bug that could be triggered if a Realm is rapidly opened and
  closed and reopened many times on multiple threads. The bug caused the
  internal version information structure to overflow, causing an assert or a
  crash (if assert was disabled).
* The error handling for `pthread_cond_wait()/pthread_cond_timedwait()`
  incorrectly attributed the failure to `pthread_mutex_lock()`.
* The error handling for several File functions incorrectly attributed the
  failure to `open()`.
* Added the bitcode marker to iOS Simulator builds so that bitcode for device
  builds can actually be used.
* Build with bitcode both enabled and disabled for iOS for compatibility with
  Xcode 6.

### API breaking changes:
* None.

### Enhancements:
* Supports finding non-null links (Link + LinkList) in queries, using
  syntax like `Query q = table->column<Link>(col).is_not_null();`
* Comparisons involving unary links on each side of the operator are now
  supported by query_expression.hpp.
* Added version chunk information and failure reason for
  `pthread_mutex_lock()`.
* Termination routines now always display the library's version before the
  error message.
* Automatically clean up stale MemOnly files which were not deleted on close
  due to the process crashing.

-----------

### Internals:

* All calls to `REALM_TERMINATE` or `util::terminate()` now display the
  library's version. It is no longer necessary to include `REALM_VER_CHUNK` in
  calls to those functions.
* Various bug fixes in `util::network`, most notably, asynchronous operations
  that complete immediately can now be canceled.
* Improved documentation in `util::network`.
* Improved exception safety in `util::network`.
* `util::network::socket_base::close()` is now `noexcept`.
* New `util::network::socket_base::cancel()`.
* Added `util::network::deadline_timer` class.
* Breaking: Free-standing functions `util::network::write()` and
  `util::network::async_write()` converted to members of
  `util::network::socket`.


----------------------------------------------

# 0.93.0 Release notes

### Bugfixes:
* Fixed severe bug in Array allocator that could give asserts like
  `Assertion failed: value <= 0xFFFFFFL [26000016, 16777215]`, especially
  for BinaryData columns. This bug could be triggered by using binary data
  blobs with a size in the range between 8M and 16M.
* Fixed assert that could happen in rare cases when calling set_null() on an
  indexed nullable column.
* Fixed all aggregate methods on Table (min, max, etc) that hadn't been
  updated/kept in sync for a long while (null support, return_ndx argument,..).
* Bug in upgrading from version 2 -> 3 (upgrade could be invoked twice for the
  same file if opened from two places simultaneously)
* `Spec` and thereby `Descriptor` and `Table` equality has been fixed. Now
  handles attributes (nullability etc), sub tables, optimized string columns
  and target link types correctly.
* A stackoverflow issue in encrypted_file_mapping. Allocing 4k bytes on the
  stack would cause some random crashes on small stack size configurations.
* Now includes a statically-linked copy of OpenSSL crypto functions rather
  than dynamically linking Androids system OpenSSL to avoid bugs introduced
  by system crypto functions on some devices.
* Added copy constructor to `BasicRow<Table>` to fix a bug that could lead to
  unregistered row accessors being created. This bug is also part of a list of
  blocking issues that prevent the test suite from running when compiled with
  `-fno-elide-constructors`.
* A bug in the `Query` copy constructor has been fixed that could cause asserts
  due to missing capacity extension in one of the object's internal members.
* `Expression` subclasses now update `Query`s current descriptor after setting
  the table. This prevents a null dereference when adding further conditions
  to the query.
* Fixes a crash due to an assert when rolling back a transaction in which a link
  or linklist column was removed.
* A bug in `Query` copying has been fixed. The bug could cause references to
  Tables which should stay under the supervision of one SharedGroup to leak
  to another during handover_export() leading to corruption.
* Query expression operators now give correct results when an argument comes
  from a link.
* Fixed a bug in the way the new memory mapping machinery interacted with
  encryption.
* Query expression comparisons now give correct results when comparing a linked
  column with a column in the base table.
* Fixed assertion failure when TableViewBase::is_row_attached() would return
  false in a debug build.

### API breaking changes:

* A number of methods in the following classes have been renamed to match the
  coding guidelines (lowercase, underscore separation):
    * `Array`, `ArrayBlob`, `ArrayInteger`, `ArrayString`, `BasicArray<T>`;
    * `Column<T, N>`, `IntegerColumn`, `StringColumn`, `StringEnumColumn`;
    * `Group`;
    * `Query`;
    * `StringIndex`.
* `TableView::remove()`, `TableView::remove_last()`, and `TableView::clear()`
  now take an extra argument of type `RemoveMode` which specifies whether rows
  must be removed in a way that does, or does not maintain the order of the
  remaining rows in the underlying table. In any case, the order of remaining
  rows in the table view is maintained. This is listed as an API breaking change
  because the situation before this change was confusing, to say the least. In
  particular, `TableView::clear()` would choose between the ordered and the
  unordered mode based on whether the underlying table had at least one link (or
  link list) column. You are strongly advised to revisit all call sites and
  check that they do the right thing. Note that both bindings (Cocoa and
  Android) are likely to want to use unordered mode everywhere.

### Enhancements:
* Added argument to Table::average() and TableView::average() that returns number
  of values that were used for computing the average
* Full null support everywhere and on all column types. See
  `TEST(Query_NullShowcase)` in `test_query.cpp` in core repo.
* Added `Descriptor::get_link_target()`, for completeness.
* Added extra `allow_file_format_upgrade` argument to `SharedGroup::open()`.
* Modifying `Descriptor` methods now throw `LogicError` when appropriate (rather
  than asserting).
* Allow querying based on the number of rows that a linked list column links to,
  using expressions like `table->column<LinkList>(0).count() > 5`.
* New `util::File::AccessError::get_path()` returns the file system path
  associated with the exception. Note that exception classes
  `util::File::PermissionDenied`, `util::File::NotFound`, `util::File::Exists`,
  and `InvalidDatabase` are subclasses of `util::File::AccessError`.
* Allow queries to include expressions that compute aggregates on columns in linked tables,
  such as `table->column<LinkList>(0).column<Int>(1).sum() >= 1000`.
* Added a check for functioning SEGV signals to fail more gracefully when
  they're broken.

-----------

### Internals:

* Added argument to SharedGroup to prevent automatic file format upgrade. If an
  upgrade is required, the constructor will throw `FileFormatUpgradeRequired`.
* Let `LinkColumn` and `LinkListColumn` adhere to the same nullability interface
  as the rest of the column types.
* The code coverage CI job now builds with the `-fno-elide-constructors` flag,
  which should improve the depth of the coverage analysis. All bugs that were
  blocking the use of this flag have been fixed.
* SharedGroup no longer needs to remap the database file when it grows. This is
  a key requirement for reusing the memory mapping across threads.
* `NOEXCEPT*` macros have been replaced by the C++11 `noexcept` specifier.
* The `REALM_CONSTEXPR` macro has been replaced by the C++11 `constexpr` keyword.
* Removed conditional compilation of null string support.

----------------------------------------------

# 0.92.3 Release notes

### Bugfixes:

* Added the bitcode marker to iOS Simulator builds so that bitcode for device
  builds can actually be used.

**NOTE: This is a hotfix release. The above bugfixes are not present in
versions [0.93.0].**

----------------------------------------------

# 0.92.2 Release notes

### Bugfixes:

* Fixed assertion failure when TableViewBase::is_row_attached() would return
  false in a debug build.
* Fixes a crash due to an assert when rolling back a transaction in which a link
  or linklist column was removed.

**NOTE: This is a hotfix release.**

-----------

### Internals:

* Now built for Apple platforms with the non-beta version of Xcode 7.

----------------------------------------------

# 0.92.1 Release notes

### Bugfixes:

* Reverted prelinking of static libraries on Apple platforms as it caused
  `dynamic_cast<>()` and `typeid()` checks to fail in some scenarios, including
  when sorting by integer or floating point columns.

-----------

### Internals:

* Renamed `Column` to `IntegerColumn` and `TColumn` to `Column`.
* Renamed `AdaptiveStringColumn` to `StringColumn`.
* Several column classes were renamed to follow the `XxxColumn` naming scheme
  (e.g., `ColumnLink` to `LinkColumn`).
* Removed conditional compilation of replication features.
* More information from `InvalidDatabase::what()`.
* Disabled support for the async daemon on iOS and watchOS.

----------------------------------------------

# 0.92.0 Release notes

### Bugfixes:

* The upgraded file format version is written out to disk, eliminating potential
  deadlocks.

### API breaking changes:

* Support for the following deprecated operations on Table has been removed:
  insert_int, insert_string, etc., insert_done, and add_int. To insert a value,
  one must now call insert_empty_row, then set the appropriate values for each
  column.
* Changed `LinkView::move` so that the `new_link_ndx` will be the index at which
  the moved link can be found after the move is completed.

### Enhancements:

* Support for ordered row removal in tables with links. This was done for
  completeness, and because an ordered insertion in tables with links, when
  reversed, becomes an ordered removal. Support for ordered insertion in tables
  with links was added recently because the sync mechanism can produce
  them. Also added a few missing pieces of support for ordered insertion in
  tables with links.

-----------

### Internals:

* Added static `Array::create_array()` for creating non-empty arrays, and extend
  `Array::create()` such that it can create non-empty arrays.
* The creation of the free-space arrays (`Group::m_free_positions`,
  `Group::m_free_lengths`, `Group::m_free_versions`) is now always done by
  `GroupWriter::write_group()`. Previously it was done in various places
  (`Group::attach()`, `Group::commit()`, `Group::reset_free_space_versions()`).
* `Group::reset_free_space_versions()` has been eliminated. These days the Realm
  version is persisted across sessions, so there is no longer any cases where
  version tracking on free-space chunks needs to be reset.
* Free-space arrays (`Group::m_free_positions`, `Group::m_free_lengths`,
  `Group::m_free_versions`) was moved to `GroupWriter`, as they are now only
  needed during `GroupWriter::write_Group()`. This significantly reduces the
  "shallow" memory footprint of `Group`.
* Improved exception safety in `Group::attach()`.
* `Group::commit()` now throws instead of aborting on an assertion if the group
  accessor is detached or if it is used in transactional mode (via
  `SharedGroup`).
* Instruction encoding changed for `InsertEmptyRows` and `EraseRows` (also used
  for `move_last_over()`). The third operand is now `prior_num_rows` (the number
  of rows prior to modification) in all cases. Previously there was a serious
  confusion about this.
* Cleaned up the batch removal of rows used by `TableView`.
* Optimize row removal by skipping cascade mechanism when table has no forward
  link columns.
* Virtual `ColumnBase::insert(row_ndx, num_rows, is_append)` was changed to
  `ColumnBase::insert_rows(row_ndx, num_rows_to_insert,
  prior_num_rows)`. Virtual `ColumnBase::erase(row_ndx, is_last)` was changed to
  `ColumnBase::erase_rows(row_ndx, num_rows_to_erase, prior_num_rows)`. Virtual
  `ColumnBase::move_last_over(row_ndx, last_row_ndx)` was changed to
  `ColumnBase::move_last_row_over(row_ndx, prior_num_rows)`. Function names were
  changed to avoid confusing similarity to the various non-virtual operations of
  the same name on subclasses of `ColumnBase`. `prior_num_rows` is passed
  because if carries more useful information than
  `is_append`/`is_last`. `num_rows_to_erase` was added for consistency.
* On some subclasses of `ColumnBase` a new non-virtual `erase(row_ndx, is_last)`
  was added for practical reasons; an intended overload of `erase(row_ndx)` for
  when you know whether the specified row index is the last one.
* Slight performance improvements in `Array::FindGTE()`.
* Renamed `Array::FindGTE()` to `Array::find_gte()`.

----------------------------------------------

# 0.91.2 Release notes

### Enhancements:

* Added support for building for watchOS.

----------------------------------------------

# 0.91.1 Release notes

### Bugfixes:

* Fixed a bug in SharedGroup::grab_specific_readlock() which would fail to
  grab the specified readlock even though the requested version was available
  in the case where a concurrent cleanup operation had a conflicting request
  for the same (oldest) entry in the ringbuffer.
* Fixed a performance regression in TableView::clear().

### API breaking changes:

* Argument `is_backend` removed from from the public version of
  `SharedGroup::open()`. Fortunately, bindings are not currently calling
  `SharedGroup::open()`.
* `File::resize()` no longer calls `fcntl()` with `F_FULLFSYNC`. This feature
  has been moved to `File::sync()`.

### Enhancements:

* New feature added to disable all forms of 'sync to disk'. This is supposed to
  be used only during unit testing. See header `disable_sync_to_disk.hpp`.
* Added `LinkList.swap()` to swap two members of a link list.
* Added a Query constructor that takes ownership of a TableView.

### Internals:

* On Linux we now call 'sync to disk' after Realm file resizes. Previusly, this
  was only done on Apple platforms.

----------------------------------------------

# 0.91.0 Release notes

### Bugfixes:

* Fixed assertion when tests are run with `REALM_OLDQUERY_FALLBACK` disabled by
  updating Value::import to work with DateTime
* Fix incorrect results when querying for < or <= on ints which requires 64 bits
  to represent with a CPU that supports SSE 4.2.

### API breaking changes:

* Named exception UnreachableVersion replaced by "unspecified" LogicError
  exception.

### Enhancements:

* Generic networking API added.
* Support for transfer/handover of TableViews, Queries, ListViews and Rows
  between SharedGroups in different threads.  Cooperative handover (where boths
  threads participate) is supported for arbitrarily nested TableViews and
  Queries.  Restrictions apply for non-cooperative handover (aka stealing): user
  must ensure that the producing thread does not trigger a modifying operation
  on any of the involved TableViews.  For TableViews the handover can be one of
  *moving*, *copying* or *staying*, reflecting how the actual payload is
  treated.
* Support for non-end row insertion in tables with link and link list columns.
* Improved documentation of functions concerning the initiation and termination
  of transactions.
* Improved exception safety in connection with the initiation and termination of
  transactions.
* Add support for systems where mremap() exists but fails with ENOTSUP.

### Internals:

* New facility for simulating failures, such as system call failures.

----------------------------------------------

# 0.90.0 Release notes

### API breaking changes:

* Merged lr_nulls into master (support for null in String column and bugfix in
String index with 0 bytes). If you want to disable all this again, then #define
REALM_NULL_STRINGS to 0 in features.h. Else API is as follows: Call add_column()
with nullable = true. You can then use realm::null() in place of any
StringData (in Query, Table::find(), get(), set(), etc) for that column. You can
also call Table::is_null(), Table::set_null() and StringData::is_null(). This
upgrades the database file from version 2 to 3 initially the first time a file
is opened. NOTE NOTE NOTE: This may take some time. It rebuilds all indexes.

----------------------------------------------

# 0.89.9 Release notes

### Bugfixes:

* The check for functioning SEGV signals threw the exception only once. Now it
always throws when trying to use encryption.

**NOTE: This is a hotfix release. The above bugfixes are not present in
versions [0.90.0, 0.92.1].**

----------------------------------------------

# 0.89.8 Release notes

### Enhancements:

* Added a check for functioning SEGV signals to fail more gracefully when
  they're broken.

**NOTE: This is a hotfix release. The above bugfixes are not present in
versions [0.90.0, 0.92.1].**

----------------------------------------------

# 0.89.7 Release notes

### Bugfixes:

* A stackoverflow issue in encrypted_file_mapping. Allocing 4k bytes on the
  stack would cause some random crashes on small stack size configurations.
* Now includes a statically-linked copy of OpenSSL crypto functions rather
  than dynamically linking Androids system OpenSSL to avoid bugs introduced
  by system crypto functions on some devices.

**NOTE: This is a hotfix release. The above bugfixes are not present in
versions [0.90.0, 0.92.1].**

----------------------------------------------

# 0.89.6 Release notes

### Bugfixes:

* Fixed durability issue in case of power / system failures on Apple
  platforms. We now use a stronger synchronization (`fcntl(fd, F_FULLFSYNC)`) to
  stable storage when the file is extended.

----------------------------------------------

# 0.89.5 Release notes

### Bugfixes:

* Fixed errors when a changes to a table with an indexed int column are rolled
  back or advanced over.

----------------------------------------------

# 0.89.4 Release notes

### Enhancements:

* Detaching (and thus destroying) row acessors and TableViews can now be done
  safely from any thread.
* Improved performance of Query::find_all() with assertions enabled.

----------------------------------------------

# 0.89.3 Release notes

### Bugfixes:

* Fixed LinkViews containing incorrect data after a write transaction
  containing a table clear is rolled back.
* Fixed errors when a changes to a table with an indexed int column are rolled
  back.

### Enhancements:

* Changes the mmap doubling treshold on mobile devices from 128MB to 16MB.
* SharedGroup::compact() will now throw a runtime_error if called in detached state.
* Make the start index of `ListView::find()` overrideable for finding multiple
  occurances of a given row in a LinkList.
* Add `Group::set_cascade_notification_handler()` to simplify tracking changes
  due to link nullification and cascading deletes.

### Internals:

* Can now be built with encryption enabled on Linux.

----------------------------------------------

# 0.89.1 Release notes

### Bugfixes:

* Fixed bug in "index rebuilding" (would delete the wrong column, causing
  crash). See https://github.com/realm/realm-core/pull/798 ; "Remove the correct
  column when removing search indexes #798"

----------------------------------------------

# 0.89.0 Release notes

### Bugfixes:

* Added a check for NUL bytes in indexed strings to avoid corrupting data
  structures.
* Fixed bug in SharedGroup::compact(). The bug left the freelist outdated in
  some situations, which could cause crash later, if work continued on the same
  shared group. The bug did not affect the data written to the compacted
  database, but later commits working on the outdated freelist might have. The
  fix forces proper (re)initialization of the free list.
* Fixed incorrect results in querying on an indexed string column via a
  LinkView.
* Fixed corruption of indexes when using move_last_over() on rows with
  duplicated values for indexed properties.

### API breaking changes:

* Changed the `tightdb` namespace to `realm`.
* We switched to C++11, and removed functionality that was duplicated from the
  C++11 standard library, including `null_ptr` and `util::UniquePtr`.

### Enhancements:

* Improved performance of advance_read() over commits with string or binary data
  insertions.
* Improved performance sorting TableView and LinkView.
* Added Table::remove_search_index().

----------------------------------------------

# 0.88.6 Release notes

### Bugfixes:

* Fixed bug in Integer index that could make it crash or return bad results
  (String index not affected)

----------------------------------------------

# 0.88.5 Release notes

### Bugfixes:

* Fixed crash when `std::exit()` is called with active encrypted mappings.
* Fixed writing over 4KB of data to an encrypted file with `Group::write()`.
* Fixed crash after making commits which produced over 4KB of writelog data with
  encryption enabled.

-----------

### Internals:

* Switched to using mach exceptions rather than signal() for encrypted mappings
  on Apple platforms.

----------------------------------------------

# 0.88.4 Release notes

### Bugfixes:

* Fixed out-of-bounds reads when using aggregate functions on sorted TableViews.
* Fixed issues with ArrayString that *could* be the cause of all the asserts the
  past few days

-----------

### Internals:

* Many `REALM_ASSERT` invocations replaced by new `REALM_ASSERT_3` macro
  that prints variable/argument contents on failure. It's not implemented
  optimally yet.

----------------------------------------------

# 0.88.3 Release notes

### Enhancements:

* Added emulation of inter-process condition variables for use on platforms which
  do not properly implement them.

----------------------------------------------

# 0.88.2 Release notes

### Bugfixes:

* Fixed duplicated results when querying on a link column with matches at row
  1000+.

-----------

### Internals:

* Added support for Android ARM64

----------------------------------------------

# 0.88.1 Release notes

### Bugfixes:

* Fix encryption on platforms with non-4KB pages.

----------------------------------------------

# 0.88.0 Release notes

### Enhancements:

* SharedGroup::compact() now appends ".tmp_compaction_space" to the database
  name in order to get the name of its temporary workspace file instead of
  ".tmp". It also automatically removes the file in case it already exists
  before compaction.
* Add support for comparing string columns to other string columns in queries.
* `WriteTransaction::has_table()` and `WriteTransaction::rollback()` were
  added. Previously, only implicit rollback was possible with
  `WriteTransaction`.

-----------

### Internals:

* All assert failures now print the release version number.

----------------------------------------------

# 0.87.6 Release notes

### Bugfixes:

* Fixed a crashbug which could cause a reading thread to miss accessor updates
  during advance_read(), if the pending updates consisted of two or more empty
  commits followed by one or more non-empty commit. The left out accessor
  updates could lead to inconsistent datastructures which could presumably later
  cause database corruption.

### Enhancements:

* Adding *null* support to `BinaryData` in exactly the same way as it was added
  to `StringData`.

----------------------------------------------

# 0.87.5 Release notes

### Bugfixes:

* `AdaptiveStringColumn::find_all()` with an index no longer returns its results
  twice.
* Fixed `Table::get_distinct_view()` on tables which have not been modified
  since they were loaded.

### Enhancements:

* Added `SharedGroup::wait_for_change_release()` which allows you to release a
  thread waiting inside wait_for_change() on a specific SharedGroup instance.
* SharedGroup now allows you to coordinate which version of the data a read
  transaction can see. The type `VersionID` represents a specific commit to the
  database. A user can obtain the `VersionID` for the active transaction from
  `SharedGroup::get_version_of_current_transaction()`, and use it to obtain a a
  transaction accessing that same version from another ShareGroup. This is done
  by new forms of `SharedGroup::begin_read()`, `SharedGroup::advance_read()`.
  Operators are provided so that VersionID's may be compared.
* Creating distinct views on integer, datetime, bool and enum columns is now
  possible.
* Add `Table::minimum_datetime()` and `Table::maximum_datetime()`.
* Extending `Table::get_sorted_view()` to support multi-column sorting.

-----------

### Internals:

* Now uses system OpenSSL on Android rather than a statically-linked copy for
  encryption.

----------------------------------------------

# 0.87.4 Release notes

### Bugfixes:

* Fixed a crash when calling get_sorted_view() on an empty LinkList.

----------------------------------------------

# 0.87.3 Release notes

### Bugfixes:

* Fixed bug in String and Integer index where find()/find_all() would return a
  wrong match.
* Fixed the values of `Table::max_string_size`, and `Table::max_binary_size`.
* Fixed a bug occuring when claring a table with a search index on a string
  column with many rows (>1000).

----------------------------------------------

# 0.87.2 Release notes

### Internals:

* Extra assertions in `src/realm/util.file.cpp`.

----------------------------------------------

# 0.87.1 Release notes

### Enhancements:

* Added 'compact' method to SharedGroup for better control of compaction of the
  database file.
* The following constants were added: `Group::max_table_name_length`,
  `Table::max_column_name_length`, `Table::max_string_size`, and
  `Table::max_binary_size`.
* Now throwing on overlong table and column names, and on oversized strings and
  binary data values.
* Fall back to the old query nodes for String as well as int/double/float.
* Log assertions failures to the native logging system on android and Apple.

-----------

### Internals:

* There is now three kinds of runtime assertions, `REALM_ASSERT_DEBUG()`,
  which is retained only in debug-mode builds, `REALM_ASSERT_RELEASE()`, which
  is also retained in release-mode builds, and finally, `REALM_ASSERT()`,
  which is normally only retained in debug-mode builds, but may occasionally be
  retained in release-mode builds too, depending on the specific build
  configuration.
* `REALM_ASSERT()` assertions are now enabled in release-mode builds by
  default.

----------------------------------------------

# 0.87.0 Release notes

### API breaking changes:

* `TransactLogRegistry` is no longer available and must therefore no longer be
  passed to `LangBindHelper::advance_read()` and
  `LangBindHelper::promote_to_write()`.
* The exceptions `PresumablyStaleLockFile` and `LockFileButNoData` are no longer
  thrown from SharedGroup and has been removed from the API.

### Enhancements:

* Support for implicit transactions has been extended to work between multiple
  processes.
* Commitlogs can now be persisted and support server-synchronization

----------------------------------------------

# 0.86.1 Release notes

### Enhancements:

* Added `SharedGroup::get_number_of_versions()` which will report the number of
  distinct versions kept in the database.
* Added support for encryption
* Adding `SharedGroup::wait_for_change()` which allows a thread to sleep until
  the database changes.

----------------------------------------------

# 0.86.0 Release notes

### Bugfixes:

* Fixed a bug where rollback of an empty transaction could cause a crash.

### API breaking changes:

* Table::erase() can no longer be used with unordered tables. Previously it was
  allowed if the specified index was the index of the last row in the table. One
  must now always use Table::move_last_over() with unordered tables. Whether a
  table is ordered or unordered is entirely decided by the way it is used by the
  application, and note that only unordered tables are allowed to contain link
  columns.

### Enhancements:

* TableView::sync_if_needed() now returns a version number. Bindings can compare
  version numbers returned in order to determine if the TableView has changed.
* Added not_equal(), equal(), contains(), begins_with(), ends_with() for String
  columns in the Query expression syntax. They work both case sensitive and
  insensitive. So now you can write 'size_t m =
  table1->column<String>(0).contains("A", true).find();'. Works with Links too.
* Major simplification of ".lock" file handling.  We now leave the ".lock" file
  behind.
* Support added for cascading row removal. See `Descriptor::set_link_type()` for
  details. All previsouly created link columns will effectively have link-type
  'weak'.
* Rows can now be removed via a row accessors (`Row::remove()`,
  `Row::move_last_over()`).
* Speedup of double/float conditions in query expression of a factor ~5 (uses
  fallback to old query nodes for double/float too, instead of only for integer
  conditions).

----------------------------------------------

# 0.85.1 Release notes

### Bugfixes:

* Made Query store a deep copy of user given strings when using the expression
  syntax

----------------------------------------------

# 0.85.0 Release notes

### Bugfixes:

* Fixed a crash when copying a query checking for equality on an indexed string
  column.
* Fixed a stack overflow when too many query conditions were combined with Or().

### API breaking changes:

* Now supports index on Integer, Bool and Date columns; API is the same as for
  String index
* `Query::tableview()` removed as it might lead to wrong results - e.g., when
  sorting a sorted tableview.

### Enhancements:

* Make the durability level settable in the `SharedGroup` constructor and
  `open()` overloads taking a `Replication`.

----------------------------------------------

# 0.84.0 Release notes

### API breaking changes:

* `Table::set_index()` and `Table::has_index()` renamed to
  `Table::add_search_index()` and `Table::has_search_index()` respectively, and
  `Table::add_search_index()` now throws instead of failing in an unspecified
  way.
* `Table::find_pkey_string()` replaces `Table::lookup()` and has slightly
  different semantics. In particular, it now throws instead of failing in an
  unspecified way.

### Enhancements:

* A row accessor (`Row`) can now be evaluated in boolean context to see whether
  it is still attached.
* `Table::try_add_primary_key()` and `Table::remove_primary_key()` added.
* `Table::find_pkey_int()` added, but not yet backed by an integer search index.
* Added method `LangBindHelper::rollback_and_continue_as_read()`. This method
  provides the ability to rollback a write transaction while retaining
  accessors: Accessors which are detached as part of the rolled back write
  transaction are *not* automatically re-attached. Accessors that were attached
  before the write transaction and which are not detached during the write
  transaction will remain attached after the rollback.

-----------

### Internals:

* Introducing `LogicError` as an alternative to expected exceptions. See
  https://github.com/Realm/realm/wiki/Exception-safety-guarantees for more
  on this.
* Various query related speed improvements.
* Test suite now passes ASAN (address sanitizer).

----------------------------------------------

# 0.83.1 Release notes

### Bugfixes:

* Fixed bug where a TableView generated from a LinkViewRef did not update when
  the origin or target table changed.

----------------------------------------------

# 0.83.0 Release notes

### API breaking changes:

* Sorting on LinkView and TableView by multiple columns: Both classes now have
  get_sorted_view() (returns sorted view) and sort() (in-place sort). Both
  methods can take either a single column index as argument (as size_t) or a
  std::vector of columns to sort by multiple columns.
* You can now query a LinkView by calling Query::where(link_view.get()).... See
  TEST(LinkList_QueryOnLinkList) in test_link_query_view.cpp for an example.
  *** IMPORTANT NOTE: Do not call sort() on a LinkView because it does not
  yet support replication ***. get_sorted_view() works fine though.

-----------

### Internals:

* Made common base class for TableView and LinkView with common shared
  functionality (so far just sort).

----------------------------------------------

# 0.82.3 Release notes

### Bugfixes:

* Fixed bug in deep copy of Query, causing the experienced crash at end of scope
  of a Query after add_constraint_to_query() had been executed. The fix may not
  be optimal as it limits nesting of group/end_group to 16 levels, and also
  makes Query take 128 extra bytes of space. Asana task has been made.

* Fixed bug that would cause `Group::commit()` and
  `LangBindHelper::commit_and_continue_as_read()` to fail in the presence of
  search indexes.

* Bugfix: Replication::m_selected_link_list was not cleared. This bug could lead
  to general corruption in cases involving link lists.

----------------------------------------------

# 0.82.2 Release notes

### Internals:

* Query must now be deep-copied using the '=' operator instead of using
  TCopyExpressionTag. Also fixed a bug in this deep-copying.

----------------------------------------------

# 0.82.1 Release notes

### Internals:

* `REALM_MAX_LIST_SIZE` was renamed to `REALM_MAX_BPNODE_SIZE`. `BPNODE`
  stands for "B+-tree node".
* `REALM_MAX_BPNODE_SIZE` now defaults to 1000 in both *release* and *debug*
  mode.

----------------------------------------------

# 0.82.0 Release notes

### API breaking changes:

* `Group::has_table<T>()` removed, because it had awkward and incongruous
  semantics, and could not be efficiently implemented.
* The version of `Group::get_table()`, that takes a name argument, can no longer
  add a table to the group, instead it returns null if there is no table with
  the spaecified name. Addition is now handled by either `Group::add_table()` or
  `Group::get_or_add_table()`.
* `Group::get_table()` and Group::get_table_name() now throw
  `realm::InvalidArgument` if the specified table index is out of range.
* Template version of `Group::get_table()` now throws `DescriptorMismatch` if
  the dynamic type disagrees with the statically specified custom table type.
* `LangBindHelper::bind_table_ref()` was renamed to
  `LangBindHelper::bind_table_ptr()`, and `LangBindHelper::unbind_table_ref()`
  to `LangBindHelper::unbind_table_ptr()`.
* LangBindHelper functions such as get_table() have been updated to reflect the
  changed Group API.
* Exception type `ResourceAllocError` eliminated, as there was no good reason
  for keeping it (it had no clear role).

### Enhancements:

* `Group::find_table()` added as a way of mapping a table name to the index of
  table in the group.
* `Group::add_table()` and `Group::get_or_add_table()` were added.
* `Group::remove_table()` and `Group::rename_table()` were added.
* `WriteTransaction::add_table()` and `WriteTransaction::get_or_add_table()`
  ware added.

----------------------------------------------

# 0.81.0 Release notes

### API breaking changes:

* `Table::get_parent_row_index()` and `Table::get_index_in_group()` together
  replace `Table::get_index_in_parent()`. This was done to avoid a confusing mix
  of distinct concepts.

### Enhancements:

* It's now possible to sort a LinkRef according to a column in the target
  table. Also lets you build a TableView with the sorted result instead. The new
  methods on LinkViewRef are `sort()` and `get_sorted_view()`

----------------------------------------------

# 0.80.5 Release notes

### Bugfixes:

* Fixed bug in `where(&tv)...find()` where it would fail to find a match, if
  usig with a TableView, tv.
* Fixed bug in `Table::remove()` which would leak memory when rows were removed
  and the table was a link target.
* Fixed bug that prevented reuse of free-space when using
  `LangBindHelper::promote_to_write()` and
  `LangBindHelper::commit_and_continue_as_read()`.

### Enhancements:

* Lets you search for null-links, such as
  `table2->column<Link>(col_link2).is_null().find()`. Works for `Link` and
  `LinkedList`.

-----------

### Internals:

* `Group::Verify()` now checks that the managed memory is precisely the
  combination of the recorded free space and the used space reachable from the
  root node.

----------------------------------------------

# 0.80.4 Release notes

### Bugfixes:

* Bug fixed that would always leave a link list accessor (`LinkView`) in a
  corrupt state after a call to `Group::commit()` or
  `LangBindHelper::commit_and_continue_as_read()`, if the link list was modified
  during the ended "transaction", and was non-empty either before, after, or
  both before and after that "transaction".

-----------

### Internals:

* Efficiency of CRUD operations has been greatly improved due to an improvement
  of SlabAlloc). The cost of end-insertion (MyTable::add()), for example, has
  been reduced to less than a 10th of its previous cost.

----------------------------------------------

# 0.80.3 Release notes

### Bugfixes:

* Fixed bug in `Table::add_column()` which would produce a corrupt underlying
  node structure if the table already contains more than N**2 rows, where N is
  `REALM_MAX_LIST_SIZE` (currently set to 1000).
* Fixed bugs in `Table::clear()` which would produce a corrupt underlying node
  structure if the table already contains more than N rows, where N is
  `REALM_MAX_LIST_SIZE` (currently set to 1000).

### Enhancements:

* Lets you find links that point at a specific row index. Works on Query and
  Table. Please see `LinkList_QueryFindLinkTarget` in `test_link_query_view.cpp`
  for usage.

-----------

### Internals:

* Table::Verify() has been heavily extended and now also checks link columns and
  link lists (debug mode only).

----------------------------------------------

# 0.80.2 Release notes

### Bugfixes:

* Fixed bug causing corrupted table accessor when advancing transaction after last regular column is removed from table with remaining hidden backlink columns.
* Fixed replication issue causing too many link list selection instructions to be generated.

----------------------------------------------

# 0.80.1 Release notes

### Bugfixes:

* Fixed several bugs in connection to removal of like-type columns.
* Fixed bug when last regular column is removed from table with remaining hidden backlink columns.
* Fixed bug causing corrupted table accessor when column are added or removed before alink column.

----------------------------------------------

# 0.80.0 Release notes

### Bugfixes:

* Fixed bug in `TableView::clear()` causing crash if its table contained link columns.
* Fixed bug which would corrupt subtable accessors when inserting or removing parent table columns.
* Fixed bug in LinkView::refresh_accessor_tree() causing problems when transaction is advanced after a link list is cleared.
* Fixed bug causing problems when transaction is advanced after a table with link-like columns is cleared.
* Fixed bug in connection with cyclic link relationships.

### Enhancements:

* Added methods `LinkView::remove_target_row()` and `LinkView::remove_all_target_rows()`.
* Support for removing link columns


----------------------------------------------

# 0.23.0 Release notes

### Bugfixes:
* Fixed bug in `TableView::remove()` causing crash or undefined behavior.
* Fixed bugs in `Table::insert_column()` and `Table::remove_column()` causing crash or undefined behaviour.
* Fixed corruption bug when a string enumeration column follows a column with attached search index (index flavor mixup).
* Fixed in `Array::erase()` causing crash in certain row insertion scenarios.
* Fixed bug in enumerated strings column (corruption was possible when inserting default values).
* Fixed bug in `Table::update_from_parent()` causing a crash if `Group::commit()` in presence of generated subtable accessor.
* Fixed several link-related bugs due to confusion about the meaning of `LinkView::m_table`.

### API breaking changes:

* Views can now be be kept synchronized with changes to the tables used to generate the view, use `TableView::sync_if_needed()` to do so. Views are no longer detached when the table they have been generated from are changed. Instead they just go out of sync. See further description in `src/realm/table_view.hpp`.
* is_attached(), detach(), get_table(), and get_index() moved from BasicRow to RowFuncs. This makes it possible to write `link_list[7].get_index()`, for instance.
* `LinkView::get_target_row(link_ndx)` was removed as it is now just a shorthand for the equally efficient `LinkView::get(link_ndx).get_index()`.
* Added missing const versions of `LinkView::get()` and `LinkView::operator[]()`.
* Confusing `LinkView::get_parent()` removed.
* Added `LinkView::get_origin_table()` and `LinkView::get_target_table()`.

### Enhancements:
* Now maximum() and minimum() can return the index of the match and not only the value. Implemented for Query, Table and TableView.
* Now supports links in Table::to_json. Please see unit tests in the new test_json.cpp file
* Now supports DateTime Query::maximum_datetime() and DateTime Query::minimum_datetime()
* Supports links in queries, like `(table1->link(3).column<Int>(0) > 550).find()`.
* Added support for links and lists of links as column types, to enable relationships between tables.
* Adding `Table::get_index_in_parent()` and `Group::get_table(std::size_t table_ndx)`. They were needed for implicit transactions.
* `Table::get_parent_table()` can now also return the index of the column in the parent.
* Support for row accessors.
* Table, row, and descriptor accessors are now generally retained and properly adjusted when the parent table is modified.
* Added methods to find rows by target in TableView and LinkView.


-----------

### Internals:

* TableView now creates and stores a deep-copy of its query, in order for the view to refresh itself

----------------------------------------------

0.5.0 Release notes (2014-04-02)
================================

C++ (core)
-----------
The C++ API has been updated and your code will break!

### Bugfixes:

* None.

### API breaking changes:

* None.

### Enhancements:

* Read transactions are now non-blocking and their overhead has been reduced by an order of magnitude.

-----------

### Internals:

* New test suite with support for parallelized testing replaces UnitTest++. See section 'Testing' in `README.md`.


----------------------------------------------


Realm Changelog:
==================

Format:

2012-mm-dd
----------
! Fixed bug [github issuenr]: ....       (user visible bug fixed       - passed on to release notes)
+ Added feature ....                     (user visible new feature     - passed on to release notes)
- Removed/deprecated feature/method .... (user visible removed feature - passed on to release notes)
. Any other notes ....                   (internal changes)


2014-05-14 (Lasse Reinhold)
+ Lets you sort a TableView according to a Float, Double or String column (only integral column types possible before)


2014-05-08 (Finn Schiermer Andersen)
+ Added negation to the query engine.


2014-04-01 (Kristian Spangsege)
+ New framework with support for parallelized unit testing replaces UnitTest++. See section 'Testing' in `README.md`.


2014-03-25 (Kristian Spangsege)
! Fixed bug when clearing table with a float/double column.


2014-03-13 (Finn Schiermer Andersen)
! Fixed missing initialization of empty columns in some scenarios.


2014-02-19 (Finn Schiermer Andersen)
! Fixed space leak in group_writer. Could in some scenarios cause massive increase in database file size.


2014-02-17 (Kristian Spangsege)
+ Adding Table::write() as a means to effieciently serialize a table, or part of a table.
! Fixing table copy bug. The error occured when the table contained strings longer than 64 bytes.
! Fixing table comparison bug. The error occured when the table has a `float` or a `double` column.


2014-02-14 (Kristian Spangsege)

* New test suite with support for parallelized testing replaces UnitTest++. See section 'Testing' in `README.md`.
+ The StringData class now distinguishes `null` from the empty string.
+ Adding StringData::is_null().


2014-02-11 (Kristian Spangsege)
+ Group::write(std::ostream&) added. This allows general online streaming of Realm databases in memory for the first time.
+ Adding Table::get_name() which returns the name of the table when the table is a direct member of a group.


2014-02-05 (Kenneth Geisshirt)
+ Two new targets in build.sh: get_version and set_version.


2014-02-04 (Kristian Spangsege)
+ Introducing Table::get_parent_table() with allows a subtable to know its parent table.
+ Table::rename_subcolumn() and Table::remove_subcolumn() now take an extra argument which is the index of the column to rename. Before the index was specified as part of the path.
+ Introducing Table::insert_column() and Table::insert_subcolumn() for inserting new columns at arbitrary positions.
+ New class `Descriptor` introduced into the public API. It plays the role that `Spec` played before. Class `Spec` is no longer to be considered part of the public API.
+ Table::add_column() now takes a third optinal argument `DescriptorRef* subdesc`.
+ Introducing Table::get_descriptor() and Table::get_subdescriptor()
- Table::get_spec() and Table::update_from_spec() removed from public API since are part of the now non-public `Spec` API.
. Table::has_shared_spec() renamed to Table::has_shared_type().


2014-01-27 (Brian Munkholm)
! Fixed bug in Query with subtables. Whith empty subtables query returned incorrect results.
  In debug mode it could assert when querying a subtable with more columns than the base table.

2014-01-23 (Kenneth Geisshirt)
! Fixed bug: Subtable queries is validated by Query::validate(). An invalid subtable query can lead to a segfault.


2014-01-07 (Kenneth Geisshirt)
+ Table::range() added. The method returns a set of rows as a TableView.


2014-01-06 (Kristian Spangsege)
! 'No parent info in shared specs' conflicts with implementation of `Group::commit()`.
! `ColumnTable::m_spec_ref` not updated when Spec object is reallocated.
! `ColumnSubtableParent::m_index` not updated when preceeding columns are removed.
+ Addition of `template<class L> std::size_t Table::add_subcolumn(const util::Tuple<L>&, DataType, StringData)`. This makes it much easier to add columns to subtable specs.
. `Spec::get_subtable_spec()` now returns `SubspecRef` or `ConstSubspecRef`. This fixes a design bug relating to propagation of constness to subspecs, and it improves the efficiency of access to subspecs by avoiding expensive copying of `Spec` objects.
. Elimination of `Spec::m_table` and `ColumnTable::m_spec_ref`.
. `Spec::add_column()` and `Spec::add_subcolumn()` now take a `Table*` as argument.


2013-12-17 (Kristian Spangsege)
+ Implicit termination of active transaction when SharedGroup is destroyed.
. Class `File` and related exceptions such as `File::AccessError` moved to namespace `realm::util`.
. Table::add_column() optimized. For integer columns, the speedup is by more than a factor of 1000 for large tables.


2013-11-07 (Alexander Stigsen)
. BREAKING CHANGE: Schema now handles attributes separately for better performance when there are many colummns.


2013-11-07 (Lasse Reinhold)
+ Added power() operator for next-generation-queries


2013-11-07 (Lasse Reinhold)
! Fixed bug: ng-queries could segfault to bug in Array::get_chunk(). Normal queries and everything else not affected.


2013-10-10 (Kenneth Geisshirt)
. Adding INTERACTIVE mode for the dist-config target in build.sh


2013-10-09 (Kenneth Geisshirt)
. Adding dist-deb target in build.sh for building debian/ubuntu/mint packages. Moreover, the ubuntu/mint version is part of package name so maintaining repositories is easier.


2013-09-26 (Brian Munkholm)
+/- Renamed Table::distinct() to Table::get_distinct_view()
+/- Renamed class Date to DateTime. Renamed DataType::type_Date to type_DateTime
+/- Renamed suffix of all methods operating on DateTime from '_date' to '_datetime'.


2013-09-26 (Kristian Spangsege)
+ File format support for streaming of output from Group::write() (not yet suported by API.)
+ Support for internal null characters in strings. This applies to table and column names as well.


2013-09-19 (Kristian Spangsege)
. CRUD performance has been greatly improved for large tables, as long as they are kept on the "compact" form. A table is kept on the compact form when every row insertion and removal, since the creation of that table, has occured, and continues to occur at the end (i.e., insert after last row, and remove last row).


2013-10-02 (Lasse Reinhold)
- Renamed find_next(lastmatch) into find(begin_at_table_row) for queries and typed tables.


2013-09-12 (Brian Munkholm)
+ Added TableView::row_to_string() and testcases for to_string() and row_to_string()
+ Added row_to_string(), to_string() and to_json() in typed TableView.


2013-08-31 (Kristian Spangsege)
+ Database files are now exanded by doubling the size until it reaches 128 MiB. After that, it is expanded in chunks of 128 MiB. Until now, it was always expanded in chunks of 1 MiB.


2013-08-28 (Kristian Spangsege)
+ Table::is_valid() renamed to Table::is_attached(). The notion of accessor attachment describes much better what is going on than the notion of validity.


2013-08-23 (Kristian Spangsege)
+ Stop throwing from destructors (all), and from SharedGroup::rollback() and SharedGroup::end_read().
+ General stability and error checking improvements.
! Fixed many bugs relating to Group::commit().
! Fixed some bugs relating to SharedGroup::commit().
! Fixed bug in TableViewBase::sort().


2013-08-18 (Kenneth Geisshirt)
! Group::to_string() formatting was incorrect. See https://app.asana.com/0/1441391972580/5659532773181.


2013-08-03 (Kristian Spangsege)
+ Table::find_sorted_int() replaced by Table::lower_bound_int() and Table::upper_bound_int() as these are standardized and provide more flexibility.
+ Addition of Table::lower_bound_bool() and Table::upper_bound_bool().
+ Addition of Table::lower_bound_float() and Table::upper_bound_float().
+ Addition of Table::lower_bound_double() and Table::upper_bound_double().
+ Addition of Table::lower_bound_string() and Table::upper_bound_string(). They rely on simple byte-wise lexicographical comparison. No Unicode or locale dependent collation is taken into account. Comparison occurs exactly as defined by std::lexicographical_compare in the C++ STL.


2013-07-19 (Dennis Fantoni)
+ Added Table::set_subtable(size_t column_ndx, size_t row_ndx, const Table*)


2013-06-25 (Kristian Spangsege)
. The default group open mode has been changed from Group::mode_Normal
  (read/write) to Group::mode_ReadOnly. This makes it possible to open
  a read-only file without specifying a special open mode. Also, since
  changed groups are generally written to new files, there is rarely a
  need for the group to be opened in read/write mode.
. Group::mode_Normal has been renamed to Group::mode_ReadWrite since it is no longer a normal mode.
. Group::mode_NoCreate has been renamed to Group::mode_ReadWriteNoCreate for clarity.


2013-06-05 (Kristian Spangsege)
. Group::write(path) now throws File::Exists if 'path' already exists in the file system.


2013-05-16 (Kristian Spangsege)
+ New SharedGroup::reserve() method added.


2013-05-13 (Kenneth Geisshirt)
. Added "uninstall" target in build.sh for simple uninstallation.


2013-05-07 (Kristian Spangsege)
. Exception File::OpenError renamed to File::AccessError. This affects
  various methods in Group and SharedGroup.


2013-04-23 (Kristian Spangsege)
+ Support for explicit string lengths added. Most method arguments and
  return values of type 'const char*' have been changed to be of type
  'StringData'. This new type is defined in
  <realm/string_data.hpp>. 'StringData' can be implicitly
  constructed from 'const char*', so no change is required when
  passing arguments. Source code change is required when dealing with
  returned strings of type 'const char*'. The following is a complete
  list:
    Affected form                      Possible replacement
    ---------------------------------------------------------------------------
    group.get_table_name(...)          group.get_table_name(...).data()
    table.get_column_name()            table.get_column_name().data()
    table.get_string(...)              table.get_string(...).data()
    table.get_mixed(...).get_string()  table.get_mixed(...).get_string().data()
    table[7].string_col                table[7].string_col.c_str()
+ Added support for table[7].string_col.data() and table[7].string_col.size().
+ Full and seamless interoperability with std::string. This comes
  about from the fact that StringData can be implicitly constructed
  from, and convert to std::string.
+ Full support for BinaryData in queries.
+ Added BinaryData::data(), BinaryData::size(), BinaryData::operator[]()
+ Added BinaryData::operator==(), BinaryData::operator!=(), BinaryData::operator<()
+ Added BinaryData::begins_with(), BinaryData::ends_with(), BinaryData::contains()
+ Allow BinaryData to be constructed from fixed size array:
  template<std::size_t N> explicit BinaryData(const char (&)[N])
- BinaryData::pointer removed, use BinaryData::data() instead.
- BinaryData::len removed, use BinaryData::size() instead.
- BinaryData::compare_payload() removed, use BinaryData::operator==() instead.
+ The methods
    Table::set_binary(std::size_t column_ndx, std::size_t row_ndx, const char* data, std::size_t size)
    Table::insert_binary(std::size_t column_ndx, std::size_t row_ndx, const char* data, std::size_t size)
    Table::find_first_binary(std::size_t column_ndx, const char* data, std::size_t size)
    Table::find_all_binary(std::size_t column_ndx, const char* data, std::size_t size)
    TableView::set_binary(std::size_t column_ndx, std::size_t row_ndx, const char* data, std::size_t size)
  have been changed to
    Table::set_binary(std::size_t column_ndx, std::size_t row_ndx, BinaryData)
    Table::insert_binary(std::size_t column_ndx, std::size_t row_ndx, BinaryData)
    Table::find_first_binary(std::size_t column_ndx, BinaryData)
    Table::find_all_binary(std::size_t column_ndx, BinaryData)
    TableView::set_binary(std::size_t column_ndx, std::size_t row_ndx, BinaryData)
  The following changes have been made in the statically
  typed API:
    Affected form                  Possible replacement
    ---------------------------------------------------------
    table[7].binary_col.pointer()  table[7].binary_col.data()
    table[7].binary_col.len()      table[7].binary_col.size()
  These changes were made for consistency with StringData.
+ Added Date::operator<()
+ Return type changed from 'std::time_t' to 'Date' on the following
  methods:
    Mixed::get_date()
    Table::get_date()
    TableView::get_date()
  Argument type changed from 'std::time_t' to 'Date' on many methods including these:
    Mixed::set_date()
    Table::set_date()
    Table::insert_date()
    TableView::set_date()
  Changes corresponding to these have been made in the statically
  typed API. These are some of the affected forms:
    time_t(table[7].date_col)
    table[7].date_col = val
    table[7].mixed_col.get_date()
  These changes were made for consistency, and to improve the
  isolation of the implementation of 'Date' (it is likely that the
  implementation of 'Date' will change). 'Date' can be implicitly
  constructed from std::time_t, but it cannot be implicitly converted
  to std::time_t (nor is it considered desireable to allow such an
  implicit conversion). This means that applications will be affected
  as follows:
    Affected form                    Possible replacement
    ---------------------------------------------------------------------------
    table.get_date(...)              table.get_date(...).get_date()
    table.get_mixed(...).get_date()  table.get_mixed(...).get_date().get_date()
    time_t(table[7].date_col)        Date(table[7].date_col).get_date()
    table[7].date_col.get()          table[7].date_col.get().get_date()
+ Group::write() and Group::write_to_mem() are now 'const'.
+ Group::BufferSpec eliminated. Using BinaryData instead.


2013-03-11 (Kristian Spangsege)
+ On Linux and OS X, installed shared libraries now carry a platform
  dependent API version which is computed from a platform neutral
  version specifier (same as GNU Libtool). This allows for multiple
  versions of the shared library to be concurrently installed.


2013-02-24 (Kristian Spangsege)
+ Adding copy constructors for Table and BasicTable.
+ Adding Table::copy(), BasicTable::copy() and LangBindHelper::copy_table().
+ Adding BasicTable::create() for symmetry with Table::create().


2013-02-21 (Brian Munkholm
-+ Renamed Group::get_table_count() to Group::size()


2013-02-19 (Kristian Spangsege)
+ Type of Group::BufferSpec::m_data changed from <char*> to <const char*>.


2013-02-06 (Kristian Spangsege)
+ New enum DataType replaces ColumnType throughout the public API.


2013-01-27 (Kristian Spangsege)
+ New Group::Group(unattached_tag) added. Same for SharedGroup.
+ New Group::open(...) methods added. Same for SharedGroup.
+ New Group::is_attached() added. Same for SharedGroup.
+ Classes ReadTransaction and WriteTransaction added for handling safe scoped transaction.
+ Many methods have now been qualified with REALM_NOEXCEPT.


2013-01-14 (Kristian Spangsege)
- Group::set_shared() removed.
- SharedGroup::is_valid() removed. Errors are now reported as exceptions from the constructor.


2013-01-11 (Kristian Spangsege)
+ Simplified open-mode for Group constructor.
- Group::is_valid() removed. Errors are now reported as exceptions from the constructor.
+ Now using Group::BufferSpec to pass a memory buffer to the Group constructor.
+ Group::write_to_mem() now returns a Group::BufferSpec.
+ Addition of 'bool no_create' arguemnt to SharedGroup constructor.


2013-01-08 (Kristian Spangsege)
+ Mixed::set_int() added (same for other value types except subtables).
+ Removed two-argument Mixed constructor for binary data since its signature is expected to be used for strings that are not zero-terminated.


2013-01-08 (Brian Munkholm)
----------
+ New: Added a bunch of methods to support two new column types: float and double.


2012-12-16 (Kristian Spangsege)
----------
+ my_table[i].foo.get() added for all column types except subtables. This is to avoid having to repeat the explicit column type in cast expressions when the actual value is needed.
+ my_table[i].foo.set(...) added for all column types except subtables. This is for completeness.
+ When passing a file name to a Group or a SharedGroup constructor, the type is now a std::string. This is made possible by allowing exception handling. It simplifies the implementation in a few places, and in general it simplifies application code.
+ A 'tag' argument has ben added to the Group constructor that takes a memory buffer as argument. Without this change, two Group constructors were way too similar.


2012-12-06 (Brian Munkholm)
----------
+ 16 New Table:get_string_length(col_ndx, row_ndx) added in Dynamic Table. Missing in Typed Table.


2012-12-06 (Kristian Spangsege)
----------
. "C" API moved to its own repository "realm_c".


2012-12-03 (Brian Munkholm)
----------
+ 15 Updated Group() constructor to take an optional 3'rd parameter 'take_ownership', which allows the caller to keep owenership of the provided data: Group::Group(const char* buffer, size_t len, bool take_ownership=true).


2012-11-13 (Kristian Spangsege)
----------
+ 14 Renamed Table::count() to Table::count_int()


2012-11-21
----------
+ Added ShareGroup::has_changed() to detect if there has been changes to the db since last transaction.


2012-11-12 (Kristian Spangsege)
----------
! Fixed a memory leak when using Table::create()


2012-10-24 (Kristian Spangsege)
----------
+ 13 Added Table::has_shared_spec().


2012-10-10 (Kristian Spangsege)
----------
! Fix a problem with MyTable::Query copy constructor that caused access to deallocated memory due to a pointer that was not updated.


2012-10-02 (Kristian Spangsege)
----------
+ New program 'realm-config'. Use it to query about the CFLAGs and/or LDFLAGs to use when linking agains the Realm core library.


2012-10-01 (Brian Munkholm)
----------
+ 12 Added double Table::average(size_t column_ndx) const


2012-09-07 (Alexander Stigsen)
----------
+ File format updated with bigger header and reordered column type [BREAKING]
+ Index is now enabled for string columns (handles string_enum columns as well).
+ 11 Added Table::count_string(size_t column_ndx, const char* target) const;
+ 11 Added Table accessor size_t count(const char* target) const to highlevel interface
+ 11 Spec::add_column(...) now takes an optional parameter for attribute (like indexed).
+ 11 Added Table::to_string() and Group::to_string() for prettified string dump.


2012-08-14 (Brian Munkholm)
----------
+ 10 Renamed FindAllMulti() to find_all_multe(). And SetThreads() to set_threads()
+ 10 Renamed cols() to column().
+ 10 Renamed get_subspec() to get_subtable_spec().
+ 10 Renamed parent() to end_subtable().


2012-08-01 (Kristian Spangsege)
----------
+ 9 Date::operator==(const Date&) and Date::operator!=(const Date&) added.
+ 9 BinaryData::operator==(const BinaryData&) and BinaryData::operator!=(const BinaryData&) added.
+ 9 Operators added for comparison between a value of type Mixed and a value of one of the possible types that a Mixed can contain. Operators are added for both orders of the two arguments.
+ 8 Comparison operators added for "foo" == my_table[i].str and "foo" != my_table[i].str. We already had a comparison operator for the reverse order case, my_table[i].str == "foo".
+ 7 my_table[i].mixed.get_subtable_size() added.


2012-07-27 (Kristian Spangsege)
----------
+ 6 realm::is_a<MyTable>(const Table&) added.
+ 6 realm::unchecked_cast<MyTable>(TableRef) added.
+ 6 realm::checked_cast<MyTable>(TableRef) added.
+ 6 my_table[i].mixed.set_subtable() added.
+ 6 my_table[i].mixed.set_subtable<MySubtable>() added.
+ 6 my_table[i].mixed.is_subtable<MyTable>() added (inefficient, do we want it at all?).
+ 6 my_table[i].mixed.get_subtable<MySubtable>() added (unsafe/unchecked, do we want it at all?).


2012-07-24 (Kristian Spangsege)
----------
+  New macro REALM_DEBUG to control compilation mode.
    The library (including all headers) is no longer affected in any way by the definition status of NDEBUG or _DEBUG.
    When we (Realm) compile the library in debug mode, we must define this macro.
    We will deliver two versions of the library, one for release mode, and one for debug mode.
    If the customer wishes to use the debugging version of the library, he must do two things:
    1) Define REALM_DEBUG in any translation unit that includes a Realm header.
    2) Use the version of the library that is compiled for debug mode (librealm_d.a).
+ 5 Removed obsolete constructor Mixed(ColumnType). Use Mixed(subtable_tag) instead, since this has no runtime overhead.


2012-07-19 (Kristian Spangsege)
----------
+ 4 Table::create() added. Use this to create a freestanding top-level table with dynamic lifetime (determined by reference counting).
+   TableRef::reset() added to set a table reference to null.


2012-07-15 (Kristian Spangsege)
----------
+ 3 Spec::compare() renamed to Spec::operator==(), and made generally available, not only while compiling in debug mode.
+ 3 Spec::operator!=() added.
+ 3 Table::compare() renamed to Table::operator==(), and made generally available, not only while compiling in debug mode.
+ 3 Table::operator!=() added.
+ 3 MyTable::compare() renamed to MyTable::operator==(), and made generally available, not only while compiling in debug mode.
+ 3 MyTable::operator!=() added.
+ 3 Group::operator==() and Group::operator!=() added.
. Array::Compare() and Column::Compare() made generally available, not only while compiling in debug mode.


2012-07-09 (Kristian Spangsege)
----------
+ 1 Table::is_valid() added. Most language bindings must check this flag before calling any member function on any table.
+ 1 MyTable::is_valid() added.
+   See documentation for Table::is_valid() for more details on when a table becomes invalid, and when it does not.
+   Destroying a Group will invalidate all table wrappers (instances of Table) as well as all direct and indirect subtable wrappers.
+   Any modifying operation on a table will generally invalidate all direct and indirect subtable wrappers.
+ 2 my_table[i].mixed.is_subtable() added.
+ 2 my_table[i].mixed.get_subtable() added.


2012-07-08 (Kristian Spangsege)
----------
. LangBindHelper::new_table() now returns null on memory allocation error. This may change in the future to instead throw an exception.


2012-06-27
----------
-+ Table::sorted(...) changed name to get_sorted_view(...)
- Removed Table::find_pos_int(...) from public API

+ Added a the following methods to a TableView:
    template<class E> void set_enum(size_t column_ndx, size_t row_ndx, E value);
    ColumnType  get_mixed_type(size_t column_ndx, size_t row_ndx) const;
    size_t      get_subtable_size(size_t column_ndx, size_t row_ndx) const;
    void        clear_subtable(size_t column_ndx, size_t row_ndx);
    size_t      find_first_bool(size_t column_ndx, bool value) const;
    size_t      find_first_date(size_t column_ndx, time_t value) const;
    void        add_int(size_t column_ndx, int64_t value);
    TableView      find_all_bool(size_t column_ndx, bool value);
    ConstTableView find_all_bool(size_t column_ndx, bool value) const; (for class TableView and ConstTableView)
    TableView      find_all_date(size_t column_ndx, time_t value);
    ConstTableView find_all_date(size_t column_ndx, time_t value) const; (for class TableView and ConstTableView)

2012-06-??
----------
- Group() interfaced changed. Now with multiple options. default option changed from readonly...
+ Generated C++ highlevel API for tables with up to 15 columns
