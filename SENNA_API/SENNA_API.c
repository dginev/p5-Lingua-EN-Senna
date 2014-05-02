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

// json_object* dom_to_pos_annotations (xmlDocPtr doc) {
//   char* log_message;
//   if (doc == NULL) {
//     fprintf(stderr, "Failed to parse workload!\n");
//     log_message = "Fatal:LibXML:parse Failed to parse document.";
//     return cortex_response_json("",log_message,-4); }
//   xmlXPathContextPtr xpath_context = xmlXPathNewContext(doc);
//   if(xpath_context == NULL) {
//     fprintf(stderr,"Error: unable to create new XPath context\n");
//     xmlFreeDoc(doc);
//     log_message = "Fatal:LibXML:XPath unable to create new XPath context\n";
//     return cortex_response_json("",log_message,-4); }

//   /* Register XHTML, MathML namespaces */
//   xmlXPathRegisterNs(xpath_context,  BAD_CAST "xhtml", BAD_CAST "http://www.w3.org/1999/xhtml");
//   xmlXPathRegisterNs(xpath_context,  BAD_CAST "m", BAD_CAST "http://www.w3.org/1998/Math/MathML");
//   xmlNsPtr xhtml_ns = xmlNewNs(xmlDocGetRootElement(doc),BAD_CAST "http://www.w3.org/1999/xhtml",BAD_CAST "xhtml");
//   if (xhtml_ns == NULL) {
//     perror("Failed to create XHTML namespace");  }

//   xmlChar *sentence_xpath = (xmlChar*) "//xhtml:span[@class='ltx_sentence']";
//   /* Generically normalize all "math" elements to "MathFormula" words */
//   xmlChar* math_xpath = (xmlChar*) "//m:math";
//   xmlXPathObjectPtr xpath_math_result = xmlXPathEvalExpression(math_xpath,xpath_context);
//   if (xpath_math_result != NULL) { // Nothing to do if there's no math in the document
//     xmlNodeSetPtr math_nodeset = xpath_math_result->nodesetval;
//     int math_index;
//     for (math_index=0; math_index < math_nodeset->nodeNr; math_index++) {
//       xmlNodePtr math_node = math_nodeset->nodeTab[math_index];
//       xmlNodePtr new_math_word = xmlNewNode(xhtml_ns, BAD_CAST "span");
//       xmlNewProp(new_math_word, BAD_CAST "class", BAD_CAST "ltx_word");
//       xmlNodePtr math_stub_text = xmlNewText(BAD_CAST "MathFormula");
//       xmlSetProp(new_math_word,BAD_CAST "id", BAD_CAST xmlGetProp(math_node,BAD_CAST "id"));
//       xmlAddChild(new_math_word,math_stub_text);
//       xmlReplaceNode(math_node,new_math_word);
//     }
//   }
//   /* Normalize all "a" anchor elements to their text content */
//   xmlChar* anchor_xpath = (xmlChar*) "//xhtml:a";
//   xmlXPathObjectPtr xpath_anchor_result = xmlXPathEvalExpression(anchor_xpath,xpath_context);
//   if (xpath_anchor_result != NULL) { // Nothing to do if there's no math in the document
//     xmlNodeSetPtr anchor_nodeset = xpath_anchor_result->nodesetval;
//     int anchor_index;
//     for (anchor_index=0; anchor_index < anchor_nodeset->nodeNr; anchor_index++) {
//       xmlNodePtr anchor_node = anchor_nodeset->nodeTab[anchor_index];
//       xmlNodePtr new_anchor_word = xmlNewNode(xhtml_ns, BAD_CAST "span");
//       xmlNewProp(new_anchor_word, BAD_CAST "class", BAD_CAST "ltx_word");
//       xmlNodePtr anchor_text = xmlNewText(BAD_CAST xmlNodeGetContent(anchor_node));
//       xmlSetProp(new_anchor_word,BAD_CAST "id", BAD_CAST xmlGetProp(anchor_node,BAD_CAST "id"));
//       xmlAddChild(new_anchor_word,anchor_text);
//       xmlReplaceNode(anchor_node,new_anchor_word);
//     }
//   }



