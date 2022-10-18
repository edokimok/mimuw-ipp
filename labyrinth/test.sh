#!/bin/bash

cnt_all=0;
cnt_correct=0;

for temp_test in $2/*.in
do
  ./$1 < $temp_test 1> temp_answer.out 2> temp_answer.err
  err=$?

  correct=0

  echo -n -e "$((cnt_all+1)). "

  # Check answer:
  if diff "${temp_test%in}out" temp_answer.out >/dev/null 2>&1
  then
    ((correct++));
  else
    err="WRONG ANSWER"
  fi

  # Check error:
  if diff "${temp_test%in}err" temp_answer.err >/dev/null 2>&1
  then
    ((correct++));
  else
    err=$(<temp_answer.err)
  fi

  # Print result:
  echo -n -e "\e[3m$temp_test\e[0m: "

    if ((correct == 2))
    then
      echo -e "\e[32mPassed\e[0m"
      ((cnt_correct++));
    else
      echo -e "\e[31mFailed with error: $err\e[0m"
    fi
  ((cnt_all++))

  rm temp_answer.out
  rm temp_answer.err
done

echo -e "\e[35m$cnt_correct/$cnt_all tests passed\e[0m"