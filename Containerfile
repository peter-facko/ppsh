FROM docker.io/archlinux:base-20231015.0.185077 as build

RUN pacman --sync --noconfirm --refresh --sysupgrade 

RUN pacman --sync --noconfirm gcc
RUN pacman --sync --noconfirm cmake
RUN pacman --sync --noconfirm ninja
RUN pacman --sync --noconfirm boost
RUN pacman --sync --noconfirm readline

COPY ./ /root

WORKDIR /root

RUN chmod u+x build.sh 

RUN ./build.sh ./ build/

RUN cmake --install build/ --config Release --prefix install/

FROM docker.io/archlinux:base-20231015.0.185077

RUN pacman --sync --noconfirm --refresh --sysupgrade 

RUN pacman --sync --noconfirm boost
RUN pacman --sync --noconfirm readline

# so there is something to try executing
RUN pacman --sync --noconfirm coreutils

COPY --from=build /root/install/ /usr/local/

ENTRYPOINT [ "PPshell" ]
