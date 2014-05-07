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

AV*
tokenize(SENNA_object, sentences)
  Lingua::EN::SENNA SENNA_object
  AV* sentences
  CODE:
    int i, len, sz = av_len(sentences);
    AV* tokenization_results = newAV();
    for (i=0; i<=sz; i++) {
      SV** sentence = av_fetch(sentences, i, 0);
      if (sentence != NULL) {
        AV* sentence_av;
        SENNA_Tokens* sentence_tokens = SENNA_Tokenize_sentence((SENNA_fields*)SENNA_object,SvPV(*sentence,len));
        sentence_av = newAV();
        int token_index;
        for (token_index=0; token_index < sentence_tokens->n; token_index++) {
          char* current_word = sentence_tokens->words[token_index];
          SV* word_sv = newSVpv(current_word,strlen(current_word));
          av_push(sentence_av,word_sv);
        }
        av_push(tokenization_results, newRV_inc((SV*)sentence_av));
      }
    }
    RETVAL = tokenization_results;
  OUTPUT:
    RETVAL
