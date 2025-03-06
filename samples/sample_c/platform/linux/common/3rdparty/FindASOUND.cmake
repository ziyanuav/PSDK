#
# Find the native ASOUND includes and library
#
# This module defines
# ASOUND_INCLUDE_DIR, where to find opus.h
# ASOUND_LIBRARY, the libraries to link against to use ASOUND.
# ASOUND_FOUND, If false, do not try to use ASOUND.

FIND_PATH(ASOUND_INCLUDE_DIR asoundlib.h
        PATHS
        /usr/local/include/asound
        /usr/include/asound
        /opt/local/include
        /opt/include
        /usr/include
        /usr/include/alsa
        )

get_filename_component(ASOUND_INCLUDE_DIR ${ASOUND_INCLUDE_DIR} ABSOLUTE)

FIND_LIBRARY(ASOUND_LIBRARY asound
        /usr/local/lib
        /usr/lib
        /usr/lib/x86_64-linux-gnu
        )

IF (ASOUND_INCLUDE_DIR)
    IF (ASOUND_LIBRARY)
        SET(ASOUND_FOUND "YES")
    ENDIF (ASOUND_LIBRARY)
ENDIF (ASOUND_INCLUDE_DIR)

MARK_AS_ADVANCED(
        ASOUND_INCLUDE_DIR
        ASOUND_LIBRARY
        ASOUND_FOUND
)