\c postgres
 
CREATE FUNCTION fn_noupdate() RETURNS TRIGGER AS $fn_noupdate$
BEGIN
    IF (TG_NARGS > 0) THEN
        RAISE EXCEPTION '%', TG_ARGV[0];
    ELSE
        RAISE EXCEPTION 'cannot update unmodifiable fields';
    END IF;
END;
$fn_noupdate$ LANGUAGE plpgsql;


-- =====================================
-- 0. noupdate
-- =====================================

-- =====================================
-- 1. BEFORE INSERT
-- =====================================

-- =====================================
-- 2. AFTER INSERT
-- =====================================

-- =====================================
-- 3. BEFORE UPDATE
-- =====================================

-- =====================================
-- 4. AFTER UPDATE
-- =====================================

-- =====================================
-- 5. BEFORE DELETE
-- =====================================

-- =====================================
-- 6. AFTER DELETE
-- =====================================



/*******************************************************************************
 workspace
================================================================================
 id | name  | config
----|-------|--------
 1  | root  |
 2  | Alice |
 3  | Bob   |
------------------------------------------------------------------------------*/
CREATE TABLE workspace (
    id      BIGSERIAL PRIMARY KEY,
    name    TEXT NOT NULL UNIQUE CHECK(LENGTH(name) > 0),
    config  JSON NOT NULL DEFAULT '{}'
);

-- reserve id 1
-- we don't specify id here, otherwise postgresql will not update sequence id to 2
-- and we will get key-duplication error
INSERT INTO workspace(name) VALUES ('root');

-- =====================================
-- 0. noupdate
-- =====================================
CREATE TRIGGER workspace_trigger_0_noupdate BEFORE UPDATE OF id
ON workspace
FOR EACH ROW
EXECUTE FUNCTION fn_noupdate('workspace.id is unmodifiable');

-- =====================================
-- 5. BEFORE DELETE
-- =====================================
CREATE FUNCTION workspace_fn_before_delete_action() RETURNS TRIGGER AS $workspace_fn_before_delete_action$
BEGIN
    IF OLD.id = 1 THEN
        RAISE EXCEPTION 'cannot delete id 1';
    END IF;
    RETURN OLD;
END;
$workspace_fn_before_delete_action$ LANGUAGE plpgsql;

CREATE TRIGGER workspace_trigger_5_before_delete BEFORE DELETE
ON workspace
FOR EACH ROW
EXECUTE FUNCTION workspace_fn_before_delete_action();


/*******************************************************************************
 rooturl
================================================================================
 id | workspaceid | url                     | period
----|-------------|-------------------------|--------
 1  | 2           | https://www.google.com  | 1000
 2  | 2           | https://www.youtube.com | 2000
 3  | 3           | https://www.google.com  | 1000
 4  | 3           | https://www.youtube.com | 2000
------------------------------------------------------------------------------*/
CREATE TABLE rooturl (
    id              BIGSERIAL PRIMARY KEY,
    workspaceid     BIGINT NOT NULL REFERENCES workspace(id) ON DELETE CASCADE,
    url             TEXT NOT NULL CHECK(LENGTH(url) > 0),
    period          INTEGER NOT NULL DEFAULT 60 CHECK(period > 0) /* minutes */,
    UNIQUE(workspaceid, url)
);

-- =====================================
-- 0. noupdate
-- =====================================
CREATE TRIGGER rooturl_trigger_0_noupdate BEFORE UPDATE OF id, workspaceid
ON rooturl
FOR EACH ROW
EXECUTE FUNCTION fn_noupdate('rooturl.id or rooturl.workspaceid is unmodifiable');


/*******************************************************************************
 urlstats
================================================================================
 id | url                    | encounter | downloadcount
----|------------------------|-----------|---------------
 1  | https://example.com    | 1         | 1
 2  | https://www.google.com | 5         | 3
------------------------------------------------------------------------------*/ 
CREATE TABLE urlstats (
    id              BIGSERIAL PRIMARY KEY,
    url             TEXT NOT NULL CHECK(LENGTH(url) > 0),
    encounter       INTEGER NOT NULL DEFAULT 1 CHECK(encounter >= 0),
    downloadcount   INTEGER NOT NULL DEFAULT 0 CHECK(downloadcount >= 0),
    CHECK(encounter >= downloadcount)
);

CREATE UNIQUE INDEX urlstats_indexu_url ON urlstats(md5(url));

