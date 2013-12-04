#include <my_global.h>
#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>

#include <glib.h>

#include "cashut_main_callbacks.h"
#include "main_mysql.h"

#include "structures.h"


extern GtkBuilder 	*builder_connexion;
extern GtkBuilder 	*builder_cashut;
extern const gchar 	*niveau_utilisateur;

extern TauxTVA *taux_tva;
extern int nombre_taux_tva;

void 
on_window_cashut_destroy (GtkWidget *widget, gpointer user_data)
{
    //gtk_main_quit ();
	gtk_widget_hide(GTK_WIDGET (gtk_builder_get_object (builder_cashut, "window_cashut")));
	gtk_widget_show(GTK_WIDGET (gtk_builder_get_object (builder_connexion, "window_connexion")));
	gtk_entry_set_text (GTK_ENTRY(gtk_builder_get_object (builder_connexion, "username")), "");
	gtk_entry_set_text (GTK_ENTRY(gtk_builder_get_object (builder_connexion, "password")), "");
	gtk_widget_grab_focus(GTK_WIDGET(gtk_builder_get_object (builder_connexion, "username")));

}

void 
on_window_cashut_show (GtkWidget *widget, gpointer user_data)
{

	if( g_strcmp0(niveau_utilisateur, "1") == 0 ){ // Si le niveau == 1
			// le page_num (2nd parametre) vaut "4" trois fois car on enlève la 4ème tab (en comptant à partir de 0) en sachant que GTK reindexe le notebook à chaque suppression
			gtk_notebook_remove_page(GTK_NOTEBOOK (gtk_builder_get_object (builder_cashut, "notebook")), 4); // Stats
			gtk_notebook_remove_page(GTK_NOTEBOOK (gtk_builder_get_object (builder_cashut, "notebook")), 4); // Gestion prod
			gtk_notebook_remove_page(GTK_NOTEBOOK (gtk_builder_get_object (builder_cashut, "notebook")), 4); // Gestion utilisateurs
	}

}


void 
on_notebook_change_page (GtkWidget *widget, gpointer user_data)
{
	gint current_page;
	current_page = gtk_notebook_get_current_page(GTK_NOTEBOOK(gtk_builder_get_object(builder_cashut, "notebook")));

	switch( current_page ){
		case 0: // ACCUEIL
			g_print("Tab0\n");
		break;

		case 1: // ENCAISSEMENT
			g_print("Tab1\n");
		break;

		case 2: // CALCULATRICE
			g_print("Tab2\n");
		break;

		case 3: // CATALOGUE
			g_print("Tab3\n");
		break;

		case 4: // STATISTIQUES
			g_print("Tab4\n");
		break;

		case 5: // GESTION PRODUITS
			g_print("Tab5\n");
		break;

		case 6: // GESTION UTILISATEURS
			on_notebook_change_page6();
		break;

		case 7: // A PROPOS
			g_print("Tab7\n");
		break;

		default:
			g_print("Error\n");
		break;
	}

}

void on_notebook_change_page6(void){
	liste_utilisateurs_create_liststore();
}




float tva_code2taux(int code){
	
	int i;
	for( i=0 ; i<=(nombre_taux_tva-1) ; i++ ){
		if( code == taux_tva[i].code ){
			return (float)taux_tva[i].taux;
		}
	}

	return 0.0;
}



/***********************************************/
extern int nombre_chargement_liststore_utilisateurs;
LISTEUTILISATEURS *data_utilisateurs;


void liste_utilisateurs_create_liststore(void){

	//struct ListeUtilisateurs;

	/*LISTEUTILISATEURS data_utilisateurs[] =
	{
	  { "Utilisateur 1", "Mot de passe 1", 1 },
	  { "Utilisateur 2", "Mot de passe 2", 2 },
	  { "Utilisateur 3", "Mot de passe 3", 1 },
	  { "Utilisateur 4", "Mot de passe 4", 2 }
	};*/

	int nombre_utilisateurs;
	nombre_utilisateurs = mysql_nombre_utilisateurs();


	data_utilisateurs = (LISTEUTILISATEURS *) malloc( sizeof(LISTEUTILISATEURS) * nombre_utilisateurs);

	data_utilisateurs = mysql_recuperer_utilisateurs();


	GtkTreeView *treeview_utilisateurs;
	treeview_utilisateurs = GTK_TREE_VIEW(gtk_builder_get_object(builder_cashut, "treeview_utilisateurs"));

	// Supprime les colonnes si elles ont déjà été chargées
	if( nombre_chargement_liststore_utilisateurs >= 1 ){
		//g_print("On supprime les colonnes\n");
		gtk_tree_view_remove_column (treeview_utilisateurs, gtk_tree_view_get_column(treeview_utilisateurs, 0));
		gtk_tree_view_remove_column (treeview_utilisateurs, gtk_tree_view_get_column(treeview_utilisateurs, 0));
		gtk_tree_view_remove_column (treeview_utilisateurs, gtk_tree_view_get_column(treeview_utilisateurs, 0));
	}

	nombre_chargement_liststore_utilisateurs++;

	//gtk_tree_view_append_column (treeview_utilisateurs, (GtkTreeViewColumn *)0);


	gint i = 0;
	GtkListStore *liste_utilisateurs_model;
	GtkTreeIter iter;

	/* create list store */
	liste_utilisateurs_model = gtk_list_store_new (3,
												  G_TYPE_STRING,
										          G_TYPE_STRING,
										          G_TYPE_STRING);
	/* add data to the list store */
	// G_N_ELEMENTS(data_utilisateurs)
	for (i = 0; i <= (nombre_utilisateurs-1); i++)
    {
		gtk_list_store_append (liste_utilisateurs_model, &iter);
		gtk_list_store_set (liste_utilisateurs_model, &iter,
                          0, data_utilisateurs[i].utilisateur,
                          1, data_utilisateurs[i].motdepasse,
                          2, data_utilisateurs[i].niveau,
                          -1);
    }


	gtk_tree_view_set_model(treeview_utilisateurs, GTK_TREE_MODEL(liste_utilisateurs_model));


	if( data_utilisateurs != NULL ){
		free(data_utilisateurs);
	}

	g_object_unref (liste_utilisateurs_model);

	GtkCellRenderer *renderer;
  	GtkTreeViewColumn *column;

	// column 1
	renderer = gtk_cell_renderer_text_new ();
	column = gtk_tree_view_column_new_with_attributes ("Nom d'utilisateur",
		                                             renderer,
		                                             "text", 0,
		                                             NULL);
	gtk_tree_view_column_set_sort_column_id (column, 0);
	gtk_tree_view_append_column (treeview_utilisateurs, column);

	// column 2
	renderer = gtk_cell_renderer_text_new ();
	column = gtk_tree_view_column_new_with_attributes ("Mot de passe",
		                                             renderer,
		                                             "text", 1,
		                                             NULL);
	gtk_tree_view_column_set_sort_column_id (column, 1);
	gtk_tree_view_append_column (treeview_utilisateurs, column);

	// column 3
	renderer = gtk_cell_renderer_text_new ();
	column = gtk_tree_view_column_new_with_attributes ("Niveau",
		                                             renderer,
		                                             "text", 2,
		                                             NULL);
	gtk_tree_view_column_set_sort_column_id (column, 2);
	gtk_tree_view_append_column (treeview_utilisateurs, column);




}

