"""A Python psiginfo wrapper
psygnal provides access to the standard POSIX call `psiginfo`
"""
__version__ = '1.0.0'
__all__ = [
    'psiginfo', 'strsiginfo',
]

__author__ = 'Jason Petersen <Jason.Petersen@microsoft.com>'

from psygnal._psygnal import (
    _fault_psiginfo_impl,
    _poll_psiginfo_impl,
    _base_psiginfo_impl
)
from signal import Signals
from io import StringIO
from sys import __stderr__
from os import dup, dup2, pipe
from contextlib import ExitStack


def psiginfo(signalnum, message=None, **kwargs):
    common_fields = frozenset(['signum', 'message', 'si_code', 'si_errno'])

    fault_fields = common_fields | frozenset(
        ['si_addr',  'si_addr_lsb', 'si_lower', 'si_upper'])
    poll_fields = common_fields | frozenset(['si_band', 'si_fd'])
    base_fields = common_fields | frozenset(['si_pid', 'si_uid', 'si_status'])

    kwargs['signum'] = signalnum

    if signalnum in frozenset([Signals.SIGILL, Signals.SIGFPE, Signals.SIGSEGV, Signals.SIGBUS]):
        argnames = fault_fields
        impl = _fault_psiginfo_impl
    elif signalnum == Signals.SIGPOLL:
        argnames = poll_fields
        impl = _poll_psiginfo_impl
    else:
        argnames = base_fields
        impl = _base_psiginfo_impl

    kwargs = {key: val for key, val in kwargs.items() if key in argnames}
    impl(**kwargs)


def strsiginfo(signalnum, message=None, **kwargs):
    stderr_fd = sys.__stderr__.fileno()
    r_fd, w_fd = os.pipe()

    with ExitStack() as stack:
        pipe_out = stack.enter_context(open(r_fd, mode='r'))
        pipe_in = stack.enter_context(open(w_fd, mode='w'))

        dup_stderr_fd = os.dup(stderr_fd)
        stack.enter_context(open(dup_stderr_fd, mode='w'))

        sys.__stderr__.flush()
        os.dup2(w_fd, stderr_fd)
        stack.callback(os.dup2, dup_stderr_fd, stderr_fd)

        psiginfo(signalnum, message, **kwargs)

        os.dup2(dup_stderr_fd, stderr_fd)

        pipe_in.flush()
        pipe_in.close()

        retval = pipe_out.read()

    return retval
