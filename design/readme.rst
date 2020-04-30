.. contents:: Table of Contents

Cross-cutting Concerns
======================

Database
--------

Database connections are usually placed in a pool. When our application needs one, it gets one from the pool and lock until one is avaiable for use. The way a connection is passed around during request handling depends on the execution model of our application:

- With thread-based applications, we can add db connections to thread local storage when we receive the request, use it throughout handling process, then return it back to the thread pool after done.
- With async-based applications, we pass it around as a parameter, from outer functions to inner ones.

Logging
-------

Logging can have multiple levels of context such as commit hash, host id, app start time, user request id...

We can also place a random unique id into data that is passed between systems to trace the flow. Let's say we have a system that when a user registers, we send to them a confirmation email. If email sending is in an external service, then adding a random id to data passed by main app to email sending application can make troubleshoot faster.

The way we pass and add more context to log is similar to database connection.

App options
-----------

It is tempting to put application options (read from config file or command line) to a global read-only variable, but this will make unit testing more complicated as we cannot run tests parallelly.

The way we pass app options is similar to database connection.

Global Resources
================

Time
----

When a function calls time-related functions, it is harder to test it. The reason is the result of given function now doesn't completely depend on its inputs alone.

There are two strategies to solve this:

- Pass required time value as inputs of function (e.g. current time).
- Create a mockable time object and use it instead of calling system provided time functions directly.

Filesystem
----------

This is similar to time, but less arbitrary. Most of the time, it is fine to use system provided functiona. However, in case we want to mock it for testing purpose, there can be two approaches:

- Create one big mockable object that is used across all functions.
- Create multiple small mockable objects, each is for one function.

Error Handling
==============

- Consider errors as part of function's signature.

- Prefer structured errors (error codes, ...) to text errors.

  + Improve error handling code. Instead of searching through text which is error prone, we compare error code.
  + Easier to customize output to different receivers (humans with i18n, or machines which mostly care about error codes).
  + Simpler to aggregate and analyze later.
  + Code searching is also more convenient.

- All errors of a given function should be enclosed in an enum instead of being implicitly formed by a set of independent constants.

  .. code-block:: rust

      // Bad (but sometimes required)
      const ERROR_INVALID_INPUT: i32 = 1;
      const ERROR_MISSING_INPUT: i32 = 2;

      fn call() -> Result<(), i32> {}

      // Good
      enum Error {
          InvalidInput,
          MissingInput,
      }

      fn call() -> Result<(), Error> {}

- Avoid gigantic error enum that can be used everywhere.

  It makes error handling easier, as the default strategy is to handle all possible errors returned from a functions instead of reading the document extremely carefully. It'll also be faster to add new type of error because we can trace all users of a particular error enum quicker.

  Hence, we should have multiple error enums and do mapping/grouping between them. Mapping/grouping usually happens when an error from lower layers is passed to higher layers.

  Usually we group fatal errors that users cannot really do anything about without making changes outside of our system (e.g. db or io errors) into one enum variant. Other types of errors. each can be mapped to a seperate variant (e.g. missing input, invalid input).

  Nesting low level error enums into higher ones is also an option.

  .. code-block:: rust

      enum SystemError {
          IoError(io::Error), // external enum
          DbError(db::Error), // external enum
      }

      enum ApplicationError {
          InvalidInput,
          MissingInput,
      }

      enum HighLevelError {
          ApplicationError(ApplicationError),
          SystemError(SystemError),
      }

- Add fail points to conditionally trigger errors of a function during testing.

  + https://docs.rs/fail/0.4.0/fail/
  + http://sled.rs/errors

  This can be combined with property based testing (which generates random inputs). To make failed tests reproducible, we should save the seed that generates random data when a test fails. Also, other random behaviors must be switched to deterministic.

  + https://dropbox.tech/infrastructure/-testing-our-new-sync-engine

