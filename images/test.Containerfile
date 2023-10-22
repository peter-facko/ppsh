FROM runtime as test

RUN pacman --sync --noconfirm python

WORKDIR /root

COPY tests/ /root/tests/

RUN python -m venv .venv/
ENV PATH="/root/.venv/bin:$PATH"

RUN pip install -r tests/requirements.txt

ENTRYPOINT [ "pytest", "tests" ]
