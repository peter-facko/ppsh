FROM runtime as deploy

RUN pacman --sync --noconfirm coreutils

ENTRYPOINT [ "PPshell" ]