- Separate expected errors and unexpected errors.

  Let's say we write a function to read configurations from a file, it usually has three outcomes (assuming that the configuration file is in good format). The first one is the configurations, second is file not found error, and third one is IO error. Often, we stop the program when we encounter the third error, but use the default configuration in second case.

  So if we use ``Result<Config, MyError>`` as the return type, the caller would look like this

    .. code-block:: rust

        let config = match get_config() {
            Ok(config) => config,
            Err(MyError::FileNotFound) => get_default_config(),
            Err(MyError::IoError) => return error to caller
        };

  Instead, we can separate the errors, using ``Result<Result<Config, FileNotFoundError>, IoError>``. It looks more cumbersome, but now we can pass the unexpected error to upper level easier.

    .. code-block:: rust

        let config = get_config()?; // pass IoError up the stack, may add some context like "reading config from file"
        let config = config.unwrap_or_else(|_| get_default_config());

- Function calls and errors flow

  .. code-block:: text

           calls      request      errors
      concrete |         1         ^ general
               |       _/ \_       |
               |     2       5     |
               |    / \     / \    |
               |   3   4   6   7   |
       general v                   | concrete

  Therefore, keeping context of errors is crucial to construct an understandable error message.

Database
========

- Make sure an object doesn't have more than one owner. And this works all the way down.
- Database with big volume of data can be partitioned/sharded, but that can effectively make its schema frozen.
- For moving objects, instead of actually moving, we can mark old object as deleted, and create new version of that same object.
- Data versioning + schema versioning.

Upgrading/Migrating
-------------------

Migration Types
~~~~~~~~~~~~~~~

- Have both up and down paths.

  * Down path can be helpful when we evaluate different strategies of updating schema. 
  * Not all changes are reversible. If a change affects not only the schema, but also the data (e.g. ``DROP TABLE``), it is irreversible.

- Have only up path.

Versioning Tactics
~~~~~~~~~~~~~~~~~~

