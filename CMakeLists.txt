cmake_minimum_required(VERSION 3.19)
project(DesktopPet LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(DesktopPet
    WIN32 MACOSX_BUNDLE
    main.cpp
    widget.cpp
    widget.h
)

target_link_libraries(DesktopPet
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS DesktopPet
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET DesktopPet
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
# ==========================================
# 自动将 assets 文件夹拷贝到构建输出目录
# ==========================================
add_custom_command(TARGET DesktopPet POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:DesktopPet>/assets"
    COMMENT "Copying assets to build directory..."
)