set (lib_src
    rusage.cpp
    nofilelimit.cpp
    )
add_library (gfsCommon-shared SHARED ${lib_src})
add_library (gfsCommon STATIC ${lib_src})
add_dependencies (gfsCommon
    version
    qcdio
)
add_dependencies (gfsCommon-shared
    version
    qcdio-shared
)
set_target_properties (kfsCommon PROPERTIES OUTPUT_NAME "qfs_common")
set_target_properties (kfsCommon-shared PROPERTIES OUTPUT_NAME "qfs_common")
set_target_properties (kfsCommon PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties (kfsCommon-shared PROPERTIES CLEAN_DIRECT_OUTPUT 1)

target_link_libraries (kfsCommon qcdio)
target_link_libraries (kfsCommon-shared qcdio-shared)

#
install (TARGETS kfsCommon-shared kfsCommon
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static)

install (FILES kfstypes.h kfsdecls.h DESTINATION include/kfs/common)