Migration Script Naming
```````````````````````

.. code-block:: none

    <YYYY><MM><DD><HH><MM><SS>_<COMMENT>

    db/migrations/2019-08-15-133237_create_posts/up.sql
    db/migrations/2019-08-15-133237_create_posts/down.sql

When migrations are run, all migrations that are not in database are run in alphabetical order. In case there are migration scripts with lower timestamp than in database but they are not in database yet, they will still be executed. Hence, keeping the order of the scripts at merge time is important.

For example, in our current dev database we have version `2019-08-15-050000`, then some one merges `2019-08-15-040000` in. Now if we install new instance of our application, `2019-08-15-040000` will be run before `2019-08-15-050000`. But for upgrading current dev database, `2019-08-15-040000` is after already existing versions. This may or may not lead to unexpected consequence.

Tracking
````````

.. code-block:: sql

    CREATE TABLE schema_changes (
      id SERIAL PRIMARY KEY,
      version TEXT NOT NULL, -- <YYYY><MM><DD><HH><MM><SS>_<COMMENT>
      date_applied TIMESTAMP WITH TIME ZONE
    )

Versioning
``````````

- All changes are migration scripts.

  * Releasing

    Releases are finalized on release branches. This scheme is suitable when we control all installations.

    In case we need to change the database schema on the release branch, we create a migration script for that on release branch (v2), and also pick it to the dev branch. Note that the picked version on dev must not conflict with the original one, otherwise we cannot upgrade our application from v2 (contains original version) to v3 (contains picked version). Specifically, the up path of picked version must be a no-op in case original one is already applied, and the down path of original must be a no-op if the picked version is already reverted.

    This is simple, but we cannot remove old migration scripts as all new installations invoke all them. It can also lead to errors as the number of scripts grows.

  * Applying migrations

    .. code-block:: none

        applicable change scripts = migrations in source code - schema_changes
        apply change scripts

- One baseline script for each application version.

  * Releasing

    Releases are finalized on release branches. This scheme is suitable when users install the software on their own.

    - Imagine we have some migrations like this on master.

      .. code-block:: none

          src/
          └── db/
              ├── baseline-details/
              └── migrations/
                  ├── 0000-00-00-000000_baseline/             <-- empty at this point
                  ├── 2019-08-15-000000_v1/                   <-- only a marker, always empty
                  ├── 2019-08-16-000000_add_table/
                  ├── 2019-08-17-000000_add_column/
                  └── 2019-08-18-000000_change_column_type/

    - Then we add a new commit with empty v2 migration script to clearly separate v1 and v2. It also will help when we merge release branch back to master.

      .. code-block:: none

          src/
          └── db/
              ├── baseline-details/
              └── migrations/
                  ├── 0000-00-00-000000_baseline/
                  ├── 2019-08-15-000000_v1/
                  ├── 2019-08-16-000000_add_table/
                  ├── 2019-08-17-000000_add_column/
                  ├── 2019-08-18-000000_change_column_type/
                  └── 2019-08-19-000000_v2/                   <-- added on master, always empty

    - Create a release branch from the commit right before the above commit. Then we do all the testing. In case we need to add a new fix with a migration, we add one on both release and master branches (and they must be compatible). Finally, we create a new baseline from all current migrations.

      .. code-block:: none

          src/
          └── db/
              ├── baseline-details/                           <-- move all v1 migrations to baseline-details
              │   │                                               this means current baseline equals to baseline-details
              │   ├── 2019-08-15-000000_v1/
              │   ├── 2019-08-16-000000_add_table/
              │   ├── 2019-08-17-000000_add_column/
              │   └── 2019-08-18-000000_change_column_type/
              └── migrations/
                  ├── 0000-00-00-000000_baseline/             <-- update to new baseline = 2019-08-16-000000 + 2019-08-17-000000 + 2019-08-18-000000
                  │                                               can be generated by exporting schema from database.
                  └── 0000-00-00-000000_v1_new_install/       <-- used to mark new installs from this particular baseline

      Note that baseline is only used for new install. If upgrading is an option, we also want to create a migration package that holds all migrations in a particular version. For instance, upgrading version 1 to version 2 requires all migrations from v2 to before v3.

    - Now we need to update baseline on master by merging release branch.

    .. code-block:: none

        src/
        └── db/
            ├── baseline-details/
            │   ├── 2019-08-15-000000_v1/
            │   ├── 2019-08-16-000000_add_table/
            │   ├── 2019-08-17-000000_add_column/
            │   └── 2019-08-18-000000_change_column_type/
            └── migrations/
                ├── 0000-00-00-000000_baseline/             <-- new baseline merged back from release branch
                │                                           <-- we don't keep v1_new_install here as it is useless and will make the schema version table in database polluted
                ├── 2019-08-19-000000_v2/
                └── 2019-08-20-000000_add_table/            <-- someone already adds new migration script in v2

  * Applying migrations

    .. code-block:: none

        if schema_changes table does not exist {
          apply baseline
        }
        applicable change scripts = migrations in source code - schema_changes
        apply change scripts

When merging migration scripts from feature branch to main development branch, we must make sure the scripts on the source branch have higher versions than the ones in the dest branch.

We also have to take care of migrations in code, like migrating data with complex logic.

References
~~~~~~~~~~

- https://odetocode.com/blogs/scott/archive/2008/02/03/versioning-databases-branching-and-merging.aspx

Building
========

- Building should be customizable without making worktree dirty (i.e. ``git status`` returns clean).
- Building on build server and in local machine must follow same procedure. Preferably, there is a ``Makefile`` and all it takes to do a full build is running ``make``.

Deployment
==========

Performance
===========

This session focuses on performance issues coming from inside the application. For system performance troubleshooting, please refer to other documents.

Two main reasons our application is slow for a specific operation:

- CPU (on CPU profiling)

  * Doing more work than necessary by accident?

    - Remove redudant works.

  * Processing big data volume?

    - Break data to smaller pieces.
    - Run in a background job.

  * Using an ineffective data structure?

    - Switch to a more effective one.

- IO (off CPU profiling)

  * Spend too much time waiting for data?

    - Do it less often with bigger data volume (e.g. aggregate logs once a week instead of once a day).

Code
====

Type
----

Concepts
~~~~~~~~

Basic
`````

    - An **abstract entity** is an individual thing that is eternal and unchangeable (e.g. blue, 13).

