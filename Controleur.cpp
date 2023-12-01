//
// Created by Utilisateur on 26/04/2023.
//

#include "Controleur.h"
#include "CarteTroupeElite.h"
#include "CarteRuse.h"
#include "CarteModeCombat.h"
#include <ctime>
#include <random>
#ifdef __unix__
# include <unistd.h>
#elif defined _WIN32
# include <windows.h>
#define sleep(x) Sleep(1000 * (x))
#endif
#include <sstream>


int generateRandomNumber() {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> distribution(1, 100);
    return distribution(rng);
}


Controleur::Controleur(bool tactique): m_tactique(tactique), m_gagnant(0), m_jeu_clan(JeuClan::getJeuClan()), m_jeu_tactique(JeuTactique::getJeuTactique()), m_carte_non_pose(m_jeu_clan.getNbCartes())
{
    m_pioche_clan = new Pioche(m_jeu_clan);
    if(tactique){
        m_pioche_tactique = new Pioche(m_jeu_tactique);
        printf("tactique\n");
        m_plateau = new Plateau(7, false, false);
    }
    else{
        m_plateau = new Plateau(6, false, false);
        printf("classique\n");
    }

    size_t i=0;
    JeuClan::Iterator it=m_jeu_clan.getIterator();
    while (!it.isDone()){
        m_carte_non_pose[i]= new CarteClan(static_cast<const CarteClan&>(it.currentItem()));
        it.next();
        i++;
    }
}

Controleur::~Controleur()
{
    delete m_pioche_clan;
    delete m_pioche_tactique;
    delete m_plateau;
    for(int i=0; i<m_carte_non_pose.size(); i++){
        delete m_carte_non_pose[i];
    }
}

Controleur& Controleur::getControleur(bool tactique)
{
    if (handler.controleur == nullptr)
        handler.controleur = new Controleur(tactique);
    return *handler.controleur;
}

void Controleur::libererControleur()
{
    delete handler.controleur;
    handler.controleur =nullptr;
}

Controleur::Handler Controleur::handler = Handler();


void clean(void)
{
    int c= 0;
    while ( (getchar()) != '\n' && c != EOF);
}

void Controleur::Afficher_Borne1(){
    for(auto i =0; i<9;i++){
        cout << "|B" << i<<":";
        affichage_vecteur_carteclan(m_plateau->getBornes(i)->getCartesJ1()->getCartes());
        cout <<"|"<< endl;
    }
    cout << endl;

}
void Controleur::Afficher_Borne2(){
    for(auto i =0; i<9;i++){
        cout << "|B" << i <<":";
        affichage_vecteur_carteclan(m_plateau->getBornes(i)->getCartesJ2()->getCartes());
        cout <<"|"<< endl;
    }
    cout << endl;
}

void Controleur::afficherCartesNonPose() const
{
    std::cout << "Cartes non posées : " << std::endl;
    for (int i = 0; i < m_carte_non_pose.size(); ++i)
    {
        std::cout << "Carte " << i + 1 << ": ";
        if (m_carte_non_pose[i] != nullptr)
            std::cout << m_carte_non_pose[i]->getCouleur() << " "<< m_carte_non_pose[i]->getPuissance()<< std::endl;
        else
            std::cout << "Aucune carte" << std::endl;
    }
}
int Joker1 = 0;
int Joker2=0;
int cartetacJoue1 =0;
int cartetacJoue2 =0;
int testclan = 1;
int testclan2 = 1;
void Controleur::JouerTour1(){
    int attente_valide = 0;
    int print = 0;
    int cartClan = 0;
   // cout <<"taille pioche clan : "<<getPiocheClan()->getNbCartes()<<"\n";
    //cout <<"taille pioche tactique : "<<getPiocheTactique()->getNbCartes()<<"\n";
    //CarteRuse::Jouer_ChasseurdeTete();
    //cout <<"taille pioche clan : "<<getPiocheClan()->getNbCartes()<<"\n";
    //cout <<"taille pioche tactique : "<<getPiocheTactique()->getNbCartes()<<"\n";
    //cout << "Voici votre main :" << endl;
    //m_plateau->afficherMainJoueur(1);

    system ("CLS");
    m_plateau->afficherPlateau(1);
    vector<Carte*> cartesMain = m_plateau->m_joueur1->getMain()->getCartes();

    testclan = 0;
    if (cartesMain.size()!=0) {


        while (attente_valide != 1) {
            cout << "Veuillez choisir la carte que vous voulez jouer (son id) :" << endl;
            int choix_carte, nb_tac = 0, nb_clan = 0, nb_total = 0;
            cin >> choix_carte;
            clean();
            while (choix_carte >= cartesMain.size() || choix_carte < 0) {
                cout << "Vous n'avez pas cette carte, veuillez saisir une carte que vous avez :" << endl;
                cin >> choix_carte;
            }
            vector<Carte *> cartes = m_plateau->m_joueur1->getMain()->getCartes();
            Carte *carte = cartes[choix_carte];

            if (CarteTroupeElite *carteTroupeEliteChoisie = dynamic_cast<CarteTroupeElite *>(carte)) {
                if (cartetacJoue1 < cartetacJoue2 + 1) {
                    if (carteTroupeEliteChoisie->getNom() == "Joker") {
                        Joker1++;
                    }
                    if (Joker1 > 1 && carteTroupeEliteChoisie->getNom() == "Joker") {
                        cout << "Vous ne pouvez pas avoir 2 joker dans la meme partie, selectionnez une autre carte"
                             << endl;
                    } else {
                        cout << "Vous avez choisi la carte : Nom :" << carteTroupeEliteChoisie->getNom() << endl;
                        cout << "Voulez-vous utiliser cette carte sur quelle borne ? :" << endl;
                        int choix_borne;
                        cin >> choix_borne;
                        m_plateau->getBornes(choix_borne)->getCartesJ1()->getBoue() == 0;
                        while ((choix_borne > 8 || choix_borne < 0) ||
                               (m_plateau->getBornes(choix_borne)->getCartesJ1()->getBoue() == 0 &&
                                m_plateau->getBornes(choix_borne)->getCartesJ1()->getCartes().size() == 3) ||
                               (m_plateau->getBornes(choix_borne)->getCartesJ1()->getBoue() == 1 &&
                                m_plateau->getBornes(choix_borne)->getCartesJ1()->getCartes().size() == 4)) {
                            cout << "Vous ne pouvez pas poser votre carte sur cette borne:" << endl;
                            cin >> choix_borne;
                        }
                        testclan++;
                        cout << "On pose la carte choisie sur la borne " << choix_borne << " :" << endl;
                        m_plateau->poser(*m_plateau->getBornes(choix_borne), carteTroupeEliteChoisie);
                        cout << "Borne " << choix_borne << " :" << endl;
                        affichage_vecteur_carteclan(m_plateau->getBornes(choix_borne)->getCartesJ1()->getCartes());
                        cout << endl;
                        cartetacJoue1++;
                        attente_valide = 1;
                        cartClan = 1;
                        m_plateau->getJoueur1()->getMain()->supprimerCarte(choix_carte);
                    }

                } else if (testclan == 0) {
                    cout
                            << "Vous ne pouvez pas jouer cette carte, vous avez une carte tactique de plus que l'adversaire"
                            << endl;
                    print++;

                }
            } else if (CarteClan *carteClanChoisie = dynamic_cast<CarteClan *>(carte)) {
                cout << "Vous avez choisi la carte : Puissance :" << carteClanChoisie->getPuissance() << " Couleur : "
                     << carteClanChoisie->getCouleur() << endl;
                system("CLS");
                m_plateau->afficherPlateau(1);
                cout << "Veuillez choisir une borne :" << endl;
                int choix_borne;
                cin >> choix_borne;
                while ((choix_borne > 8 || choix_borne < 0) ||
                       (m_plateau->getBornes(choix_borne)->getCartesJ1()->getBoue() == 0 &&
                        m_plateau->getBornes(choix_borne)->getCartesJ1()->getCartes().size() == 3) ||
                       (m_plateau->getBornes(choix_borne)->getCartesJ1()->getBoue() == 1 &&
                        m_plateau->getBornes(choix_borne)->getCartesJ1()->getCartes().size() == 4)) {
                    cout << "Vous ne pouvez pas poser de carte sur cette borne :" << endl;
                    cin >> choix_borne;
                }
                system("CLS");
                cout << "On pose la carte choisie sur la borne " << choix_borne << " :" << endl;
                m_plateau->poser(*m_plateau->getBornes(choix_borne), carteClanChoisie);
                m_plateau->afficherPlateau(1);
                m_plateau->getJoueur1()->getMain()->supprimerCarte(choix_carte);
                attente_valide = 1;
                cartClan = 1;
            }
            if (cartClan == 0) {
                if (cartetacJoue1 < cartetacJoue2 + 1) {

                    if (CarteTactique *carteTactiqueChoisie = dynamic_cast<CarteTactique *>(carte)) {

                        cout << "Vous avez choisi la carte : Nom :" << carteTactiqueChoisie->getNom() << endl;

                        if (carteTactiqueChoisie->getNom() == "Colin Maillard") {
                            cout << "Voulez-vous utiliser cette carte sur quelle borne ? :" << endl;
                            int choix_borne;
                            cin >> choix_borne;
                            while (choix_borne > 8 || choix_borne < 0) {
                                cout << "Vous n'avez pas cette borne, veuillez saisir une borne :" << endl;
                                cin >> choix_borne;
                            }
                            cout << "On pose la carte choisie sur la borne " << choix_borne << " :" << endl;
                            CarteModeCombat::jouer_ColinMaillard(m_plateau->getBornes(choix_borne));
                            m_plateau->getJoueur1()->getMain()->supprimerCarte(choix_carte);
                            attente_valide = 1;
                            cartetacJoue1++;
                        } else if (carteTactiqueChoisie->getNom() == "Combat de Boue") {
                            int choix_borne;
                            cin >> choix_borne;
                            while (choix_borne > 8 || choix_borne < 0) {
                                cout << "Vous n'avez pas cette borne, veuillez saisir une borne :" << endl;
                                cin >> choix_borne;
                            }
                            cout << "On pose la carte choisie sur la borne " << choix_borne << " :" << endl;
                            CarteModeCombat::jouer_CombatdeBoue(m_plateau->getBornes(choix_borne));
                            m_plateau->getJoueur1()->getMain()->supprimerCarte(choix_carte);
                            attente_valide = 1;
                        } else if (carteTactiqueChoisie->getNom() == "Chasseur de Tete") {
                            CarteRuse::Jouer_ChasseurdeTete();
                            attente_valide = 1;
                            m_plateau->getJoueur1()->getMain()->supprimerCarte(choix_carte);
                        } else if (carteTactiqueChoisie->getNom() == "Stratege") {
                            for (int i = 0; i < 9; i++) {
                                if (m_plateau->getBornes(i)->getCartesJ1()->getCartes().size() != 0) {
                                    nb_clan++;
                                }

                            }
                            if (nb_clan != 0) {
                                CarteRuse::Jouer_Stratege();
                                attente_valide = 1;
                                m_plateau->getJoueur1()->getMain()->supprimerCarte(choix_carte);
                            } else {
                                cout << "Vous n'avez pas de carte sur le plateau." << endl;
                                attente_valide = 0;
                            }
                            attente_valide = 0;

                        } else if (carteTactiqueChoisie->getNom() == "Banshee") {
                            for (int i = 0; i < 9; i++) {
                                if (m_plateau->getBornes(i)->getCartesJ2()->getCartes().size() != 0) {
                                    nb_clan++;
                                }

                            }
                            if (nb_clan != 0) {
                                CarteRuse::Jouer_Banshee();
                                attente_valide = 1;
                                m_plateau->getJoueur1()->getMain()->supprimerCarte(choix_carte);
                            } else {
                                cout << "Vous n'avez pas de carte sur le plateau." << endl;
                                attente_valide = 0;

                            }

                        } else if (carteTactiqueChoisie->getNom() == "Traitre") {
                            for (int i = 0; i < 9; i++) {
                                if (m_plateau->getBornes(i)->getCartesJ2()->getCartes().size() != 0) {
                                    nb_clan++;
                                }

                            }
                            if (nb_clan != 0) {
                                CarteRuse::Jouer_Traitre();
                                attente_valide = 1;
                                m_plateau->getJoueur1()->getMain()->supprimerCarte(choix_carte);
                            } else {
                                cout << "L'adversaire n'a pas de cartes sur le plateau." << endl;
                                attente_valide = 0;
                            }


                        }
                        //TODO ne sert a rien car une carte troupeELite est une carteCLan
                        cartetacJoue1++;
                    }
                } else if (testclan == 0 && print == 0) {
                    cout
                            << " Vous avez joue une carte tactique de plus que l'adversaire, donc vous pouvez pas jouer de carte tactique."
                            << endl;
                }
            }
        }
    }
    else {
        cout << "Vous passez votre tour car vous n'avez plus de cartes" << endl;
    }
    cout << "Voulez vous revendiquer une borne ? 1 pour oui, 0 pour non :" << endl;
    int choix_revendication;
    cin >> choix_revendication;

    if(choix_revendication==1){
        cout << "Veuillez choisir la borne que vous voulez revendiquer :" << endl;
        int choix_borne;
        cin >> choix_borne;
        while (m_plateau->m_bornes[choix_borne]->getRevendique()==2 || m_plateau->m_bornes[choix_borne]->getRevendique()==1 && (choix_borne > 8 || choix_borne < 0)) {
            cout << "La borne a déjà été revendiquée, veuillez choisir une autre borne :" << endl;
            cin>> choix_borne;
            return;
        }
        auto msg =revendiquer_borne(choix_borne,true);
        cout << msg << endl;
        sleep(3);
        auto cartesBornes = m_plateau->getBornes(choix_borne)->getCartesJ1()->getCartes();
        int i =0;

    }
    system ("CLS");
    m_plateau->afficherPlateau(1);
    cout << "Veuillez choisir si vous voulez choisir une carte tactique ou normale : 0 pour normale et 1 pour  tactique" << endl;
    int choix_style;
    cin >> choix_style;
    while(choix_style!=0 && choix_style!=1){
        cout << "Veuillez choisir si vous voulez choisir une carte tactique ou normale : 0 pour normale et 1 pour  tactique" << endl;
        cin >> choix_style;
    }
    if (choix_style == 0) {
        if( !getPiocheClan()->estVide()) {

            CarteClan *ci = new CarteClan(getPiocheClan()->piocherCarteClan());
            m_plateau->getJoueur1()->getMain()->ajouterCarte(ci);
            cout << "Carte piochee : " << ci->getPuissance() << " " << ci->getCouleur() << endl;

        }
    }
    else if ( !getPiocheTactique()->estVide() && choix_style==1){
        switch (getPiocheTactique()->quandjepiochejefaisattention() ) {
            case 1: {
                cout << "Vous avez pioche la carte tactique :" << endl;
                CarteTactique * a = new CarteTactique(getPiocheTactique()->piocherCarteTactique());
                m_plateau->getJoueur1()->getMain()->ajouterCarte(a);
                cout << a->getNom() << endl;
                break;
            }
            case 2: {
                cout << "Vous avez pioche la carte troupe Elite :" << endl;
                CarteTroupeElite * b = new CarteTroupeElite( getPiocheTactique()->piocherCarteTroupeElite());
                m_plateau->getJoueur1()->getMain()->ajouterCarte(b);
                cout << b->getNom() << endl;
                break;
            }
        }

    }
    else {
        cout << "Vous n'avez plus de cartes dans vos pioches" << endl;
    }
    //Affichage de la main du joueur 1
    system ("CLS");
    cout << "Votre main est maintenant composee de : " << endl;
    m_plateau->afficherPlateau(1);

    //cout <<"taille pioche tactique : "<<getPiocheTactique()->getNbCartes()<<"\n";
    //Afficher_Borne1();
    cout << "\nFIN DU TOUR" << endl;
}


