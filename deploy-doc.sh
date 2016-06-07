#!/bin/bash
echo "Publishing to GITHUB pages"

# Update the documents
git checkout master
git pull
cd report
make html
cd ..

# Publih
git checkout gh-pages
cp -r report/build/html/* .
git add -u
git commit -m "Updated the html doc" -s
git push origin gh-pages
git checkout master

