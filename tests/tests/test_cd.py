import pytest

from .utils import are_shells_equivalent


@pytest.mark.asyncio_cooperative
async def test_root():
    assert await are_shells_equivalent("cd /; pwd")


@pytest.mark.asyncio_cooperative
async def test_home():
    assert await are_shells_equivalent("cd; pwd")


@pytest.mark.asyncio_cooperative
async def test_old():
    assert await are_shells_equivalent("cd; cd /; cd -; pwd")
