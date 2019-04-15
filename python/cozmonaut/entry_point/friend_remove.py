#
# Cozmonaut
# Copyright 2019 The Cozmonaut Contributors
#

from cozmonaut.entry_point import EntryPoint


class EntryPointFriendRemove(EntryPoint):
    """
    The entry point for removing friends.
    """

    def main(self, args: dict) -> int:
        """
        The main method.

        This entry point takes the following arguments:
         - friend_id: int - The target friend ID
         - sql_addr: str - The server address
         - sql_user: str - The login username
         - sql_pass: str - The login password
         - sql_data: str - The database name

        :param args: The arguments as specified above
        :return: The exit code (zero for success)
        """

        # TODO: Delete the friend specified by friend_id

        print('friend_remove')
        return 0
