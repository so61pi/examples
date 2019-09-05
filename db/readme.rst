.. contents:: Table of Contents

Versioning
==========

Migration Types
---------------

- Have both up and down paths.

  * Down path can be helpful when we evaluate different strategies of updating schema. 
  * Not all changes are reversible. If a change affects not only the schema, but also the data (e.g. ``DROP TABLE``), it is irreversible.

- Have only up path.

Versioning Tactics
------------------

Migration Script Naming
~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: none

    <YYYY><MM><DD><HH><MM><SS>_<COMMENT>

    db/migrations/20160815133237_create_posts/up.sql
    db/migrations/20160815133237_create_posts/down.sql

Tracking
~~~~~~~~

.. code-block:: sql

    CREATE TABLE schema_changes (
      id SERIAL PRIMARY KEY,
      version TEXT NOT NULL, -- <YYYY><MM><DD><HH><MM><SS>_<COMMENT>
      date_applied TIMESTAMP WITH TIME ZONE
    )

Versioning
~~~~~~~~~~

- All changes are migration scripts.

  * Releasing

    Releases are finalized on release branches.

    In case we need to change the database schema on the release branch, we create a migration script for that on release branch (v2), and also pick it to the dev branch. Note that the picked version on dev must not conflict with the original one, otherwise we cannot upgrade our application from v2 (contains original version) to v3 (contains picked version).

    This is simple, but we cannot remove old migration scripts as all new installations invoke all of those scripts. It can also lead to errors as the number of scripts grows.

  * Applying migrations

    .. code-block:: none

        applicable change scripts = source code - schema_changes
        apply change scripts

- One baseline script for each application version.

  * Releasing

    Releases are finalized on release branches.

    When new application version is about to be released, new baseline (v2) is created on release branch (v2). The content of new baseline must be equivalent to all migration scripts plus previous baseline (v1). New empty migration script that denotes new schema version (v2) should be checked in to main development branch.

    Old migration scripts must be kept, so already installed instances of application can be upgraded to new version. Baselines are for new installs only.

    (Note: Baselines can be thought as big initial migration scripts, so they should have versions like normal migration scripts too. This also means each baseline must have a corresponding empty migration script to mark the version so when user upgrade (instead of installing) from old version to new one we still see the correct version number in ``schema_changes``).

  * Applying migrations

    .. code-block:: none

        if schema_changes table does not exist {
          apply baseline
        }
        applicable change scripts = source code - schema_changes
        apply change scripts

When merging migration scripts from feature branch to main development branch, we must make sure the scripts on the source branch have higher versions than the ones in the dest branch.

References
----------

- https://odetocode.com/blogs/scott/archive/2008/02/03/versioning-databases-branching-and-merging.aspx