//   /* Response JSON object */
//   json_object* response = json_object_new_object();
//   /* Find sentences: */
//   xmlXPathObjectPtr xpath_sentence_result = xmlXPathEvalExpression(sentence_xpath, xpath_context);
//   if(xpath_sentence_result == NULL) {
//     fprintf(stderr,"Error: unable to evaluate sentence xpath expression \"%s\"\n", sentence_xpath);
//     xmlXPathFreeContext(xpath_context);
//     xmlFreeDoc(doc);
//     log_message = "Fatal:LibXML:XPath unable to evaluate xpath expression\n";
//     return cortex_response_json("",log_message,-4); }

//   /* Sentence nodes: */
//   xmlNodeSetPtr sentences_nodeset = xpath_sentence_result->nodesetval;
//   /* Traverse each sentence and tag words */
//   xmlChar *words_xpath = (xmlChar*) "//xhtml:span[@class='ltx_word']";
//   int sentence_index;
//   for (sentence_index=0; sentence_index < sentences_nodeset->nodeNr; sentence_index++) {
//     xmlNodePtr sentence = sentences_nodeset->nodeTab[sentence_index];
//     xmlXPathContextPtr xpath_sentence_context = xmlXPathNewContext((xmlDocPtr)sentence);
//     xmlXPathRegisterNs(xpath_sentence_context,  BAD_CAST "xhtml", BAD_CAST "http://www.w3.org/1999/xhtml");
//     /* We want a list of words and a corresponding list of IDs */
//     xmlXPathObjectPtr words_xpath_result = xmlXPathEvalExpression(words_xpath, xpath_sentence_context);
//     if(words_xpath_result == NULL) { // No words, skip
//       xmlXPathFreeContext(xpath_sentence_context);
//       continue; }
//     xmlNodeSetPtr words_nodeset = words_xpath_result->nodesetval;
//     char* word_input_string;
//     size_t sentence_size;
//     FILE *word_stream;
//     word_stream = open_memstream (&word_input_string, &sentence_size);
//     int word_count = words_nodeset->nodeNr;
//     const char* ids[word_count];
//     int words_index;
//     for (words_index=0; words_index < word_count; words_index++) {
//       xmlNodePtr word_node = words_nodeset->nodeTab[words_index];
//       char* word_content = (char*) xmlNodeGetContent(word_node);
//       fprintf(word_stream, "%s", word_content);
//       fprintf(word_stream," ");
//       ids[words_index] = strdup((char*)xmlGetProp(word_node,BAD_CAST "id"));
//     }
//     //xmlElemDump(word_stream,doc,sentence);
//     fclose(word_stream);
//     /* Get the list of tokens from the input string */
//     SENNA_Tokens* tokens = SENNA_Tokenizer_tokenize(tokenizer, word_input_string);
//     if(tokens->n == 0)
//       continue;
//     /* Obtain the corresponding list of POS tags for the list of words */
//     pos_labels = SENNA_POS_forward(pos, tokens->word_idx, tokens->caps_idx, tokens->suff_idx, tokens->n);
//     int token_index;
//     for(token_index = 0; token_index < tokens->n; token_index++) {

//         json_object_object_add(response,ids[token_index],
//                                json_object_new_string(SENNA_Hash_key(pos_hash, pos_labels[token_index])));
//     }
//   }

//   // Freedom
//   xmlXPathFreeContext(xpath_context);
//   /* We return a JSON object that has "ID => POS tag" as result structure.*/
//   return response;
// }

// char* pos_labels_to_rdfxml (json_object* labels) {
//   for(entry = json_object_get_object(labels)->head; (entry ? (key = (char*)entry->k, val = (struct json_object*)entry->v, entry) : 0); entry = entry->next) {

//   }
// }
