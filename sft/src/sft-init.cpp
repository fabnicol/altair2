#include "sft2.h"
#include <R_ext/Rdynload.h>
extern "C" {
void R_init_sft(DllInfo *info) {
  R_RegisterCCallable("sft", "sft_",  (DL_FUNC) &sft_);
}
}
