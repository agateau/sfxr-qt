echo_title() {
    echo "========================================="
    echo $*
    echo "========================================="
}

detect_os() {
    local out
    out=$(uname)

    case "$out" in
    Linux)
        OS="linux"
        ;;
    Darwin)
        OS="macos"
        ;;
    MINGW*)
        OS="windows"
        ;;
    *)
        echo "Unknown OS. uname printed '$out'"
        exit 1
        ;;
    esac
}

is_linux() {
    [ "$OS" = "linux" ]
}

is_macos() {
    [ "$OS" = "macos" ]
}

is_windows() {
    [ "$OS" = "windows" ]
}

has_command() {
    command -v "$1" > /dev/null 2>&1
}

die() {
    echo "$*" >&2
    exit 1
}

mkabsdir() {
    mkdir -p "$1"
    pushd $PWD > /dev/null
    cd "$1"
    echo $PWD
    pop > /dev/null
}

init_python_cmd() {
    echo_title "Looking for a Python 3 + pip installation"
    for interpreter in python3 python ; do
        if $interpreter -m pip --version 2> /dev/null ; then
            echo "Found $interpreter"
            export PYTHON_CMD=$interpreter
            return
        fi
    done
    die "Can't find a valid Python 3 installation."
}

init_run_as_root() {
    RUN_AS_ROOT=""
    if is_windows ; then
        return
    fi
    if [ $(id -u) = "0" ] ; then
        # Already root
        return
    fi
    if has_command sudo ; then
        RUN_AS_ROOT=sudo
    else
        RUN_AS_ROOT="su -c"
    fi
}

install_prebuilt_archive() {
    local url=$1
    local sha1=$2
    local download_file=$3
    local unpack_dir=$4

    echo "Downloading '$url'"
    curl --location --continue-at - --output "$download_file" "$url"

    echo "Checking integrity"
    echo "$sha1 $download_file" | sha1sum --check

    echo "Unpacking"
    (
        cd "$unpack_dir"
        case "$download_file" in
            *.zip)
                unzip -q "$download_file"
                ;;
            *.tar.gz|*.tar.bz2|*.tar.xz)
                tar xf "$download_file"
                ;;
            *)
                die "Don't know how to unpack $download_file"
                ;;
        esac
    )
}

detect_os
init_python_cmd
init_run_as_root

if is_macos ; then
    NPROC=$(sysctl -n hw.ncpu)
else
    NPROC=$(nproc)
fi
