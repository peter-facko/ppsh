from asyncio import create_subprocess_exec
from asyncio.subprocess import DEVNULL, PIPE
from collections import namedtuple
from random import choice as random_choice
from random import randint
from string import ascii_lowercase

ExecuteResult = namedtuple("ExecuteResult", ["return_code", "stdout"])


class ProcessResult:
    def __init__(self, process) -> None:
        self.process = process

    async def __call__(self, stdin: str | None = None) -> [int, str]:
        stdin_encoded = stdin.encode("ascii") if stdin is not None else None

        stdout = (await self.process.communicate(stdin_encoded))[0].decode("ascii")

        return ExecuteResult(await self.process.wait(), stdout)


async def execute_command(command, *args):
    return ProcessResult(
        await create_subprocess_exec(
            command,
            *args,
            stdin=PIPE,
            stdout=PIPE,
            stderr=DEVNULL,
        )
    )


async def execute_PPshell(command):
    return await execute_command("PPshell", "--command", command)


async def execute_sh(command):
    return await execute_command("sh", "-c", command)


async def are_shell_tasks_equivalent(sh_task, PPshell_task, stdin=None):
    # We assign to variables for `pytest --showlocals` to show the values.
    sh_result = await sh_task(stdin)
    PPshell_result = await PPshell_task(stdin)

    return sh_result == PPshell_result


async def are_shells_equivalent(command, stdin=None):
    sh_task = await execute_sh(command)
    PPshell_task = await execute_PPshell(command)

    return await are_shell_tasks_equivalent(sh_task, PPshell_task, stdin)


def make_random_string():
    return "".join(random_choice(ascii_lowercase) for _ in range(randint(5, 20)))
