FROM ubuntu:18.04

RUN apt-get update \
    && apt-get install -y -qq --no-install-recommends \
        cmake \
        dpkg-dev \
        file \
        g++ \
        libqt5quickcontrols2-5 \
        libsdl1.2-dev \
        make \
        ninja-build \
        python3 \
        python3-pip \
        python3-setuptools \
        qt5-default \
        qtbase5-dev \
        qtdeclarative5-dev \
        qttools5-dev \
        rpm \
        xvfb

COPY requirements.txt /tmp

RUN pip3 install -r /tmp/requirements.txt

ENTRYPOINT ["/bin/bash"]