-- =====================================
-- 0. noupdate
-- =====================================
CREATE TRIGGER urlstats_trigger_0_noupdate BEFORE UPDATE OF id, url
ON urlstats
FOR EACH ROW
EXECUTE FUNCTION fn_noupdate('urlstats.id or urlstats.url is unmodifiable');

-- =====================================
-- 3. BEFORE UPDATE
-- =====================================
CREATE FUNCTION urlstats_fn_before_update_action() RETURNS TRIGGER AS $urlstats_fn_before_update_action$
BEGIN
    IF NEW.encounter != OLD.encounter + 1 AND NEW.encounter != OLD.encounter THEN
        RAISE EXCEPTION 'invalid value for encounter';
    END IF;

    IF NEW.downloadcount != OLD.downloadcount + 1 AND NEW.downloadcount != OLD.downloadcount THEN
        RAISE EXCEPTION 'invalid value for downloadcount';
    END IF;

    RETURN NEW;
END;
$urlstats_fn_before_update_action$ LANGUAGE plpgsql;

CREATE TRIGGER urlstats_trigger_3_before_update BEFORE UPDATE
ON urlstats
FOR EACH ROW
EXECUTE FUNCTION urlstats_fn_before_update_action();


/*******************************************************************************
 urljob
================================================================================
 id | workspaceid | url                    | urltype | inserttime | begintime | dstarttime | dstoptime | endtime | endtype 
----|-------------|------------------------|---------|------------|-----------|------------|-----------|---------|---------
 1  | 1           | https://example.com    | html    | now()      |           | now()      | now()     |         | success 
 2  | 2           | https://www.google.com | html    | xxx        |           | yyy        | zzz       |         | success 
------------------------------------------------------------------------------*/ 
CREATE TYPE endtype AS ENUM ('success', 'timeout');
CREATE TYPE urltype AS ENUM ('html', 'media');

CREATE TABLE urljob (
    id              BIGSERIAL PRIMARY KEY,
    parentid        BIGINT NOT NULL REFERENCES urljob(id) /* this could point to a deleted parent, and that's OK */,
    workspaceid     BIGINT NOT NULL REFERENCES workspace(id) ON DELETE CASCADE,
    url             TEXT NOT NULL CHECK(LENGTH(url) > 0),
    urltype         urltype NOT NULL,
    inserttime      TIMESTAMP WITH TIME ZONE NOT NULL /* all above fields (including this one) must be filled at INSERT time */,
    begintime       TIMESTAMP WITH TIME ZONE,
    dstarttime      TIMESTAMP WITH TIME ZONE,
    dstoptime       TIMESTAMP WITH TIME ZONE,
    endtime         TIMESTAMP WITH TIME ZONE,
    endtype         endtype,

    CHECK(id > parentid OR id = 1),

    -- /* time values must be chronologically set */
    -- CHECK(begintime IS NULL OR begintime >= inserttime),
    -- CHECK(dstarttime IS NULL OR dstarttime >= begintime),
    -- CHECK(dstoptime IS NULL OR dstoptime >= dstarttime),
    -- CHECK(endtime IS NULL OR endtime >= dstoptime),

    /* fields must be set in order */
    CHECK(NOT (inserttime IS NULL AND begintime IS NOT NULL)),
    CHECK(NOT (begintime IS NULL AND dstarttime IS NOT NULL)),
    CHECK(NOT (dstarttime IS NULL AND dstoptime IS NOT NULL)),
    CHECK(NOT (dstoptime IS NULL AND endtime IS NOT NULL)),

    /* endtime and endtype must be null or not null at the same time, which means they must be set at the same time */
    CHECK((endtime IS NULL AND endtype IS NULL) OR (endtype IS NOT NULL AND endtype IS NOT NULL))
);

-- we can only add unique constraint when parentid is not 1
-- because we can have multiple (parentid, workspaceid, url, urltype)s from that come from rooturl
--
-- note that we include urltype as url can be both html or media and we cannot know for sure which
-- one it is when we add the job, we can only know after download and check content-type
--
-- md5(url) is used instead of bare url since url can be long and PostgreSQL will complain
-- "Values larger than 1/3 of a buffer page cannot be indexed"
CREATE UNIQUE INDEX urljob_indexu_parentid_workspaceid_url ON urljob(parentid, workspaceid, md5(url), urltype)
WHERE parentid <> 1;

-- reserve id 1 as the root for all url jobs created from rooturl table
INSERT INTO urljob(
    parentid,
    workspaceid,
    url,
    urltype,
    inserttime,
    begintime,
    dstarttime,
    dstoptime,
    endtime,
    endtype)
