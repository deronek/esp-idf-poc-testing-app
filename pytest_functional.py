import logging

import pytest
from pytest_embedded_idf.dut import IdfDut

from utils import DacSetting, SwVariant, get_correct_dac_setting_commands, check_voltage_ok


@pytest.mark.esp32
@pytest.mark.poc_app_functional
def test_dac_setting(dut: IdfDut,
                     correct_dac_setting_command: str) -> None:
    command = correct_dac_setting_command
    setting = DacSetting(command)
    logging.info(f'{command=}')

    dut.write(command)
    res = dut.expect(r'\[Setting\] DAC setting set: (\d+)')
    num = res.group(1).decode('utf8')
    assert num == command

    res = dut.expect(r'\[Measure\] Voltage: (\d+) mV')
    voltage = int(res.group(1).decode('utf8'))
    assert check_voltage_ok(setting, voltage)


@pytest.mark.esp32
@pytest.mark.poc_app_functional
def test_dac_setting_negative(dut: IdfDut,
                              incorrect_command: str) -> None:
    command = incorrect_command
    logging.info(f'{command=}')

    dut.write(command)
    res = dut.expect(r"\[Command\] Incorrect command provided: ([0-9a-zA-Z]+)")
    char = res.group(1).decode('utf8')
    assert char == command


@pytest.mark.esp32
@pytest.mark.poc_app_functional
def test_dac_setting_query(dut: IdfDut,
                           status_command: str) -> None:
    dut.write(status_command)
    res = dut.expect(r'\[Setting\] Current DAC setting: (\d+)')
    num = res.group(1).decode('utf8')
    assert num in get_correct_dac_setting_commands()


@pytest.mark.esp32
@pytest.mark.poc_app_functional
def test_variant_query(variant_command: str,
                       dut: IdfDut) -> None:
    dut.write(variant_command)
    res = dut.expect(r'\[Variant\] SW variant: ([A-Z])')
    variant = SwVariant(res.group(1).decode('utf8'))
    logging.info(f'SW Variant: {variant}')
