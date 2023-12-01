//
// Created by Utilisateur on 19/04/2023.
//

#ifndef LO21_SCHOTTEN_TOTTEN_CARTETACTIQUE_H
#define LO21_SCHOTTEN_TOTTEN_CARTETACTIQUE_H
#include"Carte.h"


class CarteTactique : public Carte {
    const string m_nom;
public:
    CarteTactique(const string& nom): m_nom(nom) {};
    const string &getNom() const { return m_nom; }
    void afficherCarte()const override{
        if(getNom()=="Chasseur de Tete"){
            cout<< "| Chas Tete |";
        }
        else if(getNom()=="Stratege"){
            cout<< "| Stratege |";
        }
        else if(getNom()=="Banshee"){
            cout<< "|  Banshee |";
        }
        else if(getNom()=="Traitre"){
            cout<< "|  Traitre |";
        }
        else if(getNom()=="Colin Maillard"){
            cout<< "| Col Mail |";
        }
        else if(getNom()=="Combat de Boue"){
            cout<< "| Cbt Boue |";
        }

    }
    void afficherDosCarte() const override {
        cout << "| Tactique |";
    }
};

#endif //LO21_SCHOTTEN_TOTTEN_CARTETACTIQUE_H
