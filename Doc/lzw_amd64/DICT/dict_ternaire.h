#include "dict.h"

struct _node {
  dict_index_t father;		 // prefix string
  dict_index_t previous_brother; // string same prefix, before
  dict_index_t next_brother;     // string same prefix after
  dict_index_t suffix;           // suffix strings
  dict_char_t  last;             // string last char
};

typedef struct _node node_t;

struct _dict {
  dict_index_t last_alloc_index;	// malloc nodes
  dict_index_t first_free_index;	// first free malloced node
  dict_index_t last_autoindex; 		// root suns with index=value
  node_t *array;                        
};


dict_error_t dict_index_internal_content (dict_t d,
                                          dict_index_t index,
                                          dict_char_t *value,
                                          dict_index_t *prefix_index,
                                          dict_index_t *suffix,
                                          dict_index_t *previous,
                                          dict_index_t *next);

