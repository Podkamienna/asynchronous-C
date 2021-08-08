//
// Created by alicja on 01.01.2020.
//

#ifndef ASYNC_ERR_H
#define ASYNC_ERR_H

/* print system call error message and terminate */
_Noreturn extern void syserr(int bl, const char *fmt, ...);

/* print error message and terminate */
_Noreturn extern void fatal(const char *fmt, ...);

#endif //ASYNC_ERR_H
