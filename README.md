A fast gradient boosting framework on manifolds(from regression tree,classification tree,neural net....).

##### 1) Faster than LightGBM with higher accuracy

For example , in the latest Kaggle competition  [IEEE-CIS Fraud Detection competition](https://www.kaggle.com/c/ieee-fraud-detection/overview) (binary classification problem) :

1） **LiteMORT is much faster than LightGBM**. LiteMORT needs only a quarter of the time of LightGBM.

2）**LiteMORT has higher auc than LightGBM**. 

![auc_8_fold](https://github.com/closest-git/ieee_fraud/raw/master/auc_8_fold.jpg)

![time_8_fold](https://github.com/closest-git/ieee_fraud/raw/master/time_8_fold.jpg)

For the detail comparison of this competition, please see https://github.com/closest-git/ieee_fraud.

##### 2)Use much less memory than LightGBM for large data problems 

Share memory with pandas dataframe or numpy ndarray.

##### 3)sklearn-like api interface.

```python
from litemort import *
mode = LiteMORT(params).fit(train_x, train_y, eval_set=[(eval_x, eval_y)])
pred_val = model.predict(eval_x)
pred_raw = model.predict_raw(eval_x)
```

##### 4)Just one line to transform from lightGBM to LiteMORT.

Support parameters of LightGBM

As shown below, just one more line to transform from lightGBM to LiteMORT.  

```python
if model_type == 'mort':
    model = LiteMORT(params).fit_1(X_train, y_train, eval_set=[(X_valid, y_valid)])
if model_type == 'lgb':
    model = lgb.LGBMRegressor(**params, n_jobs=-1)
    model.fit(X_train, y_train, eval_set=[(X_train, y_train), (X_valid, y_valid)])
pred_test = model.predict(X_test)
```



|      |      |      |      |
| ---- | ---- | ---- | ---- |
|      |      |      |      |

