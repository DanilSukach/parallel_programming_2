import argparse
from system_functions import load_settings
from compare import compare_result
from ploting import visualize_statistics
from table import create_and_save_table
import logging

logger = logging.getLogger()
logger.setLevel("INFO")
SETTINGS_FILE = "./settings.json"


def main():
    parser = argparse.ArgumentParser(description="Matrix Operations")
    parser.add_argument("-set", "--settings", default=SETTINGS_FILE,
                        type=str, help="Using your own settings file")
    parser.add_argument("--compare", action="store_true",
                        help="Compare results")
    parser.add_argument("--graph", action="store_true", help="Create graph")
    parser.add_argument("--table", action="store_true", help="Create table")

    args = parser.parse_args()
    settings = load_settings(args.settings)
    if settings:
        if args.compare:
            compare_result(settings["path_to_matrix_one"],
                           settings["path_to_matrix_two"], settings["path_to_matrix_result"])
        elif args.graph:
            visualize_statistics(
                settings["path_to_data"], settings["path_to_save"])
        elif args.table:
            create_and_save_table(
                settings["path_to_data"], settings["path_to_save"])


if __name__ == "__main__":
    main()
