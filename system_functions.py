import logging
import json

logger = logging.getLogger()
logger.setLevel("INFO")


def load_settings(settings_file: str) -> dict:
    """
    Функция считывает файл настроек
    :param settigs_file: название файла с настройками
    :return: настройки
    """
    settings = None
    try:
        with open(settings_file) as json_file:
            settings = json.load(json_file)
        logging.info(f"Settings read from file '{settings_file}'")
    except OSError as err:
        logging.warning(
            f"Error reading settings from file '{settings_file}'\n{err}")
    return settings
