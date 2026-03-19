savedcmd_test_sysfs.mod := printf '%s\n'   test_sysfs.o | awk '!x[$$0]++ { print("./"$$0) }' > test_sysfs.mod
