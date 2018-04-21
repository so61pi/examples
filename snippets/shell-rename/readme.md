```shell
# Rename all files to its sha1sum
for file in "./*"; do
    hash=$(sha1sum "${file}" | cut -d' ' -f1)
    ext=${file##*.}
    mv "$file" "${hash}.${ext}"
done
```

```shell
# Copy file to ../hash with its sha1sum
dest=../hash
[ -d $dest ] || mkdir $dest

for directory in *; do
    for file in "$directory"/*; do
        hash=$(sha1sum "${file}" | cut -d' ' -f1)
        ext=${file##*.}
        cp "$file" "${dest}/${hash}.${ext}"
    done
done
```
