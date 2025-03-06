#
# Find the native PORTAUDIO includes and library
#
# This module defines
# PORTAUDIO_INCLUDE_DIR, where to find opus.h
# PORTAUDIO_LIBRARY, the libraries to link against to use PORTAUDIO.
# PORTAUDIO_FOUND, If false, do not try to use PORTAUDIO.

FIND_PATH(PORTAUDIO_INCLUDE_DIR portaudio.h
        PATHS
        /usr/local/include/portaudio
        /usr/include/portaudio
        /opt/local/include
        /opt/include
        /usr/include
        )

get_filename_component(PORTAUDIO_INCLUDE_DIR ${PORTAUDIO_INCLUDE_DIR} ABSOLUTE)

FIND_LIBRARY(PORTAUDIO_LIBRARY portaudio
        /usr/local/lib
        /usr/lib
        /usr/lib/x86_64-linux-gnu
        )

IF (PORTAUDIO_INCLUDE_DIR)
    IF (PORTAUDIO_LIBRARY)
        SET(PORTAUDIO_FOUND "YES")
    ENDIF (PORTAUDIO_LIBRARY)
ENDIF (PORTAUDIO_INCLUDE_DIR)

MARK_AS_ADVANCED(
        PORTAUDIO_INCLUDE_DIR
        PORTAUDIO_LIBRARY
        PORTAUDIO_FOUND
)