import logging
import random
import time

import pytest
from pytest_embedded_idf.dut import IdfDut

from utils import get_correct_dac_setting_commands, get_incorrect_commands, generate_random_string

MIN_BURSTS_MULTIPLE = 5
MAX_BURSTS_MULTIPLE = 10

MIN_CHAR_PER_BURST_MULTIPLE = 10
MAX_CHAR_PER_BURST_MULTIPLE = 50

MIN_CHAR_PER_BURST_BIG = 750
MAX_CHAR_PER_BURST_BIG = 1500

MIN_WAIT_AFTER_BURST = 0.1  # s
MAX_WAIT_AFTER_BURST = 0.5  # s


@pytest.mark.esp32
@pytest.mark.poc_app_fuzzing
@pytest.mark.parametrize('count', range(10))
def test_adc_setting_fuzzing_burst_big(dut: IdfDut,
                                       count: int,
                                       status_command: str) -> None:
    burst = generate_random_string(
        get_incorrect_commands(),
        random.randint(MIN_CHAR_PER_BURST_BIG, MAX_CHAR_PER_BURST_BIG))

    dut.write(burst)
    logging.info(f'{burst=}')

    time.sleep(1)
    dut.write(status_command)

    res = dut.expect(r'\[Setting\] Current DAC setting: (\d+)')
    num = res.group(1).decode('utf8')
    assert num in get_correct_dac_setting_commands()


@pytest.mark.esp32
@pytest.mark.poc_app_fuzzing
@pytest.mark.parametrize('count', range(10))
def test_adc_setting_fuzzing_multiple_bursts(dut: IdfDut,
                                             count: int,
                                             status_command: str) -> None:
    num_of_write_bursts = random.randint(MIN_BURSTS_MULTIPLE, MAX_BURSTS_MULTIPLE)
    write_bursts_chars = [generate_random_string(
        get_incorrect_commands(),
        random.randint(MIN_CHAR_PER_BURST_MULTIPLE, MAX_CHAR_PER_BURST_MULTIPLE))
        for _ in range(num_of_write_bursts)]
    for burst in write_bursts_chars:
        dut.write(burst)
        logging.info(f'{burst=}')

        # Wait for a random period between 100 ms and 500 ms
        wait_time = random.uniform(MIN_WAIT_AFTER_BURST, MAX_WAIT_AFTER_BURST)
        time.sleep(wait_time)
    dut.write(status_command)

    res = dut.expect(r'\[Setting\] Current DAC setting: (\d+)')
    num = res.group(1).decode('utf8')
    assert num in get_correct_dac_setting_commands()
