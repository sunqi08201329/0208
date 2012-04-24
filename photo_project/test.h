#ifndef __TESTT__H__
#define  __TESTT__H__
typedef void (*func_t)(void);

void a(void);
void b(void);
void c(void);

func_t f[] = {a, b, c};
#endif // __TESTT__H__