void Controleur::JouerTourIA(){
    int print = 0;
    int cartClan = 0;
    m_plateau->afficherBornes(2);
    m_plateau->afficherMainJoueur(2);
    vector<Carte*> cartesMain = m_plateau->m_joueur2->getMain()->getCartes();
    int attente_valide=0;
    if (cartesMain.size()!=0) {
        while (attente_valide != 1) {

            int choix_carte, nb_tac = 0, nb_clan = 0, nb_total = 0;
            choix_carte = generateRandomNumber() % cartesMain.size();
            clean();
            while (choix_carte >= cartesMain.size() || choix_carte < 0) {

                choix_carte = generateRandomNumber() % cartesMain.size();
            }
            vector<Carte *> cartes = m_plateau->m_joueur2->getMain()->getCartes();
            Carte *carte = cartes[choix_carte];
            if (CarteTroupeElite *carteTroupeEliteChoisie = dynamic_cast<CarteTroupeElite *>(carte)) {
                if (cartetacJoue2 < cartetacJoue1 + 1) {
                    if (carteTroupeEliteChoisie->getNom() == "Joker") {
                        Joker2++;
                    }
                    if (Joker2 > 1 && carteTroupeEliteChoisie->getNom() == "Joker") {
                        cout << "Vous ne pouvez pas avoir 2 joker dans la meme partie, selectionnez une autre carte"
                             << endl;
                    } else {
                        int choix_borne;
                        choix_borne = generateRandomNumber() % 8 + 1;
                        while ((choix_borne > 8 || choix_borne < 0) ||
                               (m_plateau->getBornes(choix_borne)->getCartesJ2()->getBoue() == 0 &&
                                m_plateau->getBornes(choix_borne)->getCartesJ2()->getCartes().size() == 3) ||
                               (m_plateau->getBornes(choix_borne)->getCartesJ2()->getBoue() == 1 &&
                                m_plateau->getBornes(choix_borne)->getCartesJ2()->getCartes().size() == 4)) {
                            choix_borne = generateRandomNumber() % 8 + 1;
                        }
                        cout << "L'ia pose la carte choisie sur la borne " << choix_borne << " :" << endl;
                        m_plateau->poser(*m_plateau->getBornes(choix_borne), carteTroupeEliteChoisie);
                        cout << "Borne " << choix_borne << " :" << endl;
                        cartClan = 1;
                        cout << endl;
                        attente_valide = 1;
                        cartetacJoue2++;
                        m_plateau->getJoueur2()->getMain()->supprimerCarte(choix_carte);
                    }
                } else if (testclan2 == 0) {
                    cout
                            << "Vous ne pouvez pas jouer cette carte, vous avez joue une carte tactique de plus que l'adversaire."
                            << endl;
                    print++;
                }
            } else if (CarteClan *carteClanChoisie = dynamic_cast<CarteClan *>(carte)) {
                cout << "L'ia a choisi la carte : Puissance :" << carteClanChoisie->getPuissance() << " Couleur : "
                     << carteClanChoisie->getCouleur() << endl;
                m_plateau->afficherPlateau(2);

                int choix_borne;
                choix_borne = generateRandomNumber() % 8 + 1;
                while ((choix_borne > 8 || choix_borne < 0) ||
                       (m_plateau->getBornes(choix_borne)->getCartesJ2()->getBoue() == 0 &&
                        m_plateau->getBornes(choix_borne)->getCartesJ2()->getCartes().size() == 3) ||
                       (m_plateau->getBornes(choix_borne)->getCartesJ2()->getBoue() == 1 &&
                        m_plateau->getBornes(choix_borne)->getCartesJ2()->getCartes().size() == 4)) {
                    choix_borne = generateRandomNumber() % 8 + 1;
                }
                cout << "L'ia pose la carte choisie sur la borne " << choix_borne << " :" << endl;
                m_plateau->poser(*m_plateau->getBornes(choix_borne), carteClanChoisie);
                m_plateau->afficherPlateau(2);
                m_plateau->getJoueur2()->getMain()->supprimerCarte(choix_carte);
                attente_valide = 1;
                cartClan = 1;
            }
            if (cartClan == 0) {
                if (cartetacJoue2 < cartetacJoue1 + 1) {


                    if (CarteTactique *carteTactiqueChoisie = dynamic_cast<CarteTactique *>(carte)) {
                        cout << "L'ia a choisi la carte : Nom :" << carteTactiqueChoisie->getNom() << endl;

                        if (carteTactiqueChoisie->getNom() == "Colin Maillard") {
                            int choix_borne;
                            choix_borne = generateRandomNumber() % 8 + 1;
                            while ((choix_borne > 8 || choix_borne < 0) ||
                                   (m_plateau->getBornes(choix_borne)->getCartesJ2()->getBoue() == 0 &&
                                    m_plateau->getBornes(choix_borne)->getCartesJ2()->getCartes().size() == 3) ||
                                   (m_plateau->getBornes(choix_borne)->getCartesJ2()->getBoue() == 1 &&
                                    m_plateau->getBornes(choix_borne)->getCartesJ2()->getCartes().size() == 4)) {
                                choix_borne = generateRandomNumber() % 8 + 1;
                            }
                            cout << "L'ia pose la carte choisie sur la borne " << choix_borne << " :" << endl;
                            CarteModeCombat::jouer_ColinMaillard(m_plateau->getBornes(choix_borne));
                            m_plateau->getJoueur2()->getMain()->supprimerCarte(choix_carte);
                            attente_valide = 1;
                        } else if (carteTactiqueChoisie->getNom() == "Combat de Boue") {
                            int choix_borne;
                            choix_borne = generateRandomNumber() % 8 + 1;
                            while ((choix_borne > 8 || choix_borne < 0) ||
                                   (m_plateau->getBornes(choix_borne)->getCartesJ2()->getBoue() == 0 &&
                                    m_plateau->getBornes(choix_borne)->getCartesJ2()->getCartes().size() == 3) ||
                                   (m_plateau->getBornes(choix_borne)->getCartesJ2()->getBoue() == 1 &&
                                    m_plateau->getBornes(choix_borne)->getCartesJ2()->getCartes().size() == 4)) {
                                choix_borne = generateRandomNumber() % 8 + 1;
                            }
                            CarteModeCombat::jouer_CombatdeBoue(m_plateau->getBornes(choix_borne));
                            m_plateau->getJoueur2()->getMain()->supprimerCarte(choix_carte);
                            attente_valide = 1;
                        } else if (carteTactiqueChoisie->getNom() == "Chasseur de Tete") {
                            CarteRuse::Jouer_ChasseurdeTeteIA();
                            attente_valide = 1;
                            m_plateau->getJoueur2()->getMain()->supprimerCarte(choix_carte);
                        } else if (carteTactiqueChoisie->getNom() == "Stratege") {
                            for (int i = 0; i < 9; i++) {
                                if (m_plateau->getBornes(i)->getCartesJ2()->getCartes().size() != 0) {
                                    nb_clan++;
                                }

                            }
                            if (nb_clan != 0) {
                                CarteRuse::Jouer_StrategeIA();
                                attente_valide = 1;
                                m_plateau->getJoueur2()->getMain()->supprimerCarte(choix_carte);
                            } else {

                                attente_valide = 0;
                            }


                        } else if (carteTactiqueChoisie->getNom() == "Banshee") {
                            for (int i = 0; i < 9; i++) {
                                if (m_plateau->getBornes(i)->getCartesJ1()->getCartes().size() != 0) {
                                    nb_clan++;
                                }

                            }
                            if (nb_clan != 0) {
                                CarteRuse::Jouer_BansheeIA();
                                attente_valide = 1;
                                m_plateau->getJoueur2()->getMain()->supprimerCarte(choix_carte);
                            } else {
                                attente_valide = 0;
                            }

                        } else if (carteTactiqueChoisie->getNom() == "Traitre") {
                            for (int i = 0; i < 9; i++) {
                                if (m_plateau->getBornes(i)->getCartesJ1()->getCartes().size() != 0) {
                                    nb_clan++;
                                }

                            }
                            if (nb_clan != 0) {
                                CarteRuse::Jouer_TraitreIA();
                                attente_valide = 1;
                                m_plateau->getJoueur2()->getMain()->supprimerCarte(choix_carte);
                            } else {
                                attente_valide = 0;
                            }

                        }
                        cartetacJoue2++;
                    }
                } else if (testclan == 0 && print == 0) {
                    cout
                            << "Vous avez joue une carte tactique de plus que l'adversaire, donc vous pouvez pas jouer de carte tactique."
                            << endl;

                }
            }
        }
    }
    else {
        cout << "Vous passez votre tour car vous n'avez plus de cartes" << endl;
    }
    //L'ia cherche à revendiquer les bornes où elle a 3 cartes
    int count_cartes = 0;
    for (int i=0 ;i <9;i++) {
        count_cartes=0;
        auto cartesBornes = m_plateau->getBornes(i)->getCartesJ2()->getCartes();
        if (m_plateau->m_bornes[i]->getRevendique()==2 || m_plateau->m_bornes[i]->getRevendique()==1 ) {
            return;
        }
        else if (cartesBornes.size() ==3 ||cartesBornes.size()==4 ) {
            cout << "L'ia va tenter de  revendiquer la borne " << i << endl;
            auto msg =revendiquer_borne(i,false);
            cout << msg << endl;

        }
    }
    int choix_style = generateRandomNumber()%4;
    if (choix_style==3){
     if ( !getPiocheTactique()->estVide()){
        switch (getPiocheTactique()->quandjepiochejefaisattention() ) {
            case 1: {
                cout << "Vous avez pioche la carte tactique :" << endl;
                CarteTactique * a = new CarteTactique(getPiocheTactique()->piocherCarteTactique());
                m_plateau->getJoueur2()->getMain()->ajouterCarte(a);
                cout << a->getNom() << endl;
                break;
            }
            case 2: {
                cout << "Vous avez pioche la carte troupe Elite :" << endl;
                CarteTroupeElite * b = new CarteTroupeElite( getPiocheTactique()->piocherCarteTroupeElite());
                m_plateau->getJoueur2()->getMain()->ajouterCarte(b);
                cout << b->getNom() << endl;
                break;
            }
        }

    }
    }

    else if (choix_style != 3) {
        if( !getPiocheClan()->estVide()) {
            CarteClan *ci = new CarteClan(getPiocheClan()->piocherCarteClan());
            m_plateau->getJoueur2()->getMain()->ajouterCarte(ci);
            cout << "Carte piochee : " << ci->getPuissance() << " " << ci->getCouleur() << endl;

        }
    }


    else {
        cout << "Vous n'avez plus de cartes dans vos pioches" << endl;
    }
    //Affichage de la main du joueur 1

    cout << "\nFIN DU TOUR" << endl;

}

