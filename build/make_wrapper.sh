#/bin/sh
#$1 file name $2 dir 
test_name="`echo $1 | sed -e 's/_wrapper.c//g'`"
echo "#include <$2${test_name}_test.h>" >> ${test_name}_wrapper.c
echo "MakeMain(${test_name});" >> ${test_name}_wrapper.c

