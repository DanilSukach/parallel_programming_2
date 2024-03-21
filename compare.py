import numpy as np
import logging

logger = logging.getLogger()
logger.setLevel("INFO")


def compare_result(path_to_first_matrix, path_to_second_matrix, path_to_result_matrix):
    """
    Функция сравнивает две матрицы, полученные в результате перемножения двух матриц
    data_file: данные для построения
    return: None
    """
    first_matrix = np.loadtxt(path_to_first_matrix, dtype=int, skiprows=1)
    second_matrix = np.loadtxt(path_to_second_matrix, dtype=int, skiprows=1)

    loaded_result_matrix = np.loadtxt(path_to_result_matrix, int)

    multiply = np.dot(first_matrix, second_matrix)

    if np.array_equal(multiply, loaded_result_matrix):
        logging.info("Matrices are equal")
    else:
        logging.info("Matrices are not equal")
