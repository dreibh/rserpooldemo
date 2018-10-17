# --------------------------------------------------------------------------
#
#           #####          #     #   ###   #
#          #     #  #    # ##    #    #    #          ##    #####
#          #        #    # # #   #    #    #         #  #   #    #
#           #####   #    # #  #  #    #    #        #    #  #####
#                #  #    # #   # #    #    #        ######  #    #
#          #     #  #    # #    ##    #    #        #    #  #    #
#           #####    ####  #     #   ###   #######  #    #  #####
#
# --------------------------------------------------------------------------
#
# SuNILab Desktop Background Script
# Copyright (C) 2017-2018 by Thomas Dreibholz
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY# without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# Contact: dreibh@iem.uni-due.de


# ###### Initialise #########################################################
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(undefined NONE)   # to be set in Setting.cmake
INCLUDE(Settings.input)

SET(CMAKE_INSTALL_BINDIR        "bin"       CACHE PATH "user executables [PREFIX/bin]")
SET(CMAKE_INSTALL_SBINDIR       "sbin"      CACHE PATH "system admin executables [EPREFIX/sbin]")
SET(CMAKE_INSTALL_DATADIR       "share"     CACHE PATH "read-only architecture-independent data [PREFIX/share]")
SET(CMAKE_INSTALL_SYSCONFDIR    "etc"       CACHE PATH "read-only single-machine data [PREFIX/etc]")
SET(CMAKE_INSTALL_LOCALSTATEDIR "var"       CACHE PATH "modifiable single-machine data [PREFIX/var]")
SET(CMAKE_INSTALL_LIBDIR        "lib"       CACHE PATH "object code libraries [PREFIX/lib]")
SET(CMAKE_INSTALL_INCLUDEDIR    "include"   CACHE PATH "read-only includes [PREFIX/include]")
SET(CMAKE_INSTALL_MANDIR        "share/man" CACHE PATH "man documentation [PREFIX/share/man]")


# ###### Options ############################################################
OPTION(FLAT_DIRECTORY_STRUCTURE "Use flat directory structure" ${TDD_FLAT_DIRECTORY_STRUCTURE})

OPTION(INSERT_HOSTNAME "Insert hostnames into images" ${TDD_INSERT_HOSTNAME})

OPTION(BUILD_BOOTSPLASH "Build bootsplash JPEG" ${TDD_BUILD_BOOTSPLASH})
IF(BUILD_BOOTSPLASH)
   LIST(APPEND TDD_RESOLUTIONS "${TDD_BOOTSPLASH_RESOLUTION}")
   LIST(APPEND TDD_VARIANTS    "${TDD_BOOTSPLASH_VARIANT}")
ENDIF()

OPTION(PRINT_A4 "Build A4 PDF print version" ${TDD_PRINT_A4})
OPTION(PRINT_A3 "Build A3 PDF print version" ${TDD_PRINT_A3})
IF(PRINT_A4 OR PRINT_A3)
   LIST(APPEND TDD_RESOLUTIONS "2970x2100")
   LIST(APPEND TDD_VARIANTS    "${TDD_PRINT_VARIANT}")
ENDIF()


# ###### Requirements #######################################################
INCLUDE(FindUnixCommands)

FIND_PROGRAM(BC bc PATHS /bin /usr/bin /usr/local/bin)
IF(NOT BC)
   MESSAGE(FATAL_ERROR "bc not found!")
ENDIF()

FIND_PROGRAM(EXIFTOOL exiftool PATHS /bin /usr/bin /usr/local/bin)
IF(NOT EXIFTOOL)
   MESSAGE(FATAL_ERROR "exiftool not found!")
ENDIF()

FIND_PROGRAM(CONVERT convert PATHS /bin /usr/bin /usr/local/bin)
IF(NOT CONVERT)
   MESSAGE(FATAL_ERROR "convert not found!")
ENDIF()

FIND_PROGRAM(IDENTIFY identify PATHS /bin /usr/bin /usr/local/bin)
IF(NOT IDENTIFY)
   MESSAGE(FATAL_ERROR "identify not found!")
