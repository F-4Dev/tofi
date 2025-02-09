#ifndef MODULES_H
#define MODULES_H

#include "tofi.h"

void modules_suggest(struct tofi *tofi, char *restrict query,
                     struct string_ref_vec *filt);
bool modules_try_execute(struct tofi *tofi, char *suggestion, char *restrict query);

#endif /* MKDIRP_H */
