#include <vector>

#include <gtest/gtest.h>


// g++ GTestExample.cpp -o GTestExample.exe -lgtest_main


/*
testing macros:

    ASSERT_*    yields fatal failure, immediately returns from current test case
    EXPECT_*    yields nonfatal failure, execution continues running

    *_TRUE  (condition);
    *_FALSE (condition);
    *_EQ    (expected, actual);
    *_NE    (val1, val2);
    *_LT    (val1, val2);
    *_LE    (val1, val2);
    *_GT    (val1, val2);
    *_GE    (val1, val2);

    *_THROW     (statement, exception_type);
    *_ANY_THROW (statement);
    *_NO_THROW  (statement);

    *_PRED1         (pred1, val1);
    *_PREDn         (predn, val1, val2, ...);
    *_PRED_FORMAT1  (pred_format1, val1);
    *_PRED_FORMATn  (pred_formatn, val1, val2, ...);

    *_FLOAT_EQ  (expected, actual);
    *_DOUBLE_EQ (expected, actual);
    *_NEAR      (val1, val2, abs_error);

    *_HRESULT_SUCCEEDED (expression);
    *_HRESULT_FAILED    (expression);


    SUCCEED();      use to document the test only, rarely used
    FAIL();         fatal failure
    ADD_FAILURE();  nonfatal failure

*/


/*
command line options:

    --gtest_filter                      run all tests that match the filter pattern
                                            --gtest_filter=*
                                            --gtest_filter=TestCaseA.*
                                            --gtest_filter=*A.*

    --gtest_list_tests                  list all test cases without running them
                                        override all other flags

    --gtest_also_run_disabled_tests     run disabled tests

    --gtest_repeat=N                    run all tests N times
                                        a negative number means forever

    --gtest_break_on_failure            stop immediately when encounter first failure

    --gtest_shuffle                     run tests in a random order
    --gtest_random_seed=SEED            give a seed to random number generator
                                            SEED = [0, 99999]

    --gtest_color=COLOR                 enable or disable color output
                                            COLOR = yes, no, auto (default)

    --gtest_print_time=0                disable printing time

    --gtest_output                      output detail report to xml
                                            --gtest_output="xml"
                                                will create file test_detail.xml
                                            --gtest_output="xml:_path_to_output_file_"

    --gtest_catch_exceptions=0          disable catching exceptions

*/


int Factorial(int n) {
    int fac = 1;
    for (; n > 0; --n) fac *= n;
    return fac;
}


TEST(FactorialTest, HandlesZeroInput) {
    EXPECT_EQ(1, Factorial(0));
}


TEST(FactorialTest, HandlesPositiveInput) {
    EXPECT_EQ(1,     Factorial(1));
    EXPECT_EQ(2,     Factorial(2));
    EXPECT_EQ(6,     Factorial(3));
    EXPECT_EQ(40320, Factorial(8));
}


TEST(FactorialTest, HandlesNegativeInput) {
    EXPECT_EQ(1, Factorial(-1));
    EXPECT_EQ(1, Factorial(-5));
    EXPECT_EQ(1, Factorial(-100));
}


TEST(FailMacroTest, FailMacroTest) {
    auto i = 4;
    switch (i) {
    case 0:
    case 1:
    case 2:
        SUCCEED();
        break;

    default:
        FAIL();
    }
}


// disable a test
TEST(DISABLED_StdVectorTest, InitializedSize) {
    std::vector<int> v;
    EXPECT_EQ(v.size(), 0U);
}


TEST(StdVectorTest, ShrinkToFitCapacity) {
    std::vector<int> v{1, 2, 3, 4, 5};
    v.clear();
    v.shrink_to_fit();
    EXPECT_EQ(0U, v.capacity());
}
