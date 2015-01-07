#include <PreConfig.h>
#if defined(USE_JEMALLOC)
#include <jemalloc/jemalloc.h>

void* operator new(size_t size, bool tag)
{
	(void)tag;
	return je_malloc(size);
}
void operator delete(void* p, bool tag)
{
	(void)tag;
	if(p)je_free(p);
}
// void* operator new[](size_t size, bool tag)
// {
// 	(void)tag;
// 	return je_malloc(size);
// }
// 
// void operator delete[](void* p, bool tag)
// {
// 	(void)tag;
// 	if(p)je_free(p);
// }


#endif