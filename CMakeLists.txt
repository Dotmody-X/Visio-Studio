cmake_minimum_required(VERSION 3.14)

project(VISIO_STUDIO LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Trouver les packages Qt nécessaires
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Concurrent)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Network) # Ajoutez cette ligne

# Fichiers source principaux
set(PROJECT_SOURCES
    src/main.cpp
    src/gui/mainwindow.cpp
    src/gui/mainwindow.h
    src/gui/mainwindow.ui
)

# Fichiers du moteur central
set(CORE_SOURCES
    src/core/engine.cpp
    src/core/engine.h
    src/core/project.cpp
    src/core/project.h
)

# Fichiers de l'interface graphique
set(GUI_SOURCES
    src/gui/mainwindow.cpp
    src/gui/mainwindow.h
    src/gui/mainwindow.ui
    src/gui/canvas.cpp
    src/gui/canvas.h
    src/gui/mixer.cpp
    src/gui/mixer.h
)

# Système de plugins
set(PLUGIN_SOURCES
    src/plugins/plugininterface.h
    src/plugins/pluginmanager.cpp
    src/plugins/pluginmanager.h
    src/plugins/baseplugin.cpp
    src/plugins/baseplugin.h
)

# Effets de base
set(EFFECTS_SOURCES
    src/effects/dithering/dithering.cpp
    src/effects/dithering/dithering.h
    src/effects/basic/basic.cpp
    src/effects/basic/basic.h
    src/effects/glitch/glitch.cpp
    src/effects/glitch/glitch.h
    # Note: Les fichiers JSON (dithering.json, basic.json) ne sont plus nécessaires
    # car nous utilisons des plugins intégrés sans Q_PLUGIN_METADATA
)

# Composants IA
set(AI_SOURCES
    src/ai/generator.cpp
    src/ai/generator.h
)

# Fichiers de ressources
set(RESOURCE_FILES
    resources/resources.qrc
)

# Compilation de l'exécutable
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(VISIO_STUDIO
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${CORE_SOURCES}
        ${GUI_SOURCES}
        ${PLUGIN_SOURCES}
        ${EFFECTS_SOURCES}
        ${AI_SOURCES}
        ${RESOURCE_FILES}
    )
else()
    add_executable(VISIO_STUDIO
        ${PROJECT_SOURCES}
        ${CORE_SOURCES}
        ${GUI_SOURCES}
        ${PLUGIN_SOURCES}
        ${EFFECTS_SOURCES}
        ${AI_SOURCES}
        ${RESOURCE_FILES}
    )
endif()

# Lier les bibliothèques
target_link_libraries(VISIO_STUDIO PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Concurrent
    Qt${QT_VERSION_MAJOR}::Network
)

# Installation
install(TARGETS VISIO_STUDIO
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(VISIO_STUDIO)
endif()
