/*
 * AEROGEST — Gestion de compagnie aérienne
 * Améliorations : C++ idiomatique, vectors dynamiques, validation des entrées,
 *                 gestion des noms avec espaces, menus enrichis, couleurs ANSI.
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
#include <climits>
#include <sstream>

// ─── Couleurs ANSI ────────────────────────────────────────────────────────────
namespace Color {
    const std::string RESET   = "\033[0m";
    const std::string BOLD    = "\033[1m";
    const std::string DIM     = "\033[2m";
    const std::string AMBER   = "\033[33m";
    const std::string GREEN   = "\033[32m";
    const std::string RED     = "\033[31m";
    const std::string CYAN    = "\033[36m";
    const std::string WHITE   = "\033[97m";
    const std::string B_AMBER = "\033[1;33m";
    const std::string B_GREEN = "\033[1;32m";
    const std::string B_RED   = "\033[1;31m";
    const std::string B_CYAN  = "\033[1;36m";
}

// ─── Structures ───────────────────────────────────────────────────────────────
struct Vol {
    int    numero;
    std::string depart;
    std::string destination;
    int    placesDisponibles;
    double prix;
};

struct Passager {
    int    id;
    std::string nom;
    int    numeroVol;
};

// ─── Données globales ─────────────────────────────────────────────────────────
std::vector<Vol>      vols;
std::vector<Passager> passagers;

// ─── Utilitaires ──────────────────────────────────────────────────────────────

// Lit une ligne entière proprement (ignore le \n résiduel)
std::string lireLigne(const std::string& invite) {
    std::string ligne;
    std::cout << invite;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, ligne);
    // Supprimer les espaces en début/fin
    size_t debut = ligne.find_first_not_of(" \t");
    if (debut == std::string::npos) return "";
    size_t fin = ligne.find_last_not_of(" \t");
    return ligne.substr(debut, fin - debut + 1);
}

// Lit un entier avec validation
int lireEntier(const std::string& invite, int min = 0, int max = INT_MAX) {
    int valeur;
    while (true) {
        std::cout << invite;
        if (std::cin >> valeur && valeur >= min && valeur <= max) {
            return valeur;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << Color::B_RED << "  ✗ Entrée invalide. Entrez un nombre entre "
                  << min << " et " << max << ".\n" << Color::RESET;
    }
}

// Lit un double avec validation
double lireDouble(const std::string& invite, double min = 0.0) {
    double valeur;
    while (true) {
        std::cout << invite;
        if (std::cin >> valeur && valeur >= min) {
            return valeur;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << Color::B_RED << "  ✗ Entrée invalide. Entrez un nombre >= "
                  << min << ".\n" << Color::RESET;
    }
}

// Cherche un vol par numéro, retourne un pointeur ou nullptr
Vol* trouverVol(int numero) {
    for (Vol& v : vols)
        if (v.numero == numero) return &v;
    return nullptr;
}

// Ligne de séparation stylée
void separateur(char c = '-', int n = 52) {
    std::cout << Color::DIM;
    for (int i = 0; i < n; ++i) std::cout << c;
    std::cout << Color::RESET << "\n";
}

// Formate un prix
std::string formatPrix(double p) {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2) << p << " €";
    return ss.str();
}

// ─── Affichage en-tête ────────────────────────────────────────────────────────
void afficherEntete() {
    std::cout << "\n" << Color::B_AMBER;
    separateur('=');
    std::cout << "  ✈  AEROGEST  ·  Gestion de compagnie aérienne\n";
    separateur('=');
    std::cout << Color::RESET;
}

// ─── Menu principal ───────────────────────────────────────────────────────────
void afficherMenu() {
    std::cout << "\n" << Color::AMBER;
    separateur();
    std::cout << Color::RESET;
    std::cout << Color::BOLD << "  Que voulez-vous faire ?\n" << Color::RESET;
    std::cout << Color::CYAN  << "  [1]" << Color::RESET << " Ajouter un vol\n";
    std::cout << Color::CYAN  << "  [2]" << Color::RESET << " Afficher tous les vols\n";
    std::cout << Color::CYAN  << "  [3]" << Color::RESET << " Rechercher un vol\n";
    std::cout << Color::CYAN  << "  [4]" << Color::RESET << " Réserver un billet\n";
    std::cout << Color::CYAN  << "  [5]" << Color::RESET << " Afficher les passagers\n";
    std::cout << Color::CYAN  << "  [6]" << Color::RESET << " Statistiques\n";
    std::cout << Color::RED   << "  [0]" << Color::RESET << " Quitter\n";
    std::cout << Color::AMBER;
    separateur();
    std::cout << Color::RESET;
}

// ─── Fonctions principales ────────────────────────────────────────────────────

void ajouterVol() {
    std::cout << "\n" << Color::B_CYAN << "  ╔═ Nouveau vol ═╗\n" << Color::RESET;

    int numero = lireEntier("  Numéro de vol        : ", 1);
    if (trouverVol(numero)) {
        std::cout << Color::B_RED << "  ✗ Le vol " << numero << " existe déjà.\n" << Color::RESET;
        return;
    }

    std::string depart      = lireLigne("  Ville de départ      : ");
    std::string destination = lireLigne("  Destination          : ");
    int places              = lireEntier("  Places disponibles   : ", 1, 999);
    double prix             = lireDouble("  Prix du billet (€)   : ", 0.01);

    vols.push_back({ numero, depart, destination, places, prix });

    std::cout << Color::B_GREEN << "\n  ✓ Vol " << numero
              << " (" << depart << " → " << destination << ") ajouté.\n"
              << Color::RESET;
}

// ─────────────────────────────────────────────────────────────────────────────
void afficherVols() {
    std::cout << "\n" << Color::B_CYAN << "  ╔═ Liste des vols ═╗\n" << Color::RESET;

    if (vols.empty()) {
        std::cout << Color::DIM << "  Aucun vol enregistré.\n" << Color::RESET;
        return;
    }

    // En-tête tableau
    std::cout << Color::BOLD;
    std::cout << std::left
              << "  " << std::setw(6)  << "N° Vol"
              << std::setw(18) << "Départ"
              << std::setw(18) << "Destination"
              << std::setw(8)  << "Places"
              << std::setw(12) << "Prix"
              << "Statut\n";
    std::cout << Color::RESET;
    separateur();

    for (const Vol& v : vols) {
        bool complet = (v.placesDisponibles == 0);
        std::string statut = complet
            ? Color::B_RED   + std::string("COMPLET") + Color::RESET
            : Color::B_GREEN + std::string("DISPO  ") + Color::RESET;

        std::cout << "  "
                  << Color::AMBER << std::setw(6)  << v.numero << Color::RESET
                  << std::setw(18) << v.depart
                  << std::setw(18) << v.destination
                  << std::setw(8)  << v.placesDisponibles
                  << std::setw(12) << formatPrix(v.prix)
                  << statut << "\n";
    }
    separateur();
    std::cout << Color::DIM << "  " << vols.size() << " vol(s) au total.\n" << Color::RESET;
}

// ─────────────────────────────────────────────────────────────────────────────
void rechercherVol() {
    int numero = lireEntier("\n  Numéro du vol à rechercher : ", 1);
    const Vol* v = trouverVol(numero);

    if (!v) {
        std::cout << Color::B_RED << "  ✗ Vol " << numero << " introuvable.\n" << Color::RESET;
        return;
    }

    std::cout << "\n" << Color::B_CYAN << "  ╔═ Vol " << v->numero << " ═╗\n" << Color::RESET;
    separateur();
    std::cout << "  Départ      : " << Color::WHITE << v->depart      << Color::RESET << "\n";
    std::cout << "  Destination : " << Color::WHITE << v->destination  << Color::RESET << "\n";
    std::cout << "  Prix        : " << Color::AMBER << formatPrix(v->prix) << Color::RESET << "\n";

    if (v->placesDisponibles > 0)
        std::cout << "  Places      : " << Color::B_GREEN << v->placesDisponibles << " disponible(s)\n" << Color::RESET;
    else
        std::cout << "  Places      : " << Color::B_RED << "COMPLET\n" << Color::RESET;

    separateur();
}

// ─────────────────────────────────────────────────────────────────────────────
void reserverBillet() {
    if (vols.empty()) {
        std::cout << Color::B_RED << "\n  ✗ Aucun vol disponible.\n" << Color::RESET;
        return;
    }

    // Afficher les vols disponibles
    std::cout << "\n" << Color::B_CYAN << "  ╔═ Réservation ═╗\n" << Color::RESET;
    std::cout << Color::DIM << "  Vols avec places disponibles :\n" << Color::RESET;
    bool auMoinsUn = false;
    for (const Vol& v : vols) {
        if (v.placesDisponibles > 0) {
            std::cout << Color::AMBER << "    Vol " << v.numero << Color::RESET
                      << " · " << v.depart << " → " << v.destination
                      << " · " << v.placesDisponibles << " place(s) · "
                      << formatPrix(v.prix) << "\n";
            auMoinsUn = true;
        }
    }
    if (!auMoinsUn) {
        std::cout << Color::B_RED << "  ✗ Tous les vols sont complets.\n" << Color::RESET;
        return;
    }

    int numero = lireEntier("\n  Numéro du vol choisi : ", 1);
    Vol* v = trouverVol(numero);

    if (!v) {
        std::cout << Color::B_RED << "  ✗ Vol " << numero << " introuvable.\n" << Color::RESET;
        return;
    }
    if (v->placesDisponibles <= 0) {
        std::cout << Color::B_RED << "  ✗ Le vol " << numero << " est complet.\n" << Color::RESET;
        return;
    }

    std::string nom = lireLigne("  Nom complet du passager : ");
    if (nom.empty()) {
        std::cout << Color::B_RED << "  ✗ Le nom ne peut pas être vide.\n" << Color::RESET;
        return;
    }

    int newId = static_cast<int>(passagers.size()) + 1;
    passagers.push_back({ newId, nom, numero });
    v->placesDisponibles--;

    std::cout << Color::B_GREEN << "\n  ✓ Billet réservé !\n" << Color::RESET;
    separateur();
    std::cout << "  Passager    : " << Color::WHITE << nom        << Color::RESET << "\n";
    std::cout << "  ID          : " << Color::AMBER << "#" << std::setfill('0') << std::setw(3) << newId << Color::RESET << "\n";
    std::cout << std::setfill(' ');
    std::cout << "  Vol         : " << Color::WHITE << numero     << Color::RESET << "\n";
    std::cout << "  Trajet      : " << v->depart << " → " << v->destination << "\n";
    std::cout << "  Prix        : " << Color::AMBER << formatPrix(v->prix) << Color::RESET << "\n";
    std::cout << "  Places rest.: " << v->placesDisponibles << "\n";
    separateur();
}

// ─────────────────────────────────────────────────────────────────────────────
void afficherPassagers() {
    std::cout << "\n" << Color::B_CYAN << "  ╔═ Liste des passagers ═╗\n" << Color::RESET;

    if (passagers.empty()) {
        std::cout << Color::DIM << "  Aucun passager enregistré.\n" << Color::RESET;
        return;
    }

    std::cout << Color::BOLD;
    std::cout << std::left
              << "  " << std::setw(6)  << "ID"
              << std::setw(25) << "Nom"
              << std::setw(8)  << "N° Vol"
              << "Trajet\n";
    std::cout << Color::RESET;
    separateur();

    for (const Passager& p : passagers) {
        const Vol* v = trouverVol(p.numeroVol);
        std::string trajet = v ? (v->depart + " → " + v->destination) : "—";

        std::cout << "  "
                  << Color::AMBER << "#" << std::setfill('0') << std::setw(3) << p.id
                  << Color::RESET << std::setfill(' ') << "  "
                  << std::setw(25) << p.nom
                  << std::setw(8)  << p.numeroVol
                  << trajet << "\n";
    }
    separateur();
    std::cout << Color::DIM << "  " << passagers.size() << " passager(s) au total.\n" << Color::RESET;
}

// ─────────────────────────────────────────────────────────────────────────────
void afficherStatistiques() {
    std::cout << "\n" << Color::B_CYAN << "  ╔═ Statistiques ═╗\n" << Color::RESET;
    separateur();

    int totalPlaces = 0, volsComplets = 0;
    double revenu = 0.0;

    for (const Vol& v : vols) {
        totalPlaces += v.placesDisponibles;
        if (v.placesDisponibles == 0) volsComplets++;
    }
    for (const Passager& p : passagers) {
        const Vol* v = trouverVol(p.numeroVol);
        if (v) revenu += v->prix;
    }

    std::cout << "  Vols enregistrés     : " << Color::AMBER << vols.size() << Color::RESET << "\n";
    std::cout << "  Vols complets        : " << Color::B_RED << volsComplets << Color::RESET << "\n";
    std::cout << "  Places encore dispos : " << Color::B_GREEN << totalPlaces << Color::RESET << "\n";
    std::cout << "  Passagers enregistrés: " << Color::AMBER << passagers.size() << Color::RESET << "\n";
    std::cout << "  Revenu total estimé  : " << Color::B_AMBER << formatPrix(revenu) << Color::RESET << "\n";

    // Vol le plus populaire
    if (!passagers.empty()) {
        std::vector<std::pair<int,int>> compteur; // {numero, count}
        for (const Vol& v : vols) {
            int cnt = 0;
            for (const Passager& p : passagers)
                if (p.numeroVol == v.numero) cnt++;
            compteur.push_back({v.numero, cnt});
        }
        auto best = std::max_element(compteur.begin(), compteur.end(),
            [](const auto& a, const auto& b){ return a.second < b.second; });
        if (best != compteur.end() && best->second > 0)
            std::cout << "  Vol le plus demandé  : " << Color::B_AMBER << "Vol " << best->first
                      << " (" << best->second << " réservation(s))" << Color::RESET << "\n";
    }
    separateur();
}

// ─── Main ─────────────────────────────────────────────────────────────────────
int main() {
    // Données de démonstration
    vols.push_back({ 204, "Nouakchott", "Dakar",       4,  95.00 });
    vols.push_back({ 318, "Casablanca", "Paris",        0, 310.50 });
    vols.push_back({ 552, "Nouakchott", "Casablanca",  27, 140.00 });

    afficherEntete();

    int choix = -1;
    do {
        afficherMenu();
        choix = lireEntier("  Votre choix : ", 0, 6);

        switch (choix) {
            case 1: ajouterVol();          break;
            case 2: afficherVols();        break;
            case 3: rechercherVol();       break;
            case 4: reserverBillet();      break;
            case 5: afficherPassagers();   break;
            case 6: afficherStatistiques();break;
            case 0:
                std::cout << Color::B_AMBER
                          << "\n  Merci d'avoir utilisé AEROGEST. À bientôt !\n\n"
                          << Color::RESET;
                break;
        }
    } while (choix != 0);

    return 0;
}
