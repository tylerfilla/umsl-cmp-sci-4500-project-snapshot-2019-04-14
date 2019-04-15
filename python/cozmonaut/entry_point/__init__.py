#
# Cozmonaut
# Copyright 2019 The Cozmonaut Contributors
#

from abc import ABC, abstractmethod


class EntryPoint(ABC):
    """
    An entry point for some operation.
    """

    @abstractmethod
    def main(self, args: dict) -> int:
        """
        The main method.

        :param args: The entry arguments
        :return: The exit code (zero for success)
        """
        raise NotImplementedError
