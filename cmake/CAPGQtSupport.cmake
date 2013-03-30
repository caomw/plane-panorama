# parse files
macro(filterFilesContain pattern out_var in_var)
    set(${out_var})
    foreach(file_name ${${in_var}})
        set(z_filter_file_contain_content)
        file(READ ${file_name} z_filter_file_contain_content)
        if(z_filter_file_contain_content MATCHES ${pattern})
            list(APPEND ${out_var} ${file_name})
        endif(z_filter_file_contain_content MATCHES ${pattern})
    endforeach(file_name)
endmacro(filterFilesContain)
# --------------------------------------------------------------------------- #
# Qt support
macro(compileWithQt)
    #find_package(Qt4 COMPONENTS QtCore QtGui QtOpenGL QtSql QtSvg Phonon REQUIRED)
    find_package(Qt4)
    set(QT_USE_QTOPENGL 1)
    set(QT_USE_QTSQL 1)
    set(QT_USE_QTSVG 1)
    set(QT_USE_QTXML 1)
    include(${QT_USE_FILE})
endmacro(compileWithQt)

macro(doQtMOC sourceFileList moc_files_var)
    set(do_qt_moc_macro_files_to_moc)
    filterFilesContain(".*Q_OBJECT.*" do_qt_moc_macro_files_to_moc ${sourceFileList})
    QT4_WRAP_CPP(${moc_files_var} ${do_qt_moc_macro_files_to_moc})
    source_group("Qt MOC Generated" FILES ${${moc_files_var}})
endmacro(doQtMOC)

macro(linkWithQt projectName)
    target_link_libraries(${projectName} ${QT_LIBRARIES})
endmacro(linkWithQt)
