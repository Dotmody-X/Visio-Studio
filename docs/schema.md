visio-studio/
│
├── CMakeLists.txt                  # Fichier de build principal
├── README.md                       # Documentation principale
│
├── src/                            # Code source principal
│   ├── core/                       # Moteur central
│   │   ├── core_engine.h           # Définition du moteur central
│   │   ├── core_engine.cpp         # Implémentation du moteur central
│   │   ├── project_manager.h       # Gestionnaire de projets
│   │   ├── project_manager.cpp     # Implémentation du gestionnaire de projets
│   │   ├── resource_manager.h      # Gestionnaire de ressources
│   │   └── resource_manager.cpp    # Implémentation du gestionnaire de ressources
│   │
│   ├── plugins/                    # Système de plugins
│   │   ├── plugin_interface.h      # Interface standardisée pour tous les plugins
│   │   ├── plugin_interface.cpp    # Implémentation de base pour l'interface des plugins
│   │   ├── plugin_manager.h        # Gestionnaire des plugins
│   │   └── plugin_manager.cpp      # Implémentation du gestionnaire des plugins
│   │
│   ├── effects/                    # Effets visuels
│   │   ├── effect_chain.h          # Système de chaîne d'effets
│   │   ├── effect_chain.cpp        # Implémentation de la chaîne d'effets
│   │   ├── glitch_effect.h         # Effet Glitch
│   │   ├── glitch_effect.cpp       # Implémentation de l'effet Glitch
│   │   ├── dithering_effect.h      # Effet Dithering
│   │   └── dithering_effect.cpp    # Implémentation de l'effet Dithering
│   │
│   ├── ai/                         # Composants IA
│   │   ├── ai_image_generator.h    # Générateur d'images par IA
│   │   ├── ai_image_generator.cpp  # Implémentation du générateur d'images
│   │   ├── model_connector.h       # Connecteurs vers différents modèles IA
│   │   └── model_connector.cpp     # Implémentation des connecteurs de modèles
│   │
│   ├── ui/                         # Interface utilisateur
│   │   ├── main_window.h           # Fenêtre principale
│   │   ├── main_window.cpp         # Implémentation de la fenêtre principale
│   │   ├── canvas_view.h           # Vue de canevas pour l'édition
│   │   ├── canvas_view.cpp         # Implémentation de la vue de canevas
│   │   ├── effect_chain_widget.h   # Widget de chaîne d'effets
│   │   ├── effect_chain_widget.cpp # Implémentation du widget de chaîne d'effets
│   │   ├── plugin_ui/              # Interfaces des plugins
│   │   │   ├── glitch_ui.h         # Interface pour l'effet Glitch
│   │   │   ├── glitch_ui.cpp       # Implémentation de l'interface Glitch
│   │   │   ├── dithering_ui.h      # Interface pour l'effet Dithering
│   │   │   ├── dithering_ui.cpp    # Implémentation de l'interface Dithering
│   │   │   ├── ai_generator_ui.h   # Interface pour le générateur IA
│   │   │   └── ai_generator_ui.cpp # Implémentation de l'interface du générateur IA
│   │   └── styles/                 # Feuilles de style et thèmes
│   │       ├── dark_theme.qss      # Thème sombre principal
│   │       └── plugin_styles.qss   # Styles spécifiques aux plugins
│   │
│   ├── utils/                      # Utilitaires
│   │   ├── image_buffer.h          # Structure de tampon d'image
│   │   ├── image_buffer.cpp        # Implémentation du tampon d'image
│   │   ├── file_format.h           # Format de fichier .vsd
│   │   └── file_format.cpp         # Implémentation du format de fichier
│   │
│   └── main.cpp                    # Point d'entrée du programme
│
├── include/                        # En-têtes publics
│   └── visio/                      # Namespace VISIO
│       ├── plugin_api.h            # API publique pour les plugins tiers
│       └── visio_types.h           # Types de données publics
│
├── plugins/                        # Plugins intégrés
│   ├── glitch/                     # Plugin Glitch (compilé séparément)
│   │   ├── CMakeLists.txt          # Build du plugin
│   │   ├── glitch_plugin.h         # Définition du plugin
│   │   └── glitch_plugin.cpp       # Implémentation du plugin
│   │
│   ├── dithering/                  # Plugin Dithering
│   │   ├── CMakeLists.txt          # Build du plugin
│   │   ├── dithering_plugin.h      # Définition du plugin
│   │   └── dithering_plugin.cpp    # Implémentation du plugin
│   │
│   └── ai_generator/               # Plugin Générateur IA
│       ├── CMakeLists.txt          # Build du plugin
│       ├── ai_generator_plugin.h   # Définition du plugin
│       └── ai_generator_plugin.cpp # Implémentation du plugin
│
├── resources/                      # Ressources
│   ├── icons/                      # Icônes
│   ├── themes/                     # Thèmes visuels
│   └── presets/                    # Préréglages
│
├── docs/                           # Documentation
│   ├── architecture.md             # Architecture du système
│   ├── file_format.md              # Spécification du format .vsd
│   ├── plugins.md                  # Documentation sur les plugins
│   ├── ai_features.md              # Documentation des fonctionnalités IA
│   └── development_plan.md         # Plan de développement
│
└── tests/                          # Tests unitaires et d'intégration
    ├── core_tests/                 # Tests du moteur central
    ├── plugins_tests/              # Tests du système de plugins
    ├── effects_tests/              # Tests des effets
    └── ui_tests/                   # Tests de l'interface utilisateur