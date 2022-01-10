Check working tree is up to date and clean:

    git checkout master
    git status

Bump version number in CMakeLists.txt

Update CHANGELOG.md:

    r!git log --pretty=format:'- \%s (\%an)' x.y.z-1..HEAD

Update README and docs

Build packages:

    rm -rf build
    ci/build-app

Smoke test binary packages:

- Test generators
- Load a file
- Save a file
- Export a file

Commit changes

Create "x.y.z" tag:

    git tag -a x.y.z

Push:

    git push
    git push --tags

Publish packages on GitHub

Write blog post

Spread the word
