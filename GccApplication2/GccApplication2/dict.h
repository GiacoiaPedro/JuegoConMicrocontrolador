#ifndef DICT_H_
#define DICT_H_

#include <stdint.h>

#define WORD_LEN  5
#define WORD_BUF  (WORD_LEN + 1)

void     dict_init(void);
void     dict_get_random_word(char *wrd);
void	 dict_seed(uint32_t s);
uint8_t  dict_size(void);

#endif