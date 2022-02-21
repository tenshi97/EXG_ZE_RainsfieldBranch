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
map_names = open ("mapcycle.txt",'r',encoding='utf-8-sig',errors='ignore')
map_config = open("mapconfig.json", 'w', encoding='utf-8', errors='ignore')
map_name_translation = open("map_name.json",'w',encoding='utf-8',errors='ignore');
id = 0;
map_L=[];
map_T=[];
# 按间距中的绿色按钮以运行脚本。
for lines in map_names:
    id+=1;
    map_property = {'id':id,'original':lines.rstrip(),'translation':'','cd':720,'last_run_time':int(time.time()),'available':1,'cost':0,'heat':0,'difficulty':0,'tag':0,'round':0,'wins':0,'translated':0}
    map_translation = {'key':id,'original':lines.rstrip()}
    map_L.append(map_property);
    map_T.append(map_translation);
map_L_data = json.dumps(map_L,indent=4,ensure_ascii=False);
map_T_data = json.dumps(map_T,indent=4,ensure_ascii=False)
map_config.write(map_L_data);
map_name_translation.write(map_T_data);
dbc = db.cursor();

# 访问 https://www.jetbrains.com/help/pycharm/ 获取 PyCharm 帮助
