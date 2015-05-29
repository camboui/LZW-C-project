
typedef short int dict_index_t;  	// le type index
typedef long dict_char_t;          // le type valeur/caractere
typedef struct _dict *dict_t;

// Return codes
typedef enum {DICT_INFOOK=0,    // uniquement pour info et node_content

	      DICT_NODICT=1,    // dictionnaire invalide
	      DICT_INVALID=2,   // index ou prefixe invalide
	      DICT_NOTFOUND=3,  // noeud cherche absent
              DICT_FOUND=4, 	// noeud (deja) present
	      DICT_ADDED=5, 	// noeud ajoute
              DICT_FULL=6,      // erreur en insertion : plus de place

              DICT_FUNCERROR=7, // uniquement pour apply
              DICT_MALLOCERROR=8  // pour getstring
             } dict_error_t;

// constantes dict_index_t 
#define DICT_ROOT_INDEX	-1		// index de la racine
#define DICT_NONODE -2			// Index de noeud illegal

// constantes dict_char_t
#define DICT_CHAR_EOS	-1		// End of string (valeur de racine)
#define DICT_CHAR_NOCHAR -2		// Valeur indefinie


// Allouer un dictionnaire pour max_elements chaines non vides
// Initialise les elements de prefix racine 
// avec index = valeur pour l'intervalle [0,nb_autoindex-1]
// et accelere leur recherche
// Fonction desactivee pour autoindex=0
// Retourne NULL en cas d'erreur

dict_t dict_new(dict_index_t max_elements, 
                dict_index_t nb_autoindex);

// Destruction ...
void dict_delete(dict_t d);

// Rechercher le noeud <prefix,value> dans le dictionnaire d
// Resultat index : index du noeud s'il est present

dict_error_t dict_search (dict_t d, dict_index_t prefix,
                           dict_char_t value, dict_index_t *index);

// Inserer le noeud <prefix,value> dans le dictionnaire d
// Resultat index : index du noeud deja present ou nouvellement insere

dict_error_t dict_insert (dict_t d, dict_index_t prefix,
                           dict_char_t value, dict_index_t *index);

// Lire les informations associees a un noeud :
// + index du prefixe
// + valeur du suffixe
dict_error_t dict_index_content (dict_t d, dict_index_t index,
                             dict_char_t *value, dict_index_t *prefix_index);

// Lire les informations associees au dictionnaire
// Last allocated : index du dernier noeud valide
// Max index      : dernier numero d'index utilisable
dict_error_t dict_info (dict_t d, dict_index_t *last_allocated, 
                        dict_index_t *max_index);

// Destruction et liberation de la mémoire ...
void dict_delete (dict_t d);

// Le type fonction d'un dict_char_t, retournant false en cas d'erreur
typedef bool dict_apply_func_t (dict_char_t carac);

// Apply f on the index character/value first
// and then recursively apply f on index prefix
// --> Apply f on each character of the string, in reverse order.
// Stops on the first call to f returning false
dict_error_t dict_apply_reverse (dict_t dict, dict_index_t index,
                                 dict_apply_func_t f);

// Obtention de caractere initial + longueur de la chaine d'un noeud
dict_error_t dict_firstchar_length (dict_t dict, dict_index_t index, 
                                    dict_char_t *carac, unsigned int *length);

// Retourne dans s un tableau de taille *s_size alloué dynamiquement
// (l'appelante devra appeler free pour le libérer)
dict_error_t dict_get_mallocedstring (dict_t dict, dict_index_t index,
                                      unsigned int *s_size, dict_char_t *s[]);

// Contrôle du mode debug
void dict_set_debug_level (unsigned int value);
