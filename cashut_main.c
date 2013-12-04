
#include <my_global.h>
#include <mysql.h>
#include <gtk/gtk.h>
/*#include "main_callbacks.h"*/

#include "structures.h"

#include "cashut_main.h"
#include "cashut_main_callbacks.h"
#include "main_mysql.h"
#include "main_gestion_liste.h"

#include "window_cashut.inc"


#include "resources/icon.xpm"
#include "resources/logo_645.xpm"



GtkBuilder 			*builder_cashut;
extern const gchar 	*niveau_utilisateur;

/******** EXTERN CONFIG ********/
extern const gchar 	*caisse_num;
/*******************************/

Liste *liste_course;

TauxTVA *taux_tva;
int nombre_taux_tva;

int nombre_chargement_liststore_utilisateurs;


int
cashut_main_window(int argc, char *argv[])
{
	
	nombre_chargement_liststore_utilisateurs = 0;
	
    /*GtkBuilder      *builder;*/
    GtkWidget  *window_cashut;


    gtk_init (&argc, &argv);
	builder_cashut = gtk_builder_new ();
	//builder_cashut1 = gtk_builder_new ();

    //gtk_builder_add_from_file (builder_cashut, "glade/window_cashut.glade", NULL);
	gtk_builder_add_from_string(builder_cashut, window_cashut_glade, -1, NULL);
	//gtk_builder_add_from_file (builder_cashut1, "glade/tab1.glade", NULL);
    window_cashut = GTK_WIDGET (gtk_builder_get_object (builder_cashut, "window_cashut"));

    gtk_builder_connect_signals (builder_cashut, NULL);


	GdkPixbuf *icon;
	icon = gdk_pixbuf_new_from_xpm_data((const char **)icon_png);
	gtk_window_set_icon(GTK_WINDOW(window_cashut), icon);

	GdkPixbuf *logo_big;
	logo_big = gdk_pixbuf_new_from_xpm_data((const char **)logo_645);
	


	gtk_image_set_from_pixbuf(GTK_IMAGE(gtk_builder_get_object(builder_cashut, "image_logo_big_accueil")), logo_big);
	gtk_image_set_from_pixbuf(GTK_IMAGE(gtk_builder_get_object(builder_cashut, "image_logo_big_apropos")), logo_big);


 	g_signal_connect (
            gtk_builder_get_object (builder_cashut, "window_cashut"),
            "delete-event", G_CALLBACK (on_window_cashut_destroy), NULL
	);


	g_signal_connect (
            gtk_builder_get_object (builder_cashut, "window_cashut"),
            "show", G_CALLBACK(on_window_cashut_show), NULL
	);


	g_signal_connect_after (
            GTK_NOTEBOOK(gtk_builder_get_object (builder_cashut, "notebook")),
            "switch-page", G_CALLBACK (on_notebook_change_page), NULL
	);



	nombre_taux_tva = mysql_nombre_taux_tva();
	taux_tva = malloc( sizeof(TauxTVA) * nombre_taux_tva);
	taux_tva = mysql_recuperer_taux_tva();


	

	/*Produit *le_produit;
	le_produit = mysql_recuperer_produit("3147690051107");


	g_print(le_produit->code_barres);
	g_print(le_produit->marque);
	g_print(le_produit->libelle);
	g_print(le_produit->type_marque);*/



/*
	initialisation_liste();

	if( Ajouter_produit("3264420101830") == 1 ){
		g_print("Produit ajouté");
	} else {
		g_print("Probleme ajout");
	}

	if( Ajouter_produit("5410076769421") == 1 ){
		g_print("Produit ajouté");
	} else {
		g_print("Probleme ajout");
	}

	if( Ajouter_produit("4015400535737") == 1 ){
		g_print("Produit ajouté");
	} else {
		g_print("Probleme ajout");
	}
	//Supprimer_produit(2,1);
	//Supprimer_liste();
	//afficherListe();*/



	gtk_widget_show (window_cashut);                
	gtk_main();
	g_object_unref (G_OBJECT (builder_cashut));
	g_object_unref (icon);
	g_object_unref (logo_big);

	return 0;

}
