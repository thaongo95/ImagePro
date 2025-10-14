cmake_minimum_required(VERSION 3.16)

project(ImagePro VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick)
find_package(OpenCV REQUIRED)

qt_standard_project_setup(REQUIRES 6.5)


# List C++ sources
set(SOURCES
    main.cpp
    imageprovider.cpp
    imagecontroller.cpp


)

set(HEADERS
    imageprovider.h
    imagecontroller.h


)

# List QML files
set(QML_FILES
    Main.qml
    MainMenu.qml
    MainToolBar.qml
    LeftView.qml
    RightView.qml

    AdvancedTools.qml
    2DFeaturesTools.qml
    BasicTools.qml
    FunctionsTools.qml
    ContoursTools.qml
    TransformationsTools.qml

    basic/Exposure.qml
    basic/Gamma.qml
    basic/Smoothing.qml
    basic/Morphology.qml
    basic/ErodeDilate.qml
    basic/ExtractLines.qml

    basic/Threshold.qml
    basic/InRange.qml

    transformations/Canny.qml
)
# List resources (images, videos)
set(RESOURCES



    assets/icons/ankh.png
    assets/icons/ant.png
    assets/icons/bee.png
    assets/icons/beehive.png
    assets/icons/butterfly.png
    assets/icons/chinese-symbol.png
    assets/icons/christian.png
    assets/icons/cow.png
    assets/icons/creek.png
    assets/icons/enso.png
    assets/icons/falcon.png
    assets/icons/frog.png
    assets/icons/gelt.png
    assets/icons/hamsa.png
    assets/icons/horse.png
    assets/icons/iceberg.png
    assets/icons/infinite.png
    assets/icons/moon.png
    assets/icons/mountain.png
    assets/icons/parrot.png
    assets/icons/peace.png
    assets/icons/peacebol.png
    assets/icons/peace-symbol.png
    assets/icons/pelican.png
    assets/icons/perspective.png
    assets/icons/radiation.png
    assets/icons/rhino.png
    assets/icons/snowflake.png
    assets/icons/star.png
    assets/icons/symbol.png
    assets/icons/tree.png
    assets/icons/triskelion.png
    assets/icons/whale.png
    assets/icons/world.png
    assets/icons/axe.png
    assets/icons/axedown.png
    assets/icons/hand-saw.png
    assets/icons/knife.png
    assets/icons/secateurs.png
    assets/icons/sword.png
    assets/icons/trowel.png
)

qt_add_executable(appImagePro
    ${SOURCES}
    ${HEADERS}
)
qt_add_qml_module(appImagePro
    URI ImagePro
    VERSION 1.0
    QML_FILES ${QML_FILES}
    RESOURCES ${RESOURCES}
    RESOURCES assets/icons/grape.png assets/icons/papaya.png assets/icons/tomato.png assets/icons/watermelon.png
    SOURCES
    SOURCES
    SOURCES
    SOURCES basic_operation.h basic_operation.cpp
    RESOURCES assets/icons/back-button.png assets/icons/dowanlod.png assets/icons/next-button.png assets/icons/repeat.png assets/icons/right-arrowhead.png
    QML_FILES basic/Convert.qml
    QML_FILES basic/ColorDetection.qml
    QML_FILES transformations/Filter2D.qml
    QML_FILES transformations/Rotation.qml
    QML_FILES transformations/Sobel.qml
    QML_FILES transformations/Laplacian.qml
    QML_FILES transformations/HoughLine.qml
    QML_FILES transformations/HoughCircle.qml
    QML_FILES transformations/GuilHough.qml
    QML_FILES transformations/Remap.qml
    QML_FILES transformations/Affine.qml
    RESOURCES assets/images/sodo.png
    RESOURCES assets/images/sudoku.png
    RESOURCES assets/icons/avocado.png
    QML_FILES contours/DrawEgde.qml
    QML_FILES contours/DrawShape.qml
    QML_FILES contours/ConvexHull.qml
    QML_FILES contours/Moments.qml
    QML_FILES contours/PointPolygon.qml
    QML_FILES basic/Contrast.qml
    QML_FILES basic/TemplateMatching.qml
    QML_FILES features2D/SIFT.qml features2D/Subpix.qml features2D/Custom.qml features2D/HarrisCorner.qml features2D/ShiTomasi.qml
    RESOURCES assets/icons/almond.png assets/icons/fruits.png assets/icons/olive.png assets/icons/pineapple.png assets/icons/strawberry.png
    RESOURCES assets/icons/dog.png assets/icons/flamingo.png
    QML_FILES features2D/FeaturesDetecting.qml
    QML_FILES features2D/FeaturesMatching.qml
    QML_FILES features2D/ORB.qml
    QML_FILES features2D/AKAZE.qml
    QML_FILES features2D/FeaturesStitching.qml
    QML_FILES functions/IconColor.qml
    RESOURCES assets/icons/circlePlus.png

)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appImagePro PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appImagePro
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appImagePro
    PRIVATE Qt6::Quick ${OpenCV_LIBS})
target_include_directories(appImagePro PRIVATE ${OpenCV_INCLUDE_DIRS})

include(GNUInstallDirs)
install(TARGETS appImagePro
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
