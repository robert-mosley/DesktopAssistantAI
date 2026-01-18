cmake_minimum_required(VERSION 3.16)

project(DIHAssistant
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS
    Widgets
    Multimedia
    TextToSpeech
)

qt_standard_project_setup()

qt_add_executable(DIHAssistant
    main.cpp
    mainwindow.h
    mainwindow.cpp
    textbox.h
    textbox.cpp
    miniwindow.h
    miniwindow.cpp
    interface.h
    interface.cpp
    largewindow.h largewindow.cpp
)

target_link_libraries(DIHAssistant
    PRIVATE
        Qt6::Widgets
        Qt6::Multimedia
        Qt6::TextToSpeech
        ole32
        uuid
        sapi
)

# -------------------------------
# Vosk
# -------------------------------
set(VOSK_DIR "C:/vosk")

target_include_directories(DIHAssistant PRIVATE
    ${VOSK_DIR}/include
)

target_link_libraries(DIHAssistant PRIVATE
    ${VOSK_DIR}/libvosk.lib
)

add_custom_command(TARGET DIHAssistant POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${VOSK_DIR}/libvosk.dll"
        $<TARGET_FILE_DIR:DIHAssistant>
)

