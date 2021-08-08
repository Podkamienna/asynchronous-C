// http: // www.jera.com/techinfo/jtns/jtn002.html

#ifndef MINUNIT_H
#define MINUNIT_H

#define mu_assert(message, test)                                               \
  do {                                                                         \
    if (!(test))                                                               \
      return message;                                                          \
  } while (0)

#define mu_run_test(test)                                                      \
  do {                                                                         \
    for (int i = 0; i < 100; i++) {                                            \
      char *message = test();                                                  \
      tests_run++;                                                             \
      if (message)                                                             \
        return message;                                                        \
    }                                                                          \
  } while (0)

extern int tests_run;

#endif
