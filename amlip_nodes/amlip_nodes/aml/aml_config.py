"""Folders required for project."""

from pathlib import Path

"""Directory name for result file."""
RESULTS_FOLDER = './INTERCHANGE_FOLDER/RESULTS'


def checkFolders():
    """TODO comment."""
    for folder in RESULTS_FOLDER:
        Path(folder).mkdir(parents=True, exist_ok=True)
