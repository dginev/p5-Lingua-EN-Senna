#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"
#include "ppport.h"

#include "SENNA_API.h"

typedef void* Lingua__EN__SENNA;

MODULE = Lingua::EN::SENNA               PACKAGE = Lingua::EN::SENNA              

PROTOTYPES: ENABLE

Lingua::EN::SENNA
new(package)
  char* package
  CODE:
    RETVAL=SENNA_new();
  OUTPUT:
    RETVAL

void
DESTROY(SENNA_object)
  Lingua::EN::SENNA SENNA_object