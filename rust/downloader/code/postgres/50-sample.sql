\c postgres

WITH up AS (
    INSERT INTO workspace(name, config)
    VALUES(
        'example',
        '{
            "htmlfilter": [
                {
                    "pattern": "sub.example.com",
                    "include": ["sub.example.com/post/show/", ".*"],
                    "exclude": [],
                    "maxurls": 50
                }
            ]
        }')
    RETURNING id
)
, data(url, period) AS (
    VALUES
        ('https://sub.example.com/', 60)
)
INSERT INTO rooturl(workspaceid, url, period)
SELECT up.id, data.url, data.period FROM up, data;
