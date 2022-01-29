#
# Cozmonaut
# Copyright 2019 The Cozmonaut Contributors
#

from typing import Tuple


class Server:
    """
    This is a placeholder.
    """

    def __init__(self, robot_id: int, robot_serial: str):
        pass

    async def next_track(self):
        pass

    def push_motor_rate_left(self, value: float):
        pass

    def push_motor_rate_right(self, value: float):
        pass

    def push_accel(self, value: Tuple[float, float, float]):
        pass

    def push_gyro(self, value: Tuple[float, float, float]):
        pass

    def push_battery_voltage(self, value: float):
        pass

    def push_charging(self, value: bool):
        pass

    def push_relative_origin_id(self, value: int):
        pass

    def push_relative_position(self, value: Tuple[float, float, float]):
        pass

    def push_relative_rotation(self, value: Tuple[float, float, float, float]):
        pass