VALUES (
    1,
    1,
    'https://example.com/',
    'html',
    'now()',
    'now()',
    'now()',
    'now()',
    'now()',
    'success');

-- =====================================
-- 0. noupdate
-- =====================================
CREATE TRIGGER urljob_trigger_0_noupdate_0
BEFORE UPDATE OF id, parentid, workspaceid, url, urltype, inserttime
ON urljob
FOR EACH ROW
EXECUTE FUNCTION fn_noupdate('urljob.id, urljob.parentid, urljob.workspaceid, urljob.url, urljob.urltype or urljob.inserttime is unmodifiable');

CREATE TRIGGER urljob_trigger_0_noupdate_1
BEFORE UPDATE OF begintime
ON urljob
FOR EACH ROW
WHEN (OLD.begintime IS NOT NULL)
EXECUTE FUNCTION fn_noupdate('urljob.begintime is unmodifiable');

CREATE TRIGGER urljob_trigger_0_noupdate_2
BEFORE UPDATE OF dstarttime
ON urljob
FOR EACH ROW
WHEN (OLD.dstarttime IS NOT NULL)
EXECUTE FUNCTION fn_noupdate('urljob.dstarttime is unmodifiable');

CREATE TRIGGER urljob_trigger_0_noupdate_3
BEFORE UPDATE OF dstoptime
ON urljob
FOR EACH ROW
WHEN (OLD.dstoptime IS NOT NULL)
EXECUTE FUNCTION fn_noupdate('urljob.dstoptime is unmodifiable');

CREATE TRIGGER urljob_trigger_0_noupdate_4
BEFORE UPDATE OF endtime
ON urljob
FOR EACH ROW
WHEN (OLD.endtime IS NOT NULL)
EXECUTE FUNCTION fn_noupdate('urljob.endtime is unmodifiable');

CREATE TRIGGER urljob_trigger_0_noupdate_5 BEFORE UPDATE OF endtime, endtype
ON urljob
FOR EACH ROW
WHEN (OLD.endtime IS NOT NULL OR OLD.endtype IS NOT NULL)
EXECUTE FUNCTION fn_noupdate('urljob.endtime or urljob.endtype is unmodifiable');

-- =====================================
-- 1. BEFORE INSERT
-- =====================================
CREATE FUNCTION urljob_fn_before_insert_action() RETURNS TRIGGER AS $urljob_fn_before_insert_action$
BEGIN
    IF NEW.parentid <> 1 THEN
        IF EXISTS (SELECT FROM urljob WHERE id = NEW.parentid AND workspaceid = NEW.workspaceid) THEN
            RETURN NEW;
        ELSE
            RAISE EXCEPTION 'relationship between id, parentid and workspaceid is invalid';
        END IF;
    END IF;
    RETURN NEW;
END;
$urljob_fn_before_insert_action$ LANGUAGE plpgsql;

CREATE TRIGGER urljob_trigger_1_before_insert BEFORE INSERT
ON urljob
FOR EACH ROW
EXECUTE FUNCTION urljob_fn_before_insert_action();

-- =====================================
-- 2. AFTER INSERT
-- =====================================
CREATE FUNCTION urljob_fn_after_insert_action() RETURNS TRIGGER AS $urljob_fn_after_insert_action$
BEGIN
    -- insert/update urlstats
    IF EXISTS (SELECT FROM urlstats WHERE url = NEW.url) THEN
        UPDATE urlstats SET encounter = encounter + 1 WHERE url = NEW.url;
    ELSE
        INSERT INTO urlstats(url) VALUES(NEW.url);
    END IF;
    RETURN NULL;
END;
$urljob_fn_after_insert_action$ LANGUAGE plpgsql;

CREATE TRIGGER urljob_trigger_2_after_insert AFTER INSERT
ON urljob
FOR EACH ROW
EXECUTE FUNCTION urljob_fn_after_insert_action();

