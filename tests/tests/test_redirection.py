from distutils.file_util import write_file
from random import choice as random_choice
from string import ascii_lowercase

import pytest

from .utils import (
    are_shell_tasks_equivalent,
    are_shells_equivalent,
    execute_ppsh,
    execute_sh,
    make_random_string,
)


@pytest.mark.asyncio_cooperative
async def test_redirection_in(tmp_path_factory):
    tmp_path = tmp_path_factory.mktemp("in")

    with open(tmp_path / "file", "w") as f:
        f.write(make_random_string())

    assert await are_shells_equivalent(f"wc <{f.name}")


def are_file_same(file1, file2):
    with open(file1) as f1:
        with open(file2) as f2:
            return f1.read() == f2.read()


@pytest.mark.asyncio_cooperative
async def test_redirection_out(tmp_path_factory):
    tmp_path = tmp_path_factory.mktemp("out")

    random_string = make_random_string()

    ppsh_file_path = tmp_path / "ppsh"
    sh_file_path = tmp_path / "sh"

    make_command = lambda file_path: f"echo {random_string} >{file_path}"

    ppsh_task = await execute_ppsh(make_command(ppsh_file_path))
    sh_task = await execute_sh(make_command(sh_file_path))

    assert await are_shell_tasks_equivalent(sh_task, ppsh_task)

    assert are_file_same(ppsh_file_path, sh_file_path)


def write_file(path, content):
    with open(path, "w") as f:
        f.write(content)


@pytest.mark.asyncio_cooperative
async def test_redirection_append(tmp_path_factory):
    tmp_path = tmp_path_factory.mktemp("out")

    random_string = make_random_string()

    ppsh_file_path = tmp_path / "ppsh"
    sh_file_path = tmp_path / "sh"

    write_file(ppsh_file_path, random_string)
    write_file(sh_file_path, random_string)

    random_string_append = make_random_string()

    make_command = lambda file_path: f"echo {random_string_append} >>{file_path}"

    ppsh_task = await execute_ppsh(make_command(ppsh_file_path))
    sh_task = await execute_sh(make_command(sh_file_path))

    assert await are_shell_tasks_equivalent(sh_task, ppsh_task)

    assert are_file_same(ppsh_file_path, sh_file_path)
