AQTINSTALL_VERSION=0.9.0
AQTINSTALL_ARCHIVES="qtbase qtimageformats qtsvg qttranslations qttools"

setup_python_cmd() {
    echo_title "Looking for a Python 3 + pip installation"
    for interpreter in python3 python ; do
        if $interpreter -m pip --version 2> /dev/null ; then
            echo "Found $interpreter"
            add_env_var PYTHON_CMD $interpreter
            return
        fi
    done
    die "Can't find a valid Python 3 installation."
}

install_qt() {
    echo_title "Installing Qt"
    local qt_install_dir=$INST_DIR/qt
    local aqt_args
    if is_windows ; then
        aqt_args="windows desktop $QT_ARCH_WINDOWS"
    fi
    if is_macos ; then
        aqt_args="mac desktop"
    fi
    $PYTHON_CMD -m pip install aqtinstall==$AQTINSTALL_VERSION
    $PYTHON_CMD -m aqt install --outputdir $qt_install_dir $QT_VERSION $aqt_args --archives $AQTINSTALL_ARCHIVES
    if is_windows ; then
        # Add Qt bin dir to $PATH so that tests can find Qt dlls
        prepend_path $(find $qt_install_dir -type d -a -name bin)
    fi
    # Add Qt plugins dir to $QT_PLUGIN_PATH because the official Qt installer
    # patches QtCore dll so that it finds its plugins, but aqt does not.
    # Not being able to find plugins causes tests to not run on macOS and
    # Windows because they can't find the matching platform plugin.
    add_env_var QT_PLUGIN_PATH $(find $qt_install_dir -type d -a -name plugins)
    add_env_var Qt5_DIR $(find $qt_install_dir -path '*/lib/cmake')
}

install_cmake() {
    echo_title "Installing CMake"
    $PYTHON_CMD -m pip install cmake==$CMAKE_VERSION
}

install_ecm() {
    echo_title "Installing ECM"
    git clone --depth 1 https://anongit.kde.org/extra-cmake-modules.git -b v$ECM_VERSION
    (
        cd extra-cmake-modules
        mkdir build
        cd build
        cmake \
            -DCMAKE_INSTALL_PREFIX=$INST_DIR \
            -DBUILD_HTML_DOCS=OFF \
            -DBUILD_MAN_DOCS=OFF \
            -DBUILD_QTHELP_DOCS=OFF \
            -DBUILD_TESTING=OFF \
            ..
        cmake --build .
        cmake --build . --target install
    )
}

install_qpropgen() {
    echo_title "Installing qpropgen"
    $PYTHON_CMD -m pip install qpropgen==$QPROPGEN_VERSION
}
