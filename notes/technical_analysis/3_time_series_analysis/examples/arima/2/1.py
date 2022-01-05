import pandas
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt
import numpy as np
import math
from statsmodels.tsa.stattools import acf, pacf
import statsmodels.tsa.stattools as ts
from statsmodels.tsa.arima.model import ARIMA


# In[5]:


variables = pandas.read_csv("data/MSFT-1Y-Hourly.csv")
Close =variables ["close"]


# In[3]:


lnClose=np.log(Close)
lnClose
plt.plot(lnClose)
plt.show()


# In[4]:


acf_1 =  acf(lnClose)[1:20]
test_df = pandas.DataFrame([acf_1]).T
test_df.columns = ['autocorreletion']
test_df.index += 1
test_df.plot(kind='bar')
plt.show()


# In[ ]:


#notice the Pacf and acf plots


# In[6]:


pacf_1 = pacf(lnClose)[1:20]
test_df = pandas.DataFrame([pacf_1]).T
test_df.columns = ['Pandas Partial Autocorrelation']
test_df.index += 1
test_df.plot(kind='bar')
plt.show


# In[ ]:



# In[7]:


lnClose_diff=lnClose-lnClose.shift()
diff=lnClose_diff.dropna()
acf_1_diff =  acf(diff)[1:20]
test_df = pandas.DataFrame([acf_1_diff]).T
test_df.columns = ['First Difference Autocorrelation']
test_df.index += 1
test_df.plot(kind='bar')
pacf_1_diff =  pacf(diff)[1:20]
plt.plot(pacf_1_diff)
plt.show()


# In[12]:


price_matrix=lnClose.to_numpy()
model = ARIMA(price_matrix, order=(0,1,0))
model_fit = model.fit()
model_fit.summary()


# In[13]:


predictions=model_fit.predict(122, 127, typ='levels')
print(predictions)
predictionsadjusted=np.exp(predictions)
print(predictionsadjusted)