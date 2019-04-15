#
# Cozmonaut
# Copyright 2019 The Cozmonaut Contributors
#

from cozmonaut.entry_point import EntryPoint


class EntryPointFriendList(EntryPoint):
    """
    The entry point for listing friends.
    """

    def main(self, args: dict) -> int:
        """
        The main method.

        This entry point takes the following arguments:
         - friend_id: int - The target friend ID (optional)
         - sql_addr: str - The server address
         - sql_user: str - The login username
         - sql_pass: str - The login password
         - sql_data: str - The database name

        If a friend ID is given, we list details just about that person. Imagine
        the Unix "ls -l" command, but with the data we have about people.

        If a friend ID is not given, we list details about all people.

        :param args: The arguments as specified above
        :return: The exit code (zero for success)
        """

        # TODO: If friend_id is set, list details for that friend
        # TODO: If friend_id is not set, list details for ALL friends

        print('friend_list')
        return 0
