import pytest

from .utils import are_shells_equivalent, make_random_string


@pytest.mark.asyncio_cooperative
async def test_pipe():
    assert await are_shells_equivalent(f"echo {make_random_string()} | cat -")
