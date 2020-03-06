#!/bin/bash
#
./fs_make.x test.fs 10
#dd if=/dev/zero of=test-file-1 bs=10 count=1
dd if=/dev/urandom of=test-file-1 bs=1024 count=1
#./test_fs.x add test.fs test-file-1./fs_ref.x ls test.fs
./fs_ref.x add test.fs test-file-1
./fs_ref.x ls test.fs
./test_fs.x ls test.fs
./fs_ref.x cat test.fs test-file-1
echo EOF
#./fs_ref.x cat test.fs test-file-1
#echo EOF
./test_fs.x cat test.fs test-file-1
echo EOF
rm -f test.fs test-file-1