void Controleur::JouerTour2(){
    int attente_valide = 0;
    int print = 0;
    int cartClan = 0;
    // cout <<"taille pioche clan : "<<getPiocheClan()->getNbCartes()<<"\n";
    //cout <<"taille pioche tactique : "<<getPiocheTactique()->getNbCartes()<<"\n";
    //CarteRuse::Jouer_ChasseurdeTete();
    //cout <<"taille pioche clan : "<<getPiocheClan()->getNbCartes()<<"\n";
    //cout <<"taille pioche tactique : "<<getPiocheTactique()->getNbCartes()<<"\n";
    //cout << "Voici votre main :" << endl;
    //m_plateau->afficherMainJoueur(1);

    system ("CLS");
    m_plateau->afficherPlateau(2);
    vector<Carte*> cartesMain = m_plateau->m_joueur2->getMain()->getCartes();

    testclan = 0;
    if (cartesMain.size()!=0) {


        while (attente_valide != 1) {
            cout << "Veuillez choisir la carte que vous voulez jouer (son id) :" << endl;
            int choix_carte, nb_tac = 0, nb_clan = 0, nb_total = 0;
            cin >> choix_carte;
            clean();
            while (choix_carte >= cartesMain.size() || choix_carte < 0) {
                cout << "Vous n'avez pas cette carte, veuillez saisir une carte que vous avez :" << endl;
                cin >> choix_carte;
            }
            vector<Carte *> cartes = m_plateau->m_joueur2->getMain()->getCartes();
            Carte *carte = cartes[choix_carte];

            if (CarteTroupeElite *carteTroupeEliteChoisie = dynamic_cast<CarteTroupeElite *>(carte)) {
                if (cartetacJoue2 < cartetacJoue1 + 1) {
                    if (carteTroupeEliteChoisie->getNom() == "Joker") {
                        Joker2++;
                    }
                    if (Joker1 > 1 && carteTroupeEliteChoisie->getNom() == "Joker") {
                        cout << "Vous ne pouvez pas avoir 2 joker dans la meme partie, selectionnez une autre carte"
                             << endl;
                    } else {
                        cout << "Vous avez choisi la carte : Nom :" << carteTroupeEliteChoisie->getNom() << endl;
                        cout << "Voulez-vous utiliser cette carte sur quelle borne ? :" << endl;
                        int choix_borne;
                        cin >> choix_borne;
                        m_plateau->getBornes(choix_borne)->getCartesJ1()->getBoue() == 0;
                        while ((choix_borne > 8 || choix_borne < 0) ||
                               (m_plateau->getBornes(choix_borne)->getCartesJ2()->getBoue() == 0 &&
                                m_plateau->getBornes(choix_borne)->getCartesJ2()->getCartes().size() == 3) ||
                               (m_plateau->getBornes(choix_borne)->getCartesJ2()->getBoue() == 1 &&
                                m_plateau->getBornes(choix_borne)->getCartesJ2()->getCartes().size() == 4)) {
                            cout << "Vous ne pouvez pas poser votre carte sur cette borne:" << endl;
                            cin >> choix_borne;
                        }
                        testclan2++;
                        cout << "On pose la carte choisie sur la borne " << choix_borne << " :" << endl;
                        m_plateau->poser(*m_plateau->getBornes(choix_borne), carteTroupeEliteChoisie);
                        cout << "Borne " << choix_borne << " :" << endl;
                        affichage_vecteur_carteclan(m_plateau->getBornes(choix_borne)->getCartesJ2()->getCartes());
                        cout << endl;
                        cartetacJoue1++;
                        attente_valide = 1;
                        cartClan = 1;
                        m_plateau->getJoueur2()->getMain()->supprimerCarte(choix_carte);
                    }

                } else if (testclan2 == 0) {
                    cout
                            << "Vous ne pouvez pas jouer cette carte, vous avez une carte tactique de plus que l'adversaire"
                            << endl;
                    print++;

                }
            } else if (CarteClan *carteClanChoisie = dynamic_cast<CarteClan *>(carte)) {
                cout << "Vous avez choisi la carte : Puissance :" << carteClanChoisie->getPuissance() << " Couleur : "
                     << carteClanChoisie->getCouleur() << endl;
                system("CLS");
                m_plateau->afficherPlateau(2);
                cout << "Veuillez choisir une borne :" << endl;
                int choix_borne;
                cin >> choix_borne;
                while ((choix_borne > 8 || choix_borne < 0) ||
                       (m_plateau->getBornes(choix_borne)->getCartesJ2()->getBoue() == 0 &&
                        m_plateau->getBornes(choix_borne)->getCartesJ2()->getCartes().size() == 3) ||
                       (m_plateau->getBornes(choix_borne)->getCartesJ2()->getBoue() == 1 &&
                        m_plateau->getBornes(choix_borne)->getCartesJ2()->getCartes().size() == 4)) {
                    cout << "Vous ne pouvez pas poser de carte sur cette borne :" << endl;
                    cin >> choix_borne;
                }
                system("CLS");
                cout << "On pose la carte choisie sur la borne " << choix_borne << " :" << endl;
                m_plateau->poser(*m_plateau->getBornes(choix_borne), carteClanChoisie);
                m_plateau->afficherPlateau(2);
                m_plateau->getJoueur2()->getMain()->supprimerCarte(choix_carte);
                attente_valide = 1;
                cartClan = 1;
            }
            if (cartClan == 0) {
                if (cartetacJoue1 < cartetacJoue2 + 1) {

                    if (CarteTactique *carteTactiqueChoisie = dynamic_cast<CarteTactique *>(carte)) {

                        cout << "Vous avez choisi la carte : Nom :" << carteTactiqueChoisie->getNom() << endl;

                        if (carteTactiqueChoisie->getNom() == "Colin Maillard") {
                            cout << "Voulez-vous utiliser cette carte sur quelle borne ? :" << endl;
                            int choix_borne;
                            cin >> choix_borne;
                            while (choix_borne > 8 || choix_borne < 0) {
                                cout << "Vous n'avez pas cette borne, veuillez saisir une borne :" << endl;
                                cin >> choix_borne;
                            }
                            cout << "On pose la carte choisie sur la borne " << choix_borne << " :" << endl;
                            CarteModeCombat::jouer_ColinMaillard(m_plateau->getBornes(choix_borne));
                            m_plateau->getJoueur2()->getMain()->supprimerCarte(choix_carte);
                            attente_valide = 1;
                            cartetacJoue2++;
                        } else if (carteTactiqueChoisie->getNom() == "Combat de Boue") {
                            int choix_borne;
                            cin >> choix_borne;
                            while (choix_borne > 8 || choix_borne < 0) {
                                cout << "Vous n'avez pas cette borne, veuillez saisir une borne :" << endl;
                                cin >> choix_borne;
                            }
                            cout << "On pose la carte choisie sur la borne " << choix_borne << " :" << endl;
                            CarteModeCombat::jouer_CombatdeBoue(m_plateau->getBornes(choix_borne));
                            m_plateau->getJoueur2()->getMain()->supprimerCarte(choix_carte);
                            attente_valide = 1;
                        } else if (carteTactiqueChoisie->getNom() == "Chasseur de Tete") {
                            CarteRuse::Jouer_ChasseurdeTete2();
                            attente_valide = 1;
                            m_plateau->getJoueur1()->getMain()->supprimerCarte(choix_carte);
                        } else if (carteTactiqueChoisie->getNom() == "Stratege") {
                            for (int i = 0; i < 9; i++) {
                                if (m_plateau->getBornes(i)->getCartesJ2()->getCartes().size() != 0) {
                                    nb_clan++;
                                }

                            }
                            if (nb_clan != 0) {
                                CarteRuse::Jouer_Stratege2();
                                attente_valide = 1;
                                m_plateau->getJoueur2()->getMain()->supprimerCarte(choix_carte);
                            } else {
                                cout << "Vous n'avez pas de carte sur le plateau." << endl;
                                attente_valide = 0;
                            }
                            attente_valide = 0;

                        } else if (carteTactiqueChoisie->getNom() == "Banshee") {
                            for (int i = 0; i < 9; i++) {
                                if (m_plateau->getBornes(i)->getCartesJ1()->getCartes().size() != 0) {
                                    nb_clan++;
                                }

                            }
                            if (nb_clan != 0) {
                                CarteRuse::Jouer_Banshee2();
                                attente_valide = 1;
                                m_plateau->getJoueur2()->getMain()->supprimerCarte(choix_carte);
                            } else {
                                cout << "Vous n'avez pas de carte sur le plateau." << endl;
                                attente_valide = 0;

                            }

                        } else if (carteTactiqueChoisie->getNom() == "Traitre") {
                            for (int i = 0; i < 9; i++) {
                                if (m_plateau->getBornes(i)->getCartesJ1()->getCartes().size() != 0) {
                                    nb_clan++;
                                }

                            }
                            if (nb_clan != 0) {
                                CarteRuse::Jouer_Traitre2();
                                attente_valide = 1;
                                m_plateau->getJoueur2()->getMain()->supprimerCarte(choix_carte);
                            } else {
                                cout << "L'adversaire n'a pas de cartes sur le plateau." << endl;
                                attente_valide = 0;
                            }


                        }
                        //TODO ne sert a rien car une carte troupeELite est une carteCLan
                        cartetacJoue2++;
                    }
                } else if (testclan2 == 0 && print == 0) {
                    cout
                            << " Vous avez joue une carte tactique de plus que l'adversaire, donc vous pouvez pas jouer de carte tactique."
                            << endl;
                }
            }
        }
    }
    else {
        cout << "Vous passez votre tour car vous n'avez plus de cartes" << endl;
    }
    cout << "Voulez vous revendiquer une borne ? 1 pour oui, 0 pour non :" << endl;
    int choix_revendication;
    cin >> choix_revendication;

    if(choix_revendication==1){
        cout << "Veuillez choisir la borne que vous voulez revendiquer :" << endl;
        int choix_borne;
        cin >> choix_borne;
        while (m_plateau->m_bornes[choix_borne]->getRevendique()==2 || m_plateau->m_bornes[choix_borne]->getRevendique()==1 && (choix_borne > 8 || choix_borne < 0)) {
            cout << "La borne a déjà été revendiquée, veuillez choisir une autre borne :" << endl;
            cin>> choix_borne;
            return;
        }
        auto msg =revendiquer_borne(choix_borne,true);
        cout << msg << endl;
        sleep(3);
        auto cartesBornes = m_plateau->getBornes(choix_borne)->getCartesJ2()->getCartes();
        int i =0;

    }
    system ("CLS");
    m_plateau->afficherPlateau(2);
    cout << "Veuillez choisir si vous voulez choisir une carte tactique ou normale : 0 pour normale et 1 pour  tactique" << endl;
    int choix_style;
    cin >> choix_style;
    while(choix_style!=0 && choix_style!=1){
        cout << "Veuillez choisir si vous voulez choisir une carte tactique ou normale : 0 pour normale et 1 pour  tactique" << endl;
        cin >> choix_style;
    }
    if (choix_style == 0) {
        if( !getPiocheClan()->estVide()) {

            CarteClan *ci = new CarteClan(getPiocheClan()->piocherCarteClan());
            m_plateau->getJoueur2()->getMain()->ajouterCarte(ci);
            cout << "Carte piochee : " << ci->getPuissance() << " " << ci->getCouleur() << endl;

        }
    }
    else if ( !getPiocheTactique()->estVide() && choix_style==1){
        switch (getPiocheTactique()->quandjepiochejefaisattention() ) {
            case 1: {
                cout << "Vous avez pioche la carte tactique :" << endl;
                CarteTactique * a = new CarteTactique(getPiocheTactique()->piocherCarteTactique());
                m_plateau->getJoueur2()->getMain()->ajouterCarte(a);
                cout << a->getNom() << endl;
                break;
            }
            case 2: {
                cout << "Vous avez pioche la carte troupe Elite :" << endl;
                CarteTroupeElite * b = new CarteTroupeElite( getPiocheTactique()->piocherCarteTroupeElite());
                m_plateau->getJoueur2()->getMain()->ajouterCarte(b);
                cout << b->getNom() << endl;
                break;
            }
        }

    }
    else {
        cout << "Vous n'avez plus de cartes dans vos pioches" << endl;
    }
    //Affichage de la main du joueur 2
    system ("CLS");
    cout << "Votre main est maintenant composee de : " << endl;
    m_plateau->afficherPlateau(2);

    //cout <<"taille pioche tactique : "<<getPiocheTactique()->getNbCartes()<<"\n";
    //Afficher_Borne1();
    cout << "\nFIN DU TOUR" << endl;
}

