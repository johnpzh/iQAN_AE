message(STATUS ${IOKIT_LIBRARY})

add_executable(
        PcmMsrDriver
        MACOSX_BUNDLE
        PcmMsr.cpp
        PcmMsrDriver_info.c
        PcmMsr-Info.plist
)

set_target_properties(PcmMsrDriver PROPERTIES BUNDLE_EXTENSION kext MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/PcmMsr-Info.plist)
target_include_directories(PcmMsrDriver PRIVATE
        ${CMAKE_OSX_SYSROOT}/System/Library/Frameworks/Kernel.framework/PrivateHeaders
        ${CMAKE_OSX_SYSROOT}/System/Library/Frameworks/Kernel.framework/Headers
)
target_compile_definitions(PcmMsrDriver PRIVATE
        -DKERNEL
        -DKERNEL_PRIVATE
        -DDRIVER_PRIVATE
        -DAPPLE
        -DNeXT
)

target_link_libraries(PcmMsrDriver PRIVATE ${IOKIT_LIBRARY}
        "-lkmodc++"
        "-lkmod"
        "-lcc_kext"
        "-nostdlib"
        "-Xlinker -object_path_lto"
        "-Xlinker -export_dynamic"
        "-Xlinker -kext"
)

# PcmMsrDriver.kext is built here and located in 'build/bin'
set(LIB_EXT_PATH "/Library/Extensions")
install(TARGETS PcmMsrDriver DESTINATION "${LIB_EXT_PATH}/")
install(CODE "execute_process(COMMAND kextload ${LIB_EXT_PATH}/PcmMsrDriver.kext)")
