```shell
# Clone upstream repository
git clone --mirror <upstream-repository>
cd <upstream-repository>
git remote add <remote-name> <mirror-repository>

# Push to mirror
git push --mirror <remote-name>

# Update the repository
git remote update origin
```
