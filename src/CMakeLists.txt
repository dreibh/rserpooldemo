SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# ###### Program ############################################################
ADD_EXECUTABLE(rserpooldemo
   rdgraphicsscene.cc
   rdgraphicsnode.cc
   rdgraphicsview.cc
   configuration.cc
   contextmenuconfig.cc
   mainwidget.cc
   networklistener.cc
   node.cc
   rserpooldemo.cc
)

TARGET_LINK_LIBRARIES(rserpooldemo Qt5::Widgets Qt5::Xml Qt5::Core Qt5::Network)
INSTALL(TARGETS             rserpooldemo
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        BUNDLE DESTINATION  ${CMAKE_INSTALL_BINDIR})
# INSTALL(FILES rserpooldemo.1 DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)
