set(SkinName siteskin)
set(TemplateFiles master.tmpl page.tmpl news.tmpl intro.tmpl)
set(GeneratedCode "${CMAKE_CURRENT_BINARY_DIR}/${SkinName}.cpp")

add_custom_command(OUTPUT ${GeneratedCode}
                   COMMAND cppcms_tmpl_cc ${TemplateFiles} -o "${GeneratedCode}"
                   WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(${SkinName} SHARED ${GeneratedCode})
install(TARGETS ${SkinName}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
