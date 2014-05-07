// Senna POS tagger
#include "SENNA_utils.h"
#include "SENNA_Hash.h"
#include "SENNA_Tokenizer.h"
#include "SENNA_POS.h"

typedef struct {
  /* SENNA inputs */
  SENNA_Hash *word_hash;
  SENNA_Hash *caps_hash;
  SENNA_Hash *suff_hash;
  SENNA_Hash *gazt_hash;
  SENNA_Hash *gazl_hash;
  SENNA_Hash *gazm_hash;
  SENNA_Hash *gazo_hash;
  SENNA_Hash *gazp_hash;
  /* SENNA labels */
  SENNA_Hash *pos_hash;
  SENNA_Hash *chk_hash;
  SENNA_Hash *pt0_hash;
  SENNA_Hash *ner_hash;
  SENNA_Hash *vbs_hash;
  SENNA_Hash *srl_hash;
  SENNA_Hash *psg_left_hash;
  SENNA_Hash *psg_right_hash;
  /* Action objects */
  SENNA_Tokenizer *tokenizer;
  SENNA_POS *pos;
} SENNA_fields;

void* SENNA_new();
void DESTROY(SENNA_fields* SENNA_object);
SENNA_Tokens* SENNA_Tokenize_sentence(SENNA_fields* SENNA_object, char* sentence);