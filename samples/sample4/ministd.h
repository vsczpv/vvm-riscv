#ifndef RV32I_SAMPLES_SAMPLE2_MINISTD_H_
#define RV32I_SAMPLES_SAMPLE2_MINISTD_H_

#define noreturn _Noreturn

typedef unsigned int size_t;

noreturn void exit  (void);
void          write (void* buf, size_t count);
size_t        read  (void* dest, size_t max);

size_t strlen(char* str);

void putstring(char* str);
void putnumber(int n);
long int power(int base, short exp);

#endif // RV32I_SAMPLES_SAMPLE2_MINISTD_H_
