FROM ubuntu:22.04

ARG USERNAME=ctf
ARG USER_UID=1000
ARG USER_GID=$USER_UID

SHELL ["/usr/bin/bash", "-o", "pipefail", "-c"]

RUN <<EOF
    apt-get update
    DEBIAN_FRONTEND=noninteractive
    apt-get install --fix-missing -y \
        bison \
        curl \
        file \
        flex \
        gcc \
        git \
        gperf \
        libffi-dev \
        libncurses-dev \
        libssl-dev \
        libxi6 \
        libxrender1 \
        libxtst6 \
        make \
        netcat-traditional \
        python2.7 \
        python3 \
        python3-pip \
        sudo \
        unzip \
        vim \
        wget \
        xxd
    JDK_INSTALLER="amazon-corretto-17-x64-linux-jdk.deb"
    curl -O -L https://corretto.aws/downloads/latest/${JDK_INSTALLER}
    apt install -y ./${JDK_INSTALLER}
    rm ${JDK_INSTALLER}
    rm -r /var/lib/apt/lists/*
EOF

# create non-root user account with sudo access
RUN <<EOF
  if [ "${USERNAME}" != "root" ]; then
    groupadd --gid ${USER_GID} ${USERNAME}
    useradd -s /usr/bin/bash --uid ${USER_UID} --gid ${USER_GID} -m ${USERNAME}
    echo "${USERNAME} ALL=(root) NOPASSWD:ALL" > /etc/sudoers.d/${USERNAME}
    chmod 0440 /etc/sudoers.d/${USERNAME}
  fi
EOF

# python is python2.7
RUN ln -s /usr/bin/python2.7 /usr/bin/python

# install pip2
RUN curl https://bootstrap.pypa.io/pip/2.7/get-pip.py --output get-pip.py \
    && python2.7 get-pip.py \
    && rm get-pip.py

USER ${USERNAME}
ENV PHOME "/home/${USERNAME}"
WORKDIR ${PHOME}

# See https://espressif-docs.readthedocs-hosted.com/projects/esp-idf/en/release-v3.1/get-started/linux-setup.html
RUN <<EOF
    # start with v3.1 since it has a requirements.txt file
    git clone -b release/v3.1 --recursive https://github.com/espressif/esp-idf.git
    pip2 install -r esp-idf/requirements.txt
    # switch to version used in the challenge
    git -C esp-idf checkout v3.1-rc2
    curl https://dl.espressif.com/dl/xtensa-esp32-elf-linux64-1.22.0-80-g6c4433a-5.2.0.tar.gz --output xtensa.tar.gz
    tar -xzf xtensa.tar.gz
    rm xtensa.tar.gz
EOF

# Install ghidra
RUN <<EOF
  export GHIDRA_REL="ghidra_11.1.2_PUBLIC_20240709.zip"
  curl -O -L https://github.com/NationalSecurityAgency/ghidra/releases/download/Ghidra_11.1.2_build/${GHIDRA_REL}
  unzip ${GHIDRA_REL}
  rm ${GHIDRA_REL}
EOF

# Install esp32knife
RUN <<EOF
  git clone https://github.com/BlackVS/esp32knife.git
  pip3 install -r ./esp32knife/requirements.txt
EOF

# Install 010 editor
RUN <<EOF
  curl -O -L https://www.sweetscape.com/download/010EditorLinux64Installer.tar.gz
  tar -zxvf 010EditorLinux64Installer.tar.gz
  rm 010EditorLinux64Installer.tar.gz
EOF

COPY --chown=${USERNAME} exploit.py ${PHOME}/
COPY --chown=${USERNAME} esp32-image-template.bt ${PHOME}/
COPY --chown=${USERNAME} hello_world_main.c ${PHOME}/esp-idf/examples/get-started/hello_world/main/
RUN pip3 install pwntools

ENV IDF_PATH="${PHOME}/esp-idf"
ENV PATH="${PATH}:${PHOME}/.local/bin"
ENV PATH="${PATH}:${PHOME}/xtensa-esp32-elf/bin"
ENV PATH="${PATH}:${IDF_PATH}/components/esptool_py/esptool"
ENV PATH="${PATH}:${IDF_PATH}/components/espcoredump"
ENV PATH="${PATH}:${IDF_PATH}/components/partition_table"