ENDIF()


# ###### Packaging ##########################################################
IF(TDD_ADD_DIST_TARGET)
   SET(CPACK_SOURCE_GENERATOR "TGZ")
   SET(CPACK_SOURCE_PACKAGE_FILE_NAME
      "${CMAKE_PROJECT_NAME}-${BUILD_MAJOR}.${BUILD_MINOR}.${BUILD_PATCH}")
   SET(CPACK_SOURCE_IGNORE_FILES
      "${PROJECT_SOURCE_DIR}/${CMAKE_PROJECT_NAME}-*;${PROJECT_SOURCE_DIR}/${CMAKE_PROJECT_NAME}_*;/build/;/.git/;/.gimp/;/config.log;/CMakeFiles/;CMakeCache.txt$;.cmake$;.tar.gz$;/_CPack_Packages;/Makefile$;~$;/build-deb;/clean-deb;/filter-empty-entries;/make-symbols;/make-ppa;/make-deb;/debian.conf;/make-rpm;/rpm.conf;${CPACK_SOURCE_IGNORE_FILES};/Output/")

   INCLUDE(CPack)
   ADD_CUSTOM_TARGET(dist COMMAND +${CMAKE_MAKE_PROGRAM} clean package_source)
ENDIF()


# ###### Targets ############################################################

# NOTE: The following build rules use custom commands. They need additional
#       targets as well, to correctly run parallel builds! For details, see:
# https://samthursfield.wordpress.com/2015/11/21/cmake-dependencies-between-targets-and-files-and-custom-commands/

LIST(REMOVE_DUPLICATES TDD_RESOLUTIONS)
LIST(SORT TDD_RESOLUTIONS)
LIST(REMOVE_DUPLICATES TDD_IMAGE_FILES)
LIST(SORT TDD_IMAGE_FILES)
LIST(REMOVE_DUPLICATES TDD_TEXT_FILES)
LIST(SORT TDD_TEXT_FILES)
LIST(REMOVE_DUPLICATES TDD_VARIANTS)
LIST(SORT TDD_VARIANTS)

FILE(MAKE_DIRECTORY ${TDD_OUTPUT_DIRECTORY})


# ###### Generate text graphics #############################################
FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${TDD_OUTPUT_DIRECTORY}/Text)
FOREACH(text IN LISTS TDD_TEXT_FILES)
   # ====== Set text graphics name ==========================================
   STRING(REGEX REPLACE ".text$" ".png" graphics "${text}")
   GET_FILENAME_COMPONENT(graphics ${graphics} NAME)

   ADD_CUSTOM_COMMAND(OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/${TDD_OUTPUT_DIRECTORY}/Text/${graphics}
                      COMMAND env GIMP2_DIRECTORY=${CMAKE_CURRENT_BINARY_DIR}/.gimp MAKE_LOGO="${MAKE_LOGO}" OUTPUT="${CMAKE_CURRENT_BINARY_DIR}/${TDD_OUTPUT_DIRECTORY}/Text/${graphics}" ${BASH} ${text}
                      DEPENDS ${text})
   LIST(APPEND all_graphics ${CMAKE_CURRENT_BINARY_DIR}/${TDD_OUTPUT_DIRECTORY}/Text/${graphics})

   ADD_CUSTOM_TARGET(generate_${graphics}
                     DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${TDD_OUTPUT_DIRECTORY}/Text/${graphics}
                             ${text})
   LIST(APPEND generate_all_graphics generate_${graphics})
ENDFOREACH()


