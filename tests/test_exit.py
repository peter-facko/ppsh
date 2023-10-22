from asyncio import create_subprocess_exec
from asyncio.subprocess import DEVNULL
from random import randint

import pytest


@pytest.mark.asyncio_cooperative
async def test_exit():
    exit_argument = randint(0, 255)

    command = f"exit {exit_argument}"

    return_code = await (
        await create_subprocess_exec(
            "PPshell",
            "--command",
            command,
            stdin=DEVNULL,
            stdout=DEVNULL,
            stderr=DEVNULL,
        )
    ).wait()

    assert return_code == exit_argument
