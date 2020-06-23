/**
 * Test framework for the C language.
 * Inspired by Googletest.
 * Copyright 2019 etinum@gmail.com
 */

#ifndef __CTEST_H__
#define __CTEST_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <time.h>

#define CTEST_STRINGIFY(x) #x

#define CTEST_COLOR_RED "\033[0;31m"
#define CTEST_COLOR_RED_BOLD "\033[1;31m"
#define CTEST_COLOR_GREEN "\033[0;32m"
#define CTEST_COLOR_GREEN_BOLD "\033[1;32m"
#define CTEST_COLOR_BLUE "\033[0;34m"
#define CTEST_COLOR_BLUE_BOLD "\033[1;34m"
#define CTEST_COLOR_YELLOW "\033[0;33m"
#define CTEST_COLOR_YELLOW_BOLD "\033[1;33m"
#define CTEST_COLOR_WHITE "\033[0;38m"
#define CTEST_COLOR_WHITE_BOLD "\033[1;38m"
#define CTEST_COLOR_GREY "\033[0;37m"
#define CTEST_COLOR_GREY_BOLD "\033[1;37m"
#define CTEST_COLOR_PURPLE "\033[0;35m"
#define CTEST_COLOR_PURPLE_BOLD "\033[1;35m"
#define CTEST_COLOR_PINK "\033[38;5;206m"

#define CTEST_COLOR_RESET "\033[0m"

#define CTEST_ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))
#define printarray(array) \
    __print_array(CTEST_STRINGIFY(array), array, CTEST_ARRAY_SIZE(array))

inline int __print_array(char* name, unsigned long *array, size_t count)
{
    for (size_t i = 0; i < count; i++)
        printf("%s [%zu] = %lu\n", name, i, array[i]);
}

#define _ABS(a) (((a) < 0) ? -(a): (a))
#define _PRINT_OK(a, b, _cmp) \
		printf("%s[        OK ]%s (%s%s%s%s%s%s%s%s%s) => ", \
			CTEST_COLOR_GREEN, CTEST_COLOR_RESET, CTEST_COLOR_WHITE_BOLD, \
			CTEST_STRINGIFY(a), CTEST_COLOR_RESET, CTEST_COLOR_YELLOW_BOLD, \
			_cmp, CTEST_COLOR_RESET, CTEST_COLOR_WHITE_BOLD, \
			CTEST_STRINGIFY(b), CTEST_COLOR_RESET)

#define _PRINT_FAIL(a, b, _cmp) \
		printf("%s[    FAILED ]%s (%s%s%s%s%s%s%s%s%s) => ", \
			CTEST_COLOR_RED, CTEST_COLOR_RESET, CTEST_COLOR_WHITE_BOLD, \
			CTEST_STRINGIFY(a), CTEST_COLOR_RESET, CTEST_COLOR_YELLOW_BOLD, \
			_cmp, CTEST_COLOR_RESET, CTEST_COLOR_WHITE_BOLD, \
			CTEST_STRINGIFY(b), CTEST_COLOR_RESET)

#define EXPECT_EQ(a, b) \
	if (a == b) { \
		_PRINT_OK(a, b, " == "); \
        printf("(%s%llu == %llu%s)\n", CTEST_COLOR_BLUE_BOLD, \
			(unsigned long long)(a), (unsigned long long)(b), CTEST_COLOR_RESET); \
	} else { \
		_PRINT_FAIL(a, b, " == "); \
		printf("(%s%llu != %llu%s)\n", CTEST_COLOR_BLUE_BOLD, \
			(unsigned long long)(a), (unsigned long long)(b), CTEST_COLOR_RESET); \
		printf("\n\033[1;31mTEST TERMINATED! \033[0;32m\n" ); \
		exit(1); \
	}

