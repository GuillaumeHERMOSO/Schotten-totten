//
// Created by Grzadziel Bartlomiej on 24/04/2023.
//

#include "Combinaison.h"
#include "Controleur.h"

int Combinaison::calculerForceCombi()
{
    auto nbr_carte = m_cartes.size();
    Couleur tab_coul[4];
    int tab_puis[4];
    for (auto i = 0; i < nbr_carte; i++) {
        tab_coul[i] = m_cartes[i]->getCouleur();
        tab_puis[i] = static_cast<int>(m_cartes[i]->getPuissance());
    }
    // on trie les puissance:
    for (auto i = 0; i < nbr_carte; i++) {
        for (auto j = 0; j < nbr_carte; j++) {
            if (tab_puis[i] < tab_puis[j]) {
                auto temp = tab_puis[i];
                tab_puis[i] = tab_puis[j];
                tab_puis[j] = temp;
            }
        }
    }


    if (nbr_carte == 3) {
        if (tab_coul[0] == tab_coul[1] && tab_coul[1] == tab_coul[2] && tab_puis[0] + 1 == tab_puis[1] && tab_puis[1] + 1 == tab_puis[2]) {
            m_force_combi = 4;
            return 4;
        }
        if (tab_puis[0] == tab_puis[1] && tab_puis[1] == tab_puis[2]) {
            m_force_combi = 3;
            return 3;
        }
        if (tab_coul[0] == tab_coul[1] && tab_coul[1] == tab_coul[2]) {
            m_force_combi = 2;
            return 2;
        }
        if (tab_puis[0] + 1 == tab_puis[1] && tab_puis[1] + 1 == tab_puis[2]) {
            m_force_combi = 1;
            return 1;
        }

    }
    else if (nbr_carte == 4) {
        if (tab_coul[0] == tab_coul[1] && tab_coul[1] == tab_coul[2] && tab_coul[2] == tab_coul[3] && tab_puis[0] + 1 == tab_puis[1] && tab_puis[1] + 1 == tab_puis[2] && tab_puis[2] + 1 == tab_puis[3]){
            m_force_combi = 4;
            return 4;
        }
        if (tab_puis[0] == tab_puis[1] && tab_puis[1] == tab_puis[2] && tab_puis[2] == tab_puis[3]){
            m_force_combi = 3;
            return 3;
        }
        if (tab_coul[0] == tab_coul[1] && tab_coul[1] == tab_coul[2] && tab_coul[2] == tab_coul[3]){
            m_force_combi = 2;
            return 2;
        }
        if (tab_puis[0] + 1 == tab_puis[1] && tab_puis[1] + 1 == tab_puis[2] && tab_puis[2] + 1 == tab_puis[3]){
            m_force_combi = 1;
            return 1;
        }


    }
    m_force_combi = 0;
    return 0;
}

