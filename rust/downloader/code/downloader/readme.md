# README

- Add sendid
- Add parentid to urljob

## COMMANDS

```shell
docker-compose up

# postgresql://user[:password]@host[:port][/database][?param1=val1[[&param2=val2]...]]
RUST_BACKTRACE=full ./bin-gate \
--port 5555 \
--postgresql-addr 'postgresql://admin:admin@localhost:5432/postgres' \
--start-urljob 'true' \
--start-rooturl 'false' \
--kafka-addr '127.0.0.1:29092' \
--kafka-group 'main' \
--kafka-topic-download-url-html 'download-url-html' \
--kafka-topic-download-url-media 'download-url-media' \
--kafka-topic-download-notify-start 'download-notify-start' \
--kafka-topic-download-notify-stop 'download-notify-stop' \
--log-level 'debug'

RUST_BACKTRACE=full ./bin-url-grabber \
--gate-addr localhost:5555 \
--redis-addr 'redis://127.0.0.1/' \
--redis-prefix 'bin-url-grabber:' \
--redis-cache-minutes 10 \
--kafka-addr '127.0.0.1:29092' \
--kafka-group 'main' \
--kafka-topic-download-url-html 'download-url-html' \
--kafka-topic-download-notify-start 'download-notify-start' \
--kafka-topic-download-notify-stop 'download-notify-stop' \
--download-delay-min-secs 5 \
--download-delay-max-secs 10 \
--log-level 'debug'

RUST_BACKTRACE=full ./bin-media-downloader \
--gate-addr localhost:5555 \
--redis-addr 'redis://127.0.0.1/' \
--redis-prefix 'bin-media-downloader:' \
--redis-cache-minutes 120 \
--kafka-addr '127.0.0.1:29092' \
--kafka-group 'main' \
--kafka-topic-download-url-media 'download-url-media' \
--kafka-topic-download-notify-start 'download-notify-start' \
--kafka-topic-download-notify-stop 'download-notify-stop' \
--download-dir ~/work/tmp/images \
--download-delay-min-secs 1 \
--download-delay-max-secs 2 \
--limit-filesize 20000000 \
--log-level 'debug'
```

```sql
-- Make sure that every entry in fileinfo has a coresponding entry in mediafile.
-- In other words, mediafile >= fileinfo.
SELECT *
FROM fileinfo
WHERE id NOT IN (SELECT fileinfoid FROM mediafile)
```
