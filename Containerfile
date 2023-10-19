FROM docker.io/archlinux:base-devel-20231015.0.185077

RUN pacman --sync --noconfirm --refresh --sysupgrade 

RUN pacman --sync --noconfirm cmake
RUN pacman --sync --noconfirm ninja
RUN pacman --sync --noconfirm boost
RUN pacman --sync --noconfirm readline

COPY ./ /root

WORKDIR /root

RUN chmod u+x build.sh

RUN ./build.sh

RUN cmake --install build/ --config Release

ENTRYPOINT [ "PPshell" ]