#define EXPECT_FLOAT_EQ(a, b) \
	if ((double)a == (double)b) { \
		_PRINT_OK(a, b, " == "); \
        printf("(%s%lf == %lf%s)\n", CTEST_COLOR_BLUE_BOLD, \
			(double)(a), (double)(b), CTEST_COLOR_RESET); \
	} else { \
		_PRINT_FAIL(a, b, " == "); \
		printf("(%s%lf != %lf%s)\n", CTEST_COLOR_BLUE_BOLD, \
			(double)(a), (double)(b), CTEST_COLOR_RESET); \
		printf("\n\033[1;31mTEST TERMINATED! \033[0;32m\n" ); \
		exit(1); \
	}

#define EXPECT_NEAR(a, b, delta) \
	if (a == b) { \
		_PRINT_OK(a, b, " ~= "); \
        printf("(%s%lf == %lf%s)\n", CTEST_COLOR_BLUE_BOLD, \
			(double)(a), (double)(b), CTEST_COLOR_RESET); \
    } else if (_ABS(a - b) <= delta) { \
        _PRINT_OK(a, b, " ~= "); \
        printf("(%s%lf ~= %lf%s)\n", CTEST_COLOR_BLUE_BOLD, \
			(double)(a), (double)(b), CTEST_COLOR_RESET); \
	} else { \
        _PRINT_FAIL(a, b, " !~= "); \
		printf("(%s%lf !~= %lf%s)\n", CTEST_COLOR_BLUE_BOLD, \
			(double)(a), (double)(b), CTEST_COLOR_RESET); \
		printf("\n\033[1;31mTEST TERMINATED! \033[0;32m\n" ); \
		exit(1); \
	}

#define EXPECT_TRUE(expr) EXPECT_EQ(expr, true)
#define EXPECT_FALSE(expr) EXPECT_EQ(expr, false)

#define EXPECT_STRING_EQ(a, b, len) \
	if (strncmp(a, b, len) == 0) { \
		_PRINT_OK(a, b, " == "); \
        printf("(%s%s == %s%s)\n", CTEST_COLOR_BLUE_BOLD, \
			(a), (b), CTEST_COLOR_RESET); \
	} else { \
		_PRINT_FAIL(a, b, " == "); \
		printf("(%s%s != %s%s)\n", CTEST_COLOR_BLUE_BOLD, \
			(a), (b), CTEST_COLOR_RESET); \
		printf("\n\033[1;31mTEST TERMINATED! \033[0;32m\n" ); \
		exit(1); \
	}
struct ctests_section_info {
	void (*func)(void);
	char* name;
};

#define TEST(__name__) \
	void TEST_##__name__(void); \
	struct ctests_section_info __attribute__((section("___CTESTS___"))) info_TEST_##__name__ = { \
		TEST_##__name__, \
		CTEST_STRINGIFY(__name__), \
	}; \
	void  TEST_##__name__(void)


#define RUN_TEST(__name__) ___run_test((void (*)(void))(TEST_##__name__), CTEST_STRINGIFY(__name__))

#define RUN_ALL_TESTS() ___run_all_tests()

static inline void ___run_test(void (*test)(void), char* name)
{
    clock_t start = clock();
    clock_t end;
    double ms;
    printf("%s[ RUN       ]%s %s%s%s\n", CTEST_COLOR_GREEN, CTEST_COLOR_RESET, 
		CTEST_COLOR_BLUE_BOLD, name, CTEST_COLOR_RESET);
    test();
    end = clock();
    ms = ((double)(end - start)*1000)/CLOCKS_PER_SEC;
    printf("%s[ PASSED    ]%s %s%s%s took %0.3lf ms\n", CTEST_COLOR_GREEN, 
		CTEST_COLOR_RESET, CTEST_COLOR_BLUE_BOLD, name, CTEST_COLOR_RESET, ms);
    printf("\n");
}

static inline void ___run_all_tests(void)
{
	extern unsigned long __start____CTESTS___;
	extern unsigned long __stop____CTESTS___;
	void* start_addr = &__start____CTESTS___;
	void* end_addr = &__stop____CTESTS___;
	struct ctests_section_info *info = (struct ctests_section_info *)start_addr;
	for (; (unsigned long)info < (unsigned long)end_addr; info++) {
		___run_test(info->func, info->name);
	}
}

#ifdef __cplusplus
}
#endif
#endif /* __CTEST_H__ */
