# ltr_hotkey_gui - Native hotkey controller GUI

find_package(Qt6 COMPONENTS Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(ltr_hotkey_gui
    main.cpp
    ltr_hotkey_gui.cpp
    ltr_hotkey_gui.h
)

target_link_libraries(ltr_hotkey_gui Qt6::Widgets)

install(TARGETS ltr_hotkey_gui DESTINATION bin)
