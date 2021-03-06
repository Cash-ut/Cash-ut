#include <my_global.h>
#include <mysql.h>
#include <gtk/gtk.h>

#include "structures.h"

#include "cashut_main.h"
#include "cashut_main_callbacks.h"

#include "cashut_gu_callbacks.h"
#include "cashut_pe_callbacks.h"
#include "cashut_cp_callbacks.h"
#include "cashut_ca_callbacks.h"
#include "cashut_gp_callbacks.h"

#include "main_mysql.h"

#include "main_gestion_liste.h"

#include "window_cashut.inc"


#include "resources/icon.xpm"
#include "resources/logo_645.xpm"



GtkBuilder 			*builder_cashut;
extern const gchar 	*niveau_utilisateur;
extern const gchar 	*nom_utilisateur;
extern const gchar 	*num_utilisateur;


/******** EXTERN CONFIG ********/
extern const gchar 	*num_caisse;
/*******************************/

Liste *liste_course;
paiement paiement_encour;

TauxTVA *tb_taux_tva;
int nombre_taux_tva;

GtkTreeView *treeview_liste_chaine;  

int nombre_chargement_liststore_utilisateurs;


void
cashut_main_window()
{

    GtkWidget  *window_cashut;

    //gtk_init (&argc, &argv);
	builder_cashut = gtk_builder_new ();

	gtk_builder_add_from_string(builder_cashut, window_cashut_glade, -1, NULL);
    window_cashut = GTK_WIDGET (gtk_builder_get_object (builder_cashut, "window_cashut"));

    //gtk_builder_connect_signals (builder_cashut, NULL);

	// Icône
	GdkPixbuf *icon;
	icon = gdk_pixbuf_new_from_xpm_data((const char **)icon_png);
	gtk_window_set_icon(GTK_WINDOW(window_cashut), icon);

	GdkPixbuf *logo_big;
	logo_big = gdk_pixbuf_new_from_xpm_data((const char **)logo_645);
	
	gtk_image_set_from_pixbuf(GTK_IMAGE(gtk_builder_get_object(builder_cashut, "image_logo_big_accueil")), logo_big);
	gtk_image_set_from_pixbuf(GTK_IMAGE(gtk_builder_get_object(builder_cashut, "image_logo_big_apropos")), logo_big);

	
	// Signaux Fenêtre Cash'UT
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


	// Signaux Gestion des Utilisateurs
	g_signal_connect (
            gtk_builder_get_object (builder_cashut, "gu_button_modifier"),
            "clicked", G_CALLBACK(gu_button_modifier_clicked), NULL
	);

	g_signal_connect (
            gtk_builder_get_object (builder_cashut, "gu_button_supprimer"),
            "clicked", G_CALLBACK(gu_button_supprimer_clicked), NULL
	);

	g_signal_connect (
            gtk_builder_get_object (builder_cashut, "gu_button_ajouter"),
            "clicked", G_CALLBACK(gu_button_ajouter_clicked), NULL
	);

	g_signal_connect (
            gtk_builder_get_object (builder_cashut, "treeview_utilisateurs"),
            "row-activated", G_CALLBACK(gu_treeview_rowactivated), NULL
	);


	// Signaux Partie Encaissement
	g_signal_connect ( 
			GTK_WIDGET (gtk_builder_get_object (builder_cashut, "pe_btn_ajouter")),
			"clicked", G_CALLBACK (pe_ajouter_produit), NULL
	);

	g_signal_connect ( 
			GTK_WIDGET (gtk_builder_get_object (builder_cashut, "pe_btn_annuler")),
			"clicked", G_CALLBACK (pe_annuler), NULL
	);
	g_signal_connect ( 
			GTK_WIDGET (gtk_builder_get_object (builder_cashut, "pe_btn_supprimer")),
			"clicked", G_CALLBACK (pe_supprimer_produit), NULL
	);
	g_signal_connect ( 
			GTK_WIDGET (gtk_builder_get_object (builder_cashut, "pe_btn_quant_plus")),
			"clicked", G_CALLBACK (pe_qte_produit_ajout), NULL
	);
	g_signal_connect ( 
			GTK_WIDGET (gtk_builder_get_object (builder_cashut, "pe_btn_quant_moins")),
			"clicked", G_CALLBACK (pe_qte_produit_retire), NULL
	);
	g_signal_connect ( 
			GTK_WIDGET (gtk_builder_get_object (builder_cashut, "pe_btn_cheque")),
			"clicked", G_CALLBACK (pe_btn_cheque), NULL
	);
	g_signal_connect ( 
			GTK_WIDGET (gtk_builder_get_object (builder_cashut, "btn_paiement_annuler")),
			"clicked", G_CALLBACK (pe_annuler_paiement), NULL
	);
	g_signal_connect ( 
			GTK_WIDGET (gtk_builder_get_object (builder_cashut, "pe_btn_espece")),
			"clicked", G_CALLBACK (pe_btn_espece), NULL
	);
	g_signal_connect ( 
			GTK_WIDGET (gtk_builder_get_object (builder_cashut, "pe_btn_carte")),
			"clicked", G_CALLBACK (pe_btn_carte), NULL
	);
	g_signal_connect ( 
			GTK_WIDGET (gtk_builder_get_object (builder_cashut, "pe_btn_terminer_nofac")),
			"clicked", G_CALLBACK (pe_facture_no_fact), NULL
	);
	g_signal_connect ( 
			GTK_WIDGET (gtk_builder_get_object (builder_cashut, "pe_btn_terminer_fac")),
			"clicked", G_CALLBACK (pe_facture_fact), NULL
	);
    g_signal_connect (
            gtk_builder_get_object (builder_cashut, "pe_entry_codebarres"),
            "changed", G_CALLBACK(pe_verif_caractere), NULL
    );

    // Signaux Catalogue Produit
    g_signal_connect (
            gtk_builder_get_object (builder_cashut, "cp_b_rechercher"),
            "clicked", G_CALLBACK(cp_b_rechercher_clicked), NULL
    );

    g_signal_connect (
            gtk_builder_get_object (builder_cashut, "cp_treeview"),
            "row-activated", G_CALLBACK(cp_treeview_rowactivated), NULL
    );

    g_signal_connect (
            gtk_builder_get_object (builder_cashut, "cp_b_ajouter"),
            "clicked", G_CALLBACK(cp_b_ajouter_clicked), NULL
    );


	// Signaux Gestion Produits
	g_signal_connect (
		gtk_builder_get_object (builder_cashut, "gp_button_valider"),
		"clicked", G_CALLBACK (gp_button_valider_click ), NULL
	);
	g_signal_connect (
		gtk_builder_get_object (builder_cashut, "gp_button_annuler"),
		"clicked", G_CALLBACK (gp_button_annuler_click ), NULL
	);
	g_signal_connect (
		gtk_builder_get_object (builder_cashut, "gp_button_modif"),
		"clicked", G_CALLBACK (gp_button_modif_click ), NULL
	);
	g_signal_connect (
		gtk_builder_get_object (builder_cashut, "gp_button_ajout"),
		"clicked", G_CALLBACK (gp_button_ajout_click ), NULL
	);
	g_signal_connect (
		gtk_builder_get_object (builder_cashut, "gp_button_sup"),
		"clicked", G_CALLBACK (gp_button_sup_click ), NULL
	);
	g_signal_connect (
		gtk_builder_get_object (builder_cashut, "gp_code_ean13"),
		"changed", G_CALLBACK (gp_verif_entiers), NULL
	);
	g_signal_connect (
		gtk_builder_get_object (builder_cashut, "gp_entry_tva"),
		"changed", G_CALLBACK (gp_verif_entiers), NULL
	);
	g_signal_connect (
		gtk_builder_get_object (builder_cashut, "gp_entry_prix"),
		"changed", G_CALLBACK (gp_verif_floats), NULL
	);

	// Signaux Calculatrice
	GtkWidget *ca_label_screen;
	ca_label_screen = GTK_WIDGET(gtk_builder_get_object (builder_cashut, "ca_screen"));

	g_signal_connect(gtk_builder_get_object (builder_cashut, "ca_7"), "clicked", G_CALLBACK(ca_append_7), ca_label_screen);
	g_signal_connect(gtk_builder_get_object (builder_cashut, "ca_8"), "clicked", G_CALLBACK(ca_append_8), ca_label_screen);
	g_signal_connect(gtk_builder_get_object (builder_cashut, "ca_9"), "clicked", G_CALLBACK(ca_append_9), ca_label_screen);
	g_signal_connect(gtk_builder_get_object (builder_cashut, "ca_slash"), "clicked", G_CALLBACK(ca_divide), ca_label_screen);
	g_signal_connect(gtk_builder_get_object (builder_cashut, "ca_4"), "clicked", G_CALLBACK(ca_append_4), ca_label_screen);
	g_signal_connect(gtk_builder_get_object (builder_cashut, "ca_5"), "clicked", G_CALLBACK(ca_append_5), ca_label_screen);
	g_signal_connect(gtk_builder_get_object (builder_cashut, "ca_6"), "clicked", G_CALLBACK(ca_append_6), ca_label_screen);
	g_signal_connect(gtk_builder_get_object (builder_cashut, "ca_asterisk"), "clicked", G_CALLBACK(ca_multiply), ca_label_screen);
	g_signal_connect(gtk_builder_get_object (builder_cashut, "ca_1"), "clicked", G_CALLBACK(ca_append_1), ca_label_screen);
	g_signal_connect(gtk_builder_get_object (builder_cashut, "ca_2"), "clicked", G_CALLBACK(ca_append_2), ca_label_screen);
	g_signal_connect(gtk_builder_get_object (builder_cashut, "ca_3"), "clicked", G_CALLBACK(ca_append_3), ca_label_screen);
	g_signal_connect(gtk_builder_get_object (builder_cashut, "ca_minus"), "clicked", G_CALLBACK(ca_subtract), ca_label_screen);
	g_signal_connect(gtk_builder_get_object (builder_cashut, "ca_0"), "clicked", G_CALLBACK(ca_append_0), ca_label_screen);
	g_signal_connect(gtk_builder_get_object (builder_cashut, "ca_decimal"), "clicked", G_CALLBACK(ca_append_decimal), ca_label_screen);
	g_signal_connect(gtk_builder_get_object (builder_cashut, "ca_percent"), "clicked", G_CALLBACK(ca_percent), ca_label_screen);
	g_signal_connect(gtk_builder_get_object (builder_cashut, "ca_plus"), "clicked", G_CALLBACK(ca_add), ca_label_screen);
	g_signal_connect(gtk_builder_get_object (builder_cashut, "ca_clear"), "clicked", G_CALLBACK(ca_clear), ca_label_screen);
	g_signal_connect(gtk_builder_get_object (builder_cashut, "ca_equals"), "clicked", G_CALLBACK(ca_equals), ca_label_screen);
	g_signal_connect(gtk_builder_get_object (builder_cashut, "window_cashut"), "key-release-event", G_CALLBACK(cashut_key_event), NULL);


	// Initialisation pour la Gestion des Utilisateurs
	nombre_chargement_liststore_utilisateurs = 0;


	// Initialisation du tableau des correspondances Codes-Taux TVA
	nombre_taux_tva = mysql_nombre_taux_tva(); 
	tb_taux_tva = malloc( sizeof(TauxTVA) * nombre_taux_tva);
	tb_taux_tva = mysql_recuperer_taux_tva();


	// Initialisation pour la Partie Encaissement
	treeview_liste_chaine = gtk_builder_get_object (builder_cashut, "pe_treeview_liste_chaine");
	if (gtk_tree_view_get_model (treeview_liste_chaine) == NULL) // normalement exécuté une seule fois au démarrage
	{
		initialisation_liste_chaine();
		init_treeview_lists_chaine();
	}
	

	mysql_nombre_ventes_aujourdhui("0");


	paiement_encour.espece= 0;
	paiement_encour.cheque= 0;
	paiement_encour.carte= 0;
	paiement_encour.reste= 0;
	
	/*Produit *le_produit;
	le_produit = mysql_recuperer_produit("3147690051107");


	g_print(le_produit->code_barres);
	g_print(le_produit->marque);
	g_print(le_produit->libelle);
	g_print(le_produit->type_marque);*/

//	init_treeview_lists(list);
/*
	if( Ajouter_produit("3264420101830") == 1 ){
		g_print("Produit ajouté\n");
	} else {
		g_print("Probleme ajout\n");
	}

	if( Ajouter_produit("5410076769421") == 1 ){
		g_print("Produit ajouté\n");
	} else {
		g_print("Probleme ajout\n");
	}

	if( Ajouter_produit("4015400535737") == 1 ){
		g_print("Produit ajouté\n");
	} else {
		g_print("Probleme ajout\n");
	}
	//Supprimer_produit(2,1);
	//Supprimer_liste();
	afficherListe();

*/


	gtk_widget_show (window_cashut);                
	gtk_main();
	g_object_unref (G_OBJECT (builder_cashut));
	g_object_unref (icon);
	g_object_unref (logo_big);

	//return 0;

}

