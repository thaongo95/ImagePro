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
    imageinfo.cpp
)

set(HEADERS
    imageprovider.h
    imagecontroller.h
    imageinfo.h
)

# List QML files
set(QML_FILES
    Main.qml
    MainMenu.qml
    MainToolBar.qml
    AdvancedTools.qml
    HistogramsTools.qml
    BasicTools.qml
    FunctionsTools.qml
    ContoursTools.qml
    MainParamInput.qml
    FileParam.qml
    HistogramsParam.qml
    BasicParam.qml
    ContoursParam.qml
    FunctionsParam.qml
    TransformationsTools.qml
)
# List resources (images, videos)
set(RESOURCES
    assets/images/aerial.png
    assets/images/anh1.jpg
    assets/images/anh2.jpg
    assets/images/anh3.jpg
    assets/images/anh4.jpg
    assets/images/cards.jpg
    assets/images/clock.png
    assets/images/drone.png
    assets/images/FPV.jpg
    assets/images/FPV.png
    assets/images/fpv.png
    assets/images/front.jpg
    assets/images/generalized_hough_image.jpg
    assets/images/generalized_hough_mini_image.jpg
    assets/images/generalized_hough_mini_template.jpg
    assets/images/generalized_hough_template.jpg
    assets/images/home.png
    assets/images/lambo.png
    assets/images/letter-p.png
    assets/images/letter-r.png
    assets/images/letter-y.png
    assets/images/list-white.png
    assets/images/location-marker-white.png
    assets/images/log-file-white.png
    assets/images/map.png
    assets/images/map2d.png
    assets/images/measure-white.png
    assets/images/origin.jpg
    assets/images/paper-plane-white.png
    assets/images/paper.jpg
    assets/images/placeholder.png
    assets/images/productPageTitle-uav-4-1.png
    assets/images/productPageTitle-uav-4-2.png
    assets/images/road.png
    assets/images/satellite.png
    assets/images/shapes.png
    assets/images/side.jpg
    assets/images/sodo.png
    assets/images/speed.png
    assets/images/temperature.png
    assets/images/test.png
    assets/images/UAV-100AI.png
    assets/images/UAV-50_1.png
    assets/images/UAV-P5_0.png
    assets/images/UAV-Z4DT.png
    assets/images/UAV-Z6D.png
    assets/images/UCAV-13_0.png
    assets/images/video-player-white.png


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
    assets/icons/whale.png assets/icons/world.png

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
    SOURCES style.h style.cpp
    SOURCES image_function.h image_function.cpp
    SOURCES app_tools.h app_tools.cpp
    SOURCES
    QML_FILES LeftView.qml
    QML_FILES RightView.qml
    QML_FILES transformations/Canny.qml
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
