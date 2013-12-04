#ifndef STRUCTURES_H
#define STRUCTURES_H


typedef struct Produit Produit; /* structure du produit */
struct Produit
{
	int produitid;
	char *code_barres;
	char *marque;
	char *libelle;
	char *type_marque;
	int	code_TVA;
	float prix;
};


typedef struct Element Element; /* structure de base le liste chainée */
struct Element
{
    Produit	*p_produit; /* pointeur vers la structure du produits */
    int		quantitee;
    Element	*suivant;
};
typedef struct Liste Liste; /* Structure de contrôle de la liste chainée*/
struct Liste
{
    Element *debut;
};


typedef struct TauxTVA TauxTVA; /* structure de la TVA */
struct TauxTVA
{
	int		code;
	float	taux;
};


typedef struct ListeUtilisateurs
{
  const gchar	*utilisateur;
  const gchar	*motdepasse;
  const gchar 	*niveau;
}LISTEUTILISATEURS;


#endif /* STRUCTURES_H */