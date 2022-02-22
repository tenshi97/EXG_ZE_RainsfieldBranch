# 这是一个示例 Python 脚本。

# 按 Shift+F10 执行或将其替换为您的代码。
# 按 双击 Shift 在所有地方搜索类、文件、工具窗口、操作和设置。


def print_hi(name):
    # 在下面的代码行中使用断点来调试脚本。
    print(f'Hi, {name}')  # 按 Ctrl+F8 切换断点。
import time;
import json;
import sqlite3;
db = sqlite3.connect('E:/csgo_db/exg_csgo.db')
map_data = open ("E:/csgo_db/map_name.json",'r',encoding='utf-8-sig',errors='ignore')
sql = 'UPDATE ZEMAPS SET CN_NAME = ? WHERE NAME = ?'
map_lang = json.load(map_data)
dbc=db.cursor();
map_load_info=[]
for ele in map_lang:
    map_load_info.clear()
    map_info = list(ele.values());
    map_load_info.append(map_info[2]);
    map_load_info.append(map_info[1]);
    dbc.execute(sql,map_load_info)
db.commit()
# 访问 https://www.jetbrains.com/help/pycharm/ 获取 PyCharm 帮助
