r"""
 * Smoke test
 * Based on ESP-IDF example:
 * - get_started\hello_world
 * https://github.com/espressif/esp-idf
 * 
 * License of that example:
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 * # SPDX-License-Identifier: CC0-1.0
"""
import hashlib
import logging

import pytest
from pytest_embedded_idf import IdfApp
from pytest_embedded_idf.dut import IdfDut


def verify_elf_sha256_embedding(app: IdfApp, sha256_reported: str) -> None:
    sha256 = hashlib.sha256()
    with open(app.elf_file, 'rb') as f:
        sha256.update(f.read())
    sha256_expected = sha256.hexdigest()

    logging.info(f'ELF file SHA256: {sha256_expected}')
    logging.info(f'ELF file SHA256 (reported by the app): {sha256_reported}')

    # the app reports only the first several hex characters of the SHA256, check that they match
    if not sha256_expected.startswith(sha256_reported):
        raise ValueError('ELF file SHA256 mismatch')


@pytest.mark.esp32
@pytest.mark.poc_app_smoke
def test_hello_world_host(app: IdfApp, dut: IdfDut) -> None:
    dut.expect(r'Project name:\s+esp-idf-poc-testing-app')
    sha256_reported = (
        dut.expect(r'ELF file SHA256:\s+([a-f0-9]+)').group(1).decode('utf-8')
    )
    verify_elf_sha256_embedding(app, sha256_reported)
