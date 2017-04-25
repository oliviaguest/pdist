from __future__ import print_function
import numpy as np
# import dist
# from dist import cdist
from dist import c_mean_dist
import pandas as pd

df = pd.read_csv('RI.csv')
print(df.head())