`````

    - A value type is **properly partial** if its values represent a proper *subset of the abstract entities* in the corresponding species; otherwise it is **total**.
    - A value type is **uniquely represented** if and only if *at most one value corresponds to each abstract entity*.
    - A value type is **ambiguous** if and only if *a value of the type has more than one interpretation*. The negation of ambiguous is **unambiguous**.

`````

    - Two values are **equal** if and only if they represent a same abstract entity.
    - They are **representationally equal** if and only if their datums are identical sequences of 0s and 1s.

      * If a value type is **uniquely represented**, *equality implies representational equality*.
      * If a value type is **unambiguous**, *representational equality implies equality*.

Computational Basis
```````````````````

    A **computational basis** for a type is a finite set of procedures that enable the construction of any other procedure on the type.

    A basis is **efficient** if and only if any procedure implemeted using it is as efficient as an equivalent procedure written in terms of an alternative basis.

    A basis is **expressive** if and only if it allows compact and convenient definitions of procedures on the type.

Regular Types
`````````````

    A type is **regular** if and only if its basis includes

    - equality
    - assignment
    - destructor
    - default constructor
    - copy constructor
    - total ordering

Whole-Part Relationship
```````````````````````

    A **whole-part relationship** is connected, non-circular, logically disjoint, and owned.

    **Connected** means any part is reachable from the object's starting address.

    **Non-circular** means that no part is a part of itself.

    **Logically disjoint** means that if two objects share a subpart where modifications to
    the subpart affect the value of both objects, then one of the objects must be a subpart
    of the other.

    **Owned** means that copying the object copies its parts, and destroying the object destroys
    its parts.

**Non-circular** actually means if A has an owning pointer to B then B cannot have an owning pointer to A.
(B still can have non-owning pointer to A though.)

**Logically disjoint** actually means if A and B both point to C and we are able to change C then A must contain B or vice versa.
In other words, we cannot have 2 independent objects sharing a same modifiable object. (Note that if the shared object
is immutable, then the relationship is always logically disjoint.)

Relations
`````````

``r(a: T, b: T) -> bool``

Transitive
''''''''''

::

    r(a, b) && r(b, c) => r(a, c)

Example:

- equal
- less
- greater

Strict
''''''

::

    !r(a, a)

Example:

- less
- greater

Reflexive
'''''''''

::

    r(a, a)

Example:

- equal

Symmetric
'''''''''

::

    r(a, b) => r(b, a)

Example:

- equal

Asymmetric
''''''''''

::

    r(a, b) => !r(b, a)

Example:

- less

