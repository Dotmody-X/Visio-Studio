# VISIO STUDIO

## Présentation

VISIO STUDIO est un logiciel modulaire de création graphique inspiré par l'interface et le workflow des Digital Audio Workstations (comme FL Studio), mais dédié aux designers visuels dans l'univers électronique. Il combine les fonctionnalités avancées d'outils comme Photoshop, Illustrator et InDesign avec une approche modulaire par plugins et une chaîne d'effets innovante.

Vision : Offrir un outil créatif qui amplifie la créativité des artistes visuels avec un workflow fluide et inspirant, particulièrement adapté aux esthétiques de la scène électronique (glitch, néon, cyberpunk, minimalisme).

### Fonctionnalités principales

    Chaîne d'effets modulaire : Appliquez, réorganisez et ajustez des effets en temps réel comme dans un DAW audio
    Système de plugins : Extensible via un format standardisé permettant d'ajouter de nouveaux effets et fonctionnalités
    Édition bitmap & vectorielle : Combinez les deux approches dans un même environnement de travail
    Intelligence artificielle intégrée : Génération d'images et d'éléments graphiques, assistant créatif, suggestions d'effets
    Interface inspirée des DAW : Organisation des effets en "chaîne de mixage" avec contrôles paramétriques précis
    Effets spécialisés : Collection d'effets orientés vers l'esthétique électronique (dithering, glitch, VHS, néon...)
    Format de projet non-destructif : Préservation de tous les calques et paramètres modifiables

## Installation
### Prérequis

    OS compatibles : Windows 10/11, macOS 10.15+, Linux (Ubuntu 20.04+)
    Configuration minimale :
        8 Go RAM
        Processeur dual-core récent
        Carte graphique avec support OpenGL 3.3+
        500 Mo d'espace disque (+ espace pour vos projets)

### Installation depuis les binaires

    Téléchargez la dernière version stable depuis la page des releases
    Exécutez l'installateur et suivez les instructions
    Au premier lancement, un assistant vous guidera dans la configuration

Compilation depuis les sources

bash

### Cloner le dépôt
git clone https://github.com/visio-studio/visio-studio.git
cd visio-studio

### Installer les dépendances (exemple pour Ubuntu)
sudo apt-get install build-essential libgl1-mesa-dev cmake qt6-base-dev

### Configurer et compiler
mkdir build && cd build
cmake ..
make -j4

### Lancer l'application
./visio-studio

##  Guide du développeur
Structure du projet

    visio-studio/
    │
    ├── CMakeLists.txt                  # Fichier de build principal
    ├── README.md                       # Documentation principale
    │
    ├── src/                            # Code source principal
    │   ├── core/                       # Moteur central
    │   ├── plugins/                    # Système de plugins
    │   ├── effects/                    # Effets visuels
    │   ├── ai/                         # Composants IA
    │   ├── ui/                         # Interface utilisateur
    │   └── utils/                      # Utilitaires
    │
    ├── include/                        # En-têtes publics
    │   └── visio/                      # API publique
    │
    ├── plugins/                        # Plugins intégrés
    │   ├── glitch/                     # Plugin Glitch
    │   ├── dithering/                  # Plugin Dithering
    │   └── ai_generator/               # Plugin Générateur IA
    │
    ├── resources/                      # Ressources
    ├── docs/                           # Documentation
    └── tests/                          # Tests unitaires

## Création d'un plugin

Les plugins utilisent une API standardisée pour s'intégrer à VISIO STUDIO. Voici un exemple minimal :

cpp

#include "plugins/plugin_interface.h"

namespace VISIO {

class MyPlugin : public QObject, public PluginInterface {
    Q_OBJECT
    Q_INTERFACES(VISIO::PluginInterface)
    Q_PLUGIN_METADATA(IID PluginInterface_iid)

public:
    QString name() const override { return "My Plugin"; }
    QString version() const override { return "1.0"; }
    QString description() const override { return "Description"; }

    QWidget* createUi() override;
    QImage processImage(const QImage &inputImage) override;
    QByteArray saveState() const override;
    bool loadState(const QByteArray &state) override;
};

} // namespace VISIO

Pour plus de détails, consultez la documentation des plugins.

## Interface utilisateur

L'interface de VISIO STUDIO s'organise autour de plusieurs zones principales :

    Zone de travail centrale : Affichage et manipulation de l'image en cours
    Chaîne d'effets (mixer) : Organisation et paramétrage des effets appliqués
    Navigateur de plugins : Accès aux effets disponibles et préréglages
    Inspecteur : Propriétés de l'élément sélectionné
    Gestionnaire de calques : Organisation des éléments du projet

L'interface utilise une palette sombre avec des accents colorés inspirés de l'esthétique néon/cyberpunk, optimisée pour le travail en environnement à faible luminosité.

## Fonctionnalités d'intelligence artificielle

VISIO STUDIO intègre plusieurs technologies d'IA pour faciliter et enrichir le processus créatif :

    Génération d'images : Création d'éléments visuels à partir de descriptions textuelles
    Assistant créatif : Interface conversationnelle pour guider l'utilisation et suggérer des techniques
    Mise en page intelligente : Organisation automatique des éléments selon les principes de design
    Analyse visuelle : Évaluation de la composition et suggestions d'améliorations

Ces fonctionnalités sont conçues pour amplifier la créativité humaine, non la remplacer. Consultez la documentation des fonctionnalités IA pour plus de détails.

## Feuille de route
### Phase 1: Fondations (MVP)

    Interface principale et gestion de projet
    Système de plugins de base
    Premiers effets essentiels (Grayscale, Threshold, Glitch)
    Fonctionnalités d'édition bitmap basiques

### Phase 2: Extension

    Système de calques avancé
    Chaîne d'effets améliorée
    Édition vectorielle de base
    Plugins additionnels

### Phase 3: Intelligence

    Intégration des premières fonctionnalités IA
    Assistant créatif
    Génération d'images basique
    Bibliothèque de styles

### Phase 4: Écosystème

    Marketplace de plugins
    Partage communautaire
    IA avancée
    Intégration avec d'autres outils

---

**Rapporter un bug ou suggérer une fonctionnalité**

**Utilisez l'onglet "Issues" sur notre dépôt GitHub pour signaler des problèmes ou proposer des améliorations.**

## Équipe et crédits

    Développement principal : Nathan Gillet
    Design UI/UX : Nathan Gillet
    Effets : Nathan Gillet

**VISIO STUDIO est un projet scholaire et n'est pas aboutit.**
