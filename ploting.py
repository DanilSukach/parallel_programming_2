import os
import logging
import numpy as np
import matplotlib.pyplot as plt
import json

logger = logging.getLogger()
logger.setLevel("INFO")


def visualize_statistics(data_file: str, path_to_file: str) -> None:
    """
    Функция создает графики по данным
    :data_file: данные для построения
    :return: None
    """
    with open(data_file) as f:
        data = json.load(f)
    plt.figure(figsize=(15, 15))
    sorted_data = dict(sorted(data.items(), key=lambda x: int(x[0])))
    for key, values in sorted_data.items():
        count = []
        averages = []
        for i in values:
            count.append(i[0])
        for i in values:
            avg = np.mean(i[1])
            averages.append(avg)
        plt.plot(count, averages, label=f'Thread {key}', marker="o",
                 linestyle="-")
    plt.title("Size versus time graph")
    plt.xlabel("Size")
    plt.ylabel("Time, s")
    plt.grid(True)
    plt.legend()
    plt.savefig(os.path.join(path_to_file, "statistics.png"))

    for key, values in data.items():
        count = []
        averages = []
        for i in values:
            count.append(i[0])
        for i in values:
            avg = np.mean(i[1])
            averages.append(avg)
        plt.figure(figsize=(12, 12))
        plt.plot(count, averages, marker="o",
                 linestyle="-", color="green")
        plt.title(f"Size versus time graph for {key}")
        plt.xlabel("Size")
        plt.ylabel("Time, s")
        plt.grid(True)
        plt.savefig(os.path.join(path_to_file, f"{key}.png"))
        plt.close()
    logging.info("Schedule saved")
