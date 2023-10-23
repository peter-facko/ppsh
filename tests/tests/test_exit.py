from random import randint

import pytest

from .utils import are_shells_equivalent


@pytest.mark.parametrize("value", [randint(0, 255) for _ in range(3)])
@pytest.mark.asyncio_cooperative
async def test_explicit(value):
    assert await are_shells_equivalent(f"exit {value}")


@pytest.mark.parametrize("command", ["false", "true", "ls", "echo"])
@pytest.mark.asyncio_cooperative
async def test_last(command):
    assert await are_shells_equivalent(f"{command}; exit")
