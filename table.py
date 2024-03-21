import json
import os
import logging
import pandas as pd
import dataframe_image as dfi
import numpy as np

logger = logging.getLogger()
logger.setLevel("INFO")


def create_and_save_table(json_file, path_to_folder):
    """
    Функция создает таблицу
    :data_file: данные для создания
    :return: None
    """
    with open(json_file, "r") as f:
        data = json.load(f)
    data_list = []
    count = []
    keys = []
    for key, values in data.items():
        for i in values:
            if not i[0] in count:
                count.append(i[0])
    sorted_data = dict(sorted(data.items(), key=lambda x: int(x[0])))
    for key, values in sorted_data.items():
        averages = []
        for i in values:
            avg = np.mean(i[1])
            averages.append(avg)
        data_list.append(averages)
        keys.append(key)
    df = pd.DataFrame(data_list, columns=count, index=keys)
    df.columns.name = 'Treads / Size'
    dfi.export(df, os.path.join(path_to_folder, "table.png"))
    logging.info("Table saved")
