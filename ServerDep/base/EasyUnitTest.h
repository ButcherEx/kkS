#ifndef EASY_UNIT_TEST_H
#define EASY_UNIT_TEST_H

BASE_NAME_SPACES

typedef void (*__UFunc)(void*);
class UnitTestCase
{
public:
	UnitTestCase(__UFunc func):func_(func), p_(0){}
public:
	void runTest(void* p = 0) { if(func_)func_(p); }
private:
	void* p_;
	__UFunc func_;
};

BASE_NAME_SPACEE

#define AUTOTEST_DEF(MARK)\
	extern base::UnitTestCase g##MARK;

#define AUTOTEST_IMP(MARK,FUNC)\
	base::UnitTestCase g##MARK(FUNC);

#define AUTOTEST_RUN(MARK,PARAM)\
	(g##MARK).runTest(PARAM);

#endif