void Controleur::JouerTourIAClassique(){

    m_plateau->afficherBornes(2);
    m_plateau->afficherMainJoueur(2);
    vector<Carte*> cartesMain = m_plateau->m_joueur1->getMain()->getCartes();
    srand(time(0));
    int choix_carte, nb_tac=0,nb_clan=0,nb_total=0;
    choix_carte = generateRandomNumber()%cartesMain.size();
    while(choix_carte>cartesMain.size() || choix_carte<0){

        choix_carte = generateRandomNumber()%cartesMain.size()+1;
    }
    vector<Carte*> cartes = m_plateau->m_joueur2->getMain()->getCartes();
    Carte* carte = cartes[choix_carte];
    if (carte = dynamic_cast<CarteClan *>(carte)) {
        Carte* carteChoisie = m_plateau->getJoueur2()->getMain()->getCarte(choix_carte);
        CarteClan* carteClanChoisie = dynamic_cast<CarteClan*>(carteChoisie);
            cout << "L'IA a choisi de poser la carte : Puissance :" << carteClanChoisie->getPuissance() << " Couleur : " << carteClanChoisie->getCouleur() << endl;
        //affichage borne
        //Choix de la borne
        int choix_borne = generateRandomNumber()%8+1;
        if (choix_borne > 8|| choix_borne < 0) {
            choix_borne = generateRandomNumber()%8+1;
        }

        while (m_plateau->getBornes(choix_borne)->getCartesJ2()->getCartes().size()==3){
            cout << "La borne est pleine, veuillez choisir une autre borne :" << endl;
            choix_borne = generateRandomNumber()%8+1;
        }


        affichage_vecteur_carteclan(m_plateau->getBornes(choix_borne)->getCartesJ2()->getCartes());cout <<endl;
        cout << "On pose la carte piochee sur la borne "<< choix_borne << " :"<<endl;
        m_plateau->poser(*m_plateau->getBornes(choix_borne),carteClanChoisie );
        cout << "Borne " << choix_borne << " :" << endl;
        affichage_vecteur_carteclan(m_plateau->getBornes(choix_borne)->getCartesJ2()->getCartes());cout <<endl;
        m_plateau->getJoueur2()->getMain()->supprimerCarte(choix_carte);
    }
    m_plateau->afficherBornes(2);
    int count_cartes = 0;
    for (int i=0 ;i <9;i++) {
        count_cartes=0;
        auto cartesBornes = m_plateau->getBornes(i)->getCartesJ2()->getCartes();
            if (cartesBornes.size() ==3) {
                count_cartes = 3;
                auto msg =revendiquer_borne(i,false);
                cout << msg << endl;
                cout << "L'ia a  revendique la borne " << i << endl;
        }
    }

    cout <<"taille pioche clan : "<<getPiocheClan()->getNbCartes()<<"\n";
        CarteClan* ci = new CarteClan(getPiocheClan()->piocherCarteClan());
        m_plateau->getJoueur2()->getMain()->ajouterCarte(ci);
        cout <<"L'ia a piochee la carte  : "<< ci->getPuissance() <<" "<< ci->getCouleur() << endl;
        m_plateau->afficherMainJoueur(1);
    //jouer carte espion
    //Affichage de la main du joueur 1
    cout << "La main de l'IA est maintenant composee de :" << endl;
    m_plateau->afficherMainJoueur(2);
    cout << "\nFIN DU TOUR" << endl;
}


