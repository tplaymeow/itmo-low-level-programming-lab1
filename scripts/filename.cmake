function(define_filename_for_sources targetname)
    get_target_property(source_files "${targetname}" SOURCES)
    foreach(sourcefile ${source_files})
        get_filename_component(basename "${sourcefile}" NAME)
        set_property(
                SOURCE "${sourcefile}" APPEND
                PROPERTY COMPILE_DEFINITIONS "FILENAME=\"${basename}\"")
    endforeach()
endfunction()