import logging
import time

import pytest
from pytest_embedded_idf.dut import IdfDut

CHECK_COUNT = 100
EXPECTED_CYCLE_TIME = 1  # s
TOLERANCE_PERCENT = 10  # %


@pytest.mark.esp32
@pytest.mark.poc_app_performance
def test_adc_cycle_time(dut: IdfDut) -> None:
    measure_times = []
    for i in range(CHECK_COUNT):
        res = dut.expect(r'\[Measure\] Voltage: (\d+) mV')
        measure_times.append(time.perf_counter())

    # Calculate time differences between measured times
    time_diffs = [measure_times[i + 1] - measure_times[i] for i in range(len(measure_times) - 1)]

    # Calculate the average time difference and tolerance
    average_time_diff = sum(time_diffs) / len(time_diffs)
    tolerance = (TOLERANCE_PERCENT / 100) * EXPECTED_CYCLE_TIME

    # Log the measured times and differences
    logging.info("Measured Times: %s", measure_times)
    logging.info("Time Differences: %s", time_diffs)

    # Check if all time differences are within tolerance
    for idx, diff in enumerate(time_diffs):
        assert abs(
            diff - EXPECTED_CYCLE_TIME) <= tolerance, f"Measured cycle time ({diff:.2f} s) deviates from expected " \
                                                      f"({EXPECTED_CYCLE_TIME} s) by more than {TOLERANCE_PERCENT} % " \
                                                      f"(tolerance: {tolerance:.2f} s) at index {idx}."