void Controleur::JouerTourClassique1(){
    system ("CLS");
    vector<Carte*> cartesMain = m_plateau->m_joueur1->getMain()->getCartes();
    m_plateau->afficherPlateau(1);
    cout<<"Veuillez choisir la carte que vous voulez jouer (son id) :"<<endl;
    int choix_carte, nb_tac=0,nb_clan=0,nb_total=0;
    cin>>choix_carte; clean();
    while(choix_carte>cartesMain.size() || choix_carte<0){
        cout<<"Vous n'avez pas cette carte, resaisissez une que vous avez :"<<endl;
        cin>>choix_carte;
    }
    vector<Carte*> cartes = m_plateau->m_joueur1->getMain()->getCartes();
    Carte* carte = cartes[choix_carte];
    if (auto carte_choisie = dynamic_cast<CarteClan *>(carte)) {
        for (auto carte: cartes) {
            nb_clan++;
            nb_total++;
        }
        if (nb_clan==0){
            cout << "Vous n'avez plus de cartes clans !" << endl;
            return;
        }
        if (nb_total ==0 ){
            cout << "Vous n'avez plus de cartes !" << endl;
            return; exit(0);
        }

        system ("CLS");
        cout << "Carte choisie : " << carte_choisie->getCouleur() << " " << carte_choisie->getPuissance() << endl;
        m_plateau->afficherPlateau(1);
        cout << "Veuillez choisir une borne :" << endl;
        int choix_borne;
        cin >> choix_borne;
        while (choix_borne > 8 || choix_borne < 0) {
            cout << "Vous n'avez pas cette borne, veuillez saisir une borne :" << endl;
            cin >> choix_borne;
        }
        while (m_plateau->getBornes(choix_borne)->getCartesJ1()->getCartes().size()==3){
            cout << "La borne est pleine, veuillez choisir une autre borne :" << endl;
            cin >> choix_borne;
        }

        m_plateau->poser(*m_plateau->getBornes(choix_borne),carte_choisie);
        m_plateau->getJoueur1()->getMain()->supprimerCarte(choix_carte);
        system ("CLS");
        m_plateau->afficherPlateau(1);

    }


    cout << "Voulez vous revendiquer une borne ? 1 pour oui, 0 pour non :" << endl;
    int choix_revendication;
    cin >> choix_revendication;

    if(choix_revendication==1) {
        cout << "Veuillez choisir la borne que vous voulez revendiquer :" << endl;
        int choix_borne;
        cin >> choix_borne;
        while (m_plateau->m_bornes[choix_borne]->getRevendique() == 2 ||
               m_plateau->m_bornes[choix_borne]->getRevendique() == 1 && (choix_borne > 9 || choix_borne < 1)) {
            cout << "La borne a déjà été revendiquée, veuillez choisir une autre borne :" << endl;
            cin >> choix_borne;
            return;
        }
        auto msg =revendiquer_borne(choix_borne,true);
        cout << msg << endl;
        sleep(3);
    }
    CarteClan* ci = new CarteClan(getPiocheClan()->piocherCarteClan());

    m_plateau->getJoueur1()->getMain()->ajouterCarte(ci);
    system ("CLS");
    m_plateau->afficherPlateau(1);
    cout <<"Carte piochee : "<< ci->getPuissance() <<" "<< ci->getCouleur() << endl;
    cout << "\nFIN DU TOUR" << endl;

}

void Controleur::JouerTourClassique2(){
    system ("CLS");
    vector<Carte*> cartesMain = m_plateau->m_joueur2->getMain()->getCartes();
    m_plateau->afficherPlateau(2);
    cout<<"Veuillez choisir la carte que vous voulez jouer (son id) :"<<endl;
    int choix_carte, nb_tac=0,nb_clan=0,nb_total=0;
    cin>>choix_carte; clean();
    while(choix_carte>cartesMain.size() || choix_carte<0){
        cout<<"Vous n'avez pas cette carte, resaisissez une que vous avez :"<<endl;
        cin>>choix_carte;
    }
    vector<Carte*> cartes = m_plateau->m_joueur2->getMain()->getCartes();
    Carte* carte = cartes[choix_carte];
    if (auto carte_choisie = dynamic_cast<CarteClan *>(carte)) {
        for (auto carte: cartes) {
            nb_clan++;
            nb_total++;
        }
        if (nb_clan==0){
            cout << "Vous n'avez plus de cartes clans !" << endl;
            return;
        }
        if (nb_total ==0 ){
            cout << "Vous n'avez plus de cartes !" << endl;
            return; exit(0);
        }

        system ("CLS");
        cout << "Carte choisie : " << carte_choisie->getCouleur() << " " << carte_choisie->getPuissance() << endl;
        m_plateau->afficherPlateau(2);
        cout << "Veuillez choisir une borne :" << endl;
        int choix_borne;
        cin >> choix_borne;
        while (choix_borne > 8 || choix_borne < 0) {
            cout << "Vous n'avez pas cette borne, veuillez saisir une borne :" << endl;
            cin >> choix_borne;
        }
        while (m_plateau->getBornes(choix_borne)->getCartesJ2()->getCartes().size()==3){
            cout << "La borne est pleine, veuillez choisir une autre borne :" << endl;
            cin >> choix_borne;
        }

        m_plateau->poser(*m_plateau->getBornes(choix_borne),carte_choisie);
        m_plateau->getJoueur2()->getMain()->supprimerCarte(choix_carte);
        system ("CLS");
        m_plateau->afficherPlateau(2);

    }


    cout << "Voulez vous revendiquer une borne ? 1 pour oui, 0 pour non :" << endl;
    int choix_revendication;
    cin >> choix_revendication;

    if(choix_revendication==1) {
        cout << "Veuillez choisir la borne que vous voulez revendiquer :" << endl;
        int choix_borne;
        cin >> choix_borne;
        while (m_plateau->m_bornes[choix_borne]->getRevendique() == 2 ||
               m_plateau->m_bornes[choix_borne]->getRevendique() == 1 && (choix_borne > 9 || choix_borne < 1)) {
            cout << "La borne a déjà été revendiquée, veuillez choisir une autre borne :" << endl;
            cin >> choix_borne;
            return;
        }
        auto msg =revendiquer_borne(choix_borne,true);
        cout << msg << endl;
        cout << "appuie" ;
        string oui;
        cin >>  oui;
    }
    CarteClan* ci = new CarteClan(getPiocheClan()->piocherCarteClan());

    m_plateau->getJoueur2()->getMain()->ajouterCarte(ci);
    system ("CLS");
    m_plateau->afficherPlateau(2);
    cout <<"Carte piochee : "<< ci->getPuissance() <<" "<< ci->getCouleur() << endl;
    cout << "\nFIN DU TOUR" << endl;

}



bool Controleur::check_fin_partie() {
    int sum1=0;
    int serie1=0;
    int sum2=0;
    int serie2=0;
    for(auto i=0; i<9; i++){
        int j = m_plateau->m_bornes[i]->getRevendique();
        //printf("test");
        if(j==1){
            sum1++;
            serie1++;
            //printf("test1");
            if(serie1==3){
                m_gagnant=1;
                //printf("test3");
                return true;
            }
            serie2=0;
        }
        else if(j==2){
            sum2++;
            serie2++;
            //printf("test2");
            if(serie2==3){
                m_gagnant=2;
                return true;
            }
            serie1=0;
        }
        if (sum1 == 5){
            m_gagnant=1;
            return true;
        }
        else if (sum2 == 5){
            m_gagnant=2;
            return true;
        }
    }
    return false;
}

