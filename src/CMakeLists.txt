SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# ###### Program ############################################################
ADD_EXECUTABLE(rserpooldemo
   canvas.cpp
   canvasnode.cpp
   canvasview.cpp
   configuration.cpp
   contextmenuconfig.cpp
   mainwidget.cpp
   networklistener.cpp
   node.cpp
   rserpooldemo.cpp
)

TARGET_LINK_LIBRARIES(rserpooldemo Qt5::Widgets Qt5::Xml Qt5::Core Qt5::Network)
INSTALL(TARGETS             rserpooldemo
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        BUNDLE DESTINATION  ${CMAKE_INSTALL_BINDIR})
# INSTALL(FILES rserpooldemo.1 DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)
