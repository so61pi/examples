## Add variables from env

```shell
BB_ENV_EXTRAWHITE="VAR1 VAR2"
VAR1=1
VAR2=2
```

## Generate tarballs

```shell
BB_GENERATE_MIRROR_TARBALLS = "1"
```

## Use your own source mirror

```shell
INHERIT += "own-mirrors"
SSTATE_MIRRORS = "file://.* https://example.com/yocto/sstate-cache/PATH;downloadfilename=PATH"
SOURCE_MIRROR_URL = "http://example.com/my-source-mirror"
```