Equivalence
'''''''''''

::

    Transitive + Reflexive + Symmetric

Example:

- equal

Total Ordering
''''''''''''''

::

    Transitive && only one of {r(a, b); r(b, a); a == b}

Example:

- less
- greater

Weak Ordering
'''''''''''''

::

    Transitive && only one of {r(a, b); r(b, a); !r(a, b) && !r(b, a)}

Properties:

- Total ordering is weak ordering
- Weak ordering is asymmetric
- Weak ordering is strict

General Checks For Type ``T``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#. Does ``T`` have only one single purpose?
#. What is ``T``'s invariant?
#. Does ``T``'s **set of valid values** equal to **the product of all sets of valid values of T's components**?

   - How are the undesirable combinations kept out?
   - Can types that are more specialized/fundamental be used for ``T``'s components instead?

#. Does ``T`` have any special value (e.g. when all of its components are zero or null)?

   - How are they handled?

#. Does ``T`` have all of the below properties?

   - Regular
   - Whole-part relationship

#. What is the precondition of each method?
#. Is there any special order for method invocation (e.g. ``method_a`` must be called before ``method_b``)?
#. Does ``T``'s computational basis have all of the below properties?

   - Efficient
   - Expressive
   - Complete (able to construct and operate on any representable value)

#. Are ``T``'s members organized in a way that has the following properties?

   - Closely related members are placed together.
   - Total size of ``T`` is smallest.
   - False sharing is prevented.

References
~~~~~~~~~~

- Elements of Programming by Alexander A. Stepanov and Paul McJones 
- `Goal: Implement Complete & Efficient Types <https://sean-parent.stlab.cc/papers-and-presentations/#goal-implement-complete--efficient-types>`__
- `Better Code: Data Structures <https://sean-parent.stlab.cc/papers-and-presentations/#better-code-data-structures>`__
- `Better Code <https://sean-parent.stlab.cc/papers-and-presentations/#better-code>`__

Function
--------

``F`` is function under inspection.

Overview
~~~~~~~~

#. Does ``F`` have only one single purpose?

Inputs/Outputs
~~~~~~~~~~~~~~

#. Does ``F``'s set of valid values equal to the product of all sets of valid values of ``F``'s inputs?

   - How are undesirable combinations avoided?
   - Can types that are more abstract be used for inputs instead?

#. Do ``F``'s inputs have any special combination?
#. Are ``F``'s inputs placed in **(in, in-out)** order?
#. Do ``F``'s inputs follow below convention?

   - In inputs that ``F`` consumes are passed by value.
   - In inputs that ``F`` does not consume are passed by const reference.
   - In-out inputs are passed by non-const reference.
   - Optional inputs should use ``Optional`` class (or something similar) instead of pointer.

     * Except for string or vector as their empty state can be treated as optional, but it should be documented.

#. Are ``F``'s outputs placed in **(out, error)** order?

Code
~~~~

#. Is ``F`` written in favor of readability instead of performance?
#. Does ``F`` invoke global components (e.g. time, filesystem, database...)?

   - Can those invocations be moved outside to make ``F`` **functional**?

#. Variables

   - Does each variable have only one single purpose?
   - Can any variable have smaller scope?

     * How many lines are there between a variable's declaration and its last usage?

   - Is there any mutable variable that can be replaced with an immutable one?

#. Loops

   - Is there any loop that can be replaced with a well-known algorithm?

     * folding
     * sorting
     * searching
     * selecting

       + https://en.cppreference.com/w/cpp/algorithm/nth_element

     * partitioning
     * rotating

   - Does every loop have only one single purpose?
   - What is the invariant of each loop?
   - Can any loop be moved to its own function?

#. Ifs

   - Is there any ``if`` that handles special case?

     * Can that special case be turned into normal case?

   - Can any ``if`` have smaller scope?
   - Is there any complex ``if`` statement can be refactored to improve readability?

#. Recursion

   - What is the terminating case for this recursive function?

Users POV
~~~~~~~~~

#. How easy is it for users to use ``F`` incorrectly?
#. Can users know and understand all possible errors that can be returned from ``F`` by doing below actions?

   - Reading ``F``'s document.
   - Or reading ``F``'s source code (without reading any other code).

#. Does ``F`` use error messages (stating errors without providing any hint to fix) or help messages?

DTO
---

If we use same object/DTO for creating and updating, they still have different validating rules (e.g. some fields required at creation aren't necessary when updating, default values only applied to creating but not updating).

When validating an object, faster and simpler validating rules should go first (although we should still keep related rules close to each other).

There can be two layers of DTO:

- The first layer is user-facing, which receive data from users without any modification/validation. This type of DTO has higher number of combinations, including invalid continuations.
- The next layer can be constructed after validating the first one, as we don't want to pass a too-relaxed object (in terms of numbers of states/invariant) around.

Retrieving, creating, and updating can have different DTOs, but users should be able to paste result of retrieving to creating/updating without much editing. It helps fast testing, and simplifies client code.

JSON
----

- Meta data versioning (e.g. put a field 'version' into json beside 'type' and 'value' so we can handle multiple version of a same type of object during migrations or supporting old APIs, or adding versio info into 'type' itself).
