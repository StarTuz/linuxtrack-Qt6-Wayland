# src/qt_gui/CMakeLists.txt

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Widgets Gui Network Help OpenGL OpenGLWidgets REQUIRED)

# Common headers
set(LTR_GUI_HEADERS
    ltr_gui.h ltr_show.h ltr_dev_help.h help_view.h
    ltr_gui_prefs.h prefs_link.h tir_prefs.h device_setup.h
    ltr_model.h window.h glwidget.h objreader.h ltr_profiles.h
    log_view.h ltr_state.h scp_form.h buffering.h progress.h 
    scurve.h scview.h wiimote_prefs.h tracker.h plugin_install.h 
    profile_setup.h profile_selector.h guardian.h xplugin.h wine_warn.h 
    extractor.h hashing.h downloading.h wine_launcher.h 
    help_viewer.h prefix_discovery.h prefix_discovery_dialog.h
    webcam_prefs.h webcam_info.h webcam_ft_prefs.h joy_prefs.h
    udp_bridge.h udp_settings.h
)

# Common forms
set(LTR_GUI_FORMS
    ltr_gui.ui dev_help.ui ltr.ui model_creation.ui scurve.ui scp_form.ui 
    logview.ui wii_setup.ui tir_setup.ui 
    cap_edit.ui clip_edit.ui single_edit.ui model_edit.ui 
    device_setup.ui profile_setup.ui profile_selector.ui cap_tweaking.ui clip_tweaking.ui 
    extractor.ui progress.ui xplugin.ui wine_warn.ui
    prefix_discovery_dialog.ui
    l_wc_setup.ui l_wcft_setup.ui joy_setup.ui
    udp_settings.ui
)

# Common sources
set(LTR_GUI_SOURCES
    main.cpp ltr_gui.cpp ltr_show.cpp ltr_dev_help.cpp 
    ltr_gui_prefs.cpp tir_prefs.cpp help_view.cpp buffering.cpp 
    ltr_model.cpp window.cpp glwidget.cpp objreader.cpp ltr_profiles.cpp 
    log_view.cpp ltr_state.cpp scp_form.cpp guardian.cpp 
    scurve.cpp scview.cpp wiimote_prefs.cpp 
    tracker.cpp device_setup.cpp 
    plugin_install.cpp profile_setup.cpp 
    profile_selector.cpp xplugin.cpp wine_warn.cpp progress.cpp 
    extractor.cpp hashing.cpp downloading.cpp wine_launcher.cpp 
    help_viewer.cpp prefix_discovery.cpp 
    prefix_discovery_dialog.cpp
    webcam_info.cpp webcam_prefs.cpp webcam_ft_prefs.cpp joy_prefs.cpp
    lal_dialog.cpp
    udp_bridge.cpp udp_settings.cpp
    
    # Files from parent directory
    ../ltr_srv_master.cpp
    ../ltr_srv_slave.c
    ../ltr_srv_comm.c
    ../game_data.c
    ../extract.c
    ../digest.c
)

if(APPLE AND BUILD_MAC_EXPERIMENTAL)
    list(APPEND LTR_GUI_HEADERS
        macps3eye_prefs.h
        macps3eyeft_prefs.h
        macwebcam_info.h
        macwebcam_prefs.h
        macwebcamft_prefs.h
    )
    list(APPEND LTR_GUI_FORMS
        m_ps3eye_setup.ui
        m_ps3eye_ft_setup.ui
        m_wc_setup.ui
        m_wcft_setup.ui
    )
    list(APPEND LTR_GUI_SOURCES
        macps3eye_prefs.cpp
        macps3eyeft_prefs.cpp
        macwebcam_info.cpp
        macwebcam_prefs.cpp
        macwebcamft_prefs.cpp
    )
endif()

# Resources
set(LTR_GUI_RESOURCES ltr_rc.qrc)
set(LTR_GUI_DATA_FILES
    sparow_opaq.obj
    sparow_glass.obj
    xm8_detail.png
    sources.txt
    spec.txt
    sphere.obj
    sphere.png
    sources_mfc.txt
    win7.reg
)
set(LTR_GUI_HELP_FILES help.qhc help.qch)

