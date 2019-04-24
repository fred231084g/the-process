/* psygnal._psygnal module implementation */

/* Implements low-level access to psiginfo for printing full descriptions of
   siginfo_t structs. */

#include <Python.h>

#include <signal.h>
#include <stdio.h>
#include <sys/types.h>

static int addr_converter(PyObject *obj, void **address) {
  *address = PyLong_AsVoidPtr(obj);
  return (PyErr_Occurred() == NULL ? 1 : 0);
}

PyDoc_STRVAR(
    psygnal_base_psiginfo_impl__doc__,
    "_base_psiginfo_impl($module, signum, message=None, si_code=0, \n"
    "                    si_errno=0, si_pid=0, si_uid=0, si_status=0)\n"
    "--\n"
    "\n"
    "Print base signal information to sys.stderr.\n"
    "\n"
    "  signum\n"
    "    The signal number whose information is to be printed.\n"
    "  message\n"
    "    If this parameter is None or empty, the function simply returns the\n"
    "    system description for the specified signal. Otherwise, the return\n"
    "    value will begin with this message followed by the string ': '\n"
    "  si_code, si_errno, si_pid, si_uid, si_status\n"
    "    Values for the fields of a siginfo_t struct required in signal.h\n"
    "    per the POSIX standard (beginning with POSIX.1b-1993).\n"
    "\n"
    "Return values are e.g. \"<message>: Interrupt Signal sent by kill()\",\n"
    "\"Segmentation fault Signal sent by the kernel [0x0badf00d]\", etc.\n"
    "Prints \"Unknown signal <signum>\" if the signal is not recognized.");

#define PSYGNAL_BASE_PSIGINFO_IMPL_METHODDEF                                   \
  {                                                                            \
    "_base_psiginfo_impl", (PyCFunction)psygnal_base_psiginfo_impl,            \
        METH_VARARGS | METH_KEYWORDS, psygnal_base_psiginfo_impl__doc__        \
  }

static PyObject *psygnal_base_psiginfo_impl(PyObject *self, PyObject *args,
                                            PyObject *kw) {
  siginfo_t siginfo = {0};
  const char *message = NULL;

  static char *keywords[] = {"signum", "message", "si_code",   "si_errno",
                             "si_pid", "si_uid",  "si_status", NULL};

  /* gdb types should not be larger than native types */
  if (!PyArg_ParseTupleAndKeywords(
          args, kw, "i|z$ii" _Py_PARSE_PID _Py_PARSE_PID "i", keywords,
          &siginfo.si_signo, &message, &siginfo.si_code, &siginfo.si_errno,
          &siginfo.si_pid, &siginfo.si_uid, &siginfo.si_status))
    return NULL;

  psiginfo(&siginfo, message);

  Py_RETURN_NONE;
}

PyDoc_STRVAR(
    psygnal_fault_psiginfo_impl__doc__,
    "_fault_psiginfo_impl($module, signum, message=None, si_code=0,\n"
    "                     si_errno=0, si_addr=0, si_addr_lsb=0, si_lower=0,\n"
    "                     si_upper=0)\n"
    "--\n"
    "\n"
    "Print fault signal information to sys.stderr.\n"
    "\n"
    "  signum\n"
    "    The signal number whose information is to be printed. Must be\n"
    "    SIGILL, SIGFPE, SIGSEGV, or SIGBUS.\n"
    "  message\n"
    "    If this parameter is None or empty, the function simply returns the\n"
    "    system description for the specified signal. Otherwise, the return\n"
    "    value will begin with this message followed by the string ': '\n"
    "  si_code, si_errno, si_addr, si_addr_lsb, si_lower, si_upper\n"
    "    Values for the fields of a siginfo_t struct required in signal.h\n"
    "    per the POSIX standard (beginning with POSIX.1b-1993).\n"
    "\n"
    "Return values are e.g. \"<message>: Interrupt Signal sent by kill()\",\n"
    "\"Segmentation fault Signal sent by the kernel [0x0badf00d]\", etc.\n"
    "Prints \"Unknown signal <signum>\" if the signal is not recognized.");

#define PSYGNAL_FAULT_PSIGINFO_IMPL_METHODDEF                                  \
  {                                                                            \
    "_fault_psiginfo_impl", (PyCFunction)psygnal_fault_psiginfo_impl,          \
        METH_VARARGS | METH_KEYWORDS, psygnal_fault_psiginfo_impl__doc__       \
  }

