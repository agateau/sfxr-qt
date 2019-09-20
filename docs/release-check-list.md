Check working tree is up to date and clean:

    git checkout master
    git status

Update CHANGELOG.md:

    r!git log --pretty=format:'- \%s (\%an)' x.y.z-1..HEAD

Bump version number in CMakeLists.txt

Build packages:

    tools/release

Update README and docs

Commit changes

Smoke test binary packages:

- Test generators
- Load a file
- Save a file
- Export a file

Create "x.y.z" tag:

    git tag -a x.y.z

Push:

    git push
    git push --tags

Publish packages on GitHub

Write blog post

Spread the word
