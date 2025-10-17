from glob import glob
from pathlib import Path
from time import sleep, time
from typing import Optional

import serial
from SCons.Script import Import

Import("env")

try:
    BOOTSEL_TIMEOUT = float(env.GetProjectOption("custom_bootsel_timeout", 6))
except TypeError:
    BOOTSEL_TIMEOUT = 6.0

MOUNT_PATH = Path(env.subst("$UPLOAD_PORT"))


def _resolve_serial_port() -> Optional[str]:
    try:
        configured = env.GetProjectOption("custom_bootsel_serial", "")
    except TypeError:
        configured = ""

    if configured:
        candidate = env.subst(configured)
        if Path(candidate).exists():
            return candidate

    for pattern in ("/dev/ttyACM*", "/dev/ttyUSB*"):
        matches = sorted(glob(pattern))
        for match in matches:
            if Path(match).exists():
                return match

    return None


def _wait_for_mount_state(should_exist: bool, timeout: float) -> bool:
    deadline = time() + timeout
    while time() < deadline:
        exists = MOUNT_PATH.is_dir()
        if exists == should_exist:
            return True
        sleep(0.1)
    return MOUNT_PATH.is_dir() == should_exist


def trigger_bootsel(target, source, env):
    serial_port = _resolve_serial_port()

    if not serial_port:
        print("[auto_bootsel] Warning: no CDC serial port available; skipping BOOTSEL trigger.")
        return

    try:
        with serial.Serial(serial_port, 115200, timeout=0.2) as port:
            port.write(b"::BOOTSEL::\n")
            port.flush()
    except Exception as exc:  # pylint: disable=broad-except
        print(f"[auto_bootsel] Warning: unable to request BOOTSEL on {serial_port}: {exc}")
        return

    # Allow current filesystem to detach first.
    _wait_for_mount_state(should_exist=False, timeout=BOOTSEL_TIMEOUT)

    if not _wait_for_mount_state(should_exist=True, timeout=BOOTSEL_TIMEOUT):
        raise RuntimeError(
            f"[auto_bootsel] Timed out waiting for BOOTSEL drive to appear at {MOUNT_PATH}"
        )

    sleep(0.5)


env.AddPreAction("upload", trigger_bootsel)
