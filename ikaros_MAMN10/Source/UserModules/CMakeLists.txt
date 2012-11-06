set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(FaceModule)
add_subdirectory(FocusSelector)
add_subdirectory(ModeSelector)
add_subdirectory(Movement)
add_subdirectory(ObjectModule)


set(SOURCE ${SOURCE} PARENT_SCOPE)
set(MODULES_INCLUDS ${MODULES_INCLUDS} PARENT_SCOPE)
set(MODULES_LIBS ${MODULES_LIBS} PARENT_SCOPE)