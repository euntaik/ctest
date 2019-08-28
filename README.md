# ctest
Test framework for C

## What is ctest?
It is a easy to use test framework for C.
All you have to do is declare a function to be tested using the TEST() macro.
And thats it!

## Getting started:
"ctest" is a single file framwork (ctest.h")
- Download the source code.
- Include in your project file.
- Declare a function with TEST macro.
```
#include "ctest.h"

TEST(my_first_test)
{
  // do somthing
  ...
  
  // Add test conditions like below
  TEST_EQ(a, b);
  
}

void main(void)
{
  // This will call all the functions declared with the TEST() macro
  RUN_ALL_TESTS();
}
```


