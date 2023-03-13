import sys


def print_stderr(error_string: str):
    sys.stderr.write(error_string)


def print_err(s_string: str):
    sys.stderr.write(f'{s_string}\n')


def print_err_and_exit(s_string: str, exit_code: int):
    sys.stderr.write(f'{s_string}\n')
    exit(exit_code)