std::string Controleur::revendiquer_borne(int num_borne, bool humain) {

    Borne *borne = m_plateau->getBornes(num_borne);
    Combinaison *combi_j1 = borne->getCartesJ1();
    Combinaison *combi_j2 = borne->getCartesJ2();
    vector<CarteClan *> cartes_pose_j1 = combi_j1->getCartes();
    vector<CarteClan *> cartes_pose_j2 = combi_j2->getCartes();

    //TODO temporaire:
    cout << "Les cartes sont\nJ1:  "<< endl;
    affichage_vecteur_carteclan(cartes_pose_j1);
    cout << "\nJ2:" << endl;
    affichage_vecteur_carteclan(cartes_pose_j2);
    cout <<"\n"<<endl;
    // on verifie que la borne n'a pas deja ete revendiquee
    if (borne->getRevendique() != 0){
        return "La borne a deja ete revendiquee";
    }

    int joueur_qui_revendique = getPlateau()->getJoueurActif();
    // on verifie que le joueur a bien pose 3 ou 4 cartes
    if (joueur_qui_revendique == 1){
        if(combi_j1->getBoue() == 0){
            if (cartes_pose_j1.size() !=3){
                return "J1 n'a pas pose 3 cartes, il ne peut pas revendiquer la borne";
            }
        }
        else{
            if (cartes_pose_j1.size() !=4){
                return "J1 n'a pas pose 4 cartes, il ne peut pas revendiquer la borne";
            }
        }
    }
    else{
        if(combi_j2->getBoue() == 0){
            if (cartes_pose_j2.size() !=3){
                return "J2 n'a pas pose 3 cartes, il ne peut pas revendiquer la borne";
            }
        }
        else{
            if (cartes_pose_j2.size() !=4){
                return "J2 n'a pas pose 4 cartes, il ne peut pas revendiquer la borne";
            }
        }
    }


    if (joueur_qui_revendique == 1) {
        cout << "J1 revendique la borne "<< num_borne << endl;
        if (humain) {
            //Si on a des cartes troupes d'elite alors J1 choisi leur valeur
            if (il_y_a_joker(cartes_pose_j1) || il_y_a_portebou(cartes_pose_j1) || il_y_a_espion(cartes_pose_j1)) {
                for (auto c: cartes_pose_j1) {
                    if (auto t = dynamic_cast<CarteTroupeElite *>(c)) {
                        auto nom = t->getNom();
                        if (nom == "Joker") {
                            t->jouer_Joker();
                            combi_j1->calculerForceCombi();
                            combi_j1->setTotalPuissance(
                                    combi_j1->getTotalPuissance() + static_cast<int>(t->getPuissance()));
                        } else if (nom == "Espion") {
                            t->jouer_Espion();
                            combi_j1->calculerForceCombi();
                            combi_j1->setTotalPuissance(
                                    combi_j1->getTotalPuissance() + static_cast<int>(t->getPuissance()));

                        } else if (nom == "Porte_Bouclier") {
                            t->jouer_PorteBouclier();
                            combi_j1->calculerForceCombi();
                            combi_j1->setTotalPuissance(
                                    combi_j1->getTotalPuissance() + static_cast<int>(t->getPuissance()));

                        }
                    }
                }
            }
        }
        else{
            if (humain) {
                if (il_y_a_joker(cartes_pose_j1) || il_y_a_portebou(cartes_pose_j1) || il_y_a_espion(cartes_pose_j1)) {
                    //Si on a des cartes troupes d'elite alors J1 choisi leur valeur
                    for (auto c: cartes_pose_j1) {
                        if (auto t = dynamic_cast<CarteTroupeElite *>(c)) {
                            auto nom = t->getNom();
                            if (nom == "Joker") {
                                t->jouer_JokerIA();
                                combi_j1->calculerForceCombi();
                                combi_j1->setTotalPuissance(
                                        combi_j1->getTotalPuissance() + static_cast<int>(t->getPuissance()));
                            } else if (nom == "Espion") {
                                t->jouer_EspionIA();
                                combi_j1->calculerForceCombi();
                                combi_j1->setTotalPuissance(
                                        combi_j1->getTotalPuissance() + static_cast<int>(t->getPuissance()));

                            } else if (nom == "Porte_Bouclier") {
                                t->jouer_PorteBouclierIA();
                                combi_j1->calculerForceCombi();
                                combi_j1->setTotalPuissance(
                                        combi_j1->getTotalPuissance() + static_cast<int>(t->getPuissance()));

                            }
                        }
                    }
                }
            }
        }

        int tab_troupe_tac[4] = {0, 0, 0, 0};
        if (il_y_a_joker(cartes_pose_j2) || il_y_a_portebou(cartes_pose_j2) || il_y_a_espion(cartes_pose_j2)) {
            // J2 a posé des carte troupe d'elite mais J1 revendique : on teste tt les valeurs possible que peuvent prendre les cartes de J2
            size_t i = 0;
            for (auto c: cartes_pose_j2) {
                if (auto t = dynamic_cast<CarteTroupeElite *>(c)) {
                    auto nom = t->getNom();
                    if (nom == "Joker") {
                        tab_troupe_tac[i] = 1;
                    } else if (nom == "Espion") {
                        tab_troupe_tac[i] = 2;
                    } else if (nom == "Porte_Bouclier") {
                        tab_troupe_tac[i] = 3;
                    }
                }
                i++;
            }
        }
        if (combi_j1->getBoue() == 0) {
            // creation des vecteur sur les quels les cartes de J2 vont varier
            vector<CarteClan *> cartes_a1 = m_carte_non_pose;
            vector<CarteClan *> cartes_a2 = m_carte_non_pose;
            vector<CarteClan *> cartes_a3 = m_carte_non_pose;
            if (cartes_pose_j2.size()) {
                for (auto i = 0; i < cartes_pose_j2.size() ; i++)
                    switch (i) {
                        case (0): {
                            switch (tab_troupe_tac[i]) {
                                case (0): {
                                    cartes_a1.clear();
                                    cartes_a1.push_back(cartes_pose_j2[i]);
                                    break;
                                }
                                case (1): {
                                    cartes_a1.clear();
                                    cartes_a1 = gen_vect_joker();
                                    break;
                                }
                                case (2): {
                                    cartes_a1.clear();
                                    cartes_a1 = gen_vect_espion();
                                    break;
                                }
                                case (3): {
                                    cartes_a1.clear();
                                    cartes_a1 = gen_vect_portebou();
                                    break;
                                }
                            }
                            break;
                        }
                        case (1): {
                            switch (tab_troupe_tac[i]) {
                                case (0): {
                                    cartes_a2.clear();
                                    cartes_a2.push_back(cartes_pose_j2[i]);
                                    break;
                                }
                                case (1): {
                                    cartes_a2.clear();
                                    cartes_a2 = gen_vect_joker();
                                    break;
                                }
                                case (2): {
                                    cartes_a2.clear();
                                    cartes_a2 = gen_vect_espion();
                                    break;
                                }
                                case (3): {
                                    cartes_a2.clear();
                                    cartes_a2 = gen_vect_portebou();
                                    break;
                                }
                            }
                            break;
                        }
                        case (2): {
                            switch (tab_troupe_tac[i]) {
                                case (0): {
                                    cartes_a3.clear();
                                    cartes_a3.push_back(cartes_pose_j2[i]);
                                    break;
                                }
                                case (1): {
                                    cartes_a3.clear();
                                    cartes_a3 = gen_vect_joker();
                                    break;
                                }
                                case (2): {
                                    cartes_a3.clear();
                                    cartes_a3 = gen_vect_espion();
                                    break;
                                }
                                case (3): {
                                    cartes_a3.clear();
                                    cartes_a3 = gen_vect_portebou();
                                    break;
                                }
                            }
                            break;
                        }
                    }
            }


            for (auto a1: cartes_a1) {
                for (auto a2: cartes_a2) {
                    for (auto a3: cartes_a3) {
                        if (CarteClan_egales(a1, a2) || CarteClan_egales(a1, a3) ||
                            CarteClan_egales(a2, a3)  )
                            continue;

                        Combinaison *combi_temp = new Combinaison();
                        combi_temp->ajouterCarte(a1);
                        combi_temp->ajouterCarte(a2);
                        combi_temp->ajouterCarte(a3);
                        if (qui_gagne(combi_j1, combi_temp) == 2) {
                            // J2 gagne
                            std::ostringstream oss;
                            oss << "\nECHEC J2 peut gagner: " << endl;
                            oss << a1->getPuissance() <<" "<< a1->getCouleur() << " "<< a2->getPuissance() <<" "<< a2->getCouleur() << " "<< a3->getPuissance() <<" "<< a3->getCouleur() << endl;
                            delete combi_temp;
                            return oss.str();
                        }

                    }
                }
            }
            std::ostringstream oss;
            oss << "REUSSITE: J2 ne peut pas gagner (J1 gagne)" << endl;
            borne->setRevendique(1);
            return oss.str();
        }
        else {
            vector<CarteClan *> cartes_a1 = m_carte_non_pose;
            vector<CarteClan *> cartes_a2 = m_carte_non_pose;
            vector<CarteClan *> cartes_a3 = m_carte_non_pose;
            vector<CarteClan *> cartes_a4 = m_carte_non_pose;
            if (cartes_pose_j2.size()) {
                for (auto i = 0; i < cartes_pose_j2.size() ; i++)
                    switch (i) {
                        case (0): {
                            switch (tab_troupe_tac[i]) {
                                case (0): {
                                    cartes_a1.clear();
                                    cartes_a1.push_back(cartes_pose_j2[i]);
                                    break;
                                }
                                case (1): {
                                    cartes_a1.clear();
                                    cartes_a1 = gen_vect_joker();
                                    break;
                                }
                                case (2): {
                                    cartes_a1.clear();
                                    cartes_a1 = gen_vect_espion();
                                    break;
                                }
                                case (3): {
                                    cartes_a1.clear();
                                    cartes_a1 = gen_vect_portebou();
                                    break;
                                }
                            }
                            break;
                        }
                        case (1): {
                            switch (tab_troupe_tac[i]) {
                                case (0): {
                                    cartes_a2.clear();
                                    cartes_a2.push_back(cartes_pose_j2[i]);
                                    break;
                                }
                                case (1): {
                                    cartes_a2.clear();
                                    cartes_a2 = gen_vect_joker();
                                    break;
                                }
                                case (2): {
                                    cartes_a2.clear();
                                    cartes_a2 = gen_vect_espion();
                                    break;
                                }
                                case (3): {
                                    cartes_a2.clear();
                                    cartes_a2 = gen_vect_portebou();
                                    break;
                                }
                            }
                            break;
                        }
                        case (2): {
                            switch (tab_troupe_tac[i]) {
                                case (0): {
                                    cartes_a3.clear();
                                    cartes_a3.push_back(cartes_pose_j2[i]);
                                    break;
                                }
                                case (1): {
                                    cartes_a3.clear();
                                    cartes_a3 = gen_vect_joker();
                                    break;
                                }
                                case (2): {
                                    cartes_a3.clear();
                                    cartes_a3 = gen_vect_espion();
                                    break;
                                }
                                case (3): {
                                    cartes_a3.clear();
                                    cartes_a3 = gen_vect_portebou();
                                    break;
                                }
                            }
                            break;
                        }
                        case (3): {
                            switch (tab_troupe_tac[i]) {
                                case (0): {
                                    cartes_a4.clear();
                                    cartes_a4.push_back(cartes_pose_j2[i]);
                                    break;
                                }
                                case (1): {
                                    cartes_a4.clear();
                                    cartes_a4 = gen_vect_joker();
                                    break;
                                }
                                case (2): {
                                    cartes_a4.clear();
                                    cartes_a4 = gen_vect_espion();
                                    break;
                                }
                                case (3): {
                                    cartes_a4.clear();
                                    cartes_a4 = gen_vect_portebou();
                                    break;
                                }
                            }
                            break;
                        }
                    }
            }

            for (auto a1: cartes_a1) {
                for (auto a2: cartes_a2) {
                    for (auto a3: cartes_a3) {
                        for (auto a4: cartes_a4) {
                            if (CarteClan_egales(a1, a2) || CarteClan_egales(a1, a3) || CarteClan_egales(a1, a4) ||
                                CarteClan_egales(a2, a3) || CarteClan_egales(a2, a4) || CarteClan_egales(a3, a4))
                                continue;
                            Combinaison *combi_temp = new Combinaison();
                            combi_temp->setBoue(true);
                            combi_temp->ajouterCarte(a1);
                            combi_temp->ajouterCarte(a2);
                            combi_temp->ajouterCarte(a3);
                            combi_temp->ajouterCarte(a4);
                            if (qui_gagne(combi_j1, combi_temp) == 2) {
                                // J2 gagne
                                std::ostringstream oss;
                                oss << "ECHEC J2 peut gagner" << endl;
                                oss << a1->getPuissance() <<" "<< a1->getCouleur() << " "<< a2->getPuissance() <<" "<< a2->getCouleur() << " "<< a3->getPuissance() <<" "<< a3->getCouleur() << " "<< a4->getPuissance() <<" "<< a4->getCouleur() << endl;
                                delete combi_temp;
                                return oss.str();

                            }
                        }
                    }
                }
            }
            std::ostringstream oss;
            oss << "REUSSITE J2 ne peut pas gagner (J1 gagne 2)" << endl;
            borne->setRevendique(1);
            return oss.str();
        }
    }
    else {
        // J2 revendique
        cout << "J2 revendique la borne "<< num_borne << endl;
        if (humain) {
            if(il_y_a_joker(cartes_pose_j2) || il_y_a_espion(cartes_pose_j2) || il_y_a_portebou(cartes_pose_j2)){
                for (auto c: cartes_pose_j2) {
                    if (auto t = dynamic_cast<CarteTroupeElite *>(c)) {
                        auto nom = t->getNom();
                        if (nom == "Joker") {
                            t->jouer_Joker();
                            combi_j2->calculerForceCombi();
                            combi_j2->setTotalPuissance(combi_j2->getTotalPuissance() + static_cast<int>(t->getPuissance()));

                        } else if (nom == "Espion") {
                            t->jouer_Espion();
                            combi_j2->calculerForceCombi();
                            combi_j2->setTotalPuissance(combi_j2->getTotalPuissance() + static_cast<int>(t->getPuissance()));

                        } else if (nom == "Porte_Bouclier") {
                            t->jouer_PorteBouclier();
                            combi_j2->calculerForceCombi();
                            combi_j2->setTotalPuissance(combi_j2->getTotalPuissance() + static_cast<int>(t->getPuissance()));

                        }
                    }
                }

            }
        }
        else {
            if(il_y_a_joker(cartes_pose_j2) || il_y_a_espion(cartes_pose_j2) || il_y_a_portebou(cartes_pose_j2)){
                for (auto c: cartes_pose_j2) {
                    if (auto t = dynamic_cast<CarteTroupeElite *>(c)) {
                        auto nom = t->getNom();
                        if (nom == "Joker") {
                            t->jouer_JokerIA();
                            combi_j2->calculerForceCombi();
                            combi_j2->setTotalPuissance(combi_j2->getTotalPuissance() + static_cast<int>(t->getPuissance()));

                        } else if (nom == "Espion") {
                            t->jouer_EspionIA();
                            combi_j2->calculerForceCombi();
                            combi_j2->setTotalPuissance(combi_j2->getTotalPuissance() + static_cast<int>(t->getPuissance()));

                        } else if (nom == "Porte_Bouclier") {
                            t->jouer_PorteBouclierIA();
                            combi_j2->calculerForceCombi();
                            combi_j2->setTotalPuissance(combi_j2->getTotalPuissance() + static_cast<int>(t->getPuissance()));

                        }
                    }
                }

            }

        }
        int tab_troupe_tac[4] = {0, 0, 0, 0};
        if (il_y_a_joker(cartes_pose_j1) || il_y_a_portebou(cartes_pose_j1) || il_y_a_espion(cartes_pose_j1)) {
            size_t i = 0;
            for (auto c: cartes_pose_j1) {
                if (auto t = dynamic_cast<CarteTroupeElite *>(c)) {
                    auto nom = t->getNom();
                    if (nom == "Joker") {
                        tab_troupe_tac[i] = 1;
                    } else if (nom == "Espion") {
                        tab_troupe_tac[i] = 2;
                    } else if (nom == "Porte_Bouclier") {
                        tab_troupe_tac[i] = 3;
                    }
                }
                i++;
            }
        }
        if (combi_j2->getBoue() == 0) {
            vector<CarteClan *> cartes_a1 = m_carte_non_pose;
            vector<CarteClan *> cartes_a2 = m_carte_non_pose;
            vector<CarteClan *> cartes_a3 = m_carte_non_pose;
            if (cartes_pose_j1.size()) {
                for (auto i = 0; i < cartes_pose_j1.size(); i++)
                    switch (i) {
                        case (0): {
                            switch (tab_troupe_tac[i]) {
                                case (0): {
                                    cartes_a1.clear();
                                    cartes_a1.push_back(cartes_pose_j1[i]);
                                    break;
                                }
                                case (1): {
                                    cartes_a1.clear();
                                    cartes_a1 = gen_vect_joker();
                                    break;
                                }
                                case (2): {
                                    cartes_a1.clear();
                                    cartes_a1 = gen_vect_espion();
                                    break;
                                }
                                case (3): {
                                    cartes_a1.clear();
                                    cartes_a1 = gen_vect_portebou();
                                    break;
                                }
                            }
                            break;
                        }
                        case (1): {
                            switch (tab_troupe_tac[i]) {
                                case (0): {
                                    cartes_a2.clear();
                                    cartes_a2.push_back(cartes_pose_j1[i]);
                                    break;
                                }
                                case (1): {
                                    cartes_a2.clear();
                                    cartes_a2 = gen_vect_joker();
                                    break;
                                }
                                case (2): {
                                    cartes_a2.clear();
                                    cartes_a2 = gen_vect_espion();
                                    break;
                                }
                                case (3): {
                                    cartes_a2.clear();
                                    cartes_a2 = gen_vect_portebou();
                                    break;
                                }
                            }
                            break;
                        }
                        case (2): {
                            switch (tab_troupe_tac[i]) {
                                case (0): {
                                    cartes_a3.clear();
                                    cartes_a3.push_back(cartes_pose_j1[i]);
                                    break;
                                }
                                case (1): {
                                    cartes_a3.clear();
                                    cartes_a3 = gen_vect_joker();
                                    break;
                                }
                                case (2): {
                                    cartes_a3.clear();
                                    cartes_a3 = gen_vect_espion();
                                    break;
                                }
                                case (3): {
                                    cartes_a3.clear();
                                    cartes_a3 = gen_vect_portebou();
                                    break;
                                }
                            }
                            break;
                        }
                    }
            }
            // TODO temporaire
            /*cout << "cartes_a1" << endl;
            affichage_vecteur_carteclan(cartes_a1);
            cout << "cartes_a2" << endl;
            affichage_vecteur_carteclan(cartes_a2);
            cout << "cartes_a3" << endl;
            affichage_vecteur_carteclan(cartes_a3);*/

            for (auto a1: cartes_a1) {
                for (auto a2: cartes_a2) {
                    for (auto a3: cartes_a3) {
                        if (CarteClan_egales(a1, a2) || CarteClan_egales(a1, a3) ||
                            CarteClan_egales(a2, a3) )
                            continue;
                        Combinaison *combi_temp = new Combinaison();
                        combi_temp->ajouterCarte(a1);
                        combi_temp->ajouterCarte(a2);
                        combi_temp->ajouterCarte(a3);
                        if (qui_gagne(combi_j2, combi_temp) == 2) {
                            // J1 gagne
                            std::ostringstream oss;
                            oss << "\nECHEC J1 peut gagner: " << endl;
                            oss << a1->getPuissance() <<" "<< a1->getCouleur() << " "<< a2->getPuissance() <<" "<< a2->getCouleur() << " "<< a3->getPuissance() <<" "<< a3->getCouleur() << endl;
                            delete combi_temp;
                            return oss.str();
                        }
                    }
                }
            }
            std::ostringstream oss;
            oss << "REUSSITE J1 ne peut pas gagner (J2 gagne 1)" << endl;
            borne->setRevendique(2);
            return oss.str();
        }
        else {
            vector<CarteClan *> cartes_a1 = m_carte_non_pose;
            vector<CarteClan *> cartes_a2 = m_carte_non_pose;
            vector<CarteClan *> cartes_a3 = m_carte_non_pose;
            vector<CarteClan *> cartes_a4 = m_carte_non_pose;
            if (cartes_pose_j1.size()) {
                for (auto i = 0; i < cartes_pose_j1.size() ; i++)
                    switch (i) {
                        case (0): {
                            switch (tab_troupe_tac[i]) {
                                case (0): {
                                    cartes_a1.clear();
                                    cartes_a1.push_back(cartes_pose_j1[i]);
                                    break;
                                }
                                case (1): {
                                    cartes_a1.clear();
                                    cartes_a1 = gen_vect_joker();
                                    break;
                                }
                                case (2): {
                                    cartes_a1.clear();
                                    cartes_a1 = gen_vect_espion();
                                    break;
                                }
                                case (3): {
                                    cartes_a1.clear();
                                    cartes_a1 = gen_vect_portebou();
                                    break;
                                }
                            }
                            break;
                        }
                        case (1): {
                            switch (tab_troupe_tac[i]) {
                                case (0): {
                                    cartes_a2.clear();
                                    cartes_a2.push_back(cartes_pose_j1[i]);
                                    break;
                                }
                                case (1): {
                                    cartes_a2.clear();
                                    cartes_a2 = gen_vect_joker();
                                    break;
                                }
                                case (2): {
                                    cartes_a2.clear();
                                    cartes_a2 = gen_vect_espion();
                                    break;
                                }
                                case (3): {
                                    cartes_a2.clear();
                                    cartes_a2 = gen_vect_portebou();
                                    break;
                                }
                            }
                            break;
                        }
                        case (2): {
                            switch (tab_troupe_tac[i]) {
                                case (0): {
                                    cartes_a3.clear();
                                    cartes_a3.push_back(cartes_pose_j1[i]);
                                    break;
                                }
                                case (1): {
                                    cartes_a3.clear();
                                    cartes_a3 = gen_vect_joker();
                                    break;
                                }
                                case (2): {
                                    cartes_a3.clear();
                                    cartes_a3 = gen_vect_espion();
                                    break;
                                }
                                case (3): {
                                    cartes_a3.clear();
                                    cartes_a3 = gen_vect_portebou();
                                    break;
                                }
                            }
                            break;
                        }
                        case (3): {
                            switch (tab_troupe_tac[i]) {
                                case (0): {
                                    cartes_a4.clear();
                                    cartes_a4.push_back(cartes_pose_j1[i]);
                                    break;
                                }
                                case (1): {
                                    cartes_a4.clear();
                                    cartes_a4 = gen_vect_joker();
                                    break;
                                }
                                case (2): {
                                    cartes_a4.clear();
                                    cartes_a4 = gen_vect_espion();
                                    break;
                                }
                                case (3): {
                                    cartes_a4.clear();
                                    cartes_a4 = gen_vect_portebou();
                                    break;
                                }
                            }
                            break;
                        }
                    }
            }
            // TODO temporaire
            /*cout << "cartes_a1" << endl;
            affichage_vecteur_carteclan(cartes_a1);
            cout << "cartes_a2" << endl;
            affichage_vecteur_carteclan(cartes_a2);
            cout << "cartes_a3" << endl;
            affichage_vecteur_carteclan(cartes_a3);
            cout << "cartes_a4" << endl;
            affichage_vecteur_carteclan(cartes_a4);*/

            for (auto a1: cartes_a1) {
                for (auto a2: cartes_a2) {
                    for (auto a3: cartes_a3) {
                        for (auto a4: cartes_a4) {
                            if (CarteClan_egales(a1, a2) || CarteClan_egales(a1, a3) || CarteClan_egales(a1, a4) ||
                                CarteClan_egales(a2, a3) || CarteClan_egales(a2, a4) || CarteClan_egales(a3, a4))
                                continue;
                            Combinaison *combi_temp = new Combinaison();
                            combi_temp->setBoue(true);
                            combi_temp->ajouterCarte(a1);
                            combi_temp->ajouterCarte(a2);
                            combi_temp->ajouterCarte(a3);
                            combi_temp->ajouterCarte(a4);
                            if (qui_gagne(combi_j2, combi_temp) == 2) {
                                // J1 gagne
                                std::ostringstream oss;
                                oss << "ECHEC J1 peut gagner" << endl;
                                oss << a1->getPuissance() <<" "<< a1->getCouleur() << " "<< a2->getPuissance() <<" "<< a2->getCouleur()  << " "<< a3->getPuissance() <<" "<< a3->getCouleur() << " "<< a4->getPuissance() <<" "<< a4->getCouleur() << endl;
                                delete combi_temp;
                                return oss.str();
                            }
                        }
                    }
                }
            }
            std::ostringstream oss;
            oss << "REUSSITE J1 ne peut pas gagner (J2 gagne 2)" << endl;
            borne->setRevendique(2);
            return oss.str();
        }
    }
}

