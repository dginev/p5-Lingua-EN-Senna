#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"
#include "ppport.h"

#include "SENNA_API.h"

typedef void* Lingua__EN__SENNA;

MODULE = Lingua::EN::SENNA               PACKAGE = Lingua::EN::SENNA              

PROTOTYPES: ENABLE


Lingua::EN::SENNA
new()
  CODE:
    RETVAL=SENNA_new();
  OUTPUT:
    RETVAL