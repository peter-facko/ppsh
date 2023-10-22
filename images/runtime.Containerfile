FROM docker.io/archlinux:base-20231015.0.185077 as base

RUN pacman --sync --noconfirm --refresh --sysupgrade


FROM base as build

RUN pacman --sync --noconfirm gcc
RUN pacman --sync --noconfirm cmake
RUN pacman --sync --noconfirm ninja
RUN pacman --sync --noconfirm boost
RUN pacman --sync --noconfirm readline

WORKDIR /root

COPY ./ /root

RUN chmod u+x build.sh 

RUN ./build.sh ./ build/

RUN cmake --install build/ --config Release --prefix install/


FROM base as runtime

RUN pacman --sync --noconfirm boost
RUN pacman --sync --noconfirm readline

COPY --from=build /root/install/ /usr/local/