if(APPLE AND BUILD_MAC_EXPERIMENTAL)
    set(LTR_GUI_BUNDLE_ICON ${CMAKE_CURRENT_SOURCE_DIR}/linuxtrack.icns)
    set_source_files_properties(${LTR_GUI_BUNDLE_ICON} PROPERTIES
        MACOSX_PACKAGE_LOCATION Resources
    )
    set(LTR_GUI_BUNDLE_RESOURCES
        ${CMAKE_SOURCE_DIR}/src/linuxtrack1.conf
        ${CMAKE_CURRENT_SOURCE_DIR}/sources_mac.txt
    )
    foreach(resource_file IN LISTS LTR_GUI_DATA_FILES)
        list(APPEND LTR_GUI_BUNDLE_RESOURCES
            ${CMAKE_CURRENT_SOURCE_DIR}/${resource_file}
        )
    endforeach()
    foreach(help_file IN LISTS LTR_GUI_HELP_FILES)
        list(APPEND LTR_GUI_BUNDLE_HELP_RESOURCES
            ${CMAKE_CURRENT_SOURCE_DIR}/${help_file}
        )
    endforeach()
    set_source_files_properties(${LTR_GUI_BUNDLE_RESOURCES} PROPERTIES
        MACOSX_PACKAGE_LOCATION Resources/linuxtrack
    )
    # Prevent Wavefront .obj 3D model files from being treated as compiled
    # object files by the linker — they are data resources, not code.
    foreach(resource_file IN LISTS LTR_GUI_BUNDLE_RESOURCES)
        if(resource_file MATCHES "\\.obj$")
            set_source_files_properties(${resource_file} PROPERTIES HEADER_FILE_ONLY ON)
        endif()
    endforeach()
    set_source_files_properties(${LTR_GUI_BUNDLE_HELP_RESOURCES} PROPERTIES
        MACOSX_PACKAGE_LOCATION Resources/linuxtrack/help/ltr_gui
    )
    set(MACOS_BUNDLE_DISPLAY_NAME "Linuxtrack")
    set(MACOS_BUNDLE_EXECUTABLE "Linuxtrack")
    set(MACOS_BUNDLE_ICON_FILE "linuxtrack.icns")
    set(MACOS_BUNDLE_IDENTIFIER "org.linuxtrack.ltr_gui.experimental")
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in
        ${CMAKE_CURRENT_BINARY_DIR}/Info.plist
        @ONLY
    )
    add_executable(ltr_gui MACOSX_BUNDLE
        ${LTR_GUI_SOURCES} ${LTR_GUI_FORMS} ${LTR_GUI_RESOURCES}
        ${LTR_GUI_BUNDLE_ICON}
        ${LTR_GUI_BUNDLE_RESOURCES}
        ${LTR_GUI_BUNDLE_HELP_RESOURCES}
    )
else()
    add_executable(ltr_gui ${LTR_GUI_SOURCES} ${LTR_GUI_FORMS} ${LTR_GUI_RESOURCES})
endif()

target_include_directories(ltr_gui PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR} 
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../lal
    ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/src
    ${MXML_INCLUDE_DIRS}
)

target_compile_definitions(ltr_gui PRIVATE 
    HAVE_CONFIG_H
    LTR_GUI
    HELP_BASE="ltr_gui/"
    QT_NO_CAST_FROM_ASCII
    QT_NO_CAST_TO_ASCII
)

if(APPLE AND BUILD_MAC_EXPERIMENTAL)
    target_compile_definitions(ltr_gui PRIVATE LTR_MAC_EXPERIMENTAL)
    set_target_properties(ltr_gui PROPERTIES
        OUTPUT_NAME Linuxtrack
        MACOSX_BUNDLE_BUNDLE_NAME Linuxtrack
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_GUI_IDENTIFIER org.linuxtrack.ltr_gui.experimental
        MACOSX_BUNDLE_ICON_FILE linuxtrack.icns
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_BINARY_DIR}/Info.plist
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
    )
endif()

target_link_libraries(ltr_gui PRIVATE 
    Qt6::Widgets 
    Qt6::Gui 
    Qt6::Network 
    Qt6::Help 
    Qt6::OpenGL 
    Qt6::OpenGLWidgets
    ltr 
    linuxtrack
    lal 
    ${GLU_LIBRARY}
    ${MXML_LIBRARIES}
    ${LTR_LIBM}
)
target_link_directories(ltr_gui PRIVATE ${MXML_LIBRARY_DIRS})

# Install executable
if(APPLE AND BUILD_MAC_EXPERIMENTAL)
    install(TARGETS ltr_gui
        BUNDLE DESTINATION .
        RUNTIME DESTINATION bin
    )
else()
    install(TARGETS ltr_gui DESTINATION bin)
endif()

# Install data files (as per .pro file)
if(APPLE AND BUILD_MAC_EXPERIMENTAL)
    install(FILES
        ${CMAKE_SOURCE_DIR}/src/linuxtrack1.conf
        ${CMAKE_CURRENT_SOURCE_DIR}/sources_mac.txt
        DESTINATION Linuxtrack.app/Contents/Resources/linuxtrack
    )
    install(FILES ${LTR_GUI_DATA_FILES}
        DESTINATION Linuxtrack.app/Contents/Resources/linuxtrack
    )
    install(FILES ${LTR_GUI_HELP_FILES}
        DESTINATION Linuxtrack.app/Contents/Resources/linuxtrack/help/ltr_gui
    )
else()
    install(FILES ${LTR_GUI_DATA_FILES} DESTINATION share/linuxtrack)

    # Install help files
    install(FILES ${LTR_GUI_HELP_FILES} DESTINATION share/linuxtrack/help/ltr_gui)
endif()