# ###### Generate hostname graphics #########################################
FOREACH(image IN LISTS TDD_IMAGE_FILES)
   GET_FILENAME_COMPONENT(imageName ${image} NAME_WE)
   IF(imageName MATCHES ^[a-z])
      SET(hostname               "${imageName}.${TDD_DOMAIN}")
      SET(hostnameImage          "${CMAKE_CURRENT_BINARY_DIR}/${TDD_OUTPUT_DIRECTORY}/Text/${hostname}.png")
      SET(generate_hostnameImage "generate_${hostname}.png")

      ADD_CUSTOM_COMMAND(OUTPUT ${hostnameImage}
                         COMMAND env GIMP2_DIRECTORY=${CMAKE_CURRENT_BINARY_DIR}/.gimp ${MAKE_LOGO} ${hostnameImage} "${hostname}" 256 10 "Brushed Aluminium" "Comic Sans MS")
      ADD_CUSTOM_TARGET(${generate_hostnameImage} DEPENDS ${hostnameImage})
   ENDIF()
ENDFOREACH()


# ###### Generate bootsplash overlay ########################################
FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${TDD_OUTPUT_DIRECTORY}/Overlay)
FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${TDD_OUTPUT_DIRECTORY}/Splash)

SET(captionFile          "${CMAKE_CURRENT_BINARY_DIR}/${TDD_OUTPUT_DIRECTORY}/Splash/Caption.input")
SET(generate_captionFile "generate_Caption.input")
ADD_CUSTOM_COMMAND(OUTPUT ${captionFile}
                   COMMAND echo "${TDD_BOOTSPLASH_TEXT}" >${captionFile})
ADD_CUSTOM_TARGET(${generate_captionFile} DEPENDS ${captionFile})

SET(bootsplash_overlay          "${CMAKE_CURRENT_BINARY_DIR}/${TDD_OUTPUT_DIRECTORY}/Overlay/Caption-${TDD_BOOTSPLASH_RESOLUTION}.png")
SET(generate_bootsplash_overlay "generate_Caption-${TDD_BOOTSPLASH_RESOLUTION}.png")
STRING(REGEX REPLACE "x.*$" "" bootsplash_width "${TDD_BOOTSPLASH_RESOLUTION}")
ADD_CUSTOM_COMMAND(OUTPUT  ${bootsplash_overlay}
                   COMMAND ${TDD_MAKE_CAPTION} ${bootsplash_overlay} ${captionFile} ${bootsplash_width}
                   DEPENDS ${generate_captionFile}
                           ${captionFile})
ADD_CUSTOM_TARGET(${generate_bootsplash_overlay} DEPENDS ${bootsplash_overlay})


