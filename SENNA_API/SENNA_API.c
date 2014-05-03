// Standard C libraries
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Senna POS tagger
#include "SENNA_utils.h"
#include "SENNA_Hash.h"
#include "SENNA_Tokenizer.h"
#include "SENNA_POS.h"
#include "SENNA_API.h"

void* SENNA_new() {
  /* Initialize SENNA toolkit components: */
  char *opt_path = "senna/";
  SENNA_fields* SENNA_object = malloc(sizeof(SENNA_fields));
  /* SENNA inputs */
  SENNA_object->word_hash = SENNA_Hash_new(opt_path, "hash/words.lst");
  SENNA_object->caps_hash = SENNA_Hash_new(opt_path, "hash/caps.lst");
  SENNA_object->suff_hash = SENNA_Hash_new(opt_path, "hash/suffix.lst");
  SENNA_object->gazt_hash = SENNA_Hash_new(opt_path, "hash/gazetteer.lst");

  SENNA_object->gazl_hash = SENNA_Hash_new_with_admissible_keys(opt_path, "hash/ner.loc.lst", "data/ner.loc.dat");
  SENNA_object->gazm_hash = SENNA_Hash_new_with_admissible_keys(opt_path, "hash/ner.msc.lst", "data/ner.msc.dat");
  SENNA_object->gazo_hash = SENNA_Hash_new_with_admissible_keys(opt_path, "hash/ner.org.lst", "data/ner.org.dat");
  SENNA_object->gazp_hash = SENNA_Hash_new_with_admissible_keys(opt_path, "hash/ner.per.lst", "data/ner.per.dat");

  /* SENNA labels */
  SENNA_object->pos_hash = SENNA_Hash_new(opt_path, "hash/pos.lst");
  SENNA_object->chk_hash = SENNA_Hash_new(opt_path, "hash/chk.lst");
  SENNA_object->pt0_hash = SENNA_Hash_new(opt_path, "hash/pt0.lst");
  SENNA_object->ner_hash = SENNA_Hash_new(opt_path, "hash/ner.lst");
  SENNA_object->vbs_hash = SENNA_Hash_new(opt_path, "hash/vbs.lst");
  SENNA_object->srl_hash = SENNA_Hash_new(opt_path, "hash/srl.lst");
  SENNA_object->psg_left_hash = SENNA_Hash_new(opt_path, "hash/psg-left.lst");
  SENNA_object->psg_right_hash = SENNA_Hash_new(opt_path, "hash/psg-right.lst");

  SENNA_object->tokenizer = SENNA_Tokenizer_new(
    SENNA_object->word_hash,
    SENNA_object->caps_hash,
    SENNA_object->suff_hash,
    SENNA_object->gazt_hash,
    SENNA_object->gazl_hash,
    SENNA_object->gazm_hash,
    SENNA_object->gazo_hash,
    SENNA_object->gazp_hash,
    1); // TODO: The 1 is a parameter, change back to a parametric "is it tokenized?"
  
  SENNA_object->pos = SENNA_POS_new(opt_path, "data/pos.dat");

  return (void*)SENNA_object;
}

void DESTROY(SENNA_fields* SENNA_object) {
  free((void*)SENNA_object);
}


void* SENNA_Tokenize_sentence(SENNA_fields* SENNA_object, char* sentence) {
     SENNA_Tokens* tokens = SENNA_Tokenizer_tokenize(SENNA_object->tokenizer, sentence);
     return tokens;
}

void* SENNA_POS_sentence(SENNA_fields* SENNA_object, char* sentence) {
  SENNA_Tokens* tokens = (SENNA_Tokens*) SENNA_Tokenize_sentence(SENNA_object,sentence);
  /* Obtain the corresponding list of POS tags for the list of words */
  int *pos_labels = NULL;
  pos_labels = SENNA_POS_forward(SENNA_object->pos, tokens->word_idx, tokens->caps_idx, tokens->suff_idx, tokens->n);
  int token_index;
  for(token_index = 0; token_index < tokens->n; token_index++) {}
  //     json_object_object_add(response,ids[token_index],
  //                           json_object_new_string(SENNA_Hash_key(pos_hash, pos_labels[token_index])));
  // }  
  return tokens; // Tentative stub
}

void* SENNA_fullproc_sentence(SENNA_fields* SENNA_object, char* sentence) {
  SENNA_Tokens* tokens = (SENNA_Tokens*) SENNA_Tokenize_sentence(SENNA_object,sentence);
  return tokens;
}