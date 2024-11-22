# -*- coding: utf-8 -*-
"""
Created on Fri Nov 22 20:16:29 2024

@author: Caterina
"""

import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv('month_results.csv')
df['Datetime'] = pd.to_datetime(df['Year'].apply(str) + ' ' + df['Month'].apply(str))
df = df.set_index(pd.DatetimeIndex(df['Datetime']))

fig, axs = plt.subplots(3,3, figsize=(55, 30))
x = df.index
axs[0][0].plot(x, df['DPM_t_C_ha'], color ="red")
axs[0][0].set_ylabel("t C / ha",fontsize=20)
axs[0][0].set_title("Decomposable Plant Material",fontsize=24)
axs[0][0].tick_params(axis='x', labelsize=20)
axs[0][0].tick_params(axis='y', labelsize=20)

axs[0][1].plot(x, df['RPM_t_C_ha'], color ="red")
axs[0][1].set_ylabel("t C / ha",fontsize=20)
axs[0][1].set_title("Resistant Plant Material",fontsize=24)
axs[0][1].tick_params(axis='x', labelsize=20)
axs[0][1].tick_params(axis='y', labelsize=20)

axs[0][2].plot(x, df['BIO_t_C_ha'], color ="red")
axs[0][2].set_ylabel("t C / ha",fontsize=20)
axs[0][2].set_title("Microbial Biomass",fontsize=24)
axs[0][2].tick_params(axis='x', labelsize=20)
axs[0][2].tick_params(axis='y', labelsize=20)


axs[1][0].plot(x, df['HUM_t_C_ha'], color ="red")
axs[1][0].set_ylabel("t C / ha",fontsize=20)
axs[1][0].set_title("Humified Organic Matter",fontsize=24)
axs[1][0].tick_params(axis='x', labelsize=20)
axs[1][0].tick_params(axis='y', labelsize=20)


axs[1][1].plot(x, df['IOM_t_C_ha'], color ="red")
axs[1][1].set_ylabel("t C / ha",fontsize=20)
axs[1][1].set_title("Inert Organic Matter",fontsize=24)
axs[1][1].tick_params(axis='x', labelsize=20)
axs[1][1].tick_params(axis='y', labelsize=20)

axs[1][2].plot(x, df['SOC_t_C_ha'], color ="red")
axs[1][2].set_ylabel("t C / ha",fontsize=20)
axs[1][2].set_title("Soil Organic Carbon",fontsize=24)
axs[1][2].tick_params(axis='x', labelsize=20)
axs[1][2].tick_params(axis='y', labelsize=20)

axs[2][0].plot(x, df['deltaC'], color ="red")
axs[2][0].set_ylabel("t C / ha",fontsize=20)
axs[2][0].set_title("delta C",fontsize=24)
axs[2][0].tick_params(axis='x', labelsize=20)
axs[2][0].tick_params(axis='y', labelsize=20)

plt.tight_layout()
plt.show()
fig.savefig('month_results.png')


df = pd.read_csv('year_results.csv')
df['Datetime'] = pd.to_datetime(df['Year'].apply(str) + ' ' + df['Month'].apply(str))
df = df.set_index(pd.DatetimeIndex(df['Datetime']))

fig, axs = plt.subplots(3,3, figsize=(55, 30))
x = df.index
axs[0][0].plot(x, df['DPM_t_C_ha'], color ="red")
axs[0][0].set_ylabel("t C / ha",fontsize=20)
axs[0][0].set_title("Decomposable Plant Material",fontsize=24)
axs[0][0].tick_params(axis='x', labelsize=20)
axs[0][0].tick_params(axis='y', labelsize=20)

axs[0][1].plot(x, df['RPM_t_C_ha'], color ="red")
axs[0][1].set_ylabel("t C / ha",fontsize=20)
axs[0][1].set_title("Resistant Plant Material",fontsize=24)
axs[0][1].tick_params(axis='x', labelsize=20)
axs[0][1].tick_params(axis='y', labelsize=20)

axs[0][2].plot(x, df['BIO_t_C_ha'], color ="red")
axs[0][2].set_ylabel("t C / ha",fontsize=20)
axs[0][2].set_title("Microbial Biomass",fontsize=24)
axs[0][2].tick_params(axis='x', labelsize=20)
axs[0][2].tick_params(axis='y', labelsize=20)


axs[1][0].plot(x, df['HUM_t_C_ha'], color ="red")
axs[1][0].set_ylabel("t C / ha",fontsize=20)
axs[1][0].set_title("Humified Organic Matter",fontsize=24)
axs[1][0].tick_params(axis='x', labelsize=20)
axs[1][0].tick_params(axis='y', labelsize=20)


axs[1][1].plot(x, df['IOM_t_C_ha'], color ="red")
axs[1][1].set_ylabel("t C / ha",fontsize=20)
axs[1][1].set_title("Inert Organic Matter",fontsize=24)
axs[1][1].tick_params(axis='x', labelsize=20)
axs[1][1].tick_params(axis='y', labelsize=20)

axs[1][2].plot(x, df['SOC_t_C_ha'], color ="red")
axs[1][2].set_ylabel("t C / ha",fontsize=20)
axs[1][2].set_title("Soil Organic Carbon",fontsize=24)
axs[1][2].tick_params(axis='x', labelsize=20)
axs[1][2].tick_params(axis='y', labelsize=20)

axs[2][0].plot(x, df['deltaC'], color ="red")
axs[2][0].set_ylabel("t C / ha",fontsize=20)
axs[2][0].set_title("delta C",fontsize=24)
axs[2][0].tick_params(axis='x', labelsize=20)
axs[2][0].tick_params(axis='y', labelsize=20)

plt.tight_layout()
plt.show()
fig.savefig('year_results.png')