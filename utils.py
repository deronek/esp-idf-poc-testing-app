import functools
import math
import os
import random
import string
from enum import Enum


def is_jenkins() -> bool:
    return os.getenv('JENKINS_URL') is not None


def get_idf_path() -> str:
    return os.getenv("IDF_PATH")


class DacSetting(Enum):
    SETTING_1 = '1'
    SETTING_2 = '2'
    SETTING_3 = '3'


class SwVariant(Enum):
    VARIANT_A = 'A'
    VARIANT_B = 'B'
    VARIANT_C = 'C'
    VARIANT_D = 'D'


class HwVariant(Enum):
    VARIANT_ALPHA = 'VARIANT_ALPHA'
    VARIANT_BETA = 'VARIANT_BETA'


MAX_VOLTAGE = 1.0  # V


@functools.cache
def get_sw_variant() -> SwVariant:
    variant_str = os.getenv('SW_VARIANT')
    variant = SwVariant[variant_str]
    return variant


@functools.cache
def get_hw_variant() -> HwVariant:
    variant_str = os.getenv('HW_VARIANT')
    variant = HwVariant[variant_str]
    return variant


def get_expected_voltage(setting: DacSetting, hw_variant: HwVariant) -> float:
    voltage = MAX_VOLTAGE
    match setting:
        case DacSetting.SETTING_1:
            voltage *= 1 / 4
        case DacSetting.SETTING_2:
            voltage *= 2 / 4
        case DacSetting.SETTING_3:
            voltage *= 3 / 4
        case _:
            raise ValueError('Incorrect DacSetting provided')
    match hw_variant:
        case HwVariant.VARIANT_ALPHA:
            pass
        case HwVariant.VARIANT_BETA:
            voltage *= 1 / 2
        case _:
            raise ValueError('Incorrect HwVariant provided')
    return voltage


def generate_random_string(source: str, length: int) -> str:
    return ''.join(random.choice(source) for _ in range(length))


def check_voltage_ok(setting: DacSetting, measured_voltage: float) -> bool:
    # Conversion from mV to V
    measured_voltage /= 1000
    expected_voltage = get_expected_voltage(setting, get_hw_variant())
    return math.isclose(measured_voltage, expected_voltage, rel_tol=0.25, abs_tol=0)


@functools.cache
def get_correct_dac_setting_commands() -> str:
    return '123'


@functools.cache
def get_status_command() -> str:
    return 's'


@functools.cache
def get_variant_command() -> str:
    return 'v'


@functools.cache
def get_all_query_commands() -> str:
    commands = get_status_command() \
               + get_variant_command()
    return commands


@functools.cache
def get_incorrect_commands() -> str:
    correct_dac_commands = get_correct_dac_setting_commands()
    query_commands = get_all_query_commands()

    all_characters = string.digits + string.ascii_lowercase

    incorrect_characters = [char for char in all_characters if
                            char not in correct_dac_commands and char not in query_commands]

    return ''.join(incorrect_characters)