# ###### Generate desktop graphics ##########################################
FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${TDD_OUTPUT_DIRECTORY}/Desktop-with-Logo)
FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${TDD_OUTPUT_DIRECTORY}/Desktop-without-Logo)
FOREACH(resolution IN LISTS TDD_RESOLUTIONS)

   # ====== Compute aspect ratio ============================================
   STRING(REGEX REPLACE "x.*$" "" width  "${resolution}")
   STRING(REGEX REPLACE "^.*x" "" height "${resolution}")
   STRING(REGEX REPLACE "x" "/"   calc   "${resolution}")
   EXECUTE_PROCESS(COMMAND echo "${calc}" COMMAND ${BC} -l OUTPUT_VARIABLE aspectRatio OUTPUT_STRIP_TRAILING_WHITESPACE)

   # ====== Generate overlay graphics =======================================
   SET(overlay          "${CMAKE_CURRENT_BINARY_DIR}/${TDD_OUTPUT_DIRECTORY}/Overlay/Overlay-${resolution}.png")
   SET(generate_overlay "generate_overlay-${resolution}.png")

   # NOTE: Dependency on generate targets and files!
   ADD_CUSTOM_COMMAND(OUTPUT ${overlay}
                      COMMAND env GIMP2_DIRECTORY=${CMAKE_CURRENT_BINARY_DIR}/.gimp ${TDD_MAKE_OVERLAY} ${overlay} ${width} ${height} ${CMAKE_CURRENT_BINARY_DIR}/${TDD_OUTPUT_DIRECTORY}/Text
                      DEPENDS ${generate_all_graphics})
   ADD_CUSTOM_TARGET(${generate_overlay} DEPENDS ${overlay})


   # ====== Generate backgrounds ============================================
   FOREACH(variant IN LISTS TDD_VARIANTS)
      FOREACH(image IN LISTS TDD_IMAGE_FILES)
         # ====== Extract name and extension ================================
         GET_FILENAME_COMPONENT(imageName      ${image} NAME_WE)
         GET_FILENAME_COMPONENT(imageExtension ${image} EXT)
         STRING(TOLOWER ${imageExtension} imageExtension)
         IF(imageExtension STREQUAL ".jpg")
            SET(imageExtension ".jpeg")
         ENDIF()


         # ====== Without logo ==============================================
         IF(FLAT_DIRECTORY_STRUCTURE)
            SET(dirWithoutLogo     "${CMAKE_CURRENT_BINARY_DIR}/${TDD_OUTPUT_DIRECTORY}/Desktop-without-Logo")
            SET(imageWithoutLogo   "${dirWithoutLogo}/${imageName}-${resolution}-${variant}${imageExtension}")
            SET(instDirWithoutLogo "${CMAKE_INSTALL_DATADIR}/${TDD_INSTALL_SUBDIRECTORY}/Desktop-without-Logo")
         ELSE()
            SET(dirWithoutLogo     "${CMAKE_CURRENT_BINARY_DIR}/${TDD_OUTPUT_DIRECTORY}/Desktop-without-Logo/${resolution}/${variant}")
            SET(imageWithoutLogo   "${dirWithoutLogo}/${imageName}${imageExtension}")
            SET(instDirWithoutLogo "${CMAKE_INSTALL_DATADIR}/${TDD_INSTALL_SUBDIRECTORY}/Desktop-without-Logo/${resolution}/${variant}")
         ENDIF()
         SET(generate_imageWithoutLogo "generate_${imageName}-without-Logo-${variant}-${resolution}${imageExtension}")
         FILE(MAKE_DIRECTORY ${dirWithoutLogo})

         SET(imageTemp2 ${imageWithoutLogo}-tmp2.png)   # Processed image
         SET(imageTemp1 ${imageWithoutLogo}-tmp1.png)   # Processed and cropped image

         # ------ Plain -----------------------------------------------------
         IF(variant STREQUAL "plain")
            IF(imageExtension STREQUAL ".png")
               ADD_CUSTOM_COMMAND(OUTPUT  ${imageWithoutLogo}
                                  COMMAND env GIMP2_DIRECTORY=${CMAKE_CURRENT_BINARY_DIR}/.gimp ${MAKE_BACKGROUND} ${image} ${imageTemp2} &&
                                          env GIMP2_DIRECTORY=${CMAKE_CURRENT_BINARY_DIR}/.gimp ${MAKE_CROPPED}    ${imageTemp2} ${imageTemp1} ${aspectRatio} &&
                                          ${CONVERT} ${imageTemp1} -resize ${resolution} -interlace line ${imageWithoutLogo} &&
                                          ${EXIFTOOL} -q -overwrite_original -Copyright="${TDD_COPYRIGHT}" ${imageWithoutLogo} &&
                                          ${RM} -f ${imageTemp1} ${imageTemp2}
                                  DEPENDS ${image})
            ELSE()
               ADD_CUSTOM_COMMAND(OUTPUT  ${imageWithoutLogo}
                                  COMMAND env GIMP2_DIRECTORY=${CMAKE_CURRENT_BINARY_DIR}/.gimp ${MAKE_CROPPED} ${image} ${imageTemp1} ${aspectRatio} &&
                                          ${CONVERT} ${imageTemp1} -resize ${resolution} -interlace line ${imageWithoutLogo} &&
                                          ${EXIFTOOL} -q -overwrite_original -Copyright="${TDD_COPYRIGHT}" ${imageWithoutLogo} &&
                                          ${RM} -f ${imageTemp1} ${imageTemp2}
                                  DEPENDS ${image})
            ENDIF()

         # ------ Mosaic ----------------------------------------------------
         ELSEIF(variant STREQUAL "mosaic")
            EXECUTE_PROCESS(COMMAND ${IDENTIFY} -format "%wx%h\n" ${image} OUTPUT_VARIABLE originalResulution OUTPUT_STRIP_TRAILING_WHITESPACE)
            STRING(REGEX REPLACE "x.*$" "" originalWidth  "${originalResulution}")
            STRING(REGEX REPLACE "^.*x" "" originalHeight "${originalResulution}")
            EXECUTE_PROCESS(COMMAND echo "(5.5*${originalWidth})/1024" COMMAND ${BC} -l OUTPUT_VARIABLE tileSize   OUTPUT_STRIP_TRAILING_WHITESPACE)
            EXECUTE_PROCESS(COMMAND echo "(1.0*${originalWidth})/1024" COMMAND ${BC} -l OUTPUT_VARIABLE tileHeight OUTPUT_STRIP_TRAILING_WHITESPACE)
            # MESSAGE("Mosaic: ${originalResulution} ${width} -> ${tileSize} ${tileHeight}")
            ADD_CUSTOM_COMMAND(OUTPUT  ${imageWithoutLogo}
                               COMMAND env GIMP2_DIRECTORY=${CMAKE_CURRENT_BINARY_DIR}/.gimp ${MAKE_MOSAIC}  ${image} ${imageTemp2} ${tileSize} ${tileHeight} ${tileHeight} hexagons rough >/dev/null &&
                                       env GIMP2_DIRECTORY=${CMAKE_CURRENT_BINARY_DIR}/.gimp ${MAKE_CROPPED} ${imageTemp2} ${imageTemp1} ${aspectRatio} &&
                                       ${CONVERT} ${imageTemp1} -resize ${resolution} -interlace line ${imageWithoutLogo} &&
                                       ${EXIFTOOL} -q -overwrite_original -Copyright="${TDD_COPYRIGHT}" ${imageWithoutLogo} &&
                                       ${RM} -f ${imageTemp1} ${imageTemp2}
                               DEPENDS ${image})

         # ------ Oil Painting ----------------------------------------------
         ELSEIF(variant STREQUAL "oil")
            EXECUTE_PROCESS(COMMAND ${IDENTIFY} -format "%wx%h\n" ${image} OUTPUT_VARIABLE originalResulution OUTPUT_STRIP_TRAILING_WHITESPACE)
            STRING(REGEX REPLACE "x.*$" "" originalWidth  "${originalResulution}")
            STRING(REGEX REPLACE "^.*x" "" originalHeight "${originalResulution}")
            EXECUTE_PROCESS(COMMAND echo "(8*${originalWidth})/1024" COMMAND ${BC} -l OUTPUT_VARIABLE maskSize   OUTPUT_STRIP_TRAILING_WHITESPACE)
            # MESSAGE("Painting: ${originalResulution} ${width} -> ${maskSize}")
            ADD_CUSTOM_COMMAND(OUTPUT  ${imageWithoutLogo}
                               COMMAND env GIMP2_DIRECTORY=${CMAKE_CURRENT_BINARY_DIR}/.gimp ${MAKE_PAINTING} ${image} ${imageTemp2} ${maskSize} >/dev/null &&
                                       env GIMP2_DIRECTORY=${CMAKE_CURRENT_BINARY_DIR}/.gimp ${MAKE_CROPPED}  ${imageTemp2} ${imageTemp1} ${aspectRatio} &&
                                       ${CONVERT} ${imageTemp1} -resize ${resolution} -interlace line ${imageWithoutLogo} &&
                                       ${EXIFTOOL} -q -overwrite_original -Copyright="${TDD_COPYRIGHT}" ${imageWithoutLogo} &&
                                       ${RM} -f ${imageTemp1} ${imageTemp2}
                               DEPENDS ${image})

         # ------ Error -----------------------------------------------------
         ELSE()
            MESSAGE(FATAL_ERROR "Invalid variant: ${variant}!")
         ENDIF()

         ADD_CUSTOM_TARGET(${generate_imageWithoutLogo} ALL DEPENDS ${imageWithoutLogo})
         INSTALL(FILES ${imageWithoutLogo} DESTINATION ${instDirWithoutLogo})


         # ====== With logo =================================================
         IF(FLAT_DIRECTORY_STRUCTURE)
            SET(dirWithLogo     "${CMAKE_CURRENT_BINARY_DIR}/${TDD_OUTPUT_DIRECTORY}/Desktop-with-Logo")
            SET(imageWithLogo   "${dirWithLogo}/${imageName}-${resolution}-${variant}${imageExtension}")
            SET(instDirWithLogo "${CMAKE_INSTALL_DATADIR}/${TDD_INSTALL_SUBDIRECTORY}/Desktop-with-Logo")
         ELSE()
            SET(dirWithLogo     "${CMAKE_CURRENT_BINARY_DIR}/${TDD_OUTPUT_DIRECTORY}/Desktop-with-Logo/${resolution}/${variant}")
            SET(imageWithLogo   "${dirWithLogo}/${imageName}${imageExtension}")
            SET(instDirWithLogo "${CMAKE_INSTALL_DATADIR}/${TDD_INSTALL_SUBDIRECTORY}/Desktop-with-Logo/${resolution}/${variant}")
         ENDIF()
         SET(generate_imageWithLogo "generate_${imageName}-with-Logo-${variant}-${resolution}${imageExtension}")
         FILE(MAKE_DIRECTORY ${dirWithLogo})

         # ------ With hostname ---------------------------------------------
         IF((INSERT_HOSTNAME) AND (imageName MATCHES ^[a-z]))
            SET(hostname               "${imageName}.${TDD_DOMAIN}")
            SET(hostnameImage          "${CMAKE_CURRENT_BINARY_DIR}/${TDD_OUTPUT_DIRECTORY}/Text/${hostname}.png")
            SET(generate_hostnameImage "generate_${hostname}.png")

            MATH(EXPR hostnameHeight "${height}/20")   # 5% of height
            MATH(EXPR hostnameIndent "${height}/20")   # 5% of height
            ADD_CUSTOM_COMMAND(OUTPUT ${imageWithLogo}
                               COMMAND ${CONVERT} '\(' ${imageWithoutLogo} '\)' -append '\(' ${overlay} '\)' -gravity center -composite '\(' ${hostnameImage} -trim +repage -resize x${hostnameHeight} '\)' -gravity north -geometry +0+${hostnameIndent} -composite -interlace line -quality ${TDD_JPEG_QUALITY} ${imageWithLogo} &&
                                       ${EXIFTOOL} -q -overwrite_original -Copyright="${TDD_COPYRIGHT}" ${imageWithLogo}
                               DEPENDS ${generate_overlay}
                                       ${generate_hostnameImage}
                                       ${generate_imageWithoutLogo})

         # ------ Without hostname ------------------------------------------
         ELSE()
            ADD_CUSTOM_COMMAND(OUTPUT ${imageWithLogo}
                               COMMAND ${CONVERT} '\(' ${imageWithoutLogo} '\)' -append '\(' ${overlay} '\)' -gravity center -composite -interlace line -quality ${TDD_JPEG_QUALITY} ${imageWithLogo} &&
                                       ${EXIFTOOL} -q -overwrite_original -Copyright="${TDD_COPYRIGHT}" ${imageWithLogo}
                               DEPENDS ${generate_overlay}
                                       ${generate_imageWithoutLogo})
         ENDIF()

         ADD_CUSTOM_TARGET(${generate_imageWithLogo} ALL DEPENDS ${imageWithLogo})
         INSTALL(FILES ${imageWithLogo} DESTINATION ${instDirWithLogo})


         # ====== Bootsplash ================================================
         IF((BUILD_BOOTSPLASH) AND (resolution STREQUAL "${TDD_BOOTSPLASH_RESOLUTION}") AND (variant STREQUAL "${TDD_BOOTSPLASH_VARIANT}"))
            SET(imageBootsplash          "${CMAKE_CURRENT_BINARY_DIR}/${TDD_OUTPUT_DIRECTORY}/Splash/${imageName}-${resolution}.jpeg")
            SET(generate_imageBootsplash "generate_${imageName}-Splash-${resolution}.jpeg")

            ADD_CUSTOM_COMMAND(OUTPUT  ${imageBootsplash}
                               COMMAND ${CONVERT} -size ${TDD_BOOTSPLASH_RESOLUTION} xc:skyblue '\(' ${imageWithLogo} '\)' -gravity center -composite '\(' ${bootsplash_overlay} '\)' -gravity South -composite -quality ${TDD_JPEG_QUALITY} ${imageBootsplash} &&
                                       ${EXIFTOOL} -q -overwrite_original -Copyright="${TDD_COPYRIGHT}" ${imageBootsplash}
                               DEPENDS ${generate_bootsplash_overlay}
                                       ${generate_imageWithLogo})
            ADD_CUSTOM_TARGET(${generate_imageBootsplash} ALL DEPENDS ${imageBootsplash})
            INSTALL(FILES ${imageBootsplash} DESTINATION ${CMAKE_INSTALL_DATADIR}/${TDD_INSTALL_SUBDIRECTORY}/Splash)
         ENDIF()


         # ====== Print PDF DIN A4 ==========================================
         IF((PRINT_A4) AND
           (resolution STREQUAL "2970x2100") AND
           (variant STREQUAL "${TDD_PRINT_VARIANT}") AND
           ("${imageName}${imageExtension}" STREQUAL "${TDD_PRINT_IMAGE}"))
            SET(pdfA4          "${CMAKE_CURRENT_BINARY_DIR}/${TDD_OUTPUT_DIRECTORY}/${TDD_PRINT_PREFIX}-A4.pdf")
            SET(generate_pdfA4 "generate_${TDD_PRINT_PREFIX}-A4.pdf")

            ADD_CUSTOM_COMMAND(OUTPUT ${pdfA4}
                              COMMAND ${CONVERT} ${imageWithLogo} ${overlay}  -units PixelsPerCentimeter -density 100x100 -page 2970x2100 -resize 2970x2100 -gravity center -compress lossless ${pdfA4}
                              DEPENDS ${generate_overlay}
                                      ${generate_imageWithLogo})
            ADD_CUSTOM_TARGET(${generate_pdfA4} ALL DEPENDS ${pdfA4})
            INSTALL(FILES ${pdfA4} DESTINATION ${CMAKE_INSTALL_DATADIR}/${TDD_INSTALL_SUBDIRECTORY})
         ENDIF()

         # ====== Print PDF DIN A3 ==========================================
         IF((PRINT_A3) AND
           (resolution STREQUAL "2970x2100") AND
           (variant STREQUAL "${TDD_PRINT_VARIANT}") AND
           ("${imageName}${imageExtension}" STREQUAL "${TDD_PRINT_IMAGE}"))
            SET(pdfA3          "${CMAKE_CURRENT_BINARY_DIR}/${TDD_OUTPUT_DIRECTORY}/${TDD_PRINT_PREFIX}-A3.pdf")
            SET(generate_pdfA3 "generate_${TDD_PRINT_PREFIX}-A3.pdf")

            ADD_CUSTOM_COMMAND(OUTPUT ${pdfA3}
                              COMMAND ${CONVERT} ${imageWithLogo} ${overlay}  -units PixelsPerCentimeter -density 100x100 -page 5940x4200 -resize 5940x4200 -gravity center -compress lossless ${pdfA3}
                              DEPENDS ${generate_overlay}
                                      ${generate_imageWithLogo})
            ADD_CUSTOM_TARGET(${generate_pdfA3} ALL DEPENDS ${pdfA3})
            INSTALL(FILES ${pdfA3} DESTINATION ${CMAKE_INSTALL_DATADIR}/${TDD_INSTALL_SUBDIRECTORY})
         ENDIF()

      ENDFOREACH()
   ENDFOREACH()
ENDFOREACH()
