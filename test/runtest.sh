rm -f ut_* try.o

${CC:-gcc} -O2 -std=c99 -I.. -o try.o -c ../try.c

for f in t*.c; do
  echo "compiling '$f'"
  ${CC:-gcc} -O2 -std=c99 -I.. -o ut_${f%.c} $f
done

for f in lt*.c; do
  echo "compiling '$f'"
  ${CC:-gcc} -O2 -std=c99 -I.. -o ut_${f%.c} $f try.o
done

rm -f runtest.log

for f in ut_*; do
  echo "running '$f' 2> runtest.log"
  ./$f 2>> runtest.log
done 

echo "done"

fail=`grep -c "^FAIL: " runtest.log`
pass=`grep -c "^PASS: " runtest.log`

echo "FAIL: $fail"
echo "PASS: $pass"
test "$fail" == "0"
