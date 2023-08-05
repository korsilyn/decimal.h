STD_FLAGS = -Wall -Wextra -Werror -std=c11
COMPILER = gcc
COV_FLAGS = --coverage
TEST_FLAGS = -lcheck -lpthread -lm
SOURCES_0 = *.o
SOURCES_1 = comparison/*.c helping_tools/*.c math_operations/*.c other_functions/*.c processing_tools/*.c
SOURCES_2 = s21_decimal_test.c
SOURCES_3 = s21_decimal.a

ifeq ($(shell uname), Linux)
	TEST_FLAGS += -lrt -lsubunit
endif

all: s21_decimal.a test 

s21_test_decimal.o: $(SOURCES_2)
	$(COMPILER) $(STD_FLAGS) -c  $(SOURCES_2)

test_case:
	$(COMPILER) $(STD_FLAGS) -c tests/*.c

s21_decimal.a:
	$(COMPILER) -c $(SOURCES_1)
	ar rcs $(SOURCES_3) $(SOURCES_0)

test: s21_decimal_with_coverage s21_test_decimal.o  test_case
	$(COMPILER) -o test_result $(STD_FLAGS) *.o -L. $(SOURCES_3) $(COV_FLAGS) $(TEST_FLAGS)
	./test_result

s21_decimal_with_coverage:
	$(COMPILER) $(STD_FLAGS) -c $(SOURCES_1) 
	ar rcs $(SOURCES_3) $(SOURCES_0)

gcov_report:
	gcovr -r . --html --html-details -o coverage_report.html
	open coverage_report.html

leak:
	leaks -atExit -- ./test_result

clean:
	@rm -rf *.html *.gcno *.gcda *.o *.out *.css *coverage* *.a test_result

clang:
	cp ../materials/linters/.clang-format .
	clang-format -n $(SOURCES_1) test_cases/*.c *.h *.c
	rm -rf .clang-format
.PHONY: all check gcov_report
