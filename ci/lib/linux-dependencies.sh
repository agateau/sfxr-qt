main() {
    echo_title "Installing Linux packages"
    if has_command apt-get ; then
        $RUN_AS_ROOT apt-get update
        # file is needed by dpkg to generate shlib dependencies
        # xvfb and openbox are needed to run UI tests in headless environments
        $RUN_AS_ROOT apt-get install -y --no-install-recommends \
            clang-format-10 \
            dpkg-dev \
            extra-cmake-modules \
            file \
            g++ \
            git \
            libsdl1.2-dev \
            make \
            openbox \
            python3 \
            python3-pip \
            python3-setuptools \
            qtbase5-dev \
            qtdeclarative5-dev \
            qtquickcontrols2-5-dev \
            qttools5-dev \
            rpm \
            xvfb
    else
        die "Sorry, I don't know how to install the required packages on your distribution."
    fi

    setup_python_cmd
    install_cmake
    install_qpropgen
}