void Controleur::fin_de_partie(){

    delete m_plateau->m_joueur1;
    delete m_plateau->m_joueur2;


    // Libérer la mémoire des objets membres des différentes classes

    // Vider et supprimer les conteneurs de pointeurs
    vector<Carte*> cartesMain1 = m_plateau->m_joueur1->getMain()->getCartes();
    for (int i = 0; i < cartesMain1.size(); i++) {
        delete cartesMain1[i];
    }
    cartesMain1.clear();


    vector<Carte*> cartesMain2 = m_plateau->m_joueur2->getMain()->getCartes();
    for (int i = 0; i < cartesMain2.size(); i++) {
        delete cartesMain2[i];
    }
    cartesMain2.clear();
    delete[] m_plateau->m_joueur1->getMain();
    delete[] m_plateau->m_joueur2->getMain();
    for (int i=0;i<9;i++){
        delete m_plateau->m_bornes[i];
    }
    delete m_plateau;
}

void Controleur::debut_de_partie_classique() {
    // Ajout des 6 cartes dans la main du joueur 1
    for (int i = 0; i < 6; i++) {
        CarteClan *ci = new CarteClan(getPiocheClan()->piocherCarteClan());
        m_plateau->m_joueur1->getMain()->ajouterCarte(ci);
    }

    // Ajout des 6 cartes dans la main du joueur 2
    for (int i = 0; i < 6; i++) {
        CarteClan *ci = new CarteClan(getPiocheClan()->piocherCarteClan());
        m_plateau->m_joueur2->getMain()->ajouterCarte(ci);
    }

    // Affichage des mains des joueurs
    m_plateau->afficherMainJoueur(1);
    m_plateau->afficherMainJoueur(2);
    cout << "Fin de la phase de pioche" << endl<<endl;
}