-- =====================================
-- 3. BEFORE UPDATE
-- =====================================
CREATE FUNCTION urljob_fn_before_update_action() RETURNS TRIGGER AS $urljob_fn_before_update_action$
BEGIN
    -- dstoptime IS NULL AND endtime IS NOT NULL
    IF NEW.endtime IS NOT NULL THEN
        IF NEW.dstoptime IS NULL AND OLD.dstoptime IS NULL THEN
            NEW.dstoptime := NEW.endtime;
        END IF;
    END IF;

    -- dstarttime IS NULL AND dstoptime IS NOT NULL
    IF NEW.dstoptime IS NOT NULL THEN
        IF NEW.dstarttime IS NULL AND OLD.dstarttime IS NULL THEN
            NEW.dstarttime := NEW.dstoptime;
        END IF;
    END IF;

    -- begintime IS NULL AND dstarttime IS NOT NULL
    IF NEW.dstarttime IS NOT NULL THEN
        IF NEW.begintime IS NULL AND OLD.begintime IS NULL THEN
            NEW.begintime := NEW.dstarttime;
        END IF;
    END IF;

    RETURN NEW;
END;
$urljob_fn_before_update_action$ LANGUAGE plpgsql;

CREATE TRIGGER urljob_trigger_3_before_update BEFORE UPDATE
ON urljob
FOR EACH ROW
EXECUTE FUNCTION urljob_fn_before_update_action();

-- =====================================
-- 4. AFTER UPDATE
-- =====================================
CREATE FUNCTION urljob_fn_after_update_action() RETURNS TRIGGER AS $urljob_fn_after_update_action$
BEGIN
    -- update urlstats
    IF NEW.endtype = 'success' THEN
        UPDATE urlstats SET downloadcount = downloadcount + 1 WHERE URL = NEW.url;
    END IF;
    RETURN NULL;
END;
$urljob_fn_after_update_action$ LANGUAGE plpgsql;

CREATE TRIGGER urljob_trigger_4_after_update AFTER UPDATE OF endtype
ON urljob
FOR EACH ROW
EXECUTE FUNCTION urljob_fn_after_update_action();

-- =====================================
-- 5. BEFORE DELETE
-- =====================================
CREATE FUNCTION urljob_fn_before_delete_action() RETURNS TRIGGER AS $urljob_fn_before_delete_action$
BEGIN
    IF OLD.id = 1 THEN
        RAISE EXCEPTION 'cannot delete id 1';
    END IF;
    RETURN OLD;
END;
$urljob_fn_before_delete_action$ LANGUAGE plpgsql;

CREATE TRIGGER urljob_trigger_5_before_delete BEFORE DELETE
ON urljob
FOR EACH ROW
EXECUTE FUNCTION urljob_fn_before_delete_action();

-- =====================================
-- 6. AFTER DELETE
-- =====================================


/*******************************************************************************
 fileinfo
================================================================================
 id | filehash | hashtype | filesize
----|----------|----------|----------
 1  | xxx      | sha2-256 | 5
------------------------------------------------------------------------------*/
CREATE TYPE hashtype AS ENUM (
    'sha2-256',
    'sha2-512',
    'sha3-256',
    'sha3-512'
);

CREATE TABLE fileinfo (
    id              BIGSERIAL PRIMARY KEY,
    filehash        TEXT NOT NULL UNIQUE CHECK(LENGTH(filehash) > 0),
    hashtype        hashtype NOT NULL,
    filesize        BIGINT NOT NULL CHECK(filesize > 0)
);

-- =====================================
-- 0. noupdate
-- =====================================
CREATE TRIGGER fileinfo_trigger_0_noupdate BEFORE UPDATE
ON fileinfo
FOR EACH ROW
EXECUTE FUNCTION fn_noupdate('fileinfo data cannot be updated after set');


/*******************************************************************************
 mediafile
================================================================================
 id | urljobid | fileinfoid | mediawidth | mediaheight
----|----------|------------|------------|-------------
 1  | 1        | 1          | 1          | 1
------------------------------------------------------------------------------*/ 
CREATE TABLE mediafile (
    id              BIGSERIAL PRIMARY KEY,
    urljobid        BIGINT NOT NULL UNIQUE REFERENCES urljob(id) ON DELETE CASCADE,
    fileinfoid      BIGINT NOT NULL REFERENCES fileinfo(id) ON DELETE RESTRICT,
    filename        TEXT NOT NULL CHECK(LENGTH(filename) > 0),
    mediawidth      INTEGER NOT NULL CHECK(mediawidth > 0),
    mediaheight     INTEGER NOT NULL CHECK(mediaheight > 0),
    inserttime      TIMESTAMP WITH TIME ZONE NOT NULL
);

-- =====================================
-- 0. noupdate
-- =====================================
CREATE TRIGGER mediafile_trigger_0_noupdate BEFORE UPDATE
ON mediafile
FOR EACH ROW
EXECUTE FUNCTION fn_noupdate('mediafile data cannot be updated after set');

