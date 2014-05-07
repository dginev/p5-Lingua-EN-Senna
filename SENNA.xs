#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"
#include "ppport.h"

#include "SENNA_API.h"

#define UNUSED(x) (void)(x)
typedef void* Lingua__EN__SENNA;

MODULE = Lingua::EN::SENNA               PACKAGE = Lingua::EN::SENNA              

PROTOTYPES: ENABLE

Lingua::EN::SENNA
new(package)
  char* package
  CODE:
    UNUSED(package);
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
    int i, sz = av_len(sentences);
    STRLEN len;
    AV* tokenization_results = newAV();
    for (i=0; i<=sz; i++) {
      SV** sentence = av_fetch(sentences, i, 0);
      if (sentence != NULL) {
        AV* sentence_av;
        SENNA_Tokens* sentence_tokens = SENNA_Tokenize_sentence((SENNA_fields*)SENNA_object, SvPV(*sentence, len));
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

AV* pos_tag(Lingua_SENNA_object, sentences)
  Lingua::EN::SENNA Lingua_SENNA_object
  AV* sentences
  CODE:
    SENNA_fields* SENNA_object = (SENNA_fields*) Lingua_SENNA_object;
    int i, sz = av_len(sentences);
    STRLEN len;
    AV* pos_results = newAV();
    for (i=0; i<=sz; i++) {
      SV** sentence = av_fetch(sentences, i, 0);
      if (sentence != NULL) {
        AV* sentence_av;
        SENNA_Tokens* sentence_tokens = SENNA_Tokenize_sentence(SENNA_object,SvPV(*sentence,len));
        int *pos_labels = NULL;
        pos_labels = SENNA_POS_forward(SENNA_object->pos, sentence_tokens->word_idx, sentence_tokens->caps_idx, sentence_tokens->suff_idx, sentence_tokens->n);
        sentence_av = newAV();
        int token_index;
        for (token_index=0; token_index < sentence_tokens->n; token_index++) {
          char* current_word = sentence_tokens->words[token_index];
          HV* word_hv = newHV();
          SV* word_sv = newSVpv(current_word,strlen(current_word));
          const char* pos_label = SENNA_Hash_key(SENNA_object->pos_hash, pos_labels[token_index]);
          SV* pos_sv = newSVpv(pos_label,strlen(pos_label));
          hv_store(word_hv,"word",4,word_sv,0);
          hv_store(word_hv,"POS",3,pos_sv,0);
          av_push(sentence_av,newRV_inc((SV*)word_hv));
        }
        av_push(pos_results, newRV_inc((SV*)sentence_av));
      }
    }
    RETVAL = pos_results;
  OUTPUT:
    RETVAL