void Controleur::debut_de_partie_tactique() {
    // Ajout des 7 cartes dans la main du joueur 1
    for (int i = 0; i < 7; i++) {
        CarteClan *ci = new CarteClan(getPiocheClan()->piocherCarteClan());
        m_plateau->m_joueur1->getMain()->ajouterCarte(ci);
    }

    // Ajout des 6 cartes dans la main du joueur 2
    for (int i = 0; i < 7; i++) {
        CarteClan *ci = new CarteClan(getPiocheClan()->piocherCarteClan());
        m_plateau->m_joueur2->getMain()->ajouterCarte(ci);
    }

    // Affichage des mains des joueurs
    cout << "Main du joueur 1 :" << endl;
    affichage_vecteur_carte(m_plateau->m_joueur1->getMain()->getCartes());
    cout << "Main du joueur 2 :" << endl;
    affichage_vecteur_carte(m_plateau->m_joueur2->getMain()->getCartes());
    cout << "Fin de la phase de pioche" << endl<<endl;
}

void Controleur::supprimer_carte_pose_v1(CarteClan *carte) {
    for (int i = 0; i < m_carte_non_pose.size(); i++) {
        if (m_carte_non_pose[i] == carte) {
            m_carte_non_pose.erase(m_carte_non_pose.begin() + i);
        }
    }
}
void Controleur::supprimer_carte_pose_v2(CarteClan *carte) {
    auto p = carte->getPuissance();
    auto c = carte->getCouleur();
    for (int i = 0; i < m_carte_non_pose.size(); i++) {
        if (m_carte_non_pose[i]->getPuissance() == p && m_carte_non_pose[i]->getCouleur() == c) {
            m_carte_non_pose.erase(m_carte_non_pose.begin() + i);
        }
    }
}


// Joker tt les cartes
vector<CarteClan *> gen_vect_joker(){
    vector<CarteClan *> res;
    for (auto c : Couleurs)
        for (auto p : Puissances)
            res.push_back( new CarteClan(p, c));
    return res;

}

//Espion 7 de nimporte quel couleur
vector<CarteClan *> gen_vect_espion(){
    vector<CarteClan *> res;
    for (auto c : Couleurs)
        res.push_back( new CarteClan(Puissance::sept, c));
    return res;

}
// Porte bou
vector<CarteClan *> gen_vect_portebou(){
    vector<CarteClan *> res;
    for (auto c : Couleurs){
        res.push_back( new CarteClan(Puissance::un, c));
        res.push_back( new CarteClan(Puissance::deux, c));
        res.push_back( new CarteClan(Puissance::trois, c));
    }
    return res;

}

bool il_y_a_joker(vector<CarteClan *> v){
    for (auto c:v){
        if(auto carte =  dynamic_cast<CarteTroupeElite *>(c)){
            if (carte->getNom()=="Joker")
                return true;
        }
    }
    return false;
}

bool il_y_a_espion(vector<CarteClan *> v){
    for (auto c:v){
        if(auto carte =  dynamic_cast<CarteTroupeElite *>(c)){
            if (carte->getNom()=="Espion")
                return true;
        }
    }
    return false;
}
bool il_y_a_portebou(vector<CarteClan *> v){
    for (auto c:v){
        if(auto carte =  dynamic_cast<CarteTroupeElite *>(c)){
            if (carte->getNom()=="Porte_Bouclier")
                return true;
        }
    }
    return false;
}

int qui_gagne(Combinaison * combi1, Combinaison * combi2){
    if (combi1->getForceCombi()>combi2->getForceCombi()) {
        //cout << "combi1 gagne car force combi" << endl;
        return 1;
    }
    else if (combi1->getForceCombi()<combi2->getForceCombi()) {
        //cout << "combi2 gagne car force combi" << endl;
        return 2;
    }
    else if (combi1->getTotalPuissance()>combi2->getTotalPuissance()) {
        //cout << "combi1 gagne car total puissance" << endl;
        return 1;
    }
    else if (combi1->getTotalPuissance()<combi2->getTotalPuissance()) {
        //cout << "combi2 gagne car total puissance" << endl;
        return 2;
    }
    else {
        //cout << "combi gagne car dernier a poser" << endl;
        return combi1->getPremierComplet();
    }

}

int CarteClan_egales(CarteClan* c1, CarteClan* c2){
    if (c1->getPuissance()==c2->getPuissance() && c1->getCouleur()==c2->getCouleur())
        return 1;
    else
        return 0;
}



