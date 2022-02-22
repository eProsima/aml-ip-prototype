"""Folders required for project."""

from pathlib import Path

"""Directory name for result file."""
RESULTS_FOLDER = './INTERCHANGE_FOLDER/RESULTS'


def checkFolders(folder_name):
    """TODO comment."""
    Path(folder_name).mkdir(parents=True, exist_ok=True)
