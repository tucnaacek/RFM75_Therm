// $Id: buffer.c 17652 2014-02-19 12:36:10Z krejsa $


#include <string.h>
#include "global.h"
#include "buffer.h"
#include "stm32f0xx_conf.h"



int binit(t_buff *p_buff, char *content, int size)
{
	// TODO Better implementation of assert
	assert_param((size==   2)||(size==   4)||(size==   8)||(size==  16)||
				(size==  32)||(size==  64)||(size== 128)||(size== 256)||
				(size== 512)||(size==1024)||(size==2048)||(size==4096)||
				(size== 8192)||(size==16384)||(size==32768)||
				(size==65536));

	p_buff->content = content;
	p_buff->size = size;
	p_buff->size_mask = size - 1;
	p_buff->head = 0;
	p_buff->tail = 0;

	return 0;
}


int bgetc(t_buff *p_buff, char *p_val)
{
//    ISTORE;
	__asm__ volatile ("cpsid i" : : : "memory");//    DIS_INT;
	if (p_buff->head == p_buff->tail) {
        // Buffer is empty
        __asm__ volatile ("cpsie i" : : : "memory");// ENA_INT;
		return 0;
    }
    // Buffer is not empty return one element
    assert_param(p_buff->head < p_buff->size);
    *p_val = p_buff->content[p_buff->head];
    p_buff->head = (p_buff->head + 1) & p_buff->size_mask;
    __asm__ volatile ("cpsie i" : : : "memory");// ENA_INT;//    ENA_INT;
    return 1;
}
// TODO fdsfs

// Read uint16 value from char buffer. Use BigEndian byte order.
int bget_uint16be(t_buff *p_buff, uint16_t *val)
{
    char val1, val2;

    if (bgetc(p_buff, &val1)) {
        return -1;
    }
    if (bgetc(p_buff, &val2)) {
        return -2;
    }

    *val = ((int)val1 << 8) + (int)val2;
    return 0;
}


//TODO: tady se mi to nezda....ten ASSERT nebo je to OK????
int bputc(t_buff *p_buff, char val)
{
//    ISTORE;
    __asm__ volatile ("cpsid i" : : : "memory");// DIS_INT;
    int tail_new = (p_buff->tail + 1) & p_buff->size_mask;
    if (tail_new == p_buff->head)
    {
        // Buffer is full
    __asm__ volatile ("cpsie i" : : : "memory");// ENA_INT;
    	return 0;
    }

    // Insert new value into the buffer
    assert_param(p_buff->tail < p_buff->size);
    p_buff->content[p_buff->tail] = val;
    p_buff->tail = tail_new;
    __asm__ volatile ("cpsie i" : : : "memory");// ENA_INT;//    ENA_INT;
    return 1;
}


int bsize(t_buff *p_buff)
{
//	ISTORE;
	__asm__ volatile ("cpsid i" : : : "memory");//	DIS_INT;
	int size = p_buff->tail - p_buff->head;
	if (size < 0) {
		size += p_buff->size;
	}
	__asm__ volatile ("cpsie i" : : : "memory");//	ENA_INT;
	return size;
}


int bwrite(t_buff *pbuff, const char *text, int len)
{
    char *pc, *pe= (char*) text + len;
    for (pc=(char*)text; pc<pe; pc++) {
        if (!bputc(pbuff, *pc))
        {
            return 0;
        }
    }
    return len;
}



int bwrite_str(t_buff *pbuff, const char *text)
{
    return bwrite(pbuff, text, strlen(text));
}
