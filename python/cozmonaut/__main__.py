#
# Cozmonaut
# Copyright 2019 The Cozmonaut Contributors
#

from cozmonaut.entry_point.friend_list import EntryPointFriendList
from cozmonaut.entry_point.friend_remove import EntryPointFriendRemove
from cozmonaut.entry_point.interact import EntryPointInteract


def test_friend_list():
    """
    Test the "list" entry point.
    """

    test_args_1 = {
        'sql_host': 'hostname',
        'sql_user': 'username',
        'sql_pass': 'password',
        'sql_data': 'database',
    }

    test_args_1 = {
        'friend_id': 100,
        'sql_host': 'hostname',
        'sql_user': 'username',
        'sql_pass': 'password',
        'sql_data': 'database',
    }

    ep = EntryPointFriendList()
    ep.main(test_args_1)


def test_friend_remove():
    """
    Test the "remove" entry point.
    """

    args = {
        'friend_id': 100,
        'sql_host': 'hostname',
        'sql_user': 'username',
        'sql_pass': 'password',
        'sql_data': 'database',
    }

    ep = EntryPointFriendRemove()
    ep.main(args)


def test_interact():
    """
    Test the "interact" entry point.
    """

    args = {
        'friend_id': 100,
        'sql_host': 'hostname',
        'sql_user': 'username',
        'sql_pass': 'password',
        'sql_data': 'database',
    }

    ep = EntryPointInteract()
    ep.main(args)


if __name__ == '__main__':
    # TODO: Uncomment the one you want
    # test_friend_list()
    # test_friend_remove()
    test_interact()
