#
# Cozmonaut
# Copyright 2019 The Cozmonaut Contributors
#

import asyncio

import cozmo

import base
from cozmonaut.entry_point import EntryPoint


class EntryPointInteract(EntryPoint):
    """
    The entry point for actual robot interaction.
    """

    def __init__(self):
        self.stopping = False

    async def face_handler(self, robot: cozmo.robot.Robot, server: base.Server, track: base.FaceTrack):
        ident = await track.identify()

    async def face_loop(self, robot: cozmo.robot.Robot, server: base.Server):
        """
        Continually watch for new face tracks.

        :param robot: The Cozmo robot
        :param server: The server
        """

        while not self.stopping:
            # Wait for a new face track
            track = await server.next_track()

            await self.face_handler()

            # Yield control
            await asyncio.sleep(0)

    async def sensor_monitor(self, robot: cozmo.robot.Robot, server: base.Server):
        """
        This is a coroutine that monitors Cozmo sensor data. Specifically, it
        polls the sensors and pushes values to the server.

        @param robot The Cozmo robot
        @param server The server
        """

        while not self.stopping:
            # Actuator feedback senses
            server.push_motor_rate_left(robot.left_wheel_speed.speed_mmps)
            server.push_motor_rate_right(robot.right_wheel_speed.speed_mmps)

            # Motion senses
            server.push_accel(robot.accelerometer.x_y_z)
            server.push_gyro(robot.gyro.x_y_z)

            # Power senses
            server.push_battery_voltage(robot.battery_voltage)
            server.push_charging(robot.is_charging)

            # Computed position and rotation
            # These are relative to the Cozmo app's internal map
            server.push_relative_origin_id(robot.pose.origin_id)
            server.push_relative_position(robot.pose.position.x_y_z)
            server.push_relative_rotation(robot.pose.rotation.q0_q1_q2_q3)  # Quaternion wrt. current origin

            # Yield control
            await asyncio.sleep(0)

    async def robot_main(self, conn: cozmo.conn.CozmoConnection):
        """
        This is the main coroutine for a robot.

        :param conn The Cozmo SDK connection
        :return:
        """

        # Get its robot interface
        robot: cozmo.robot.Robot = await conn.wait_for_robot()

        # Create server instance for this robot
        server = base.Server(robot_id=robot.robot_id, robot_serial=robot.serial)

        # Built a list of tasks to run concurrently for this robot
        tasks = []
        tasks += self.face_loop(robot, server)
        tasks += self.sensor_monitor(robot, server)

        # Wait on all of them
        await asyncio.gather(tasks)

    def main(self, args: dict) -> int:
        """
        The main method.

        This entry point takes the following arguments:
         - sql_addr: str - The server address
         - sql_user: str - The login username
         - sql_pass: str - The login password
         - sql_data: str - The database name

        :param args: The arguments as specified above
        :return: The exit code (zero for success)
        """

        # Get event loop for this thread
        loop = asyncio.get_event_loop()

        # TODO: Set up these things for each connected Cozmo
        #   Right now, only one Cozmo is dealt with

        # Connect to first available app
        conn = cozmo.connect_on_loop(loop)

        # Run the Cozmo main function
        loop.run_until_complete(asyncio.ensure_future(self.robot_main(conn)))
        return 0


# Don't drive off the charger as soon as we connect
# This would cause both Cozmos to drive forward on program start
cozmo.robot.Robot.drive_off_charger_on_connect = False