static PyObject *psygnal_fault_psiginfo_impl(PyObject *self, PyObject *args,
                                             PyObject *kw) {
  siginfo_t siginfo = {0};
  const char *message = NULL;

  static char *keywords[] = {"signum",   "message",  "si_code",
                             "si_errno", "si_addr",  "si_addr_lsb",
                             "si_lower", "si_upper", NULL};

  /* gdb types should not be larger than native types */
  if (!PyArg_ParseTupleAndKeywords(
          args, kw, "i|z$iiO&hO&O&", keywords, &siginfo.si_signo, &message,
          &siginfo.si_code, &siginfo.si_errno, addr_converter, &siginfo.si_addr,
          &siginfo.si_addr_lsb, addr_converter, &siginfo.si_lower,
          addr_converter, &siginfo.si_upper))
    return NULL;

  psiginfo(&siginfo, message);

  Py_RETURN_NONE;
}

PyDoc_STRVAR(
    psygnal_poll_psiginfo_impl__doc__,
    "_poll_psiginfo_impl($module, signum, message=None, si_code=0,\n"
    "                    si_errno=0, si_band=0, si_fd=0)\n"
    "--\n"
    "\n"
    "Print poll signal information to sys.stderr.\n"
    "\n"
    "  signum\n"
    "    The only acceptable value here is SIGPOLL.\n"
    "  message\n"
    "    If this parameter is None or empty, the function simply returns the\n"
    "    system description for the specified signal. Otherwise, the return\n"
    "    value will begin with this message followed by the string ': '\n"
    "  si_code, si_errno, si_band, si_fd\n"
    "    Values for the fields of a siginfo_t struct required in signal.h\n"
    "    per the POSIX standard (beginning with POSIX.1b-1993).\n"
    "\n"
    "Return values are e.g. \"<message>: Interrupt Signal sent by kill()\",\n"
    "\"Segmentation fault Signal sent by the kernel [0x0badf00d]\", etc.\n"
    "Prints \"Unknown signal <signum>\" if the signal is not recognized.");

#define PSYGNAL_POLL_PSIGINFO_IMPL_METHODDEF                                   \
  {                                                                            \
    "_poll_psiginfo_impl", (PyCFunction)psygnal_poll_psiginfo_impl,            \
        METH_VARARGS | METH_KEYWORDS, psygnal_poll_psiginfo_impl__doc__        \
  }

static PyObject *psygnal_poll_psiginfo_impl(PyObject *self, PyObject *args,
                                            PyObject *kw) {
  siginfo_t siginfo = {0};
  const char *message = NULL;

  static char *keywords[] = {"signum",  "message", "si_code", "si_errno",
                             "si_band", "si_fd",   NULL};

  /* gdb types should not be larger than native types */
  if (!PyArg_ParseTupleAndKeywords(args, kw, "i|z$iili", keywords,
                                   &siginfo.si_signo, &message,
                                   &siginfo.si_code, &siginfo.si_errno,
                                   &siginfo.si_band, &siginfo.si_fd))
    return NULL;

  psiginfo(&siginfo, message);

  Py_RETURN_NONE;
}

/* List of functions defined in the module */

static PyMethodDef psygnal_methods[] = {
    PSYGNAL_BASE_PSIGINFO_IMPL_METHODDEF,
    PSYGNAL_FAULT_PSIGINFO_IMPL_METHODDEF,
    PSYGNAL_POLL_PSIGINFO_IMPL_METHODDEF,
    {NULL, NULL} /* sentinel */
};

PyDoc_STRVAR(module_doc,
             "This module provides full access to psiginfo in Python.\n\
\n\
Functions:\n\
\n\
_base_psiginfo_impl() -- print base signal information to sys.stderr\n\
_fault_psiginfo_impl() -- print poll signal information to sys.stderr\n\
_poll_psiginfo_impl() -- print poll signal information to sys.stderr");

static struct PyModuleDef psygnalmodule = {PyModuleDef_HEAD_INIT,
                                           "_psygnal",
                                           module_doc,
                                           0,
                                           psygnal_methods,
                                           NULL,
                                           NULL,
                                           NULL,
                                           NULL};

PyMODINIT_FUNC PyInit__psygnal(void) {
  PyObject *module = PyModule_Create(&psygnalmodule);

  return module;
}
