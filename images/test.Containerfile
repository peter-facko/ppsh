FROM runtime as test

RUN pacman --sync --noconfirm python

WORKDIR /root

RUN python -m venv .venv/
ENV PATH="/root/.venv/bin:$PATH"

COPY tests/requirements.txt /root/tests/requirements.txt

RUN pip install -r tests/requirements.txt

COPY tests/ /root/tests/tests/

ENTRYPOINT [ "pytest", "tests/tests" ]
