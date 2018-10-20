SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# ###### Program ############################################################
ADD_EXECUTABLE(rserpooldemo
   networklistener.cc
   rdconfignode.cc
   rdconfiguration.cc
   rdcontextmenuconfig.cc
   rdgraphicsnode.cc
   rdgraphicsscene.cc
   rdgraphicsview.cc
   rdmainwindow.cc
   rserpooldemo.cc
)

TARGET_LINK_LIBRARIES(rserpooldemo Qt5::Widgets Qt5::Xml Qt5::Core Qt5::Network)
INSTALL(TARGETS             rserpooldemo
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        BUNDLE DESTINATION  ${CMAKE_INSTALL_BINDIR})
# INSTALL(FILES rserpooldemo.1 DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)
